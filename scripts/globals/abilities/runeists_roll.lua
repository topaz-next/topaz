-----------------------------------
-- Ability: Runeist's Roll
-- Enhances magic evasion for party members within area of effect.
-- Optimal Job: Rune Fencer
-- Lucky Number: 4
-- Unlucky Number: 8
-- Level: 70
-- Phantom Roll +1 Value: 2
--
-- Die Roll    | MEVA+
-- --------    -------
-- 1           |+4
-- 2           |+6
-- 3           |+8
-- 4           |+25
-- 5           |+10
-- 6           |+12
-- 7           |+14
-- 8           |+2
-- 9           |+17
-- 10          |+20
-- 11          |+30
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
