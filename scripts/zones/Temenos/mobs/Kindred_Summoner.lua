-----------------------------------
-- Area: Temenos N T
--  Mob: Kindred Summoner
-----------------------------------
mixins = {require("scripts/mixins/job_special")}
local ID = require("scripts/zones/Temenos/IDs")
-----------------------------------
local entity = {}

entity.onMobEngaged = function(mob, target)
    GetMobByID(ID.mob.TEMENOS_N_MOB[4]):updateEnmity(target)
    GetMobByID(ID.mob.TEMENOS_N_MOB[4]+1):updateEnmity(target)
end

entity.onMobDeath = function(mob, player, isKiller, noKiller)
    if isKiller or noKiller then
        if GetMobByID(ID.mob.TEMENOS_N_MOB[4]):isDead() and GetMobByID(ID.mob.TEMENOS_N_MOB[4]+1):isDead() then
            GetNPCByID(ID.npc.TEMENOS_N_CRATE[4]):setStatus(tpz.status.NORMAL)
            GetNPCByID(ID.npc.TEMENOS_N_CRATE[4]+1):setStatus(tpz.status.NORMAL)
            GetNPCByID(ID.npc.TEMENOS_N_CRATE[4]+2):setStatus(tpz.status.NORMAL)
        end
    end
end

return entity
