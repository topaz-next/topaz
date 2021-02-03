-----------------------------------
-- Area: Lebros Cavern (Excavation Duty)
-- MOB: Qiqirn Mine
-----------------------------------
local ID = require("scripts/zones/Lebros_Cavern/IDs")
require("scripts/globals/allyassist")
require("scripts/globals/missions")
require("scripts/globals/status")
-----------------------------------

function onMobSpawn(mob)
    local instance = mob:getInstance()
    local players = instance:getChars()
    mob:setMobMod(dsp.mobMod.NO_MOVE, 1)
    mob:SetAutoAttackEnabled(false)
    
    for i,v in pairs(players) do
        v:messageSpecial(ID.text.MINE_COUNTDOWN,10)
        mob:timer(5000, function(mob) v:messageSpecial(ID.text.MINE_COUNTDOWN,5) end)
        mob:timer(6000, function(mob) v:messageSpecial(ID.text.MINE_COUNTDOWN,4) end)
        mob:timer(7000, function(mob) v:messageSpecial(ID.text.MINE_COUNTDOWN,3) end)
        mob:timer(8000, function(mob) v:messageSpecial(ID.text.MINE_COUNTDOWN,2) end)
        mob:timer(9000, function(mob) v:messageSpecial(ID.text.MINE_COUNTDOWN,1) mob:useMobAbility(1838) end)
        mob:timer(11000, function(mob) mob:setHP(0) end)
    end
end

function onMobDeath(mob, player, isKiller)
end
