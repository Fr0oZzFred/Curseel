#include "Characters/Abilities/AttributeSets/AttributeSetBase.h"

#pragma region Attributes Replications
void UAttributeSetBase::OnRep_Health(const FGameplayAttributeData& OldHealth) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, Health, OldHealth);
}
void UAttributeSetBase::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, MaxHealth, OldMaxHealth);
}


void UAttributeSetBase::OnRep_Shield(const FGameplayAttributeData& OldShield) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, Shield, OldShield);
}
void UAttributeSetBase::OnRep_MaxShield(const FGameplayAttributeData& OldMaxShield) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, MaxShield, OldMaxShield);
}


void UAttributeSetBase::OnRep_Attack(const FGameplayAttributeData& OldAttack) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, Attack, OldAttack);
}
void UAttributeSetBase::OnRep_AttackSpeed(const FGameplayAttributeData& OldAttackSpeed) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, AttackSpeed, OldAttackSpeed);
}
void UAttributeSetBase::OnRep_AttackPotency(const FGameplayAttributeData& OldAttackPotency) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, AttackPotency, OldAttackPotency);
}


void UAttributeSetBase::OnRep_HealthSteal(const FGameplayAttributeData& OldHealthSteal) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, HealthSteal, OldHealthSteal);
}
void UAttributeSetBase::OnRep_HealthStealPotency(const FGameplayAttributeData& OldHealthStealPotency) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, HealthStealPotency, OldHealthStealPotency);
}
void UAttributeSetBase::OnRep_ShieldSteal(const FGameplayAttributeData& OldShieldSteal) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, ShieldSteal, OldShieldSteal);
}
void UAttributeSetBase::OnRep_ShieldStealPotency(const FGameplayAttributeData& OldShieldStealPotency) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, ShieldStealPotency, OldShieldStealPotency);
}
void UAttributeSetBase::OnRep_AttackResistance(const FGameplayAttributeData& OldAttackResistance) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, AttackResistance, OldAttackResistance);
}
void UAttributeSetBase::OnRep_StealResistance(const FGameplayAttributeData& OldStealResistance) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, StealResistance, OldStealResistance);
}


void UAttributeSetBase::OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, MoveSpeed, OldMoveSpeed);
}
void UAttributeSetBase::OnRep_DashPower(const FGameplayAttributeData& OldDashPower) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, DashPower, OldDashPower);
}
#pragma endregion

void UAttributeSetBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSetBase, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSetBase, MaxHealth, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSetBase, Shield, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSetBase, MaxShield, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSetBase, Attack, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSetBase, AttackSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSetBase, AttackPotency, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSetBase, HealthSteal, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSetBase, HealthStealPotency, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSetBase, ShieldSteal, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSetBase, ShieldStealPotency, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSetBase, AttackResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSetBase, StealResistance, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSetBase, MoveSpeed, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSetBase, DashPower, COND_None, REPNOTIFY_Always);
}

void UAttributeSetBase::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) {
	Super::PreAttributeChange(Attribute, NewValue);
	if (Attribute == GetAttackSpeedAttribute()) {
		NewValue = FMath::Clamp<float>(NewValue, 0.1f, 10.0f);
	}
}

void UAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) {
	Super::PostGameplayEffectExecute(Data);
	if (!Data.Target.IsValidLowLevel()) return;


	if (Data.EvaluatedData.Attribute == GetHealthAttribute()) {
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute()) {
		SetMaxHealth(FMath::Abs(GetMaxHealth()));
	}

	else if (Data.EvaluatedData.Attribute == GetShieldAttribute()) {
		SetShield(FMath::Clamp(GetShield(), 0.0f, GetMaxShield()));
	}
	else if (Data.EvaluatedData.Attribute == GetMaxShieldAttribute()) {
		SetMaxShield(FMath::Abs(GetMaxShield()));
	}

	if (Data.EvaluatedData.Attribute == GetAttackAttribute()) {
		SetAttack(FMath::Abs(GetAttack()));
	}
	else if (Data.EvaluatedData.Attribute == GetAttackSpeedAttribute()) {
		SetAttackSpeed(FMath::Clamp(GetAttackSpeed(), 0.1f, 10.0f));
	}
	else if (Data.EvaluatedData.Attribute == GetAttackPotencyAttribute()) {
		SetAttackPotency(FMath::Clamp(GetAttackPotency(), 0.0f, 10.0f));
	}

	else if (Data.EvaluatedData.Attribute == GetHealthStealAttribute()) {
		SetHealthSteal(FMath::Abs(GetHealthSteal()));
	}
	else if (Data.EvaluatedData.Attribute == GetHealthStealPotencyAttribute()) {
		SetHealthStealPotency(FMath::Clamp(GetHealthStealPotency(), 0.0f, 10.0f));
	}
	else if (Data.EvaluatedData.Attribute == GetShieldStealAttribute()) {
		SetShieldSteal(FMath::Abs(GetShieldSteal()));
	}
	else if (Data.EvaluatedData.Attribute == GetShieldStealPotencyAttribute()) {
		SetShieldStealPotency(FMath::Clamp(GetShieldStealPotency(), 0.0f, 10.0f));
	}

	else if (Data.EvaluatedData.Attribute == GetAttackResistanceAttribute()) {
		SetAttackResistance(FMath::Clamp(GetAttackResistance(), 0.0f, 10.0f));
	}
	else if (Data.EvaluatedData.Attribute == GetStealResistanceAttribute()) {
		SetStealResistance(FMath::Clamp(GetStealResistance(), 0.0f, 10.0f));
	}

	else if (Data.EvaluatedData.Attribute == GetMoveSpeedAttribute()) {
		SetMoveSpeed(FMath::Clamp(GetMoveSpeed(), 0.0f, 1800.0f));
	}
	else if (Data.EvaluatedData.Attribute == GetDashPowerAttribute()) {
		SetDashPower(FMath::Abs(GetDashPower()));
	}
}