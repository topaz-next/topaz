-----------------------------------------
-- Teaches Fira
-----------------------------------------

function onItemCheck(target)
    return target:canLearnSpell(829)
end

function onItemUse(target)
    target:addSpell(829)
end