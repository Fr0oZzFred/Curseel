#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/StaticMesh.h"
#include "DissolvableMesh.generated.h"

UCLASS(BlueprintType)
class CURSEEL_API UDissolvableMesh : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMesh* Mesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMesh* DissolvableMesh;
};
