#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WheelPawn.h"
#include "UObject/UObjectIterator.h"
#include "CurseelBlueprintFunctionLibrary.generated.h"

UCLASS()
class CURSEEL_API UCurseelBlueprintFunctionLibrary : public UBlueprintFunctionLibrary {
	GENERATED_UCLASS_BODY()
	/** Starts an analytics session without any custom attributes specified */
	UFUNCTION(BlueprintCallable, Category = "Curseel Functions")
	static AWheelPawn* GetWheel();

};
