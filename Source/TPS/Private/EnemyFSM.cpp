// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "TPSPlayer.h"
#include "Enemy.h"
#include "Components/CapsuleComponent.h"

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	// 태어날 때 Enemy를 기억하고싶다.
	Me = Cast<AEnemy>(GetOwner());

}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 내 상태 따라 각 각 일을 처리하고싶다.
	switch ( State )
	{
	case EEnemyState::Idle:		TickIdle();		break;
	case EEnemyState::Move:		TickMove();		break;
	case EEnemyState::Attack:   TickAttack();	break;
	case EEnemyState::Damage:   TickDamage();	break;
	case EEnemyState::Die:		TickDie();		break;
	}
}

void UEnemyFSM::TickIdle()
{
	// 플레이어를 찾아 목적지로 기억하고싶다.
	Target = Cast<ATPSPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());
	// 만약 찾았다면
	if ( Target )
	{
		// 이동상태로 전이하고싶다.
		SetState(EEnemyState::Move); // 전이
	}
}

void UEnemyFSM::TickMove()
{
	// 목적지를 향해서 이동하고싶다.
	FVector dir = Target->GetActorLocation() - Me->GetActorLocation();

	Me->AddMovementInput(dir.GetSafeNormal());
	// 만약 공격 가능거리라면
	if ( dir.Size() <= AttackDistance )
	{
		// 공격 상태로 전이하고싶다.
		SetState(EEnemyState::Attack);
	}
}

void UEnemyFSM::TickAttack()
{

}

void UEnemyFSM::TickDamage()
{
	// 시간이 흐르다가
	CurrentTime += GetWorld()->GetDeltaSeconds();
	// 현재시간이 DamageTime를 초과하면
	if ( CurrentTime > DamageTime )
	{
		// 이동상태로 전이하고싶다.
		SetState(EEnemyState::Move);
	}
}

void UEnemyFSM::TickDie()
{
	// 시간이 흐르다가
	CurrentTime += GetWorld()->GetDeltaSeconds();
	// 현재시간이 DieTime를 초과하면
	if ( CurrentTime > DieTime )
	{
		// 파괴되고싶다.
		Me->Destroy();
	}
	else 
	{
		// 바닥으로 내려가고 싶다.
		// P = P0 + vt;
		//FVector P0 = Me->GetActorLocation();
		//FVector velocity = FVector::DownVector * 200;
		//FVector vt = velocity * GetWorld()->GetDeltaSeconds();
		//Me->SetActorLocation(P0 + vt);

		// 선형보간
		// 시작 Me->GetActorLocation()
		// 끝 DieEndLoc
		// t : ds * 6

		FVector NewLoc = FMath::Lerp(Me->GetActorLocation(), DieEndLoc, GetWorld()->GetDeltaSeconds() * 2);

		Me->SetActorLocation(NewLoc);
	}
}

void UEnemyFSM::OnTakeDamage(int32 damage)
{
	// 데미지를 입었으면 체력을 1 감소하고싶다.
	HP -= damage;
	if ( HP > 0 ) // 체력이 0보다 크면
	{
		//  데미지 상태로 전이하고싶다.
		SetState(EEnemyState::Damage);
	}
	else // 그렇지 않고 체력이 0 이하라면
	{
		//  죽음 상태로 전이하고싶다.
		SetState(EEnemyState::Die);
		// 바닥과 충돌하지 않게 충돌설정을 끄고싶다.
		Me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		DieEndLoc = Me->GetActorLocation() + FVector::DownVector * 200;
	}

}

void UEnemyFSM::SetState(EEnemyState next)
{
	State = next;
	CurrentTime = 0;
}
