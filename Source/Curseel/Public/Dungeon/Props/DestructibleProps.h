#pragma once

#include "CoreMinimal.h"
#include "Dungeon/Props/Props.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "DataAssets/DestructiblePropsAsset.h"
#include "Field/FieldSystem.h"
#include "Kismet/GameplayStatics.h"
#include "DestructibleProps.generated.h"

/**
 *
 */
UCLASS()
class CURSEEL_API ADestructibleProps : public AProps {
	GENERATED_BODY()
public:
	ADestructibleProps();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void OnWheelCharacterAOE_Overlaped(AWheelPawn* Wheel) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UGeometryCollectionComponent> GeoCollectionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UDestructiblePropsAsset> PropsAsset;

	void InitDestructibleProps();
};
