-----------------------------------
-- Ability: Healer's Roll
-- Increases potency of "Cure" effect received for party members within area of effect
-- Optimal Job: White Mage
-- Lucky Number: 3
-- Unlucky Number: 7
-- Level: 20
-- Phantom Roll +1 Value: 3
--
-- Die Roll    |No WHM  |With WHM
-- --------    -------  -----------
-- 1           |+3%     |+7%
-- 2           |+4%     |+8%
-- 3           |+12%    |+16%
-- 4           |+5%     |+9%
-- 5           |+6%     |+10%
-- 6           |+7%     |+11%
-- 7           |+1%     |+5%
-- 8           |+8%     |+12%
-- 9           |+9%     |+13%
-- 10          |+10%    |+14%
-- 11          |+16%    |+20%
-- Bust        |-4%     |-4%
--
-- Note that this roll will increase potency of cures received, not the potency of the caster's spells
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
