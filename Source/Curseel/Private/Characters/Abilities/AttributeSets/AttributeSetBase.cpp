#include "Characters/Abilities/AttributeSets/AttributeSetBase.h"

void UAttributeSetBase::OnRep_Health(const FGameplayAttributeData& OldHealth) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, Health, OldHealth);
}
void UAttributeSetBase::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, MaxHealth, OldMaxHealth);
}
void UAttributeSetBase::OnRep_Damage(const FGameplayAttributeData& OldDamage) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, Damage, OldDamage);
}
void UAttributeSetBase::OnRep_DamageBuff(const FGameplayAttributeData& OldDamageBuff) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, DamageBuff, OldDamageBuff);
}

void UAttributeSetBase::OnRep_MoveSpeed(const FGameplayAttributeData& OldMoveSpeed) {
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAttributeSetBase, MoveSpeed, OldMoveSpeed);
}

void UAttributeSetBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSetBase, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSetBase, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSetBase, Damage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSetBase, DamageBuff, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAttributeSetBase, MoveSpeed, COND_None, REPNOTIFY_Always);
}

void UAttributeSetBase::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) {
	Super::PostGameplayEffectExecute(Data);

	//Clamp Attributes
	if (Data.EvaluatedData.Attribute == GetHealthAttribute()) {
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetDamageAttribute()) {
		SetDamage(FMath::Abs(GetDamage()));
	}
	else if (Data.EvaluatedData.Attribute == GetDamageBuffAttribute()) {
		SetDamageBuff(FMath::Abs(GetDamageBuff()));
	}
	else if (Data.EvaluatedData.Attribute == GetMoveSpeedAttribute()) {
		SetMoveSpeed(FMath::Abs(GetMoveSpeed()));
	}
}