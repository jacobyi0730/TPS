// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHpUI.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "TPS.h"

void UPlayerHpUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	// 체력이 바뀌면 가이드가 체력퍼센트로 계속 움직이고싶다.
	float newPercent = FMath::Lerp(HPBarGuide->GetPercent(), targetPercent, InDeltaTime * 5);

	HPBarGuide->SetPercent(newPercent);
}

void UPlayerHpUI::UpdateHP(int HP, int MaxHP)
{
	MY_LOG(TEXT("UpdateHP %d / %d"), HP, MaxHP);
	targetPercent = (float)HP / MaxHP;
	HPBar->SetPercent(targetPercent);
	FString txt = FString::Printf(TEXT("%d / %d"), HP, MaxHP);
	TxtHPRate->SetText(FText::FromString(txt));
}
