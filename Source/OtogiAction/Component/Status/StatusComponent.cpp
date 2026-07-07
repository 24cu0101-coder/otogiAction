#include "StatusComponent.h"
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
    CurrentHP -= Damage;

    UE_LOG(LogTemp, Warning,
        TEXT("%s HP:%f"),
        *GetOwner()->GetName(),
        CurrentHP);

    if (CurrentHP <= 0.f)
    {
        CurrentHP = 0.f;

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

