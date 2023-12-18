#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "AttributeSetBase.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class CURSEEL_API UAttributeSetBase : public UAttributeSet {
	GENERATED_BODY()

public:
#pragma region Attributes
	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, Health)

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, MaxHealth)



	UPROPERTY(BlueprintReadOnly, Category = "Shield", ReplicatedUsing = OnRep_Shield)
	FGameplayAttributeData Shield;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, Shield)
		
	UPROPERTY(BlueprintReadOnly, Category = "Shield", ReplicatedUsing = OnRep_MaxShield)
	FGameplayAttributeData MaxShield;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, MaxShield)



	UPROPERTY(BlueprintReadOnly, Category = "Attack", ReplicatedUsing = OnRep_Attack)
	FGameplayAttributeData Attack;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, Attack)

	UPROPERTY(BlueprintReadOnly, Category = "Attack", ReplicatedUsing = OnRep_AttackSpeed)
	FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, AttackSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "Attack", ReplicatedUsing = OnRep_AttackPotency)
	FGameplayAttributeData AttackPotency;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, AttackPotency)



	UPROPERTY(BlueprintReadOnly, Category = "Attributes Steal", ReplicatedUsing = OnRep_HealthSteal)
	FGameplayAttributeData HealthSteal;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, HealthSteal)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes Steal", ReplicatedUsing = OnRep_HealthStealPotency)
	FGameplayAttributeData HealthStealPotency;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, HealthStealPotency)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes Steal", ReplicatedUsing = OnRep_ShieldSteal)
	FGameplayAttributeData ShieldSteal;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, ShieldSteal)

	UPROPERTY(BlueprintReadOnly, Category = "Attributes Steal", ReplicatedUsing = OnRep_ShieldStealPotency)
	FGameplayAttributeData ShieldStealPotency;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, ShieldStealPotency)



	UPROPERTY(BlueprintReadOnly, Category = "Resistances", ReplicatedUsing = OnRep_AttackResistance)
	FGameplayAttributeData AttackResistance;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, AttackResistance)

	UPROPERTY(BlueprintReadOnly, Category = "Resistances", ReplicatedUsing = OnRep_StealResistance)
	FGameplayAttributeData StealResistance;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, StealResistance)



	UPROPERTY(BlueprintReadOnly, Category = "Movement", ReplicatedUsing = OnRep_MoveSpeed)
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, MoveSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "Movement", ReplicatedUsing = OnRep_DashPower)
	FGameplayAttributeData DashPower;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, DashPower)

	UPROPERTY(BlueprintReadOnly, Category = "Movement", ReplicatedUsing = OnRep_DashCooldown)
	FGameplayAttributeData DashCooldown;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, DashCooldown)
#pragma endregion

#pragma region Attributes Replications
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	virtual void OnRep_Shield(const FGameplayAttributeData& OldShield);
	UFUNCTION()
	virtual void OnRep_MaxShield(const FGameplayAttributeData& OldMaxShield);

	UFUNCTION()
	virtual void OnRep_Attack(const FGameplayAttributeData& OldAttack);
	UFUNCTION()
	virtual void OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed);
	UFUNCTION()
	virtual void OnRep_AttackPotency(const FGameplayAttributeData& OldAttackPotency);

	UFUNCTION()
	virtual void OnRep_HealthSteal(const FGameplayAttributeData& OldHealthSteal);
	UFUNCTION()
	virtual void OnRep_HealthStealPotency(const FGameplayAttributeData& OldHealthStealPotency);
	UFUNCTION()
	virtual void OnRep_ShieldSteal(const FGameplayAttributeData& OldShieldSteal);
	UFUNCTION()
	virtual void OnRep_ShieldStealPotency(const FGameplayAttributeData& OldShieldStealPotency);

	UFUNCTION()
	virtual void OnRep_AttackResistance(const FGameplayAttributeData& OldAttackResistance);
	UFUNCTION()
	virtual void OnRep_StealResistance(const FGameplayAttributeData& OldStealResistance);

	UFUNCTION()
	virtual void OnRep_MoveSpeed(const FGameplayAttributeData& OlMoveSpeed);
	UFUNCTION()
	virtual void OnRep_DashPower(const FGameplayAttributeData& OldDashPower);
	UFUNCTION()
	virtual void OnRep_DashCooldown(const FGameplayAttributeData& OldDashCooldown);


#pragma endregion

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
};