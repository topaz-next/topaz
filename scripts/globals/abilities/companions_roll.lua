-----------------------------------
-- Ability: Companion's Roll
-- Grants "Regain" and "Regen" effects to pets of party members within area of effect.
-- Optimal Job: None
-- Lucky Number: 2
-- Unlucky Number: 10
-- Level: 95
-- Phantom Roll +1 Value: 5/2
--
-- Die Roll Logic in globals/effects/companions_roll.lua
--
-- Die Roll    | Regain | Regen
-- --------    -------   -------
-- 1           |+20     |+4
-- 2           |+50     |+20
-- 3           |+20     |+6
-- 4           |+20     |+8
-- 5           |+30     |+10
-- 6           |+30     |+12
-- 7           |+30     |+14
-- 8           |+40     |+16
-- 9           |+40     |+18
-- 10          |+10     |+3
-- 11          |+60     |+25
-- Bust        |-0      | 0
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
