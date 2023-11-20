#include "Characters/Abilities/CharacterASC.h"

void UCharacterASC::ReceiveDamage(UCharacterASC* SourceASC, float BaseDamage, float ModifiedDamage) {
	ReceivedDamage.Broadcast(SourceASC, BaseDamage, ModifiedDamage);
}