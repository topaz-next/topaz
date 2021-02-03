-----------------------------------------
-- Stonera II
-- Teaches Stonera
-----------------------------------------

function onItemCheck(target)
    return target:canLearnSpell(835)
end

function onItemUse(target)
    target:addSpell(835)
end