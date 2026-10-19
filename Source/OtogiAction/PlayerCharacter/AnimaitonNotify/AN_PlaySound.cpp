//担当：イイジマ


#include "AN_PlaySound.h"

#include "Components/SkeletalMeshComponent.h"
#include "../../Component/Audio/BGMManager.h"
#include "Sound/SoundBase.h"

void UAN_PlaySound::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation);

	// Meshが存在しない
	if (!MeshComp) return;

	// MeshのOwnerを取得
	AActor* Owner = MeshComp->GetOwner();

	if (!Owner) return;

	// OwnerからSoundManagerComponentを取得
	USoundManagerComponent* SoundManager = Owner->FindComponentByClass<USoundManagerComponent>();

	if (!Sound)return;

	// サウンド再生
	SoundManager->PlayAnimNotifySound(Sound);

}
