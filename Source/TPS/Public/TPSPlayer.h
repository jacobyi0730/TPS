// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"

//// 싱글캐스트
//DECLARE_DELEGATE(FInputSingleDelegate);
//DECLARE_DELEGATE_OneParam(FInputSingleParamDelegate, int32);
//// 멀티캐스트
//DECLARE_MULTICAST_DELEGATE(FInputMultiDelegate);
//DECLARE_MULTICAST_DELEGATE_OneParam(FInputMultiParamDelegate, FVector);
//// 블루프린트용 델리게이트 선언
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInputDynamicMultiDelegate);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputDynamicMultiParamDelegate, FVector, dir);


DECLARE_MULTICAST_DELEGATE_OneParam(FInputDelegate, class UInputComponent*);


UCLASS()
class TPS_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSPlayer();
public:
	FInputDelegate OnSetupInputDelegate;

#pragma region Delegate Example
	//ATPSPlayer(int a)
//{
//	// 싱글 캐스트 함수연결
//	//OnSingleDelegate.BindUFunction(this, TEXT("AA"));
//	OnSingleDelegate.BindUObject(this, &ATPSPlayer::AA);
//	// 싱글 캐스트 함수 호출
//	OnSingleDelegate.Execute();

//	// 멀티 캐스트 함수 연결과 호출
//	OnMultiDelegate.AddUObject(this, &ATPSPlayer::AA);
//	OnMultiDelegate.AddUObject(this, &ATPSPlayer::AB);

//	OnMultiDelegate.Broadcast();

//	// 
//	OnDynamicMultiDelegate.AddDynamic(this, &ATPSPlayer::GetDir);

//	FVector fff;
//	OnDynamicMultiDelegate.Broadcast(fff);
//}
//void AA(){}
//void AB(){}

//void GetDir(FVector dir) {}
//FInputSingleDelegate OnSingleDelegate;

//FInputMultiDelegate OnMultiDelegate;

//FInputDynamicMultiParamDelegate OnDynamicMultiDelegate;
#pragma endregion



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

	UPROPERTY(EditDefaultsOnly)
	class UPlayerBaseComp* MoveComp;

	UPROPERTY(EditDefaultsOnly)
	class UPlayerBaseComp* FireComp;

	// 태어날 때 HP를 MaxHP로 하고 HP UI를 생성해서 보이게 하고싶다.
	int HP;
	int MaxHP = 100;

	UPROPERTY(EditDefaultsOnly);
	TSubclassOf<class UPlayerHpUI> HpUIFactory;

	UPROPERTY()
	class UPlayerHpUI* PlayerHP;

	void OnMyHit();
};
