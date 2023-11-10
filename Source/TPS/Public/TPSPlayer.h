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
	// ������Ʈ ��������, ī�޶�, ��ź��
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



	// �Է¿� �����ϴ� �Լ� ����
	void AxisHorizontal(float value);
	void AxisVertical(float value);
	void AxisLookUp(float value);
	void AxisTurn(float value);

	void ActionJump();
	void ActionFire();
	void ActionChooseGrenadeGun();
	void ActionChooseSniperGun();


	//1. ũ�ν����, �������� ���� ������ �������ִ�.
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> CrosshairUIFactory;

	UPROPERTY(EditAnywhere)
	class UUserWidget* CrosshairUI;
	
	// �������� ������ ȭ�鿡 ���̰� �ϰ�ʹ�.
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> SniperUIFactory;

	UPROPERTY(EditAnywhere)
	class UUserWidget* SniperUI;


	//3. ũ�ν��� ȭ�鿡 ���̰� �ϰ�ʹ�.
	//4. 1��Ű�� 2��Ű �� Ű�� ������ ũ�ν���� Ȥ�� �������� UI�� ���̰� �ϰ�ʹ�.
	//5. ��������UI�� ���϶��� ZoomIn�� �ϰ�Ͱ� �׷��� �������� ZoomOut�� �ϰ�ʹ�.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TPS)
	bool bChooseGrenadeGun;
};
