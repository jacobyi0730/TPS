// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Bullet.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"
#include "EnemyFSM.h"

// Sets default values
ATPSPlayer::ATPSPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	// 스프링암을 생성해서 루트에 붙이고
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	// 스프링암의 TargetArmLength값을 300으로 하고싶다.
	SpringArmComp->TargetArmLength = 300;
	// 위치도...(X = 0.000000, Y = 50.000000, Z = 80.000000)
	SpringArmComp->SetRelativeLocation(FVector(0, 50, 80));
	// 카메라를 생성해서 스프링암에 붙이고싶다.
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	// 메시에 스켈레탈메시를 로드해서 넣어주고싶다.
	// 생성자 도우미를 이용해서 스켈레탈메시를 로드하고싶다.
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));

	// 만약 로드가 성공했다면
	if ( tempMesh.Succeeded() )
	{
		// 메시에 넣고싶다.
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	bUseControllerRotationYaw = true;
	SpringArmComp->bUsePawnControlRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	JumpMaxCount = 2;
	GetCharacterMovement()->AirControl = 1;


	// 유탄총을 생성하고 에셋도 적용하고 배치하고싶다.
	GrenadeGun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GrenadeGun"));
	// 유탄총을 Mesh에 어태치 하고싶다.
	GrenadeGun->SetupAttachment(GetMesh());
	GrenadeGun->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempGrenadeGun(TEXT("/Script/Engine.SkeletalMesh'/Game/Models/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));

	if ( tempGrenadeGun.Succeeded() )
	{
		GrenadeGun->SetSkeletalMesh(tempGrenadeGun.Object);
		GrenadeGun->SetRelativeLocation(FVector(0, 60, 130));
	}

	// 스나이퍼건을 생성해서 Mesh에 붙이고 싶다.
	SniperGun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperGun"));
	SniperGun->SetupAttachment(GetMesh());
	SniperGun->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// 에셋도 로드해서 적용하고싶다.
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempSniperGun(TEXT("/Script/Engine.StaticMesh'/Game/Models/SniperGun/sniper1.sniper1'"));

	if ( tempSniperGun.Succeeded() )
	{
		SniperGun->SetStaticMesh(tempSniperGun.Object);
		SniperGun->SetRelativeLocation(FVector(0, 80, 130));
		SniperGun->SetRelativeScale3D(FVector(0.15f));
	}

}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	//2. 태어날 때 두 개의 위젯을 생성하고싶다.
	CrosshairUI = CreateWidget(GetWorld(), CrosshairUIFactory);
	SniperUI = CreateWidget(GetWorld(), SniperUIFactory);

	CrosshairUI->AddToViewport();
	SniperUI->AddToViewport();

	// 유탄총을 쥐고 시작하고싶다.
	ActionChooseGrenadeGun();
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Direction을 정규화 하고싶다.
	Direction.Normalize();
	// Direction을 컨트롤러를 기준으로 방향을 재정렬하고싶다.
	FTransform controllerTransform = FTransform(GetControlRotation());
	FVector dir = controllerTransform.TransformVector(Direction);
	// Direction방향으로 이동하고싶다.
	AddMovementInput(dir);
}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// 입력 함수들을 모두 연결

	PlayerInputComponent->BindAxis(TEXT("Look Up / Down Mouse"), this, &ATPSPlayer::AxisLookUp);
	PlayerInputComponent->BindAxis(TEXT("Move Forward / Backward"), this, &ATPSPlayer::AxisVertical);
	PlayerInputComponent->BindAxis(TEXT("Move Right / Left"), this, &ATPSPlayer::AxisHorizontal);
	PlayerInputComponent->BindAxis(TEXT("Turn Right / Left Mouse"), this, &ATPSPlayer::AxisTurn);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ATPSPlayer::ActionJump);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATPSPlayer::ActionFire);

	PlayerInputComponent->BindAction(TEXT("ChooseGrenadeGun"), IE_Pressed, this, &ATPSPlayer::ActionChooseGrenadeGun);

	PlayerInputComponent->BindAction(TEXT("ChooseSniperGun"), IE_Pressed, this, &ATPSPlayer::ActionChooseSniperGun);

	PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Pressed, this, &ATPSPlayer::ActionZoomIn);

	PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Released, this, &ATPSPlayer::ActionZoomOut);
}

