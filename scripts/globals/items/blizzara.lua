-----------------------------------------
-- Teaches Blizzara
-----------------------------------------

function onItemCheck(target)
    return target:canLearnSpell(830)
end

function onItemUse(target)
    target:addSpell(830)
end