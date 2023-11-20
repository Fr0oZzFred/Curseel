#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "Characters/Abilities/CharacterASC.h"
#include "Characters/Abilities/AttributeSets/AttributeSetBase.h"
#include "CharacterPlayerState.generated.h"

UCLASS()
class CURSEEL_API ACharacterPlayerState : public APlayerState, public IAbilitySystemInterface {
	GENERATED_BODY()

public:
	ACharacterPlayerState();

	class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	class UAttributeSetBase* GetAttributeSetBase() const;

	UFUNCTION(BlueprintCallable, Category = "GAS|PlayerState")
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "GAS|PlayerState|Attributes")
	float GetHealth() const;
	UFUNCTION(BlueprintCallable, Category = "GAS|PlayerState|Attributes")
	float GetMaxHealth() const;
	UFUNCTION(BlueprintCallable, Category = "GAS|PlayerState|Attributes")
	float GetMana() const;
	UFUNCTION(BlueprintCallable, Category = "GAS|PlayerState|Attributes")
	float GetMaxMana() const;

protected:
	UPROPERTY()
	class UCharacterASC* AbilitySystemComponent;
	UPROPERTY()
	class UAttributeSetBase* AttributeSetBase;

	FGameplayTag DeadTag;
	FDelegateHandle CharacterLevelChangedDelegateHandle;
	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle MaxHealthChangedDelegateHandle;
	FDelegateHandle ManaChangedDelegateHandle;
	FDelegateHandle MaxManaChangedDelegateHandle;

	virtual void BeginPlay() override;

	virtual void CharacterLevelChanged(const FOnAttributeChangeData& Data);
	virtual void HealthChanged(const FOnAttributeChangeData& Data);
	virtual void MaxHealthChanged(const FOnAttributeChangeData& Data);
	virtual void ManaChanged(const FOnAttributeChangeData& Data);
	virtual void MaxManaChanged(const FOnAttributeChangeData& Data);

	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

};
