-----------------------------------
-- Magian trials
-----------------------------------
require("scripts/globals/magianobjectives")
require("scripts/globals/common")
require("scripts/globals/zone")
require("scripts/globals/msg")
require("scripts/globals/utils")
-----------------------------------
tpz = tpz or {}
tpz.magian = tpz.magian or {}
tpz.magian.trialCache = tpz.magian.trialCache or {}

-- creates table to track trial and progress per trial slot
local function getPlayerTrials(player)
    local activeTrials = tpz.magian.trialCache[player:getID()]
    if activeTrials then
        return activeTrials
    end
    activeTrials = {}
    for i = 1, 10 do
        local trialBits = player:getCharVar("[trial]" .. i)
        local trialId = bit.rshift(trialBits, 16)
        local progression = bit.band(trialBits, 0xFFFF)
        local trialSQL = GetMagianTrial(trialId)
        local objectiveTotal = trialSQL.objectiveTotal or 0
        activeTrials[i] = { trial = trialId, progress = progression, objectiveTotal = objectiveTotal }
    end
    tpz.magian.trialCache[player:getID()] = activeTrials
    return activeTrials
end
-- packs current trials into params for onTrigger
local function parseParams(player)
    local paramTrials = {}
    for _,v in pairs(getPlayerTrials(player)) do
        if v.trial > 0 then table.insert(paramTrials, v.trial) end
    end
    local params = { 0, 0, 0, 0, 0 }
    for i = 1, #paramTrials, 2 do
        params[(i+1)/2] = paramTrials[i] + bit.lshift((paramTrials[i+1] or 0), 16)
    end
    return params, #paramTrials
end

-- trial id and progress
local function getPlayerTrialByIndex(player, i)
    local activeTrials = getPlayerTrials(player)
    local progress = activeTrials[i] and activeTrials[i].progress
    local trial = activeTrials[i] and activeTrials[i].trial
    local objectiveTotal = activeTrials[i] and activeTrials[i].objectiveTotal
    return trial, progress, objectiveTotal
end

-- Get the active trials who requires the item in parameter
local function getPlayerTrialByItemId(player, itemId)
    local trials = tpz.magian.trials
    local trialsPlayer = getPlayerTrials(player)
    local resultTrials = {}

    for index, obj in pairs(trialsPlayer) do
        local trialId = obj.trial
        if trials[trialId] and
           trials[trialId].reqs and
           trials[trialId].reqs.itemId and
           trials[trialId].reqs.itemId[itemId] and
           trialsPlayer[index].progress < trialsPlayer[index].objectiveTotal then
            table.insert(resultTrials, { trial = trialId, progress = trialsPlayer[index].progress, objectiveTotal = trialsPlayer[index].objectiveTotal })
        end
    end
    return resultTrials
end

-- packs trial id and trial progress
local function setTrial(player, slot, trialId, progress)
    local activeTrials = getPlayerTrials(player)
    if trialId == activeTrials[slot].trial and progress == activeTrials[slot].progress then
        return
    end
    local trialSQL = GetMagianTrial(trialId)
    local objectiveTotal = trialSQL.objectiveTotal or 0
    activeTrials[slot].trial = trialId
    activeTrials[slot].progress = progress or 0
    activeTrials[slot].objectiveTotal = objectiveTotal
    local trialBits = bit.lshift(trialId,16) + progress
    player:setCharVar("[trial]" .. slot, trialBits)
end

-- finds empty trial slot
local function firstEmptySlot(player)
    for i, v in ipairs(getPlayerTrials(player)) do
        if v.trial == 0 then
            return i
        end
    end
end

local function hasTrial(player, trialId)
    if trialId == nil then return nil end
    for i, v in ipairs(getPlayerTrials(player)) do
        if v.trial == trialId then
            return i, v
        end
    end
    return false
end

-- builds augment params for required items
local function reqAugmentParams(t)
    local leftAug1 = bit.lshift(t.reqItemAugValue1, 11) + t.reqItemAug1
    local rightAug1 = bit.lshift(t.reqItemAugValue2, 11) + t.reqItemAug2
    local augBits1 = bit.lshift(leftAug1, 16) + rightAug1
    local leftAug2 = bit.lshift(t.reqItemAugValue3, 11) + t.reqItemAug3
    local rightAug2 = bit.lshift(t.reqItemAugValue4, 11) + t.reqItemAug4
    local augBits2 = bit.lshift(leftAug2, 16) + rightAug2
    return augBits1, augBits2
