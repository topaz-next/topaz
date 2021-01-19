-----------------------------------
-- Area: LaLoff Amphitheater
--  Mob: Ark Angel's Tiger
-----------------------------------
require("scripts/globals/status")
-----------------------------------
local entity = {}

-- TODO: Implement shared spawning and victory system with Ark Angel's Mandragora.

entity.onMobEngaged = function(mob, target)
    local mobid = mob:getID()

    for member = mobid-2, mobid+5 do
        local m = GetMobByID(member)
        if m:getCurrentAction() == tpz.act.ROAMING then
            m:updateEnmity(target)
        end
    end
end

entity.onMobFight = function(mob, target)

end

entity.onMobDeath = function(mob, player, isKiller)
end

return entity
