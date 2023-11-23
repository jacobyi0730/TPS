// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnManager.generated.h"

UCLASS()
class TPS_API ASpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// 필요속성
	// - 스폰목록, 생성시간정보, 타이머핸들, 적공장
	// 필요기능
	// - 적생성기능
	UPROPERTY(EditAnywhere)
	TArray<class AEnemySpawn*> SpawnList;

	void MakeSpawnList(bool bUseIterator);

	float makeTimeMin = 1;
	float makeTimeMax = 3;
	FTimerHandle timerHandle;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AEnemy> EnemyFactory;

	void MakeEnemy();
};
