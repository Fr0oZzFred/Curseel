#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterInfos.generated.h"

UCLASS(BlueprintType)
class CURSEEL_API UCharacterInfos : public UDataAsset {
	GENERATED_BODY()
public:
	UPROPERTY(EditdefaultsOnly, BlueprintReadOnly)
	FText CharacterName;

	UPROPERTY(EditdefaultsOnly, BlueprintReadOnly)
	FString RichTextStyleName;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FText GetNameWithStyle() const;

	UPROPERTY(EditdefaultsOnly, BlueprintReadOnly)
	UTexture2D* CharacterIcon;
};