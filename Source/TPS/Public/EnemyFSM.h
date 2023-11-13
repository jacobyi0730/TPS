// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle	UMETA(DisplayName = "IDLE"),
	Move	UMETA(DisplayName = "MOVE"),
	Attack	UMETA(DisplayName = "ATTACK"),
	Damage	UMETA(DisplayName = "DAMAGE"),
	Die		UMETA(DisplayName = "DIE"),
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPS_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EEnemyState State;

	UPROPERTY()
	class AEnemy* Me;

	UPROPERTY()
	class ATPSPlayer* Target;

	// 공격 가능 거리
	float AttackDistance = 200;

	void TickIdle();
	void TickMove();
	void TickAttack();
	void TickDamage();
	void TickDie();

public:
	// 데미지를 입었으면 체력을 1 감소하고싶다.
	// 체력이 0보다 크면 데미지 상태로 전이하고싶다.
	// 그렇지 않고 체력이 0 이하라면 죽음 상태로 전이하고싶다. 
	int32 HP = 2;
	void OnTakeDamage(int32 damage);

	float CurrentTime;
	float DamageTime = 2;
	float DieTime = 2;

	void SetState(EEnemyState next);
};