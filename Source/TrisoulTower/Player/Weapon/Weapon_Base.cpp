#include "Weapon_Base.h"

TSubclassOf<UGameplayAbility> AWeapon_Base::GetAbility(int32 AbilityIndex)
{
	if (WeaponProperties.AbilitiesToGrant.IsValidIndex(AbilityIndex))
		return WeaponProperties.AbilitiesToGrant[AbilityIndex];
	return nullptr;
}
