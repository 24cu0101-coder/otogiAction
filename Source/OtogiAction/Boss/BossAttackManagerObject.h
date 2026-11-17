// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BossAttackManagerObject.generated.h"

/**
 * ボスの1攻撃パターンを表す基底クラス
 * EditInlineNew と DefaultToInstanced により、エディタの配列内で直接インスタンス化・編集が可能になる
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossAttackFinished);

UCLASS(Abstract, Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced)
class OTOGIACTION_API UBossAttackManagerObject : public UObject
{
	GENERATED_BODY()
	
public:
	// 攻撃の識別名（デバッグや抽選用）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	FName AttackName;

	// 抽選時の重み（確率の調整用）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (ClampMin = "0.1"))
	float SelectionWeight = 1.0f;

	// 攻撃終了時にマネージャへ通知するデリゲート
	UPROPERTY(BlueprintAssignable, Category = "Attack")
	FOnBossAttackFinished OnAttackFinished;

	// 初期化（マネージャのBeginPlayなどで呼ぶ）
	virtual void Initialize(AActor* InOwnerActor);

	// 攻撃開始処理
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attack")
	void ExecuteAttack();
	virtual void ExecuteAttack_Implementation();

	// 攻撃中断・キャンセル処理
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Attack")
	void CancelAttack();
	virtual void CancelAttack_Implementation();

	// 毎フレーム更新が必要な場合（突進処理やホーミングなど）
	virtual void TickAttack(float DeltaTime) {}

	// UObject内でSpawnActorやTimerを使えるようにするためのオーバーライド
	virtual UWorld* GetWorld() const override;

protected:
	// 攻撃終了時にサブクラスから必ず呼び出す関数
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void FinishAttack();

	// ボス本体のアクター参照
	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	TObjectPtr<AActor> OwnerActor;
};
