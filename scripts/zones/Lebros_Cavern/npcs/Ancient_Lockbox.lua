-----------------------------------
-- Area: Lebros Cavern
-- Ancient Lockbox
-----------------------------------
local ID = require("scripts/zones/Lebros_Cavern/IDs")
require("scripts/globals/items")
require("scripts/globals/missions")
require("scripts/globals/status")
require("scripts/globals/appraisal")
-----------------------------------

function onTrigger(player,npc)
    local qItem =
    {
        [EXCAVATION_DUTY] =
        {
            {
                {itemid = dsp.items.APPRAISAL_BOX, droprate = dsp.frequency.SUPER_COMMON},
                {itemid = dsp.items.APPRAISAL_EARRING, droprate = dsp.frequency.COMMON},
                {itemid = 0, droprate = dsp.frequency.VERY_RARE},
            },
        },
        [LEBROS_SUPPLIES] =
        {
            {
                {itemid = dsp.items.APPRAISAL_BOX, droprate = dsp.frequency.SUPER_COMMON},
                {itemid = dsp.items.APPRAISAL_CAPE, droprate = dsp.frequency.COMMON},
                {itemid = 0, droprate = dsp.frequency.VERY_RARE},
            },
        },
    }
    local regItem =
    {
        [EXCAVATION_DUTY] =
        {
            {
                {itemid = dsp.items.REMEDY, droprate = dsp.frequency.SUPER_COMMON},
                {itemid = 0, droprate = dsp.frequency.RARE},
            },
            {
                {itemid = dsp.items.REMEDY, droprate = dsp.frequency.RARE},
                {itemid = 0, droprate = dsp.frequency.SUPER_COMMON},
            },
            {
                {itemid = 4119, droprate = dsp.frequency.COMMON},
                {itemid = 0, droprate = dsp.frequency.COMMON},
            },
            {
                {itemid = 4119, droprate = 200},
                {itemid = 0, droprate = dsp.frequency.SUPER_COMMON},
            },
        },
        [LEBROS_SUPPLIES] =
        {
            {
                {itemid = dsp.items.REMEDY, droprate = dsp.frequency.SUPER_COMMON},
                {itemid = 0, droprate = dsp.frequency.RARE},
            },
            {
                {itemid = dsp.items.RERAISER, droprate = dsp.frequency.RARE},
                {itemid = 0, droprate = dsp.frequency.SUPER_COMMON},
            },
            {
                {itemid = dsp.items.HI_POTION_TANK, droprate = dsp.frequency.RARE},
                {itemid = 0, droprate = dsp.frequency.SUPER_COMMON},
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