end

-- builds augment params for reward items
local function rewardAugmentParams(t)
    local leftAug1 = bit.lshift(t.rewardItemAugValue1, 11) + t.rewardItemAug1
    local rightAug1 = bit.lshift(t.rewardItemAugValue2, 11) + t.rewardItemAug2
    local augBits1 = bit.lshift(leftAug1, 16) + rightAug1
    local leftAug2 = bit.lshift(t.rewardItemAugValue3, 11) + t.rewardItemAug3
    local rightAug2 = bit.lshift(t.rewardItemAugValue4, 11) + t.rewardItemAug4
    local augBits2 = bit.lshift(leftAug2, 16) + rightAug2
    return augBits1, augBits2
end


local function checkAndSetProgression(player, trialId, conditions, multiplier)
    local trials = tpz.magian.trials
    local cachePosition, cacheData = hasTrial(player, trialId)
    if trialId and cachePosition then
        local increment = trials[trialId]:check(player, conditions)
        if increment > 0 then
            if cacheData.progress < cacheData.objectiveTotal then
                local newProgress = math.min(cacheData.progress + (increment * multiplier), cacheData.objectiveTotal)
                local remainingObjectives = cacheData.objectiveTotal - newProgress
                setTrial(player, cachePosition, trialId, newProgress)
                if remainingObjectives == 0 then
                    player:messageBasic(tpz.msg.basic.MAGIAN_TRIAL_COMPLETE, trialId) -- trial complete
                else
                    player:messageBasic(tpz.msg.basic.MAGIAN_TRIAL_COMPLETE - 1, trialId, remainingObjectives) -- trial <trialid>: x objectives remain
                end
            elseif cacheData.progress > cacheData.objectiveTotal then
                setTrial(player, cachePosition, trialId, cacheData.objectiveTotal)
            end
        end
    end
end



local function checkItemIdExistsInTable(table, itemId)
    local exists = false
    for index, v in ipairs(table) do
        if not exists and v.id == itemId then
            exists = true
        end
    end
    return exists
end

local function getItemsInTrade(trade)
    local itemsTrials = { }
    local otherItems = { }

    for i=0,7 do
        local item = trade:getItem(i)
        if item then
            local itemId = item:getID()
            local trialNumber = item:getTrialNumber()

            if trialNumber ~= 0 then
                table.insert(itemsTrials, { id = itemId, trialId = trialNumber })
            elseif not checkItemIdExistsInTable(otherItems, itemId) then
                table.insert(otherItems, { id = itemId, quantity = trade:getItemQty(itemId) })
            end
        end
    end
    return itemsTrials, otherItems
end

local function getTrialsBits(player, trials)
    local trialsBits = {}

    for i=5,1,-1 do
        local currentTrial = trials[i]
        if currentTrial and currentTrial.trial ~= 0 then
            local remainingObjectives = currentTrial.objectiveTotal - currentTrial.progress
            table.insert(trialsBits, bit.lshift(remainingObjectives, 16) + currentTrial.trial)
        else
            table.insert(trialsBits, 0)
        end
    end
    return trialsBits
end

local function getItemIdByTrials(trialId)
    local trial = tpz.magian.trials[trialId]
    local itemId = 0
    if trial and trial.reqs and trial.reqs.itemId then
        for item in pairs(trial.reqs.itemId) do
            itemId = item
        end
    end
    return itemId
end



local function returnUselessItems(player, items, itemIdException)
    for i,item in ipairs(items) do
        if item.id ~= itemIdException and item.quantity then
            player:addItem(item.id, item.quantity)
        elseif item.id ~= itemIdException and item.trialId then
            local t = GetMagianTrial(item.trialId)
            player:addItem(t.reqItem, 1, t.reqItemAug1, t.reqItemAugValue1, t.reqItemAug2, t.reqItemAugValue2, t.reqItemAug3, t.reqItemAugValue3, t.reqItemAug4, t.reqItemAugValue4, item.trialId)
        end
    end
end


-----------------------------------
-- Delivery Crate
-----------------------------------

tpz.magian.deliveryCrateOnTrigger = function(player, npc)
    local zoneid = player:getZoneID()
    local msg = zones[zoneid].text
    player:messageSpecial(msg.DELIVERY_CRATE_TEXT, itemId)
end

