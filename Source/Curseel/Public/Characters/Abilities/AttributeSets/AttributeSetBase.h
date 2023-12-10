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
	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, Health)

	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Damage", ReplicatedUsing = OnRep_Damage)
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, Damage)

	UPROPERTY(BlueprintReadOnly, Category = "DamageBuff", ReplicatedUsing = OnRep_DamageBuff)
	FGameplayAttributeData DamageBuff;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, DamageBuff)

	UPROPERTY(BlueprintReadOnly, Category = "DamageReduction", ReplicatedUsing = OnRep_DamageReduction)
	FGameplayAttributeData DamageReduction;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, DamageReduction)

	UPROPERTY(BlueprintReadOnly, Category = "MoveSpeed", ReplicatedUsing = OnRep_MoveSpeed)
	FGameplayAttributeData MoveSpeed;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, MoveSpeed)
	
	UPROPERTY(BlueprintReadOnly, Category = "AttackSpeed", ReplicatedUsing = OnRep_AttackSpeed)
	FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS(UAttributeSetBase, AttackSpeed)

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);
	UFUNCTION()
	virtual void OnRep_Damage(const FGameplayAttributeData& OldDamage);
	UFUNCTION()
	virtual void OnRep_DamageBuff(const FGameplayAttributeData& OldDamageBuff);
	UFUNCTION()
	virtual void OnRep_DamageReduction(const FGameplayAttributeData& OldDamageReduction);
	UFUNCTION()
	virtual void OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed);
	UFUNCTION()
	virtual void OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
};