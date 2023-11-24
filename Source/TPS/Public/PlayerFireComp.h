// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComp.h"
#include "PlayerFireComp.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(TPSPlayer), meta=(BlueprintSpawnableComponent) )
class TPS_API UPlayerFireComp : public UPlayerBaseComp
{
	GENERATED_BODY()
public:
	UPlayerFireComp();

	virtual void BeginPlay() override;

	virtual void SetupPlayerInput(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABullet> BulletFactory;

	void ActionFire();
	void ActionChooseGrenadeGun();
	void ActionChooseSniperGun();
	void ActionZoomIn();
	void ActionZoomOut();

	void GrenadeFire();
	void SniperFire();

	//1. 크로스헤어, 스나이퍼 위젯 공장을 가지고있다.
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> CrosshairUIFactory;

	UPROPERTY()
	class UUserWidget* CrosshairUI;
	
	// 스나이퍼 위젯을 화면에 보이게 하고싶다.
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> SniperUIFactory;

	UPROPERTY()
	class UUserWidget* SniperUI;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TPS)
	bool bChooseGrenadeGun;
	
	UPROPERTY(EditAnywhere)
	class UParticleSystem* ExplosionVFXFactory;

	// 총쏘기 동작을 위한 몽타주를 담을 그릇을 만들고싶다.
	UPROPERTY(EditAnywhere)
	class UAnimMontage* FireAnimMontage;
	// 총쏘는 동작 호출 기능
	void PlayFireAnim();

	UPROPERTY(EditAnywhere)
	class USoundBase* FireSound;
	// 총쏘면 진동하게 하고싶다.
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> CameraShakeFactory;

	UPROPERTY()
	UCameraShakeBase* CameraShake;
};
