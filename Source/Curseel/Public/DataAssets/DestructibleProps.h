#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DestructibleProps.generated.h"

UCLASS(BlueprintType)
class CURSEEL_API UDestructibleProps : public UDataAsset {
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UStaticMesh> DestructibleMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UObject> RestCollection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class USoundBase> DestroySound;
};