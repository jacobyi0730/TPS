// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHpUI.generated.h"

/**
 * 
 */
UCLASS()
class TPS_API UPlayerHpUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(EditInstanceOnly, meta = (BindWidget))
	class UProgressBar* HPBarGuide;

	UPROPERTY(EditInstanceOnly, meta = (BindWidget))
	class UProgressBar* HPBar;

	UPROPERTY(EditInstanceOnly, meta = (BindWidget))
	class UTextBlock* TxtHPRate;

	float targetPercent = 1;

	void UpdateHP(int HP, int MaxHP);
};
