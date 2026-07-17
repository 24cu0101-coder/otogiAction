#include "StatusComponent.h"
#include "OtogiAction/PlayerCharacter/PlayerComponent/PlayerDodgeComponent.h"
#include "GameplayTagContainer.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"



UStatusComponent::UStatusComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UStatusComponent::BeginPlay()
{
    Super::BeginPlay();

    CurrentHP = MaxHP;
}
void UStatusComponent::TakeDamage(float Damage)
{
    //プレイヤーが無敵なら処理しない
    if (PlayerInvincible())return;


    CurrentHP -= Damage;
    CurrentHP = FMath::Clamp(CurrentHP, 0.f, MaxHP);

    // HPが変わったことを通知
    OnDamaged.Broadcast(CurrentHP);

    UE_LOG(LogTemp, Warning,
        TEXT("%s HP:%f"),
        *GetOwner()->GetName(),
        CurrentHP);

    if (CurrentHP <= 0.f)
    {
        UE_LOG(LogTemp, Warning,
            TEXT("%s Dead"),
            *GetOwner()->GetName());

        OnDead.Broadcast();
    }
}

void UStatusComponent::Heal(float HealAmount)
{
    CurrentHP += HealAmount;

    CurrentHP = FMath::Clamp(
        CurrentHP,
        0.f,
        MaxHP);
}

bool UStatusComponent::IsDead() const
{
    return CurrentHP <= 0.f;
}

//プレイヤーが無敵かどうか
bool UStatusComponent::PlayerInvincible()
{
    AActor* OwnerActor = GetOwner();
    if (!OwnerActor)
    {
        return false;
    }

    APlayerCharacter* PlayerActor = Cast<APlayerCharacter>(OwnerActor);
    if (!PlayerActor)
    {
        return false;
    }


    UAbilitySystemComponent* ASC = PlayerActor->GetAbilitySystemComponent();

    //アビリティシステムコンポーネントがあれば
    if (ASC)
    {
        //無敵状態のタグを取得
        FGameplayTag InvincibleTag = FGameplayTag::RequestGameplayTag(FName("Invincible"));

        //タグがあったら
        if (ASC->HasMatchingGameplayTag(InvincibleTag))
        {
            //trueを返す
            return true;
        }
    }

    //falseを返す6
    return false;
}

