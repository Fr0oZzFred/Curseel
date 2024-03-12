#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Boost.generated.h"

UCLASS(BlueprintType)
class CURSEEL_API UBoost : public UDataAsset {
	GENERATED_BODY()
public:
	UPROPERTY(EditdefaultsOnly, BlueprintReadOnly, Category = "Boost Infos")
	UTexture2D* Icon;

	UPROPERTY(EditdefaultsOnly, BlueprintReadOnly, Category = "Boost Infos")
	FText Name;

	UPROPERTY(EditdefaultsOnly, BlueprintReadOnly, Category = "Boost Infos", meta = (MultiLine = true))
	FText Description;

	UPROPERTY(EditdefaultsOnly, BlueprintReadOnly, Category = "Boost Infos")
	TSubclassOf<class UGameplayEffect> Effect;

	UPROPERTY(EditdefaultsOnly, BlueprintReadOnly, Category = "Boost Infos")
	bool bHard;


	UPROPERTY(EditdefaultsOnly, BlueprintReadOnly, Category = "Boost Droprate")
	bool bUnique;

	UPROPERTY(EditdefaultsOnly, BlueprintReadOnly, Category = "Boost Droprate")
	TArray<class UBoost*> BlockedBoosts;

	UPROPERTY(EditdefaultsOnly, BlueprintReadOnly, Category = "Boost Droprate")
	TArray<class UBoost*> SynergyBoosts;

};
