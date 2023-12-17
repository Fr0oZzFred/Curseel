#include "GEC_Steal.h"

struct FSourceStealCapture {
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(StealResistance);

	FSourceStealCapture() {
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, AttackResistance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, StealResistance, Source, false);
	}
};
struct FTargetStealCapture {
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Shield);

	DECLARE_ATTRIBUTE_CAPTUREDEF(HealthSteal);
	DECLARE_ATTRIBUTE_CAPTUREDEF(HealthStealPotency);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ShieldSteal);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ShieldStealPotency);

	FTargetStealCapture() {
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, Health, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, Shield, Target, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, HealthSteal, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, HealthStealPotency, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, ShieldSteal, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, ShieldStealPotency, Target, false);
	}
};

static const FSourceStealCapture& SourceStealCapture() {
	static FSourceStealCapture SourceStealCapture;
	return SourceStealCapture;
}
static const FTargetStealCapture& TargetStealCapture() {
	static FTargetStealCapture TargetStealCapture;
	return TargetStealCapture;
}

UGEC_Steal::UGEC_Steal() {
	RelevantAttributesToCapture.Add(FSourceStealCapture().AttackResistanceDef);
	RelevantAttributesToCapture.Add(FSourceStealCapture().StealResistanceDef);


	RelevantAttributesToCapture.Add(FTargetStealCapture().HealthDef);
	RelevantAttributesToCapture.Add(FTargetStealCapture().ShieldDef);
	RelevantAttributesToCapture.Add(FTargetStealCapture().HealthStealDef);
	RelevantAttributesToCapture.Add(FTargetStealCapture().HealthStealPotencyDef);
	RelevantAttributesToCapture.Add(FTargetStealCapture().ShieldStealDef);
	RelevantAttributesToCapture.Add(FTargetStealCapture().ShieldStealPotencyDef);
}

//Source and Target is inversed since we can only exec on target
void UGEC_Steal::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const {
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

	//Source and Target is inversed since we can only exec on target

	//Target
	float HealthSteal = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		FTargetStealCapture().HealthStealDef, EvaluationParameters, HealthSteal);
	float HealthStealPotency = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		FTargetStealCapture().HealthStealPotencyDef, EvaluationParameters, HealthStealPotency);
	float ShieldSteal = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		FTargetStealCapture().ShieldStealDef, EvaluationParameters, ShieldSteal);
	float ShieldStealPotency = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		FTargetStealCapture().ShieldStealPotencyDef, EvaluationParameters, ShieldStealPotency);

	//Source
	float AttackResistance = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		FSourceStealCapture().AttackResistanceDef, EvaluationParameters, AttackResistance);
	float StealResistance = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		FSourceStealCapture().StealResistanceDef, EvaluationParameters, StealResistance);

	//Damage Boost
	float SourceHealthSteal = HealthSteal * HealthStealPotency;
	float SourceShieldSteal = ShieldSteal * ShieldStealPotency;

	//Mitigate Steal
	SourceHealthSteal *= StealResistance;
	SourceShieldSteal *= StealResistance;

	//Apply
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
		FTargetStealCapture().HealthProperty, EGameplayModOp::Additive, SourceHealthSteal));
	
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
		FTargetStealCapture().ShieldProperty, EGameplayModOp::Additive, SourceShieldSteal));

}