-----------------------------------
-- Ability: Fly High
-- Description: Decreases the recast time of jumps.
-- Obtained: DRG Level 96
-- Recast Time: 01:00:00
-- Duration: 00:00:30
-----------------------------------
require("scripts/globals/job_utils/dragoon")
-----------------------------------
local ability_object = {}

ability_object.onAbilityCheck = function(player, target, ability)
    return 0, 0
end

ability_object.onUseAbility = function(player, target, ability)
    tpz.job_utils.dragoon.useFlyHigh(player, target, ability)
end

return ability_object
