// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"
#include "Enemy.h"
#include "TPSPlayer.h"

void UEnemyAnim::OnDamageEnd()
{
	// Enemy를 가져와서 EnemyFSM 의 OnChangeMoveState 함수를 호출하고싶다.
	AEnemy* enemy = Cast<AEnemy>(TryGetPawnOwner());
	if (enemy)
	{
		enemy->EnemyFSM->OnChangeMoveState();
	}
}

void UEnemyAnim::AnimNotify_DamageEnd()
{
	// Enemy를 가져와서 EnemyFSM 의 OnChangeMoveState 함수를 호출하고싶다.
	// 몽타주도 스톱하고싶다.
	AEnemy* enemy = Cast<AEnemy>(TryGetPawnOwner());
	if ( enemy )
	{
		enemy->EnemyFSM->OnChangeMoveState();
		Montage_Stop(0.1f, enemy->EnemyFSM->EnemyActionMontage);
	}
}

void UEnemyAnim::AnimNotify_DieEnd()
{
	// 죽음 애니메이션이 끝나면 IsDieDone을 true로 해서 바닥으로 내려가게 하고싶다.
	IsDieDone = true;
}

void UEnemyAnim::AnimNotify_OnAttackHit()
{
	UE_LOG(LogTemp, Warning, TEXT("UEnemyAnim::AnimNotify_OnAttackHit"));
	// 플레이어를 찾아서 데미지를 입히고싶다.
	auto c = GetWorld()->GetFirstPlayerController()->GetCharacter();
	ATPSPlayer* player = Cast<ATPSPlayer>(c);
	player->OnMyHit();
}
