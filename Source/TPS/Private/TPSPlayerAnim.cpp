// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerAnim.h"
#include "TPSPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

void UTPSPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	// 오너를 가져와서 유효성 체크를 하고싶다.
	if ( nullptr == player )
	{
		auto owner = TryGetPawnOwner();
		// 오너를 ATPSPlayer로 캐스팅하고
		player = Cast<ATPSPlayer>(owner);
	}
	if ( nullptr == player )
	{
		return;
	}
	// Actor Forward Vector
	FVector forward = player->GetActorForwardVector();
	// Actor Right Vector
	FVector right = player->GetActorRightVector();
	// Velocity
	FVector velocity = player->GetVelocity();
	// Speed의 값을 채우고 싶다.
	Speed = FVector::DotProduct(forward, velocity);
	// Direction의 값을 채우고 싶다.
	Direction = FVector::DotProduct(right, velocity);
	// 캐릭터 무브먼트에서 IsFalling을 가져와서 IsFalling을 채우고싶다.
	UCharacterMovementComponent* movement = player->GetCharacterMovement();
	IsFalling = movement->IsFalling();
}
