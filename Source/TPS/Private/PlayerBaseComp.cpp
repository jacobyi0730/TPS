// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBaseComp.h"
#include "TPSPlayer.h"

// Sets default values for this component's properties
UPlayerBaseComp::UPlayerBaseComp()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerBaseComp::BeginPlay()
{
	Super::BeginPlay();
	// 오너를 가져와서 Me에 담고싶다.
	Me = Cast<ATPSPlayer>(GetOwner());
}


// Called every frame
void UPlayerBaseComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerBaseComp::SetupPlayerInput(UInputComponent* PlayerInputComponent)
{
}

