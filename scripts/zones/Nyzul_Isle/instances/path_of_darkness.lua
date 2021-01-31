-----------------------------------
-- TOAU-42: Path of Darkness (Instance 58)
-----------------------------------
local ID = require("scripts/zones/Nyzul_Isle/IDs")
require("scripts/globals/instance")
require("scripts/globals/keyitems")
-----------------------------------
local instance_object = {}

-- Called on the instance, once it is created and ready
instance_object.onInstanceCreated = function(instance)
    print("onInstanceCreated")
    SpawnMob(ID.mob[58].AMNAF_BLU, instance)
    SpawnMob(ID.mob[58].NAJA, instance)
end

-- Once the instance is ready, inform the requester that it's ready (teleport them)
instance_object.onInstanceCreatedCallback = function(player, instance)
    print("onInstanceCreatedCallback")

    -- Send
    player:setInstance(instance)
    player:setPos(0, 0, 0, 0, instance:getZone():getID())
end

instance_object.afterInstanceRegister = function(player)
    print("afterInstanceRegister")
    local instance = player:getInstance()
    player:messageSpecial(ID.text.TIME_TO_COMPLETE, instance:getTimeLimit())
end

instance_object.onInstanceTimeUpdate = function(instance, elapsed)
    print("onInstanceTimeUpdate")
    updateInstanceTime(instance, elapsed, ID.text)
end

instance_object.onInstanceFailure = function(instance)
    print("onInstanceFailure")
    local chars = instance:getChars()

    for i, v in pairs(chars) do
        v:messageSpecial(ID.text.MISSION_FAILED, 10, 10)
        v:startEvent(1)
    end
end

instance_object.onInstanceProgressUpdate = function(instance, progress)
    print("onInstanceProgressUpdate")
    if(progress >= 10 and progress < 20) then
        DespawnMob(ID.mob[58].AMNAF_BLU, instance)
    elseif(progress == 24) then
        local v = GetMobByID(ID.mob[58].NAJA, instance)
        v:setLocalVar("ready", 0)
        v:setLocalVar("Stage", 2)

        SpawnMob(ID.mob[58].AMNAF_BLU, instance)
    elseif(progress >= 30 and progress < 40) then
        DespawnMob(ID.mob[58].AMNAF_BLU, instance)
    elseif(progress == 48) then
        SpawnMob(ID.mob[58].AMNAF_PSYCHEFLAYER, instance)

        local v = GetMobByID(ID.mob[58].NAJA, instance)
        v:setLocalVar("ready", 0)
        v:setLocalVar("Stage", 3)

        local npcs = instance:getNpcs()

        for i, v in pairs(npcs) do
            if(v:getID() == ID.npc._259) then
                v:setAnimation(8)
            end
        end
    elseif(progress == 50) then
        instance:complete()
    end
end

instance_object.onInstanceComplete = function(instance)
    print("onInstanceComplete")

    local chars = instance:getChars()

    for i, v in pairs(chars) do
        if (v:getCurrentMission(TOAU) == tpz.mission.id.toau.PATH_OF_DARKNESS and v:getCharVar("AhtUrganStatus") == 1) then
            v:setCharVar("AhtUrganStatus", 2)
        end

        v:setPos(0, 0, 0, 0, 72)
    end
end

instance_object.onEventUpdate = function(player, csid, option)
    print("onEventUpdate")
end

instance_object.onEventFinish = function(player, csid, option)
    print("onEventFinish")
    if csid == 1 then
        player:setPos(0, 0, 0, 0, 72)
    end
end

return instance_object
