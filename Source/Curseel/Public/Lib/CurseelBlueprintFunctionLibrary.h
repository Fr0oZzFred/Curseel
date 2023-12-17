#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WheelPawn.h"
#include "Characters/CharacterBase.h"
#include "UObject/UObjectIterator.h"
#include "CurseelBlueprintFunctionLibrary.generated.h"

UENUM(BlueprintType)
enum class EffectTarget : uint8 {
	Target = 0 UMETA(DisplayName = "Target"),
	Source = 1  UMETA(DisplayName = "Source")
};

USTRUCT(BlueprintType)
struct FGameplayEffectApplier {
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	UAbilitySystemComponent* SourceASC;
	UPROPERTY(BlueprintReadWrite)
	UAbilitySystemComponent* TargetASC;
	UPROPERTY(BlueprintReadWrite)
	EffectTarget EffectTarget;
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> GameplayEffect;
	UPROPERTY(BlueprintReadWrite)
	FGameplayTag MagnitudeTag;
	UPROPERTY(BlueprintReadWrite)
	float Magnitude;
};

UCLASS()
class CURSEEL_API UCurseelBlueprintFunctionLibrary : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

	UFUNCTION(BlueprintPure, Category = "Curseel Functions")
	static AWheelPawn* GetWheel();


	UFUNCTION(BlueprintCallable, Category = "Curseel Functions")
	static void FocusGame(APlayerController* PC);

	UFUNCTION(BlueprintCallable, Category = "Curseel Functions")
	static void FocusUI(APlayerController* PC);

	UFUNCTION(BlueprintCallable, Category = "Curseel Functions")
	static UAbilitySystemComponent* GetCurseelASC(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Curseel Functions")
	static FActiveGameplayEffectHandle ApplyEffect(FGameplayEffectApplier EffectApplier);

	UFUNCTION(BlueprintPure, Category = "Curseel Functions")
	static float InverseLerp(float X, float Y, float Value);
};