void ATPSPlayer::AxisHorizontal(float value)
{
	Direction.Y = value;
}

void ATPSPlayer::AxisVertical(float value)
{
	Direction.X = value;
}

void ATPSPlayer::AxisLookUp(float value)
{
	AddControllerPitchInput(value);
}

void ATPSPlayer::AxisTurn(float value)
{
	AddControllerYawInput(value);
}

void ATPSPlayer::ActionJump()
{
	Jump();
}

void ATPSPlayer::ActionFire()
{
	if ( bChooseGrenadeGun )
	{
		GrenadeFire();
	}
	else
	{
		SniperFire();
	}
}

void ATPSPlayer::ActionChooseGrenadeGun()
{
	bChooseGrenadeGun = true;
	GrenadeGun->SetVisibility(true);
	SniperGun->SetVisibility(false);
	CrosshairUI->SetVisibility(ESlateVisibility::Hidden);
	SniperUI->SetVisibility(ESlateVisibility::Hidden);
	CameraComp->FieldOfView = 90;
}

void ATPSPlayer::ActionChooseSniperGun()
{
	bChooseGrenadeGun = false;
	GrenadeGun->SetVisibility(false);
	SniperGun->SetVisibility(true);
	CrosshairUI->SetVisibility(ESlateVisibility::Visible);
	SniperUI->SetVisibility(ESlateVisibility::Hidden);
}

void ATPSPlayer::ActionZoomIn()
{
	if ( bChooseGrenadeGun )
	{
		return;
	}
	CameraComp->FieldOfView = 45;
	CrosshairUI->SetVisibility(ESlateVisibility::Hidden);
	SniperUI->SetVisibility(ESlateVisibility::Visible);
}

void ATPSPlayer::ActionZoomOut()
{
	if ( bChooseGrenadeGun )
	{
		return;
	}
	CrosshairUI->SetVisibility(ESlateVisibility::Visible);
	SniperUI->SetVisibility(ESlateVisibility::Hidden);
	CameraComp->FieldOfView = 90;
}

void ATPSPlayer::GrenadeFire()
{
	// 총알을 생성해서 유탄총의 총구 소켓 위치에 배치하고싶다.
	FTransform firePosition = GrenadeGun->GetSocketTransform(TEXT("FirePosition"));
	GetWorld()->SpawnActor<ABullet>(BulletFactory, firePosition);
}

void ATPSPlayer::SniperFire()
{
	// 카메라의 위치에서 카메라의 앞방향 1km로 바라보고싶다.
	FHitResult hitResult;
	FVector start = CameraComp->GetComponentLocation();
	FVector end = start + CameraComp->GetForwardVector() * 100000.f;
	FCollisionQueryParams params;
	// 바라보고
	if ( GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_Visibility) )
	{
		// 어딘가 부딪혔다.
		// 그곳에 폭발 이펙트를 생성하고싶다.
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionVFXFactory, FTransform(hitResult.ImpactPoint));

		// 만약 부딪힌 물체가 물리작용을 할 수 있다면
		auto hitComp = hitResult.GetComponent();
		if ( hitComp && hitComp->IsSimulatingPhysics() )
		{
			// 그 물체에게 힘을 가하고싶다.
			hitComp->AddForce(-hitResult.ImpactNormal * hitComp->GetMass() * 500000.f);
		}

		// 그 물체가 Enemy라면
		AEnemy* enemy = Cast<AEnemy>(hitResult.GetActor());
		if (enemy)
		{
			// 너 맞았어 라고 알려주고싶다.
			//enemy->EnemyFSM
			auto fsm = enemy->GetDefaultSubobjectByName(TEXT("EnemyFSM"));
			UEnemyFSM* enemyFSM = Cast<UEnemyFSM>(fsm);
			enemyFSM->OnTakeDamage(1);
		}
	}
	else
	{
		// 부딪힌 곳이 없다... => 허공
	}
}

