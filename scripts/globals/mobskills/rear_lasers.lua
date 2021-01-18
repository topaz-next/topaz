-----------------------------------
-- Rear Lasers
-- Fires aft lasers at players behind user. Additional effects: Petrification
-----------------------------------
require("scripts/globals/settings")
require("scripts/globals/status")
require("scripts/globals/monstertpmoves")
-----------------------------------
local mobskill_object = {}

mobskill_object.onMobSkillCheck = function(target,mob,skill)
    -- skillList  54 = Omega
    -- skillList 727 = Proto-Omega
    -- skillList 728 = Ultima
    -- skillList 729 = Proto-Ultima
    local skillList = mob:getMobMod(tpz.mobMod.SKILL_LIST)

    if (skillList == 54 or (skillList == 727 and mob:getAnimationSub() == 1)) and target:isBehind(mob) then
        return 0
    end
    return 1
end

mobskill_object.onMobWeaponSkill = function(target, mob, skill)
    local typeEffect = tpz.effect.PETRIFICATION

    skill:setMsg(MobStatusEffectMove(mob, target, typeEffect, 1, 0, 30))

    return typeEffect
end
return mobskill_object
