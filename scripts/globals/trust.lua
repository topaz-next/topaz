-----------------------------------
-- Trust
-----------------------------------
require("scripts/globals/keyitems")
require("scripts/globals/settings")
require("scripts/globals/status")
require("scripts/globals/common")
require("scripts/globals/magic")
require("scripts/globals/msg")
require("scripts/globals/roe")
-----------------------------------

tpz = tpz or {}
tpz.trust = tpz.trust or {}

tpz.trust.message_offset =
{
    SPAWN          = 1,
    TEAMWORK_1     = 4,
    TEAMWORK_2     = 5,
    TEAMWORK_3     = 6,
    TEAMWORK_4     = 7,
    TEAMWORK_5     = 8,
    DEATH          = 9,
    DESPAWN        = 11,
    SPECIAL_MOVE_1 = 18,
}

local MAX_MESSAGE_PAGE = 121

local rovKIBattlefieldIDs = set{
    5,    -- Shattering Stars (WAR LB5)
    6,    -- Shattering Stars (BLM LB5)
    7,    -- Shattering Stars (RNG LB5)
    70,   -- Shattering Stars (RDM LB5)
    71,   -- Shattering Stars (THF LB5)
    72,   -- Shattering Stars (BST LB5)
    101,  -- Shattering Stars (MNK LB5)
    102,  -- Shattering Stars (WHM LB5)
    103,  -- Shattering Stars (SMN LB5)
    163,  -- Survival of the Wisest (SCH LB5)
    194,  -- Shattering Stars (SAM LB5)
    195,  -- Shattering Stars (NIN LB5)
    196,  -- Shattering Stars (DRG LB5)
    517,  -- Shattering Stars (PLD LB5)
    518,  -- Shattering Stars (DRK LB5)
    519,  -- Shattering Stars (BRD LB5)
    530,  -- A Furious Finale (DNC LB5)
    1091, -- Breaking the Bonds of Fate (COR LB5)
    1123, -- Achieving True Power (PUP LB5)
    1154, -- The Beast Within (BLU LB5)
-- TODO: GEO LB5
-- TODO: RUN LB5
}

