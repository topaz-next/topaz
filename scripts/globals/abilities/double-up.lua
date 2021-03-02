-----------------------------------
-- Ability: Double Up
-- Enhances an active Phantom Roll effect that is eligible for Double-Up.
-- Obtained: Corsair Level 5
-- Recast Time: 8 seconds
-- Duration: Instant
-----------------------------------
require("scripts/globals/job_utils/corsair")
-----------------------------------
local ability_object = {}

ability_object.onAbilityCheck = function(player, target, ability)
    return tpz.job_utils.corsair.onDoubleUpAbilityCheck(player, target, ability)
end

ability_object.onUseAbility = function(caster, target, ability, action)
    return tpz.job_utils.corsair.useDoubleUp(caster, target, ability, action)
end

return ability_object
