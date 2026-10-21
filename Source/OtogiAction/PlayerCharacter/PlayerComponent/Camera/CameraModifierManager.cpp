//担当：飯島
//Modifierを管理するクラス

#include "CameraModifierManager.h"

//コンストラクタ
UCameraModifierManager::UCameraModifierManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}


//生成されたときに処理される
void UCameraModifierManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

//Modifierの更新
void UCameraModifierManager::UpdateModifiers(float DeltaTime, FCameraModifierData& OutData)
{
    OutData = FCameraModifierData();

    for (int32 Index = ActiveModifiers.Num() - 1;
        Index >= 0;
        --Index)
    {
        UPlayerCamearModifier* Modifier = ActiveModifiers[Index];

        if (!Modifier)
        {
            ActiveModifiers.RemoveAt(Index);
            continue;
        }

        Modifier->UpdateModifier(DeltaTime,OutData);

        if (Modifier->IsFinish())
        {
            ActiveModifiers.RemoveAt(Index);
        }
    }
}

//Modifier追加
void UCameraModifierManager::AddModifier(UPlayerCamearModifier* Modifier)
{
    if (!Modifier)
    {
        return;
    }

    ActiveModifiers.AddUnique(Modifier);
}

//Modifierの取り外し
void UCameraModifierManager::RemoveModifier(UPlayerCamearModifier* Modifier)
{
    ActiveModifiers.Remove(Modifier);
}

//Modifierの削除
void UCameraModifierManager::ClearModifiers()
{
    ActiveModifiers.Empty();
}