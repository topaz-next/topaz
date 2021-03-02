-----------------------------------
-- Area: Windurst Walls
--  NPC: Hiwon-Biwon
-- Involved In Quest: Making Headlines, Curses, Foiled...Again!?
-- Working 100%
-----------------------------------
require("scripts/globals/keyitems")
require("scripts/globals/npc_util")
require("scripts/globals/quests")
require("scripts/globals/utils")
-----------------------------------
local entity = {}

entity.onTrade = function(player, npc, trade)
end

entity.onTrigger = function(player, npc)
    local makingHeadlines = player:getQuestStatus(tpz.quest.log_id.WINDURST, tpz.quest.id.windurst.MAKING_HEADLINES)
    local cursesFoiledAgain1 = player:getQuestStatus(tpz.quest.log_id.WINDURST, tpz.quest.id.windurst.CURSES_FOILED_AGAIN_1)
    local cursesFoiledAgain2 = player:getQuestStatus(tpz.quest.log_id.WINDURST, tpz.quest.id.windurst.CURSES_FOILED_AGAIN_2)

    -- Curses, Foiled ... Again!?
    if cursesFoiledAgain2 == QUEST_ACCEPTED and not player:hasItem(552) then
        player:startEvent(182) -- get Hiwon's hair

    -- Making Headlines
    elseif makingHeadlines == QUEST_ACCEPTED then
        -- bitmask of progress: 0 = Kyume-Romeh, 1 = Yuyuju, 2 = Hiwom-Gomoi, 3 = Umumu, 4 = Mahogany Door
        local prog = player:getCharVar("QuestMakingHeadlines_var")

        if not utils.mask.getBit(prog, 2) then
            if cursesFoiledAgain1 == QUEST_ACCEPTED then
                if math.random(1, 2) == 1 then
                    player:startEvent(283) -- Give scoop while sick
                else
                    player:startEvent(284) -- Give scoop while sick
                end
            else
                player:startEvent(281) -- Give scoop
            end
        else
            player:startEvent(282) -- "Getting back to the maater at hand-wand..."
        end

    -- dialog after CFA2
    elseif cursesFoiledAgain2 == QUEST_COMPLETED then
        player:startEvent(185)

    -- default dialog
    else
        local rand = math.random(1, 5)

        if rand == 1 then
            player:startEvent(305)
        elseif rand == 2 then
            player:startEvent(306)
        elseif rand == 3 then
            player:startEvent(168)
        elseif rand == 4 then
            player:startEvent(170)
        elseif rand == 5 then
            player:startEvent(169)
        end
    end
end

entity.onEventUpdate = function(player, csid, option)

end

entity.onEventFinish = function(player, csid, option)
    -- Making Headlines
    if csid == 281 or csid == 283 or csid == 284 then
        npcUtil.giveKeyItem(player, tpz.ki.WINDURST_WALLS_SCOOP)
        player:setCharVar("QuestMakingHeadlines_var", utils.mask.setBit(player:getCharVar("QuestMakingHeadlines_var"), 2, true))

    -- Curses, Foiled...Again!?
    elseif csid == 182 then
        npcUtil.giveItem(player, 552)
    end
end

return entity
