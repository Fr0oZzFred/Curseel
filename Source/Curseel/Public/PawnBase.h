#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "Curseel/Curseel.h"
#include "Characters/Abilities/CharacterASC.h"
#include "Characters/Abilities/CharacterGameplayAbility.h"
#include "Characters/Abilities/AttributeSets/AttributeSetBase.h"
#include "DataAssets/Boost.h"
#include "PawnBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPawnDiedDelegate, APawnBase*, Pawn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPawnStunnedDelegate, APawnBase*, Pawn, bool, Stunned);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FPawnHealthChangedDelegate, APawnBase*, Pawn, float, NewHealth, float, OldHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPawnMoveSpeedChangedDelegate, APawnBase*, Pawn);

UCLASS()
class CURSEEL_API APawnBase : public APawn, public IAbilitySystemInterface {
	GENERATED_BODY()

public:
	APawnBase(const class FObjectInitializer& ObjectInitializer);

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable, Category = "GAS|Pawns")
	virtual bool IsAlive() const;

	UFUNCTION(BlueprintCallable, Category = "GAS|Pawns")
	virtual void Die();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS|Pawns")
	FText Name;

protected:
	virtual void BeginPlay() override;
	//Not used for now
	//virtual void Tick(float DeltaTime) override;
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


#pragma region ASC
protected:

	// Actual hard pointer to AbilitySystemComponent
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS")
	class UCharacterASC* HardRefAbilitySystemComponent;

	// Actual hard pointer to AttributeSetBase
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS")
	class UAttributeSetBase* HardRefAttributeSetBase;

	TWeakObjectPtr<class UCharacterASC> AbilitySystemComponent;
	TWeakObjectPtr<class UAttributeSetBase> AttributeSetBase;

	FGameplayTag DeadTag;
	FGameplayTag EffectRemoveOnDeathTag;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS|Abilities")
	TArray<TSubclassOf<class UCharacterGameplayAbility>> Abilities;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS|Abilities")
	TSubclassOf<class UGameplayEffect> DefaultAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS|Abilities")
	TArray<TSubclassOf<class UGameplayEffect>> StartupEffects;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "GAS|Abilities")
	TArray<class UBoost*> StartupBoosts;

	virtual void AddAbilities();

	virtual void InitializeAttributes();

	virtual void AddStartupEffects();
	virtual void AddStartupBoosts();

	virtual void RemoveAbilities();

	UFUNCTION(BlueprintCallable, Category = "GAS|Pawns")
	virtual int32 GetAbilityLevel(AbilityID AbilityID) const;
#pragma endregion



#pragma region Delegates
public:
	UPROPERTY(BlueprintAssignable, Category = "GAS|Pawns")
	FPawnDiedDelegate OnPawnDied;
	UPROPERTY(BlueprintAssignable, Category = "GAS|Pawns")
	FPawnStunnedDelegate OnPawnStunned;
	UPROPERTY(BlueprintAssignable, Category = "GAS|Pawns")
	FPawnHealthChangedDelegate OnPawnHealthChanged;
	UPROPERTY(BlueprintAssignable, Category = "GAS|Pawns")
	FPawnMoveSpeedChangedDelegate OnPawnMoveSpeedChanged;


protected:
	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle MoveSpeedChangedDelegateHandle;

	virtual void HealthChanged(const FOnAttributeChangeData& Data);

	virtual void MoveSpeedChanged(const FOnAttributeChangeData& Data);

	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
#pragma endregion



