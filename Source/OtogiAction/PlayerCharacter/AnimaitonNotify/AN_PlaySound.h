//担当：イイジマ

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_PlaySound.generated.h"

//前方宣言
class USoundBase;

UCLASS()
class OTOGIACTION_API UAN_PlaySound : public UAnimNotify
{
	GENERATED_BODY()

public:

	// 通知の処理
	virtual void Notify(USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference)
		override;


	// Notifyに設定するサウンド
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	TObjectPtr<USoundBase> Sound;
};
