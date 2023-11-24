// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMoveComp.h"
#include "TPSPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

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

}

void UPlayerMoveComp::SetupPlayerInput(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInput(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Look Up / Down Mouse"), this, &UPlayerMoveComp::AxisLookUp);
	PlayerInputComponent->BindAxis(TEXT("Move Forward / Backward"), this, &UPlayerMoveComp::AxisVertical);
	PlayerInputComponent->BindAxis(TEXT("Move Right / Left"), this, &UPlayerMoveComp::AxisHorizontal);
	PlayerInputComponent->BindAxis(TEXT("Turn Right / Left Mouse"), this, &UPlayerMoveComp::AxisTurn);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &UPlayerMoveComp::ActionJump);

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

