#include "GameplayEffectCalculations/GEC_HealthSteal.h"

struct FSourceHealthStealCapture {
	DECLARE_ATTRIBUTE_CAPTUREDEF(StealResistance);

	FSourceHealthStealCapture() {
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, StealResistance, Source, false);
	}
};
struct FTargetHealthStealCapture {
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);
	DECLARE_ATTRIBUTE_CAPTUREDEF(HealthSteal);
	DECLARE_ATTRIBUTE_CAPTUREDEF(HealthStealPotency);

	FTargetHealthStealCapture() {
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, Health, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, HealthSteal, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, HealthStealPotency, Target, false);
	}
};

static const FSourceHealthStealCapture& SourceHealthStealCapture() {
	static FSourceHealthStealCapture SourceHealthStealCapture;
	return SourceHealthStealCapture;
}
static const FTargetHealthStealCapture& TargetHealthStealCapture() {
	static FTargetHealthStealCapture TargetHealthStealCapture;
	return TargetHealthStealCapture;
}

UGEC_HealthSteal::UGEC_HealthSteal() {
	RelevantAttributesToCapture.Add(FSourceHealthStealCapture().StealResistanceDef);

	RelevantAttributesToCapture.Add(FTargetHealthStealCapture().HealthDef);
	RelevantAttributesToCapture.Add(FTargetHealthStealCapture().HealthStealDef);
	RelevantAttributesToCapture.Add(FTargetHealthStealCapture().HealthStealPotencyDef);
}

//Source and Target is inversed since we can only exec on target
void UGEC_HealthSteal::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const {
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
		FTargetHealthStealCapture().HealthStealDef, EvaluationParameters, HealthSteal);
	HealthSteal += FMath::Max<float>(Spec.GetSetByCallerMagnitude(
		FGameplayTag::RequestGameplayTag(FName("Data.HealthSteal")), false, -1.0f), 0.0f);

	float HealthStealPotency = 1.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		FTargetHealthStealCapture().HealthStealPotencyDef, EvaluationParameters, HealthStealPotency);

	//Source
	float StealResistance = 1.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		FSourceHealthStealCapture().StealResistanceDef, EvaluationParameters, StealResistance);

	//Damage Boost
	float SourceHealthSteal = HealthSteal * HealthStealPotency;

	//Mitigate Steal
	SourceHealthSteal *= StealResistance;

	//Apply
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
		FTargetHealthStealCapture().HealthProperty, EGameplayModOp::Additive, SourceHealthSteal));

}
