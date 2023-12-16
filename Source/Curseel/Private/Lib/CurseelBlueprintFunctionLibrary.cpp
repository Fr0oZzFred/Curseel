#include "Lib/CurseelBlueprintFunctionLibrary.h"

AWheelPawn* UCurseelBlueprintFunctionLibrary::GetWheel() {
    TObjectIterator<AWheelPawn> Itr;
    if (!Itr) return nullptr;
    return *Itr;
}
