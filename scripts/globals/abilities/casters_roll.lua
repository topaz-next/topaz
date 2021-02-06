-----------------------------------
-- Ability: Caster's Roll
-- Enhances "Fast Cast" effect for party members within area of effect.
-- Optimal Job: None
-- Lucky Number: 2
-- Unlucky Number: 7
-- Level: 79
-- Phantom Roll +1 Value: 3
--
-- Die Roll    | Fast Cast%
-- --------    -------
-- 1           |+6
-- 2           |+15
-- 3           |+7
-- 4           |+8
-- 5           |+9
-- 6           |+10
-- 7           |+5
-- 8           |+11
-- 9           |+12
-- 10          |+13
-- 11          |+20
-- Bust        |-10
-----------------------------------
local corsair = require("scripts/globals/job_utils/corsair")
-----------------------------------
local ability_object = {}

ability_object.onAbilityCheck = function(player, target, ability)
    return corsair.onRollAbilityCheck(player, target, ability)
end

ability_object.onUseAbility = function(caster, target, ability, action)
    return corsair.onRollUseAbility(caster, target, ability, action)
end

return ability_object
