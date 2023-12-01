// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMoveComp.h"
#include "TPSPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h>
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>

UPlayerMoveComp::UPlayerMoveComp()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerMoveComp::BeginPlay()
{
	Super::BeginPlay();

	ActionWalk();
}

void UPlayerMoveComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Direction을 정규화 하고싶다.
	Direction.Normalize();
	// Direction을 컨트롤러를 기준으로 방향을 재정렬하고싶다.
	FTransform controllerTransform = FTransform(Me->GetControlRotation());
	FVector dir = controllerTransform.TransformVector(Direction);
	// Direction방향으로 이동하고싶다.
	Me->AddMovementInput(dir);
	// 이동처리 후 방향을 초기화 하고싶다.
	Direction = FVector::ZeroVector;
}

void UPlayerMoveComp::SetupPlayerInput(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInput(PlayerInputComponent);

	UEnhancedInputComponent* input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &UPlayerMoveComp::IA_AxisMove);

	input->BindAction(IA_Jump, ETriggerEvent::Started, this, &UPlayerMoveComp::IA_ActionJump);


	//PlayerInputComponent->BindAxis(TEXT("Move Forward / Backward"), this, &UPlayerMoveComp::AxisVertical);
	//PlayerInputComponent->BindAxis(TEXT("Move Right / Left"), this, &UPlayerMoveComp::AxisHorizontal);
	//PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &UPlayerMoveComp::ActionJump);


	PlayerInputComponent->BindAxis(TEXT("Look Up / Down Mouse"), this, &UPlayerMoveComp::AxisLookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn Right / Left Mouse"), this, &UPlayerMoveComp::AxisTurn);


	PlayerInputComponent->BindAction(TEXT("Run"), IE_Pressed, this, &UPlayerMoveComp::ActionRun);

	PlayerInputComponent->BindAction(TEXT("Run"), IE_Released, this, &UPlayerMoveComp::ActionWalk);


}

void UPlayerMoveComp::AxisHorizontal(float value)
{
	Direction.Y = value;
}

void UPlayerMoveComp::AxisVertical(float value)
{
	Direction.X = value;
}

void UPlayerMoveComp::AxisLookUp(float value)
{
	Me->AddControllerPitchInput(value);
}

void UPlayerMoveComp::AxisTurn(float value)
{
	Me->AddControllerYawInput(value);
}

void UPlayerMoveComp::ActionJump()
{
	Me->Jump();
}

void UPlayerMoveComp::ActionWalk()
{
	Me->GetCharacterMovement()->MaxWalkSpeed = 300;
}

void UPlayerMoveComp::ActionRun()
{
	Me->GetCharacterMovement()->MaxWalkSpeed = 600;
}

void UPlayerMoveComp::IA_AxisMove(const FInputActionValue& value)
{
	FVector2D v = value.Get<FVector2D>();
	Direction.X = v.X;
	Direction.Y = v.Y;
}
void UPlayerMoveComp::IA_ActionJump()
{
	Me->Jump();
}

