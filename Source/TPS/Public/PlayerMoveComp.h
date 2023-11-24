// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComp.h"
#include "PlayerMoveComp.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(TPSPlayer), meta=(BlueprintSpawnableComponent) )
class TPS_API UPlayerMoveComp : public UPlayerBaseComp
{
	GENERATED_BODY()

public:

	UPlayerMoveComp();

	virtual void BeginPlay() override;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void SetupPlayerInput(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TPS)
	FVector Direction;

	// 입력에 대응하는 함수 제작
	void AxisHorizontal(float value);
	void AxisVertical(float value);
	void AxisLookUp(float value);
	void AxisTurn(float value);
	void ActionJump();

	void ActionWalk();
	void ActionRun();


};
