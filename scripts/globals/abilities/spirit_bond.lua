-----------------------------------
-- Ability: Spirit Bond
-- Description: Enables the dragoon to take some damage on behalf of their wyvern. Using Healing Breath also restores the wyvern's HP.
-- Obtained: DRG Level 65
-- Recast Time: 00:03:00
-- Duration: 00:01:00
-----------------------------------
require("scripts/globals/job_utils/dragoon")
-----------------------------------
local ability_object = {}

ability_object.onAbilityCheck = function(player, target, ability)
    return 0, 0
end

ability_object.onUseAbility = function(player, target, ability)
    tpz.job_utils.dragoon.useSpiritBond(player, target, ability)
end

return ability_object
