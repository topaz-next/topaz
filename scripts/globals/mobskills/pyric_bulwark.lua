-----------------------------------
-- Pyric Bulwark
--
-- Description: Grants a Physical Shield effect for a time.
-- Type: Enhancing
--
-- Range: Self
-----------------------------------
require("scripts/globals/monstertpmoves")
require("scripts/globals/settings")
require("scripts/globals/status")
require("scripts/globals/msg")
-----------------------------------
local mobskill_object = {}

mobskill_object.onMobSkillCheck = function(target, mob, skill)

  if (mob:getFamily() == 316) then
    local mobSkin = mob:getModelId()

    if (mobSkin == 1796) then
        return 0
    else
        return 1
    end
  end
   -- TODO: Used only when second/left head is alive (animationsub 0 or 1)
   if (mob:getAnimationSub() <= 1) then
      return 0
   else
      return 1
   end
end

mobskill_object.onMobWeaponSkill = function(target, mob, skill)

    -- addEx to pervent dispel
    mob:addStatusEffectEx(tpz.effect.PHYSICAL_SHIELD, 0, 1, 0, 45)
    skill:setMsg(tpz.msg.basic.SKILL_GAIN_EFFECT)
    if (mob:getFamily() == 313) then -- Tinnin follows this up immediately with Nerve Gas
        mob:useMobAbility(1580)
    end

    return tpz.effect.PHYSICAL_SHIELD
end

return mobskill_object