tpz.trust.onTradeCipher = function(player, trade, csid, rovCs, arkAngelCs)
    local hasPermit = player:hasKeyItem(tpz.ki.WINDURST_TRUST_PERMIT) or
                      player:hasKeyItem(tpz.ki.BASTOK_TRUST_PERMIT) or
                      player:hasKeyItem(tpz.ki.SAN_DORIA_TRUST_PERMIT)

    local itemId = trade:getItemId(0)
    local subId = trade:getItemSubId(0)
    local isCipher = itemId >= 10112 and itemId <= 10193

    if hasPermit and trade:getSlotCount() == 1 and subId ~= 0 and isCipher then
        -- subId is a smallInt in the database (16 bits).
        -- The bottom 12 bits of the subId are the spellId taught by the ciper
        -- The top 4 bits of the subId are for the flags to be given to the csid
        local spellId = bit.band(subId, 0x0FFF)
        local flags = bit.rshift(bit.band(subId, 0xF000), 12)

        -- To generate this packed subId for storage in the db:
        -- local encoded = spellId + bit.lshift(flags, 12)

        -- Cipher type cs args (Wetata's text as example):
        -- 0 (add 0)    : Did you know that the person mentioned here is also a participant in the Trust initiative?
        --                All the stuffiest scholars... (Default)
        -- 1 (add 4096) : Wait a second... just who is that? How am I supposed to use <cipher> in conditions like these? (WOTG)
        -- 2 (add 8192) : You may be shocked to hear that there are trusts beyond the five races (Beasts & Monsters)
        -- 3 (add 12288): How on earth did you get your hands on this? If it's a real cipher I have to try! (Special)
        -- 4 (add 16384): Progressed leaps and bounds. You and that person must have something truly special-wecial going on between you.
        --                (Mainline story princesses and II trust versions??)

        player:setLocalVar("TradingTrustCipher", spellId)

        -- TODO Blocking for ROV ciphers
        local rovBlock = false
        local arkAngelCipher = itemId >= 10188 and itemId <= 10192

        if rovBlock then
            player:startEvent(rovCs)
        elseif arkAngelCipher then
            player:startEvent(arkAngelCs, 0, 0, 0, itemId)
        else
            player:startEvent(csid, 0, 0, flags, itemId)
        end
    end
end

tpz.trust.canCast = function(caster, spell, not_allowed_trust_ids)

    -- Trusts must be enabled in settings
    if ENABLE_TRUST_CASTING == 0 then
        return tpz.msg.basic.TRUST_NO_CAST_TRUST
    end

    -- Trusts not allowed in an alliance
    if caster:checkSoloPartyAlliance() == 2 then
        return tpz.msg.basic.TRUST_NO_CAST_TRUST
    end

    -- Trusts only allowed in certain zones (Remove this for trusts everywhere)
    if not caster:canUseMisc(tpz.zoneMisc.TRUST) then
        return tpz.msg.basic.TRUST_NO_CALL_AE
    end

    -- You can only summon trusts if you are the party leader or solo
    local leader = caster:getPartyLeader()
    if leader and caster:getID() ~= leader:getID() then
        caster:messageSystem(tpz.msg.system.TRUST_SOLO_OR_LEADER)
        return -1
    end

    -- Block summoning trusts if seeking a party
    if caster:isSeekingParty() then
        caster:messageSystem(tpz.msg.system.TRUST_NO_SEEKING_PARTY)
        return -1
    end

    -- Block summoning trusts if someone recently joined party (120s)
    local last_party_member_added_time = caster:getPartyLastMemberJoinedTime()
    if os.time() - last_party_member_added_time < 120 then
        caster:messageSystem(tpz.msg.system.TRUST_DELAY_NEW_PARTY_MEMBER)
        return -1
    end

    -- Trusts cannot be summoned if you have hate
    if caster:hasEnmity() then
        caster:messageSystem(tpz.msg.system.TRUST_NO_ENMITY)
        return -1
    end

    -- Check party for trusts
    local num_pt = 0
    local num_trusts = 0
    local party = caster:getPartyWithTrusts()
    for _, member in pairs(party) do
        if member:getObjType() == tpz.objType.TRUST then
            -- Check for same trust
            if member:getTrustID() == spell:getID() then
                caster:messageSystem(tpz.msg.system.TRUST_ALREADY_CALLED)
                return -1
            -- Check not allowed trust combinations (Shantotto I vs Shantotto II)
            elseif type(not_allowed_trust_ids) == "number" then
                if member:getTrustID() == not_allowed_trust_ids then
                    caster:messageSystem(tpz.msg.system.TRUST_ALREADY_CALLED)
                    return -1
                end
            elseif type(not_allowed_trust_ids) == "table" then
                for _, v in pairs(not_allowed_trust_ids) do
                    if type(v) == "number" then
                        if member:getTrustID() == v then
                            caster:messageSystem(tpz.msg.system.TRUST_ALREADY_CALLED)
                            return -1
                        end
                    end
                end
            end
            num_trusts = num_trusts + 1
        end
        num_pt = num_pt + 1
    end

    -- Max party size
    if num_pt >= 6 then
        caster:messageSystem(tpz.msg.system.TRUST_MAXIMUM_NUMBER)
        return -1
    end

    -- Some battlefields allow trusts after you get this ROV Key Item
    local casterBattlefieldID = caster:getBattlefieldID()
    if rovKIBattlefieldIDs[casterBattlefieldID] and not caster:hasKeyItem(tpz.ki.RHAPSODY_IN_UMBER) then
        return tpz.msg.basic.TRUST_NO_CAST_TRUST
    end

    -- Limits set by ROV Key Items
    if num_trusts >= 3 and not caster:hasKeyItem(tpz.ki.RHAPSODY_IN_WHITE) then
        caster:messageSystem(tpz.msg.system.TRUST_MAXIMUM_NUMBER)
        return -1
    elseif num_trusts >= 4 and not caster:hasKeyItem(tpz.ki.RHAPSODY_IN_CRIMSON) then
        caster:messageSystem(tpz.msg.system.TRUST_MAXIMUM_NUMBER)
        return -1
    end

    return 0
end

tpz.trust.spawn = function(caster, spell)
    caster:spawnTrust(spell:getID())

    -- Records of Eminence: Call Forth an Alter Ego
    if caster:getEminenceProgress(932) then
        tpz.roe.onRecordTrigger(caster, 932)
    end

    return 0
end

-- page_offset is: (summon_message_id - 1) / 100
-- Example: Shantotto II summon message ID: 11201
-- page_offset: (11201 - 1) / 100 = 112
tpz.trust.message = function(mob, page_offset, message_offset)

    if page_offset > MAX_MESSAGE_PAGE then
        print("trust.lua: MAX_MESSAGE_PAGE exceeded!")
        return
    end

    local trust_offset = tpz.msg.system.GLOBAL_TRUST_OFFSET + (page_offset * 100)
    mob:trustPartyMessage(trust_offset + message_offset)
end

tpz.trust.teamworkMessage = function(mob, page_offset, teamwork_messages)
    local messages = {}

    local master = mob:getMaster()
    local party = master:getPartyWithTrusts()
    for _, member in pairs(party) do
        if member:getObjType() == tpz.objType.TRUST then
            for id, message in pairs(teamwork_messages) do
                if member:getTrustID() == id then
                    table.insert(messages, message)
                end
            end
        end
    end

    if table.getn(messages) > 0 then
        tpz.trust.message(mob, page_offset, messages[math.random(#messages)])
    else
        -- Defaults to regular spawn message
        tpz.trust.message(mob, page_offset, tpz.trust.message_offset.SPAWN)
    end
end

-- For debugging and lining up teamwork messages
tpz.trust.dumpMessages = function(mob, page_offset)
    for i=0, 20 do
        tpz.trust.message(mob, page_offset, i)
    end
end

tpz.trust.dumpMessagePages = function(mob)
    for i=0, 120 do
        tpz.trust.message(mob, i, tpz.trust.message_offset.SPAWN)
    end
end
