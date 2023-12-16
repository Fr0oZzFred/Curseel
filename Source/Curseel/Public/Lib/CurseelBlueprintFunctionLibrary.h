#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WheelPawn.h"
#include "UObject/UObjectIterator.h"
#include "CurseelBlueprintFunctionLibrary.generated.h"

UCLASS()
class CURSEEL_API UCurseelBlueprintFunctionLibrary : public UBlueprintFunctionLibrary {
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintPure, Category = "Curseel Functions")
	static AWheelPawn* GetWheel();


	UFUNCTION(BlueprintCallable, Category = "Curseel Functions")
	static void FocusGame(APlayerController* PC);

	UFUNCTION(BlueprintCallable, Category = "Curseel Functions")
	static void FocusUI(APlayerController* PC);
};
