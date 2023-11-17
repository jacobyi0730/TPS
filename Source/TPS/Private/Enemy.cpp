// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyFSM.h"
#include "EnemyAnim.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	//"/Script/Engine.SkeletalMesh'/Game/Models/Enemy/Model/vampire_a_lusth.vampire_a_lusth'"
	// 외관을 바꾸고싶다.
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/Models/Enemy/Model/vampire_a_lusth.vampire_a_lusth'"));

	// 만약 로드를 성공했다면 외관을 바꾸고
	if ( tempMesh.Succeeded() )
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		// 위치, 회전, 크기를 적용하고싶다.
		GetMesh()->SetRelativeLocationAndRotation(
			FVector(0, 0, -90),
			FRotator(0, -90, 0)
		);
		GetMesh()->SetRelativeScale3D(FVector(0.8f));
	}


	EnemyFSM = CreateDefaultSubobject<UEnemyFSM>(TEXT("EnemyFSM"));

	ConstructorHelpers::FClassFinder<UEnemyAnim> tempAnim(TEXT("/Script/Engine.AnimBlueprint'/Game/Blueprints/ABP_Enemy.ABP_Enemy_C'"));

	if ( tempAnim.Succeeded() )
	{
		GetMesh()->SetAnimClass(tempAnim.Class);
	}
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

