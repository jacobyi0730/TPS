// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"

UCLASS()
class TPS_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// 컴포넌트 스프링암, 카메라, 유탄총
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* CameraComp;
	
	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent*	GrenadeGun;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent*		SniperGun;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TPS)
	FVector Direction;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABullet> BulletFactory;



	// 입력에 대응하는 함수 제작
	void AxisHorizontal(float value);
	void AxisVertical(float value);
	void AxisLookUp(float value);
	void AxisTurn(float value);

	void ActionJump();
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

	
	//3. 크로스헤어를 화면에 보이게 하고싶다.
	//4. 1번키와 2번키 각 키를 누르면 크로스헤어 혹은 스나이퍼 UI를 보이게 하고싶다.
	//5. 스나이퍼UI가 보일때는 ZoomIn을 하고싶고 그렇지 않을때는 ZoomOut을 하고싶다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TPS)
	bool bChooseGrenadeGun;
	
	UPROPERTY(EditAnywhere)
	class UParticleSystem* ExplosionVFXFactory;
};
