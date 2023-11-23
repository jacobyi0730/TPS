// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnManager.h"
#include "EnemySpawn.h"
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "TPS.h"
#include "Enemy.h"

// Sets default values
ASpawnManager::ASpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawnManager::BeginPlay()
{
	Super::BeginPlay();
	// SpawnList를 채우고싶다.
	MakeSpawnList(true);

	// 일정시간마다 적을 생성하고싶다.
	float randTime = FMath::RandRange(makeTimeMin, makeTimeMax);
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ASpawnManager::MakeEnemy, randTime, false);
	// 적의 위치는 SpawnList에서 랜덤하게 정하고싶다.
	// 생성시간도 랜덤으로 정하고싶다.
}

// Called every frame
void ASpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnManager::MakeSpawnList(bool bUseIterator)
{
	SpawnList.Empty();
	if ( bUseIterator )
	{
		for ( TActorIterator<AEnemySpawn> It(GetWorld()); It; ++It )
		{
			SpawnList.Add(*It);
			MY_LOG(TEXT("%s"), *It->GetActorNameOrLabel());
		}
	}
	else
	{
		TArray<AActor*> temp;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemySpawn::StaticClass(), temp);

		for(AActor* var : temp)
		{
			AEnemySpawn* es = Cast<AEnemySpawn>(var);
			SpawnList.Add(es);
			MY_LOG(TEXT("%s"), *es->GetActorNameOrLabel());
		}
	}
}

int prevIndex = -1;
void ASpawnManager::MakeEnemy()
{
	// 이전에 생성했던 위치는 이번에 생성목록에서 제외해주세요.

	int idx = FMath::RandRange(0, SpawnList.Num() - 1);
	
	if ( prevIndex == idx)
	{
		// idx를 다시 랜덤으로 결정하고 싶다.
		idx = (idx + 1) % SpawnList.Num();
		//if ( ++idx >= SpawnList.Num() )
		//	idx = 0;
	}
	
	FVector spawnLocation = SpawnList[idx]->GetActorLocation();

	GetWorld()->SpawnActor<AEnemy>(EnemyFactory, spawnLocation, FRotator::ZeroRotator);

	float randTime = FMath::RandRange(makeTimeMin, makeTimeMax);
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ASpawnManager::MakeEnemy, randTime, false);
}

