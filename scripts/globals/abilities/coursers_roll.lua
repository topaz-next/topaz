-----------------------------------
-- Ability: Courser's Roll
-- Enhances "Snapshot" effect for party members within area of effect.
-- Optimal Job: None
-- Lucky Number: 3
-- Unlucky Number: 9
-- Level: 81
-- Phantom Roll +1 Value: 1
--
-- No Reliable Community Data available. Numbers Based on Blitzer's Roll Values.
--
-- Die Roll    | Snapshot+
-- --------    -------
-- 1           |+2
-- 2           |+3
-- 3           |+11
-- 4           |+4
-- 5           |+5
-- 6           |+6
-- 7           |+7
-- 8           |+8
-- 9           |+1
-- 10          |+10
-- 11          |+12
-- Bust        |-5
-----------------------------------
require("scripts/globals/job_utils/corsair")
-----------------------------------
local ability_object = {}

ability_object.onAbilityCheck = function(player, target, ability)
    return tpz.job_utils.corsair.onRollAbilityCheck(player, target, ability)
end

ability_object.onUseAbility = function(caster, target, ability, action)
    return tpz.job_utils.corsair.onRollUseAbility(caster, target, ability, action)
end

return ability_object
