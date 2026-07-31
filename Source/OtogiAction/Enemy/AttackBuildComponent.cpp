// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackBuildComponent.h"

UAttackBuildComponent::UAttackBuildComponent()
{
}

//攻撃開始時に呼ばれる関数
void UAttackBuildComponent::ExecuteAttack()
{
	//発火
	//第一引数：半径、第二引数：対象のTag、第三引数：与ダメージ、第四引数：スタン値、第五引数：ヒットストップの時間
	//第六引数：ヒットストップ時の時間の流れの倍率、第七引数：判定をどれだけ前に出すか、第八引数：判定をどれだけ横に出すか
	StartAttackHandle.Broadcast(Radius,TargetTag,Damage,StunPoint,HitStopDuration,
		HitStopTimeScale,ForwardOffset,SideOffset);
}