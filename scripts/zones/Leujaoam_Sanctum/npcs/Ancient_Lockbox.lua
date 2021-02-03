-----------------------------------
-- Area: Leujaoam Sanctum
-- Ancient Lockbox
-----------------------------------
local ID = require("scripts/zones/Leujaoam_Sanctum/IDs")
require("scripts/globals/items")
require("scripts/globals/missions")
require("scripts/globals/utils/appraisal")
-----------------------------------

function onTrigger(player,npc)
    local qItem =
    {
        [LEUJAOAM_CLEANSING] =
        {
            {
                {itemid = dsp.items.APPRAISAL_RING, droprate = 600},
                {itemid = dsp.items.APPRAISAL_BOX, droprate = 300},
                {itemid = 0, droprate = 100}, -- Nothing
            },
        },
        [ORICHALCUM_SURVEY] =
        {
            {
                {itemid = dsp.items.APPRAISAL_NECKLACE, droprate = 300},
                {itemid = dsp.items.APPRAISAL_BOX, droprate = 550},
                {itemid = dsp.items.APPRAISAL_GLOVES, droprate = 50},
                {itemid = 0, droprate = 100}, -- Nothing
            },
        },
    }
    local regItem =
    {
        [LEUJAOAM_CLEANSING] =
        {
            {
                {itemid = dsp.items.HI_POTION_III, droprate = 1000},
            },
            {
                {itemid = dsp.items.HI_POTION_III, droprate = 100},
                {itemid = 0, droprate = 900},
            },
            {
                {itemid = dsp.items.REMEDY, droprate = 530}, -- Remedy
                {itemid = 0, droprate = 470},
            },
        },
        [ORICHALCUM_SURVEY] =
        {
            {
                {itemid = dsp.items.HI_POTION_III, droprate = 1000},
            },
            {
                {itemid = dsp.items.REMEDY, droprate = 530},
                {itemid = 0, droprate = 470},
            },
        },
    }
    local area = player:getCurrentAssault()
    appraisalUtil.assaultChestTrigger(player, npc, qItem[area], regItem[area], ID.text)
end

function onEventUpdate(player,csid,option)
end

function onEventFinish(player,csid,option)
end
