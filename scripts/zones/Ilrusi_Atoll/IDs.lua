-----------------------------------
-- Area: Ilrusi_Atoll
-----------------------------------
require("scripts/globals/missions")
require("scripts/globals/zone")
-----------------------------------

zones = zones or {}

zones[dsp.zone.ILRUSI_ATOLL] =
{
    text =
    {
        ITEM_CANNOT_BE_OBTAINED = 6382, -- You cannot obtain the <item>. Come back after sorting your inventory.
        ITEM_OBTAINED           = 6388, -- Obtained: <item>.
        GIL_OBTAINED            = 6389, -- Obtained <number> gil.
        KEYITEM_OBTAINED        = 6391, -- Obtained key item: <keyitem>.
        PLAYER_OBTAINS_ITEM     = 7311, -- <player> obtains a <item>!
        ASSAULT_START_OFFSET    = 7446, -- Max MP Down removed for <player>
        TIME_TO_COMPLETE        = 7507, -- You have <number> [minute/minutes] (Earth time) to complete this mission.
        MISSION_FAILED          = 7508, -- The mission has failed. Leaving area.
        RUNE_UNLOCKED_POS       = 7509, -- Mission objective completed. Unlocking Rune of Release ([A/B/C/D/E/F/G/H/I/J/K/L/M/N/O/P/Q/R/S/T/U/V/W/X/Y/Z]-#).
        RUNE_UNLOCKED           = 7510, -- Mission objective completed. Unlocking Rune of Release.
        ASSAULT_POINTS_OBTAINED = 7511, -- You gain <number> [Assault point/Assault points]!
        TIME_REMAINING_MINUTES  = 7512, -- Time remaining: <number> [minute/minutes] (Earth time).
        TIME_REMAINING_SECONDS  = 7513, -- Time remaining: <number> [second/seconds] (Earth time).
        PARTY_FALLEN            = 7515, -- all party members have fallen in battle. Mission failure in <number> [minute/minutes].
        CHEST_GET_CLOSER        = 7524, -- You must be closer to the chest to open it.
        CHEST                   = 7525, -- The chest contains...
        GOLDEN                  = 7526, -- ...a golden figurehead!
    },
    mob =
    {
        [GOLDEN_SALVAGE] =
        {
            MOBS_START =
            {
                17002497, 17002498, 17002499, 17002500, 17002501, 17002502, 17002503, 17002504, 17002513, 17002514, 17002515, 17002516,
            },
            ILRUSI_CURSED_CHEST_OFFSET = 17002505,
        },
        [EXTERMINATION] =
        {
            MOBS_START =
            {
                17002521, 17002522, 17002523, 17002524, 17002525, 17002526, 17002527, 17002528, 17002529, 17002530,
                17002531, 17002532, 17002533, 17002534, 17002535, 17002536, 17002537, 17002538, 17002539, 17002540,
            },
            NMS =
            {
                CRAB = 17002541, LEECH = 17002542, SLIME = 17002543, TOAD = 17002544
            },
        },
    },
    npc =
    {
        ANCIENT_LOCKBOX            = 17002654,
        RUNE_OF_RELEASE            = 17002655,
    },
}

return zones[dsp.zone.ILRUSI_ATOLL]
