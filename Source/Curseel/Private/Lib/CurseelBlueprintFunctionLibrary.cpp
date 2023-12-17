#include "Lib/CurseelBlueprintFunctionLibrary.h"
#include <AbilitySystemBlueprintLibrary.h>

AWheelPawn* UCurseelBlueprintFunctionLibrary::GetWheel() {
    TObjectIterator<AWheelPawn> Itr;
    if (!Itr) return nullptr;
    return *Itr;
}

void UCurseelBlueprintFunctionLibrary::FocusGame(APlayerController* PC) {
    if (!PC) return;

    PC->SetInputMode(FInputModeGameOnly());
    PC->SetShowMouseCursor(false);
}

void UCurseelBlueprintFunctionLibrary::FocusUI(APlayerController* PC) {
    if (!PC) return;

    PC->SetInputMode(FInputModeUIOnly());
    PC->SetShowMouseCursor(true);
}

UAbilitySystemComponent* UCurseelBlueprintFunctionLibrary::GetCurseelASC(AActor* Actor) {
    ACharacterBase* Character = Cast<ACharacterBase>(Actor);
    if (Character)
        return Character->GetAbilitySystemComponent();

    APawnBase* Pawn = Cast<APawnBase>(Actor);
    if (Pawn)
        return Pawn->GetAbilitySystemComponent();

    return nullptr;
}

FActiveGameplayEffectHandle UCurseelBlueprintFunctionLibrary::ApplyEffect(FGameplayEffectApplier EffectApplier) {
    if (!EffectApplier.SourceASC) return NULL;
    if (EffectApplier.EffectTarget == EffectTarget::Target) {
        if (!EffectApplier.TargetASC) return NULL;
    }

    UAbilitySystemComponent* SourceASC = EffectApplier.SourceASC;
    UAbilitySystemComponent* TargetASC = 
        EffectApplier.EffectTarget == EffectTarget::Target ? EffectApplier.TargetASC : SourceASC;

    FGameplayEffectContextHandle EffectContext = EffectApplier.SourceASC->MakeEffectContext();
    EffectContext.AddSourceObject(EffectApplier.SourceASC);


    FGameplayEffectSpecHandle NewHandle = EffectApplier.SourceASC->MakeOutgoingSpec(
        EffectApplier.GameplayEffect, 0.0f, EffectContext);

    if (NewHandle.IsValid()) {
        UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(
            NewHandle, EffectApplier.MagnitudeTag, EffectApplier.Magnitude);
        FActiveGameplayEffectHandle ActiveGEHandle = 
            SourceASC->ApplyGameplayEffectSpecToTarget(*NewHandle.Data.Get(), TargetASC);

        return ActiveGEHandle;
    }
    return NULL;
}

float UCurseelBlueprintFunctionLibrary::InverseLerp(float X, float Y, float Value) {
    return (Value - X) / (Y - X);
}
