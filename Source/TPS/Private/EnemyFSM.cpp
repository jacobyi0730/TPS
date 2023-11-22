// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "TPSPlayer.h"
#include "Enemy.h"
#include "Components/CapsuleComponent.h"
#include "TPS.h"
#include "EnemyAnim.h"
#include "Animation/AnimMontage.h"
#include "AIController.h"
#include <../../../../../../../Source/Runtime/NavigationSystem/Public/NavigationSystem.h>
#include <../../../../../../../Source/Runtime/AIModule/Classes/Navigation/PathFollowingComponent.h>

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	//
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	// 태어날 때 Enemy를 기억하고싶다.
	Me = Cast<AEnemy>(GetOwner());

	EnemyAnim = Cast<UEnemyAnim>(Me->GetMesh()->GetAnimInstance());

	ai = Cast<AAIController>(Me->GetController());
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
	FVector destination = Target->GetActorLocation();
	// 목적지를 향해서 이동하고싶다.
	FVector dir = destination - Me->GetActorLocation();


	// 타겟이 길 위에 있다면
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FPathFindingQuery query;
	FAIMoveRequest req;
	req.SetAcceptanceRadius(50);
	req.SetGoalLocation(destination);
	ai->BuildPathfindingQuery(req, query);
	auto result = ns->FindPathSync(query);
	if (result.IsSuccessful())
	{
		//  타겟을 향해 이동하고싶다.
		ai->MoveToLocation(destination);
	}
	else // 그렇지 않다면
	{
		//  길 위에 랜덤한 위치를 하나 정해서 그곳으로 이동하고싶다.
		FPathFollowingRequestResult r;
		r.Code = ai->MoveToLocation(RandomLocation);
		//  만약 그곳에 도착했거나 문제가 있다면 랜덤한 위치를 갱신하고싶다.
		if ( r != EPathFollowingRequestResult::RequestSuccessful )
		{
			UpdateRandomLocation(Me->GetActorLocation(), 500, RandomLocation);
		}
	}
	
	// 만약 공격 가능거리라면
	if ( dir.Size() <= AttackDistance )
	{
		// 공격 상태로 전이하고싶다.
		SetState(EEnemyState::Attack);
		CurrentTime = AttackTime;
		ai->StopMovement();
	}
}
void UEnemyFSM::TickAttack()
{
	// 시간이 흐르다가(AttackWait시간)
	CurrentTime += GetWorld()->GetDeltaSeconds();
	// 공격 타격 시간이 되면
	if ( CurrentTime > AttackTime )
	{
		EnemyAnim->IsAttack = true;
		CurrentTime = 0;

		// 만약 타겟과의 거리를 재고
		float dist = FVector::Dist(Target->GetActorLocation(), Me->GetActorLocation());
		// 그 거리가 AttackDistance를 초과한다면
		if ( dist > AttackDistance ) {
			// 이동상태로 전이하고싶다.
			SetState(EEnemyState::Move);
			EnemyAnim->IsAttack = false;
		}
		else {
			// 공격을 하고싶다.
			MY_LOG(TEXT("Attack!!!!"));
		}

	}
}

void UEnemyFSM::TickDamage()
{
	//// 시간이 흐르다가
	//CurrentTime += GetWorld()->GetDeltaSeconds();
	//// 현재시간이 DamageTime를 초과하면
	//if ( CurrentTime > DamageTime )
	//{
	//	// 이동상태로 전이하고싶다.
	//	SetState(EEnemyState::Move);
	//}
}

void UEnemyFSM::TickDie()
{
	// 만약 EnemyAnim->IsDieDone이 false라면 함수를 바로 종료하고싶다.
	if ( false == EnemyAnim->IsDieDone )
	{
		return;
	}
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
	ai->StopMovement();
	// 데미지를 입었으면 체력을 1 감소하고싶다.
	HP -= damage;
	if ( HP > 0 ) // 체력이 0보다 크면
	{
		//  데미지 상태로 전이하고싶다.
		SetState(EEnemyState::Damage);
		PlayMontageDamage();
	}
	else // 그렇지 않고 체력이 0 이하라면
	{
		//  죽음 상태로 전이하고싶다.
		SetState(EEnemyState::Die);
		PlayMontageDie();
		// 바닥과 충돌하지 않게 충돌설정을 끄고싶다.
		Me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		DieEndLoc = Me->GetActorLocation() + FVector::DownVector * 200;
	}

}

void UEnemyFSM::SetState(EEnemyState next)
{
	check(EnemyAnim);
	// 이동상태로 전이한다면
	if (next == EEnemyState::Move)
	{
		// 랜덤위치를 갱신하고싶다.
		UpdateRandomLocation(Me->GetActorLocation(), 500, RandomLocation);
	}

	State = next;
	EnemyAnim->State = next;
	CurrentTime = 0;
}

void UEnemyFSM::PlayMontageDamage()
{
	FName sectionName = TEXT("Damage0");
	if ( FMath::RandBool() )
	{
		sectionName = TEXT("Damage1");
	}
	EnemyAnim->PlayMontageDamage(sectionName);
	//Me->PlayAnimMontage(EnemyActionMontage, 1, sectionName);

	//int idx = FMath::RandRange(0, 1);
	//FString str = FString::Printf(TEXT("Damage%d"), idx);
	//FName sectionName = FName(*str);
	//Me->PlayAnimMontage(EnemyActionMontage, 1, sectionName);

}

void UEnemyFSM::PlayMontageDie()
{
	//Me->PlayAnimMontage(EnemyActionMontage, 1, TEXT("Die"));
	FName sectionName = TEXT("Die");
	EnemyAnim->PlayMontageDamage(sectionName);
}

void UEnemyFSM::OnChangeMoveState()
{
	SetState(EEnemyState::Move);
}

bool UEnemyFSM::UpdateRandomLocation(FVector origin, float radius, FVector& outLocation)
{
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;
	bool result = ns->GetRandomReachablePointInRadius(origin, radius, loc);
	if (result)
	{
		outLocation = loc.Location;
		return true;
	}

	return UpdateRandomLocation(origin, radius, outLocation);
}
