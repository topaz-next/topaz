-----------------------------------
-- Ability: High Jump
-- Performs a high jumping attack on enemy.
-- Obtained: Dragoon Level 35
-- Recast Time: 2:00
-- Duration: Instant
-----------------------------------
require("scripts/globals/job_utils/dragoon")
-----------------------------------
local ability_object = {}

ability_object.onAbilityCheck = function(player, target, ability)
    return 0, 0
end

ability_object.onUseAbility = function(player, target, ability, action)
    return tpz.job_utils.dragoon.useHighJump(player, target, ability, action)
end

return ability_object
