-----------------------------------
-- Ability: Spirit Jump
-- Description: Delivers a short jumping attack on a targeted enemy which suppresses enmity. Effect enhanced when wyvern is present.
-- Obtained: DRG Level 77
-- Recast Time: 1:00
-----------------------------------
require("scripts/globals/job_utils/dragoon")
-----------------------------------
local ability_object = {}

ability_object.onAbilityCheck = function(player, target, ability)
    return 0, 0
end

ability_object.onUseAbility = function(player, target, ability, action)
    return tpz.job_utils.dragoon.useSpiritJump(player, target, ability, action)
end

return ability_object
