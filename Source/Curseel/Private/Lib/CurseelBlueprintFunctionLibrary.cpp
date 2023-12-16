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
