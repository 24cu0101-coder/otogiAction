//担当：飯島

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraDirectorComponent.generated.h"

//前方宣言
class UCameraModifierDataAsset;
class ULevelSequence;
class ULevelSequencePlayer;
class APlayerCharacter;
class UMoveCameraComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OTOGIACTION_API UCameraDirectorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	//コンストラクタ
	UCameraDirectorComponent();

protected:
	// ゲーム開始時と生成時に呼ばれる
	virtual void BeginPlay() override;

public:
	// 毎フレーム呼ばれる
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//=======================
	//通常カメラを管理する関数
	//=======================

	//カメラのデータアセットをセットする関数
	UFUNCTION(BlueprintCallable, Category = "Camera|Modifier")
	void SetModifier(UCameraModifierDataAsset* Modifier, float Weight);

	//カメラのデータアセットをクリア
	UFUNCTION(BlueprintCallable, Category = "Camera|Modifier")
	void ClearModifier(UCameraModifierDataAsset* Modifier);


	//==================================
	//シネマティックカメラを制御する関数
	//==================================

	//レベルシーケンスを開始
	UFUNCTION(BlueprintCallable, Category = "Camera|Cinematic")
	void PlaySequence(ULevelSequence* Sequence);

	//シーケンス終了
	UFUNCTION(BlueprintCallable, Category = "Camera|Cinematic")
	void StopSequence();

	//現在シーケンスを再生しているか？
	UFUNCTION(BlueprintPure, Category = "Camera|Cinematic")
	bool IsPlayingSequence() const { return bPlayingSequence; }

private:
	//===========================
	//メンバー変数
	//==========================

	//プレイヤーのポインター
	UPROPERTY()
	TObjectPtr<APlayerCharacter>m_PlayerCharacter;

	//カメラの制御クラスポインター
	UPROPERTY()
	TObjectPtr<UMoveCameraComponent>m_MoveCameraComp;

	//スプリングアームコンポーネントのポインター
	UPROPERTY()
	TObjectPtr<USpringArmComponent>m_SpringArmComp;

	//カメラコンポーネントのポインター
	UPROPERTY()
	TObjectPtr<UCameraComponent>m_CameraComp;

	//シーケンスプレイヤーポインター
	UPROPERTY()
	TObjectPtr<ULevelSequencePlayer>m_SequencePlayer;

	//現在シーケンスが再生されているか？
	bool bPlayingSequence = false;

	//使用しているデーターアセットの中身
	struct FActiveModifier
	{
		TObjectPtr<UCameraModifierDataAsset>m_Data = nullptr;
		float Weight = 0.0f;
	};

	//使用できるデータアセット配列
	TArray<FActiveModifier>m_ActiveModifier;

	//データアセット適応
	void ApplyModifier(float DeltaTime);

	//通常カメラに戻る
	void ReturnToGameplayCamra();
};
