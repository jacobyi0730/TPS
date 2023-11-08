// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

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
}