tpz.magian.deliveryCrateOnTrade = function(player, npc, trade)
    -- items = parts of stuff
    -- itemsTrial = items for trial
    local items, itemsTrial = getItemsInTrade(trade)

    -- get size of tables
    local nbitems = table.getn(items)
    local nbitemsTrial = table.getn(itemsTrial)

    -- playerTrials = active trials for the player
    local trialsPlayer = nil
    local nbTrialsPlayer = 0

    -- currentItem = Part of stuff use for event
    local currentItem = { id=0, quantity=0 }

    -- currentTrial = trial use for event
    local currentTrial = nil
    local currentTrialId = 0

    if nbitemsTrial == 0 then
        return
    end

    if nbitemsTrial >= 1 then
        for i, item in ipairs(itemsTrial) do
            local trials = getPlayerTrialByItemId(player, item.id)
            local nbtrials = table.getn(trials)
            if nbtrials > 0 then
                currentTrial = trials[1]
                currentTrialId = trials[1].trial
                currentItemTrial = item
                trialsPlayer = trials
                nbTrialsPlayer = nbtrials
            end
        end
    end

    if nbTrialsPlayer == 0 or currentTrialId == 0 then
        return
    end

    if nbitems > 0 then -- if player trade 1 part of stuff and item
        currentItem = items[1]
    end

    local nbOverItems = (currentTrial.objectiveTotal - (currentTrial.progress + currentItemTrial.quantity)) * -1
    if nbOverItems > 0 then
        player:addItem(currentItemTrial.id, nbOverItems)
        currentItemTrial.quantity = currentItemTrial.quantity - nbOverItems
    end

    player:setLocalVar("storeTrialId", currentTrialId)
    player:setLocalVar("storeItemId", currentItem.id)
    player:setLocalVar("storeItemTrialId", currentItemTrial.id)
    player:setLocalVar("storeItemTrialQty", currentItemTrial.quantity)
    player:setLocalVar("storeNbTrialsPlayer", nbTrialsPlayer)

    player:tradeComplete()
    player:startEvent(10134, currentItemTrial.id, currentItemTrial.quantity, nbTrialsPlayer, currentTrialId, 0, 0, 0, 0)

    returnUselessItems(player, itemsTrial, currentItemTrial.id)
    returnUselessItems(player, items, currentItem.id)
end

tpz.magian.deliveryCrateOnEventUpdate = function(player, csid, option)
    local optionMod = bit.band(option, 0xFF)
    local itemTrialId = player:getLocalVar("storeItemTrialId")
    local nbTrialsPlayer = player:getLocalVar("storeNbTrialsPlayer")
    local maxNumber = 31

    if csid == 10134 and optionMod == 101 then
        player:updateEvent(itemTrialId, 0, 0, 0, 0, 0, maxNumber, 0)
    end
    if csid == 10134 and optionMod == 103 then
        local places = bit.rshift(maxNumber, nbTrialsPlayer)
        local trials = getPlayerTrialByItemId(player, itemTrialId)

        local trialBits = getTrialsBits(player, trials)
        player:updateEvent(trialBits[1], trialBits[2], trialBits[3], trialBits[4], trialBits[5], nbTrialsPlayer, places, 0)
    end

end

tpz.magian.deliveryCrateOnEventFinish = function(player, csid, option)
    local optionMod = bit.band(option, 0xFF)
    local zoneid = player:getZoneID()
    local msg = zones[zoneid].text
    local trialId = bit.rshift(option, 8)
    local itemTrialId = player:getLocalVar("storeItemTrialId")
    local itemTrialQuantity = player:getLocalVar("storeItemTrialQty")
    local nbTrialsPlayer = player:getLocalVar("storeNbTrialsPlayer")
    local itemId = player:getLocalVar("storeItemId")

    if csid == 10134 and optionMod == 0 then
        player:addItem(itemTrialId, itemTrialQuantity)
        player:messageSpecial(msg.RETURN_ITEM, itemTrialId)

    elseif csid == 10134 and optionMod == 102 then
        checkAndSetProgression(player, trialId, { itemId = itemTrialId, quantity = itemTrialQuantity }, MAGIAN_TRIALS_TRADE_MULTIPLIER)
    end

    if  csid == 10134 and (optionMod == 0 or optionMod == 102) then
        if itemId ~= 0 and nbTrialsPlayer > 0 then
            local t = GetMagianTrial(trialId)
            player:addItem(t.reqItem, 1, t.reqItemAug1, t.reqItemAugValue1, t.reqItemAug2, t.reqItemAugValue2, t.reqItemAug3, t.reqItemAugValue3, t.reqItemAug4, t.reqItemAugValue4, trialId)
        end
        player:setLocalVar("storeTrialId", 0)
        player:setLocalVar("storeItemId", 0)
        player:setLocalVar("storeItemTrialId", 0)
        player:setLocalVar("storeItemTrialQty", 0)
        player:setLocalVar("storeNbTrialsPlayer", 0)
    end
