-----------------------------------
-- Area: Leujaoam Sanctum
-----------------------------------
require("scripts/globals/missions")
require("scripts/globals/zone")
-----------------------------------

zones = zones or {}

zones[tpz.zone.LEUJAOAM_SANCTUM] =
{
    text =
    {
        ITEM_CANNOT_BE_OBTAINED    = 6383, -- You cannot obtain the <item>. Come back after sorting your inventory.
        FULL_INVENTORY_AFTER_TRADE = 6387, -- You cannot obtain the <item>. Try trading again after sorting your inventory.
        ITEM_OBTAINED              = 6389, -- Obtained: <item>.
        GIL_OBTAINED               = 6390, -- Obtained <number> gil.
        KEYITEM_OBTAINED           = 6392, -- Obtained key item: <keyitem>.
        KEYITEM_LOST               = 6393, -- Lost key item: <keyitem>.
        NOT_HAVE_ENOUGH_GIL        = 6394, -- You do not have enough gil.
        ITEMS_OBTAINED             = 6398, -- You obtain <number> <item>!
        CARRIED_OVER_POINTS        = 7000, -- You have carried over <number> login point[/s].
        LOGIN_CAMPAIGN_UNDERWAY    = 7001, -- The [/January/February/March/April/May/June/July/August/September/October/November/December] <number> Login Campaign is currently underway!<space>
        LOGIN_NUMBER               = 7002, -- In celebration of your most recent login (login no. <number>), we have provided you with <number> points! You currently have a total of <number> points.
        OBTAINED_TEMP_ITEM         = 7311, -- Obtained temporary item: <item>!
        PLAYER_OBTAINS_ITEM        = 7312, -- <name> obtains <item>!    
        ASSAULT_START_OFFSET       = 7447, -- Max MP Down removed for <player>
        TIME_TO_COMPLETE           = 7508, -- You have <number> [minute/minutes] (Earth time) to complete this mission.
        MISSION_FAILED             = 7509, -- The mission has failed. Leaving area.
        RUNE_UNLOCKED_POS          = 7510, -- Mission objective completed. Unlocking Rune of Release ([A/B/C/D/E/F/G/H/I/J/K/L/M/N/O/P/Q/R/S/T/U/V/W/X/Y/Z]-#).
        RUNE_UNLOCKED              = 7511, -- Mission objective completed. Unlocking Rune of Release.
        ASSAULT_POINTS_OBTAINED    = 7512, -- You gain <number> [Assault point/Assault points]!
        TIME_REMAINING_MINUTES     = 7513, -- Time remaining: <number> [minute/minutes] (Earth time).
        TIME_REMAINING_SECONDS     = 7514, -- Time remaining: <number> [second/seconds] (Earth time).
        PARTY_FALLEN               = 7516, -- All party members have fallen in battle. Mission failure in <number> [minute/minutes].
        MINE_FAIL                  = 7525, -- You find nothing.
        MINE_SUCCESS               = 7526, -- <name> finds <item>!
        OBTAINS_TEMP_ITEM          = 7527, -- <name> obtains the temporary item: <item>!
        BREAK_PICKAXE              = 7528, -- Your <item> (temporary item) breaks!
        MINE_NO_PICK               = 7529, -- Mining is possible here if you have <item> (temporary item).
        MINE_TOO_FAR               = 7530, -- You must move closer to the target.
        CANT_MINE                  = 7531, -- You can't mine here right now...
        MULWAHAH_FINISH            = 7533, -- You found some? Let's take a look then...
        MULWAHAH_TAKE_THIS         = 7537, -- Take this. If you come across <item>, bring it directly back to me.
    },

    mob =
    {
        [LEUJAOAM_CLEANSING] =
        {
            MOBS_START =
            {
                17059841, 17059842, 17059843, 17059844, 17059845, 17059846, 17059847, 17059848,
                17059849, 17059850, 17059851, 17059852, 17059853, 17059854, 17059855,
            },
        },
        [ORICHALCUM_SURVEY] =
        {
            MOBS_START =
            {
                17059856, 17059857, 17059858, 17059859, 17059860, 17059861, 17059862, 17059863,
            },
            MINING_POINTS =
            {
                17060016, 17060017, 17060018, 17060019, 17060020, 17060021, 17060022, 17060023, 17060024, 17060025,
            },
        },
    },

    npc =
    {
        ANCIENT_LOCKBOX = 17060014,
        RUNE_OF_RELEASE = 17060015,
        MULWAHAH        = 17060026,
    }
}

return zones[tpz.zone.LEUJAOAM_SANCTUM]
