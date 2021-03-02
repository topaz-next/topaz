-----------------------------------
-- Ability: Miser's Roll
-- Grants "Save TP" effect to party members within area of effect.
-- Optimal Job: None
-- Lucky Number: 5
-- Unlucky Number: 7
-- Level: 92
-- Phantom Roll +1 Value: 15
--
-- Die Roll    | Skillchain Bonus
-- --------    -------
-- 1           |+30
-- 2           |+50
-- 3           |+70
-- 4           |+90
-- 5           |+200
-- 6           |+110
-- 7           |+20
-- 8           |+130
-- 9           |+150
-- 10          |+170
-- 11          |+250
-- Bust        |-0
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
