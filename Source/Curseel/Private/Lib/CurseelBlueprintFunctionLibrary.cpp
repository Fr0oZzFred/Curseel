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
    EffectContext.AddSourceObject(EffectApplier.SourceASC->GetAvatarActor());


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

void UCurseelBlueprintFunctionLibrary::GetRandomNames(int Number, const TArray<FName>& InNames, TArray<FName>& OutNames) {
    if (Number == 0) return;
    
    int r = FMath::RandRange(0, InNames.Num() - 1);
    for (int i = 0; i < Number; i++) {
        while (OutNames.Find(InNames[r]) != INDEX_NONE) {
            r = FMath::RandRange(0, InNames.Num() - 1);
        }
        OutNames.Add(InNames[r]);
    }
}

void UCurseelBlueprintFunctionLibrary::ApplyBoost(UAbilitySystemComponent* ASC, UBoost* Boost) {
    FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
    EffectContext.AddSourceObject(ASC->GetAvatarActor());

    FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(Boost->Effect, 0.0f, EffectContext);
    ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void UCurseelBlueprintFunctionLibrary::GetRandomBoosts(int Number, const TArray<UBoost*>& InBoosts, const TArray<UBoost*>& ActiveBoosts, TArray<UBoost*>& OutBoosts) {
    if (Number == 0) return;
    TArray<UBoost*> Boosts = InBoosts;
    
    //Change Boosts Drop Rate
    for (UBoost* B : ActiveBoosts) {
        if (B->bUnique) Boosts.Remove(B);
        for (UBoost* Blocked : B->BlockedBoosts) {
            Boosts.Remove(Blocked);
        }
    }

    for (UBoost* Boost : ActiveBoosts) {
        for (UBoost* Synergy : Boost->SynergyBoosts) {
            if (Boosts.Find(Synergy) != INDEX_NONE) {
                Boosts.Add(Synergy);
            }
        }
    }

    //if (true) {
    //    OutBoosts = Boosts;
    //    return;
    //}


    //Avoid exceptions
    if (Boosts.Num() == 0) return;
    if (Boosts.Num() < Number) {
        Boosts.Empty();
        return;
    }
    if (Boosts.Num() == Number) {
        OutBoosts = Boosts;
        return;
    }

    //Random
    int r = FMath::RandRange(0, Boosts.Num() - 1);
    for (int i = 0; i < Number; i++) {
        while (OutBoosts.Find(Boosts[r]) != INDEX_NONE) {
            r = FMath::RandRange(0, Boosts.Num() - 1);
        }
        OutBoosts.Add(Boosts[r]);
    }
}

void UCurseelBlueprintFunctionLibrary::AddCutsceneInput(UEnhancedInputLocalPlayerSubsystem* PS, const UInputMappingContext* ContextToRemove, const UInputMappingContext* ContextToAdd) {
    PS->RemoveMappingContext(ContextToRemove);
    PS->AddMappingContext(ContextToAdd, 20);
}

void UCurseelBlueprintFunctionLibrary::RemoveCutsceneInput(UEnhancedInputLocalPlayerSubsystem* PS, UInputMappingContext* ContextToRemove, const UInputMappingContext* ContextToAdd) {
    PS->RemoveMappingContext(ContextToRemove);
    PS->AddMappingContext(ContextToAdd, 0);
}