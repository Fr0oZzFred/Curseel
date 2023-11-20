#pragma once

#include "CoreMinimal.h"
#include "Characters/CharacterBase.h"
#include "GameplayEffectTypes.h"
#include "Characters/Abilities/CharacterASC.h"
#include "Characters/Abilities/AttributeSets/AttributeSetBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MonsterCharacter.generated.h"

UCLASS()
class CURSEEL_API AMonsterCharacter : public ACharacterBase
{
	GENERATED_BODY()
public:
	AMonsterCharacter (const class FObjectInitializer& ObjectInitializer);

protected:

	// Actual hard pointer to AbilitySystemComponent
	UPROPERTY()
	class UCharacterASC* HardRefAbilitySystemComponent;

	// Actual hard pointer to AttributeSetBase
	UPROPERTY()
	class UAttributeSetBase* HardRefAttributeSetBase;

	virtual void BeginPlay() override;

	FDelegateHandle HealthChangedDelegateHandle;

	virtual void HealthChanged(const FOnAttributeChangeData& Data);

	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
};