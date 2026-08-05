
#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GAPlayerDodge.generated.h"

/**
 *
 */
 //クラス前方宣言
class APlayerCharacter;		//プレイヤークラス

UCLASS()
class OTOGIACTION_API UGAPlayerDodge : public UGameplayAbility
{
	GENERATED_BODY()

	UGAPlayerDodge();

public:

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Dodging,
		const FGameplayAbilityActorInfo* playerActorInfo,
		const FGameplayAbilityActivationInfo AvtivationInfo,
		const FGameplayEventData* DodgeTriggerEvent
	) override;



protected:

	//回避のアニメーションモンタージュ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* DodgeMontage;

	//ジャスト回避のアニメーションモンタージュ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Montage")
	UAnimMontage* JustDodgeMontage;


	//プレイヤーの情報
	UPROPERTY(Transient)
	APlayerCharacter* PlayerActor;

	//-------------------
	//関数
	//-------------------


	//回避の開始
	UFUNCTION()

	void DodgeStart();

	//回避時の移動関数
	UFUNCTION()

	void DodgeLocation(float Distance);

	//回避終了時の関数
	UFUNCTION()

	void DodgeEnd();

	//モンタージュ終了
	UFUNCTION()

	void DodgeMontageEnd();

	//ジャスト回避受付関数
	UFUNCTION()
	void JustDodgeWindow();

	//ジャスト回避受付終了の関数
	UFUNCTION()
	void EndJustDodgeWindow();

	//回避開始の関数
	UFUNCTION()
	void PlayDodge();

	//ジャスト回避の関数
	UFUNCTION()
	void PlayJustDodge();

	//ジャスト回避終了の関数
	UFUNCTION()
	void EndJustDodge();

	UFUNCTION()
	void OnPlayerTakeDamage(
		AActor* DamagedActor,
		float Damage,
		const UDamageType* DamageType,
		AController* InstigatedBy,
		AActor* DamageCauser);

	//-------------------
	//-------------------


	//回避実行中のタグ
	FGameplayTag IsDodgeTag;

	//無敵中のタグタグ
	FGameplayTag IsInvincible;


	//アビリティシステム
	UPROPERTY(BlueprintReadOnly, Category = "GAS")
	UAbilitySystemComponent* ASC;


private:

	//回避の数値
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameter.NormalDodge", meta = (AllowPrivateAccess = "true"))
	float DodgeDistance;

	//回避する時間
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameter.NormalDodge", meta = (AllowPrivateAccess = "true"))
	float DodgeTime;

	//回避のディレイの時間
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameter.NormalDodge", meta = (AllowPrivateAccess = "true"))
	float DelayTiem;

	//just回避のフレーム
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameter.JustDodge", meta = (AllowPrivateAccess = "true"))
	float JustFrame;

	//just回避時のスロー倍率
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameter.JustDodge", meta = (AllowPrivateAccess = "true"))
	float SlowMagnification;

	//ジャスト回避のスローのフレーム
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameter.JustDodge", meta = (AllowPrivateAccess = "true"))
	float SlowTime;

	//回避時の無敵の時間
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Parameter.JustDodge", meta = (AllowPrivateAccess = "true"))
	float InvincibleTime;

private:

	//just回避している
	bool JustDodge = false;

	//現在無敵かどうか
	bool IsInvincibleFlag = false;

	//回避時間
	FTimerHandle DodgeTimer;

	//数秒後処理終了
	FTimerHandle EndDodgeTimer;

	//数秒後処理終了
	FTimerHandle EndJustDodgeTimer;


	UFUNCTION()
	void StickRotate();

	//移動の関数をバインドするでりげーど変数 
	FTimerDelegate LocationDelegate;


};
