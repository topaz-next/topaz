-----------------------------------
-- Area: Ranguemont Pass
--   NM: Taisaijin
-----------------------------------
local ID = require("scripts/zones/Ranguemont_Pass/IDs")
require("scripts/globals/titles")
-----------------------------------
local entity = {}

entity.onMobDeath = function(mob, player, isKiller)
    player:addTitle(tpz.title.BYEBYE_TAISAI)
end

entity.onMobDespawn = function(mob)
    local phIndex = mob:getLocalVar("phIndex")
    local ph = GetMobByID(ID.mob.TAISAIJIN_PH[phIndex])

    -- allow current placeholder to respawn
    DisallowRespawn(mob:getID(), true)
    DisallowRespawn(ph:getID(), false)
    ph:setRespawnTime(GetMobRespawnTime(ph:getID()))

    -- pick next placeholder
    phIndex = (phIndex % 3) + 1
    ph = GetMobByID(ID.mob.TAISAIJIN_PH[phIndex])
    ph:setLocalVar("timeToGrow", os.time() + math.random(86400, 259200)) -- 1 to 3 days
    ph:setLocalVar("phIndex", phIndex)
end

return entity