#pragma region GetAttributes
	UFUNCTION(BlueprintCallable, Category = "GAS|Pawns|Attributes")
	float GetHealth() const;
	UFUNCTION(BlueprintCallable, Category = "GAS|Pawns|Attributes")
	float GetMaxHealth() const;
	
	
	UFUNCTION(BlueprintCallable, Category = "GAS|Pawns|Attributes")
	float GetShield() const;
	UFUNCTION(BlueprintCallable, Category = "GAS|Pawns|Attributes")
	float GetMaxShield() const;
	
	
	UFUNCTION(BlueprintCallable, Category = "GAS|Pawns|Attributes")
	float GetAttack() ;
	UFUNCTION(BlueprintCallable, Category = "GAS|Pawns|Attributes")
	float GetAttackSpeed() const;
	UFUNCTION(BlueprintCallable, Category = "GAS|Pawns|Attributes")
	float GetAttackPotency() const;
	
	
	UFUNCTION(BlueprintCallable, Category = "GAS|Pawns|Attributes")
	float GetHealthSteal() const;
	UFUNCTION(BlueprintCallable, Category = "GAS|Pawns|Attributes")
	float GetHealthStealPotency() const;
	UFUNCTION(BlueprintCallable, Category = "GAS|Pawns|Attributes")
	float GetShieldSteal() const;
	UFUNCTION(BlueprintCallable, Category = "GAS|Pawns|Attributes")
	float GetShieldStealPotency() const;
	
	
	UFUNCTION(BlueprintCallable, Category = "GAS|Pawns|Attributes")
	float GetAttackResistance() const;
	UFUNCTION(BlueprintCallable, Category = "GAS|Pawns|Attributes")
	float GetStealResistance() const;
	
	
	UFUNCTION(BlueprintCallable, Category = "GAS|Pawns|Attributes")
	float GetMoveSpeed() const;
	UFUNCTION(BlueprintCallable, Category = "GAS|Pawns|Attributes")
	float GetDashPower() const;
	UFUNCTION(BlueprintCallable, Category = "GAS|Pawns|Attributes")
	float GetDashCooldown() const;
#pragma endregion

#pragma region SetAttributes
	UFUNCTION(BlueprintCallable, Category = "GAS|Pawns|Attributes|Hardcode")
	void SetHealth(float NewValue);
	UFUNCTION(BlueprintCallable, Category = "GAS|Pawns|Attributes|Hardcode")
	void SetMaxHealth(float NewValue);


	UFUNCTION(BlueprintCallable, Category = "GAS|Pawns|Attributes|Hardcode")
	void SetShield(float NewValue);
	UFUNCTION(BlueprintCallable, Category = "GAS|Pawns|Attributes|Hardcode")
	void SetMaxShield(float NewValue);


	UFUNCTION(BlueprintCallable, Category = "GAS|Pawns|Attributes|Hardcode")
	void SetAttack(float NewValue);
	UFUNCTION(BlueprintCallable, Category = "GAS|Pawns|Attributes|Hardcode")
	void SetAttackSpeed(float NewValue);
	UFUNCTION(BlueprintCallable, Category = "GAS|Pawns|Attributes|Hardcode")
	void SetAttackPotency(float NewValue);


	UFUNCTION(BlueprintCallable, Category = "GAS|Pawns|Attributes|Hardcode")
	void SetHealthSteal(float NewValue);
	UFUNCTION(BlueprintCallable, Category = "GAS|Pawns|Attributes|Hardcode")
	void SetHealthStealPotency(float NewValue);
	UFUNCTION(BlueprintCallable, Category = "GAS|Pawns|Attributes|Hardcode")
	void SetShieldSteal(float NewValue);
	UFUNCTION(BlueprintCallable, Category = "GAS|Pawns|Attributes|Hardcode")
	void SetShieldStealPotency(float NewValue);


	UFUNCTION(BlueprintCallable, Category = "GAS|Pawns|Attributes|Hardcode")
	void SetAttackResistance(float NewValue);
	UFUNCTION(BlueprintCallable, Category = "GAS|Pawns|Attributes|Hardcode")
	void SetStealResistance(float NewValue);


	UFUNCTION(BlueprintCallable, Category = "GAS|Pawns|Attributes|Hardcode")
	void SetMoveSpeed(float NewValue);
	UFUNCTION(BlueprintCallable, Category = "GAS|Pawns|Attributes|Hardcode")
	void SetDashPower(float NewValue);
	UFUNCTION(BlueprintCallable, Category = "GAS|Pawns|Attributes|Hardcode")
	void SetDashCooldown(float NewValue);
#pragma endregion
};