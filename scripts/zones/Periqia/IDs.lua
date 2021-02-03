-----------------------------------
-- Area: Periqia
-----------------------------------
require("scripts/globals/missions")
require("scripts/globals/zone")
-----------------------------------

zones = zones or {}

zones[dsp.zone.PERIQIA] =
{
    text = {
        ITEM_CANNOT_BE_OBTAINED    = 6382, -- You cannot obtain the <item>. Come back after sorting your inventory.
        FULL_INVENTORY_AFTER_TRADE = 6386, -- You cannot obtain the <item>. Try trading again after sorting your inventory.
        ITEM_OBTAINED              = 6388, -- Obtained: <item>.
        GIL_OBTAINED               = 6389, -- Obtained <number> gil.
        KEYITEM_OBTAINED           = 6391, -- Obtained key item: <keyitem>.
        KEYITEM_LOST               = 6392, -- Lost key item: <keyitem>.
        NOT_HAVE_ENOUGH_GIL        = 6393, -- You do not have enough gil.
        ITEMS_OBTAINED             = 6397, -- You obtain <number> <item>!
        PLAYER_OBTAINS_ITEM        = 7311, -- <player> obtains a <item>!
        ASSAULT_START_OFFSET       = 7446, -- Max MP Down removed for <player>
        TIME_TO_COMPLETE           = 7507, -- You have <number> [minute/minutes] (Earth time) to complete this mission.
        MISSION_FAILED             = 7508, -- The mission has failed. Leaving area.
        RUNE_UNLOCKED_POS          = 7509, -- ission objective completed. Unlocking Rune of Release ([A/B/C/D/E/F/G/H/I/J/K/L/M/N/O/P/Q/R/S/T/U/V/W/X/Y/Z]-#).
        RUNE_UNLOCKED              = 7510, -- ission objective completed. Unlocking Rune of Release.
        ASSAULT_POINTS_OBTAINED    = 7511, -- You gain <number> [Assault point/Assault points]!
        TIME_REMAINING_MINUTES     = 7512, -- ime remaining: <number> [minute/minutes] (Earth time).
        TIME_REMAINING_SECONDS     = 7513, -- ime remaining: <number> [second/seconds] (Earth time).
        PARTY_FALLEN               = 7515, -- ll party members have fallen in battle. Mission failure in <number> [minute/minutes].
        EXCALIACE_START            = 7524, -- Such a lot of trouble for one little corsair... Shall we be on our way?
        EXCALIACE_END1             = 7525, -- Yeah, I got it. Stay here and keep quiet.
        EXCALIACE_END2             = 7526, -- Hey... It was a short trip, but nothing is ever dull around you, huh?
        EXCALIACE_ESCAPE           = 7527, -- Heh. The Immortals really must be having troubles finding troops if they sent this bunch of slowpokes to watch over me...
        EXCALIACE_PAIN1            = 7528, -- Oomph!
        EXCALIACE_PAIN2            = 7529, -- Ouch!
        EXCALIACE_PAIN3            = 7530, -- Youch!
        EXCALIACE_PAIN4            = 7531, -- Damn, that's gonna leave a mark!
        EXCALIACE_PAIN5            = 7532, -- Urggh!
        EXCALIACE_CRAB1            = 7533, -- Over to you.
        EXCALIACE_CRAB2            = 7534, -- What's this guy up to?
        EXCALIACE_CRAB3            = 7535, -- Uh-oh.
        EXCALIACE_DEBAUCHER1       = 7536, -- Wh-what the...!?
        EXCALIACE_DEBAUCHER2       = 7537, -- H-help!!!
        EXCALIACE_RUN              = 7538, -- Now's my chance!
        EXCALIACE_TOO_CLOSE        = 7539, -- Okay, okay, you got me! I promise I won't run again if you step back a bit...please. Someone's been eating too much garlic...
        EXCALIACE_TIRED            = 7540, -- <Pant>...<wheeze>...
        EXCALIACE_CAUGHT           = 7541, -- Damn...
    },

    mob = 
    {
        [SEAGULL_GROUNDED] = 
        {
            MOBS_START =
            {
                EXCALIAC = 17006593, 17006594, 17006595, 17006596, 17006597, 17006598, 17006599, 17006600, 17006601,
                17006602, 17006603, 17006604, 17006605, 17006606, 17006607, 17006608, 17006610, 17006611,
            },
        },
        [REQUIEM] =
        {
            MOBS_START =
            {
                17006612, 17006613, 17006614, 17006615, 17006616, 17006617, 17006619, 17006620, 17006621,
                17006623, 17006625, 17006626, 17006627, 17006628, 17006630, 17006631, 17006633, 17006634,
            },
        },
        [SHADES_OF_VENGEANCE] = 
        {
            MOBS_START =
            {
                17006754, 17006755, 17006756, 17006757, 17006758, 17006759, 17006760, 17006761, 17006762, 17006763,
            },
        },
    },

    npc = 
    {
        ANCIENT_LOCKBOX = 17006809,
        RUNE_OF_RELEASE = 17006810,
        _1K6            = 17006845,
        _1KX            = 17006872,
        _1KZ            = 17006874,
        _JK1            = 17006876,
        _JK3            = 17006878,
    }
}

return zones[dsp.zone.PERIQIA]
