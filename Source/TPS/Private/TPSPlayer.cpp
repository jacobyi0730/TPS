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
#include "PlayerMoveComp.h"
#include "PlayerFireComp.h"
#include "PlayerHpUI.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>

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
	GrenadeGun->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	GrenadeGun->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempGrenadeGun(TEXT("/Script/Engine.SkeletalMesh'/Game/Models/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));

	if ( tempGrenadeGun.Succeeded() )
	{
		GrenadeGun->SetSkeletalMesh(tempGrenadeGun.Object);
		GrenadeGun->SetRelativeLocationAndRotation(
			FVector(-1.835274f, 2.205643f, -2.518352f),
			FRotator(9.489230f, 101.377560f, -7.563355f)
		);
	}

	// 스나이퍼건을 생성해서 Mesh에 붙이고 싶다.
	SniperGun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperGun"));
	SniperGun->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	SniperGun->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// 에셋도 로드해서 적용하고싶다.
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempSniperGun(TEXT("/Script/Engine.StaticMesh'/Game/Models/SniperGun/sniper1.sniper1'"));

	if ( tempSniperGun.Succeeded() )
	{
		SniperGun->SetStaticMesh(tempSniperGun.Object);
		SniperGun->SetRelativeLocationAndRotation(
			FVector(-38.369813f, -6.961525f, 6.490548f),
			FRotator(10.131268f, 101.024829f, -7.322219f)
		);
		SniperGun->SetRelativeScale3D(FVector(0.15f));
	}

	MoveComp = CreateDefaultSubobject<UPlayerMoveComp>(TEXT("MoveComp"));

	FireComp = CreateDefaultSubobject<UPlayerFireComp>(TEXT("FireComp"));

}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();
	// 태어날 때 HP를 MaxHP로 하고 HP UI를 생성해서 보이게 하고싶다.

	HP = MaxHP;
	auto ui = CreateWidget<UPlayerHpUI>(GetWorld(), HpUIFactory);

	ui->AddToViewport(1);

	PlayerHP = Cast<UPlayerHpUI>(ui);

}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UE_LOG(LogTemp, Warning, TEXT("ATPSPlayer::SetupPlayerInputComponent"));

	Super::SetupPlayerInputComponent(PlayerInputComponent);



	OnSetupInputDelegate.Broadcast(PlayerInputComponent);
}

void ATPSPlayer::OnMyHit()
{
	// 체력을 1 감소하고 UI도 갱신하고싶다.
	HP -= 10;
	PlayerHP->UpdateHP(HP, MaxHP);
	if ( HP <= 0 )
	{
		// 게임 오버
		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}
}

