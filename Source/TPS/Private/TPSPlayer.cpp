// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/SkeletalMesh.h"
#include "Bullet.h"
#include "Blueprint/UserWidget.h"

// Sets default values
ATPSPlayer::ATPSPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	// ���������� �����ؼ� ��Ʈ�� ���̰�
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	// ���������� TargetArmLength���� 300���� �ϰ�ʹ�.
	SpringArmComp->TargetArmLength = 300;
	// ��ġ��...(X = 0.000000, Y = 50.000000, Z = 80.000000)
	SpringArmComp->SetRelativeLocation(FVector(0, 50, 80));
	// ī�޶� �����ؼ� �������Ͽ� ���̰�ʹ�.
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	// �޽ÿ� ���̷�Ż�޽ø� �ε��ؼ� �־��ְ�ʹ�.
	// ������ ����̸� �̿��ؼ� ���̷�Ż�޽ø� �ε��ϰ�ʹ�.
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));

	// ���� �ε尡 �����ߴٸ�
	if (tempMesh.Succeeded())
	{
		// �޽ÿ� �ְ�ʹ�.
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	bUseControllerRotationYaw = true;
	SpringArmComp->bUsePawnControlRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	JumpMaxCount = 2;
	GetCharacterMovement()->AirControl = 1;


	// ��ź���� �����ϰ� ���µ� �����ϰ� ��ġ�ϰ�ʹ�.
	GrenadeGun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GrenadeGun"));
	// ��ź���� Mesh�� ����ġ �ϰ�ʹ�.
	GrenadeGun->SetupAttachment(GetMesh());
	GrenadeGun->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempGrenadeGun(TEXT("/Script/Engine.SkeletalMesh'/Game/Models/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));

	if (tempGrenadeGun.Succeeded())
	{
		GrenadeGun->SetSkeletalMesh(tempGrenadeGun.Object);
		GrenadeGun->SetRelativeLocation(FVector(0, 60, 130));
	}

	// �������۰��� �����ؼ� Mesh�� ���̰� �ʹ�.
	SniperGun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperGun"));
	SniperGun->SetupAttachment(GetMesh());
	SniperGun->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// ���µ� �ε��ؼ� �����ϰ�ʹ�.
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempSniperGun(TEXT("/Script/Engine.StaticMesh'/Game/Models/SniperGun/sniper1.sniper1'"));

	if (tempSniperGun.Succeeded())
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

	//2. �¾ �� �� ���� ������ �����ϰ�ʹ�.
	CrosshairUI = CreateWidget(GetWorld(), CrosshairUIFactory);
	SniperUI = CreateWidget(GetWorld(), SniperUIFactory);
	
	// ��ź���� ��� �����ϰ�ʹ�.
	ActionChooseGrenadeGun();
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Direction�� ����ȭ �ϰ�ʹ�.
	Direction.Normalize();
	// Direction�� ��Ʈ�ѷ��� �������� ������ �������ϰ�ʹ�.
	FTransform controllerTransform = FTransform(GetControlRotation());
	FVector dir = controllerTransform.TransformVector(Direction);
	// Direction�������� �̵��ϰ�ʹ�.
	AddMovementInput(dir);
}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// �Է� �Լ����� ��� ����

	PlayerInputComponent->BindAxis(TEXT("Look Up / Down Mouse"), this, &ATPSPlayer::AxisLookUp);
	PlayerInputComponent->BindAxis(TEXT("Move Forward / Backward"), this, &ATPSPlayer::AxisVertical);
	PlayerInputComponent->BindAxis(TEXT("Move Right / Left"), this, &ATPSPlayer::AxisHorizontal);
	PlayerInputComponent->BindAxis(TEXT("Turn Right / Left Mouse"), this, &ATPSPlayer::AxisTurn);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ATPSPlayer::ActionJump);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATPSPlayer::ActionFire);

	PlayerInputComponent->BindAction(TEXT("ChooseGrenadeGun"), IE_Pressed, this, &ATPSPlayer::ActionChooseGrenadeGun);

	PlayerInputComponent->BindAction(TEXT("ChooseSniperGun"), IE_Pressed, this, &ATPSPlayer::ActionChooseSniperGun);
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
	if (bChooseGrenadeGun)
	{
		// �Ѿ��� �����ؼ� ��ź���� �ѱ� ���� ��ġ�� ��ġ�ϰ�ʹ�.
		FTransform firePosition = GrenadeGun->GetSocketTransform(TEXT("FirePosition"));
		GetWorld()->SpawnActor<ABullet>(BulletFactory, firePosition);
	}
	else
	{
		// �ٶ󺸰�
		// �ε������� Ÿ���� ������ʹ�.
	}
}

void ATPSPlayer::ActionChooseGrenadeGun()
{
	bChooseGrenadeGun = true;
	GrenadeGun->SetVisibility(true);
	SniperGun->SetVisibility(false);
	CrosshairUI->AddToViewport();
	SniperUI->RemoveFromParent();

	CameraComp->FieldOfView = 90;
}

void ATPSPlayer::ActionChooseSniperGun()
{
	bChooseGrenadeGun = false;
	GrenadeGun->SetVisibility(false);
	SniperGun->SetVisibility(true);
	SniperUI->AddToViewport();
	CrosshairUI->RemoveFromParent();

	CameraComp->FieldOfView = 45;
}

