#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GeometryCollection/GeometryCollectionObject.h"
#include "DestructibleProps.generated.h"

UCLASS(BlueprintType)
class CURSEEL_API UDestructibleProps : public UDataAsset {
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMesh* Mesh;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<const UGeometryCollection> ALLO;

};
