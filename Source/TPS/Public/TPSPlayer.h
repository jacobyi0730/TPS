// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"

UCLASS()
class TPS_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// 컴포넌트 스프링암, 카메라, 유탄총
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* CameraComp;
	
	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent*	GrenadeGun;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent*		SniperGun;

public:

	UPROPERTY(EditDefaultsOnly)
	class UPlayerBaseComp* MoveComp;

	UPROPERTY(EditDefaultsOnly)
	class UPlayerBaseComp* FireComp;
};
