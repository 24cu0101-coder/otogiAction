#include "StatusComponent.h"
#include "OtogiAction/PlayerCharacter/PlayerCharacter.h"
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
    AActor* OwnerActor = GetOwner();
    APlayerCharacter* PlayerActor = Cast<APlayerCharacter>(OwnerActor);
    UAbilitySystemComponent* ASC = PlayerActor->GetAbilitySystemComponent();
    
    FGameplayTag InvincibleTag = FGameplayTag::RequestGameplayTag(FName("Iinvincible "));


    //アビリティシステムコンポーネントがあれば
    if (ASC)
    {
        if (ASC->HasMatchingGameplayTag(InvincibleTag))
        {
            return;
        }
    }



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

