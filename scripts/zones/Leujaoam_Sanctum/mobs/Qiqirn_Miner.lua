-----------------------------------
-- Area: Leujaoam Sanctum
-- MOB: Qiqirn Miner
-- Assault: Orichalcum Survey
-----------------------------------
local ID = require("scripts/zones/Leujaoam_Sanctum/IDs")
require("scripts/globals/items")
require("scripts/globals/status")
-----------------------------------

function onMobSpawn(mob)
	mob:setStatus(dsp.status.NPC)
end

function onMobRoam(mob)
	instance = mob:getInstance()

	if instance:getStage() == 1 and mob:getLocalVar("Stage") == 0 then
		mob:setStatus(dsp.status.MOB)
		mob:setAnimation(1)
		mob:speed(100)
		mob:setAggressive(1)
		mob:setLocalVar("Stage", 1)
	end
end

function onTrigger(player, npc)
end

function onMobDeath(mob, player)
end