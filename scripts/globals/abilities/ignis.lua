-----------------------------------
require("scripts/globals/status")
-----------------------------------

function onAbilityCheck(player, target, ability)
    return 0, 0
end

function onUseAbility(player, target, ability)
       local effect = tpz.effect.ENFIRE
    local magicskill = target:getSkillLevel(tpz.skill.ENHANCING_MAGIC)
    local potency = (magicskill / 8) + 12.5

    if target:addStatusEffect(effect, potency, 0, 500) then
        
  end
end