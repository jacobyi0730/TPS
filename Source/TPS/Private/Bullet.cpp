// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;
	// �浹ü�� ��Ʈ���ϰ� �������� 12.5f��
	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("collisionComp"));
	SetRootComponent(collisionComp);
	collisionComp->SetSphereRadius(12.5f);
	collisionComp->SetCollisionProfileName(TEXT("BlockAll"));

	// �ܰ��� �浹ü�� ���̰� ũ�� 0.25f
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetRelativeScale3D(FVector(0.25f));
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// �̵�������Ʈ �����. �ӷ¼���, ƨ�輳��
	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("movementComp"));
	movementComp->InitialSpeed = 3000.f;
	movementComp->MaxSpeed = 3000.f;
	movementComp->bShouldBounce = true;
	movementComp->Bounciness = .3f;

	movementComp->SetUpdatedComponent(collisionComp);

}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

