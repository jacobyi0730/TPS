// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyFSM.h"
#include "EnemyAnim.generated.h"

UCLASS()
class TPS_API UEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEnemyState State;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsDieDone;

	UFUNCTION(BlueprintImplementableEvent)
	void PlayMontageDamage(FName sectionName);

	UFUNCTION(BlueprintCallable)
	void OnDamageEnd();

	UFUNCTION()
	void AnimNotify_DamageEnd();

	UFUNCTION()
	void AnimNotify_DieEnd();

	UFUNCTION()
	void AnimNotify_OnAttackHit();
};
