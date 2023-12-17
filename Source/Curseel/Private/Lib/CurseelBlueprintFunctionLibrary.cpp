#include "Lib/CurseelBlueprintFunctionLibrary.h"

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