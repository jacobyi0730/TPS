// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFireComp.h"
#include "TPSPlayer.h"
#include "Camera/CameraComponent.h"
#include "Bullet.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"
#include "EnemyFSM.h"

UPlayerFireComp::UPlayerFireComp()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayerFireComp::BeginPlay()
{
	Super::BeginPlay();

	//2. 태어날 때 두 개의 위젯을 생성하고싶다.
	CrosshairUI = CreateWidget(GetWorld(), CrosshairUIFactory);
	SniperUI = CreateWidget(GetWorld(), SniperUIFactory);

	CrosshairUI->AddToViewport();
	SniperUI->AddToViewport();

	// 유탄총을 쥐고 시작하고싶다.
	ActionChooseGrenadeGun();

}

void UPlayerFireComp::SetupPlayerInput(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInput(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &UPlayerFireComp::ActionFire);

	PlayerInputComponent->BindAction(TEXT("ChooseGrenadeGun"), IE_Pressed, this, &UPlayerFireComp::ActionChooseGrenadeGun);

	PlayerInputComponent->BindAction(TEXT("ChooseSniperGun"), IE_Pressed, this, &UPlayerFireComp::ActionChooseSniperGun);

	PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Pressed, this, &UPlayerFireComp::ActionZoomIn);

	PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Released, this, &UPlayerFireComp::ActionZoomOut);
}


void UPlayerFireComp::ActionFire()
{
	PlayFireAnim();

	if ( bChooseGrenadeGun )
	{
		GrenadeFire();
	}
	else
	{
		SniperFire();
	}
}

void UPlayerFireComp::ActionChooseGrenadeGun()
{
	bChooseGrenadeGun = true;
	Me->GrenadeGun->SetVisibility(true);
	Me->SniperGun->SetVisibility(false);
	CrosshairUI->SetVisibility(ESlateVisibility::Hidden);
	SniperUI->SetVisibility(ESlateVisibility::Hidden);
	Me->CameraComp->FieldOfView = 90;
	Me->OnChangeGun(true);
}

void UPlayerFireComp::ActionChooseSniperGun()
{
	Me->OnChangeGun(false);
	bChooseGrenadeGun = false;
	Me->GrenadeGun->SetVisibility(false);
	Me->SniperGun->SetVisibility(true);
	CrosshairUI->SetVisibility(ESlateVisibility::Visible);
	SniperUI->SetVisibility(ESlateVisibility::Hidden);
}

void UPlayerFireComp::ActionZoomIn()
{
	if ( bChooseGrenadeGun )
	{
		return;
	}
	Me->CameraComp->FieldOfView = 45;
	CrosshairUI->SetVisibility(ESlateVisibility::Hidden);
	SniperUI->SetVisibility(ESlateVisibility::Visible);
}

void UPlayerFireComp::ActionZoomOut()
{
	if ( bChooseGrenadeGun )
	{
		return;
	}
	CrosshairUI->SetVisibility(ESlateVisibility::Visible);
	SniperUI->SetVisibility(ESlateVisibility::Hidden);
	Me->CameraComp->FieldOfView = 90;
}

void UPlayerFireComp::GrenadeFire()
{
	// 총알을 생성해서 유탄총의 총구 소켓 위치에 배치하고싶다.
	FTransform firePosition = Me->GrenadeGun->GetSocketTransform(TEXT("FirePosition"));
	GetWorld()->SpawnActor<ABullet>(BulletFactory, firePosition);
}

void UPlayerFireComp::SniperFire()
{
	// 카메라의 위치에서 카메라의 앞방향 1km로 바라보고싶다.
	FHitResult hitResult;
	FVector start = Me->CameraComp->GetComponentLocation();
	FVector end = start + Me->CameraComp->GetForwardVector() * 100000.f;
	FCollisionQueryParams params;
	// 바라보고
	if ( GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_Visibility) )
	{
		// 어딘가 부딪혔다.
		// 그곳에 폭발 이펙트를 생성하고싶다.
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionVFXFactory, FTransform(hitResult.ImpactPoint));

		// 만약 부딪힌 물체가 물리작용을 할 수 있다면
		auto hitComp = hitResult.GetComponent();
		if ( hitComp && hitComp->IsSimulatingPhysics() )
		{
			// 그 물체에게 힘을 가하고싶다.
			hitComp->AddForce(-hitResult.ImpactNormal * hitComp->GetMass() * 500000.f);
		}

		// 그 물체가 Enemy라면
		AEnemy* enemy = Cast<AEnemy>(hitResult.GetActor());
		if ( enemy )
		{
			// 너 맞았어 라고 알려주고싶다.
			//enemy->EnemyFSM
			auto fsm = enemy->GetDefaultSubobjectByName(TEXT("EnemyFSM"));
			UEnemyFSM* enemyFSM = Cast<UEnemyFSM>(fsm);
			enemyFSM->OnTakeDamage(1);
		}
	}
	else
	{
		// 부딪힌 곳이 없다... => 허공
	}
}

void UPlayerFireComp::PlayFireAnim()
{
	// 카메라가 흔들리는 중이라면 중지하고 다시 카메라를 처음부터 흔들고싶다.

	if ( CameraShake )
	{
		CameraShake->StopShake();
	}
	// 플레이어 카메라를 가져오고싶다.
	// UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	auto cameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	// 그녀석에게 카메라를 흔들어라 라고싶다.
	CameraShake = cameraManager->StartCameraShake(CameraShakeFactory);

	Me->PlayAnimMontage(FireAnimMontage);

	// 총소리를 내고싶다.
	UGameplayStatics::PlaySound2D(GetWorld(), FireSound);


}
