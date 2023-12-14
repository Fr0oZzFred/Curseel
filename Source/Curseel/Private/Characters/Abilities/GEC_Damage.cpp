#include "Characters/Abilities/GEC_Damage.h"

struct FDamageStatics {
	//DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);
	//DECLARE_ATTRIBUTE_CAPTUREDEF(DamageBuff);
	//DECLARE_ATTRIBUTE_CAPTUREDEF(DamageReduction);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Health);

	FDamageStatics() {
		//DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, Damage, Source, false);
		//DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, DamageBuff, Source, false);
		//DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, DamageReduction, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAttributeSetBase, Health, Source, false);
	}
};

static const FDamageStatics& DamageStatics() {
	static FDamageStatics DStatics;
	return DStatics;
}

UGEC_Damage::UGEC_Damage() {
	//RelevantAttributesToCapture.Add(DamageStatics().DamageDef);
	//RelevantAttributesToCapture.Add(DamageStatics().DamageBuffDef);
	//RelevantAttributesToCapture.Add(DamageStatics().DamageReductionDef);
	RelevantAttributesToCapture.Add(DamageStatics().HealthDef);
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



	float Damage = 0.0f;
	//ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageDef, EvaluationParameters, Damage);
	
	
	Damage += FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), false, -1.0f), 0.0f);


	float DamageBuff = 0.0f;
	//ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageBuffDef, EvaluationParameters, DamageBuff);

	float DamageReduction = 0.0f;
	//ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageReductionDef, EvaluationParameters, DamageReduction);

	float BaseDamage = Damage;

	float ModifiedDamage = BaseDamage * DamageBuff * DamageReduction;

	if (ModifiedDamage < 0.0f) ModifiedDamage = 0.0f;

	ModifiedDamage = 100.0f;
	// Set the Target's damage meta attribute
	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().HealthProperty, EGameplayModOp::Additive, -ModifiedDamage));
}