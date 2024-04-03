#pragma once

#include "CoreMinimal.h"
#include "Dungeon/Props/DestructibleProps.h"
#include "NiagaraComponent.h"
#include "DataAssets/EffectablePropsAsset.h"
#include "Kismet/KismetMathLibrary.h"
#include "EffectableProps.generated.h"

UCLASS()
class CURSEEL_API AEffectableProps : public ADestructibleProps
{
	GENERATED_BODY()
public:
	AEffectableProps();

protected:
	virtual void BeginPlay() override;
	virtual void OnWheelCharacterAOE_Overlaped(AWheelPawn* Wheel) override;

	virtual void OnConstruction(const FTransform& Transform) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> FluidMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UNiagaraComponent> VFX_OnHit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UEffectablePropsAsset> EffectablePropsAsset;

	UFUNCTION()
	void InitEffectableProps();
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit );
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FVector LinearVelocity;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FTimerHandle TimerHandle;

	UFUNCTION()
	void CheckVelocity();
};
