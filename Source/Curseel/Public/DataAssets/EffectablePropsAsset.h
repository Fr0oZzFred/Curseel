#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include <NiagaraSystem.h>
#include <AbilitySystemBlueprintLibrary.h>
#include "EffectablePropsAsset.generated.h"

UCLASS()
class CURSEEL_API UEffectablePropsAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel")
	TObjectPtr<UStaticMesh> FluidMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Barrel")
	TObjectPtr<UMaterialInstance> FluidMeshMat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OnHit")
	TObjectPtr<UNiagaraSystem> OnHitFX;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OnHit")
	FLinearColor HitColor;



	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OnDestroy")
	TObjectPtr<UNiagaraSystem> OnDestroyFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OnDestroy")
	FLinearColor DestroyColor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "OnDestroy")
	TObjectPtr<class USoundBase> StealSound;



	UPROPERTY(EditdefaultsOnly, BlueprintReadOnly, Category = "Effect")
	TSubclassOf<class UGameplayEffect> Effect;

	UPROPERTY(EditdefaultsOnly, BlueprintReadOnly, Category = "Effect")
	FGameplayTag EffectApplier;
	UPROPERTY(EditdefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float EffectMAgnitude;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	TObjectPtr<UNiagaraSystem> StealVFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	FLinearColor StealColor;
	
};
