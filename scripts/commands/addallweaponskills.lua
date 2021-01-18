-----------------------------------
-- func: addallweaponskills
-- desc: Adds all learned weaponskills to the given target. If no target then to the current player.
-----------------------------------

cmdprops =
{
    permission = 1,
    parameters = "s"
}

function error(player, msg)
    player:PrintToPlayer(msg)
    player:PrintToPlayer("!addallweaponskills {player}")
end

function onTrigger(player, target)

    -- validate target
    local targ
    if target then
        targ = GetPlayerByName(target)
        if not targ then
            error(player, string.format("Player named '%s' not found!", target))
            return
        end
    else
        targ = player
    end

    -- add all learned weaponskills
    for i = 1, 48 do
        targ:addLearnedWeaponskill(i)
    end
    player:PrintToPlayer(string.format("%s now has all learned weaponskills.", targ:getName()))
end
