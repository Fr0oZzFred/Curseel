#include "GameplayEffectCalculations/GEC_Damage.h"

struct FSourceCapture {
	DECLARE_ATTRIBUTE_CAPTUREDEF(Attack);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPotency);

	FSourceCapture() {
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, Attack, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, AttackPotency, Source, false);
	}
};
struct FTargetCapture {
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Shield);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackResistance);

	FTargetCapture() {
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, Health, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, Shield, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, AttackResistance, Target, false);
	}
};

static const FSourceCapture& SourceCapture() {
	static FSourceCapture SourceCapture;
	return SourceCapture;
}
static const FTargetCapture& TargetCapture() {
	static FTargetCapture TargetCapture;
	return TargetCapture;
}

UGEC_Damage::UGEC_Damage() {
	RelevantAttributesToCapture.Add(FSourceCapture().AttackDef);
	RelevantAttributesToCapture.Add(FSourceCapture().AttackPotencyDef);
	
	
	RelevantAttributesToCapture.Add(FTargetCapture().HealthDef);
	RelevantAttributesToCapture.Add(FTargetCapture().ShieldDef);
	RelevantAttributesToCapture.Add(FTargetCapture().AttackResistanceDef);
}


void UGEC_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const {
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	
	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor() : nullptr;
	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor() : nullptr;
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	
	// Gather the tags from the source and target as that can affect which buffs should be used
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	
	//Source
	float Attack = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		FSourceCapture().AttackDef, EvaluationParameters, Attack);
	Attack += FMath::Max<float>(Spec.GetSetByCallerMagnitude(
		FGameplayTag::RequestGameplayTag(FName("Data.Damage")), false, -1.0f), 0.0f);

	float AttackPotency = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		FSourceCapture().AttackPotencyDef, EvaluationParameters, AttackPotency);
	
	//Target
	float Health = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		FTargetCapture().HealthDef, EvaluationParameters, Health);
	float Shield = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		FTargetCapture().ShieldDef, EvaluationParameters, Shield);
	
	float AttackResistance = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		FTargetCapture().AttackResistanceDef, EvaluationParameters, AttackResistance);
	
	//Boosted Damage
	float SourceDamage = Attack * AttackPotency;
	
	//Mitigated Damage
	SourceDamage *= AttackResistance;
	
	
	//Damage
	float Damage = Shield - SourceDamage;

	//Damage Health
	if (Damage < 0.0f) {
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
			FTargetCapture().HealthProperty, EGameplayModOp::Additive, Damage));
	
		//Damage Shield
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
			FTargetCapture().ShieldProperty, EGameplayModOp::Additive, -Shield));
	} else {
		//Damage Shield
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
			FTargetCapture().ShieldProperty, EGameplayModOp::Additive, -SourceDamage));
	}
}