end

-- increments progress if conditions are met
tpz.magian.checkMagianTrial = function(player, conditions)
    for _, slot in pairs( {tpz.slot.MAIN, tpz.slot.SUB, tpz.slot.RANGED} ) do
        local trialIdOnItem = player:getEquippedItem(slot) and player:getEquippedItem(slot):getTrialNumber()
        if trialIdOnItem ~= 0 then
            checkAndSetProgression(player, trialIdOnItem, conditions, MAGIAN_TRIALS_MOBKILL_MULTIPLIER)
        end
    end
end

-----------------------------------
-- Magian Orange / Blue
-----------------------------------

tpz.magian.magianOnTrigger = function(player, npc, EVENT_IDS)
    local p, t = parseParams(player)

    if EVENT_IDS[1] and player:getMainLvl() < 75 then
        player:startEvent(EVENT_IDS[1]) -- can't take a trial before lvl 75

    elseif player:hasKeyItem(tpz.ki.MAGIAN_TRIAL_LOG) == false then
        player:startEvent(EVENT_IDS[2]) -- player can start magian for the first time

    else
        player:startEvent(EVENT_IDS[3], p[1], p[2], p[3], p[4], p[5], 0, 0, t) -- standard dialogue
    end
end

tpz.magian.magianOnTrade = function(player, npc, trade, TYPE, EVENT_IDS)
    local itemId = trade:getItemId()
    local item = trade:getItem()
    local matchId = item:getMatchingTrials()
    local trialId = item:getTrialNumber()
    local t = GetMagianTrial(trialId)
    local zoneid = player:getZoneID()
    local msg = zones[zoneid].text
    local _, pt = parseParams(player)

    player:setLocalVar("storeItemId", itemId)

    if player:hasKeyItem(tpz.ki.MAGIAN_TRIAL_LOG) == true and trade:getSlotCount() == 1 then
        if not next(matchId) and item:isType(TYPE) then
            player:setLocalVar("invalidItem", 1)
            player:startEvent(EVENT_IDS[4], 0, 0, 0, 0, 0, 0, 0, utils.MAX_UINT32) -- invalid weapon
            return

        -- player can only keep 10 trials at once
        elseif pt >= 10 and trialId == 0 then
            player:startEvent(EVENT_IDS[4], 0, 0, 0, 0, 0, 0, 0, utils.MAX_UINT32 - 254)
            return

        elseif trialId ~= 0 then
            for i, v in pairs(getPlayerTrials(player)) do
                if v.trial == trialId then
                    player:setLocalVar("storeTrialId", trialId)
                    player:tradeComplete()
                    if v.progress >= t.objectiveTotal then
                        player:startEvent(EVENT_IDS[6], 0, 0, 0, t.rewardItem, 0, 0, 0, itemId) -- completes trial
                    else
                        player:startEvent(EVENT_IDS[5], trialId, itemId, 0, 0, v, 0, 0, utils.MAX_UINT32 - 1) -- checks status of trial
                    end
                    return
                end
            end
            -- item has trial, player does not
            player:setLocalVar("storeTrialId", trialId)
            player:startEvent(EVENT_IDS[5], trialId, t.reqItem, 0, 0, 0, 0, 0, utils.MAX_UINT32 - 2)
            player:tradeComplete()
            return

        elseif next(matchId) then
            player:setLocalVar("storeTrialId", matchId[1])
            player:tradeComplete()
            player:startEvent(EVENT_IDS[4], matchId[1], matchId[2], matchId[3], matchId[4], 0, itemId) -- starts trial
            return
        else
            player:messageSpecial(msg.ITEM_NOT_WEAPON_MAGIAN) -- item traded isn't a weapon
        end
    else
        player:messageSpecial(msg.ITEM_NOT_WEAPON_MAGIAN) -- item traded isn't a weapon
    end
end

