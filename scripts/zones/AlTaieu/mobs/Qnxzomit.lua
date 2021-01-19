-----------------------------------
-- Area: Al'Taieu
--  Mob: Qn'xzomit
-- Note: Pet for JOL and JOJ
-----------------------------------
mixins = {require("scripts/mixins/job_special")}
local ID = require("scripts/zones/AlTaieu/IDs")
-----------------------------------
local entity = {}

entity.onMobDeath = function(mob, player, isKiller)
end

entity.onMobDespawn = function(mob)
    if (mob:getID() > ID.mob.JAILER_OF_LOVE) then
        local JoL = GetMobByID(ID.mob.JAILER_OF_LOVE)
        local xzomitsKilled = JoL:getLocalVar("JoL_Qn_xzomit_Killed")
        JoL:setLocalVar("JoL_Qn_xzomit_Killed", xzomitsKilled + 1)
    end
end

return entity
