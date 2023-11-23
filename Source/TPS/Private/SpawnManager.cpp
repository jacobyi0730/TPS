// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnManager.h"
#include "EnemySpawn.h"
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "TPS.h"

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
	if ( bUseIterator )
	{
		for ( TActorIterator<AEnemySpawn> It(GetWorld()); It; ++It )
		{
			SpawnList.Add(*It);
			MY_LOG(TEXT("%s"), *It->GetName());
		}
	}
	else
	{
		TArray<AActor*> temp;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemySpawn::StaticClass(), temp);

		for(AActor* var : temp)
		{
			SpawnList.Add(Cast<AEnemySpawn>(var));
		}
	}
}

void ASpawnManager::MakeEnemy()
{

}

