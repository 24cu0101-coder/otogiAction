//担当：飯島
//Modifierを管理するクラス

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../Object/CamearModifier.h"
#include "CameraModifierManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OTOGIACTION_API UCameraModifierManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	//コンストラクタ
	UCameraModifierManager();

	//生成されたときに処理される
	virtual void BeginPlay() override;

	//Modifier更新
	void UpdateModifiers(float DeltaTime,FCameraModifierData& OutData);

	//Modifierの追加
	void AddModifier(UPlayerCamearModifier* Modifier);

	//Modifierの取り外し
	void RemoveModifier(UPlayerCamearModifier* Modifier);

	//Modifierの削除
	void ClearModifiers();

	//Modifierを追加するためのテンプレート関数
	template<class T>
	T* AddModifier()
	{
		T* Modifier = NewObject<T>(this);

		if (!Modifier)
		{
			return nullptr;
		}

		AddModifier(Modifier);

		return Modifier;
	}

private:

	UPROPERTY()
	TArray<TObjectPtr<UPlayerCamearModifier>> ActiveModifiers;
		
};
