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
	int32 HP;
	int32 MaxHP = 3;
	void OnTakeDamage(int32 damage);

	float CurrentTime;

	UPROPERTY(EditAnywhere)
	float DamageTime = 2;

	UPROPERTY(EditAnywhere)
	float DieTime = 2;

	UPROPERTY(EditAnywhere)
	float AttackTime = 2;

	void SetState(EEnemyState next);

	// 죽었을 때 내려갈 목적지를 정하고 싶다.
	FVector DieEndLoc;

	// 나의 상태가 바뀔 때 애니메이션의 상태도 바꾸고싶다.
	// 생성자에서 직접 ABP_Enemy를 로드하고싶다.
	UPROPERTY()
	class UEnemyAnim* EnemyAnim;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* EnemyActionMontage;
	
	void PlayMontageDamage();
	void PlayMontageDie();

	void OnChangeMoveState();

	// AIController를 이용해서 길찾기를 하고싶다.
	UPROPERTY()
	class AAIController* ai;

	FVector RandomLocation;

	bool UpdateRandomLocation(FVector origin, float radius, FVector& outLocation);

	void UpdateHP(int32 newHp);

	void InitHP();
};