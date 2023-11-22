// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyFSM.h"
#include "EnemyAnim.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/WidgetComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/CapsuleComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Camera/PlayerCameraManager.h>



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

	HpComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpComp"));
	HpComp->SetupAttachment(RootComponent);
	// WBP_EnemyHP를 로드해서 HpComp의 위젯으로 설정하고싶다.
	ConstructorHelpers::FClassFinder<UUserWidget> tempHP(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/WBP_EnemyHP.WBP_EnemyHP_C'"));

	if ( tempHP.Succeeded())
	{
		HpComp->SetWidgetClass(tempHP.Class);
		HpComp->SetDrawSize(FVector2D(150, 20));
		HpComp->SetRelativeLocation(FVector(0, 0, 90));
		HpComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// 캡슐과 메시의 충돌채널을 설정하고싶다.
	// 캡슐 vis : block / cam : ig
	// 메시 vis : ig / cam : ig

	UCapsuleComponent* cap = GetCapsuleComponent();
	auto mesh = GetMesh();
	cap->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	cap->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	mesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	mesh->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
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

	// UKismetMathLibrary::FindLookAtRotation를 이용해서 HPComp을 회전하시오.
	FVector start = HpComp->GetComponentLocation();
	FVector target =
		UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation();

	FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(start, target);

	HpComp->SetWorldRotation(newRotation);
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
