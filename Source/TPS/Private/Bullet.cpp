// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;
	// 충돌체를 루트로하고 반지름을 12.5f로
	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("collisionComp"));
	SetRootComponent(collisionComp);
	collisionComp->SetSphereRadius(12.5f);
	collisionComp->SetCollisionProfileName(TEXT("BlockAll"));

	// 외관을 충돌체에 붙이고 크기 0.25f
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetRelativeScale3D(FVector(0.25f));
	meshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 이동컴포넌트 만들고. 속력설정, 튕김설정
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

	FTimerHandle handle;
	
	//GetWorld()->GetTimerManager().SetTimer(handle, this, &ABullet::KillMySelf, 1);
	GetWorld()->GetTimerManager().SetTimer(handle,
		FTimerDelegate::CreateLambda(
			[this]()->void
			{
				// TO DO :
				this->Destroy();
			}),
		1, false);

	//int sum = 0;
	////[캡처](매개변수)->반환자료형{ 구현 }
	//auto lambdaAddFunc = [&sum](int32 a, int32 b)->void
	//{
	//	sum = a + b; 
	//};

	//lambdaAddFunc(10, 20);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//void ABullet::KillMySelf()
//{
//	Destroy();
//}

