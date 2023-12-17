#include "GameplayEffectCalculations/GEC_ShieldSteal.h"


struct FSourceShieldStealCapture {
	DECLARE_ATTRIBUTE_CAPTUREDEF(StealResistance);

	FSourceShieldStealCapture() {
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, StealResistance, Source, false);
	}
};
struct FTargetShieldStealCapture {
	DECLARE_ATTRIBUTE_CAPTUREDEF(Shield);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ShieldSteal);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ShieldStealPotency);

	FTargetShieldStealCapture() {
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, Shield, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, ShieldSteal, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, ShieldStealPotency, Target, false);
	}
};

static const FSourceShieldStealCapture& SourceShieldStealCapture() {
	static FSourceShieldStealCapture SourceShieldStealCapture;
	return SourceShieldStealCapture;
}
static const FTargetShieldStealCapture& TargetShieldStealCapture() {
	static FTargetShieldStealCapture TargetShieldStealCapture;
	return TargetShieldStealCapture;
}

UGEC_ShieldSteal::UGEC_ShieldSteal() {
	RelevantAttributesToCapture.Add(FSourceShieldStealCapture().StealResistanceDef);

	RelevantAttributesToCapture.Add(FTargetShieldStealCapture().ShieldDef);
	RelevantAttributesToCapture.Add(FTargetShieldStealCapture().ShieldStealDef);
	RelevantAttributesToCapture.Add(FTargetShieldStealCapture().ShieldStealPotencyDef);
}

//Source and Target is inversed since we can only exec on target
void UGEC_ShieldSteal::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const {
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
	float ShieldSteal = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		FTargetShieldStealCapture().ShieldStealDef, EvaluationParameters, ShieldSteal);
	ShieldSteal += FMath::Max<float>(Spec.GetSetByCallerMagnitude(
		FGameplayTag::RequestGameplayTag(FName("Data.ShieldSteal")), false, -1.0f), 0.0f);

	float ShieldStealPotency = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		FTargetShieldStealCapture().ShieldStealPotencyDef, EvaluationParameters, ShieldStealPotency);

	//Source
	float StealResistance = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		FSourceShieldStealCapture().StealResistanceDef, EvaluationParameters, StealResistance);

	//Damage Boost
	float SourceShieldSteal = ShieldSteal * ShieldStealPotency;

	//Mitigate Steal
	SourceShieldSteal *= StealResistance;

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
		FTargetShieldStealCapture().ShieldProperty, EGameplayModOp::Additive, SourceShieldSteal));

}