local rareItems = set{ 16192, 18574, 19397, 19398, 19399, 19400, 19401, 19402, 19403, 19404, 19405, 19406, 19407, 19408, 19409, 19410 }

tpz.magian.magianEventUpdate = function(player, itemId, csid, option, EVENT_IDS)
    local optionMod = bit.band(option, 0xFF)

    if (csid == EVENT_IDS[3] or csid == EVENT_IDS[4] or csid == EVENT_IDS[5]) and optionMod == 1 then
        local trialId = bit.rshift(option, 16)
        local t = GetMagianTrial(trialId)
        local a1, a2 = reqAugmentParams(t)
        local itemId = getItemIdByTrials(trialId)
        player:updateEvent(2, a1, a2, t.reqItem, 0, itemId, t.trialTarget)

    elseif (csid == EVENT_IDS[3] or csid == EVENT_IDS[4] or csid == EVENT_IDS[5]) and optionMod == 2 then
        local trialId = bit.rshift(option, 16)
        local t = GetMagianTrial(trialId)
        local _,cacheData = hasTrial(player, trialId)
        local progress = cacheData and cacheData.progress or 0
        local itemId = getItemIdByTrials(trialId)
        player:updateEvent(t.objectiveTotal, 0, progress, 0, 0, itemId, t.element)

    elseif (csid == EVENT_IDS[3] or csid == EVENT_IDS[4] or csid == EVENT_IDS[5]) and optionMod == 3 then
        local trialId = bit.rshift(option, 16)
        local t = GetMagianTrial(trialId)
        local a1, a2 = rewardAugmentParams(t)
        player:updateEvent(2, a1, a2, t.rewardItem, 0, t.objectiveItem)

    elseif (csid == EVENT_IDS[3] or csid == EVENT_IDS[4] or csid == EVENT_IDS[5]) and optionMod == 4 then
        local trialId = bit.rshift(option, 16)
        local t = GetMagianTrial(trialId)
        local results = GetMagianTrialsWithParent(trialId)
        if results then
            player:updateEvent(results[1], results[2], results[3], results[4], t.previousTrial, t.objectiveItem)
        end

    -- lists trials to abandon
    elseif csid == EVENT_IDS[3] and optionMod == 5 then
        local params, trialCount = parseParams(player)
        player:updateEvent(params[1], params[2], params[3], params[4], params[5], 0, 0, trialCount)

    -- abandon trial through menu
    elseif csid == EVENT_IDS[3] and optionMod == 6 then
        local trialId = bit.rshift(option, 8)
        local slot = hasTrial(player, trialId)
        if slot then
            player:updateEvent(0, 0, 0, 0, 0, slot)
            setTrial(player, slot, 0, 0)
        end

    -- checks if trial is already in progress
    elseif csid == EVENT_IDS[4] and optionMod == 7 then
        local trialId = bit.rshift(option, 8)
        local t = GetMagianTrial(trialId)
        local a1, a2 = reqAugmentParams(t)
        local slot = hasTrial(player, trialId)
        if slot then
            player:updateEvent(0, 0, 0, 0, 0, 0, 0, utils.MAX_UINT32)
            return
        end
        player:updateEvent(2, a1, a2, t.reqItem)

    -- checks if item's level will increase
    elseif csid == EVENT_IDS[4] and optionMod == 13 then
        local trialId = bit.rshift(option, 8)
        local t = GetMagianTrial(trialId)
        local reqItem = GetReadOnlyItem(t.reqItem)
        local rewardItem = GetReadOnlyItem(t.rewardItem)
        if reqItem:getReqLvl() < rewardItem:getReqLvl() then
            player:updateEvent(1)
        else
            player:updateEvent(0)
        end

    -- checks if player already owns reward item (if it's rare)
    elseif csid == EVENT_IDS[4] and optionMod == 14 then
        local trialId = bit.rshift(option, 8)
        local t = GetMagianTrial(trialId)
        if (player:hasItem(t.rewardItem) and rareItems[t.rewardItem] == true) then
            player:updateEvent(1)
        else
            player:updateEvent(0)
        end

    -- abandoning trial through trade
    elseif csid == EVENT_IDS[5] and (optionMod == 8 or optionMod == 11) then
        local trialId = bit.rshift(option, 8)
        local t = GetMagianTrial(trialId)
        player:updateEvent(0, 0, 0, t.reqItem)
    end
end

tpz.magian.magianOnEventFinish = function(player, itemId, csid, option, EVENT_IDS)
    local optionMod = bit.band(option, 0xFF)
    local zoneid = player:getZoneID()
    local msg = zones[zoneid].text
    local ID = require("scripts/zones/RuLude_Gardens/IDs")

    if csid == EVENT_IDS[2] and option == 1 then
        player:messageSpecial(ID.text.KEYITEM_OBTAINED,tpz.ki.MAGIAN_TRIAL_LOG)
        player:addKeyItem(tpz.ki.MAGIAN_TRIAL_LOG)

    -- starts a trial
    elseif csid == EVENT_IDS[4] and optionMod == 7 then
        local trialId = bit.rshift(option, 8)
        local t = GetMagianTrial(trialId)
        player:addItem(t.reqItem, 1, t.reqItemAug1, t.reqItemAugValue1, t.reqItemAug2, t.reqItemAugValue2, t.reqItemAug3, t.reqItemAugValue3, t.reqItemAug4, t.reqItemAugValue4, trialId)
        player:messageSpecial(msg.RETURN_MAGIAN_ITEM, t.reqItem)
        setTrial(player, firstEmptySlot(player), trialId, 0)
        player:setLocalVar("storeTrialId", 0)
        player:setLocalVar("storeItemId", 0)

    -- returns item to player
   elseif csid == EVENT_IDS[5] and (optionMod == 0 or optionMod == 4) then
        local trialId = player:getLocalVar("storeTrialId")
        local t = GetMagianTrial(trialId)
        player:addItem(t.reqItem, 1, t.reqItemAug1, t.reqItemAugValue1, t.reqItemAug2, t.reqItemAugValue2, t.reqItemAug3, t.reqItemAugValue3, t.reqItemAug4, t.reqItemAugValue4, trialId)
        player:messageSpecial(msg.RETURN_MAGIAN_ITEM, t.reqItem)
        player:setLocalVar("storeTrialId", 0)

    elseif csid == EVENT_IDS[4] and (optionMod == 0 or option == utils.MAX_UINT32) then
        local trialId = player:getLocalVar("storeTrialId")
        local itemId = player:getLocalVar("storeItemId")
        local t = GetMagianTrial(trialId)
        if player:getLocalVar("invalidItem") ~= 1 then
            player:addItem(t.reqItem, 1, t.reqItemAug1, t.reqItemAugValue1, t.reqItemAug2, t.reqItemAugValue2, t.reqItemAug3, t.reqItemAugValue3, t.reqItemAug4, t.reqItemAugValue4, trialId)
        end
        player:messageSpecial(msg.RETURN_MAGIAN_ITEM, itemId)
        player:setLocalVar("invalidItem", 0)
        player:setLocalVar("storeTrialId", 0)
        player:setLocalVar("storeItemId", 0)


    -- gives back item after removing trial id
    elseif csid == EVENT_IDS[5] and (optionMod == 8 or optionMod == 11) then
        local trialId = bit.rshift(option, 8)
        local t = GetMagianTrial(trialId)
        for i = 1, 10 do
            local tr, _, __ = getPlayerTrialByIndex(player, i)
            if tr == trialId then
                setTrial(player, i, 0, 0)
                break
            end
        end
        player:addItem(t.reqItem, 1, t.reqItemAug1, t.reqItemAugValue1, t.reqItemAug2, t.reqItemAugValue2, t.reqItemAug3, t.reqItemAugValue3, t.reqItemAug4, t.reqItemAugValue4, trialId)
        player:messageSpecial(msg.RETURN_MAGIAN_ITEM, t.reqItem)

    -- finishes a trial
    elseif csid == EVENT_IDS[6] and optionMod == 0 then
        local trialId = player:getLocalVar("storeTrialId")
        local slot = hasTrial(player, trialId)
        if slot then
            setTrial(player, slot, 0, 0)
        end
        local t = GetMagianTrial(trialId)
        player:addItem(t.rewardItem, 1, t.rewardItemAug1, t.rewardItemAugValue1, t.rewardItemAug2, t.rewardItemAugValue2, t.rewardItemAug3, t.rewardItemAugValue3, t.rewardItemAug4, t.rewardItemAugValue4)
        player:messageSpecial(msg.ITEM_OBTAINED, t.rewardItem)
        player:setLocalVar("storeTrialId", 0)
    end
end

-----------------------------------
-- Magian Green
-----------------------------------

function magianGreenEventUpdate(player, ItemID, csid, option)
end
