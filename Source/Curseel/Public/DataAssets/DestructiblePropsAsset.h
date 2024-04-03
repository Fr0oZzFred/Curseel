#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GeometryCollection/GeometryCollection.h"
#include "Field/FieldSystem.h"
#include "DestructiblePropsAsset.generated.h"

UCLASS(BlueprintType)
class CURSEEL_API UDestructiblePropsAsset : public UDataAsset {
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UStaticMesh> DestructibleMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class UGeometryCollection> RestCollection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<class USoundBase> DestroySound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class AFieldSystemActor> FieldSystem;
};