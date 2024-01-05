#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Boost.generated.h"

UCLASS(BlueprintType)
class CURSEEL_API UBoost : public UDataAsset {
	GENERATED_BODY()
public:
	UPROPERTY(EditdefaultsOnly, BlueprintReadOnly)
	UTexture2D* Icon;

	UPROPERTY(EditdefaultsOnly, BlueprintReadOnly)
	FText Name;

	UPROPERTY(EditdefaultsOnly, BlueprintReadOnly)
	FText Description;

	UPROPERTY(EditdefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UGameplayEffect> Effect;

	UPROPERTY(EditdefaultsOnly, BlueprintReadOnly)
	bool bHard;
};
