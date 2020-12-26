﻿/*
===========================================================================

  Copyright (c) 2010-2015 Darkstar Dev Teams

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see http://www.gnu.org/licenses/

===========================================================================
*/

#include <cmath>

#include "lua_baseentity.h"
#include "lua_battlefield.h"
#include "lua_instance.h"
#include "lua_item.h"
#include "lua_statuseffect.h"
#include "lua_trade_container.h"
#include "lua_zone.h"
#include "luautils.h"

#include "../../common/kernel.h"
#include "../../common/showmsg.h"
#include "../../common/timer.h"
#include "../../common/utils.h"

#include "../ability.h"
#include "../alliance.h"
#include "../battlefield.h"
#include "../daily_system.h"
#include "../enmity_container.h"
#include "../guild.h"
#include "../instance.h"
#include "../item_container.h"
#include "../latent_effect_container.h"
#include "../linkshell.h"
#include "../map.h"
#include "../message.h"
#include "../mob_modifier.h"
#include "../mob_spell_container.h"
#include "../mobskill.h"
#include "../notoriety_container.h"
#include "../recast_container.h"
#include "../roe.h"
#include "../spell.h"
#include "../status_effect_container.h"
#include "../timetriggers.h"
#include "../trade_container.h"
#include "../transport.h"
#include "../treasure_pool.h"
#include "../weapon_skill.h"

#include "../ai/ai_container.h"

#include "../ai/states/ability_state.h"
#include "../ai/states/attack_state.h"
#include "../ai/states/death_state.h"
#include "../ai/states/inactive_state.h"
#include "../ai/states/item_state.h"
#include "../ai/states/magic_state.h"
#include "../ai/states/mobskill_state.h"
#include "../ai/states/raise_state.h"
#include "../ai/states/range_state.h"
#include "../ai/states/respawn_state.h"
#include "../ai/states/weaponskill_state.h"

#include "../ai/controllers/mob_controller.h"
#include "../ai/controllers/trust_controller.h"

#include "../ai/helpers/gambits_container.h"

#include "../entities/automatonentity.h"
#include "../entities/charentity.h"
#include "../entities/mobentity.h"
#include "../entities/npcentity.h"
#include "../entities/petentity.h"
#include "../entities/trustentity.h"

#include "../packets/action.h"
#include "../packets/auction_house.h"
#include "../packets/change_music.h"
#include "../packets/char.h"
#include "../packets/char_abilities.h"
#include "../packets/char_appearance.h"
#include "../packets/char_emotion.h"
#include "../packets/char_equip.h"
#include "../packets/char_health.h"
#include "../packets/char_job_extra.h"
#include "../packets/char_jobs.h"
#include "../packets/char_mounts.h"
#include "../packets/char_recast.h"
#include "../packets/char_skills.h"
#include "../packets/char_spells.h"
#include "../packets/char_stats.h"
#include "../packets/char_sync.h"
#include "../packets/char_update.h"
#include "../packets/chat_message.h"
#include "../packets/conquest_map.h"
#include "../packets/entity_animation.h"
#include "../packets/entity_enable_list.h"
#include "../packets/entity_update.h"
#include "../packets/entity_visual.h"
#include "../packets/event.h"
#include "../packets/event_string.h"
#include "../packets/event_update.h"
#include "../packets/event_update_string.h"
#include "../packets/guild_menu.h"
#include "../packets/guild_menu_buy.h"
#include "../packets/independant_animation.h"
#include "../packets/instance_entry.h"
#include "../packets/inventory_finish.h"
#include "../packets/inventory_modify.h"
#include "../packets/inventory_size.h"
#include "../packets/key_items.h"
#include "../packets/menu_merit.h"
#include "../packets/menu_mog.h"
#include "../packets/menu_raisetractor.h"
#include "../packets/message_basic.h"
#include "../packets/message_combat.h"
#include "../packets/message_name.h"
#include "../packets/message_special.h"
#include "../packets/message_standard.h"
#include "../packets/message_system.h"
#include "../packets/message_text.h"
#include "../packets/position.h"
#include "../packets/quest_mission_log.h"
#include "../packets/release.h"
#include "../packets/roe_questlog.h"
#include "../packets/server_ip.h"
#include "../packets/shop_items.h"
#include "../packets/shop_menu.h"
#include "../packets/timer_bar_util.h"
#include "../packets/weather.h"

#include "../utils/battleutils.h"
#include "../utils/blueutils.h"
#include "../utils/charutils.h"
#include "../utils/guildutils.h"
#include "../utils/instanceutils.h"
#include "../utils/itemutils.h"
#include "../utils/jailutils.h"
#include "../utils/mobutils.h"
#include "../utils/petutils.h"
#include "../utils/puppetutils.h"
#include "../utils/trustutils.h"
#include "../utils/zoneutils.h"

//======================================================//

CLuaBaseEntity::CLuaBaseEntity(CBaseEntity* PEntity)
{
    m_PBaseEntity = PEntity;
}

/************************************************************************
 *  Function: showText()
 *  Purpose : Displays dialogue for NPC
 *  Example : target:showText(mob,YOU_DECIDED_TO_SHOW_UP) -- Fighting Maat
 *  Notes   : Mainly used for showing retail text specific to an NPC
 ************************************************************************/

inline int32 CLuaBaseEntity::showText(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    // TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isuserdata(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));

    auto messageID = (uint16)lua_tointeger(L, 2);

    CLuaBaseEntity* PLuaBaseEntity = nullptr; // nullptr;

    if (PLuaBaseEntity != nullptr)
    {
        CBaseEntity* PBaseEntity = PLuaBaseEntity->GetBaseEntity();
        if (PBaseEntity->objtype == TYPE_NPC)
        {
            PBaseEntity->m_TargID       = m_PBaseEntity->targid;
            PBaseEntity->loc.p.rotation = worldAngle(PBaseEntity->loc.p, m_PBaseEntity->loc.p);

            PBaseEntity->loc.zone->PushPacket(PBaseEntity, CHAR_INRANGE, new CEntityUpdatePacket(PBaseEntity, ENTITY_UPDATE, UPDATE_POS));
        }

        uint32 param0 = 0;
        uint32 param1 = 0;
        uint32 param2 = 0;
        uint32 param3 = 0;

        if (!lua_isnil(L, 3) && lua_isnumber(L, 3))
        {
            param0 = (uint32)lua_tointeger(L, 3);
        }
        if (!lua_isnil(L, 4) && lua_isnumber(L, 4))
        {
            param1 = (uint32)lua_tointeger(L, 4);
        }
        if (!lua_isnil(L, 5) && lua_isnumber(L, 5))
        {
            param2 = (uint32)lua_tointeger(L, 5);
        }
        if (!lua_isnil(L, 6) && lua_isnumber(L, 6))
        {
            param3 = (uint32)lua_tointeger(L, 6);
        }

        if (m_PBaseEntity->objtype == TYPE_PC)
        {
            ((CCharEntity*)m_PBaseEntity)->pushPacket(new CMessageSpecialPacket(PBaseEntity, messageID, param0, param1, param2, param3));
        }
        else
        {
            m_PBaseEntity->loc.zone->PushPacket(m_PBaseEntity, CHAR_INRANGE, new CMessageSpecialPacket(PBaseEntity, messageID, param0, param1, param3));
        }
    }
    return 0;
}

/************************************************************************
 *  Function: messageText()
 *  Purpose : Displays text to a target PC (private)
 *  Example : player:messageText(target, NOT_HAVE_ENOUGH_GP, false, 6);
 *  Notes   : Mainly used for sending retail text messages
 ************************************************************************/

inline int32 CLuaBaseEntity::messageText(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isuserdata(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));

    CLuaBaseEntity* PLuaBaseEntity = nullptr; // nullptr;
    CBaseEntity*    PTarget        = nullptr; // PLuaBaseEntity->m_PBaseEntity;

    auto messageID = (uint16)lua_tointeger(L, 2);

    bool  showName = true;
    uint8 mode     = 0;

    if (!lua_isnil(L, 3))
    {
        if (lua_isboolean(L, 3))
        {
            showName = lua_toboolean(L, 3);
        }
        else if (lua_isnumber(L, 3))
        {
            mode = (uint8)lua_tointeger(L, 3);
        }
    }

    if (!lua_isnil(L, 4) && lua_isnumber(L, 4))
    {
        mode = (uint8)lua_tointeger(L, 4);
    }

    if (m_PBaseEntity->objtype == TYPE_PC)
    {
        ((CCharEntity*)m_PBaseEntity)->pushPacket(new CMessageTextPacket(PTarget, messageID, showName, mode));
    }
    else
    { // broadcast in range
        m_PBaseEntity->loc.zone->PushPacket(m_PBaseEntity, CHAR_INRANGE, new CMessageTextPacket(PTarget, messageID, showName, mode));
    }
    return 0;
}

/************************************************************************
 *  Function: PrintToPlayer()
 *  Purpose : Displays either standad messages to a PC or custom text
 *  Example : player:PrintToPlayer("Hello!", 0x1F)
 *          : p:P2P(string.format("Hello, %s!", player:getName()), 0x1F)
 *  Notes   : Available hex codes: 0x1C, 0xD, 0xF, 0x1F, ___, ___
 *          : Can modify the name shown through explicit declaration
 ************************************************************************/

void CLuaBaseEntity::PrintToPlayer(const char* message, sol::object messageType, sol::object name)
{
    CHAT_MESSAGE_TYPE int_messageType = (messageType == sol::lua_nil) ? MESSAGE_SYSTEM_1 : messageType.as<CHAT_MESSAGE_TYPE>();
    const char*       cstr_name       = (name == sol::lua_nil) ? "" : name.as<const char*>();

    if (auto* PChar = dynamic_cast<CCharEntity*>(m_PBaseEntity))
    {
        PChar->pushPacket(new CChatMessagePacket(PChar, int_messageType, message, cstr_name));
    }
}

/************************************************************************
 *  Function: PrintToArea()
 *  Purpose : version of PrintToPlayer that passes to messageserver
 *  Example : player:PrintToArea("Im a real boy!", tpz.msg.channel.SHOUT, tpz.msg.area.SYSTEM, "Pinocchio");
 *          : would print a shout type message from Pinocchio to the entire server
 ************************************************************************/

inline int32 CLuaBaseEntity::PrintToArea(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isstring(L, 1));

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

    // see scripts\globals\msg.lua or src\map\packets\chat_message.h for values
    CHAT_MESSAGE_TYPE messageLook  = (lua_isnil(L, 2) || !lua_isnumber(L, 2)) ? MESSAGE_SYSTEM_1 : (CHAT_MESSAGE_TYPE)lua_tointeger(L, 2);
    uint8             messageRange = (lua_isnil(L, 3) || !lua_isnumber(L, 3)) ? 0 : (CHAT_MESSAGE_TYPE)lua_tointeger(L, 3);
    std::string       name         = (lua_isnil(L, 4) || !lua_isstring(L, 4)) ? std::string() : lua_tostring(L, 4);

    if (messageRange == 0) // All zones world wide
    {
        message::send(MSG_CHAT_SERVMES, nullptr, 0, new CChatMessagePacket(PChar, messageLook, (char*)lua_tostring(L, 1), name));
    }
    else if (messageRange == 1) // Say range
    {
        PChar->loc.zone->PushPacket(PChar, CHAR_INRANGE, new CChatMessagePacket(PChar, messageLook, (char*)lua_tostring(L, 1), name));
    }
    else if (messageRange == 2) // Shout
    {
        PChar->loc.zone->PushPacket(PChar, CHAR_INSHOUT, new CChatMessagePacket(PChar, messageLook, (char*)lua_tostring(L, 1), name));
    }
    else if (messageRange == 3) // Party and Alliance
    {
        message::send(MSG_CHAT_PARTY, nullptr, 0, new CChatMessagePacket(PChar, messageLook, (char*)lua_tostring(L, 1), name));
    }
    else if (messageRange == 4) // Yell zones only
    {
        message::send(MSG_CHAT_YELL, nullptr, 0, new CChatMessagePacket(PChar, messageLook, (char*)lua_tostring(L, 1), name));
    }
    /*
    Todo: Unity, LS 1 and LS 2 for the lols?
    */

    return 0;
}

/************************************************************************
 *  Function: messageBasic()
 *  Purpose : Send a basic message packet to the PC
 *  Example : target:messageBasic(msgBasic.RECOVERS_HP_AND_MP);
 *  Notes   : Mainly used when effects are applied
 ************************************************************************/

inline int32 CLuaBaseEntity::messageBasic(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    uint16 messageID = (uint16)lua_tointeger(L, 1);

    uint32 param0  = 0;
    uint32 param1  = 0;
    auto*  PTarget = m_PBaseEntity;

    if (!lua_isnil(L, 2) && lua_isnumber(L, 2))
    {
        param0 = (uint32)lua_tointeger(L, 2);
    }
    if (!lua_isnil(L, 3) && lua_isnumber(L, 3))
    {
        param1 = (uint32)lua_tointeger(L, 3);
    }
    if (!lua_isnil(L, 4) && lua_isuserdata(L, 4))
    {
        PTarget = nullptr; // Lunar<CLuaBaseEntity>::check(L, 4)->m_PBaseEntity;
    }

    if (m_PBaseEntity->objtype == TYPE_PC)
    {
        ((CCharEntity*)m_PBaseEntity)->pushPacket(new CMessageBasicPacket(m_PBaseEntity, PTarget, param0, param1, messageID));
    }
    else
    { // broadcast in range
        m_PBaseEntity->loc.zone->PushPacket(m_PBaseEntity, CHAR_INRANGE, new CMessageBasicPacket(m_PBaseEntity, PTarget, param0, param1, messageID));
    }
    return 0;
}

/************************************************************************
 *  Function: messageName()
 *  Purpose : Message displayed with an entity's name in it
 *  Example : target:messageName(messageID, entity, param0, param1, param2, param3, chatType);
 *  Notes   : Used in Doom countdown messages, as an example
 ************************************************************************/

inline int32 CLuaBaseEntity::messageName(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(!m_PBaseEntity);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    uint16 messageID = (uint16)lua_tointeger(L, 1);

    CLuaBaseEntity* PLuaEntity  = nullptr; // nullptr;
    CBaseEntity*    PNameEntity = nullptr; // PLuaEntity ? PLuaEntity->m_PBaseEntity : nullptr;

    int32 param0 = 0;
    int32 param1 = 0;
    int32 param2 = 0;
    int32 param3 = 0;

    if (!lua_isnil(L, 3) && lua_isnumber(L, 3))
    {
        param0 = (int32)lua_tointeger(L, 3);
    }
    if (!lua_isnil(L, 4) && lua_isnumber(L, 4))
    {
        param1 = (int32)lua_tointeger(L, 4);
    }
    if (!lua_isnil(L, 5) && lua_isnumber(L, 5))
    {
        param2 = (int32)lua_tointeger(L, 5);
    }
    if (!lua_isnil(L, 6) && lua_isnumber(L, 6))
    {
        param3 = (int32)lua_tointeger(L, 6);
    }

    int32 chatType = 4;
    if (!lua_isnil(L, 7) && lua_isnumber(L, 7))
    {
        chatType = (int32)lua_tointeger(L, 7);
    }

    if (CCharEntity* PChar = dynamic_cast<CCharEntity*>(m_PBaseEntity))
    {
        PChar->pushPacket(new CMessageNamePacket(PChar, messageID, PNameEntity, param0, param1, param2, param3, chatType));
    }
    else
    {
        m_PBaseEntity->loc.zone->PushPacket(m_PBaseEntity, CHAR_INRANGE,
                                            new CMessageNamePacket(m_PBaseEntity, messageID, PNameEntity, param0, param1, param2, param3, chatType));
    }

    return 0;
}

/************************************************************************
 *  Function: messagePublic()
 *  Purpose : Push message to all players
 *  Example : target:messagePublic(112, target, remainingTicks, remainingTicks);
 *  Notes   : Used in Doom countdown messages, as an example
 ************************************************************************/

inline int32 CLuaBaseEntity::messagePublic(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    uint16 messageID = (uint16)lua_tointeger(L, 1);

    uint32 param0 = 0;
    uint32 param1 = 0;

    CLuaBaseEntity* PEntity = nullptr; // nullptr;

    if (PEntity != nullptr)
    {
        if (!lua_isnil(L, 2) && lua_isnumber(L, 3))
        {
            param0 = (uint32)lua_tointeger(L, 3);
        }
        if (!lua_isnil(L, 3) && lua_isnumber(L, 4))
        {
            param1 = (uint32)lua_tointeger(L, 4);
        }

        m_PBaseEntity->loc.zone->PushPacket(m_PBaseEntity, CHAR_INRANGE_SELF,
                                            new CMessageBasicPacket(m_PBaseEntity, PEntity->GetBaseEntity(), param0, param1, messageID));
    }
    return 0;
}

/************************************************************************
 *  Function: messageSpecial()
 *  Purpose : Displays special messages
 *  Example : player:messageSpecial(NOTHING_OUT_OF_ORDINARY);
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::messageSpecial(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    uint16 messageID = (uint16)lua_tointeger(L, 1);

    uint32 param0 = 0;
    uint32 param1 = 0;
    uint32 param2 = 0;
    uint32 param3 = 0;

    bool showName = false;

    if (!lua_isnil(L, 2) && lua_isnumber(L, 2))
    {
        param0 = (uint32)lua_tointeger(L, 2);
    }
    if (!lua_isnil(L, 3) && lua_isnumber(L, 3))
    {
        param1 = (uint32)lua_tointeger(L, 3);
    }
    if (!lua_isnil(L, 4) && lua_isnumber(L, 4))
    {
        param2 = (uint32)lua_tointeger(L, 4);
    }
    if (!lua_isnil(L, 5) && lua_isnumber(L, 5))
    {
        param3 = (uint32)lua_tointeger(L, 5);
    }

    if (!lua_isnil(L, 6) && lua_isboolean(L, 6))
    {
        showName = (lua_toboolean(L, 6) != 0);
    }

    ((CCharEntity*)m_PBaseEntity)->pushPacket(new CMessageSpecialPacket(m_PBaseEntity, messageID, param0, param1, param2, param3, showName));
    return 0;
}

/************************************************************************
 *  Function: messageSystem()
 *  Purpose : Sends a standard system message
 *  Example : player:messageSystem("Text")
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::messageSystem(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    uint16 messageID = (uint16)lua_tointeger(L, 1);

    uint32 param0 = 0;
    uint32 param1 = 0;

    if (!lua_isnil(L, 2) && lua_isnumber(L, 2))
    {
        param0 = (uint32)lua_tointeger(L, 2);
    }
    if (!lua_isnil(L, 3) && lua_isnumber(L, 3))
    {
        param1 = (uint32)lua_tointeger(L, 3);
    }

    ((CCharEntity*)m_PBaseEntity)->pushPacket(new CMessageSystemPacket(param0, param1, messageID));
    return 0;
}

/************************************************************************
 *  Function: messageCombat(...)
 *  Purpose : Various combat related messages are ended with this packet
 *  Example : master:messageCombat(mob, offset + id, 0, 711)
 *  Notes   :
 ************************************************************************/
int32 CLuaBaseEntity::messageCombat(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

    CBaseEntity* PSpeaker;
    if (!lua_isnil(L, 1) && lua_isuserdata(L, 1))
    {
        CLuaBaseEntity* PLuaBaseEntity = nullptr; // nullptr;
        PSpeaker                       = nullptr; // PLuaBaseEntity->m_PBaseEntity;
    }
    else
    {
        PSpeaker = m_PBaseEntity;
    }

    auto p0      = (int32)lua_tointeger(L, 2);
    auto p1      = (int32)lua_tointeger(L, 3);
    auto message = (int16)lua_tointeger(L, 4);

    PChar->pushPacket(new CMessageCombatPacket(PSpeaker, PChar, p0, p1, message));

    return 0;
}

/************************************************************************
 *  Function: getCharVar()
 *  Purpose : Returns a var value assigned to a PC (in char_vars.sql)
 *  Example : local status = player:getCharVar("[ZM]Status")
 *  Notes   :
 ************************************************************************/

int32 CLuaBaseEntity::getCharVar(std::string const& varName)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);
    auto* PChar = dynamic_cast<CCharEntity*>(m_PBaseEntity);
    return charutils::GetCharVar(PChar, varName.c_str());
}

/************************************************************************
 *  Function: setCharVar()
 *  Purpose : Updates PC's variable to an explicit value
 *  Example : player:setCharVar("[ZM]Status", 4)
 *  Notes   : Passing a '0' value will delete the variable
 ************************************************************************/

void CLuaBaseEntity::setCharVar(std::string const& varname, int32 value)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    if (value == 0)
    {
        Sql_Query(SqlHandle, "DELETE FROM char_vars WHERE charid = %u AND varname = '%s' LIMIT 1;", m_PBaseEntity->id, varname.c_str());
    }
    else
    {
        const char* fmtQuery = "INSERT INTO char_vars SET charid = %u, varname = '%s', value = %i ON DUPLICATE KEY UPDATE value = %i;";
        Sql_Query(SqlHandle, fmtQuery, m_PBaseEntity->id, varname.c_str(), value, value);
    }
}

/************************************************************************
 *  Function: addCharVar()
 *  Purpose : Increments PC's variable by an explicit amount
 *  Example : player:addCharVar("[ZM]Status", 1) -- if 4, becomes 5
 *  Notes   : Can use values greater than 1 to increment more
 ************************************************************************/

void CLuaBaseEntity::addCharVar(std::string const& varname, int32 value)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    const char* Query = "INSERT INTO char_vars SET charid = %u, varname = '%s', value = %i ON DUPLICATE KEY UPDATE value = value + %i;";

    Sql_Query(SqlHandle, Query, m_PBaseEntity->id, varname, value, value);
}

/************************************************************************
 *  Function: getLocalVar()
 *  Purpose : Returns a variable assigned locally to an entity
 *  Example : if (KingArthro:getLocalVar("[POP]King_Arthro") > 0) then
 *  Notes   :
 ************************************************************************/

uint32 CLuaBaseEntity::getLocalVar(std::string const& var)
{
    return m_PBaseEntity->GetLocalVar(var.c_str());
}

/************************************************************************
 *  Function: setLocalVar()
 *  Purpose : Assigns a local variable to an entity
 *  Example : mob:setLocalVar("pop", os.time() + math.random(1200,7200));
 *  Notes   :
 ************************************************************************/

void CLuaBaseEntity::setLocalVar(std::string const& var, uint32 val)
{
    m_PBaseEntity->SetLocalVar(var.c_str(), val);
}

/************************************************************************
 *  Function: resetLocalVars()
 *  Purpose : Reset local variables back to default (ex: on Mob disengage)
 *  Example : GetMobByID(Defender):resetLocalVars();
 *  Notes   :
 ************************************************************************/

void CLuaBaseEntity::resetLocalVars()
{
    m_PBaseEntity->ResetLocalVars();
}

/************************************************************************
 *  Function: getLastOnline()
 *  Purpose : Returns the unix timestamp of the last time the char logged off or zoned
 *  Example : player:getLastOnline()
 *  Notes   :
 ************************************************************************/

uint32 CLuaBaseEntity::getLastOnline()
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    if (auto* PChar = dynamic_cast<CCharEntity*>(m_PBaseEntity))
    {
        return PChar->lastOnline;
    }

    return 0;
}

/************************************************************************
 *  Function: injectPacket()
 *  Purpose : Injects a packet to the player's client
 *  Example : player:injectPacket(packet)
 *  Notes   : Used only for testing through inject.lua command
 ************************************************************************/

inline int32 CLuaBaseEntity::injectPacket(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1));

    uint8 size = 0;
    FILE* File = fopen(lua_tostring(L, 1), "rb");

    if (File)
    {
        CBasicPacket* PPacket = new CBasicPacket();

        fseek(File, 1, SEEK_SET);
        if (fread(&size, 1, 1, File) != 1)
        {
            ShowError(CL_RED "CLuaBaseEntity::injectPacket : Did not read size\n" CL_RESET);
            return 0;
        }

        fseek(File, 0, SEEK_SET);
        if (fread(*PPacket, 1, size * 2, File) != size * 2)
        {
            ShowError(CL_RED "CLuaBaseEntity::injectPacket : Did not read entire packet\n" CL_RESET);
            return 0;
        }

        ((CCharEntity*)m_PBaseEntity)->pushPacket(PPacket);

        fclose(File);
    }
    else
    {
        ShowError(CL_RED "CLuaBaseEntity::injectPacket : Cannot open file\n" CL_RESET);
    }
    return 0;
}

/************************************************************************
 *  Function: injectActionPacket()
 *  Purpose : Used for testing and finding animations, not production use
 *  Example : player:injectActionPacket(actID, animID, spEffect, react, msg);
 *  Notes   : Used only for testing through injectaction.lua command
 ************************************************************************/

inline int32 CLuaBaseEntity::injectActionPacket(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

    uint16     action     = (uint16)lua_tointeger(L, 1);
    uint16     anim       = (uint16)lua_tointeger(L, 2);
    SPECEFFECT speceffect = (SPECEFFECT)lua_tointeger(L, 3);
    REACTION   reaction   = (REACTION)lua_tointeger(L, 4);
    uint16     message    = (uint16)lua_tointeger(L, 5);

    ACTIONTYPE actiontype = ACTION_MAGIC_FINISH;
    switch (action)
    {
        case 3:
            actiontype = ACTION_WEAPONSKILL_FINISH;
            break;
        case 4:
            actiontype = ACTION_MAGIC_FINISH;
            break;
        case 5:
            actiontype = ACTION_ITEM_FINISH;
            break;
        case 6:
            actiontype = ACTION_JOBABILITY_FINISH;
            break;
        case 8:
            actiontype = ACTION_MAGIC_START;
            break;
        case 11:
            actiontype = ACTION_MOBABILITY_FINISH;
            break;
        case 13:
            actiontype = ACTION_PET_MOBABILITY_FINISH;
            break;
        case 14:
            actiontype = ACTION_DANCE;
            break;
    }

    action_t Action;

    Action.id       = PChar->id;
    Action.actionid = 1;

    // If you use ACTION_MOBABILITY_FINISH, the first param = anim, the second param = skill id.
    if (actiontype == ACTION_MOBABILITY_FINISH || actiontype == ACTION_PET_MOBABILITY_FINISH)
    {
        CBattleEntity* PTarget = (CBattleEntity*)PChar->GetEntity(PChar->m_TargID);
        if (PTarget == nullptr)
        {
            ShowError("Cannot use MOBABILITY_FINISH on a nullptr battle target! Target a mob! \n");
            return 0;
        }
        Action.id              = PTarget->id;
        Action.actiontype      = actiontype;
        actionList_t& list     = Action.getNewActionList();
        list.ActionTargetID    = PChar->id;
        actionTarget_t& target = list.getNewActionTarget();
        target.animation       = anim;
        target.param           = 10;
        target.messageID       = message;
        PTarget->loc.zone->PushPacket(PTarget, CHAR_INRANGE, new CActionPacket(Action));
        return 0;
    }

    Action.actiontype      = actiontype;
    actionList_t& list     = Action.getNewActionList();
    list.ActionTargetID    = PChar->id;
    actionTarget_t& target = list.getNewActionTarget();
    target.animation       = anim;
    target.param           = 10;
    target.messageID       = message;
    target.speceffect      = speceffect;
    target.reaction        = reaction;

    if (actiontype == ACTION_MAGIC_START)
    {
        SPELLGROUP castType = (SPELLGROUP)lua_tointeger(L, 2);
        uint16     castAnim = (uint16)lua_tointeger(L, 3);

        Action.spellgroup = castType;
        Action.actiontype = actiontype;
        target.reaction   = REACTION::NONE;
        target.speceffect = SPECEFFECT::NONE;
        if (lua_isnil(L, 3))
        {
            target.animation = 0;
        }
        else
        {
            target.animation = castAnim;
        }
        target.param     = message;
        target.messageID = 327; // starts casting
        return 0;
    }

    PChar->loc.zone->PushPacket(PChar, CHAR_INRANGE_SELF, new CActionPacket(Action));

    return 0;
}

/************************************************************************
 *  Function: entityVisualPacket()
 *  Purpose : Sends a visual packet to the PC
 *  Example : player:entityVisualPacket("byc7")
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::entityVisualPacket(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);
    int32 n = lua_gettop(L);
    TPZ_DEBUG_BREAK_IF(n < 1 || n > 2);

    const char* command = lua_tostring(L, -1);

    CBaseEntity* PNpc = nullptr;
    if (n == 2 && lua_isuserdata(L, 1))
    {
        CLuaBaseEntity* PLuaBaseEntity = nullptr; // nullptr;
        PNpc                           = nullptr; // PLuaBaseEntity->m_PBaseEntity;
    }
    ((CCharEntity*)m_PBaseEntity)->pushPacket(new CEntityVisualPacket(PNpc, command));
    return 0;
}

/************************************************************************
 *  Function: entityAnimationPacket()
 *  Purpose : Sends an animation packet to the entity
 *  Example : mob:entityAnimationPacket("sp00")
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::entityAnimationPacket(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isstring(L, 1));

    const char* command = lua_tostring(L, 1);

    if (m_PBaseEntity->objtype == TYPE_PC)
    {
        ((CCharEntity*)m_PBaseEntity)->pushPacket(new CEntityAnimationPacket(m_PBaseEntity, command));
    }
    else
    {
        m_PBaseEntity->loc.zone->PushPacket(m_PBaseEntity, CHAR_INRANGE, new CEntityAnimationPacket(m_PBaseEntity, command));
    }
    return 0;
}

/************************************************************************
 *  Function: startEvent()
 *  Purpose : Starts an event (cutscene)
 *  Example : player:startEvent(4)
 *  Notes   : Cutscene ID must be associated with the zone
 ************************************************************************/

inline int32 CLuaBaseEntity::startEvent(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    auto* PChar = dynamic_cast<CCharEntity*>(m_PBaseEntity);
    if (!PChar)
    {
        ShowError("CLuaBaseEntity::startEvent: Could not start event, Base Entity is not a Character Entity.\n");
        return 0;
    }

    int32 n = lua_gettop(L);

    if (n > 10)
    {
        ShowError("CLuaBaseEntity::startEvent: Could not start event, Lack of arguments.\n");
        lua_settop(L, -n);
        return 0;
    }
    if (PChar->animation == ANIMATION_HEALING)
    {
        PChar->StatusEffectContainer->DelStatusEffect(EFFECT_HEALING);
    }

    if (PChar->PPet)
    {
        PChar->PPet->PAI->Disengage();
    }

    uint16 EventID = (uint16)lua_tointeger(L, 1);

    uint32 param0    = 0;
    uint32 param1    = 0;
    uint32 param2    = 0;
    uint32 param3    = 0;
    uint32 param4    = 0;
    uint32 param5    = 0;
    uint32 param6    = 0;
    uint32 param7    = 0;
    int16  textTable = -1;

    if (!lua_isnil(L, 2) && lua_isnumber(L, 2))
    {
        param0 = (uint32)lua_tointeger(L, 2);
    }
    if (!lua_isnil(L, 3) && lua_isnumber(L, 3))
    {
        param1 = (uint32)lua_tointeger(L, 3);
    }
    if (!lua_isnil(L, 4) && lua_isnumber(L, 4))
    {
        param2 = (uint32)lua_tointeger(L, 4);
    }
    if (!lua_isnil(L, 5) && lua_isnumber(L, 5))
    {
        param3 = (uint32)lua_tointeger(L, 5);
    }
    if (!lua_isnil(L, 6) && lua_isnumber(L, 6))
    {
        param4 = (uint32)lua_tointeger(L, 6);
    }
    if (!lua_isnil(L, 7) && lua_isnumber(L, 7))
    {
        param5 = (uint32)lua_tointeger(L, 7);
    }
    if (!lua_isnil(L, 8) && lua_isnumber(L, 8))
    {
        param6 = (uint32)lua_tointeger(L, 8);
    }
    if (!lua_isnil(L, 9) && lua_isnumber(L, 9))
    {
        param7 = (uint32)lua_tointeger(L, 9);
    }
    if (!lua_isnil(L, 10) && lua_isnumber(L, 10))
    {
        textTable = (int16)lua_tointeger(L, 10);
    }

    PChar->pushPacket(new CEventPacket(PChar, EventID, n - 1, param0, param1, param2, param3, param4, param5, param6, param7, textTable));

    // если требуется вернуть фиктивный результат, то делаем это
    if (!lua_isnil(L, 10) && lua_isnumber(L, 10))
    {
        PChar->m_event.Option = (int32)lua_tointeger(L, 10);
    }

    PChar->m_Substate = CHAR_SUBSTATE::SUBSTATE_IN_CS;

    return 0;
}

/************************************************************************
 *  Function: startEventString()
 *  Purpose : Starts an event (cutscene) with string parameters (0x33 packet)
 *  Example : Too long to show
 *  Notes   : See scripts/zones/Aht_Urhgan_Whitegate/npcs/Ghatsad.lua
 ************************************************************************/

inline int32 CLuaBaseEntity::startEventString(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    auto* PChar = dynamic_cast<CCharEntity*>(m_PBaseEntity);
    if (!PChar)
    {
        ShowError("CLuaBaseEntity::startEventString: Could not start event, Base Entity is not a Character Entity.\n");
        return 0;
    }

    if (PChar->animation == ANIMATION_HEALING)
    {
        PChar->StatusEffectContainer->DelStatusEffect(EFFECT_HEALING);
    }

    if (PChar->PPet)
    {
        PChar->PPet->PAI->Disengage();
    }

    uint16 EventID = (uint16)lua_tointeger(L, 1);

    string_t string0;
    string_t string1;
    string_t string2;
    string_t string3;

    uint32 param0 = 0;
    uint32 param1 = 0;
    uint32 param2 = 0;
    uint32 param3 = 0;
    uint32 param4 = 0;
    uint32 param5 = 0;
    uint32 param6 = 0;
    uint32 param7 = 0;

    if (!lua_isnil(L, 2) && lua_isstring(L, 2))
    {
        string0 = lua_tolstring(L, 2, nullptr);
    }
    if (!lua_isnil(L, 3) && lua_isstring(L, 3))
    {
        string1 = lua_tolstring(L, 3, nullptr);
    }
    if (!lua_isnil(L, 4) && lua_isstring(L, 4))
    {
        string2 = lua_tolstring(L, 4, nullptr);
    }
    if (!lua_isnil(L, 5) && lua_isstring(L, 5))
    {
        string3 = lua_tolstring(L, 5, nullptr);
    }
    if (!lua_isnil(L, 6) && lua_isnumber(L, 6))
    {
        param0 = (uint32)lua_tointeger(L, 6);
    }
    if (!lua_isnil(L, 7) && lua_isnumber(L, 7))
    {
        param1 = (uint32)lua_tointeger(L, 7);
    }
    if (!lua_isnil(L, 8) && lua_isnumber(L, 8))
    {
        param2 = (uint32)lua_tointeger(L, 8);
    }
    if (!lua_isnil(L, 9) && lua_isnumber(L, 9))
    {
        param3 = (uint32)lua_tointeger(L, 9);
    }
    if (!lua_isnil(L, 10) && lua_isnumber(L, 10))
    {
        param4 = (uint32)lua_tointeger(L, 10);
    }
    if (!lua_isnil(L, 11) && lua_isnumber(L, 11))
    {
        param5 = (uint32)lua_tointeger(L, 11);
    }
    if (!lua_isnil(L, 12) && lua_isnumber(L, 12))
    {
        param6 = (uint32)lua_tointeger(L, 12);
    }
    if (!lua_isnil(L, 13) && lua_isnumber(L, 13))
    {
        param7 = (uint32)lua_tointeger(L, 13);
    }

    PChar->pushPacket(
        new CEventStringPacket(PChar, EventID, string0, string1, string2, string3, param0, param1, param2, param3, param4, param5, param6, param7));

    return 0;
}

/************************************************************************
 *  Function: updateEvent()
 *  Purpose : Sends new arguments to an event
 *  Example : player:updateEvent(ring[1],ring[2],ring[3])
 *  Notes   : Ex: CoP ring selection uses this to redisplay correct order of rings
 ************************************************************************/

inline int32 CLuaBaseEntity::updateEvent(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    int32 n = lua_gettop(L);

    if (n > 8)
    {
        ShowError("CLuaBaseEntity::updateEvent: Could not update event, Lack of arguments.\n");
        lua_settop(L, -n);
        return 0;
    }

    uint32 param0 = 0;
    uint32 param1 = 0;
    uint32 param2 = 0;
    uint32 param3 = 0;
    uint32 param4 = 0;
    uint32 param5 = 0;
    uint32 param6 = 0;
    uint32 param7 = 0;

    if (!lua_isnil(L, 1) && lua_isnumber(L, 1))
    {
        param0 = (uint32)lua_tointeger(L, 1);
    }
    if (!lua_isnil(L, 2) && lua_isnumber(L, 2))
    {
        param1 = (uint32)lua_tointeger(L, 2);
    }
    if (!lua_isnil(L, 3) && lua_isnumber(L, 3))
    {
        param2 = (uint32)lua_tointeger(L, 3);
    }
    if (!lua_isnil(L, 4) && lua_isnumber(L, 4))
    {
        param3 = (uint32)lua_tointeger(L, 4);
    }
    if (!lua_isnil(L, 5) && lua_isnumber(L, 5))
    {
        param4 = (uint32)lua_tointeger(L, 5);
    }
    if (!lua_isnil(L, 6) && lua_isnumber(L, 6))
    {
        param5 = (uint32)lua_tointeger(L, 6);
    }
    if (!lua_isnil(L, 7) && lua_isnumber(L, 7))
    {
        param6 = (uint32)lua_tointeger(L, 7);
    }
    if (!lua_isnil(L, 8) && lua_isnumber(L, 8))
    {
        param7 = (uint32)lua_tointeger(L, 8);
    }

    ((CCharEntity*)m_PBaseEntity)->pushPacket(new CEventUpdatePacket(param0, param1, param2, param3, param4, param5, param6, param7));
    return 0;
}

/************************************************************************
 *  Function: updateEventString()
 *  Purpose : Sends a string to an event in progress
 *  Example : player:updateEventString(name)
 *  Notes   : Used by BCNM to display record holder's name
 ************************************************************************/

inline int32 CLuaBaseEntity::updateEventString(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    std::string string0;
    std::string string1;
    std::string string2;
    std::string string3;

    uint32 param0 = 0;
    uint32 param1 = 0;
    uint32 param2 = 0;
    uint32 param3 = 0;
    uint32 param4 = 0;
    uint32 param5 = 0;
    uint32 param6 = 0;
    uint32 param7 = 0;
    uint32 param8 = 0;

    if (!lua_isnil(L, 1) && lua_isstring(L, 1))
    {
        string0 = std::string(lua_tostring(L, 1));
    }
    if (!lua_isnil(L, 2) && lua_isstring(L, 2))
    {
        string1 = std::string(lua_tostring(L, 2));
    }
    if (!lua_isnil(L, 3) && lua_isstring(L, 3))
    {
        string2 = std::string(lua_tostring(L, 3));
    }
    if (!lua_isnil(L, 4) && lua_isstring(L, 4))
    {
        string3 = std::string(lua_tostring(L, 4));
    }

    if (!lua_isnil(L, 5) && lua_isnumber(L, 5))
    {
        param0 = (uint32)lua_tointeger(L, 5);
    }
    if (!lua_isnil(L, 6) && lua_isnumber(L, 6))
    {
        param1 = (uint32)lua_tointeger(L, 6);
    }
    if (!lua_isnil(L, 7) && lua_isnumber(L, 7))
    {
        param2 = (uint32)lua_tointeger(L, 7);
    }
    if (!lua_isnil(L, 8) && lua_isnumber(L, 8))
    {
        param3 = (uint32)lua_tointeger(L, 8);
    }
    if (!lua_isnil(L, 9) && lua_isnumber(L, 9))
    {
        param4 = (uint32)lua_tointeger(L, 9);
    }
    if (!lua_isnil(L, 10) && lua_isnumber(L, 10))
    {
        param5 = (uint32)lua_tointeger(L, 10);
    }
    if (!lua_isnil(L, 11) && lua_isnumber(L, 11))
    {
        param6 = (uint32)lua_tointeger(L, 11);
    }
    if (!lua_isnil(L, 12) && lua_isnumber(L, 12))
    {
        param7 = (uint32)lua_tointeger(L, 12);
    }

    ((CCharEntity*)m_PBaseEntity)
        ->pushPacket(new CEventUpdateStringPacket(string0, string1, string2, string3, param0, param1, param2, param3, param4, param5, param6, param7, param8));
    return 0;
}

/************************************************************************
 *  Function: getEventTarget()
 *  Purpose : Returns object data of the NPC in the event
 *  Example : local npc = player:getEventTarget()
 *  Notes   : Used to relocate Siren's Tear, as an example
 ************************************************************************/

inline int32 CLuaBaseEntity::getEventTarget(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    if (((CCharEntity*)m_PBaseEntity)->m_event.Target == nullptr)
    {
        ShowWarning(CL_YELLOW "EventTarget is empty: %s\n" CL_RESET, m_PBaseEntity->GetName());
    }
    lua_getglobal(L, "CBaseEntity");
    lua_pushstring(L, "new");
    lua_gettable(L, -2);
    lua_insert(L, -2);
    lua_pushlightuserdata(L, (void*)((CCharEntity*)m_PBaseEntity)->m_event.Target);
    lua_pcall(L, 2, 1, 0);
    return 1;
}

/************************************************************************
 *  Function: release()
 *  Purpose : Ends an event for a PC; releases from cutscene
 *  Example : player:release()
 *  Notes   :
 ************************************************************************/

void CLuaBaseEntity::release()
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);

    RELEASE_TYPE releaseType = RELEASE_TYPE::STANDARD;

    if (PChar->m_event.EventID != -1)
    {
        // Message: Event skipped
        releaseType = RELEASE_TYPE::SKIPPING;
        PChar->pushPacket(new CMessageSystemPacket(0, 0, 117));
    }
    PChar->pushPacket(new CReleasePacket(PChar, releaseType));
    PChar->pushPacket(new CReleasePacket(PChar, RELEASE_TYPE::EVENT));
}

/************************************************************************
 *  Function: setFlag()
 *  Purpose : Sets a flag for a PC
 *  Example : player:setFlag(FLAG_GM)
 *  Notes   : Also used for Regain and Spike spell effects
 ************************************************************************/

void CLuaBaseEntity::setFlag(uint32 flags)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    static_cast<CCharEntity*>(m_PBaseEntity)->nameflags.flags ^= flags;
    m_PBaseEntity->updatemask |= UPDATE_HP;
}

/************************************************************************
 *  Function: moghouseFlag()
 *  Purpose : Creates or returns exit flag for Mog House
 *  Example : player:moghouseFlag(2)
 *  Notes   :  Used in Mog House exit quests (ex. A Lady's Heart)
 ************************************************************************/

inline int32 CLuaBaseEntity::moghouseFlag(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);

    if (!lua_isnil(L, 1) && lua_isnumber(L, 1))
    {
        PChar->profile.mhflag |= (uint8)lua_tointeger(L, 1);
        charutils::SaveCharStats(PChar);
        return 0;
    }
    lua_pushinteger(L, PChar->profile.mhflag);
    return 1;
}

/************************************************************************
 *  Function: needToZone()
 *  Purpose : Checks to see if a player has zoned since the flag was raised
 *  Example : player:needToZone(true)
 *  Notes   : Used in events where player needs to zone before continuing
 ************************************************************************/

inline int32 CLuaBaseEntity::needToZone(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    if (!lua_isnil(L, 1) && lua_isboolean(L, 1))
    {
        m_PBaseEntity->loc.zoning = lua_toboolean(L, 1);
        return 0;
    }
    lua_pushboolean(L, m_PBaseEntity->loc.zoning);
    return 1;
}

/************************************************************************
 *  Function: getID()
 *  Purpose : Get Entity's ID
 *  Example : npc:getID(); target:getID()
 *  Notes   :
 ************************************************************************/

uint32 CLuaBaseEntity::getID()
{
    return m_PBaseEntity->id;
}

/************************************************************************
 *  Function: getShortID()
 *  Purpose : Gets the ID of a Target
 *  Example : mob:getShortID(); pet:getShortID()
 *  Notes   : To Do: Should be renamed to getTargID
 ************************************************************************/

uint16 CLuaBaseEntity::getShortID()
{
    return m_PBaseEntity->targid;
}

/************************************************************************
 *  Function: getCursorTarget()
 *  Purpose : GM command - gets the ID of selected Mob's, NPC's, Players
 *  Example : player:getCursorTarget()
 *  Notes   :
 ************************************************************************/

std::shared_ptr<CLuaBaseEntity> CLuaBaseEntity::getCursorTarget()
{
    if (auto* PChar = dynamic_cast<CCharEntity*>(m_PBaseEntity))
    {
        if (auto* PTarget = PChar->GetEntity(PChar->m_TargID))
        {
            return std::make_shared<CLuaBaseEntity>(PTarget);
        }
    }

    return nullptr;
}

/************************************************************************
 *  Function: getObjType()
 *  Purpose : Returns the int value of an entity's object type (Mob,PC...)
 *  Example : if (caster:getObjType() == TYPE_PC) then
 *  Notes   :
 ************************************************************************/

uint8 CLuaBaseEntity::getObjType()
{
    return m_PBaseEntity->objtype;
}

/************************************************************************
 *  Function: isPC()
 *  Purpose : Returns true if entity is of the PC object type
 *  Example : if (target:isPC()) then
 *  Notes   :
 ************************************************************************/

bool CLuaBaseEntity::isPC()
{
    return m_PBaseEntity->objtype == TYPE_PC;
}

/************************************************************************
 *  Function: isNPC()
 *  Purpose : Returns true if entity is of the NPC object type
 *  Example : if (target:isNPC()) then
 *  Notes   :
 ************************************************************************/

bool CLuaBaseEntity::isNPC()
{
    return m_PBaseEntity->objtype == TYPE_NPC;
}

/************************************************************************
 *  Function: isMob()
 *  Purpose : Returns true if entity is of the Mob object type
 *  Example : if (target:isMob()) then
 *  Notes   :
 ************************************************************************/

bool CLuaBaseEntity::isMob()
{
    return m_PBaseEntity->objtype == TYPE_MOB;
}

/************************************************************************
 *  Function: isPet()
 *  Purpose : Returns true if entity is of the Pet object type
 *  Example : if (caster:isPet()) then
 *  Notes   :
 ************************************************************************/

bool CLuaBaseEntity::isPet()
{
    return m_PBaseEntity->objtype == TYPE_PET;
}

/************************************************************************
 *  Function: isAlly()
 *  Purpose : Returns true if entity is an ally
 *  Example : if (mob:isAlly()) then table.insert(allies, mob) end
 *  Notes   :
 ************************************************************************/

bool CLuaBaseEntity::isAlly()
{
    return m_PBaseEntity->objtype == TYPE_MOB && m_PBaseEntity->allegiance == ALLEGIANCE_TYPE::PLAYER;
}

/************************************************************************
 *  Function: initNpcAi()
 *  Purpose : Initiate pre-defined NPC AI
 *  Example : npc:initNpcAi(); -- Red Ghost in Port Jeuno (walks a path)
 *  Notes   : To Do: Change name, this is ugly
 ************************************************************************/

void CLuaBaseEntity::initNpcAi()
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_NPC);

    m_PBaseEntity->PAI = std::make_unique<CAIContainer>(m_PBaseEntity, std::make_unique<CPathFind>(m_PBaseEntity), nullptr, nullptr);
}

/************************************************************************
 *  Function: resetAI()
 *  Purpose : Resets the AI to the default state
 *  Example : mob:resetAI()
 *  Notes   : Most used by mobs (esp Aerns after Reraise)
 ************************************************************************/

void CLuaBaseEntity::resetAI()
{
    m_PBaseEntity->PAI->Reset();
}

/************************************************************************
 *  Function: getStatus()
 *  Purpose : Returns the status (or 'state') of an entity
 *  Example : if (qm2:getStatus() ~= STATUS_DISAPPEAR) then
 *  Notes   :
 ************************************************************************/

uint8 CLuaBaseEntity::getStatus()
{
    return static_cast<uint8>(m_PBaseEntity->status);
}

/************************************************************************
 *  Function: setStatus()
 *  Purpose : Updates the status (or 'state') of an entity
 *  Example : npc:setStatus(STATUS_NORMAL)
 *  Notes   :
 ************************************************************************/

void CLuaBaseEntity::setStatus(uint8 status)
{
    m_PBaseEntity->status = static_cast<STATUS_TYPE>(status);
    m_PBaseEntity->updatemask |= UPDATE_HP;
}

/************************************************************************
 *  Function: getCurrentAction()
 *  Purpose : Returns the current state of a non-NPC entity
 *  Example : if (target:getCurrentAction() ~= ACTION_MOBABILITY_USING)
 *  Notes   : Function name ambiguous, but getCurrentState() in use already
 ************************************************************************/

inline int32 CLuaBaseEntity::getCurrentAction(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    if (m_PBaseEntity->PAI->IsStateStackEmpty())
    {
        lua_pushinteger(L, 16);
    }
    else if (m_PBaseEntity->PAI->IsCurrentState<CRespawnState>())
    {
        lua_pushinteger(L, 0);
    }
    else if (m_PBaseEntity->PAI->IsCurrentState<CAttackState>())
    {
        lua_pushinteger(L, 1);
    }
    else if (m_PBaseEntity->PAI->IsCurrentState<CRangeState>())
    {
        lua_pushinteger(L, 12);
    }
    else if (m_PBaseEntity->PAI->IsCurrentState<CWeaponSkillState>())
    {
        lua_pushinteger(L, 3);
    }
    else if (m_PBaseEntity->PAI->IsCurrentState<CMagicState>())
    {
        lua_pushinteger(L, 30);
    }
    else if (m_PBaseEntity->PAI->IsCurrentState<CItemState>())
    {
        lua_pushinteger(L, 28);
    }
    else if (m_PBaseEntity->PAI->IsCurrentState<CAbilityState>())
    {
        lua_pushinteger(L, 6);
    }
    else if (m_PBaseEntity->PAI->IsCurrentState<CInactiveState>())
    {
        lua_pushinteger(L, 27);
    }
    else if (m_PBaseEntity->PAI->IsCurrentState<CDeathState>())
    {
        lua_pushinteger(L, 22);
    }
    else if (m_PBaseEntity->PAI->IsCurrentState<CRaiseState>())
    {
        lua_pushinteger(L, 37);
    }
    else if (m_PBaseEntity->PAI->IsCurrentState<CMobSkillState>())
    {
        lua_pushinteger(L, 34);
    }
    else
    {
        lua_pushnil(L);
    }

    return 1;
}

/************************************************************************
 *  Function: lookAt()
 *  Purpose : Forces an entity to 'look' at something like it's self-aware
 *  Example : npc:lookAt(player:getPos()) -- Make an NPC look at the PC
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::lookAt(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    float posX = 0;
    float posY = 0;
    float posZ = 0;

    if (lua_isnumber(L, 1))
    {
        posX = (float)lua_tonumber(L, 1);
        posY = (float)lua_tonumber(L, 2);
        posZ = (float)lua_tonumber(L, 3);
    }
    else
    {
        lua_getfield(L, 1, "x");
        posX = (float)lua_tonumber(L, -1);
        lua_getfield(L, 1, "y");
        posY = (float)lua_tonumber(L, -1);
        lua_getfield(L, 1, "z");
        posZ = (float)lua_tonumber(L, -1);
    }

    position_t point;

    point.x = posX;
    point.y = posY;
    point.z = posZ;

    m_PBaseEntity->loc.p.rotation = worldAngle(m_PBaseEntity->loc.p, point);

    m_PBaseEntity->updatemask |= UPDATE_POS;

    return 0;
}

/************************************************************************
 *  Function: clearTargID()
 *  Purpose : Clears an active target from an Entity
 *  Example : GetNPCByID(17719350):clearTargID()
 *  Notes   : This is only used in scripts/zones/Southern_San_dOria/npcs/Raminel.lua
 *          : to get the NPC to stop looking at Raminel after he gets too far
 ************************************************************************/

inline int32 CLuaBaseEntity::clearTargID(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    m_PBaseEntity->m_TargID = 0;
    m_PBaseEntity->loc.zone->PushPacket(m_PBaseEntity, CHAR_INRANGE, new CEntityUpdatePacket(m_PBaseEntity, ENTITY_UPDATE, UPDATE_POS));
    return 0;
}

/************************************************************************
 *  Function: atPoint()
 *  Purpose : Used to check whether an entity is at a specified point in the specified path
 *  Example : if (npc:atPoint(pathfind.get(path, 45))) then
 *  Notes   : Used to trigger delays, messages, etc (Ex: Patroller in West Ronfaure)
 ************************************************************************/

inline int32 CLuaBaseEntity::atPoint(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    float posX = 0;
    float posY = 0;
    float posZ = 0;

    if (lua_isnumber(L, 1))
    {
        posX = (float)lua_tonumber(L, 1);
        posY = (float)lua_tonumber(L, 2);
        posZ = (float)lua_tonumber(L, 3);
    }
    else
    {
        // It's a table
        lua_rawgeti(L, 1, 1);
        posX = (float)lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_rawgeti(L, 1, 2);
        posY = (float)lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_rawgeti(L, 1, 3);
        posZ = (float)lua_tonumber(L, -1);
        lua_pop(L, 1);
    }

    lua_pushboolean(L, m_PBaseEntity->loc.p.x == posX && m_PBaseEntity->loc.p.y == posY && m_PBaseEntity->loc.p.z == posZ);

    return 1;
}

/************************************************************************
 *  Function: pathTo()
 *  Purpose : Makes a non-PC move toward a target without changing action
 *  Example : mob:pathTo(Pos.x + math.cos(radians) * 16, Pos.y, Pos.z + math.sin(radians) * 16);
 *  Notes   : Currently only used by Selh'Teus during final CoP fight
 ************************************************************************/

inline int32 CLuaBaseEntity::pathTo(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_PC);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 3) || !lua_isnumber(L, 3));

    position_t point;
    point.x = (float)lua_tonumber(L, 1);
    point.y = (float)lua_tonumber(L, 2);
    point.z = (float)lua_tonumber(L, 3);

    if (m_PBaseEntity->PAI->PathFind)
    {
        if (lua_isnumber(L, 4))
        {
            m_PBaseEntity->PAI->PathFind->PathTo(point, (uint8)lua_tointeger(L, 4));
        }
        else
        {
            m_PBaseEntity->PAI->PathFind->PathTo(point, PATHFLAG_RUN | PATHFLAG_WALLHACK | PATHFLAG_SCRIPT);
        }
    }

    return 0;
}

/************************************************************************
 *  Function: pathThrough()
 *  Purpose : Makes an Entity follow a given set of points
 *  Example : mob:pathThrough(pathfind.first(path), PATHFLAG_RUN)
 *  Notes   : Ex: Gets Zipacna back on his specified path
 ************************************************************************/

inline int32 CLuaBaseEntity::pathThrough(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    uint8 length = (uint8)lua_objlen(L, 1);

    std::vector<position_t> points;

    // Grab points from array and store in points array
    for (uint8 i = 1; i < length; i += 3)
    {
        lua_rawgeti(L, 1, i);
        lua_rawgeti(L, 1, i + 1);
        lua_rawgeti(L, 1, i + 2);

        points.push_back({ (float)lua_tointeger(L, -3), (float)lua_tointeger(L, -2), (float)lua_tointeger(L, -1), 0, 0 });

        lua_pop(L, 3);
    }

    uint8 flags = 0;

    if (lua_isnumber(L, 2))
    {
        flags = (uint8)lua_tointeger(L, 2);
    }

    CBattleEntity* PBattle = (CBattleEntity*)m_PBaseEntity;

    if (PBattle->PAI->PathFind->PathThrough(std::move(points), flags))
    {
        lua_pushboolean(L, true);
    }
    else
    {
        lua_pushboolean(L, false);
    }

    return 1;
}

/************************************************************************
 *  Function: isFollowingPath()
 *  Purpose : Returns true if entity is following its specified path
 *  Example : if (npc:isFollowingPath()) then
 *  Notes   : Often used in conjunction with pathThrough()
 ************************************************************************/

inline int32 CLuaBaseEntity::isFollowingPath(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    CBattleEntity* PBattle = (CBattleEntity*)m_PBaseEntity;

    lua_pushboolean(L, PBattle->PAI->PathFind != nullptr && PBattle->PAI->PathFind->IsFollowingPath());

    return 1;
}

/************************************************************************
 *  Function: clearPath()
 *  Purpose : Clears all path points and stops entity movement
 *  Example : npc:clearPath()
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::clearPath(lua_State* L)
{
    CBattleEntity* PBattle = (CBattleEntity*)m_PBaseEntity;

    if (PBattle->PAI->PathFind != nullptr)
    {
        PBattle->PAI->PathFind->Clear();
    }

    return 0;
}

/************************************************************************
 *  Function: checkDistance()
 *  Purpose : Returns the yalm distance between entities
 *  Example1: if player:checkDistance(target) <= 25 then
 *  Example2: if player:checkDistance(pos) <= 25 then
 *  Example3: if player:checkDistance(posX, posY, PosZ) <= 25 then
 *  Notes   : Example1 is an entity, the others are coordinate point inputs
 ************************************************************************/

inline int32 CLuaBaseEntity::checkDistance(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1));

    float calcdistance = 0;

    if (lua_isuserdata(L, 1))
    {
        // CLuaBaseEntity* PLuaBaseEntity = nullptr;
        // calcdistance                   = distance(m_PBaseEntity->loc.p, PLuaBaseEntity->GetBaseEntity()->loc.p);
    }
    else
    {
        float      posX = 0;
        float      posY = 0;
        float      posZ = 0;
        position_t point;

        if (lua_istable(L, 1))
        {
            lua_getfield(L, 1, "x");
            posX = (float)lua_tonumber(L, -1);
            lua_getfield(L, 1, "y");
            posY = (float)lua_tonumber(L, -1);
            lua_getfield(L, 1, "z");
            posZ = (float)lua_tonumber(L, -1);
        }
        else if (lua_isnumber(L, 1))
        {
            posX = (float)lua_tonumber(L, 1);

            if (!lua_isnil(L, 2) && lua_isnumber(L, 2))
            {
                posY = (float)lua_tonumber(L, 2);
            }

            if (!lua_isnil(L, 3) && lua_isnumber(L, 3))
            {
                posZ = (float)lua_tonumber(L, 3);
            }
        }
        else
        {
            ShowError("Lua::checkDistance : invalid inputs.");
            return 0;
        }
        point.x      = posX;
        point.y      = posY;
        point.z      = posZ;
        calcdistance = distance(m_PBaseEntity->loc.p, point);
    }

    lua_pushnumber(L, calcdistance);
    return 1;
}

/************************************************************************
 *  Function: wait()
 *  Purpose : Makes a non-PC inactive for a set amount of time
 *  Example : npc:wait(10000) -- wait 10 seconds
 *  Notes   : Default is 4 seconds unless specified in ms
 ************************************************************************/

inline int32 CLuaBaseEntity::wait(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_PC);

    CBattleEntity* PBattle = (CBattleEntity*)m_PBaseEntity;

    int32 waitTime = 4000;

    if (lua_isnumber(L, 1))
    {
        waitTime = (int32)lua_tonumber(L, 1);
    }
    PBattle->PAI->Inactive(std::chrono::milliseconds(waitTime), true);

    return 0;
}

/************************************************************************
 *  Function: openDoor()
 *  Purpose : Opens a door for 7 seconds; different time can be specified
 *  Example : npc:openDoor(30) -- Open for 30 sec; npc:openDoor() -- 7 sec
 ************************************************************************/

void CLuaBaseEntity::openDoor(sol::object const& seconds)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_NPC);

    if (m_PBaseEntity->animation == ANIMATION_CLOSE_DOOR)
    {
        uint32 OpenTime = (seconds != sol::nil) ? seconds.as<uint32>() * 1000 : 7000;

        m_PBaseEntity->animation = ANIMATION_OPEN_DOOR;
        m_PBaseEntity->loc.zone->PushPacket(m_PBaseEntity, CHAR_INRANGE, new CEntityUpdatePacket(m_PBaseEntity, ENTITY_UPDATE, UPDATE_COMBAT));

        m_PBaseEntity->PAI->QueueAction(queueAction_t(std::chrono::milliseconds(OpenTime), false, [](CBaseEntity* PNpc) {
            PNpc->animation = ANIMATION_CLOSE_DOOR;
            PNpc->loc.zone->PushPacket(PNpc, CHAR_INRANGE, new CEntityUpdatePacket(PNpc, ENTITY_UPDATE, UPDATE_COMBAT));
        }));
    }
}

/************************************************************************
 *  Function: closeDoor()
 *  Purpose : Closes a door for 7 seconds; different delay can be specified
 *  Example : npc:closeDoor(); GetNPCByID(Lantern_ID):closeDoor(1)
 ************************************************************************/

void CLuaBaseEntity::closeDoor(sol::object const& seconds)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_NPC);

    if (m_PBaseEntity->animation == ANIMATION_OPEN_DOOR)
    {
        uint32 CloseTime         = (seconds != sol::nil) ? seconds.as<uint32>() * 1000 : 7000;
        m_PBaseEntity->animation = ANIMATION_CLOSE_DOOR;
        m_PBaseEntity->loc.zone->PushPacket(m_PBaseEntity, CHAR_INRANGE, new CEntityUpdatePacket(m_PBaseEntity, ENTITY_UPDATE, UPDATE_COMBAT));

        m_PBaseEntity->PAI->QueueAction(queueAction_t(std::chrono::milliseconds(CloseTime), false, [](CBaseEntity* PNpc) {
            PNpc->animation = ANIMATION_OPEN_DOOR;
            PNpc->loc.zone->PushPacket(PNpc, CHAR_INRANGE, new CEntityUpdatePacket(PNpc, ENTITY_UPDATE, UPDATE_COMBAT));
        }));
    }
}

/************************************************************************
 *  Function: setElevator()
 *  Purpose : Initializes an elevator or something that moves regularly
 *  Example : See Comments Below
 *  Notes   : See: scripts/zones/Metalworks/npcs/_6lt.lua
 ************************************************************************/

void CLuaBaseEntity::setElevator(uint8 id, uint32 lowerDoor, uint32 upperDoor, uint32 elevatorId, bool reversed)
{
    // Usage: setElevator(id, lower door id, upper door id, elevator platform id, animations reversed bool)
    // If giving the elevator ANIMATION_ELEVATOR_UP makes it go down, set this bool to true
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_NPC);

    Elevator_t elevator;

    elevator.id                 = id;
    elevator.LowerDoor          = static_cast<CNpcEntity*>(zoneutils::GetEntity(lowerDoor, TYPE_NPC));
    elevator.UpperDoor          = static_cast<CNpcEntity*>(zoneutils::GetEntity(upperDoor, TYPE_NPC));
    elevator.Elevator           = static_cast<CNpcEntity*>(zoneutils::GetEntity(elevatorId, TYPE_NPC));
    elevator.animationsReversed = reversed;

    if (!elevator.Elevator || !elevator.LowerDoor || !elevator.UpperDoor)
    {
        ShowWarning("Elevator id %d initialization failed - an ID resolved to no entity.", elevatorId);
        return;
    }

    // ID of 0 means it is a timed, automatic elevator
    elevator.activated   = elevator.id == 0;
    elevator.isPermanent = elevator.id == 0;

    elevator.movetime = 3;
    elevator.interval = 8;

    elevator.zoneID = m_PBaseEntity->loc.zone->GetID();

    elevator.Elevator->name.resize(10);
    CTransportHandler::getInstance()->insertElevator(elevator);
}

/************************************************************************
 *  Function: addPeriodicTrigger()
 *  Purpose : registers a periodic trigger for an NPC
 *  Example : BastokDrawbridge:addPeriodicTrigger(0, 360, 80)
 *  Notes   : See usage below
 ************************************************************************/

void CLuaBaseEntity::addPeriodicTrigger(uint8 id, uint16 period, uint16 minOffset)
{
    // Usage npc:addPeriodicTrigger( triggerID, period, minute offset )
    // triggerID is an ID unique to the NPC
    // period is the time in vanadiel minutes that separates two triggers of the event
    // minute offset is the time in vanadiel minutes after the se epoch that the trigger period should synchronize to

    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_NPC);

    Trigger_t trigger;

    trigger.id           = id;
    trigger.period       = period;
    trigger.minuteOffset = minOffset;
    trigger.npc          = dynamic_cast<CNpcEntity*>(zoneutils::GetEntity(m_PBaseEntity->id, TYPE_NPC));

    if (!trigger.npc)
    {
        ShowWarning("Trigger initialization failed - npc ID %d resolved to no entity.", m_PBaseEntity->id);
        return;
    }

    CTriggerHandler::getInstance()->insertTrigger(trigger);
}

/************************************************************************
 *  Function: showNPC()
 *  Purpose : Opposite of hide, shows an NPC for a set amount of time
 *  Example : npc:showNPC(30) -- Appears for 30 sec, then disappears
 *  Notes   : Default is 15 seconds
 ************************************************************************/

void CLuaBaseEntity::showNPC(sol::object const& seconds)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_NPC);

    uint32 showTime = (seconds != sol::nil) ? seconds.as<uint32>() * 1000 : 15000;

    m_PBaseEntity->status = STATUS_TYPE::NORMAL;
    m_PBaseEntity->loc.zone->PushPacket(m_PBaseEntity, CHAR_INRANGE, new CEntityUpdatePacket(m_PBaseEntity, ENTITY_UPDATE, UPDATE_COMBAT));

    m_PBaseEntity->PAI->QueueAction(queueAction_t(std::chrono::milliseconds(showTime), false, [](CBaseEntity* PNpc) {
        PNpc->status = STATUS_TYPE::DISAPPEAR;
        PNpc->loc.zone->PushPacket(PNpc, CHAR_INRANGE, new CEntityUpdatePacket(PNpc, ENTITY_DESPAWN, UPDATE_NONE));
    }));
}

/************************************************************************
 *  Function: hideNPC()
 *  Purpose : Hides an NPC for a set amount of time
 *  Example : npc:hideNPC(30) -- Will hide for 30 seconds
 *  Notes   : Default is 15 seconds
 ************************************************************************/

void CLuaBaseEntity::hideNPC(sol::object const& seconds)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_NPC);

    if (m_PBaseEntity->status == STATUS_TYPE::NORMAL)
    {
        uint32 hideTime = (seconds != sol::nil) ? seconds.as<uint32>() * 1000 : 15000;

        m_PBaseEntity->status = STATUS_TYPE::DISAPPEAR;
        m_PBaseEntity->loc.zone->PushPacket(m_PBaseEntity, CHAR_INRANGE, new CEntityUpdatePacket(m_PBaseEntity, ENTITY_DESPAWN, UPDATE_NONE));

        m_PBaseEntity->PAI->QueueAction(queueAction_t(std::chrono::milliseconds(hideTime), false, [](CBaseEntity* PNpc) {
            PNpc->status = STATUS_TYPE::NORMAL;
            PNpc->loc.zone->PushPacket(PNpc, CHAR_INRANGE, new CEntityUpdatePacket(PNpc, ENTITY_UPDATE, UPDATE_COMBAT));
        }));
    }
}

/************************************************************************
 *  Function: updateNPCHideTime()
 *  Purpose : Adds more time to an NPC being hidden
 *  Example : npc:updateNPCHideTime(50000) -- Hide-and-Seek World Champ
 *  Notes   : Default is 15 seconds
 ************************************************************************/

void CLuaBaseEntity::updateNPCHideTime(sol::object const& seconds)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_NPC);

    if (m_PBaseEntity->status == STATUS_TYPE::DISAPPEAR)
    {
        uint32 hideTime = (seconds != sol::nil) ? seconds.as<uint32>() * 1000 : 15000;

        m_PBaseEntity->PAI->QueueAction(queueAction_t(std::chrono::milliseconds(hideTime), false, [](CBaseEntity* PNpc) {
            PNpc->status = STATUS_TYPE::NORMAL;
            PNpc->loc.zone->PushPacket(PNpc, CHAR_INRANGE, new CEntityUpdatePacket(PNpc, ENTITY_UPDATE, UPDATE_COMBAT));
        }));
    }
}

/************************************************************************
 *  Function: getWeather()
 *  Purpose : Returns the current weather status
 *  Example : if (player:getWeather() == WEATHER_WIND) then
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getWeather(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    WEATHER weather = WEATHER_NONE;
    if (m_PBaseEntity->objtype & TYPE_PC || m_PBaseEntity->objtype & TYPE_MOB)
    {
        weather = battleutils::GetWeather((CBattleEntity*)m_PBaseEntity, false);
    }
    else
    {
        weather = zoneutils::GetZone(m_PBaseEntity->getZone())->GetWeather();
    }

    lua_pushinteger(L, weather);

    return 1;
}

/************************************************************************
 *  Function: setWeather()
 *  Purpose : Updates the current weather status
 *  Example : player:setWeather(weatherList.sunshine);
 *  Notes   : Only used for GM command: scripts/commands/setweather.lua
 ************************************************************************/

inline int32 CLuaBaseEntity::setWeather(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    uint8 weather = (uint8)lua_tointeger(L, 1);

    if (weather < MAX_WEATHER_ID)
    {
        zoneutils::GetZone(m_PBaseEntity->getZone())->SetWeather((WEATHER)weather);
        luautils::OnZoneWeatherChange(m_PBaseEntity->getZone(), weather);
    }
    return 0;
}

/************************************************************************
 *  Function: ChangeMusic()
 *  Purpose : Select a new .bgw file to play on the client
 *  Example : player:ChangeMusic(5,84)
 *  Notes   : Used for mounting Chocobo and changing Jeuno music in Winter
 ************************************************************************/

inline int32 CLuaBaseEntity::ChangeMusic(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

    uint8 BlockID      = (uint32)lua_tointeger(L, 1);
    uint8 MusicTrackID = (uint32)lua_tointeger(L, 2);

    PChar->pushPacket(new CChangeMusicPacket(BlockID, MusicTrackID));
    return 0;
}

/************************************************************************
 *  Function: sendMenu()
 *  Purpose : Sends a menu to the PC (Ex: Auction, Mog House, Shop)
 *  Example : player:sendMenu(3)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::sendMenu(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

    uint32 menu = (uint32)lua_tointeger(L, 1);

    switch (menu)
    {
        case 1:
            PChar->pushPacket(new CMenuMogPacket());
            break;
        case 2:
            PChar->pushPacket(new CShopMenuPacket(PChar));
            PChar->pushPacket(new CShopItemsPacket(PChar));
            break;
        case 3:
            PChar->pushPacket(new CAuctionHousePacket(2));
            break;
        default:
            ShowDebug("Menu %i not implemented, yet.\n", menu);
            break;
    }
    return 0;
}

/************************************************************************
 *  Function: sendGuild()
 *  Purpose : Sends a guild menu to the PC (Ex: Cooking, Smithing, etc)
 *  Example : if (player:sendGuild(60426,1,18,6)) then
 *  Notes   : L2 and L3 only need simplified 24-hour time format (1,2,etc)
 ************************************************************************/

inline int32 CLuaBaseEntity::sendGuild(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 3) || !lua_isnumber(L, 3));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 4) || !lua_isnumber(L, 4));

    uint16 GuildID = (uint16)lua_tonumber(L, 1);
    uint8  open    = (uint8)lua_tonumber(L, 2);
    uint8  close   = (uint8)lua_tonumber(L, 3);
    uint8  holiday = (uint8)lua_tonumber(L, 4);

    TPZ_DEBUG_BREAK_IF(open > close);

    uint8 VanadielHour = (uint8)CVanaTime::getInstance()->getHour();
    // uint8 VanadielDay = (uint8)CVanaTime::getInstance()->getWeekday();

    GUILDSTATUS status = GUILD_OPEN;

    /*
     * No more guild holidays since 2014
    if (VanadielDay == holiday)
    {
        status = GUILD_HOLYDAY;
    }
    */
    if ((VanadielHour < open) || (VanadielHour >= close))
    {
        status = GUILD_CLOSE;
    }
    CItemContainer* PGuildShop                = guildutils::GetGuildShop(GuildID);
    ((CCharEntity*)m_PBaseEntity)->PGuildShop = PGuildShop;
    ((CCharEntity*)m_PBaseEntity)->pushPacket(new CGuildMenuPacket(status, open, close, holiday));
    if (status == GUILD_OPEN)
    {
        ((CCharEntity*)m_PBaseEntity)->pushPacket(new CGuildMenuBuyPacket((CCharEntity*)m_PBaseEntity, PGuildShop));
    }

    lua_pushboolean(L, status == GUILD_OPEN);
    return 1;
}

/************************************************************************
 *  Function: openSendBox()
 *  Purpose : Opens the send box for a PC
 *  Example : player:openSendBox()
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::openSendBox(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    charutils::OpenSendBox((CCharEntity*)m_PBaseEntity);

    return 0;
}

/************************************************************************
 *  Function: leaveGame()
 *  Purpose : Forces a client shutdown
 *  Example : player:leaveGame()
 *  Notes   : Used for logoff commands
 ************************************************************************/

void CLuaBaseEntity::leaveGame()
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);

    PChar->status = STATUS_TYPE::SHUTDOWN;
    charutils::SendToZone(PChar, 1, 0);
}

/************************************************************************
 *  Function: sendEmote()
 *  Purpose : Makes a player entity emit an emote.
 *  Example : player:sendEmote(npc, tpz.emote.EXCAVATION, tpz.emoteMode.MOTION)
 *  Notes   : Currently only used for HELM animations.
 ************************************************************************/

inline int32 CLuaBaseEntity::sendEmote(lua_State* L)
{
    /*
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC)

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isuserdata(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 3) || !lua_isnumber(L, 3));

    auto* const PChar   = dynamic_cast<CCharEntity*>(m_PBaseEntity);
    CCharEntity* const PTarget = nullptr;


    if (PChar && PTarget)
    {
        const auto emoteID   = static_cast<Emote>(lua_tointeger(L, 2));
        const auto emoteMode = static_cast<EmoteMode>(lua_tointeger(L, 3));

        PChar->loc.zone->PushPacket(PChar, CHAR_INRANGE,
                                    new CCharEmotionPacket(PChar, PTarget->GetBaseEntity()->id, PTarget->GetBaseEntity()->targid, emoteID, emoteMode, 0));
    }
    */

    return 0;
}

/************************************************************************
 *  Function: getWorldAngle()
 *  Purpose : Returns angle between two entities, relative to cardinal direction
 *  Example : player:worldAngle(target)
 *  Notes   : Target is... 0: east; 64: south; 128: west, 192: north
 *            Default angle is 255-based mob rotation value - NOT a 360 angle
 ************************************************************************/

inline int32 CLuaBaseEntity::getWorldAngle(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isuserdata(L, 1));

    // TODO:
    CLuaBaseEntity* PLuaBaseEntity = nullptr;

    TPZ_DEBUG_BREAK_IF(PLuaBaseEntity == nullptr);

    int16 angle   = worldAngle(m_PBaseEntity->loc.p, PLuaBaseEntity->GetBaseEntity()->loc.p);
    int16 degrees = (int16)(lua_gettop(L) > 1 ? lua_tointeger(L, 2) : 256);
    if (degrees != 256)
    {
        if (degrees % 4 == 0)
        {
            angle = static_cast<int16>(round((angle * M_PI / 128) * (degrees / (2 * M_PI))));
            angle = angle % degrees; // If we rounded up to the "final" angle, we want the starting angle
        }
        else
        {
            ShowError(CL_RED "getWorldAngle: Called with degrees %d which isn't multiple of 4 \n" CL_RESET, degrees);
        }
    }

    lua_pushnumber(L, angle);
    return 1;
}

/************************************************************************
 *  Function: getFacingAngle()
 *  Purpose : Returns angle comparison of where entity is facing versus where
 *            a target is. Returned value is how many degrees the
 *            entity would need to turn to directly face the target.
 *  Example : attacker:getFacingAngle(defender)
 *  Notes   : 0: directly facing; 64: to a side; 128 target behind entity
 *            Returned angle is 255-based rotation value - NOT a 360 angle
 *            Return value is signed to indicate this shortest turn direction.
 *            Negative: counter-clockwise (left), Positive: clockwise (right)
 ************************************************************************/

inline int32 CLuaBaseEntity::getFacingAngle(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isuserdata(L, 1));

    CLuaBaseEntity* PLuaBaseEntity = nullptr;

    TPZ_DEBUG_BREAK_IF(PLuaBaseEntity == nullptr);

    lua_pushnumber(L, facingAngle(m_PBaseEntity->loc.p, PLuaBaseEntity->GetBaseEntity()->loc.p));
    return 1;
}

/************************************************************************
 *  Function: isFacing()
 *  Purpose : Returns true if an entity is facing another entity
 *  Example : if (attacker:isFacing(target)) then
 *  Notes   : Can specify angle for wider/narrower ranges
 ************************************************************************/

inline int32 CLuaBaseEntity::isFacing(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isuserdata(L, 1));

    CLuaBaseEntity* PLuaBaseEntity = nullptr;

    auto angle = (uint8)(lua_gettop(L) > 1 ? lua_tointeger(L, 2) : 64);

    TPZ_DEBUG_BREAK_IF(PLuaBaseEntity == nullptr);

    lua_pushboolean(L, facing(m_PBaseEntity->loc.p, PLuaBaseEntity->GetBaseEntity()->loc.p, angle));
    return 1;
}

/************************************************************************
 *  Function: isInfront()
 *  Purpose : Returns true if an entity is in front of another entity
 *  Example : if (attacker:isInfront(target)) then
 *  Notes   : Can specify angle for wider/narrower ranges
 ************************************************************************/

inline int32 CLuaBaseEntity::isInfront(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isuserdata(L, 1));

    CLuaBaseEntity* PLuaBaseEntity = nullptr;

    auto angle = (uint8)(lua_gettop(L) > 1 ? lua_tointeger(L, 2) : 64);

    TPZ_DEBUG_BREAK_IF(PLuaBaseEntity == nullptr);

    lua_pushboolean(L, infront(m_PBaseEntity->loc.p, PLuaBaseEntity->GetBaseEntity()->loc.p, angle));
    return 1;
}

/************************************************************************
 *  Function: isBehind()
 *  Purpose : Returns true if an entity is behind another entity
 *  Example : if (attacker:isBehind(target)) then
 *  Notes   : Can specify angle for wider/narrower ranges
 ************************************************************************/

inline int32 CLuaBaseEntity::isBehind(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isuserdata(L, 1));

    CLuaBaseEntity* PLuaBaseEntity = nullptr;

    auto angle = (uint8)(lua_gettop(L) > 1 ? lua_tointeger(L, 2) : 64);

    TPZ_DEBUG_BREAK_IF(PLuaBaseEntity == nullptr);

    lua_pushboolean(L, behind(m_PBaseEntity->loc.p, PLuaBaseEntity->GetBaseEntity()->loc.p, angle));
    return 1;
}

/************************************************************************
 *  Function: isBeside()
 *  Purpose : Returns true if an entity is to the side of another entity
 *  Example : if (attacker:isBeside(target)) then
 *  Notes   : Can specify angle for wider/narrower ranges
 ************************************************************************/

inline int32 CLuaBaseEntity::isBeside(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isuserdata(L, 1));

    CLuaBaseEntity* PLuaBaseEntity = nullptr;

    auto angle = (uint8)(lua_gettop(L) > 1 ? lua_tointeger(L, 2) : 64);

    TPZ_DEBUG_BREAK_IF(PLuaBaseEntity == nullptr);

    lua_pushboolean(L, beside(m_PBaseEntity->loc.p, PLuaBaseEntity->GetBaseEntity()->loc.p, angle));
    return 1;
}

/************************************************************************
 *  Function: getZone(isZoning)
 *  Purpose : Returns a pointer to a zone object?
 *  Example : if (player:getZone() == mob:getZone()) then
 *  Notes   : To Do: I don't think some scripts are using this correctly...
 *  Optional isZoning parameter will return player's destination zone if
 *  they are in the process of zoning (for use in onZoneIn)
 ************************************************************************/

inline int32 CLuaBaseEntity::getZone(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    if (m_PBaseEntity->loc.zone)
    {
        lua_getglobal(L, "CZone");
        lua_pushstring(L, "new");
        lua_gettable(L, -2);
        lua_insert(L, -2);
        lua_pushlightuserdata(L, (void*)m_PBaseEntity->loc.zone);
        lua_pcall(L, 2, 1, 0);
    }
    else if (m_PBaseEntity->loc.destination && !lua_isnil(L, 1) && lua_isboolean(L, 1) && lua_toboolean(L, 1) != 0)
    {
        auto* PZone = zoneutils::GetZone(m_PBaseEntity->loc.destination);

        lua_getglobal(L, "CZone");
        lua_pushstring(L, "new");
        lua_gettable(L, -2);
        lua_insert(L, -2);
        lua_pushlightuserdata(L, (void*)PZone);
        lua_pcall(L, 2, 1, 0);
    }
    else
    {
        lua_pushnil(L);
    }

    return 1;
}

/************************************************************************
 *  Function: getZoneID()
 *  Purpose : Returns the integer value associated with the current zone
 *  Example : if (player:getZoneID() == 50) then
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getZoneID(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    lua_pushinteger(L, m_PBaseEntity->getZone());
    return 1;
}

/************************************************************************
 *  Function: getZoneName()
 *  Purpose : Returns the string text assigned to the zone
 *  Example : require("scripts/zones/"..player:getZoneName().."/TextIDs")
 *  Notes   : Highly useful for the above example
 ************************************************************************/

inline int32 CLuaBaseEntity::getZoneName(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->loc.zone == nullptr);

    lua_pushstring(L, (const char*)m_PBaseEntity->loc.zone->GetName());
    return 1;
}

/************************************************************************
 *  Function: isZoneVisited()
 *  Purpose : Returns true if a player has ever visited the zone
 *  Example : if (target:isZoneVisited(4) == false) then
 *  Notes   : Mainly used for teleport items (like to Bibiki Bay)
 ************************************************************************/

inline int32 CLuaBaseEntity::isZoneVisited(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

    lua_pushboolean(L, hasBit((uint16)lua_tointeger(L, 1), PChar->m_ZonesList, sizeof(PChar->m_ZonesList)));
    return 1;
}

/************************************************************************
 *  Function: getPreviousZone()
 *  Purpose : Returns the integer ID of the last zone the PC visited
 *  Example : local prev = player:getPreviousZone()
 *  Notes   : Useful for returning players to their last position
 ************************************************************************/

inline int32 CLuaBaseEntity::getPreviousZone(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    lua_pushinteger(L, m_PBaseEntity->loc.prevzone);
    return 1;
}

/************************************************************************
 *  Function: getCurrentRegion()
 *  Purpose : Returns the integer value of the PC's region
 *  Example : local region = target:getCurrentRegion()
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getCurrentRegion(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    lua_pushinteger(L, static_cast<uint8>(zoneutils::GetCurrentRegion(m_PBaseEntity->getZone())));
    return 1;
}

/************************************************************************
 *  Function: getContinentID()
 *  Purpose : Returns the integer value of a continent
 *  Example : local ID = (player:getContinentID())
 *  Notes   : Used in Signet/Sanction applications
 ************************************************************************/

inline int32 CLuaBaseEntity::getContinentID(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    lua_pushinteger(L, static_cast<uint8>(m_PBaseEntity->loc.zone->GetContinentID()));
    return 1;
}

/************************************************************************
 *  Function: isInMogHouse()
 *  Purpose : Returns true if a PC is in their Mog House
 *  Example : if (player:isInMogHouse()) then -- watch Netflix and chill
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::isInMogHouse(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    lua_pushboolean(L, ((CCharEntity*)m_PBaseEntity)->m_moghouseID);
    return 1;
}
/************************************************************************
*  Function: getPlayerRegionInZone
*  Purpose : Returns the player's current region inside the zone
*  Example : local regionID = player:getPlayerRegionInZone()
*  Notes   : This refers to regions added via the registerRegion function
             Currently only used for port bastok drawbridge
************************************************************************/
inline int32 CLuaBaseEntity::getPlayerRegionInZone(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;
    lua_pushinteger(L, PChar->m_InsideRegionID);

    return 1;
}
/************************************************************************
*  Function: updateToEntireZone
*  Purpose : Updates NPC status/animation then sends an update packet zone-wide.
*  Example : drawBridge:updateToEntireZone()
*  Notes   : Currently only used for port bastok drawbridge as
             setAnimation() only updates for chars in range.
************************************************************************/
inline int32 CLuaBaseEntity::updateToEntireZone(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_NPC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));

    CNpcEntity* PNpc = (CNpcEntity*)m_PBaseEntity;

    PNpc->status    = static_cast<STATUS_TYPE>(lua_tointeger(L, 1));
    PNpc->animation = (uint8)lua_tointeger(L, 2);

    // If this flag is high, update the NPC's name to match the current time
    if (!lua_isnil(L, 3) && lua_toboolean(L, 3) != 0)
    {
        PNpc->name.resize(10);
        ref<uint32>(&PNpc->name[0], 4) = CVanaTime::getInstance()->getVanaTime();
        PNpc->name[8]                  = 8;
    }

    PNpc->loc.zone->PushPacket(nullptr, CHAR_INZONE, new CEntityUpdatePacket(PNpc, ENTITY_UPDATE, UPDATE_COMBAT));
    return 1;
}
/************************************************************************
 *  Function: getPos()
 *  Purpose : Returns a table of signed coordinates (x,y,z,rot)
 *  Example : local pos = battletarget:getPos() -- pos becomes a Lua table
 *  Notes   : Access values with key identifiers (pos.x or pos.y)
 ************************************************************************/

inline int32 CLuaBaseEntity::getPos(lua_State* L)
{
    lua_createtable(L, 4, 0);
    int8 newTable = lua_gettop(L);

    lua_pushnumber(L, m_PBaseEntity->loc.p.x);
    lua_setfield(L, newTable, "x");

    lua_pushnumber(L, m_PBaseEntity->loc.p.y);
    lua_setfield(L, newTable, "y");

    lua_pushnumber(L, m_PBaseEntity->loc.p.z);
    lua_setfield(L, newTable, "z");

    lua_pushnumber(L, m_PBaseEntity->loc.p.rotation);
    lua_setfield(L, newTable, "rot");

    return 1;
}

/************************************************************************
 *  Function: showPosition()
 *  Purpose : Displays PC's coordinates in standard message
 *  Example : player:showPosition()
 *  Notes   : Format: x,y,z,rot
 ************************************************************************/

inline int32 CLuaBaseEntity::showPosition(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    ((CCharEntity*)m_PBaseEntity)
        ->pushPacket(new CMessageStandardPacket((int32)m_PBaseEntity->loc.p.x, (int32)m_PBaseEntity->loc.p.y, (int32)m_PBaseEntity->loc.p.z,
                                                m_PBaseEntity->loc.p.rotation, MsgStd::Compass));
    return 0;
}

/************************************************************************
 *  Function: getXPos()
 *  Purpose : Returns a signed x coordinate
 *  Example : local x = player:getXPos()
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getXPos(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    lua_pushnumber(L, m_PBaseEntity->GetXPos());
    return 1;
}

/************************************************************************
 *  Function: getYPos()
 *  Purpose : Returns a signed y coordinate
 *  Example : local y = player:getYPos()
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getYPos(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    lua_pushnumber(L, m_PBaseEntity->GetYPos());
    return 1;
}

/************************************************************************
 *  Function: getZPos()
 *  Purpose : Returns a signed z coordinate
 *  Example : local z = player:getZPos()
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getZPos(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    lua_pushnumber(L, m_PBaseEntity->GetZPos());
    return 1;
}

/************************************************************************
 *  Function: getRotPos()
 *  Purpose : Returns an unsigned rotation coordinate
 *  Example : local rot = player:getRotPos()
 *  Notes   : Rot = Rotation of 0-359 degrees
 ************************************************************************/

inline int32 CLuaBaseEntity::getRotPos(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    lua_pushnumber(L, m_PBaseEntity->GetRotPos());
    return 1;
}

/************************************************************************
 *  Function: setPos()
 *  Purpose : Sends a PC to a new position
 *  Example : player:setPos(x,y,z,rot,zone) -- zone value is optional
 *  Notes   : Using without zone will send player to coordinates on same map
 ************************************************************************/

inline int32 CLuaBaseEntity::setPos(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    if (m_PBaseEntity->objtype == TYPE_PC)
    {
        if (!lua_isnil(L, 5) && lua_isnumber(L, 5) && ((CCharEntity*)m_PBaseEntity)->status == STATUS_TYPE::DISAPPEAR)
        {
            // do not modify zone/position if the character is already zoning
            return 0;
        }
    }

    if (lua_isnumber(L, 1))
    {
        if (!lua_isnil(L, 1) && lua_isnumber(L, 1))
        {
            m_PBaseEntity->loc.p.x = (float)lua_tonumber(L, 1);
        }
        if (!lua_isnil(L, 2) && lua_isnumber(L, 2))
        {
            m_PBaseEntity->loc.p.y = (float)lua_tonumber(L, 2);
        }
        if (!lua_isnil(L, 3) && lua_isnumber(L, 3))
        {
            m_PBaseEntity->loc.p.z = (float)lua_tonumber(L, 3);
        }
        if (!lua_isnil(L, 4) && lua_isnumber(L, 4))
        {
            m_PBaseEntity->loc.p.rotation = (uint8)lua_tointeger(L, 4);
        }
    }
    else
    {
        // its a table
        lua_rawgeti(L, 1, 1);
        m_PBaseEntity->loc.p.x = (float)lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_rawgeti(L, 1, 2);
        m_PBaseEntity->loc.p.y = (float)lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_rawgeti(L, 1, 3);
        m_PBaseEntity->loc.p.z = (float)lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_rawgeti(L, 1, 4);
        m_PBaseEntity->loc.p.rotation = (uint8)lua_tointeger(L, -1);
        lua_pop(L, 1);
    }

    if (m_PBaseEntity->objtype == TYPE_PC)
    {
        if (!lua_isnil(L, 5) && lua_isnumber(L, 5))
        {
            if ((uint16)lua_tointeger(L, 5) >= MAX_ZONEID)
            {
                return 0;
            }

            ((CCharEntity*)m_PBaseEntity)->loc.destination = (uint16)lua_tointeger(L, 5);
            ((CCharEntity*)m_PBaseEntity)->status          = STATUS_TYPE::DISAPPEAR;
            ((CCharEntity*)m_PBaseEntity)->loc.boundary    = 0;
            ((CCharEntity*)m_PBaseEntity)->m_moghouseID    = 0;
            ((CCharEntity*)m_PBaseEntity)->clearPacketList();
            charutils::SendToZone((CCharEntity*)m_PBaseEntity, 2, zoneutils::GetZoneIPP(m_PBaseEntity->loc.destination));
            //((CCharEntity*)m_PBaseEntity)->loc.zone->DecreaseZoneCounter(((CCharEntity*)m_PBaseEntity));
        }
        else if (((CCharEntity*)m_PBaseEntity)->status != STATUS_TYPE::DISAPPEAR)
        {
            ((CCharEntity*)m_PBaseEntity)->pushPacket(new CPositionPacket((CCharEntity*)m_PBaseEntity));
        }
    }
    m_PBaseEntity->updatemask |= UPDATE_POS;

    return 0;
}

/************************************************************************
 *  Function: warp()
 *  Purpose : Warps a PC to their established homepoint
 *  Example : player:warp()
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::warp(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    ((CCharEntity*)m_PBaseEntity)->loc.boundary    = 0;
    ((CCharEntity*)m_PBaseEntity)->m_moghouseID    = 0;
    ((CCharEntity*)m_PBaseEntity)->loc.p           = ((CCharEntity*)m_PBaseEntity)->profile.home_point.p;
    ((CCharEntity*)m_PBaseEntity)->loc.destination = ((CCharEntity*)m_PBaseEntity)->profile.home_point.destination;

    ((CCharEntity*)m_PBaseEntity)->status    = STATUS_TYPE::DISAPPEAR;
    ((CCharEntity*)m_PBaseEntity)->animation = ANIMATION_NONE;

    ((CCharEntity*)m_PBaseEntity)->clearPacketList();
    charutils::SendToZone((CCharEntity*)m_PBaseEntity, 2, zoneutils::GetZoneIPP(m_PBaseEntity->loc.destination));

    return 0;
}

/************************************************************************
 *  Function:teleport()
 *  Purpose : Teleports an entity to a position
 *  Example : mob:teleport(pos, battletarget);
 *  Notes   : scripts/globals/mobskills/tarutaru_warp_ii.lua
 ************************************************************************/

inline int32 CLuaBaseEntity::teleport(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1));

    lua_getfield(L, 1, "x");
    m_PBaseEntity->loc.p.x = (float)lua_tonumber(L, -1);
    lua_getfield(L, 1, "y");
    m_PBaseEntity->loc.p.y = (float)lua_tonumber(L, -1);
    lua_getfield(L, 1, "z");
    m_PBaseEntity->loc.p.z = (float)lua_tonumber(L, -1);

    if (lua_isnumber(L, 2))
    {
        m_PBaseEntity->loc.p.rotation = (uint8)lua_tonumber(L, 2);
    }
    else if (lua_isuserdata(L, 2))
    {
        CLuaBaseEntity* PLuaBaseEntity = nullptr;
        m_PBaseEntity->loc.p.rotation  = worldAngle(m_PBaseEntity->loc.p, PLuaBaseEntity->GetBaseEntity()->loc.p);
    }

    m_PBaseEntity->loc.zone->PushPacket(m_PBaseEntity, CHAR_INRANGE, new CPositionPacket(m_PBaseEntity));
    m_PBaseEntity->updatemask |= UPDATE_POS;
    return 0;
}

/************************************************************************
 *  Function: addTeleport(uint8 type, uint32 destination)
 *  Purpose : Grants acces to a new teleport for a PC
 *  Example : player:addTeleport(tpz.teleport.type.HOMEPOINT,16);
 *  Notes   : Param 2 is bits to shift, not exponentiated value
 ************************************************************************/

inline int32 CLuaBaseEntity::addTeleport(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

    TELEPORT_TYPE type = static_cast<TELEPORT_TYPE>(lua_tointeger(L, 1));
    uint32        bit  = 1 << (uint32)lua_tointeger(L, 2);
    uint8         set  = lua_isnil(L, 3) ? 0 : (uint8)lua_tointeger(L, 3);

    if ((type == TELEPORT_TYPE::HOMEPOINT || type == TELEPORT_TYPE::SURVIVAL) && (lua_isnil(L, 3) || set > 3))
    {
        ShowError("Lua::addteleport : Attempt to index array out-of-bounds or parameter is nil.");
        return 0;
    }

    switch (type)
    {
        case TELEPORT_TYPE::OUTPOST_SANDY:
            PChar->teleport.outpostSandy |= bit;
            break;
        case TELEPORT_TYPE::OUTPOST_BASTOK:
            PChar->teleport.outpostBastok |= bit;
            break;
        case TELEPORT_TYPE::OUTPOST_WINDY:
            PChar->teleport.outpostWindy |= bit;
            break;
        case TELEPORT_TYPE::RUNIC_PORTAL:
            PChar->teleport.runicPortal |= bit;
            break;
        case TELEPORT_TYPE::PAST_MAW:
            PChar->teleport.pastMaw |= bit;
            break;
        case TELEPORT_TYPE::CAMPAIGN_SANDY:
            PChar->teleport.campaignSandy |= bit;
            break;
        case TELEPORT_TYPE::CAMPAIGN_BASTOK:
            PChar->teleport.campaignBastok |= bit;
            break;
        case TELEPORT_TYPE::CAMPAIGN_WINDY:
            PChar->teleport.campaignWindy |= bit;
            break;
        case TELEPORT_TYPE::HOMEPOINT:
            PChar->teleport.homepoint.access[set] |= bit;
            break;
        case TELEPORT_TYPE::SURVIVAL:
            PChar->teleport.survival.access[set] |= bit;
            break;
        default:
            ShowError("LuaBaseEntity::addTeleport : Parameter 1 out of bounds.\n");
            return 0;
    }
    charutils::SaveTeleport(PChar, type);
    return 0;
}

/************************************************************************
 *  Function: getTeleport(uint8 type)
 *  Purpose : Returns bit mask or table for supplied type of teleport
 *  Example : player:getTeleport(tpz.teleport.type.HOMEPOINT)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getTeleport(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    TELEPORT_TYPE type  = static_cast<TELEPORT_TYPE>(lua_tointeger(L, 1));
    CCharEntity*  PChar = (CCharEntity*)m_PBaseEntity;

    switch (type)
    {
        case TELEPORT_TYPE::OUTPOST_SANDY:
            lua_pushinteger(L, PChar->teleport.outpostSandy);
            break;
        case TELEPORT_TYPE::OUTPOST_BASTOK:
            lua_pushinteger(L, PChar->teleport.outpostBastok);
            break;
        case TELEPORT_TYPE::OUTPOST_WINDY:
            lua_pushinteger(L, PChar->teleport.outpostWindy);
            break;
        case TELEPORT_TYPE::RUNIC_PORTAL:
            lua_pushinteger(L, PChar->teleport.runicPortal);
            break;
        case TELEPORT_TYPE::PAST_MAW:
            lua_pushinteger(L, PChar->teleport.pastMaw);
            break;
        case TELEPORT_TYPE::CAMPAIGN_SANDY:
            lua_pushinteger(L, PChar->teleport.campaignSandy);
            break;
        case TELEPORT_TYPE::CAMPAIGN_BASTOK:
            lua_pushinteger(L, PChar->teleport.campaignBastok);
            break;
        case TELEPORT_TYPE::CAMPAIGN_WINDY:
            lua_pushinteger(L, PChar->teleport.campaignWindy);
            break;
        case TELEPORT_TYPE::HOMEPOINT:
            lua_newtable(L);
            for (uint8 x = 0; x < 4; x++)
            {
                lua_pushnumber(L, PChar->teleport.homepoint.access[x]);
                lua_rawseti(L, -2, x + 1);
            }
            break;
        case TELEPORT_TYPE::SURVIVAL:
            lua_newtable(L);
            for (uint8 x = 0; x < 4; x++)
            {
                lua_pushnumber(L, PChar->teleport.survival.access[x]);
                lua_rawseti(L, -2, x + 1);
            }
            break;
        default:
            ShowError("LuaBaseEntity::getteleport : Parameter 1 out of bounds.\n");
            return 0;
    }
    return 1;
}

/************************************************************************
 *  Function: hasTeleport(uint8 type, uint8 bit, uint8 set (optional))
 *  Purpose : Returns true if player has HP, false otherwise
 *  Example : player:hasTeleport(tpz.teleport.type.HOMEPOINT, bit, set)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::hasTeleport(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

    TELEPORT_TYPE type = static_cast<TELEPORT_TYPE>(lua_tointeger(L, 1));
    uint8         bit  = (uint8)lua_tointeger(L, 2);
    uint8         set  = lua_isnil(L, 3) ? 0 : (uint8)lua_tointeger(L, 3);

    if (type == TELEPORT_TYPE::HOMEPOINT || type == TELEPORT_TYPE::SURVIVAL)
    {
        if (lua_isnil(L, 3) || set > 3)
        {
            ShowError("Lua::addTeleport : Attempt to index array out-of-bounds or parameter is nil.");
            return 0;
        }

        if (type == TELEPORT_TYPE::HOMEPOINT)
        {
            lua_pushboolean(L, PChar->teleport.homepoint.access[set] & (1 << bit));
        }
        else
        {
            lua_pushboolean(L, PChar->teleport.survival.access[set] & (1 << bit));
        }
        return 1;
    }

    switch (type)
    {
        case TELEPORT_TYPE::OUTPOST_SANDY:
            lua_pushboolean(L, PChar->teleport.outpostSandy & (1 << bit));
            break;
        case TELEPORT_TYPE::OUTPOST_BASTOK:
            lua_pushboolean(L, PChar->teleport.outpostBastok & (1 << bit));
            break;
        case TELEPORT_TYPE::OUTPOST_WINDY:
            lua_pushboolean(L, PChar->teleport.outpostWindy & (1 << bit));
            break;
        case TELEPORT_TYPE::RUNIC_PORTAL:
            lua_pushboolean(L, PChar->teleport.runicPortal & (1 << bit));
            break;
        case TELEPORT_TYPE::PAST_MAW:
            lua_pushboolean(L, PChar->teleport.pastMaw & (1 << bit));
            break;
        case TELEPORT_TYPE::CAMPAIGN_SANDY:
            lua_pushboolean(L, PChar->teleport.campaignSandy & (1 << bit));
            break;
        case TELEPORT_TYPE::CAMPAIGN_BASTOK:
            lua_pushboolean(L, PChar->teleport.campaignBastok & (1 << bit));
            break;
        case TELEPORT_TYPE::CAMPAIGN_WINDY:
            lua_pushboolean(L, PChar->teleport.campaignWindy & (1 << bit));
            break;
        default:
            ShowError("LuaBaseEntity::hasTeleport : Parameter 1 out of bounds.\n");
            return 0;
    }
    return 1;
}

/************************************************************************
 *  Function: setTeleportMenu(uint8 type)
 *  Purpose : Store favorite homepoints or menu layout
 *  Example : player:setTeleportMenu(tpz.teleport.type.HOMEPOINT)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::setTeleportMenu(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

    TELEPORT_TYPE type = static_cast<TELEPORT_TYPE>(lua_tointeger(L, 1));
    if (!lua_istable(L, 2))
    {
        ShowError("LuaBaseEntity::setteleportMenu : Table not passed in Parameter 2.\n");
        return 0;
    }

    if (type != TELEPORT_TYPE::HOMEPOINT && type != TELEPORT_TYPE::SURVIVAL)
    {
        ShowError("LuaBaseEntity::setteleportMenu : Incorrect value for Parameter 1.\n");
        return 0;
    }

    uint8 x     = 0;
    auto  index = lua_gettop(L);
    lua_pushnil(L);

    while (lua_next(L, index))
    {
        if (type == TELEPORT_TYPE::HOMEPOINT)
        {
            PChar->teleport.homepoint.menu[x++] = (int32)lua_tointeger(L, -1);
        }
        else
        {
            PChar->teleport.survival.menu[x++] = (int32)lua_tointeger(L, -1);
        }
        lua_pop(L, 1);
    }

    charutils::SaveTeleport(PChar, type);
    return 0;
}

/************************************************************************
 *  Function: getTeleportMenu(uint8)
 *  Purpose : Return lua table containing integer values for favs + layout
 *  Example : player:getTeleportMenu(tpz.teleport.teleport.HOMEPOINT)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getTeleportMenu(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    CCharEntity*  PChar = (CCharEntity*)m_PBaseEntity;
    TELEPORT_TYPE type  = static_cast<TELEPORT_TYPE>(lua_tointeger(L, 1));

    if (type != TELEPORT_TYPE::HOMEPOINT && type != TELEPORT_TYPE::SURVIVAL)
    {
        ShowError("LuaBaseEntity::getTeleportMenu : Incorrect value or parameter 1.\n");
        return 0;
    }

    lua_newtable(L);

    for (uint8 x = 0; x < 10; x++)
    {
        if (type == TELEPORT_TYPE::HOMEPOINT)
        {
            lua_pushnumber(L, PChar->teleport.homepoint.menu[x]);
        }
        else
        {
            lua_pushnumber(L, PChar->teleport.survival.menu[x]);
        }
        lua_rawseti(L, -2, x + 1);
    }
    return 1;
}

/************************************************************************
 *  Function: setHomePoint()
 *  Purpose : Sets a PC's homepoint.
 *  Example : player:setHomePoint(tpz.teleport.type.HOMEPOINT)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::setHomePoint(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

    PChar->profile.home_point.p           = PChar->loc.p;
    PChar->profile.home_point.destination = PChar->getZone();

    const char* fmtQuery = "UPDATE chars \
                            SET home_zone = %u, home_rot = %u, home_x = %.3f, home_y = %.3f, home_z = %.3f \
                            WHERE charid = %u;";

    Sql_Query(SqlHandle, fmtQuery, PChar->profile.home_point.destination, PChar->profile.home_point.p.rotation, PChar->profile.home_point.p.x,
              PChar->profile.home_point.p.y, PChar->profile.home_point.p.z, PChar->id);
    return 0;
}

/************************************************************************
 *  Function: resetPlayer()
 *  Purpose : Delete player's account session and send them to Lower Jeuno
 *  Example : player:resetPlayer()
 *  Notes   : Function name is ambiguous
 ************************************************************************/

inline int32 CLuaBaseEntity::resetPlayer(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1));

    const char* charName = lua_tostring(L, 1);
    uint32      id       = 0;

    // char will not be logged in so get the id manually
    const char* Query = "SELECT charid FROM chars WHERE charname = '%s';";
    int32       ret   = Sql_Query(SqlHandle, Query, charName);

    if (ret != SQL_ERROR && Sql_NumRows(SqlHandle) != 0 && Sql_NextRow(SqlHandle) == SQL_SUCCESS)
    {
        id = (int32)Sql_GetIntData(SqlHandle, 0);
    }

    // could not get player from database
    if (id == 0)
    {
        ShowDebug("Could not get the character from database.\n");
        return 1;
    }

    // delete the account session
    Query = "DELETE FROM accounts_sessions WHERE charid = %u;";
    Sql_Query(SqlHandle, Query, id);

    // send the player to lower jeuno
    Query = "UPDATE chars "
            "SET "
            "pos_zone = %u,"
            "pos_prevzone = %u,"
            "pos_rot = %u,"
            "pos_x = %.3f,"
            "pos_y = %.3f,"
            "pos_z = %.3f,"
            "boundary = %u,"
            "moghouse = %u "
            "WHERE charid = %u;";

    Sql_Query(SqlHandle, Query,
              245,     // lower jeuno
              122,     // prev zone
              86,      // rotation
              33.464f, // x
              -5.000f, // y
              69.162f, // z
              0,       // boundary,
              0,       // moghouse,
              id);

    ShowDebug("Player reset was successful.\n");

    return 1;
}

/************************************************************************
 *  Function: goToEntity()
 *  Purpose : Transports PC to a Mob or NPC; works across multiple servers
 *  Example : player:goToEntity(ID, Option)
 *  Notes   : Option 0: Spawned/Unspawned | Option 1: Spawned only
 ************************************************************************/

int32 CLuaBaseEntity::goToEntity(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    if (!lua_isnil(L, 1) && lua_isnumber(L, 1))
    {
        CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

        bool spawnedOnly = !lua_isnil(L, 2) ? lua_tonumber(L, 2) : false;

        uint32 targetID   = (uint32)lua_tonumber(L, 1);
        uint16 targetZone = (targetID >> 12) & 0x0FFF;
        uint16 playerID   = m_PBaseEntity->id;
        uint16 playerZone = PChar->loc.zone->GetID();

        char buf[12];
        memset(&buf[0], 0, sizeof(buf));

        ref<bool>(&buf, 0)    = true;        // Toggle for message routing; goes to entity server first
        ref<bool>(&buf, 1)    = spawnedOnly; // Specification for Spawned Only or Any
        ref<uint16>(&buf, 2)  = targetZone;
        ref<uint16>(&buf, 4)  = playerZone;
        ref<uint32>(&buf, 6)  = targetID;
        ref<uint16>(&buf, 10) = playerID;

        message::send(MSG_SEND_TO_ENTITY, &buf[0], sizeof(buf), nullptr);
    }
    return 0;
}

/************************************************************************
 *  Function: gotoPlayer()
 *  Purpose : Transports PC to another PC
 *  Example : player:gotoPlayer(playername)
 *  Notes   :
 ************************************************************************/

int32 CLuaBaseEntity::gotoPlayer(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    bool found = false;
    if (!lua_isnil(L, 1) && lua_isstring(L, 1))
    {
        const char* fmtQuery = "SELECT charid FROM chars WHERE charname = '%s';";
        int32       ret      = Sql_Query(SqlHandle, fmtQuery, std::string(lua_tostring(L, 1)).c_str());

        if (ret != SQL_ERROR && Sql_NumRows(SqlHandle) != 0 && Sql_NextRow(SqlHandle) == SQL_SUCCESS)
        {
            char buf[30];
            memset(&buf[0], 0, sizeof(buf));

            ref<uint16>(&buf, 0) = Sql_GetUIntData(SqlHandle, 0); // target char
            ref<uint16>(&buf, 4) = m_PBaseEntity->id;             // warping to target char, their server will send us a zoning message with their pos

            message::send(MSG_SEND_TO_ZONE, &buf[0], sizeof(buf), nullptr);
            found = true;
        }
    }
    lua_pushboolean(L, found);
    return 1;
}

/************************************************************************
 *  Function: bringPlayer()
 *  Purpose : Brings a PC to another PC; returns true if success
 *  Example : player:bringPlayer(playername)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::bringPlayer(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    bool found = false;

    if (!lua_isnil(L, 1) && lua_isstring(L, 1))
    {
        const char* fmtQuery = "SELECT charid FROM chars WHERE charname = '%s';";
        int32       ret      = Sql_Query(SqlHandle, fmtQuery, std::string(lua_tostring(L, 1)).c_str());
        if (ret != SQL_ERROR && Sql_NumRows(SqlHandle) != 0 && Sql_NextRow(SqlHandle) == SQL_SUCCESS)
        {
            char buf[30];
            memset(&buf[0], 0, sizeof(buf));

            ref<uint16>(&buf, 0)  = Sql_GetUIntData(SqlHandle, 0); // target char
            ref<uint16>(&buf, 4)  = 0;                             // wanting to bring target char here so wont give our id
            ref<uint16>(&buf, 8)  = m_PBaseEntity->getZone();
            ref<uint16>(&buf, 10) = (uint16)m_PBaseEntity->loc.p.x;
            ref<uint16>(&buf, 14) = (uint16)m_PBaseEntity->loc.p.y;
            ref<uint16>(&buf, 18) = (uint16)m_PBaseEntity->loc.p.z;
            ref<uint8>(&buf, 22)  = m_PBaseEntity->loc.p.rotation;

            if (m_PBaseEntity->objtype == TYPE_PC)
            {
                ref<uint32>(&buf, 23) = ((CCharEntity*)m_PBaseEntity)->m_moghouseID;
            }

            message::send(MSG_SEND_TO_ZONE, &buf[0], sizeof(buf), nullptr);
            found = true;
        }
    }
    lua_pushboolean(L, found);
    return 1;
}

/************************************************************************
 *  Function: getEquipID()
 *  Purpose : Returns the Item ID for an item
 *  Example : player:getEquipID(SLOT_MAIN)
 *  Notes   :
 ************************************************************************/

uint16 CLuaBaseEntity::getEquipID(SLOTTYPE slot)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC && m_PBaseEntity->objtype != TYPE_PET && m_PBaseEntity->objtype != TYPE_MOB);

    if (m_PBaseEntity->objtype == TYPE_PC)
    {
        TPZ_DEBUG_BREAK_IF(slot > 15);

        CCharEntity* PChar = dynamic_cast<CCharEntity*>(m_PBaseEntity);
        CItem*       PItem = PChar->getEquip(slot);

        if (PItem && PItem->isType(ITEM_EQUIPMENT))
        {
            return PItem->getID();
        }
    }

    return 0;
}

/************************************************************************
 *  Function: getEquippedItem()
 *  Purpose : Returns the Item for a given slot
 *  Example : player:getEquippedItem(SLOT_MAIN)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getEquippedItem(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    if (m_PBaseEntity->objtype == TYPE_PC)
    {
        uint8 SLOT = (uint8)lua_tointeger(L, 1);
        TPZ_DEBUG_BREAK_IF(SLOT > 15);

        auto* PChar    = static_cast<CCharEntity*>(m_PBaseEntity);
        auto* slotItem = PChar->getEquip((SLOTTYPE)SLOT);
        if (slotItem)
        {
            lua_getglobal(L, "CItem");
            lua_pushstring(L, "new");
            lua_gettable(L, -2);
            lua_insert(L, -2);
            lua_pushlightuserdata(L, (void*)slotItem);

            if (lua_pcall(L, 2, 1, 0))
            {
                return 0;
            }
            return 1;
        }
    }
    lua_pushnil(L);
    return 1;
}

/************************************************************************
 *  Function: hasItem()
 *  Purpose : Returns true if a player possesses an item
 *  Example : if (player:hasItem(500) --Second var optional
 *  Notes   : Send with an L2 value to specify container
 ************************************************************************/

inline int32 CLuaBaseEntity::hasItem(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

    uint16 ItemID = (uint16)lua_tointeger(L, 1);

    if (!lua_isnil(L, 2) && lua_isnumber(L, 2))
    {
        uint8 locationID = LOC_INVENTORY;

        locationID = (uint8)lua_tointeger(L, 2);
        locationID = (locationID < MAX_CONTAINER_ID ? locationID : LOC_INVENTORY);

        lua_pushboolean(L, PChar->getStorage(locationID)->SearchItem(ItemID) != ERROR_SLOTID);
        return 1;
    }
    lua_pushboolean(L, charutils::HasItem(PChar, ItemID));
    return 1;
}

/************************************************************************
 *  Function: addItem()
 *  Purpose : Adds an item to a player's inventory
 *  Example : player:addItem(4102,12) -- a stack of Light Crystals
 *  Notes   : See format and variable options below
 ************************************************************************/

inline int32 CLuaBaseEntity::addItem(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1));

    uint8 SlotID = ERROR_SLOTID;

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

    /* FORMAT 1:
    player:addItem({id=itemID, quantity=quantity}) -- add quantity of itemID

    player:addItem({id=itemID, silent=true}) -- silently add 1 of itemID

    player:addItem({id=itemID, signature="Char"}) -- add 1 signed of itemID
    player:addItem({id=itemID, augments={[4]=5,[10]=10}}) -- add 1 of itemID with augment id 4 and 10,
        with values of 5 and 10, respectively
    */

    if (lua_istable(L, 1))
    {
        lua_getfield(L, 1, "id");
        if (lua_isnil(L, -1))
        {
            ShowError("AddItem: id is nil");
            lua_pop(L, 1);
            lua_pushboolean(L, 1);
            return 1;
        }

        uint16 id = (uint16)lua_tointeger(L, -1);
        lua_getfield(L, 1, "quantity");
        int32 quantity = (int32)lua_tointeger(L, -1);
        if (quantity == 0)
        {
            quantity = 1;
        }
        lua_pop(L, 2);

        while (PChar->getStorage(LOC_INVENTORY)->GetFreeSlotsCount() != 0 && quantity > 0)
        {
            if (CItem* PItem = itemutils::GetItem(id))
            {
                PItem->setQuantity(quantity);
                quantity -= PItem->getStackSize();

                lua_getfield(L, 1, "silent");
                bool silent = false;
                if (!lua_isnil(L, -1))
                {
                    silent = lua_toboolean(L, -1);
                }
                lua_getfield(L, 1, "signature");
                const auto* signature = lua_tostring(L, -1);
                if (signature)
                {
                    int8 encoded[12];
                    PItem->setSignature(EncodeStringSignature((int8*)signature, encoded));
                }
                lua_pop(L, 2);

                if (PItem->isType(ITEM_EQUIPMENT))
                {
                    lua_getfield(L, 1, "trial");
                    uint16 trial = (uint16)lua_tointeger(L, -1);
                    if (trial != 0)
                    {
                        ((CItemEquipment*)PItem)->setTrialNumber(trial);
                    }
                    lua_getfield(L, 1, "augments");
                    if (lua_istable(L, -1))
                    {
                        auto table = lua_gettop(L);
                        lua_pushnil(L);
                        while (lua_next(L, table) != 0)
                        {
                            uint16 augid  = (uint16)lua_tointeger(L, -2);
                            uint8  augval = (uint8)lua_tointeger(L, -1);
                            ((CItemEquipment*)PItem)->PushAugment(augid, augval);
                            lua_pop(L, 1);
                        }
                    }
                    lua_pop(L, 2);
                }
                SlotID = charutils::AddItem(PChar, LOC_INVENTORY, PItem, silent);
                if (SlotID == ERROR_SLOTID)
                {
                    break;
                }
            }
            else
            {
                ShowWarning(CL_YELLOW "charplugin::AddItem: Item <%i> is not found in a database\n" CL_RESET, id);
                break;
            }
        }
    }
    else
    {
        /* FORMAT 2:
        player:addItem(itemID, quantity) -- add quantity of itemID

        player:addItem(itemID, true) -- silently add 1 of itemID

        player:addItem(itemID, quantity, true) -- silently add quantity of itemID
        */

        bool   silence     = false;
        uint16 itemID      = (uint16)lua_tointeger(L, 1);
        int32  quantity    = 1;
        uint16 augment0    = 0;
        uint8  augment0val = 0;
        uint16 augment1    = 0;
        uint8  augment1val = 0;
        uint16 augment2    = 0;
        uint8  augment2val = 0;
        uint16 augment3    = 0;
        uint8  augment3val = 0;
        uint16 trialNumber = 0;

        if (!lua_isnil(L, 2) && lua_isboolean(L, 2))
        {
            silence = lua_toboolean(L, 2);
        }
        else if (!lua_isnil(L, 2) && lua_isnumber(L, 2))
        {
            quantity = (int32)lua_tointeger(L, 2);
            if (!lua_isnil(L, 3) && lua_isboolean(L, 3))
            {
                silence = lua_toboolean(L, 3);
            }
        }

        if (!lua_isnil(L, 3) && lua_isnumber(L, 3))
        {
            augment0 = (uint16)lua_tointeger(L, 3);
        }
        if (!lua_isnil(L, 4) && lua_isnumber(L, 4))
        {
            augment0val = (uint8)lua_tointeger(L, 4);
        }
        if (!lua_isnil(L, 5) && lua_isnumber(L, 5))
        {
            augment1 = (uint16)lua_tointeger(L, 5);
        }
        if (!lua_isnil(L, 6) && lua_isnumber(L, 6))
        {
            augment1val = (uint8)lua_tointeger(L, 6);
        }
        if (!lua_isnil(L, 7) && lua_isnumber(L, 7))
        {
            augment2 = (uint16)lua_tointeger(L, 7);
        }
        if (!lua_isnil(L, 8) && lua_isnumber(L, 8))
        {
            augment2val = (uint8)lua_tointeger(L, 8);
        }
        if (!lua_isnil(L, 9) && lua_isnumber(L, 9))
        {
            augment3 = (uint16)lua_tointeger(L, 9);
        }
        if (!lua_isnil(L, 10) && lua_isnumber(L, 10))
        {
            augment3val = (uint8)lua_tointeger(L, 10);
        }

        if (!lua_isnil(L, 11) && lua_isnumber(L, 11))
        {
            trialNumber = (uint16)lua_tointeger(L, 11);
        }

        while (PChar->getStorage(LOC_INVENTORY)->GetFreeSlotsCount() != 0 && quantity > 0)
        {
            if (CItem* PItem = itemutils::GetItem(itemID))
            {
                PItem->setQuantity(quantity);
                quantity -= PItem->getStackSize();

                if (PItem->isType(ITEM_EQUIPMENT))
                {
                    if (augment0 != 0)
                    {
                        ((CItemEquipment*)PItem)->setAugment(0, augment0, augment0val);
                    }
                    if (augment1 != 0)
                    {
                        ((CItemEquipment*)PItem)->setAugment(1, augment1, augment1val);
                    }
                    if (augment2 != 0)
                    {
                        ((CItemEquipment*)PItem)->setAugment(2, augment2, augment2val);
                    }
                    if (augment3 != 0)
                    {
                        ((CItemEquipment*)PItem)->setAugment(3, augment3, augment3val);
                    }
                    if (trialNumber != 0)
                    {
                        ((CItemEquipment*)PItem)->setTrialNumber(trialNumber);
                    }
                }
                SlotID = charutils::AddItem(PChar, LOC_INVENTORY, PItem, silence);

                // Paranoid check
                if (SlotID == ERROR_SLOTID)
                {
                    break;
                }
            }
            else
            {
                ShowWarning(CL_YELLOW "charplugin::AddItem: Item <%i> is not found in a database\n" CL_RESET, itemID);
                break;
            }
        }
    }

    lua_pushboolean(L, (SlotID != ERROR_SLOTID));
    return 1;
}

/************************************************************************
 *  Function: delItem()
 *  Purpose : Deletes an item from a player's inventory
 *  Example : player:delItem(4102,12)
 *  Notes   : Can specify contianer using third variable
 ************************************************************************/

int32 CLuaBaseEntity::delItem(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));
    auto quantity = 0;
    auto location = 0;

    if (!lua_isnil(L, 2) && lua_isnumber(L, 2))
    {
        quantity = (uint32)lua_tointeger(L, 2);
    }

    if (!lua_isnil(L, 3) && lua_isnumber(L, 3))
    {
        if ((uint32)lua_tointeger(L, 3) < MAX_CONTAINER_ID)
        {
            location = (uint32)lua_tointeger(L, 3);
        }
        else
        {
            ShowWarning(CL_YELLOW "Lua::delItem: Attempting to delete an item from an invalid slot. Defaulting to main inventory.\n" CL_RESET);
        }
    }

    auto* PChar  = static_cast<CCharEntity*>(m_PBaseEntity);
    auto  SlotID = PChar->getStorage(location)->SearchItem((uint16)lua_tointeger(L, 1));
    if (SlotID != ERROR_SLOTID)
    {
        charutils::UpdateItem(PChar, location, SlotID, -quantity);
        lua_pushboolean(L, true);
        PChar->pushPacket(new CInventoryFinishPacket());
        return 1;
    }
    lua_pushboolean(L, false);
    return 1;
}

/************************************************************************
 *  Function: addUsedItem()
 *  Purpose : Add charged item with use timer already on full cooldown
 *  Example : player:addUsedItem(17040) -- Warp Cudgel
 *  Notes   : Currently unused, but should be
 ************************************************************************/

inline int32 CLuaBaseEntity::addUsedItem(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    uint16       itemID = (uint16)lua_tointeger(L, 1);
    CCharEntity* PChar  = (CCharEntity*)m_PBaseEntity;
    uint8        SlotID = ERROR_SLOTID;

    if (PChar->getStorage(LOC_INVENTORY)->GetFreeSlotsCount() != 0)
    {
        CItem* PItem = itemutils::GetItem(itemID);

        if (PItem != nullptr)
        {
            if (PItem->isSubType(ITEM_CHARGED))
            {
                auto* PUsable = static_cast<CItemUsable*>(PItem);
                PUsable->setQuantity(1);
                PUsable->setLastUseTime(CVanaTime::getInstance()->getVanaTime());
                SlotID = charutils::AddItem(PChar, LOC_INVENTORY, PUsable, false);
            }
            else
            {
                ShowWarning(CL_YELLOW "addUsedItem: tried to setLastUseTime but itemID <%i> is not type ITEM_CHARGED\n" CL_RESET, itemID);
            }
        }
        else
        {
            ShowWarning(CL_YELLOW "charplugin::AddItem: Item <%i> is not found in a database\n" CL_RESET, itemID);
        }
    }
    lua_pushboolean(L, (SlotID != ERROR_SLOTID));
    return 1;
}

/************************************************************************
 *  Function: hasWornItem()
 *  Purpose : Returns true if a player has a worn (unusable) item
 *  Example : if (player:hasWornItem(trade:getItemId())) then
 *  Notes   : Used mainly for Testimonies and BCNM orbs
 ************************************************************************/

inline int32 CLuaBaseEntity::hasWornItem(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    CCharEntity* PChar  = (CCharEntity*)m_PBaseEntity;
    uint8        slotID = PChar->getStorage(LOC_INVENTORY)->SearchItem((uint16)lua_tointeger(L, 1));

    if (slotID != ERROR_SLOTID)
    {
        CItem* PItem = PChar->getStorage(LOC_INVENTORY)->GetItem(slotID);

        lua_pushboolean(L, PItem->m_extra[0] == 1);
        return 1;
    }
    return 0;
}

/************************************************************************
 *  Function: createWornItem()
 *  Purpose : Flags an item as being used up (worn)
 *  Example : player:createWornItem(itemID)
 *  Notes   : Prevent Orbs and Testimonies from being used again
 ************************************************************************/

inline int32 CLuaBaseEntity::createWornItem(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    CCharEntity* PChar  = (CCharEntity*)m_PBaseEntity;
    uint8        slotID = PChar->getStorage(LOC_INVENTORY)->SearchItem((uint16)lua_tointeger(L, 1));

    if (slotID != ERROR_SLOTID)
    {
        CItem* PItem      = PChar->getStorage(LOC_INVENTORY)->GetItem(slotID);
        PItem->m_extra[0] = 1;

        char extra[sizeof(PItem->m_extra) * 2 + 1];
        Sql_EscapeStringLen(SqlHandle, extra, (const char*)PItem->m_extra, sizeof(PItem->m_extra));

        const char* Query = "UPDATE char_inventory "
                            "SET extra = '%s' "
                            "WHERE charid = %u AND location = %u AND slot = %u;";

        Sql_Query(SqlHandle, Query, extra, PChar->id, PItem->getLocationID(), PItem->getSlotID());
    }

    return 0;
}

/************************************************************************
 *  Function: addTempItem()
 *  Purpose : Add a temporary item to the player
 *  Example : player:addTempItem(5399) -- Azouph Fireflies
 *  Notes   : Used almost exclusively for instances
 ************************************************************************/

inline int32 CLuaBaseEntity::addTempItem(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    uint16 itemID   = (uint16)lua_tointeger(L, 1);
    uint32 quantity = 1;

    if (!lua_isnil(L, 2) && lua_isnumber(L, 2))
    {
        quantity = (uint32)lua_tointeger(L, 2);
    }

    uint8 SlotID = ERROR_SLOTID;

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

    if (PChar->getStorage(LOC_TEMPITEMS)->GetFreeSlotsCount() != 0 && quantity != 0)
    {
        CItem* PItem = itemutils::GetItem(itemID);

        if (PItem != nullptr)
        {
            PItem->setQuantity(quantity);

            SlotID = charutils::AddItem(PChar, LOC_TEMPITEMS, PItem);
        }
        else
        {
            ShowWarning(CL_YELLOW "charplugin::AddItem: Item <%i> is not found in a database\n" CL_RESET, itemID);
        }
    }
    lua_pushboolean(L, (SlotID != ERROR_SLOTID));
    return 1;
}

/************************************************************************
 *  Function: createShop()
 *  Purpose : Create a temporary shop for display to a player
 *  Example : player:createShop(Size, Nation);
 *  Notes   : Used solely in scripts/globals/shop.lua
 ************************************************************************/

inline int32 CLuaBaseEntity::createShop(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    ((CCharEntity*)m_PBaseEntity)->Container->Clean();
    ((CCharEntity*)m_PBaseEntity)->Container->setSize((uint8)lua_tointeger(L, 1) + 1);

    if (!lua_isnil(L, 2) && lua_isnumber(L, 2))
    {
        ((CCharEntity*)m_PBaseEntity)->Container->setType((uint8)lua_tointeger(L, 2));
    }
    return 0;
}

/************************************************************************
 *  Function: addShopItem()
 *  Purpose : Adds an item and established price to an existing shop
 *          : Optionally accepts a GuildID + Guild Rank requirement
 *  Example : addShopItem(512, 8000)                                                   --Regular item
 *          : addShopItem(512, 8000, tpz.skill.CLOTHCRAFT, tpz.craftRank.JOURNEYMAN)   --Guild-rank locked item
 *  Notes   : Use with createShop() - 16 Max Items in Shop
 ************************************************************************/

inline int32 CLuaBaseEntity::addShopItem(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));

    uint16 itemID = (uint16)lua_tonumber(L, 1);
    uint32 price  = (uint32)lua_tonumber(L, 2);

    CCharEntity* PChar  = (CCharEntity*)m_PBaseEntity;
    uint8        slotID = PChar->Container->getItemsCount();

    PChar->Container->setItem(slotID, itemID, 0, price);

    // Players can add items to the shop container during shop interaction,
    // so track the shop's number of items separately from the container's size.
    PChar->Container->setExSize(PChar->Container->getExSize() + 1);

    if (lua_isnumber(L, 3) && lua_isnumber(L, 4))
    {
        uint8  guildID   = (uint8)lua_tonumber(L, 3);
        uint16 guildRank = (uint16)lua_tonumber(L, 4);

        ((CCharEntity*)m_PBaseEntity)->Container->setGuildID(slotID, guildID);
        ((CCharEntity*)m_PBaseEntity)->Container->setGuildRank(slotID, guildRank);
    }
    return 0;
}

/************************************************************************
 *  Function: getCurrentGPItem()
 *  Purpose : Returns the current Guild Point Item needed
 *  Example : player:getCurrentGPItem(guildID)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getCurrentGPItem(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    auto GuildID = (uint8)lua_tonumber(L, 1);

    CGuild*      PGuild = guildutils::GetGuild(GuildID);
    CCharEntity* PChar  = (CCharEntity*)m_PBaseEntity;

    auto GPItem = PGuild->getDailyGPItem(PChar);

    lua_pushinteger(L, GPItem.first);
    lua_pushinteger(L, GPItem.second);

    return 2;
}

/************************************************************************
 *  Function: breakLinkshell()
 *  Purpose : Breaks linkshell and all pearls/sacks
 *  Example : player:breakLinkshell(LSname)
 *  Notes   : Used by GMs to break a linkshell
 ************************************************************************/

inline int32 CLuaBaseEntity::breakLinkshell(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isstring(L, 1));

    const auto* lsname = lua_tostring(L, 1);
    bool        found  = false;

    int32 ret = Sql_Query(SqlHandle, "SELECT broken, linkshellid FROM linkshells WHERE name = '%s'", lsname);
    if (ret != SQL_ERROR && Sql_NumRows(SqlHandle) != 0 && Sql_NextRow(SqlHandle) == SQL_SUCCESS)
    {
        uint8 broken = Sql_GetUIntData(SqlHandle, 0);
        if (broken)
        {
            lua_pushboolean(L, true);
            return 1;
        }
        uint32      lsid       = Sql_GetUIntData(SqlHandle, 1);
        CLinkshell* PLinkshell = linkshell::GetLinkshell(lsid);
        if (!PLinkshell)
        {
            PLinkshell = linkshell::LoadLinkshell(lsid);
        }
        int8 EncodedName[16];
        EncodeStringLinkshell((int8*)lsname, EncodedName);
        PLinkshell->BreakLinkshell(EncodedName, true);
        linkshell::UnloadLinkshell(lsid);
        found = true;
    }

    lua_pushboolean(L, found);
    return 1;
}

/************************************************************************
 *  Function: getContainerSize()
 *  Purpose : Returns the size of an item container
 *  Example : player:getContainerSize(LOC_INVENTORY)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getContainerSize(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    CCharEntity* PChar = ((CCharEntity*)m_PBaseEntity);
    lua_pushinteger(L, PChar->getStorage((uint8)lua_tointeger(L, 1))->GetSize());
    return 1;
}

/************************************************************************
 *  Function: changeContainerSize()
 *  Purpose : Upgrades the capacity of a container
 *  Example : player:changeContainerSize(container,newSize)
 *  Notes   : Used primarily for Gobbie Bag quests
 ************************************************************************/

inline int32 CLuaBaseEntity::changeContainerSize(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    if (!lua_isnil(L, 1) && lua_isnumber(L, 1) && !lua_isnil(L, 2) && lua_isnumber(L, 2))
    {
        uint8 LocationID = (uint8)lua_tointeger(L, 1);

        if (LocationID < MAX_CONTAINER_ID)
        {
            CCharEntity* PChar = ((CCharEntity*)m_PBaseEntity);

            PChar->getStorage(LocationID)->AddBuff((int8)lua_tointeger(L, 2));
            PChar->pushPacket(new CInventorySizePacket(PChar));
            charutils::SaveCharInventoryCapacity(PChar);
        }
        else
        {
            ShowError(CL_RED "CLuaBaseEntity::changeContainerSize: bad container id (%u)\n", LocationID);
        }
    }
    return 0;
}

/************************************************************************
 *  Function: getFreeSlotsCount()
 *  Purpose : Returns the amount of free slots in a container
 *  Example : if (player:getFreeSlotsCount() == 0) then
 *  Notes   : Default slot is inventory; add value to specify containers
 ************************************************************************/

inline int32 CLuaBaseEntity::getFreeSlotsCount(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    uint8 locationID = LOC_INVENTORY;

    if (!lua_isnil(L, 1) && lua_isnumber(L, 1))
    {
        locationID = (uint8)lua_tointeger(L, 1);
        locationID = (locationID < MAX_CONTAINER_ID ? locationID : LOC_INVENTORY);
    }

    uint8 FreeSlots = ((CCharEntity*)m_PBaseEntity)->getStorage(locationID)->GetFreeSlotsCount();

    lua_pushinteger(L, FreeSlots);
    return 1;
}

/************************************************************************
 *  Function: confirmTrade()
 *  Purpose : Completes a trade and takes ONLY confirmed items
 *  Example : player:confirmTrade()
 *  Notes   : Must use trade:confirmItem(slotID) first
 ************************************************************************/

inline int32 CLuaBaseEntity::confirmTrade(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

    for (uint8 slotID = 0; slotID < TRADE_CONTAINER_SIZE; ++slotID)
    {
        if (PChar->TradeContainer->getInvSlotID(slotID) != 0xFF)
        {
            CItem* PItem = PChar->TradeContainer->getItem(slotID);
            if (PItem)
            {
                uint8 confirmedItems = PChar->TradeContainer->getConfirmedStatus(slotID);
                auto  quantity       = (int32)std::min<uint32>(PChar->TradeContainer->getQuantity(slotID), confirmedItems);

                PItem->setReserve(PItem->getReserve() - quantity);
                if (confirmedItems > 0)
                {
                    uint8 invSlotID = PChar->TradeContainer->getInvSlotID(slotID);
                    charutils::UpdateItem(PChar, LOC_INVENTORY, invSlotID, -quantity);
                }
            }
        }
    }
    PChar->TradeContainer->Clean();
    PChar->pushPacket(new CInventoryFinishPacket());
    return 0;
}

/************************************************************************
 *  Function: tradeComplete()
 *  Purpose : Completes trade and removes all items in trade container
 *  Example : player:tradeComplete()
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::tradeComplete(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

    for (uint8 slotID = 0; slotID < TRADE_CONTAINER_SIZE; ++slotID)
    {
        if (PChar->TradeContainer->getInvSlotID(slotID) != 0xFF)
        {
            uint8  invSlotID = PChar->TradeContainer->getInvSlotID(slotID);
            int32  quantity  = PChar->TradeContainer->getQuantity(slotID);
            CItem* PItem     = PChar->TradeContainer->getItem(slotID);
            if (PItem)
            {
                PItem->setReserve(0);
                charutils::UpdateItem(PChar, LOC_INVENTORY, invSlotID, -quantity);
            }
        }
    }
    PChar->TradeContainer->Clean();
    PChar->pushPacket(new CInventoryFinishPacket());
    return 0;
}

/************************************************************************
 *  Function: canEquipItem()
 *  Purpose : Returns true if a player can equip the item
 *  Example : if (player:canEquipItem(JOY_TOY)) then
 *  Notes   : CItemEquipment* is a pointer to weapons or armor
 ************************************************************************/

inline int32 CLuaBaseEntity::canEquipItem(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    uint16 itemID = (uint16)lua_tointeger(L, 1);
    TPZ_DEBUG_BREAK_IF(itemID > MAX_ITEMID);

    bool checkLevel = false;
    if (!lua_isnil(L, 2) && lua_isboolean(L, 2))
    {
        checkLevel = lua_toboolean(L, 2);
    }

    CItemEquipment* PItem = (CItemEquipment*)itemutils::GetItem(itemID);
    CBattleEntity*  PChar = (CBattleEntity*)m_PBaseEntity;

    if (!(PItem->getJobs() & (1 << (PChar->GetMJob() - 1))))
    {
        lua_pushboolean(L, false);
        return 1;
    }
    if (checkLevel && (PItem->getReqLvl() > PChar->GetMLevel()))
    {
        lua_pushboolean(L, false);
        return 1;
    }
    // ShowDebug("Item ID: %u Item Jobs: %u Player Job: %u\n",itemID,PItem->getJobs(),PChar->GetMJob());
    lua_pushboolean(L, true);
    return 1;
}

/************************************************************************
 *  Function: equipItem()
 *  Purpose : Equips an item on the player
 *  Example : equipItem(itemID, optional container ID)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::equipItem(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

    uint16 itemID = (uint16)lua_tointeger(L, 1);
    uint8  containerID;
    if (lua_isnil(L, 2) || !lua_isnumber(L, 2))
    {
        containerID = LOC_INVENTORY;
    }
    else
    {
        containerID = (uint8)lua_tointeger(L, 2);
    }
    uint8           SLOT = PChar->getStorage(containerID)->SearchItem(itemID);
    CItemEquipment* PItem;

    if (SLOT != ERROR_SLOTID)
    {
        PItem = (CItemEquipment*)PChar->getStorage(containerID)->GetItem(SLOT);
        charutils::EquipItem(PChar, SLOT, PItem->getSlotType(), containerID);
        charutils::SaveCharEquip(PChar);
        charutils::SaveCharLook(PChar);
    }
    return 0;
}

/************************************************************************
 *  Function: unequipItem()
 *  Purpose : Unequips an item from player
 *  Example : player:unequipItem(17845)
 *  Notes   :
 ************************************************************************/

int32 CLuaBaseEntity::unequipItem(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    if (m_PBaseEntity->objtype == TYPE_PC)
    {
        auto* PChar{ static_cast<CCharEntity*>(m_PBaseEntity) };
        charutils::UnequipItem(PChar, (uint8)lua_tointeger(L, 1));
    }
    return 0;
}

/************************************************************************
 *  Function: setEquipBlock()
 *  Purpose : Blocks player from equipping gear
 *  Example : target:setEquipBlock(effect:getPower())
 *  Notes   : Used exclusively for Encumbrance
 ************************************************************************/

int32 CLuaBaseEntity::setEquipBlock(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    if (m_PBaseEntity->objtype == TYPE_PC)
    {
        auto* PChar{ static_cast<CCharEntity*>(m_PBaseEntity) };
        PChar->m_EquipBlock = (uint16)lua_tointeger(L, 1);
        PChar->pushPacket(new CCharJobsPacket(PChar));
    }
    return 0;
}

/************************************************************************
 *  Function: lockEquipSlot()
 *  Purpose : Used to keep players from equipment certain equipment?
 *  Example : player:lockEquipSlot(SLOT)
 *  Notes   : Currently not implemented in any file, imagine this is for Salvage
 ************************************************************************/

inline int32 CLuaBaseEntity::lockEquipSlot(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    uint8 SLOT = (uint8)lua_tointeger(L, 1);

    TPZ_DEBUG_BREAK_IF(SLOT > 15);

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

    charutils::EquipItem(PChar, 0, SLOT, LOC_INVENTORY);

    PChar->m_EquipBlock |= 1 << SLOT;
    PChar->pushPacket(new CCharAppearancePacket(PChar));
    PChar->pushPacket(new CEquipPacket(0, SLOT, LOC_INVENTORY));
    PChar->pushPacket(new CCharJobsPacket(PChar));
    PChar->updatemask |= UPDATE_LOOK;

    return 0;
}

/************************************************************************
 *  Function: unlockEquipSlot()
 *  Purpose : Allows player to equip items in that slot again
 *  Example : player:unlockEquipSlot(SLOT)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::unlockEquipSlot(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    uint8 SLOT = (uint8)lua_tointeger(L, 1);

    TPZ_DEBUG_BREAK_IF(SLOT > 15);

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

    PChar->m_EquipBlock &= ~(1 << SLOT);
    PChar->pushPacket(new CCharJobsPacket(PChar));

    return 0;
}

/************************************************************************
 *  Function: getShieldSize()
 *  Purpose : Return the shield size of the equipped shield
 *  Example : player:getShieldSize()
 *  Notes   : Returns 0 if player does not have shield equipped
 ************************************************************************/

inline int32 CLuaBaseEntity::getShieldSize(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC && m_PBaseEntity->objtype != TYPE_PET);

    if (m_PBaseEntity->objtype == TYPE_PC)
    {
        lua_pushinteger(L, ((CCharEntity*)m_PBaseEntity)->getShieldSize());
        return 1;
    }

    lua_pushinteger(L, 0);
    return 1;
}

/************************************************************************
 *  Function: hasGearSetMod()
 *  Purpose : Need to research functionality more to provide description
 *  Example :  if (player:hasGearSetMod(gearset.id) == false) then
 *  Notes   : Used exclusively in scripts/globals/gear_sets.lua
 ************************************************************************/

bool CLuaBaseEntity::hasGearSetMod(uint8 modNameId)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    if (auto* PChar = dynamic_cast<CCharEntity*>(m_PBaseEntity))
    {
        for (auto exsistingMod : PChar->m_GearSetMods)
        {
            if (modNameId == exsistingMod.modNameId)
            {
                return true;
            }
        }
    }

    return false;
}

/************************************************************************
 *  Function: addGearSetMod()
 *  Purpose : Need to research functionality more to provide description
 *  Example :  player:addGearSetMod(gearset.id + i, modId, modValue + addSetBonus)
 *  Notes   : Used exclusively in scripts/globals/gear_sets.lua
 ************************************************************************/

void CLuaBaseEntity::addGearSetMod(uint8 modNameId, Mod modId, uint16 modValue)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    GearSetMod_t gearSetMod;
    gearSetMod.modNameId = modNameId;
    gearSetMod.modId     = modId;
    gearSetMod.modValue  = modValue;

    CCharEntity* PChar = dynamic_cast<CCharEntity*>(m_PBaseEntity);

    for (auto& exsistingMod : PChar->m_GearSetMods)
    {
        if (gearSetMod.modNameId == exsistingMod.modNameId)
        {
            return;
        }
    }

    PChar->m_GearSetMods.push_back(gearSetMod);
    PChar->addModifier(gearSetMod.modId, gearSetMod.modValue);
}

/************************************************************************
 *  Function: clearGearSetMods()
 *  Purpose : Clears all mods the player has from gear sets
 *  Example : player:clearGearSetMods()
 *  Notes   :
 ************************************************************************/

void CLuaBaseEntity::clearGearSetMods()
{
    if (auto* PChar = dynamic_cast<CCharEntity*>(m_PBaseEntity))
    {
        for (uint8 i = 0; i < PChar->m_GearSetMods.size(); ++i)
        {
            GearSetMod_t gearSetMod = PChar->m_GearSetMods.at(i);
            PChar->delModifier(gearSetMod.modId, gearSetMod.modValue);
        }
        PChar->m_GearSetMods.clear();
    }
}

/************************************************************************
 *  Function: getStorageItem()
 *  Purpose : Returns object data for an item in a container
 *  Example : player:getStorageItem(0, 0, SLOT_RANGED)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getStorageItem(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

    auto container = (uint8)lua_tointeger(L, 1);                            // LOC_INVENTORY etc
    auto slotID    = (uint8)lua_tointeger(L, 2);                            // slot in container
    auto equipID   = (uint8)(!lua_isnil(L, 3) ? lua_tointeger(L, 3) : 255); // SLOT_MAIN etc

    CItem* PItem = nullptr;

    if (equipID == 255)
    {
        PItem = PChar->getStorage(container)->GetItem(slotID);
    }
    else
    {
        PItem = PChar->getEquip((SLOTTYPE)equipID);
    }

    if (PItem != nullptr)
    {
        lua_getglobal(L, "CItem");
        lua_pushstring(L, "new");
        lua_gettable(L, -2);
        lua_insert(L, -2);
        lua_pushlightuserdata(L, PItem);
        lua_pcall(L, 2, 1, 0);
        return 1;
    }
    lua_pushnil(L);
    return 1;
}

/************************************************************************
 *  Function: storeWithPorterMoogle()
 *  Purpose : Stores an item with a Porter Moogle
 *  Example : local result = player:storeWithPorterMoogle(slipId, extra, storableItemIds)
 *  Notes   : Sets an 'extra' value so item doesn't appear in inventory
 ************************************************************************/

inline int32 CLuaBaseEntity::storeWithPorterMoogle(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);
    lua_settop(L, 3);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_istable(L, 2));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 3) || !lua_istable(L, 3));

    auto         slipId     = (uint16)lua_tointeger(L, 1);
    CCharEntity* PChar      = (CCharEntity*)m_PBaseEntity;
    auto         slipSlotId = PChar->getStorage(LOC_INVENTORY)->SearchItem(slipId);
    if (slipSlotId == 255)
    {
        return 0;
    }
    auto* slip = PChar->getStorage(LOC_INVENTORY)->GetItem(slipSlotId);

    auto extraSize = lua_objlen(L, 2);
    lua_pushnil(L);
    for (size_t i = 0; i < extraSize && lua_next(L, 2) != 0; i++)
    {
        auto extra = (uint8)lua_tonumber(L, -1);
        if ((slip->m_extra[i] & extra) != 0)
        {
            lua_pushinteger(L, 1);
            return 1;
        }
        slip->m_extra[i] |= extra;
        lua_pop(L, 1);
    }

    auto   storableSize = lua_objlen(L, 3);
    uint16 storedItemIds[7];

    lua_pushnil(L);
    for (size_t i = 0; i < storableSize && lua_next(L, 3) != 0; i++)
    {
        auto itemId = (uint16)lua_tonumber(L, -1);
        if (itemId != 0)
        {
            storedItemIds[i] = itemId;
        }
        else
        {
            storedItemIds[i] = 0;
        }
        lua_pop(L, 1);
    }

    for (auto itemId : storedItemIds)
    {
        if (itemId != 0)
        {
            auto slotId = PChar->getStorage(LOC_INVENTORY)->SearchItem(itemId);
            if (slotId != 255)
            {
                // TODO: Items need to be checked for an in-progress magian trial before storing.
                // auto item = PChar->getStorage(LOC_INVENTORY)->GetItem(slotId);
                // if (item->isType(ITEM_EQUIPMENT) && ((CItemEquipment*)item)->getTrialNumber() != 0)
                CItem* PItem = PChar->getStorage(LOC_INVENTORY)->GetItem(slotId);
                if (PItem)
                {
                    PItem->setReserve(0);
                    charutils::UpdateItem(PChar, LOC_INVENTORY, slotId, -1);
                }
                // else
                //{
                // lua_pushinteger(L, 2);
                // return 1;
                //}
            }
        }
    }

    char extra[sizeof(slip->m_extra) * 2 + 1];
    Sql_EscapeStringLen(SqlHandle, extra, (const char*)slip->m_extra, sizeof(slip->m_extra));

    const char* Query = "UPDATE char_inventory "
                        "SET extra = '%s' "
                        "WHERE charid = %u AND location = %u AND slot = %u;";

    Sql_Query(SqlHandle, Query, extra, PChar->id, slip->getLocationID(), slip->getSlotID());

    lua_pushinteger(L, 0);
    return 1;
}

/************************************************************************
 *  Function: getRetrievableItemsForSlip()
 *  Purpose : Returns listing of 'stored' items as Lua table
 *  Example : local extra = player:getRetrievableItemsForSlip(slipId)
 *  Notes   : See scripts/globals/porter_moogle_util.lua
 ************************************************************************/

inline int32 CLuaBaseEntity::getRetrievableItemsForSlip(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);
    lua_settop(L, 1);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    auto         slipId     = (uint16)lua_tointeger(L, 1);
    CCharEntity* PChar      = (CCharEntity*)m_PBaseEntity;
    auto         slipSlotId = PChar->getStorage(LOC_INVENTORY)->SearchItem(slipId);
    if (slipSlotId == 255)
    {
        return 0;
    }
    auto* slip = PChar->getStorage(LOC_INVENTORY)->GetItem(slipSlotId);

    lua_newtable(L);
    // TODO Is extra sized defined anywhere?
    for (int i = 0; i < 24; i++)
    {
        lua_pushnumber(L, slip->m_extra[i]);
        lua_rawseti(L, -2, i + 1);
    }

    return 1;
}

/************************************************************************
 *  Function: retrieveItemFromSlip()
 *  Purpose : Retrieves an item stored with Porter Moogle
 *  Example : player:retrieveItemFromSlip(slipId, retrievedItemId, extraId, bitmask)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::retrieveItemFromSlip(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);
    lua_settop(L, 4);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 3) || !lua_isnumber(L, 3));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 4) || !lua_isnumber(L, 4));

    auto slipId    = (uint16)lua_tointeger(L, 1);
    auto itemId    = (uint16)lua_tointeger(L, 2);
    auto extraId   = (uint16)lua_tointeger(L, 3);
    auto extraData = (uint8)lua_tointeger(L, 4);

    CCharEntity* PChar      = (CCharEntity*)m_PBaseEntity;
    auto         slipSlotId = PChar->getStorage(LOC_INVENTORY)->SearchItem(slipId);
    if (slipSlotId == 255)
    {
        return 0;
    }
    auto* slip = PChar->getStorage(LOC_INVENTORY)->GetItem(slipSlotId);

    slip->m_extra[extraId] &= extraData;

    char extra[sizeof(slip->m_extra) * 2 + 1];
    Sql_EscapeStringLen(SqlHandle, extra, (const char*)slip->m_extra, sizeof(slip->m_extra));

    const char* Query = "UPDATE char_inventory "
                        "SET extra = '%s' "
                        "WHERE charid = %u AND location = %u AND slot = %u;";

    Sql_Query(SqlHandle, Query, extra, PChar->id, slip->getLocationID(), slip->getSlotID());

    auto* item = itemutils::GetItem(itemId);
    item->setQuantity(1);
    charutils::AddItem(PChar, LOC_INVENTORY, item);

    return 0;
}

/************************************************************************
 *  Function: getRace()
 *  Purpose : Returns the integer value of the race of the character
 *  Example : player:getRace()
 ************************************************************************/

uint8 CLuaBaseEntity::getRace()
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    return static_cast<CCharEntity*>(m_PBaseEntity)->look.race;
}

/************************************************************************
 *  Function: getGender()
 *  Purpose : Returns the integer value of the gender of the character
 *  Example : player:getGender()
 ************************************************************************/

uint8 CLuaBaseEntity::getGender()
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);

    return PChar->GetGender();
}

/************************************************************************
 *  Function: getName()
 *  Purpose : Returns the string name of the character
 *  Example : player:getName()
 ************************************************************************/

const char* CLuaBaseEntity::getName()
{
    // TODO: Fix C-style cast
    return (const char*)m_PBaseEntity->GetName();
}

/************************************************************************
 *  Function: hideName()
 *  Purpose : Hides the name of the entity
 *  Example : mob:hideName()
 *  Notes   :
 ************************************************************************/

void CLuaBaseEntity::hideName(bool isHidden)
{
    m_PBaseEntity->HideName(isHidden);
}

/************************************************************************
 *  Function: checkNameFlags()
 *  Purpose : Returns true if a player has name flags
 ************************************************************************/

bool CLuaBaseEntity::checkNameFlags(uint32 flags)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);

    if (PChar->nameflags.flags & flags)
    {
        return true;
    }

    return false;
}

/************************************************************************
 *  Function: getModelId()
 *  Purpose : Returns the integer value of the entity's Model ID
 *  Example : mob:getModelId()
 *  Notes   :
 ************************************************************************/

uint16 CLuaBaseEntity::getModelId()
{
    return m_PBaseEntity->GetModelId();
}

/************************************************************************
 *  Function: setModelId()
 *  Purpose : Updates the Model ID of the entity
 *  Example : mob:setModelId(1168)
 *  Notes   :
 ************************************************************************/

void CLuaBaseEntity::setModelId(uint16 modelId, uint8 slot = 0)
{
    if (m_PBaseEntity->objtype == TYPE_PC)
    {
        switch (static_cast<SLOTTYPE>(slot))
        {
            case SLOT_MAIN:
                m_PBaseEntity->look.main = modelId;
                break;
            case SLOT_SUB:
                m_PBaseEntity->look.sub = modelId;
                break;
            case SLOT_RANGED:
                m_PBaseEntity->look.ranged = modelId;
                break;
            case SLOT_HEAD:
                m_PBaseEntity->look.head = modelId;
                break;
            case SLOT_BODY:
                m_PBaseEntity->look.body = modelId;
                break;
            case SLOT_HANDS:
                m_PBaseEntity->look.hands = modelId;
                break;
            case SLOT_LEGS:
                m_PBaseEntity->look.legs = modelId;
                break;
            case SLOT_FEET:
                m_PBaseEntity->look.feet = modelId;
                break;
            default:
                break;
        }

        auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);
        PChar->pushPacket(new CCharAppearancePacket(PChar));
    }
    else
    {
        m_PBaseEntity->SetModelId(modelId);
    }
    m_PBaseEntity->updatemask |= UPDATE_LOOK;
}

/************************************************************************
 *  Function: setCostume()
 *  Purpose : Updates the PC's appearance
 *  Example : player:setCostume( costumeId )
 ************************************************************************/

void CLuaBaseEntity::setCostume(uint16 costume)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);

    if (PChar->m_Costume != costume && PChar->status != STATUS_TYPE::SHUTDOWN && PChar->status != STATUS_TYPE::DISAPPEAR)
    {
        PChar->m_Costume = costume;
        PChar->updatemask |= UPDATE_LOOK;
        PChar->pushPacket(new CCharUpdatePacket(PChar));
    }
}

/************************************************************************
 *  Function: getCostume()
 *  Purpose : Returns the PC's appearance
 *  Example : player:getCostume()
 ************************************************************************/

uint16 CLuaBaseEntity::getCostume()
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);

    return PChar->m_Costume;
}

/************************************************************************
 *  Function: costume2()
 *  Purpose : Sets or returns a monstrosity costume
 *  Example : player:costume2( costumeId )
 *  Notes   : Not currently implemented
 ************************************************************************/

inline int32 CLuaBaseEntity::costume2(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

    if (!lua_isnil(L, 1) && lua_isnumber(L, 1))
    {
        uint16 model = (uint16)lua_tointeger(L, 1);

        if (PChar->m_Monstrosity != model && PChar->status != STATUS_TYPE::SHUTDOWN && PChar->status != STATUS_TYPE::DISAPPEAR)
        {
            PChar->m_Monstrosity = model;
            PChar->updatemask |= UPDATE_LOOK;
            PChar->pushPacket(new CCharAppearancePacket(PChar));
        }
        return 0;
    }
    lua_pushinteger(L, PChar->m_Monstrosity);
    return 1;
}

/************************************************************************
 *  Function: getAnimation()
 *  Purpose : Returns the assigned default animation of an entity
 *  Example : GetNPCByID(TrapDoor):getAnimation()
 *  Notes   : ANIMATION_ATTACK
 ************************************************************************/

inline int32 CLuaBaseEntity::getAnimation(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    lua_pushinteger(L, m_PBaseEntity->animation);
    return 1;
}

/************************************************************************
 *  Function: setAnimation()
 *  Purpose : Updates an animation for the entity
 *  Example : GetNPCByID(Door_Offset+12):setAnimation(8)
 *  Notes   : Look at scripts/zones/VeLugannon_Palace/npcs/Monolith.lua
 ************************************************************************/

inline int32 CLuaBaseEntity::setAnimation(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    uint8 animation = (uint8)lua_tointeger(L, 1);

    if (m_PBaseEntity->animation != animation)
    {
        m_PBaseEntity->animation = animation;
        m_PBaseEntity->updatemask |= UPDATE_HP;
    }
    return 0;
}

/************************************************************************
 *  Function: AnimationSub()
 *  Purpose : Returns animation sub for an entity or updates if var supplied
 *  Example : if (mob:AnimationSub() == 1) then mob:AnimationSub(2)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::AnimationSub(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    if (!lua_isnil(L, 1) && lua_isnumber(L, 1))
    {
        uint8 animationsub = (uint8)lua_tointeger(L, 1);

        if (m_PBaseEntity->animationsub != animationsub)
        {
            m_PBaseEntity->animationsub = animationsub;

            if (m_PBaseEntity->objtype == TYPE_PC)
            {
                ((CCharEntity*)m_PBaseEntity)->pushPacket(new CCharUpdatePacket((CCharEntity*)m_PBaseEntity));
            }
            else
            {
                m_PBaseEntity->loc.zone->PushPacket(m_PBaseEntity, CHAR_INRANGE, new CEntityUpdatePacket(m_PBaseEntity, ENTITY_UPDATE, UPDATE_COMBAT));
            }
        }
        return 0;
    }
    lua_pushinteger(L, m_PBaseEntity->animationsub);
    return 1;
}

/************************************************************************
 *  Function: getNation()
 *  Purpose : Returns the integer value of the player's nation
 *  Example : player:getNation()
 *  Notes   :
 ************************************************************************/

uint8 CLuaBaseEntity::getNation()
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    return static_cast<CCharEntity*>(m_PBaseEntity)->profile.nation;
}

/************************************************************************
 *  Function: setNation()
 *  Purpose : Changes a player's nation allegiance
 *  Example : player:setNation(2)
 *  Notes   :
 ************************************************************************/

void CLuaBaseEntity::setNation(uint8 nation)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);

    PChar->profile.nation = nation;
    charutils::SaveCharNation(PChar);
}

/************************************************************************
 *  Function: getAllegiance()
 *  Purpose : Gets allegiance of entity (mob/player/ballista team)
 *  Example : if (target:getAllegiance() == caster:getAllegiance()) then
 ************************************************************************/

uint8 CLuaBaseEntity::getAllegiance()
{
    return static_cast<uint8>(m_PBaseEntity->allegiance);
}

/************************************************************************
 *  Function: setAllegiance()
 *  Purpose : Sets allegiance of entity (mob/player/ballista team)
 *  Example : target:setAllegiance(???)
 ************************************************************************/

void CLuaBaseEntity::setAllegiance(uint8 allegiance)
{
    m_PBaseEntity->allegiance = static_cast<ALLEGIANCE_TYPE>(allegiance);
    m_PBaseEntity->updatemask |= UPDATE_HP;
}

/************************************************************************
 *  Function: getCampaignAllegiance()
 *  Purpose : Returns the integer value of a player's Campaign allegiance
 *  Example : if (player:getCampaignAllegiance() > 0) then
 *  Notes   : A return of 0 means the player doesn't have any allegiances
 ************************************************************************/

uint8 CLuaBaseEntity::getCampaignAllegiance()
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    return static_cast<CCharEntity*>(m_PBaseEntity)->profile.campaign_allegiance;
}

/************************************************************************
 *  Function: setCampaignAllegiance()
 *  Purpose : Affiliates the player with a particular nation in the past
 *  Example : targ:setCampaignAllegiance(nation)
 ************************************************************************/

void CLuaBaseEntity::setCampaignAllegiance(uint8 allegiance)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);

    PChar->profile.campaign_allegiance = allegiance;
    charutils::SaveCampaignAllegiance(PChar);
}

/************************************************************************
 *  Function: isSeekingParty()
 *  Purpose : Returns true if a player is seeking a party
 *  Example : if player:isSeekingParty() then
 *  Notes   :
 ************************************************************************/

bool CLuaBaseEntity::isSeekingParty()
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    return (static_cast<CCharEntity*>(m_PBaseEntity)->nameflags.flags & FLAG_INVITE);
}

/************************************************************************
 *  Function: getNewPlayer()
 *  Purpose : Returns true if a player is new
 *  Example : if not (player:getNewPlayer()) then
 *  Notes   :
 ************************************************************************/

bool CLuaBaseEntity::getNewPlayer()
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    return (static_cast<CCharEntity*>(m_PBaseEntity)->menuConfigFlags.flags & NFLAG_NEWPLAYER) == 0;
}

/************************************************************************
 *  Function: setNewPlayer()
 *  Purpose : Marks a player as being new and calls charutils to update DB
 *  Example : player:setNewPlayer(1)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::setNewPlayer(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isboolean(L, 1));

    auto* PChar = dynamic_cast<CCharEntity*>(m_PBaseEntity);

    if (lua_toboolean(L, 1))
    {
        PChar->menuConfigFlags.flags |= NFLAG_NEWPLAYER;
    }
    else
    {
        PChar->menuConfigFlags.flags &= ~NFLAG_NEWPLAYER;
    }

    PChar->updatemask |= UPDATE_HP;

    charutils::SaveMenuConfigFlags(PChar);

    return 0;
}

/************************************************************************
 *  Function: getMentor()
 *  Purpose : Returns true if a player is flagged as a mentor
 *  Example :
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getMentor(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;
    lua_pushnumber(L, PChar->m_mentorUnlocked ? 1 : 0);
    return 1;
}

/************************************************************************
 *  Function: setMentor()
 *  Purpose : Sets the mentor flag for a character
 *  Example : player:setMentor(1)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::setMentor(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    CCharEntity* PChar      = (CCharEntity*)m_PBaseEntity;
    PChar->m_mentorUnlocked = (uint8)lua_tonumber(L, 1) == 1;

    charutils::SaveMentorFlag(PChar);
    PChar->updatemask |= UPDATE_HP;
    return 0;
}

/************************************************************************
 *  Function: getGMLevel()
 *  Purpose : Returns the GM level (0-5)
 *  Example : if (player:getGMLevel() == 5) then -- kill pixies
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getGMLevel(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

    lua_pushnumber(L, PChar->m_GMlevel);
    return 1;
}

/************************************************************************
 *  Function: setGMLevel()
 *  Purpose : Updates a player's GM status (0-5)
 *  Example : player:setGMLevel(3)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::setGMLevel(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

    PChar->m_GMlevel = (uint8)lua_tonumber(L, 1);
    charutils::SaveCharGMLevel(PChar);
    return 0;
}

/************************************************************************
 *  Function: getGMHidden()
 *  Purpose : Returns true if a GM is currently hidden
 *  Example : if (player:getGMHidden()) then
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getGMHidden(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;
    lua_pushboolean(L, PChar->m_isGMHidden);
    return 1;
}

/************************************************************************
 *  Function: setGMHidden()
 *  Purpose : Sets a GM to hidden mode
 *  Example : player:setGMHidden(1)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::setGMHidden(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    CCharEntity* PChar  = (CCharEntity*)m_PBaseEntity;
    PChar->m_isGMHidden = lua_toboolean(L, 1);

    if (PChar->loc.zone)
    {
        if (PChar->m_isGMHidden)
        {
            PChar->loc.zone->PushPacket(PChar, CHAR_INRANGE, new CCharPacket(PChar, ENTITY_DESPAWN, 0));
        }
        else
        {
            PChar->loc.zone->PushPacket(PChar, CHAR_INRANGE, new CCharPacket(PChar, ENTITY_SPAWN, 0));
        }
    }

    return 0;
}

/************************************************************************
 *  Function: isJailed()
 *  Purpose : Returns true if a player is a violent felon
 *  Example : if (player:isJailed()) then
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::isJailed(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    lua_pushboolean(L, (jailutils::InPrison((CCharEntity*)m_PBaseEntity)));
    return 1;
}

/************************************************************************
 *  Function: jail()
 *  Purpose : Locks up a misbehaving Elvaan
 *  Example : player:jail()
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::jail(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    jailutils::Add(static_cast<CCharEntity*>(m_PBaseEntity));
    return 0;
}

/************************************************************************
 *  Function: canUseMisc()
 *  Purpose : Returns true if ZONEMISC contains flag being checked.
 *  Example : if (player:canUseMisc(tpz.zoneMisc.MISC_MOUNT)) then -- kew
 *  Notes   : Checks if specified MISC flag is set in current zone
 ************************************************************************/

inline int32 CLuaBaseEntity::canUseMisc(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->loc.zone == nullptr);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    lua_pushboolean(L, m_PBaseEntity->loc.zone->CanUseMisc((uint16)lua_tointeger(L, 1)));
    return 1;
}

/************************************************************************
 *  Function: speed()
 *  Purpose : Sets a player's speed or returns their current speed
 *  Example : player:speed(40) -- Sets; player:speed() -- returns value
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::speed(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    if (!lua_isnil(L, 1) && lua_isnumber(L, 1))
    {
        auto speed = std::min<uint8>((uint8)lua_tointeger(L, 1), 255);

        if (m_PBaseEntity->speed != speed)
        {
            m_PBaseEntity->speed = speed;

            if (m_PBaseEntity->objtype == TYPE_PC)
            {
                ((CCharEntity*)m_PBaseEntity)->pushPacket(new CCharUpdatePacket((CCharEntity*)m_PBaseEntity));
            }
            else
            {
                m_PBaseEntity->loc.zone->PushPacket(m_PBaseEntity, CHAR_INRANGE, new CEntityUpdatePacket(m_PBaseEntity, ENTITY_UPDATE, UPDATE_POS));
            }
        }
        return 0;
    }
    lua_pushinteger(L, m_PBaseEntity->speed);
    return 1;
}

/************************************************************************
 *  Function: getPlaytime()
 *  Purpose : Returns a player's total play time, or updates
 *  Example : player:getPlaytime()
 *  Notes   : See scripts/zones/Port_Bastok/Zone.lua for no playtime example
 ************************************************************************/

inline int32 CLuaBaseEntity::getPlaytime(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    bool update = true;
    if (!lua_isnil(L, 1) && lua_isboolean(L, 1))
    {
        update = lua_toboolean(L, 1);
    }

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

    lua_pushinteger(L, PChar->GetPlayTime(update));

    return 1;
}

/************************************************************************
 *  Function: getTimeCreated()
 *  Purpose : Get unix timestamp of when character was created
 *  Example : player:getTimeCreated()
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getTimeCreated(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

    lua_pushinteger(L, PChar->GetTimeCreated());
    return 1;
}

/************************************************************************
 *  Function: getMainJob()
 *  Purpose : Returns the integer value of the entity's main job
 *  Example : mob:getMainJob(); player:getMainJob()
 *  Notes   :
 ************************************************************************/

uint8 CLuaBaseEntity::getMainJob()
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    return static_cast<CBattleEntity*>(m_PBaseEntity)->GetMJob();
}

/************************************************************************
 *  Function: getSubJob()
 *  Purpose : Returns the integer value of the entity's sub job
 *  Example : mob:getSubJob(); player:getSubJob()
 *  Notes   :
 ************************************************************************/

uint8 CLuaBaseEntity::getSubJob()
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    return static_cast<CBattleEntity*>(m_PBaseEntity)->GetSJob();
}

/************************************************************************
 *  Function: changeJob()
 *  Purpose : Changes an entities main job
 *  Example : mob:changeJob(RDM); player:changeJob(2)
 *  Notes   :
 ************************************************************************/

void CLuaBaseEntity::changeJob(uint8 newJob)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto*   PChar   = dynamic_cast<CCharEntity*>(m_PBaseEntity);
    JOBTYPE prevjob = PChar->GetMJob();

    PChar->resetPetZoningInfo();

    PChar->jobs.unlocked |= (1 << newJob);
    PChar->SetMJob(newJob);

    if (newJob == JOB_BLU)
    {
        if (prevjob != JOB_BLU)
        {
            blueutils::LoadSetSpells(PChar);
        }
    }
    else if (PChar->GetSJob() != JOB_BLU)
    {
        blueutils::UnequipAllBlueSpells(PChar);
    }
    puppetutils::LoadAutomaton(PChar);
    charutils::SetStyleLock(PChar, false);
    luautils::CheckForGearSet(PChar); // check for gear set on gear change
    charutils::BuildingCharSkillsTable(PChar);
    charutils::CalculateStats(PChar);
    charutils::CheckValidEquipment(PChar);
    PChar->PRecastContainer->ChangeJob();
    charutils::BuildingCharAbilityTable(PChar);
    charutils::BuildingCharTraitsTable(PChar);

    PChar->ForParty([](CBattleEntity* PMember) { ((CCharEntity*)PMember)->PLatentEffectContainer->CheckLatentsPartyJobs(); });

    PChar->UpdateHealth();
    PChar->health.hp = PChar->GetMaxHP();
    PChar->health.mp = PChar->GetMaxMP();

    charutils::SaveCharStats(PChar);
    charutils::SaveCharJob(PChar, PChar->GetMJob());
    charutils::SaveCharExp(PChar, PChar->GetMJob());
    PChar->updatemask |= UPDATE_HP;

    PChar->pushPacket(new CCharJobsPacket(PChar));
    PChar->pushPacket(new CCharStatsPacket(PChar));
    PChar->pushPacket(new CCharSkillsPacket(PChar));
    PChar->pushPacket(new CCharRecastPacket(PChar));
    PChar->pushPacket(new CCharAbilitiesPacket(PChar));
    PChar->pushPacket(new CCharUpdatePacket(PChar));
    PChar->pushPacket(new CMenuMeritPacket(PChar));
    PChar->pushPacket(new CCharSyncPacket(PChar));
}

/************************************************************************
 *  Function: changesJob()
 *  Purpose : Changes an entities sub job
 *  Example : mob:changesJob(RDM); player:changesJob(2)
 *  Notes   : To Do: Change name to changeSubJob for visual clarity?
 ************************************************************************/

void CLuaBaseEntity::changesJob(uint8 subJob)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);

    PChar->jobs.unlocked |= (1 << subJob);
    PChar->SetSJob(subJob);
    charutils::UpdateSubJob(PChar);

    if (subJob == JOB_BLU)
    {
        blueutils::LoadSetSpells(PChar);
    }
    else
    {
        blueutils::UnequipAllBlueSpells(PChar);
    }

    puppetutils::LoadAutomaton(PChar);
}

/************************************************************************
 *  Function: unlockJob()
 *  Purpose : Unlocks a new job for a player (updates char_jobs.sql)
 *  Example : player:unlockJob(SAM)
 *  Notes   : Changes value of job from 0 (locked) to 1(unlocked)
 ************************************************************************/

void CLuaBaseEntity::unlockJob(uint8 JobID)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);

    if (JobID < MAX_JOBTYPE)
    {
        PChar->jobs.unlocked |= (1 << JobID);

        if (JobID == JOB_NON)
        {
            JobID = JOB_WAR;
        }
        if (PChar->jobs.job[JobID] == 0)
        {
            PChar->jobs.job[JobID] = 1;
        }

        charutils::SaveCharJob(PChar, static_cast<JOBTYPE>(JobID));
        PChar->pushPacket(new CCharJobsPacket(PChar));
    }
}

/************************************************************************
 *  Function: hasJob()
 *  Purpose : Check to see if JOBTYPE is unlocked
 *  Example : player:hasJob(BRD)
 ************************************************************************/

bool CLuaBaseEntity::hasJob(uint8 job)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    JOBTYPE JobID = static_cast<JOBTYPE>(job);

    TPZ_DEBUG_BREAK_IF(JobID > MAX_JOBTYPE || JobID < 0);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);

    return (PChar->jobs.unlocked >> JobID) & 1;
}

/************************************************************************
 *  Function: getMainLvl()
 *  Purpose : Returns the main level of entity's current job
 *  Example : player:getMainLvl()
 ************************************************************************/

uint8 CLuaBaseEntity::getMainLvl()
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    return static_cast<CBattleEntity*>(m_PBaseEntity)->GetMLevel();
}

/************************************************************************
 *  Function: getSubLvl()
 *  Purpose : Returns the level of entity's current sub job
 *  Example : player:getSubLvl()
 ************************************************************************/

uint8 CLuaBaseEntity::getSubLvl()
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    return static_cast<CBattleEntity*>(m_PBaseEntity)->GetSLevel();
}

/************************************************************************
 *  Function: getJobLevel()
 *  Purpose : Return the levle of job specified by JOBTYPE
 *  Example : player:getJobLevel(BRD)
 ************************************************************************/

uint8 CLuaBaseEntity::getJobLevel(uint8 JobID)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(JobID > MAX_JOBTYPE || JobID < 0);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);
    return PChar->jobs.job[JobID];
}

/************************************************************************
 *  Function: setLevel()
 *  Purpose : Updates the level of the entity's main job
 *  Example : player:setLevel(50)
 *  Notes   :
 ************************************************************************/

void CLuaBaseEntity::setLevel(uint8 level)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(level > 99);

    if (auto* PChar = dynamic_cast<CCharEntity*>(m_PBaseEntity))
    {
        PChar->SetMLevel(level);
        PChar->jobs.job[PChar->GetMJob()] = level;
        PChar->SetSLevel(PChar->jobs.job[PChar->GetSJob()]);
        PChar->jobs.exp[PChar->GetMJob()] = charutils::GetExpNEXTLevel(PChar->jobs.job[PChar->GetMJob()]) - 1;

        charutils::SetStyleLock(PChar, false);
        blueutils::ValidateBlueSpells(PChar);
        charutils::CalculateStats(PChar);
        charutils::CheckValidEquipment(PChar);
        charutils::BuildingCharSkillsTable(PChar);
        charutils::BuildingCharAbilityTable(PChar);
        charutils::BuildingCharTraitsTable(PChar);

        PChar->UpdateHealth();
        PChar->health.hp = PChar->GetMaxHP();
        PChar->health.mp = PChar->GetMaxMP();

        charutils::SaveCharStats(PChar);
        charutils::SaveCharJob(PChar, PChar->GetMJob());
        charutils::SaveCharExp(PChar, PChar->GetMJob());
        PChar->updatemask |= UPDATE_HP;

        PChar->pushPacket(new CCharJobsPacket(PChar));
        PChar->pushPacket(new CCharStatsPacket(PChar));
        PChar->pushPacket(new CCharSkillsPacket(PChar));
        PChar->pushPacket(new CCharRecastPacket(PChar));
        PChar->pushPacket(new CCharAbilitiesPacket(PChar));
        PChar->pushPacket(new CCharUpdatePacket(PChar));
        PChar->pushPacket(new CMenuMeritPacket(PChar));
        PChar->pushPacket(new CCharSyncPacket(PChar));
    }
}

/************************************************************************
 *  Function: setsLevel()
 *  Purpose : Updates the level of the entity's sub job
 *  Example : player:setsLvl(30)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::setsLevel(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_tointeger(L, 1) > 99);

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

    PChar->jobs.job[PChar->GetSJob()] = (uint8)lua_tointeger(L, 1);
    PChar->SetSLevel(PChar->jobs.job[PChar->GetSJob()]);
    PChar->jobs.exp[PChar->GetSJob()] = charutils::GetExpNEXTLevel(PChar->jobs.job[PChar->GetSJob()]) - 1;

    charutils::SetStyleLock(PChar, false);
    charutils::BuildingCharSkillsTable(PChar);
    charutils::CalculateStats(PChar);
    charutils::CheckValidEquipment(PChar);
    charutils::BuildingCharAbilityTable(PChar);
    charutils::BuildingCharTraitsTable(PChar);

    PChar->UpdateHealth();
    PChar->health.hp = PChar->GetMaxHP();
    PChar->health.mp = PChar->GetMaxMP();

    charutils::SaveCharStats(PChar);
    charutils::SaveCharJob(PChar, PChar->GetMJob());
    charutils::SaveCharJob(PChar, PChar->GetSJob());
    charutils::SaveCharExp(PChar, PChar->GetMJob());

    PChar->pushPacket(new CCharJobsPacket(PChar));
    PChar->pushPacket(new CCharStatsPacket(PChar));
    PChar->pushPacket(new CCharSkillsPacket(PChar));
    PChar->pushPacket(new CCharRecastPacket(PChar));
    PChar->pushPacket(new CCharAbilitiesPacket(PChar));
    PChar->pushPacket(new CCharUpdatePacket(PChar));
    PChar->pushPacket(new CMenuMeritPacket(PChar));
    PChar->pushPacket(new CCharSyncPacket(PChar));
    return 0;
}

/************************************************************************
 *  Function: levelCap()
 *  Purpose : Updates the player's level cap (genkai)
 *  Example : player:levelCap(55)
 *  Notes   : To Do: Rename to setGenkai() for clarity?
 ************************************************************************/

inline int32 CLuaBaseEntity::levelCap(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

    if (!lua_isnil(L, 1) && lua_isnumber(L, 1))
    {
        uint8 genkai = (uint8)lua_tointeger(L, 1);

        if (PChar->jobs.genkai != genkai)
        {
            PChar->jobs.genkai = (uint8)lua_tointeger(L, 1);

            Sql_Query(SqlHandle, "UPDATE char_jobs SET genkai = %u WHERE charid = %u LIMIT 1", PChar->jobs.genkai, PChar->id);
        }
        return 0;
    }
    lua_pushinteger(L, PChar->jobs.genkai);
    return 1;
}

/************************************************************************
 *  Function: levelRestriction()
 *  Purpose : Places a level restriction on the PC and recalculates stats
 *  Example : player:levelRestriction(50)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::levelRestriction(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

    if (!lua_isnil(L, 1) && lua_isnumber(L, 1))
    {
        PChar->m_LevelRestriction = (uint32)lua_tointeger(L, 1);

        uint8 NewMLevel = 0;

        if (PChar->m_LevelRestriction != 0 && PChar->m_LevelRestriction < PChar->jobs.job[PChar->GetMJob()])
        {
            NewMLevel = PChar->m_LevelRestriction;
        }
        else
        {
            NewMLevel = PChar->jobs.job[PChar->GetMJob()];
        }

        if (PChar->GetMLevel() != NewMLevel)
        {
            charutils::RemoveAllEquipMods(PChar);
            PChar->SetMLevel(NewMLevel);
            PChar->SetSLevel(PChar->jobs.job[PChar->GetSJob()]);
            charutils::ApplyAllEquipMods(PChar);

            if (PChar->status != STATUS_TYPE::DISAPPEAR)
            {
                blueutils::ValidateBlueSpells(PChar);
                charutils::BuildingCharSkillsTable(PChar);
                charutils::CalculateStats(PChar);
                charutils::BuildingCharTraitsTable(PChar);
                charutils::BuildingCharAbilityTable(PChar);
                charutils::CheckValidEquipment(PChar);
                PChar->pushPacket(new CCharJobsPacket(PChar));
                PChar->pushPacket(new CCharStatsPacket(PChar));
                PChar->pushPacket(new CCharSkillsPacket(PChar));
                PChar->pushPacket(new CCharRecastPacket(PChar));
                PChar->pushPacket(new CCharAbilitiesPacket(PChar));
                PChar->pushPacket(new CCharSpellsPacket(PChar));
                PChar->pushPacket(new CCharUpdatePacket(PChar));
                PChar->pushPacket(new CCharSyncPacket(PChar));
                PChar->updatemask |= UPDATE_HP;
            }

            if (PChar->PPet)
            {
                CPetEntity* PPet = (CPetEntity*)PChar->PPet;
                if (PPet->getPetType() == PET_TYPE::WYVERN)
                {
                    petutils::LoadWyvernStatistics(PChar, PPet, true);
                }
                else
                {
                    petutils::DespawnPet(PChar);
                }
            }
        }
    }
    lua_pushinteger(L, PChar->m_LevelRestriction);
    return 1;
}

/************************************************************************
 *  Function: addJobTraits
 *  Purpose : Add job traits
 *  Example : player:addJobTraits(tpz.job.WHM, 75)
 ************************************************************************/

int32 CLuaBaseEntity::addJobTraits(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));

    CBattleEntity* PEntity = dynamic_cast<CBattleEntity*>(m_PBaseEntity);

    if (PEntity != nullptr)
    {
        battleutils::AddTraits(PEntity, traits::GetTraits((uint8)lua_tointeger(L, 1)), (uint8)lua_tointeger(L, 2));
    }

    return 0;
}

/************************************************************************
 *  Function: getTitle()
 *  Purpose : Returns the integer value of the player's current title
 *  Example : if (player:getTitle()) == 123) then
 ************************************************************************/

uint16 CLuaBaseEntity::getTitle()
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    return static_cast<CCharEntity*>(m_PBaseEntity)->profile.title;
}

/************************************************************************
 *  Function: hasTitle()
 *  Purpose : Returns true if a player's current title matches a value
 *  Example : if (player:hasTitle(AWESOME_SAUCE)) then
 ************************************************************************/

bool CLuaBaseEntity::hasTitle(uint16 titleID)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    return charutils::hasTitle(static_cast<CCharEntity*>(m_PBaseEntity), titleID) != 0;
}

/************************************************************************
 *  Function: addTitle()
 *  Purpose : Adds a title to the character's profile only (doesn't change current)
 *  Example : player:addTitle(BLACK_DRAGON_SLAYER)
 *  Notes   : Use setTitle to both change and add
 ************************************************************************/

void CLuaBaseEntity::addTitle(uint16 titleID)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);

    PChar->profile.title = titleID;
    PChar->pushPacket(new CCharStatsPacket(PChar));

    charutils::addTitle(PChar, titleID);
    charutils::SaveTitles(PChar);
}

/************************************************************************
 *  Function: setTitle()
 *  Purpose : Updates the player's current title and adds to their profile
 *  Example : player:setTitle(SOB_SUPERHERO)
 ************************************************************************/

void CLuaBaseEntity::setTitle(uint16 titleID)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);
    charutils::setTitle(PChar, titleID);
}

/************************************************************************
 *  Function: delTitle()
 *  Purpose : Deletes a title from a character's profile
 *  Example : player:delTitle(IXION_HORNBREAKER)
 ************************************************************************/

void CLuaBaseEntity::delTitle(uint16 titleID)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);

    if (charutils::delTitle(PChar, titleID))
    {
        if (PChar->profile.title == titleID)
        {
            PChar->profile.title = 0;
        }

        PChar->pushPacket(new CCharStatsPacket(PChar));
        charutils::SaveTitles(PChar);
    }
}

/************************************************************************
 *  Function: getFame()
 *  Purpose : Returns the current fame level of the player
 *  Example : player:getFame(area)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getFame(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1));
    if (lua_istable(L, 1))
    {
        lua_getfield(L, 1, "fame_area");
    }
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, -1) || !lua_isnumber(L, -1));

    uint8 fameArea = (uint8)lua_tointeger(L, -1);

    if (fameArea <= 15)
    {
        uint16       fame           = 0;
        float        fameMultiplier = map_config.fame_multiplier;
        CCharEntity* PChar          = (CCharEntity*)m_PBaseEntity;

        switch (fameArea)
        {
            case 0: // San d'Oria
            case 1: // Bastok
            case 2: // Windurst
                fame = (uint16)(PChar->profile.fame[fameArea] * fameMultiplier);
                break;
            case 3: // Jeuno
                fame = (uint16)(PChar->profile.fame[4] + ((PChar->profile.fame[0] + PChar->profile.fame[1] + PChar->profile.fame[2]) * fameMultiplier / 3));
                break;
            case 4: // Selbina / Rabao
                fame = (uint16)((PChar->profile.fame[0] + PChar->profile.fame[1]) * fameMultiplier / 2);
                break;
            case 5: // Norg
                fame = (uint16)(PChar->profile.fame[3] * fameMultiplier);
                break;
            // Abyssea
            case 6:  // Konschtat
            case 7:  // Tahrongi
            case 8:  // La Theine
            case 9:  // Misareaux
            case 10: // Vunkerl
            case 11: // Attohwa
            case 12: // Altepa
            case 13: // Grauberg
            case 14: // Uleguerand
                fame = (uint16)(PChar->profile.fame[fameArea - 1] * fameMultiplier);
                break;
            case 15: // Adoulin
                fame = (uint16)(PChar->profile.fame[14] * fameMultiplier);
                break;
        }
        lua_pushinteger(L, fame);
    }
    else
    {
        ShowError(CL_RED "Lua::getFame: fameArea %i is invalid\n" CL_RESET, fameArea);
        lua_pushinteger(L, 0);
    }
    return 1;
}

/************************************************************************
 *  Function: addFame()
 *  Purpose : Adds a specified amount of fame to the player's balance
 *  Example : player:addFame(WINDURST, 30)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::addFame(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || (!lua_isnumber(L, 1) && !lua_istable(L, 1)));
    if (lua_istable(L, 1))
    {
        lua_getfield(L, 1, "fame_area");
        TPZ_DEBUG_BREAK_IF(lua_isnil(L, -1) || !lua_isnumber(L, -1));
    }
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));

    uint8  fameArea = (uint8)lua_tointeger(L, lua_isnumber(L, 1) ? 1 : -1);
    uint16 fame     = (uint16)lua_tointeger(L, 2);

    if (fameArea <= 15)
    {
        CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

        switch (fameArea)
        {
            case 0: // San d'Oria
            case 1: // Bastok
            case 2: // Windurst
                PChar->profile.fame[fameArea] += fame;
                break;
            case 3: // Jeuno
                PChar->profile.fame[4] += fame;
                break;
            case 4: // Selbina / Rabao
                PChar->profile.fame[0] += fame;
                PChar->profile.fame[1] += fame;
                break;
            case 5: // Norg
                PChar->profile.fame[3] += fame;
                break;
            // Abyssea
            case 6:  // Konschtat
            case 7:  // Tahrongi
            case 8:  // La Theine
            case 9:  // Misareaux
            case 10: // Vunkerl
            case 11: // Attohwa
            case 12: // Altepa
            case 13: // Grauberg
            case 14: // Uleguerand
                PChar->profile.fame[fameArea - 1] += fame;
                break;
            case 15: // Adoulin
                PChar->profile.fame[14] += fame;
                break;
        }
        charutils::SaveFame(PChar);
    }
    else
    {
        ShowError(CL_RED "Lua::addFame: fameArea %i is invalid\n" CL_RESET, fameArea);
    }
    return 0;
}

/************************************************************************
 *  Function: setFame()
 *  Purpose : Sets the fame level for a player to a specified amount
 *  Example : player:setFame(BASTOK,1500)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::setFame(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || (!lua_isnumber(L, 1) && !lua_istable(L, 1)));
    if (lua_istable(L, 1))
    {
        lua_getfield(L, 1, "fame_area");
        TPZ_DEBUG_BREAK_IF(lua_isnil(L, -1) || !lua_isnumber(L, -1));
    }
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));

    uint8  fameArea = (uint8)lua_tointeger(L, lua_isnumber(L, 1) ? 1 : -1);
    uint16 fame     = (uint16)lua_tointeger(L, 2);

    if (fameArea <= 15)
    {
        switch (fameArea)
        {
            case 0: // San d'Oria
            case 1: // Bastok
            case 2: // Windurst
                ((CCharEntity*)m_PBaseEntity)->profile.fame[fameArea] = fame;
                break;
            case 3: // Jeuno
                ((CCharEntity*)m_PBaseEntity)->profile.fame[4] = fame;
                break;
            case 4: // Selbina / Rabao
                ((CCharEntity*)m_PBaseEntity)->profile.fame[0] = fame;
                ((CCharEntity*)m_PBaseEntity)->profile.fame[1] = fame;
                break;
            case 5: // Norg
                ((CCharEntity*)m_PBaseEntity)->profile.fame[3] = fame;
                break;
            // Abyssea
            case 6:  // Konschtat
            case 7:  // Tahrongi
            case 8:  // La Theine
            case 9:  // Misareaux
            case 10: // Vunkerl
            case 11: // Attohwa
            case 12: // Altepa
            case 13: // Grauberg
            case 14: // Uleguerand
                ((CCharEntity*)m_PBaseEntity)->profile.fame[fameArea - 1] = fame;
                break;
            case 15: // Adoulin
                ((CCharEntity*)m_PBaseEntity)->profile.fame[14] = fame;
                break;
        }
        charutils::SaveFame((CCharEntity*)m_PBaseEntity);
    }
    else
    {
        ShowError(CL_RED "Lua::setFame: fameArea %i is invalid\n" CL_RESET, fameArea);
    }
    return 0;
}

/************************************************************************
 *  Function: getFameLevel()
 *  Purpose : Returns the player's baseline fame level for an area
 *  Example : player:getFameLevel(TENSHODO)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getFameLevel(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1));
    if (lua_istable(L, 1))
    {
        lua_getfield(L, 1, "fame_area");
    }
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, -1) || !lua_isnumber(L, -1));

    uint8 fameArea = (uint8)lua_tointeger(L, -1);

    if (fameArea <= 15)
    {
        this->getFame(L);
        uint16 fame      = (uint16)lua_tointeger(L, -1);
        uint8  fameLevel = 1;

        if (fame >= 613)
        {
            fameLevel = 9;
        }
        else if (fame >= 550)
        {
            fameLevel = 8;
        }
        else if (fame >= 488)
        {
            fameLevel = 7;
        }
        else if (fame >= 425)
        {
            fameLevel = 6;
        }
        else if (fame >= 325)
        {
            fameLevel = 5;
        }
        else if (fame >= 225)
        {
            fameLevel = 4;
        }
        else if (fame >= 125)
        {
            fameLevel = 3;
        }
        else if (fame >= 50)
        {
            fameLevel = 2;
        }

        if ((fameArea >= 6) && (fameArea <= 14) && (fameLevel >= 6))
        {
            fameLevel = 6; // Abyssea areas cap out at level 6 fame.
        }

        lua_pushinteger(L, fameLevel);
    }
    else
    {
        ShowError(CL_RED "Lua::getFameLevel: fameArea %i is invalid\n" CL_RESET, fameArea);
        lua_pushinteger(L, 1);
    }
    return 1;
}

/************************************************************************
 *  Function: getRank()
 *  Purpose : Returns the rank of a player's current nation
 *  Example : player:getRank()
 ************************************************************************/

uint8 CLuaBaseEntity::getRank()
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);

    return PChar->profile.rank[PChar->profile.nation];
}

/************************************************************************
 *  Function: getOtherRank()
 *  Purpose : Returns the rank of <nation> for the player
 *  Example : player:getOtherRank(WINDURST)
 ************************************************************************/

uint8 CLuaBaseEntity::getOtherRank(uint8 nation)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);

    return PChar->profile.rank[nation];
}

/************************************************************************
 *  Function: setRank()
 *  Purpose : Sets the player's nation rank to a specified value
 *  Example : player:setRank(10)
 ************************************************************************/

void CLuaBaseEntity::setRank(uint8 rank)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);

    PChar->profile.rank[PChar->profile.nation] = rank;

    charutils::SaveMissionsList(PChar);
}

/************************************************************************
 *  Function: getRankPoints()
 *  Purpose : Returns the current rank points (rank bar) of a player
 *  Example : player:getRankPoints()
 ************************************************************************/

uint32 CLuaBaseEntity::getRankPoints()
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    return static_cast<CCharEntity*>(m_PBaseEntity)->profile.rankpoints;
}

/************************************************************************
 *  Function: addRankPoints()
 *  Purpose : Adds a set amount of rank points to the player's balance
 *  Example : player:addRankPoints(10)
 *  Notes   : Like, when you trade crystals
 ************************************************************************/

void CLuaBaseEntity::addRankPoints(uint32 rankpoints)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);

    PChar->profile.rankpoints += rankpoints;

    charutils::SaveMissionsList(PChar);
}

/************************************************************************
 *  Function: setRankPoints()
 *  Purpose : Sets the current rank points of a player to a specified value
 *  Example : player:setRankPoints(100)
 *  Notes   : player:setRankPoints(0) is called after switching nations
 ************************************************************************/

void CLuaBaseEntity::setRankPoints(uint32 rankpoints)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);

    PChar->profile.rankpoints = rankpoints;
    charutils::SaveMissionsList(PChar);
}

/************************************************************************
 *  Function: addQuest()
 *  Purpose : Adds a new quest to the character's in-progress quest log
 *  Example : player:addQuest(BASTOK,LURE_OF_THE_WILDCAT)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::addQuest(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || (!lua_isnumber(L, 1) && !lua_istable(L, 1)));
    if (lua_istable(L, 1))
    {
        lua_getfield(L, 1, "quest_log");
        TPZ_DEBUG_BREAK_IF(lua_isnil(L, -1) || !lua_isnumber(L, -1));
    }
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));

    CCharEntity* PChar      = (CCharEntity*)m_PBaseEntity;
    uint8        questLogID = (uint8)lua_tointeger(L, lua_isnumber(L, 1) ? 1 : -1);
    uint16       questID    = (uint16)lua_tointeger(L, 2);

    if (questLogID < MAX_QUESTAREA && questID < MAX_QUESTID)
    {
        uint8 current  = PChar->m_questLog[questLogID].current[questID / 8] & (1 << (questID % 8));
        uint8 complete = PChar->m_questLog[questLogID].complete[questID / 8] & (1 << (questID % 8));

        if ((current == 0) && (complete == 0))
        {
            PChar->m_questLog[questLogID].current[questID / 8] |= (1 << (questID % 8));
            PChar->pushPacket(new CQuestMissionLogPacket(PChar, questLogID, LOG_QUEST_CURRENT));

            charutils::SaveQuestsList(PChar);
        }
    }
    else
    {
        ShowError(CL_RED "Lua::addQuest: questLogID %i or QuestID %i is invalid\n" CL_RESET, questLogID, questID);
    }
    return 0;
}

/************************************************************************
 *  Function: delQuest()
 *  Purpose : Deletes a quest from a character's quest log
 *  Example : player:delQuest(WINDURST,LURE_OF_THE_WILDCAT)
 *  Notes   : Doesn't delete any player variables associated with quest
 ************************************************************************/

inline int32 CLuaBaseEntity::delQuest(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || (!lua_isnumber(L, 1) && !lua_istable(L, 1)));
    if (lua_istable(L, 1))
    {
        lua_getfield(L, 1, "quest_log");
        TPZ_DEBUG_BREAK_IF(lua_isnil(L, -1) || !lua_isnumber(L, -1));
    }
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));

    CCharEntity* PChar      = (CCharEntity*)m_PBaseEntity;
    uint8        questLogID = (uint8)lua_tointeger(L, lua_isnumber(L, 1) ? 1 : -1);
    uint16       questID    = (uint16)lua_tointeger(L, 2);

    if (questLogID < MAX_QUESTAREA && questID < MAX_QUESTID)
    {
        uint8 current  = PChar->m_questLog[questLogID].current[questID / 8] & (1 << (questID % 8));
        uint8 complete = PChar->m_questLog[questLogID].complete[questID / 8] & (1 << (questID % 8));

        if ((current != 0) || (complete != 0))
        {
            PChar->m_questLog[questLogID].current[questID / 8] &= ~(1 << (questID % 8));
            PChar->m_questLog[questLogID].complete[questID / 8] &= ~(1 << (questID % 8));

            PChar->pushPacket(new CQuestMissionLogPacket(PChar, questLogID, LOG_QUEST_CURRENT));
            PChar->pushPacket(new CQuestMissionLogPacket(PChar, questLogID, LOG_QUEST_COMPLETE));

            charutils::SaveQuestsList(PChar);
        }
    }
    else
    {
        ShowError(CL_RED "Lua::delQuest: questLogID %i or QuestID %i is invalid\n" CL_RESET, questLogID, questID);
    }
    return 0;
}

/************************************************************************
 *  Function: getQuestStatus()
 *  Purpose : Gets the current quest status of the player
 *  Example : player:getQuestStatus(WINDURST,MAKING_THE_GRADE)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getQuestStatus(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || (!lua_isnumber(L, 1) && !lua_istable(L, 1)));
    if (lua_istable(L, 1))
    {
        lua_getfield(L, 1, "quest_log");
        TPZ_DEBUG_BREAK_IF(lua_isnil(L, -1) || !lua_isnumber(L, -1));
    }
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));

    uint8  questLogID = (uint8)lua_tointeger(L, lua_isnumber(L, 1) ? 1 : -1);
    uint16 questID    = (uint16)lua_tointeger(L, 2);

    if (questLogID < MAX_QUESTAREA && questID < MAX_QUESTID)
    {
        uint8 current  = ((CCharEntity*)m_PBaseEntity)->m_questLog[questLogID].current[questID / 8] & (1 << (questID % 8));
        uint8 complete = ((CCharEntity*)m_PBaseEntity)->m_questLog[questLogID].complete[questID / 8] & (1 << (questID % 8));

        lua_pushinteger(L, (complete != 0 ? 2 : (current != 0 ? 1 : 0)));
        return 1;
    }
    else
    {
        ShowError(CL_RED "Lua::getQuestStatus: questLogID %i or QuestID %i is invalid\n" CL_RESET, questLogID, questID);
    }
    lua_pushnil(L);
    return 1;
}

/************************************************************************
 *  Function: hasCompletedQuest()
 *  Purpose : Returns true if a player has completed a quest
 *  Example : if (player:hasCompletedQuest(JEUNO,BEYOND_INFINITY)) then
 ************************************************************************/

inline int32 CLuaBaseEntity::hasCompletedQuest(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || (!lua_isnumber(L, 1) && !lua_istable(L, 1)));
    if (lua_istable(L, 1))
    {
        lua_getfield(L, 1, "quest_log");
        TPZ_DEBUG_BREAK_IF(lua_isnil(L, -1) || !lua_isnumber(L, -1));
    }
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));

    uint8  questLogID = (uint8)lua_tointeger(L, lua_isnumber(L, 1) ? 1 : -1);
    uint16 questID    = (uint16)lua_tointeger(L, 2);

    if (questLogID < MAX_QUESTAREA && questID < MAX_QUESTID)
    {
        uint8 complete = ((CCharEntity*)m_PBaseEntity)->m_questLog[questLogID].complete[questID / 8] & (1 << (questID % 8));

        lua_pushboolean(L, (complete != 0));
        return 1;
    }
    ShowError(CL_RED "Lua::hasCompletedQuest: questLogID %i or QuestID %i is invalid\n" CL_RESET, questLogID, questID);
    lua_pushboolean(L, false);
    return 1;
}

/************************************************************************
 *  Function: completeQuest()
 *  Purpose : Completes a current quest for the player
 *  Example : player:completeQuest(OTHER_AREAS,ONLY_THE_BEST)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::completeQuest(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || (!lua_isnumber(L, 1) && !lua_istable(L, 1)));
    if (lua_istable(L, 1))
    {
        lua_getfield(L, 1, "quest_log");
        TPZ_DEBUG_BREAK_IF(lua_isnil(L, -1) || !lua_isnumber(L, -1));
    }
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));

    CCharEntity* PChar      = (CCharEntity*)m_PBaseEntity;
    uint8        questLogID = (uint8)lua_tointeger(L, lua_isnumber(L, 1) ? 1 : -1);
    uint16       questID    = (uint16)lua_tointeger(L, 2);

    if (questLogID < MAX_QUESTAREA && questID < MAX_QUESTID)
    {
        uint8 complete = PChar->m_questLog[questLogID].complete[questID / 8] & (1 << (questID % 8));

        if (!complete)
        {
            PChar->m_questLog[questLogID].current[questID / 8] &= ~(1 << (questID % 8));
            PChar->m_questLog[questLogID].complete[questID / 8] |= (1 << (questID % 8));

            PChar->pushPacket(new CQuestMissionLogPacket(PChar, questLogID, LOG_QUEST_CURRENT));
            PChar->pushPacket(new CQuestMissionLogPacket(PChar, questLogID, LOG_QUEST_COMPLETE));
            charutils::SaveQuestsList(PChar);
        }
    }
    else
    {
        ShowError(CL_RED "Lua::completeQuest: questLogID %i or QuestID %i is invalid\n" CL_RESET, questLogID, questID);
    }
    return 0;
}

/************************************************************************
 *  Function: addMission()
 *  Purpose : Adds a mission to the player's mission log
 *  Example : player:addMission(SANDORIA,JOURNEY_TO_BASTOK)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::addMission(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || (!lua_isnumber(L, 1) && !lua_istable(L, 1)));
    if (lua_istable(L, 1))
    {
        lua_getfield(L, 1, "mission_log");
        TPZ_DEBUG_BREAK_IF(lua_isnil(L, -1) || !lua_isnumber(L, -1));
    }
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));

    uint8  missionLogID = (uint8)lua_tointeger(L, lua_isnumber(L, 1) ? 1 : -1);
    uint16 MissionID    = (uint16)lua_tointeger(L, 2);

    if (missionLogID < MAX_MISSIONAREA && MissionID < MAX_MISSIONID)
    {
        CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

        if (PChar->m_missionLog[missionLogID].current != (missionLogID > 2 ? 0 : -1))
        {
            ShowWarning(CL_YELLOW "Lua::addMission: player has a current mission\n" CL_RESET, missionLogID);
        }
        PChar->m_missionLog[missionLogID].current = MissionID;
        PChar->pushPacket(new CQuestMissionLogPacket(PChar, missionLogID, LOG_MISSION_CURRENT));

        charutils::SaveMissionsList(PChar);
    }
    else
    {
        ShowError(CL_RED "Lua::delMission: missionLogID %i or Mission %i is invalid\n" CL_RESET, missionLogID, MissionID);
    }
    return 0;
}

/************************************************************************
 *  Function: delMission()
 *  Purpose : Delets a mission from a player's mission log
 *  Example : player:delMission(TOAU,KNIGHT_OF_GOLD)
 *  Notes   : Doesn't delete any player variables associated with mission
 ************************************************************************/

inline int32 CLuaBaseEntity::delMission(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || (!lua_isnumber(L, 1) && !lua_istable(L, 1)));
    if (lua_istable(L, 1))
    {
        lua_getfield(L, 1, "mission_log");
        TPZ_DEBUG_BREAK_IF(lua_isnil(L, -1) || !lua_isnumber(L, -1));
    }
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));

    uint8  missionLogID = (uint8)lua_tointeger(L, lua_isnumber(L, 1) ? 1 : -1);
    uint16 MissionID    = (uint16)lua_tointeger(L, 2);

    if (missionLogID < MAX_MISSIONAREA && MissionID < MAX_MISSIONID)
    {
        CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

        auto current  = (uint16)PChar->m_missionLog[missionLogID].current;
        bool complete = (missionLogID == MISSION_COP || MissionID >= 64) ? false : PChar->m_missionLog[missionLogID].complete[MissionID];

        if (current == MissionID)
        {
            PChar->m_missionLog[missionLogID].current = missionLogID > 2 ? 0 : -1;
            PChar->pushPacket(new CQuestMissionLogPacket(PChar, missionLogID, LOG_MISSION_CURRENT));
        }
        if (complete)
        {
            PChar->m_missionLog[missionLogID].complete[MissionID] = false;
            PChar->pushPacket(new CQuestMissionLogPacket(PChar, missionLogID, LOG_MISSION_COMPLETE));
        }
        charutils::SaveMissionsList(PChar);
    }
    else
    {
        ShowError(CL_RED "Lua::delMission: missionLogID %i or Mission %i is invalid\n" CL_RESET, missionLogID, MissionID);
    }
    return 0;
}

/************************************************************************
 *  Function: getCurrentMission()
 *  Purpose : Returns the integer associated with the player's current mission
 *  Example : player:getCurrentMission(TOAU)
 *  Notes   : Specify the area to pass a Lua table object
 ************************************************************************/

inline int32 CLuaBaseEntity::getCurrentMission(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1));
    if (lua_istable(L, 1))
    {
        lua_getfield(L, 1, "mission_log");
    }
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, -1) || !lua_isnumber(L, -1));

    uint8  missionLogID = (uint8)lua_tointeger(L, -1);
    uint16 MissionID    = 0;

    if (missionLogID < MAX_MISSIONAREA)
    {
        MissionID = (uint16)((CCharEntity*)m_PBaseEntity)->m_missionLog[missionLogID].current;
    }
    else
    {
        ShowError(CL_RED "Lua::getCurrentMission: missionLogID %i is invalid\n" CL_RESET, missionLogID);
    }
    lua_pushinteger(L, MissionID);
    return 1;
}

/************************************************************************
 *  Function: hasCompletedMission()
 *  Purpose : Returns true if a player has completed a specified mission
 *  Example : if (player:hasCompletedMission(TOAU,PRESIDENT_SALAHEEM)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::hasCompletedMission(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || (!lua_isnumber(L, 1) && !lua_istable(L, 1)));
    if (lua_istable(L, 1))
    {
        lua_getfield(L, 1, "mission_log");
        TPZ_DEBUG_BREAK_IF(lua_isnil(L, -1) || !lua_isnumber(L, -1));
    }
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));

    uint8  missionLogID = (uint8)lua_tointeger(L, lua_isnumber(L, 1) ? 1 : -1);
    uint16 MissionID    = (uint16)lua_tointeger(L, 2);

    bool complete = false;

    if (missionLogID < MAX_MISSIONAREA && MissionID < MAX_MISSIONID)
    {
        complete = (missionLogID == MISSION_COP || MissionID >= 64) ? MissionID < ((CCharEntity*)m_PBaseEntity)->m_missionLog[missionLogID].current
                                                                    : ((CCharEntity*)m_PBaseEntity)->m_missionLog[missionLogID].complete[MissionID];
    }
    else
    {
        ShowError(CL_RED "Lua::hasCompletedMission: missionLogID %i or Mission %i is invalid\n" CL_RESET, missionLogID, MissionID);
    }
    lua_pushboolean(L, complete);
    return 1;
}

/************************************************************************
 *  Function: completeMission()
 *  Purpose : Completes a specified mission for the player
 *  Example : player:completeMission(COP,THREE_PATHS)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::completeMission(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || (!lua_isnumber(L, 1) && !lua_istable(L, 1)));
    if (lua_istable(L, 1))
    {
        lua_getfield(L, 1, "mission_log");
        TPZ_DEBUG_BREAK_IF(lua_isnil(L, -1) || !lua_isnumber(L, -1));
    }
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));

    uint8  missionLogID = (uint8)lua_tointeger(L, lua_isnumber(L, 1) ? 1 : -1);
    uint16 MissionID    = (uint16)lua_tointeger(L, 2);

    if (missionLogID < MAX_MISSIONAREA && MissionID < MAX_MISSIONID)
    {
        CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

        if (PChar->m_missionLog[missionLogID].current != MissionID)
        {
            ShowWarning(CL_YELLOW "Lua::completeMission: can't complete non current mission\n" CL_RESET, missionLogID);
        }
        else
        {
            PChar->m_missionLog[missionLogID].current = missionLogID > 2 ? 0 : -1;
            if ((missionLogID != MISSION_COP) && (MissionID < 64))
            {
                PChar->m_missionLog[missionLogID].complete[MissionID] = true;
                PChar->pushPacket(new CQuestMissionLogPacket(PChar, missionLogID, LOG_MISSION_COMPLETE));
            }
            PChar->pushPacket(new CQuestMissionLogPacket(PChar, missionLogID, LOG_MISSION_CURRENT));

            charutils::SaveMissionsList(PChar);
        }
    }
    else
    {
        ShowError(CL_RED "Lua::completeMission: missionLogID %i or Mission %i is invalid\n" CL_RESET, missionLogID, MissionID);
    }
    return 0;
}

/************************************************************************
 *  Function: setMissionLogEx()
 *  Purpose : Sets mission log extra data to correctly track progress in branching missions.
 *  Example : player:setMissionLogEx(tpz.mission.log_id.COP, tpz.mission.logEx.ULMIA, 14)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::setMissionLogEx(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));

    CCharEntity* PChar        = (CCharEntity*)m_PBaseEntity;
    uint8        missionLogID = (uint8)lua_tointeger(L, 1);
    if (missionLogID >= MAX_MISSIONAREA)
    {
        ShowError(CL_RED "Lua::setMissionLogEx: missionLogID %i is invalid\n" CL_RESET, missionLogID);
        return 0;
    }
    int32 n = lua_gettop(L);
    if (n == 3)
    {
        uint8 missionLogExPos = (uint8)lua_tointeger(L, 2);
        if (missionLogExPos > 7)
        {
            ShowError(CL_RED "Lua::setMissionLogEx: position %i is invalid\n" CL_RESET, missionLogExPos);
            return 0;
        }
        uint8 missionLogExValue = (uint8)lua_tointeger(L, 3);
        if (missionLogExValue > 0xF)
        {
            ShowError(CL_RED "Lua::setMissionLogEx: value %i is invalid\n" CL_RESET, missionLogExValue);
            return 0;
        }
        uint32 logEx = (PChar->m_missionLog[missionLogID].logExUpper << 16) | PChar->m_missionLog[missionLogID].logExLower;
        uint32 mask  = ~(0xF << (4 * missionLogExPos));

        logEx &= mask;
        logEx |= missionLogExValue << (4 * missionLogExPos);
        PChar->m_missionLog[missionLogID].logExLower = logEx;
        PChar->m_missionLog[missionLogID].logExUpper = logEx >> 16;
        PChar->pushPacket(new CQuestMissionLogPacket(PChar, missionLogID, LOG_MISSION_CURRENT));
    }
    else if (n == 2)
    {
        uint32 missionLogExValue                     = (uint32)lua_tointeger(L, 2);
        PChar->m_missionLog[missionLogID].logExLower = missionLogExValue;
        PChar->m_missionLog[missionLogID].logExUpper = missionLogExValue >> 16;
    }

    charutils::SaveMissionsList(PChar);

    return 0;
}

/************************************************************************
 *  Function: getMissionLogEx()
 *  Purpose : Gets mission log extra data.
 *  Example : player:getMissionLogEx(tpz.mission.log_id.COP, tpz.mission.logEx.ULMIA)
 *  Notes   :  If arg2 isn't provided, the whole 32 bits are returned.
 ************************************************************************/

inline int32 CLuaBaseEntity::getMissionLogEx(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    CCharEntity* PChar        = (CCharEntity*)m_PBaseEntity;
    uint8        missionLogID = (uint8)lua_tointeger(L, 1);
    if (missionLogID < MAX_MISSIONAREA)
    {
        uint32 logEx = (PChar->m_missionLog[missionLogID].logExUpper << 16) | PChar->m_missionLog[missionLogID].logExLower;
        if (lua_isnumber(L, 2))
        {
            uint8 missionLogExPos = (uint8)lua_tointeger(L, 2);
            if (missionLogExPos > 7)
            {
                ShowError(CL_RED "Lua::getMissionLogEx: position %i is invalid\n" CL_RESET, missionLogExPos);
                return 0;
            }
            lua_pushinteger(L, ((logEx >> (4 * missionLogExPos)) & 0xF));
        }
        else
        {
            lua_pushinteger(L, logEx);
        }
    }
    else
    {
        ShowError(CL_RED "Lua::getMissionLogEx: missionLogID %i is invalid\n" CL_RESET, missionLogID);
        return 0;
    }
    return 1;
}

/************************************************************************
 *  Function: setEminenceCompleted()
 *  Purpose :
 *  Example : player:setEminenceCompleted(1)
 *  Notes   : optional arg 1 flags for repeat record (1/0) (Does not remove from log)
 *            optional arg 2 can set completion state explicitly (1/0)
 ************************************************************************/

inline int32 CLuaBaseEntity::setEminenceCompleted(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    CCharEntity* PChar    = (CCharEntity*)m_PBaseEntity;
    uint16       recordID = (uint16)lua_tointeger(L, 1);

    bool repeat = false;
    if (lua_gettop(L) > 1)
    {
        repeat = lua_tointeger(L, 2) != 0;
    }

    bool status = true;
    if (lua_gettop(L) > 2)
    {
        status = lua_tointeger(L, 3) != 0;
    }

    if (repeat)
    {
        roeutils::SetEminenceRecordProgress(PChar, recordID, 0);
    }
    else
    {
        roeutils::DelEminenceRecord(PChar, recordID);
    }

    roeutils::SetEminenceRecordCompletion(PChar, recordID, status);

    return 0;
}

/************************************************************************
 *  Function: getEminenceCompleted()
 *  Purpose : Returns true if eminence is flagged complete for player
 *  Example : player:getEminenceCompleted(1)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getEminenceCompleted(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    CCharEntity* PChar    = (CCharEntity*)m_PBaseEntity;
    uint16       recordID = (uint16)lua_tointeger(L, 1);

    lua_pushboolean(L, (bool)roeutils::GetEminenceRecordCompletion(PChar, recordID));

    return 1;
}

/************************************************************************
 *  Function: setEminenceProgress(record, progress, total)
 *  Purpose :
 *  Example : player:setEminenceProgress(12, 3, 200)
 *  Notes   : The 3rd param is optional. However, no message will be shown if not given.
 ************************************************************************/

inline int32 CLuaBaseEntity::setEminenceProgress(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));

    CCharEntity* PChar    = (CCharEntity*)m_PBaseEntity;
    uint16       recordID = static_cast<uint16>(lua_tointeger(L, 1));
    uint32       progress = static_cast<uint32>(lua_tointeger(L, 2));
    uint32       total    = static_cast<uint32>(lua_tointeger(L, 3));

    // Determine threshold for sending progress messages
    bool progressNotify{ true };
    if (uint32 threshold = roeutils::RoeSystem.NotifyThresholds[recordID]; threshold > 1)
    {
        uint32 prevStep = static_cast<uint32>(roeutils::GetEminenceRecordProgress(PChar, recordID) / threshold);
        uint32 nextStep = static_cast<uint32>(progress / threshold);
        progressNotify  = nextStep > prevStep;
    }

    bool result = roeutils::SetEminenceRecordProgress(PChar, recordID, progress);
    lua_pushboolean(L, result);

    if (total && progressNotify)
    {
        PChar->pushPacket(new CMessageBasicPacket(PChar, PChar, recordID, 0, MSGBASIC_ROE_RECORD));
        PChar->pushPacket(new CMessageBasicPacket(PChar, PChar, progress, total, MSGBASIC_ROE_PROGRESS));
    }

    return 1;
}

/************************************************************************
 *  Function: getEminenceProgress(record)
 *  Purpose :
 *  Example : player:getEminenceProgress(19)
 *  Notes   : returns nil if player does not have the record.
 ************************************************************************/

inline int32 CLuaBaseEntity::getEminenceProgress(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    if (m_PBaseEntity->objtype != TYPE_PC)
    {
        lua_pushnil(L);
        return 1;
    }

    CCharEntity* PChar    = (CCharEntity*)m_PBaseEntity;
    uint16       recordID = (uint16)lua_tointeger(L, 1);

    if (roeutils::HasEminenceRecord(PChar, recordID))
    {
        uint32 progress = roeutils::GetEminenceRecordProgress(PChar, recordID);
        lua_pushinteger(L, progress);
    }
    else
    {
        lua_pushnil(L);
    }

    return 1;
}

/************************************************************************
 *  Function: addAssault()
 *  Purpose : Adds an assault mission to the player's log
 *  Example : player:addAssault(bit.rshift(option,4))
 *  Notes   : See scripts/zones/Aht_Urhgan_Whitegate/npcs/Famad.lua
 ************************************************************************/

inline int32 CLuaBaseEntity::addAssault(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    uint8 MissionID = (uint8)lua_tointeger(L, 1);

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

    if (PChar->m_assaultLog.current != 0)
    {
        ShowWarning(CL_YELLOW "Lua::addAssault: player has a current assault\n" CL_RESET);
    }
    PChar->m_assaultLog.current = MissionID;
    PChar->pushPacket(new CQuestMissionLogPacket(PChar, MISSION_ASSAULT, LOG_MISSION_CURRENT));

    charutils::SaveMissionsList(PChar);

    return 0;
}

/************************************************************************
 *  Function: delAssault()
 *  Purpose : Deletes an assault mission from a player's log
 *  Example : player:delAssault(currentAssault)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::delAssault(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    uint8 MissionID = (uint8)lua_tointeger(L, 1);

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

    auto current = (uint8)PChar->m_assaultLog.current;

    if (current == MissionID)
    {
        PChar->m_assaultLog.current = 0;
        PChar->pushPacket(new CQuestMissionLogPacket(PChar, MISSION_ASSAULT, LOG_MISSION_CURRENT));
    }
    charutils::SaveMissionsList(PChar);

    return 0;
}

/************************************************************************
 *  Function: getCurrentAssault()
 *  Purpose : Returns the current assault mission for the player
 *  Example : local assaultid = player:getCurrentAssault()
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getCurrentAssault(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    uint8 MissionID = (uint8)((CCharEntity*)m_PBaseEntity)->m_assaultLog.current;

    lua_pushinteger(L, MissionID);
    return 1;
}

/************************************************************************
 *  Function: hasCompletedAssault()
 *  Purpose : Returns true if a player has completed a specified assault
 *  Example : if (v:hasCompletedAssault(v:getCurrentAssault())) then
 *  Notes   : See scripts/zones/Leujaoam_Sanctum/npcs/rune_of_release.lua
 ************************************************************************/

inline int32 CLuaBaseEntity::hasCompletedAssault(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    uint8 MissionID = (uint8)lua_tointeger(L, 1);

    bool complete = ((CCharEntity*)m_PBaseEntity)->m_assaultLog.complete[MissionID];

    lua_pushboolean(L, complete);
    return 1;
}

/************************************************************************
 *  Function: completeAssault()
 *  Purpose : Completes the current assault mission for the player
 *  Example : player:completeAssault(currentAssault)
 *  Notes   : See scripts/zones/Aht_Urhgan_Whitegate/npcs/Rytaal.lua
 ************************************************************************/

inline int32 CLuaBaseEntity::completeAssault(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    uint8 MissionID = (uint8)lua_tointeger(L, 1);

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

    if (PChar->m_assaultLog.current != MissionID)
    {
        ShowWarning(CL_YELLOW "Lua::completeAssault: completion of not current assault\n" CL_RESET);
    }
    PChar->m_assaultLog.current             = 0;
    PChar->m_assaultLog.complete[MissionID] = true;
    PChar->pushPacket(new CQuestMissionLogPacket(PChar, MISSION_ASSAULT, LOG_MISSION_CURRENT));
    PChar->pushPacket(new CQuestMissionLogPacket(PChar, MISSION_ASSAULT, LOG_MISSION_COMPLETE));

    charutils::SaveMissionsList(PChar);

    return 0;
}

/************************************************************************
 *  Function: addKeyItem()
 *  Purpose : Adds a key item to the player
 *  Example : player:addKeyItem(512)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::addKeyItem(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

    uint16 KeyItemID = (uint16)lua_tointeger(L, 1);
    uint8  table     = KeyItemID >> 9;

    charutils::addKeyItem(PChar, KeyItemID);
    PChar->pushPacket(new CKeyItemsPacket(PChar, (KEYS_TABLE)table));

    if (table == 6)
    {
        PChar->pushPacket(new CCharMountsPacket(PChar));
    }
    charutils::SaveKeyItems(PChar);
    return 0;
}

/************************************************************************
 *  Function: hasKeyItem()
 *  Purpose : Returns true if a player has a specified key item
 *  Example : if (player:hasItem(TORN_PAPER)) then
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::hasKeyItem(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    uint16 KeyItemID = (uint16)lua_tointeger(L, 1);

    lua_pushboolean(L, (charutils::hasKeyItem((CCharEntity*)m_PBaseEntity, KeyItemID) != 0));
    return 1;
}

/************************************************************************
 *  Function: delKeyItem()
 *  Purpose : Deletes a key item from the player
 *  Example : player:delKeyItem(SUNBEAM_FRAGMENT)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::delKeyItem(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

    uint16 KeyItemID = (uint16)lua_tointeger(L, 1);

    charutils::delKeyItem(PChar, KeyItemID);
    PChar->pushPacket(new CKeyItemsPacket(PChar, (KEYS_TABLE)(KeyItemID >> 9)));

    charutils::SaveKeyItems(PChar);
    return 0;
}

/************************************************************************
 *  Function: seenKeyItem()
 *  Purpose : Returns true if a player has peeked at the key item
 *  Example : if (player:seenKeyItem(LETTER_FROM_ROH_LATTEH)) then
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::seenKeyItem(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    uint16 KeyItemID = (uint16)lua_tointeger(L, 1);

    lua_pushboolean(L, (charutils::seenKeyItem((CCharEntity*)m_PBaseEntity, KeyItemID) != 0));
    return 1;
}

/************************************************************************
 *  Function: unseenKeyItem()
 *  Purpose : Restores a key item to unseen status
 *  Example : player:unseenKeyItem(512)
 *  Notes   : Some things just can't be unseen... (not implemented though)
 ************************************************************************/

inline int32 CLuaBaseEntity::unseenKeyItem(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

    uint16 KeyItemID = (uint16)lua_tointeger(L, 1);

    charutils::unseenKeyItem(PChar, KeyItemID);
    PChar->pushPacket(new CKeyItemsPacket(PChar, (KEYS_TABLE)(KeyItemID >> 9)));

    charutils::SaveKeyItems(PChar);
    return 0;
}

/************************************************************************
 *  Function: addExp()
 *  Purpose : Adds a set amount of XP to the player
 *  Example : player:addExp(math.random(500,1000))
 *  Notes   : Used in Dynamis Pages, etc
 ************************************************************************/

void CLuaBaseEntity::addExp(uint32 exp)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);

    charutils::AddExperiencePoints(false, PChar, m_PBaseEntity, exp);
}

/************************************************************************
 *  Function: delExp()
 *  Purpose : Takes XP from a player
 *  Example : player:delExp(amount)
 *  Notes   : Used only in GM command takexp.lua
 ************************************************************************/

void CLuaBaseEntity::delExp(uint32 exp)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);

    charutils::DelExperiencePoints(PChar, 0, std::clamp<uint16>(exp, 0, 65535));
}

/************************************************************************
 *  Function: getMerit()
 *  Purpose : Checks for the existence of a merit and returns the value
 *  Example : caster:getMerit(MERIT_DOTON_EFFECT)
 *  Notes   :
 ************************************************************************/

int32 CLuaBaseEntity::getMerit(uint16 merit)
{
    if (m_PBaseEntity->objtype == TYPE_PC)
    {
        auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);

        return PChar->PMeritPoints->GetMeritValue(static_cast<MERIT_TYPE>(merit), PChar);
    }

    return 0;
}

/************************************************************************
 *  Function: getMeritCount()
 *  Purpose : Returns the current value of merits a player has
 *  Example : player:getMeritCount()
 *  Notes   :
 ************************************************************************/

uint8 CLuaBaseEntity::getMeritCount()
{
    if (m_PBaseEntity->objtype == TYPE_PC)
    {
        auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);

        return PChar->PMeritPoints->GetMeritPoints();
    }

    return 0;
}

/************************************************************************
 *  Function: setMerits()
 *  Purpose : Sets the merit points for a player to a specified amount
 *  Example : player:setMerits(30)
 *  Notes   : Used in GM command and Nomad Moogle for Genkai quest
 ************************************************************************/

void CLuaBaseEntity::setMerits(uint8 numPoints)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);

    PChar->PMeritPoints->SetMeritPoints(numPoints);
    PChar->pushPacket(new CMenuMeritPacket(PChar));

    charutils::SaveCharExp(PChar, PChar->GetMJob());
}

/************************************************************************
 *  Function: getGil()
 *  Purpose : Returns the total amount of a gil a player has
 *  Example : player:getGil()
 *  Notes   : Used mostly as a control to make sure player has enough gil
 ************************************************************************/

uint32 CLuaBaseEntity::getGil()
{
    if (m_PBaseEntity->objtype == TYPE_PC)
    {
        CItem* item = static_cast<CCharEntity*>(m_PBaseEntity)->getStorage(LOC_INVENTORY)->GetItem(0);

        if (item == nullptr) // Player has no money
        {
            return 0;
        }
        else if (!item->isType(ITEM_CURRENCY))
        {
            ShowFatalError(CL_RED "lua::getGil : Item in currency slot is not gil!\n" CL_RESET);
            return 0;
        }

        return item->getQuantity();
    }

    if (m_PBaseEntity->objtype == TYPE_MOB)
    {
        auto* PMob = dynamic_cast<CMobEntity*>(m_PBaseEntity);
        if (PMob->CanStealGil())
        {
            return PMob->GetRandomGil();
        }
    }

    return 0;
}

/************************************************************************
 *  Function: addGil()
 *  Purpose : Add a specified amount of gil to the player
 *  Example : player:addGil(500)
 *  Notes   : Use messageSpecial() to display the 'obtained' message
 ************************************************************************/

void CLuaBaseEntity::addGil(int32 gil)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    CItem* item = static_cast<CCharEntity*>(m_PBaseEntity)->getStorage(LOC_INVENTORY)->GetItem(0);

    if (item == nullptr || !item->isType(ITEM_CURRENCY))
    {
        ShowFatalError(CL_RED "lua::addGil : No Gil in currency slot\n" CL_RESET);
        return;
    }

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);
    charutils::UpdateItem(PChar, LOC_INVENTORY, 0, gil);
}

/************************************************************************
 *  Function: setGil()
 *  Purpose : Sets a player's current gil to a specified value
 *  Example : player:setGil(1)
 *  Notes   :
 ************************************************************************/

void CLuaBaseEntity::setGil(int32 amount)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    CItem* item = static_cast<CCharEntity*>(m_PBaseEntity)->getStorage(LOC_INVENTORY)->GetItem(0);

    if (item == nullptr || !item->isType(ITEM_CURRENCY))
    {
        ShowFatalError(CL_RED "lua::setGil : No Gil in currency slot\n" CL_RESET);
        return;
    }

    int32 gil   = amount - item->getQuantity();
    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);

    charutils::UpdateItem(PChar, LOC_INVENTORY, 0, gil);
}

/************************************************************************
 *  Function: delGil()
 *  Purpose : Takes a specified amount of gil from the player
 *  Example : player:delGil(100)
 *  Notes   : Returns bool on success/fail
 ************************************************************************/

bool CLuaBaseEntity::delGil(int32 gil)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    bool result = false;

    CItem* PItem = static_cast<CCharEntity*>(m_PBaseEntity)->getStorage(LOC_INVENTORY)->GetItem(0);

    if (PItem != nullptr && PItem->isType(ITEM_CURRENCY))
    {
        auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);
        result      = charutils::UpdateItem(PChar, LOC_INVENTORY, 0, -gil) == 0xFFFF;
    }
    else
    {
        ShowFatalError(CL_RED "lua::delGil : No Gil in currency slot\n" CL_RESET);
    }

    return result;
}

/************************************************************************
 *  Function: getCurrency()
 *  Purpose : Get a player's current balance of a specified type
 *  Example : player:getCurrency("cruor") -- Must pass a string value
 *  Notes   : See char_points.sql for all currency types
 ************************************************************************/

int32 CLuaBaseEntity::getCurrency(std::string const& currencyType)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);
    return charutils::GetPoints(PChar, currencyType.c_str());
}

/************************************************************************
 *  Function: addCurrency()
 *  Purpose : Add a specified amount to a player's currency balance
 *  Example : player:addCurrency("traverser_stones",3)
 *  Notes   :
 ************************************************************************/

void CLuaBaseEntity::addCurrency(std::string const& currencyType, int32 amount)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);
    charutils::AddPoints(PChar, currencyType.c_str(), amount);
}

/************************************************************************
 *  Function: setCurrency()
 *  Purpose : Set a player's currency to a specified amount
 *  Example : player:setCurrency("zeni_points",400)
 *  Notes   :
 ************************************************************************/

void CLuaBaseEntity::setCurrency(std::string const& currencyType, int32 amount)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);
    charutils::SetPoints(PChar, currencyType.c_str(), amount);
}

/************************************************************************
 *  Function: delCurrency()
 *  Purpose : Remove a specified amount from a player's currency balance
 *  Example : player:delCurrency("traverser_stones",2)
 *  Notes   :
 ************************************************************************/

void CLuaBaseEntity::delCurrency(std::string const& currencyType, int32 amount)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);
    charutils::AddPoints(PChar, currencyType.c_str(), -amount);
}

/************************************************************************
 *  Function: getCP()
 *  Purpose : Returns the current amount of conquest points for a player
 *  Example : player:getCP()
 *  Notes   : Current nation only
 ************************************************************************/

int32 CLuaBaseEntity::getCP()
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);
    return charutils::GetPoints(PChar, charutils::GetConquestPointsName(PChar).c_str());
}

/************************************************************************
 *  Function: addCP()
 *  Purpose : Adds a specified amount of conquest points to the player
 *  Example : player:addCP(50)
 *  Notes   : Current nation only
 ************************************************************************/

void CLuaBaseEntity::addCP(int32 cp)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);

    charutils::AddPoints(PChar, charutils::GetConquestPointsName(PChar).c_str(), cp);
    PChar->pushPacket(new CConquestPacket(PChar));
}

/************************************************************************
 *  Function: delCP()
 *  Purpose : Takes conquest points from a player
 *  Example : player:delCP(2500)
 *  Notes   : Current nation only
 ************************************************************************/

void CLuaBaseEntity::delCP(int32 cp)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);

    charutils::AddPoints(PChar, charutils::GetConquestPointsName(PChar).c_str(), -cp);
    PChar->pushPacket(new CConquestPacket(PChar));
}

/************************************************************************
 *  Function: getSeals()
 *  Purpose : Returns the current seal balance for a player
 *  Example : player:getSeals(type)
 *  Notes   : 0 = Beast; 1 = Kindred; 2 = KCrest; 3 = HKCrest; 4 = SKCrest
 ************************************************************************/

int32 CLuaBaseEntity::getSeals(uint8 sealType)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar  = static_cast<CCharEntity*>(m_PBaseEntity);
    int32 amount = 0;

    switch (sealType)
    {
        case 0:
            amount = charutils::GetPoints(PChar, "beastman_seal");
            break;
        case 1:
            amount = charutils::GetPoints(PChar, "kindred_seal");
            break;
        case 2:
            amount = charutils::GetPoints(PChar, "kindred_crest");
            break;
        case 3:
            amount = charutils::GetPoints(PChar, "high_kindred_crest");
            break;
        case 4:
            amount = charutils::GetPoints(PChar, "sacred_kindred_crest");
            break;
        default:
            break;
    }

    return amount;
}

/************************************************************************
 *  Function: addSeals()
 *  Purpose : Adds to the player's seal balance
 *  Example : player:addSeals(amount, type)
 *  Notes   : 0 = Beast; 1 = Kindred; 2 = KCrest; 3 = HKCrest; 4 = SKCrest
 ************************************************************************/

void CLuaBaseEntity::addSeals(int32 points, uint8 sealType)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);

    switch (sealType)
    {
        case 0:
            charutils::AddPoints(PChar, "beastman_seal", points);
            break;
        case 1:
            charutils::AddPoints(PChar, "kindred_seal", points);
            break;
        case 2:
            charutils::AddPoints(PChar, "kindred_crest", points);
            break;
        case 3:
            charutils::AddPoints(PChar, "high_kindred_crest", points);
            break;
        case 4:
            charutils::AddPoints(PChar, "sacred_kindred_crest", points);
            break;
        default:
            break;
    }
}

/************************************************************************
 *  Function: delSeals()
 *  Purpose : Takes seals from a player's balance
 *  Example : player:delSeals(amount, type)
 *  Notes   : 0 = Beast; 1 = Kindred; 2 = KCrest; 3 = HKCrest; 4 = SKCrest
 ************************************************************************/

void CLuaBaseEntity::delSeals(int32 points, uint8 sealType)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);

    switch (sealType)
    {
        case 0:
            charutils::AddPoints(PChar, "beastman_seal", -points);
            break;
        case 1:
            charutils::AddPoints(PChar, "kindred_seal", -points);
            break;
        case 2:
            charutils::AddPoints(PChar, "kindred_crest", -points);
            break;
        case 3:
            charutils::AddPoints(PChar, "high_kindred_crest", -points);
            break;
        case 4:
            charutils::AddPoints(PChar, "sacred_kindred_crest", -points);
            break;
        default:
            break;
    }
}

/************************************************************************
 *  Function: getAssaultPoint()
 *  Purpose : Get the player's current assault points balance
 *  Example : player:getAssaultPoint(4)
 *  Notes   :
 ************************************************************************/

int32 CLuaBaseEntity::getAssaultPoint(uint8 region)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar  = static_cast<CCharEntity*>(m_PBaseEntity);
    int32 points = 0;

    switch (region)
    {
        case 0:
            points = charutils::GetPoints(PChar, "leujaoam_assault_point");
            break;
        case 1:
            points = charutils::GetPoints(PChar, "mamool_assault_point");
            break;
        case 2:
            points = charutils::GetPoints(PChar, "lebros_assault_point");
            break;
        case 3:
            points = charutils::GetPoints(PChar, "periqia_assault_point");
            break;
        case 4:
            points = charutils::GetPoints(PChar, "ilrusi_assault_point");
            break;
        default:
            break;
    }

    return points;
}

/************************************************************************
 *  Function: addAssaultPoint()
 *  Purpose : Add a specified amount of Assault points to a defined region
 *  Example : player:addAssaultPoint(0,300)
 *  Notes   :
 ************************************************************************/

void CLuaBaseEntity::addAssaultPoint(uint8 region, int32 points)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);

    switch (region)
    {
        case 0:
            charutils::AddPoints(PChar, "leujaoam_assault_point", points);
            break;
        case 1:
            charutils::AddPoints(PChar, "mamool_assault_point", points);
            break;
        case 2:
            charutils::AddPoints(PChar, "lebros_assault_point", points);
            break;
        case 3:
            charutils::AddPoints(PChar, "periqia_assault_point", points);
            break;
        case 4:
            charutils::AddPoints(PChar, "ilrusi_assault_point", points);
            break;
        default:
            break;
    }
}

/************************************************************************
 *  Function: delAssaultPoint()
 *  Purpose : Subtract Assault points from a particular region
 *  Example : player:delAssaultPoint(0,300)
 *  Notes   :
 ************************************************************************/

void CLuaBaseEntity::delAssaultPoint(uint8 region, int32 points)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);

    switch (region)
    {
        case 0:
            charutils::AddPoints(PChar, "leujaoam_assault_point", -points);
            break;
        case 1:
            charutils::AddPoints(PChar, "mamool_assault_point", -points);
            break;
        case 2:
            charutils::AddPoints(PChar, "lebros_assault_point", -points);
            break;
        case 3:
            charutils::AddPoints(PChar, "periqia_assault_point", -points);
            break;
        case 4:
            charutils::AddPoints(PChar, "ilrusi_assault_point", -points);
            break;
        default:
            break;
    }
}

/************************************************************************
 *  Function: addGuildPoints()
 *  Purpose : Add guild points based on the item in a specified slot ID
 *  Example : player:addGuildPoints(GuildID,SlotID)
 *  Notes   :
 ************************************************************************/

auto CLuaBaseEntity::addGuildPoints(uint8 guildID, uint8 slotID) -> std::tuple<uint8, int16>
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    CGuild* PGuild = guildutils::GetGuild(guildID);
    auto*   PChar  = static_cast<CCharEntity*>(m_PBaseEntity);

    int16 points = 0;
    uint8 items  = PGuild->addGuildPoints(PChar, PChar->TradeContainer->getItem(slotID), points);

    return { items, points };
}

/************************************************************************
 *  Function: getHP()
 *  Purpose : Returns current Hit Points of an Entity
 *  Example : player:getHP()
 *  Notes   :
 ************************************************************************/

int32 CLuaBaseEntity::getHP()
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    return static_cast<CBattleEntity*>(m_PBaseEntity)->health.hp;
}

/************************************************************************
 *  Function: getHPP()
 *  Purpose : Returns current Hit Points Percentage of Entity
 *  Example : player:getHPP()
 *  Notes   : Hit Points / Max Hit Points, rounded up to whole integer
 ************************************************************************/

uint8 CLuaBaseEntity::getHPP()
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    return static_cast<CBattleEntity*>(m_PBaseEntity)->GetHPP();
}

/************************************************************************
 *  Function: getMaxHP()
 *  Purpose : Returns the Max Hit Points of an Entity
 *  Example : player:getMaxHP()
 *  Notes   :
 ************************************************************************/

int32 CLuaBaseEntity::getMaxHP()
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    return static_cast<CBattleEntity*>(m_PBaseEntity)->GetMaxHP();
}

/************************************************************************
 *  Function: getBaseHP()
 *  Purpose : Returns the Base Hit Points of an Entity
 *  Example : player:getBaseHP()
 *  Notes   :
 ************************************************************************/

int32 CLuaBaseEntity::getBaseHP()
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    auto* PEntity = static_cast<CBattleEntity*>(m_PBaseEntity);

    return PEntity->health.maxhp;
}

/************************************************************************
 *  Function: addHP()
 *  Purpose : Adds to the Hit Points of an Entity
 *  Example : player:addHP(500)
 *  Notes   :
 ************************************************************************/

int32 CLuaBaseEntity::addHP(int32 hpAdd)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    auto* PBattle = static_cast<CBattleEntity*>(m_PBaseEntity);

    int32 result = PBattle->addHP(hpAdd);

    // will always remove sleep effect
    PBattle->StatusEffectContainer->DelStatusEffect(EFFECT_SLEEP);
    PBattle->StatusEffectContainer->DelStatusEffect(EFFECT_SLEEP_II);
    PBattle->StatusEffectContainer->DelStatusEffect(EFFECT_LULLABY);

    return result;
}

/************************************************************************
 *  Function: setHP()
 *  Purpose : Sets the Hit Points of an Entity
 *  Example : player:setHP(player:getMaxHP())
 *  Notes   :
 ************************************************************************/

void CLuaBaseEntity::setHP(int32 value)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    auto* PBattle = static_cast<CBattleEntity*>(m_PBaseEntity);

    PBattle->health.hp = 0;
    PBattle->addHP(value);
    m_PBaseEntity->updatemask |= UPDATE_HP;

    // When setting the HP to 0 the entity "falls to the ground" so the last attacker needs to be cleared
    if (value == 0)
    {
        PBattle->PLastAttacker = nullptr;
    }
}

/************************************************************************
 *  Function: restoreHP()
 *  Purpose : Restores the Hit Points of an Entity by a specified amount
 *  Example : player:restoreHP(1000)
 *  Notes   : Returns amount restored if not dead
 ************************************************************************/

int32 CLuaBaseEntity::restoreHP(int32 restoreAmt)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    if (m_PBaseEntity->animation != ANIMATION_DEATH)
    {
        int32 result = static_cast<CBattleEntity*>(m_PBaseEntity)->addHP(restoreAmt);

        return result;
    }

    return 0;
}

/************************************************************************
 *  Function: delHP()
 *  Purpose : Subtracts from the Hit Points of an Entity
 *  Example : player:delHP(500)
 *  Notes   :
 ************************************************************************/

void CLuaBaseEntity::delHP(int32 delAmt)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    static_cast<CBattleEntity*>(m_PBaseEntity)->takeDamage(delAmt);
}

/************************************************************************
 *  Function: takeDamage()
 *  Purpose : Takes damage from the provided attacker. If no attacker is provided then it clears the last attacker.
 *  Example : target:takeDamage(500, attacker=nil, attackType=ATTACK_NONE, damageType=DAMAGE_NONE, flags={wakeUp=true})
 ************************************************************************/

inline int32 CLuaBaseEntity::takeDamage(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    int32 damage = (int32)(lua_tointeger(L, 1));

    // Attempt to retrieve the attacker
    CBattleEntity* PAttacker = nullptr;
    if (!lua_isnil(L, 2) && lua_isuserdata(L, 2))
    {
        CLuaBaseEntity* PLuaAttacker = nullptr;
        TPZ_DEBUG_BREAK_IF(PLuaAttacker == nullptr);
        TPZ_DEBUG_BREAK_IF(PLuaAttacker->m_PBaseEntity->objtype == TYPE_NPC);
        PAttacker = dynamic_cast<CBattleEntity*>(PLuaAttacker->m_PBaseEntity);
    }

    CBattleEntity* PDefender = dynamic_cast<CBattleEntity*>(m_PBaseEntity);

    // Check for special flags which may prevent damage from waking up the target
    bool wakeUp        = true;
    bool breakBind     = true;
    bool removePetrify = true;

    if (!lua_isnil(L, 5) && lua_istable(L, 5))
    {
        // Attempt to wake up the target unless wakeUp is provided and is false.
        lua_getfield(L, 5, "wakeUp");
        wakeUp = (lua_isnil(L, -1) || !lua_isboolean(L, -1) || lua_toboolean(L, -1));

        // Remove petrification unless removePetrify is provided and is false
        lua_getfield(L, 5, "removePetrify");
        removePetrify = (lua_isnil(L, -1) || !lua_isboolean(L, -1) || lua_toboolean(L, -1));

        // Attempt to break Bind unless breakBind is provided and is false
        lua_getfield(L, 5, "breakBind");
        breakBind = (lua_isnil(L, -1) || !lua_isboolean(L, -1) || lua_toboolean(L, -1));
    }

    // Deal damage and liberate target when applicable
    if (damage > 0)
    {
        ATTACK_TYPE attackType = !lua_isnil(L, 3) && lua_isnumber(L, 3) ? static_cast<ATTACK_TYPE>(lua_tointeger(L, 3)) : ATTACK_TYPE::NONE;
        DAMAGE_TYPE damageType = !lua_isnil(L, 4) && lua_isnumber(L, 4) ? static_cast<DAMAGE_TYPE>(lua_tointeger(L, 4)) : DAMAGE_TYPE::NONE;

        PDefender->takeDamage(damage, PAttacker, attackType, damageType);

        if (wakeUp)
        {
            PDefender->StatusEffectContainer->WakeUp();
        }

        if (removePetrify)
        {
            PDefender->StatusEffectContainer->DelStatusEffect(EFFECT_PETRIFICATION);
        }
    }

    // Bind has a chance to break from all direct attacks, even if they don't deal damage
    if (PAttacker && breakBind)
    {
        battleutils::BindBreakCheck(PAttacker, PDefender);
    }

    return 0;
}

/************************************************************************
 *  Function: hideHP()
 *  Purpose : Toggles the display of the Hit Points bar for a Mob or NPC
 *  Example : mob:hideHP(true)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::hideHP(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isboolean(L, 1));
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_MOB && m_PBaseEntity->objtype != TYPE_NPC);

    if (m_PBaseEntity->objtype == TYPE_MOB)
    {
        ((CMobEntity*)m_PBaseEntity)->HideHP(lua_toboolean(L, 1));
    }
    else if (m_PBaseEntity->objtype == TYPE_NPC)
    {
        ((CNpcEntity*)m_PBaseEntity)->HideHP(lua_toboolean(L, 1));
    }
    m_PBaseEntity->updatemask |= UPDATE_HP;
    return 0;
}

/************************************************************************
 *  Function: getMP()
 *  Purpose : Returns the current Mana Points of an entity
 *  Example : player:getMP()
 ************************************************************************/

int32 CLuaBaseEntity::getMP()
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    return static_cast<CBattleEntity*>(m_PBaseEntity)->health.mp;
}

/************************************************************************
 *  Function: getMaxMP()
 *  Purpose : Returns the Max Mana Points of an entity
 *  Example : player:getMaxMP()
 ************************************************************************/

int32 CLuaBaseEntity::getMaxMP()
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    return static_cast<CBattleEntity*>(m_PBaseEntity)->GetMaxMP();
}

/************************************************************************
 *  Function: getBaseMP()
 *  Purpose : Returns the Base Mana Points of an entity
 *  Example : player:getBaseMP()
 ************************************************************************/

int32 CLuaBaseEntity::getBaseMP()
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    auto* PEntity = static_cast<CBattleEntity*>(m_PBaseEntity);
    return PEntity->health.maxmp;
}

/************************************************************************
 *  Function: addMP()
 *  Purpose : Adds Mana Points to an entity
 *  Example : player:addMP(50)
 ************************************************************************/

int32 CLuaBaseEntity::addMP(int32 amount)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    return static_cast<CBattleEntity*>(m_PBaseEntity)->addMP(amount);
}

/************************************************************************
 *  Function: setMP()
 *  Purpose : Sets the Mana Points of an entity to a specified amount
 *  Example : player:setMP(player:getMaxMP())
 ************************************************************************/

inline int32 CLuaBaseEntity::setMP(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    ((CBattleEntity*)m_PBaseEntity)->health.mp = 0;
    int32 value                                = (int32)(lua_tointeger(L, 1) - ((CBattleEntity*)m_PBaseEntity)->health.mp);
    ((CBattleEntity*)m_PBaseEntity)->addMP(value);

    return 0;
}

/************************************************************************
 *  Function: restoreMP()
 *  Purpose : Restores Mana Points to a player
 *  Example : player:restoreMP(player:getMaxHP() - player:getHP())
 *  Notes   : Returns the restored amount if not dead
 ************************************************************************/

int32 CLuaBaseEntity::restoreMP(int32 amount)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    if (m_PBaseEntity->animation != ANIMATION_DEATH)
    {
        return static_cast<CBattleEntity*>(m_PBaseEntity)->addMP(amount);
    }

    return 0;
}

/************************************************************************
 *  Function: delMP()
 *  Purpose : Subtracts Mana Points from an Entity
 *  Example : player:delMP(1000)
 ************************************************************************/

void CLuaBaseEntity::delMP(int32 amount)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    static_cast<CBattleEntity*>(m_PBaseEntity)->addMP(-amount);
}

/************************************************************************
 *  Function: getTP()
 *  Purpose : Returns the current Tactical Points of an Entity
 *  Example : if (player:getTP() > 1000) then
 ************************************************************************/

float CLuaBaseEntity::getTP()
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    auto* PBattle = static_cast<CBattleEntity*>(m_PBaseEntity);
    return static_cast<float>(PBattle->health.tp);
}

/************************************************************************
 *  Function: addTP()
 *  Purpose : Adds Tactical Points to an Entity
 *  Example : player:addTP(1000) - Icarus Wing
 ************************************************************************/

void CLuaBaseEntity::addTP(int16 amount)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    static_cast<CBattleEntity*>(m_PBaseEntity)->addTP(amount);
}

/************************************************************************
 *  Function: setTP()
 *  Purpose : Sets the Tactical Points of an entity
 *  Example : player:setTP(1000)
 ************************************************************************/

void CLuaBaseEntity::setTP(int16 value)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    auto* PBattle = static_cast<CBattleEntity*>(m_PBaseEntity);

    PBattle->health.tp    = 0;
    int16 available_value = value - (PBattle->health.tp);
    PBattle->addTP(available_value);
}

/************************************************************************
 *  Function: delTP()
 *  Purpose : Subtracts Tactical Points from an Entity
 *  Example : player:delTP(50)
 ************************************************************************/

void CLuaBaseEntity::delTP(int16 amount)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    static_cast<CBattleEntity*>(m_PBaseEntity)->addTP(-amount);
}

/************************************************************************
 *  Function: updateHealth()
 *  Purpose : Forces a health update for an Entity
 *  Example : target:updateHealth()
 *  Notes   : Currently only used in Spirit Surge; Health update for
 *  Notes   : Level Sync is handled by CParty::SetSyncTarget
 ************************************************************************/

void CLuaBaseEntity::updateHealth()
{
    static_cast<CBattleEntity*>(m_PBaseEntity)->UpdateHealth();
}

/************************************************************************
 *  Function: capSkill()
 *  Purpose : Caps a particular skill for a PC
 *  Example : player:capSkill(DAGGER_SKILL)
 *  Notes   :
 ************************************************************************/

void CLuaBaseEntity::capSkill(uint8 skill)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    if (skill < MAX_SKILLTYPE)
    {
        auto* PChar = dynamic_cast<CCharEntity*>(m_PBaseEntity);
        // CItemWeapon* PItem = ((CBattleEntity*)m_PBaseEntity)->m_Weapons[SLOT_MAIN];
        /* let's just ignore this part for the moment
        //remove modifiers if valid
        if(skill>=1 && skill<=12 && PItem!=nullptr && PItem->getSkillType()==skill){
            PChar->delModifier(Mod::ATT, PChar->GetSkill(skill));
            PChar->delModifier(Mod::ACC, PChar->GetSkill(skill));
        }
        */
        uint16 maxSkill                   = 10 * battleutils::GetMaxSkill((SKILLTYPE)skill, PChar->GetMJob(), PChar->GetMLevel());
        PChar->RealSkills.skill[skill]    = maxSkill; // set to capped
        PChar->WorkingSkills.skill[skill] = maxSkill / 10;
        PChar->WorkingSkills.skill[skill] |= 0x8000; // set blue capped flag
        PChar->pushPacket(new CCharSkillsPacket(PChar));
        charutils::CheckWeaponSkill(PChar, skill);
        /* and ignore this part
        //reapply modifiers if valid
        if(skill>=1 && skill<=12 && PItem!=nullptr && PItem->getSkillType()==skill){
            PChar->addModifier(Mod::ATT, PChar->GetSkill(skill));
            PChar->addModifier(Mod::ACC, PChar->GetSkill(skill));
        }
        */
        charutils::SaveCharSkills(PChar, skill); // save to db
    }
}

/************************************************************************
 *  Function: capAllSkills()
 *  Purpose : Cap all skills for a PC
 *  Example : player:capAllSkills()
 *  Notes   :
 ************************************************************************/

void CLuaBaseEntity::capAllSkills()
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);

    for (uint8 i = 1; i < 45; ++i)
    {
        const char* Query = "INSERT INTO char_skills "
                            "SET "
                            "charid = %u,"
                            "skillid = %u,"
                            "value = %u,"
                            "rank = %u "
                            "ON DUPLICATE KEY UPDATE value = %u, rank = %u;";

        Sql_Query(SqlHandle, Query, PChar->id, i, 5000, PChar->RealSkills.rank[i], 5000, PChar->RealSkills.rank[i]);

        uint16 maxSkill               = 10 * battleutils::GetMaxSkill(static_cast<SKILLTYPE>(i), PChar->GetMJob(), PChar->GetMLevel());
        PChar->RealSkills.skill[i]    = maxSkill; // set to capped
        PChar->WorkingSkills.skill[i] = maxSkill / 10;
        PChar->WorkingSkills.skill[i] |= 0x8000; // set blue capped flag
    }
    charutils::CheckWeaponSkill(PChar, SKILL_NONE);
    PChar->pushPacket(new CCharSkillsPacket(PChar));
}

/************************************************************************
 *  Function: getSkillLevel()
 *  Purpose : Returns the level for a specified skill of a PC
 *  Example : player:getSkillLevel(ENHANCING_MAGIC)
 *  Notes   :
 ************************************************************************/

uint16 CLuaBaseEntity::getSkillLevel(uint16 skillId)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype & TYPE_NPC);

    TPZ_DEBUG_BREAK_IF(skillId >= MAX_SKILLTYPE);

    return static_cast<CBattleEntity*>(m_PBaseEntity)->GetSkill(skillId);
}

/************************************************************************
 *  Function: setSkillLevel()
 *  Purpose : Sets a particular skill for a PC
 *  Example : player:setSkillLevel(ENHANCING_MAGIC, 200)
 *  Notes   :
 ************************************************************************/

void CLuaBaseEntity::setSkillLevel(uint8 SkillID, uint16 SkillAmount)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(SkillID >= MAX_SKILLTYPE);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);

    PChar->RealSkills.skill[SkillID]    = SkillAmount;
    PChar->WorkingSkills.skill[SkillID] = (SkillAmount / 10) * 0x20 + PChar->WorkingSkills.rank[SkillID];

    charutils::BuildingCharSkillsTable(PChar);
    charutils::CheckWeaponSkill(PChar, SkillID);
    charutils::SaveCharSkills(PChar, SkillID);

    PChar->pushPacket(new CCharSkillsPacket(PChar));
}

/************************************************************************
 *  Function: getMaxSkillLevel()
 *  Purpose : Returns the Max Skill Level for a PC's current main job
 *  Example : master:getMaxSkillLevel(avatar:getMainLvl(), JOBS.SMN, SUMMONING_SKILL), 0, 200)
 *  Notes   : Used in Meteor, summons, and some Mob TP moves
 ************************************************************************/

uint16 CLuaBaseEntity::getMaxSkillLevel(uint8 skillId, uint8 jobId, uint8 level)
{
    auto skill = static_cast<SKILLTYPE>(skillId);
    auto job   = static_cast<JOBTYPE>(jobId);

    return battleutils::GetMaxSkill(skill, job, level);
}

/************************************************************************
 *  Function: getSkillRank()
 *  Purpose : Returns the Real Rank for a particular skill
 *  Example : player:getSkillRank(craftID)
 *  Notes   : TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);?
 ************************************************************************/

uint8 CLuaBaseEntity::getSkillRank(uint8 rankID)
{
    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);

    return PChar->RealSkills.rank[rankID];
}

/************************************************************************
 *  Function: setSkillRank()
 *  Purpose : Sets a Skill Rank for a particular skill
 *  Example : player:setSkillRank(SKILL_DIG, 20)
 *  Notes   :
 ************************************************************************/

void CLuaBaseEntity::setSkillRank(uint8 skillID, uint8 newrank)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);

    PChar->WorkingSkills.rank[skillID]  = newrank;
    PChar->WorkingSkills.skill[skillID] = (PChar->RealSkills.skill[skillID] / 10) * 0x20 + newrank;
    PChar->RealSkills.rank[skillID]     = newrank;
    // PChar->RealSkills.skill[skillID] += 1;

    charutils::BuildingCharSkillsTable(PChar);
    charutils::SaveCharSkills(PChar, skillID);
    PChar->pushPacket(new CCharSkillsPacket(PChar));
}

/************************************************************************
 *  Function: getCharSkillLevel()
 *  Purpose : Returns the level for a particular skill
 *  Example : player:getCharSkillLevel(SKILL_DIG)
 *  Notes   : Only used for Chocobo Digging currently
 ************************************************************************/

uint16 CLuaBaseEntity::getCharSkillLevel(uint8 skillID)
{
    if (m_PBaseEntity->objtype == TYPE_PC)
    {
        auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);

        return PChar->RealSkills.skill[skillID];
    }

    return 0;
}

/************************************************************************
 *  Function: addLearnedWeaponskill()
 *  Purpose : Manually add a new weaponskill for the player using WSID
 *  Example : player:addLearnedWeaponskill(DECIMATION)
 *  Notes   : Do not see implemented in any script
 ************************************************************************/

void CLuaBaseEntity::addLearnedWeaponskill(uint8 wsID)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);

    charutils::addLearnedWeaponskill(PChar, wsID);
    charutils::BuildingCharWeaponSkills(PChar);
    charutils::SaveLearnedAbilities(PChar);
    PChar->pushPacket(new CCharAbilitiesPacket(PChar));
}

/************************************************************************
 *  Function: hasLearnedWeaponskill()
 *  Purpose : Returns true if a player has learned a particular weaponskill
 *  Example : if (player:hasLearnedWeaponskill(DECIMATION)) then
 *  Notes   :
 ************************************************************************/

bool CLuaBaseEntity::hasLearnedWeaponskill(uint8 wsID)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    return (charutils::hasLearnedWeaponskill(static_cast<CCharEntity*>(m_PBaseEntity), wsID) != 0);
}

/************************************************************************
 *  Function: delLearnedWeaponskill()
 *  Purpose : Removes a learned weaponskill from the player
 *  Example : player:delLearnedWeaponskill(ASURAN_FISTS)
 *  Notes   :
 ************************************************************************/

void CLuaBaseEntity::delLearnedWeaponskill(uint8 wsID)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);

    charutils::delLearnedWeaponskill(PChar, wsID);
    charutils::BuildingCharWeaponSkills(PChar);
    charutils::SaveLearnedAbilities(PChar);
    PChar->pushPacket(new CCharAbilitiesPacket(PChar));
}

/************************************************************************
 *  Function: addWeaponSkillPoints()
 *  Purpose : Removes a learned weaponskill from the player
 *  Example : player:addWeaponSkillPoints(tpz.slot.MAIN,300)
 *  Notes   : Returns true if points were successfully added.
 ************************************************************************/

bool CLuaBaseEntity::addWeaponSkillPoints(uint8 slotID, uint16 points)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);
    auto  slot  = static_cast<SLOTTYPE>(slotID);

    return charutils::AddWeaponSkillPoints(PChar, slot, points);
}

/************************************************************************
 *  Function: addLearnedAbility()
 *  Purpose : Adds a new learned ability to the player
 *  Example : target:addLearnedAbility(89) -- Chaos Roll
 *  Notes   : Used exclusively for Corsair Die usage
 ************************************************************************/

inline int32 CLuaBaseEntity::addLearnedAbility(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

    uint16 AbilityID = (uint16)lua_tointeger(L, 1);

    if (charutils::addLearnedAbility(PChar, AbilityID))
    {
        charutils::addAbility(PChar, AbilityID);
        charutils::SaveLearnedAbilities(PChar);
        PChar->pushPacket(new CCharAbilitiesPacket(PChar));
        PChar->pushPacket(new CMessageBasicPacket(PChar, PChar, 0, 0, 442));
    }
    return 0;
}

/************************************************************************
 *  Function: hasLearnedAbility()
 *  Purpose : Returns true if player has a particular learned ability
 *  Example : target:hasLearnedAbility(84) -- Healer's Roll
 *  Notes   : Although canLearnAbility is similar, they have distinct diff
 ************************************************************************/

inline int32 CLuaBaseEntity::hasLearnedAbility(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    uint16 AbilityID = (uint16)lua_tointeger(L, 1);

    lua_pushboolean(L, (charutils::hasLearnedAbility((CCharEntity*)m_PBaseEntity, AbilityID) != 0));
    return 1;
}

/************************************************************************
 *  Function: canLearnAbility()
 *  Purpose : Returns true if player can learn the ability
 *  Example : if (player:canLearnAbility(89)) -- Chaos Roll
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::canLearnAbility(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    uint32 Message   = 0;
    uint16 AbilityID = (uint16)lua_tointeger(L, 1);

    if (charutils::hasLearnedAbility((CCharEntity*)m_PBaseEntity, AbilityID))
    {
        Message = 444;
    }
    else if (!ability::CanLearnAbility((CCharEntity*)m_PBaseEntity, AbilityID))
    {
        Message = 443;
    }
    lua_pushinteger(L, Message);
    return 1;
}

/************************************************************************
 *  Function: delLearnedAbility()
 *  Purpose : Removes a learned ability from the player
 *  Example : player:delLearnedAbility(89) -- Chaos Roll
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::delLearnedAbility(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

    uint16 AbilityID = (uint16)lua_tointeger(L, 1);

    if (charutils::delLearnedAbility(PChar, AbilityID))
    {
        charutils::SaveLearnedAbilities(PChar);
        PChar->pushPacket(new CCharAbilitiesPacket(PChar));
    }
    return 0;
}

/************************************************************************
 *  Function: addSpell()
 *  Purpose : Adds a specified spell to the player
 *  Example : player:addSpell(128)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::addSpell(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    CCharEntity* PChar  = (CCharEntity*)m_PBaseEntity;
    bool         silent = false;
    bool         save   = true;

    int32 n = lua_gettop(L);
    if (n > 1)
    {
        silent = lua_toboolean(L, 2);
        if (n > 2)
        {
            save = lua_toboolean(L, 3);
        }
    }

    uint16 SpellID = (uint16)lua_tointeger(L, 1);

    if (charutils::addSpell(PChar, SpellID))
    {
        if (!silent)
        {
            PChar->pushPacket(new CCharSpellsPacket(PChar));
            PChar->pushPacket(new CMessageBasicPacket(PChar, PChar, 0, 0, 23));
        }

        if (save)
        {
            charutils::SaveSpell(PChar, SpellID);
        }
    }
    return 0;
}

/************************************************************************
 *  Function: hasSpell()
 *  Purpose : Returns true if a player has learned a particular spell
 *  Example : if (player:hasSpell(125)) then
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::hasSpell(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    uint16 SpellID = (uint16)lua_tointeger(L, 1);

    lua_pushboolean(L, (charutils::hasSpell((CCharEntity*)m_PBaseEntity, SpellID) != 0));
    return 1;
}

/************************************************************************
 *  Function: canLearnSpell()
 *  Purpose : Returns true if a player can learn a particular spell
 *  Example : if (player:canLearnSpell(528)) then
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::canLearnSpell(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    uint32 Message = 0;
    uint16 spellID = (uint16)lua_tointeger(L, 1);

    if (charutils::hasSpell((CCharEntity*)m_PBaseEntity, spellID))
    {
        Message = 96;
    }
    else if (!spell::CanUseSpell((CCharEntity*)m_PBaseEntity, static_cast<SpellID>(spellID)))
    {
        Message = 95;
    }
    lua_pushinteger(L, Message);
    return 1;
}

/************************************************************************
 *  Function: delSpell()
 *  Purpose : Deletes a learned spell from a player
 *  Example : player:delSpell(528)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::delSpell(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

    uint16 SpellID = (uint16)lua_tointeger(L, 1);

    if (charutils::delSpell(PChar, SpellID))
    {
        charutils::DeleteSpell(PChar, SpellID);
        PChar->pushPacket(new CCharSpellsPacket(PChar));
    }
    return 0;
}

/************************************************************************
 *  Function: recalculateSkillsTable()
 *  Purpose : Recalculates skill tables to get new values and calculations
 *  Example : target:recalculateSkillsTable()
 *  Notes   : Used exclusively for Scholar abilities
 ************************************************************************/

inline int32 CLuaBaseEntity::recalculateSkillsTable(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC)

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

    charutils::BuildingCharSkillsTable(PChar);
    charutils::BuildingCharWeaponSkills(PChar);

    PChar->pushPacket(new CCharSkillsPacket(PChar));
    PChar->pushPacket(new CCharRecastPacket(PChar));
    PChar->pushPacket(new CCharAbilitiesPacket(PChar));
    return 0;
}

/************************************************************************
 *  Function: recalculateAbilitiesTable()
 *  Purpose : Recalculates ability tables to get new values and calculations
 *  Example : target:recalculateAbilitiesTable()
 *  Notes   : Used exlusively for Scholar abilities and Astral Flow
 ************************************************************************/

inline int32 CLuaBaseEntity::recalculateAbilitiesTable(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC)

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

    charutils::BuildingCharAbilityTable(PChar);
    charutils::BuildingCharTraitsTable(PChar);
    charutils::BuildingCharWeaponSkills(PChar);
    if (PChar->PPet != nullptr && PChar->PPet->objtype == TYPE_PET)
    {
        charutils::BuildingCharPetAbilityTable(PChar, (CPetEntity*)PChar->PPet, ((CPetEntity*)PChar->PPet)->m_PetID);
    }

    PChar->pushPacket(new CCharAbilitiesPacket(PChar));
    return 0;
}

/************************************************************************
 *  Function: getParty()
 *  Purpose : Returns a Lua table of party member Entity objects
 *  Example : local party = player:getParty()
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getParty(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    CParty* party = ((CBattleEntity*)m_PBaseEntity)->PParty;

    int size = 0;
    if (party)
    {
        size = party->MemberCount(m_PBaseEntity->getZone());
    }
    else
    {
        size = 1;
    }

    lua_createtable(L, size, 0);
    int i = 1;
    ((CBattleEntity*)m_PBaseEntity)->ForParty([&L, &i](CBattleEntity* member) {
        lua_getglobal(L, "CBaseEntity");
        lua_pushstring(L, "new");
        lua_gettable(L, -2);
        lua_insert(L, -2);
        lua_pushlightuserdata(L, (void*)member);
        lua_pcall(L, 2, 1, 0);

        lua_rawseti(L, -2, i++);
    });

    return 1;
}

/************************************************************************
 *  Function: getPartyWithTrusts()
 *  Purpose : Returns a Lua table of party member and trust Entity objects
 *  Example : local party = player:getPartyWithTrusts()
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getPartyWithTrusts(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    CParty* party = ((CCharEntity*)m_PBaseEntity)->PParty;

    int size = 0;
    if (party)
    {
        size = party->MemberCount(m_PBaseEntity->getZone());
    }
    else
    {
        size = 1;
    }

    lua_createtable(L, size, 0);
    int i = 1;
    ((CCharEntity*)m_PBaseEntity)->ForPartyWithTrusts([&L, &i](CBattleEntity* member) {
        lua_getglobal(L, "CBaseEntity");
        lua_pushstring(L, "new");
        lua_gettable(L, -2);
        lua_insert(L, -2);
        lua_pushlightuserdata(L, (void*)member);
        lua_pcall(L, 2, 1, 0);

        lua_rawseti(L, -2, i++);
    });

    return 1;
}

/************************************************************************
 *  Function: getPartySize()
 *  Purpose : Returns the count of members in the party
 *  Example : local count = player:getPartySize()
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getPartySize(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    uint8 allianceparty = lua_isnil(L, 1) ? 0 : (uint8)lua_tonumber(L, 1);
    uint8 partysize     = 1;

    if (((CBattleEntity*)m_PBaseEntity)->PParty != nullptr)
    {
        if (allianceparty == 0)
        {
            partysize = (uint8)((CBattleEntity*)m_PBaseEntity)->PParty->members.size();
        }
        else if (((CBattleEntity*)m_PBaseEntity)->PParty->m_PAlliance != nullptr)
        {
            partysize = (uint8)((CBattleEntity*)m_PBaseEntity)->PParty->m_PAlliance->partyList.at(allianceparty)->members.size();
        }
    }

    lua_pushnumber(L, partysize);
    return 1;
}

/************************************************************************
 *  Function: hasPartyJob()
 *  Purpose : Loops over party members and returns true if job is found
 *  Example : if (caster:hasPartyJob(JOBS.DRK)) then
 *  Notes   : Highly useful for future addition of features
 ************************************************************************/

inline int32 CLuaBaseEntity::hasPartyJob(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    auto job = lua_tointeger(L, 1);

    if (((CCharEntity*)m_PBaseEntity)->PParty != nullptr)
    {
        for (auto& member : ((CCharEntity*)m_PBaseEntity)->PParty->members)
        {
            CCharEntity* PTarget = (CCharEntity*)member;
            if (PTarget->GetMJob() == job)
            {
                lua_pushboolean(L, true);
                return 1;
            }
            for (auto* PTrust : PTarget->PTrusts)
            {
                if (PTrust->GetMJob() == job)
                {
                    lua_pushboolean(L, true);
                    return 1;
                }
            }
        }
    }
    lua_pushboolean(L, false);
    return 1;
}

/************************************************************************
 *  Function: getPartyMember()
 *  Purpose : Returns the object Entity of a Party Member from another Party/Alliance
 *  Example : player:getPartyMember(4?)
 *  Notes   : Passed value is position in party? What is this used for?
 ************************************************************************/

inline int32 CLuaBaseEntity::getPartyMember(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, -1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, -2));

    uint8 member        = (uint8)lua_tonumber(L, -1);
    uint8 allianceparty = (uint8)lua_tonumber(L, -2);

    CBattleEntity* PTargetChar = nullptr;

    if (allianceparty == 0 && member == 0)
    {
        PTargetChar = ((CBattleEntity*)m_PBaseEntity);
    }
    else if (((CBattleEntity*)m_PBaseEntity)->PParty != nullptr)
    {
        if (allianceparty == 0 && member <= ((CBattleEntity*)m_PBaseEntity)->PParty->members.size())
        {
            PTargetChar = ((CBattleEntity*)m_PBaseEntity)->PParty->members[member];
        }
        else if (((CBattleEntity*)m_PBaseEntity)->PParty->m_PAlliance != nullptr &&
                 member <= ((CBattleEntity*)m_PBaseEntity)->PParty->m_PAlliance->partyList.at(allianceparty)->members.size())
        {
            PTargetChar = ((CBattleEntity*)m_PBaseEntity)->PParty->m_PAlliance->partyList.at(allianceparty)->members[member];
        }
    }

    if (PTargetChar != nullptr)
    {
        lua_getglobal(L, "CBaseEntity");
        lua_pushstring(L, "new");
        lua_gettable(L, -2);
        lua_insert(L, -2);
        lua_pushlightuserdata(L, (void*)PTargetChar);
        lua_pcall(L, 2, 1, 0);
        return 1;
    }
    ShowError(CL_RED "Lua::getPartyMember :: Member or Alliance Number is not valid.\n" CL_RESET);
    lua_pushnil(L);
    return 1;
}

/************************************************************************
 *  Function: getPartyLeader()
 *  Purpose : Returns the entity object of the party leader
 *  Example : local leader = player:getPartyLeader()
 *  Notes   : Todo: also add ability for find Alliance Leader via lua?
 ************************************************************************/

inline int32 CLuaBaseEntity::getPartyLeader(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;
    if (PChar->PParty)
    {
        CBattleEntity* PLeader = PChar->PParty->GetLeader();
        if (PLeader != nullptr)
        {
            lua_getglobal(L, "CBaseEntity");
            lua_pushstring(L, "new");
            lua_gettable(L, -2);
            lua_insert(L, -2);
            lua_pushlightuserdata(L, (void*)PLeader);
            lua_pcall(L, 2, 1, 0);
            return 1;
        }
    }

    lua_pushnil(L);
    return 1;
}

/************************************************************************
 *  Function: forMembersInRange()
 *  Purpose : Apply function to party members within range
 *  Example : forMembersInRange(target, distance, function)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::forMembersInRange(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isfunction(L, 2));

    auto* target   = (CBattleEntity*)m_PBaseEntity;
    auto  range    = (uint8)lua_tointeger(L, 1);
    auto  function = luautils::register_fp(2);

    target->ForParty([&target, &range, &function](CBattleEntity* member) {
        if (target->loc.zone == member->loc.zone && distanceSquared(target->loc.p, member->loc.p) < (range * range))
        {
            std::ignore = member;
            // luautils::pushFunc(function);
            // luautils::pushArg<CBattleEntity*>(member);
            // luautils::callFunc(1);
        }
    });

    luautils::unregister_fp(function);

    return 0;
}

/************************************************************************
 *  Function: addPartyEffect()
 *  Purpose : Adds effect to members of the entire party
 *  Example : player:addPartyEffect(EFFECT, 1, 2, 3, ...)?
 *  Notes   : Must have at least three members, scales to six max
 ************************************************************************/

inline int32 CLuaBaseEntity::addPartyEffect(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 3) || !lua_isnumber(L, 3));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 4) || !lua_isnumber(L, 4));

    int32 n = lua_gettop(L);

    CStatusEffect* PEffect =
        new CStatusEffect((EFFECT)lua_tointeger(L, 1), (uint16)lua_tointeger(L, 2), (uint16)lua_tointeger(L, 3), (uint16)lua_tointeger(L, 4),
                          (n >= 5 ? (uint16)lua_tointeger(L, 5) : 0), (n >= 6 ? (uint16)lua_tointeger(L, 6) : 0), (n >= 7 ? (uint16)lua_tointeger(L, 7) : 0));

    CBattleEntity* PEntity = ((CBattleEntity*)m_PBaseEntity);

    PEntity->ForParty([PEffect](CBattleEntity* PMember) { PMember->StatusEffectContainer->AddStatusEffect(PEffect); });
    return 0;
}

/************************************************************************
 *  Function: hasPartyEffect()
 *  Purpose : Returns true if all members of party have a specified effect
 *  Example : if (player:hasPartyEffect(EFFECT)) then
 *  Notes   : Currently not used in any script
 ************************************************************************/

inline int32 CLuaBaseEntity::hasPartyEffect(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    CCharEntity* PChar = ((CCharEntity*)m_PBaseEntity);

    if (PChar->PParty != nullptr)
    {
        for (const auto& member : PChar->PParty->members)
        {
            if (member->loc.zone == PChar->loc.zone)
            {
                if (member->StatusEffectContainer->HasStatusEffect((EFFECT)lua_tointeger(L, 1)))
                {
                    lua_pushboolean(L, true);
                    return 1;
                }
            }
        }
    }
    lua_pushboolean(L, false);
    return 1;
}

/************************************************************************
 *  Function: removePartyEffect()
 *  Purpose : Deletes specified effect from all party members
 *  Example : player:removePartyEffect(EFFECT)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::removePartyEffect(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    CCharEntity* PChar = ((CCharEntity*)m_PBaseEntity);

    for (const auto& member : PChar->PParty->members)
    {
        if (member->loc.zone == PChar->loc.zone)
        {
            member->StatusEffectContainer->DelStatusEffect((EFFECT)lua_tointeger(L, 1));
        }
    }
    return 0;
}

/************************************************************************
 *  Function: getAlliance()
 *  Purpose : Returns a Lua table of all members of the alliance
 *  Example : local alliance = player:getAlliance()
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getAlliance(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

    int size = 1;

    if (PChar->PParty && PChar->PParty->m_PAlliance)
    {
        size = 0;
        for (auto* PParty : PChar->PParty->m_PAlliance->partyList)
        {
            size += PParty->MemberCount(m_PBaseEntity->getZone());
        }
    }
    else if (PChar->PParty)
    {
        size = PChar->PParty->MemberCount(m_PBaseEntity->getZone());
    }

    lua_createtable(L, size, 0);
    int i = 1;

    PChar->ForAlliance([&L, &i](CBattleEntity* PMember) {
        lua_getglobal(L, "CBaseEntity");
        lua_pushstring(L, "new");
        lua_gettable(L, -2);
        lua_insert(L, -2);
        lua_pushlightuserdata(L, (void*)PMember);
        lua_pcall(L, 2, 1, 0);

        lua_rawseti(L, -2, i++);
    });

    return 1;
}

/************************************************************************
 *  Function: getAllianceSize()
 *  Purpose : Returns the number of members in the alliance
 *  Example : local count = player:getAllianceSize()
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getAllianceSize(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    uint8 alliancesize = 1;

    if (((CBattleEntity*)m_PBaseEntity)->PParty != nullptr)
    {
        if (((CBattleEntity*)m_PBaseEntity)->PParty->m_PAlliance != nullptr)
        {
            alliancesize = (uint8)((CBattleEntity*)m_PBaseEntity)->PParty->m_PAlliance->partyList.size();
        }
    }
    lua_pushnumber(L, alliancesize);
    return 1;
}

/************************************************************************
 *  Function: getLeaderID()
 *  Purpose : Returns the fallback id of the Alliance/Party or player
 *  Example : local leaderid = player:getLeaderID()
 *  Notes   : if the player is in an alliance, returns alliance ID
 *  Notes   : if the player is in a party and no alliance, returns party ID
 *  Notes   : if the player is not in a party, returns player ID
 ************************************************************************/

inline int32 CLuaBaseEntity::getLeaderID(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    if (const CCharEntity* PChar = dynamic_cast<CCharEntity*>(m_PBaseEntity))
    {
        if (PChar->PParty != nullptr)
        {
            if (PChar->PParty->m_PAlliance != nullptr)
            {
                lua_pushnumber(L, PChar->PParty->m_PAlliance->m_AllianceID);
                return 1;
            }
            lua_pushnumber(L, PChar->PParty->GetPartyID());
            return 1;
        }
        lua_pushnumber(L, PChar->id);
        return 1;
    }
    lua_pushnil(L);
    return 1;
}

/************************************************************************
 *  Function: getPartyLastMemberJoinedTime()
 *  Purpose : Get the epoch time point in seconds that the last PC joined the party (if any)
 *  Example : seconds_since_last_member_joined = os.time() - player:getPartyLastMemberJoinedTime()
 *  Notes   :
 ************************************************************************/

int32 CLuaBaseEntity::getPartyLastMemberJoinedTime(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    CCharEntity* PChar = ((CCharEntity*)m_PBaseEntity);

    if (PChar->PParty != nullptr)
    {
        lua_pushnumber(L, PChar->PParty->GetTimeLastMemberJoined());
        return 1;
    }

    lua_pushnumber(L, 0);
    return 1;
}

/************************************************************************
 *  Function: reloadParty()
 *  Purpose : Display a new party in the event of alliance form/disband
 *  Example : Creates/Destroys the other parties being displayed
 *  Notes   : Only a function of the core at the moment - future plans?
 ************************************************************************/

inline int32 CLuaBaseEntity::reloadParty(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);
    ((CCharEntity*)m_PBaseEntity)->ReloadPartyInc();

    return 0;
}

/************************************************************************
 *  Function: disableLevelSync()
 *  Purpose : Disables...wait for it...Level Sync
 *  Example : target:disableLevelSync()
 *  Notes   : Only used in scripts/globals/effects/level_sync.lua
 ************************************************************************/

inline int32 CLuaBaseEntity::disableLevelSync(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;
    if (PChar->PParty)
    {
        if (PChar->PParty->GetSyncTarget() == PChar)
        {
            PChar->PParty->SetSyncTarget(nullptr, 553);
        }
        else
        {
            PChar->PParty->DisableSync();
        }
    }

    PChar->loc.zone->PushPacket(PChar, CHAR_INRANGE, new CCharSyncPacket(PChar));
    return 0;
}

/************************************************************************
 *  Function: isLevelSync()
 *  Purpose :
 *  Example : player:isLevelSync()
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::isLevelSync(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

    if (PChar->PParty)
    {
        lua_pushboolean(L, (PChar->PParty->GetSyncTarget() && PChar->PParty->GetSyncTarget() != PChar));
    }
    else
    {
        lua_pushboolean(L, false);
    }

    return 1;
}

/************************************************************************
 *  Function: checkSoloPartyAlliance()
 *  Purpose : Checks if Entity is solo, in a party, or in an alliance
 *  Example : local popularityCheck = player:checkSoloPartyAlliance()
 *  Notes   : Returns 0 (Solo), 1 (Party), or 2 (Alliance)
 ************************************************************************/

inline int32 CLuaBaseEntity::checkSoloPartyAlliance(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

    uint8 SoloPartyAlliance = 0;
    if (PChar->PParty != nullptr)
    {
        SoloPartyAlliance = 1;
        if (PChar->PParty->m_PAlliance != nullptr)
        {
            SoloPartyAlliance = 2;
        }
    }

    lua_pushinteger(L, SoloPartyAlliance);
    return 1;
}

/************************************************************************
 *  Function: checkKillCredit()
 *  Purpose : Used to determine if kill counts towards regimes/etc.
 *  Example : if (player:checkKillCredit(mob)) then
 *          : optionally specify lv diff and distance e.g.
 *          : player:checkKillCredit(mob, 15, 100)
 *  Notes   : Returns true if player is in range of sync target upon kill
 *  Notes   : and the mob is able to give exp to the members
 ************************************************************************/

inline int32 CLuaBaseEntity::checkKillCredit(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isuserdata(L, 1));

    CLuaBaseEntity* PLuaBaseEntity = nullptr;

    TPZ_DEBUG_BREAK_IF(PLuaBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(PLuaBaseEntity->GetBaseEntity()->objtype != TYPE_MOB);

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;
    CMobEntity*  PMob  = (CMobEntity*)PLuaBaseEntity->GetBaseEntity();

    bool  credit  = false;
    int   lvlDiff = (int)(PMob->GetMLevel()) - (int)(PChar->GetMLevel());
    int   maxDiff = (!lua_isnil(L, 2) && lua_isnumber(L, 2)) ? (int)lua_tonumber(L, 2) : 15;
    float range   = (!lua_isnil(L, 3) && lua_isnumber(L, 3)) ? (float)lua_tonumber(L, 3) : 100;

    if (charutils::CheckMob(PMob->m_HiPCLvl, PMob->GetMLevel()) > EMobDifficulty::TooWeak && distance(PMob->loc.p, PChar->loc.p) < range && lvlDiff < maxDiff)
    {
        if (PChar->PParty && PChar->PParty->GetSyncTarget())
        {
            if (distance(PMob->loc.p, PChar->PParty->GetSyncTarget()->loc.p) < range && PChar->PParty->GetSyncTarget()->health.hp)
            {
                credit = true;
            }
        }
        else
        {
            credit = true;
        }
    }

    lua_pushboolean(L, credit);
    return 1;
}

/************************************************************************
 *  Function: getInstance()
 *  Purpose : Get the instance object that the Entity is part of
 *  Example : local instance = door:getInstance()
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getInstance(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    if (m_PBaseEntity->PInstance)
    {
        lua_getglobal(L, "CInstance");
        lua_pushstring(L, "new");
        lua_gettable(L, -2);
        lua_insert(L, -2);
        lua_pushlightuserdata(L, (void*)m_PBaseEntity->PInstance);
        lua_pcall(L, 2, 1, 0);
    }
    else
    {
        lua_pushnil(L);
    }
    return 1;
}

/************************************************************************
 *  Function: setInstance()
 *  Purpose : Registers a character for an established instance
 *  Example : player:setInstance()
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::setInstance(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isuserdata(L, 1));

    CLuaInstance* PLuaInstance = nullptr;
    CInstance*    PInstance    = PLuaInstance->GetInstance();
    m_PBaseEntity->PInstance   = PInstance;

    if (PInstance)
    {
        PInstance->RegisterChar(dynamic_cast<CCharEntity*>(m_PBaseEntity));
    }

    return 0;
}

/************************************************************************
 *  Function: createInstance()
 *  Purpose : Creates a new instance for a PC
 *  Example : player:createInstance(player:getCurrentAssault(), 63)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::createInstance(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));

    uint8  instanceid = (uint8)lua_tonumber(L, 1);
    uint16 zoneid     = (uint16)lua_tonumber(L, 2);

    instanceutils::LoadInstance(instanceid, zoneid, (CCharEntity*)m_PBaseEntity);

    return 0;
}

/************************************************************************
 *  Function: instanceEntry()
 *  Purpose : Creates an instance entry packet for the player
 *  Example : player:instanceEntry(target,1)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::instanceEntry(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isuserdata(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    CLuaBaseEntity* PLuaBaseEntity = nullptr;
    CBaseEntity*    PTarget        = PLuaBaseEntity->m_PBaseEntity;

    auto response = (uint32)lua_tointeger(L, 2);

    ((CCharEntity*)m_PBaseEntity)->pushPacket(new CInstanceEntryPacket(PTarget, response));

    return 0;
}

/************************************************************************
 *  Function: getConfrontationEffect()
 *  Purpose : None yet
 *  Example :
 *  Notes   : Not moved to scripts
 ************************************************************************/

inline int32 CLuaBaseEntity::getConfrontationEffect(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    lua_pushinteger(L, ((CBattleEntity*)m_PBaseEntity)->StatusEffectContainer->GetConfrontationEffect());
    return 1;
}

/************************************************************************
 *  Function: copyConfrontationEffect()
 *  Purpose : None yet
 *  Example :
 *  Notes   : Not moved to scripts
 ************************************************************************/

inline int32 CLuaBaseEntity::copyConfrontationEffect(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    CBattleEntity* PEntity = (CBattleEntity*)m_PBaseEntity->GetEntity((uint16)lua_tointeger(L, 1), TYPE_PC | TYPE_MOB);

    uint16 power = 0;

    if (PEntity && PEntity->StatusEffectContainer->GetConfrontationEffect())
    {
        ((CBattleEntity*)m_PBaseEntity)->StatusEffectContainer->CopyConfrontationEffect(PEntity);
        power = PEntity->StatusEffectContainer->GetConfrontationEffect();
    }

    lua_pushinteger(L, power);
    return 1;
}

/************************************************************************
 *  Function: getBattlefield()
 *  Purpose : Returns a Battlefield Instance Object to the entity
 *  Example : local battlefield = player:getBattlefield();
 *  Notes   : Used to check if entity is inside a battlefield
 ************************************************************************/

inline int32 CLuaBaseEntity::getBattlefield(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    auto* PBattlefield = m_PBaseEntity->PBattlefield;

    if (PBattlefield)
    {
        lua_getglobal(L, "CBattlefield");
        lua_pushstring(L, "new");
        lua_gettable(L, -2);
        lua_insert(L, -2);
        lua_pushlightuserdata(L, (void*)PBattlefield);
        lua_pcall(L, 2, 1, 0);
        return 1;
    }
    else
    {
        lua_pushnil(L);
    }
    return 1;
}

/************************************************************************
 *  Function: getBattlefieldID()
 *  Purpose : Returns the integer ID for the battlefield, -1 if not found
 *  Example : local battlefieldId = player:getBattlefieldID()
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getBattlefieldID(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    lua_pushinteger(L, m_PBaseEntity->PBattlefield ? m_PBaseEntity->PBattlefield->GetID() : -1);
    return 1;
}

/************************************************************************
 *  Function: registerBattlefield()
 *  Purpose : Attempts to register an existing battlefield or create a new one.
 *  Example : local returnCode = player:registerBattlefield(id, area, initiatorId)
 *  Notes   : Returns BATTLEFIELD_RETURNCODE (see scripts/globals/battlefield.lua or src/map/battlefield.h)
 ************************************************************************/

inline int32 CLuaBaseEntity::registerBattlefield(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr || m_PBaseEntity->loc.zone->m_BattlefieldHandler == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);
    auto* PZone = PChar->loc.zone == nullptr ? zoneutils::GetZone(PChar->loc.destination) : PChar->loc.zone;

    int    battlefield = -1;
    uint8  area        = 0;
    uint32 initiator   = 0;

    battlefield = !lua_isnil(L, 1) ? (int)lua_tointeger(L, 1) : -1;
    area        = !lua_isnil(L, 2) ? (uint8)lua_tointeger(L, 2) : 1;
    initiator   = !lua_isnil(L, 3) ? (uint32)lua_tointeger(L, 3) : 0;

    lua_pushinteger(L, PZone->m_BattlefieldHandler->RegisterBattlefield(PChar, (uint16)battlefield, area, initiator));
    return 1;
}

inline int32 CLuaBaseEntity::battlefieldAtCapacity(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr || m_PBaseEntity->loc.zone->m_BattlefieldHandler == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);
    auto* PZone = PChar->loc.zone == nullptr ? zoneutils::GetZone(PChar->loc.destination) : PChar->loc.zone;

    TPZ_DEBUG_BREAK_IF(PZone->m_BattlefieldHandler == nullptr);

    bool full = false;

    if (PZone != nullptr && PZone->m_BattlefieldHandler != nullptr && PZone->m_BattlefieldHandler->ReachedMaxCapacity((int)lua_tointeger(L, 1)))
    {
        full = true;
    }
    lua_pushboolean(L, full);
    return 1;
}

/************************************************************************
 *  Function: enterBattlefield(area)
 *  Purpose : Places an entity into a battlefield they are registered for (or tries enter a specific area if not full)
 *  Example : player:enterBattlefield(area)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::enterBattlefield(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr || m_PBaseEntity->loc.zone->m_BattlefieldHandler == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    CBattlefield* PBattlefield = nullptr;
    if (lua_isnil(L, 1) || !lua_isnumber(L, 1))
    {
        PBattlefield = m_PBaseEntity->loc.zone->m_BattlefieldHandler->GetBattlefield(m_PBaseEntity, true);
    }
    else
    {
        PBattlefield = m_PBaseEntity->loc.zone->m_BattlefieldHandler->GetBattlefieldByArea((uint8)lua_tointeger(L, 1));
    }

    lua_pushboolean(L, PBattlefield ? PBattlefield->InsertEntity(m_PBaseEntity, true) : false);
    return 1;
}

/************************************************************************
 *  Function: leaveBattlefield()
 *  Purpose : Removes an entity from battlefield and removes battlefield status
 *  Example : player:leaveBattlefield(leaveCode)
 *  Notes   : leaveCode can be found in scripts/globals/battlefield.lua or src/map/battlefield.h
 ************************************************************************/

inline int32 CLuaBaseEntity::leaveBattlefield(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr || m_PBaseEntity->loc.zone->m_BattlefieldHandler == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    auto leavecode = (uint8)lua_tointeger(L, 1);

    lua_pushinteger(L, m_PBaseEntity->loc.zone->m_BattlefieldHandler->RemoveFromBattlefield(m_PBaseEntity, m_PBaseEntity->PBattlefield, leavecode));
    return 1;
}

/************************************************************************
 *  Function: isInDynamis()
 *  Purpose : Returns true if an entity is in Dynamis
 *  Example : if (player:isInDynamis()) then
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::isInDynamis(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    lua_pushboolean(L, ((CBattleEntity*)m_PBaseEntity)->isInDynamis());
    return 1;
}

/************************************************************************
 *  Function: isAlive()
 *  Purpose : Returns true if an Entity is alive
 *  Example : if (mob:isAlive())
 *  Notes   :
 ************************************************************************/

bool CLuaBaseEntity::isAlive()
{
    return static_cast<CBattleEntity*>(m_PBaseEntity)->isAlive();
}

/************************************************************************
 *  Function: isDead()
 *  Purpose : Returns true if an Entity is dead
 *  Example : if (pet:isDead())
 *  Notes   :
 ************************************************************************/

bool CLuaBaseEntity::isDead()
{
    return static_cast<CBattleEntity*>(m_PBaseEntity)->isDead();
}

/************************************************************************
 *  Function: sendRaise()
 *  Purpose : Updates the m_hasRaise private member with the Raise Level
 *  Example : target:sendRaise(1) -- 2, or 3 for R2, R3
 *  Notes   : Sending the Raise menu is handled by CDeathState::Update
 ************************************************************************/

void CLuaBaseEntity::sendRaise(uint8 raiseLevel)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);

    if (raiseLevel == 0 || raiseLevel > 3)
    {
        ShowDebug(CL_CYAN "lua::sendRaise raise value is not valid!\n" CL_RESET);
    }
    else if (PChar->m_hasTractor == 0 && PChar->m_hasRaise == 0)
    {
        PChar->m_hasRaise = raiseLevel;
        PChar->pushPacket(new CRaiseTractorMenuPacket(PChar, TYPE_RAISE));
    }
}

/************************************************************************
 *  Function: sendReraise()
 *  Purpose : Updates the m_hasRaise private member with the Reraise Level
 *  Example : target:sendReraise(effect:getPower())
 *  Notes   : Sending the Reraise menu is handled by CDeathState::Update
 ************************************************************************/

void CLuaBaseEntity::sendReraise(uint8 raiseLevel)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);

    if (raiseLevel == 0 || raiseLevel > 3)
    {
        ShowDebug(CL_CYAN "lua::sendRaise raise value is not valide!\n" CL_RESET);
    }
    else if (PChar->m_hasRaise == 0)
    {
        PChar->m_hasRaise = raiseLevel;
    }
}

/************************************************************************
 *  Function: sendTractor()
 *  Purpose : Sends a Tractor request to a PC
 *  Example : target:sendTractor(caster:getXPos(), caster:getYPos(), caster:getZPos()
 *  Notes   : Sets position to Tractor to, Tractor available flag, and shows menu to PC
 ************************************************************************/

void CLuaBaseEntity::sendTractor(float xPos, float yPos, float zPos, uint8 rotation)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar = static_cast<CCharEntity*>(m_PBaseEntity);

    if (PChar->m_hasTractor == 0 && PChar->m_hasRaise == 0)
    {
        PChar->m_hasTractor = 1;

        PChar->m_StartActionPos.x        = xPos;
        PChar->m_StartActionPos.y        = yPos;
        PChar->m_StartActionPos.z        = zPos;
        PChar->m_StartActionPos.rotation = rotation;

        PChar->pushPacket(new CRaiseTractorMenuPacket(PChar, TYPE_TRACTOR));
    }
}

/************************************************************************
 *  Function: countdown()
 *  Purpose : Starts or clears a visible countdown bar for player
 *  Example : player:countdown(60)
 *  Notes   : Using 0 or no argument removes the countdown bar from the player
 ************************************************************************/
int32 CLuaBaseEntity::countdown(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto         seconds = static_cast<uint32>(lua_tonumber(L, 1));
    CCharEntity* PChar   = (CCharEntity*)m_PBaseEntity;
    auto*        packet  = new CTimerBarUtilPacket();

    if (seconds)
    {
        packet->addCountdown(seconds);
    }

    if (lua_isstring(L, 2) && lua_isnumber(L, 3))
    {
        packet->addBar1(lua_tostring(L, 2), (uint8)lua_tonumber(L, 3));
    }

    if (lua_isstring(L, 4) && lua_isnumber(L, 5))
    {
        packet->addBar2(lua_tostring(L, 4), (uint8)lua_tonumber(L, 5));
    }

    PChar->pushPacket(packet);

    return 0;
}

/************************************************************************
 *  Function: enableEntities()
 *  Purpose : Enables/disables the list of given special hidden entities for just the target char
 *  Example : player:enableEntities({ 17207972, 17207973})
 *  Notes   : Default is all off, so sending the ID enables the special entity
 ************************************************************************/
int32 CLuaBaseEntity::enableEntities(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    CCharEntity*        PChar = (CCharEntity*)m_PBaseEntity;
    std::vector<uint32> data;

    if (lua_istable(L, 1))
    {
        lua_pushnil(L);
        while (lua_next(L, -2))
        {
            lua_pushvalue(L, -2);
            auto value = (uint32)lua_tonumber(L, -2);
            data.push_back(value);
            lua_pop(L, 2);
        }
    }

    PChar->pushPacket(new CEntityEnableList(data));

    return 0;
}

/************************************************************************
 *  Function: independantAnimation()
 *  Purpose : Play an animation independant of action messages
 *  Example : player:independantAnimation(player, 251, 4) -- Plays little hearts
 *  Notes   : Accepts a target, but works perfectly fine on self
 ************************************************************************/
int32 CLuaBaseEntity::independantAnimation(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

    CBaseEntity* PTarget;
    if (!lua_isnil(L, 1) && lua_isuserdata(L, 1))
    {
        CLuaBaseEntity* PLuaBaseEntity = nullptr;
        PTarget                        = PLuaBaseEntity->m_PBaseEntity;
    }
    else
    {
        PTarget = m_PBaseEntity;
    }

    auto animId = (uint16)lua_tointeger(L, 2);
    auto mode   = (uint8)lua_tointeger(L, 3);

    PChar->pushPacket(new CIndependantAnimationPacket(PChar, PTarget, animId, mode));

    return 0;
}

/************************************************************************
 *  Function: engage()
 *  Purpose : Instructs a Battle Entity to engage in combat
 *  Example : pet:engage(target)
 *  Notes   :
 ************************************************************************/

int32 CLuaBaseEntity::engage(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    CBattleEntity* battleEntity    = static_cast<CBattleEntity*>(m_PBaseEntity);
    auto           requestedTarget = (uint16)lua_tointeger(L, 1);

    if (requestedTarget > 0)
    {
        battleEntity->PAI->Engage(requestedTarget);
    }

    return 0;
}

/************************************************************************
 *  Function: isEngaged()
 *  Purpose : Returns true if an Entity is engaged in battle
 *  Example : if (mob:isEngaged()) then
 *  Notes   :
 ************************************************************************/

int32 CLuaBaseEntity::isEngaged(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    lua_pushboolean(L, m_PBaseEntity->PAI->IsEngaged());
    return 1;
}

/************************************************************************
 *  Function: disengage()
 *  Purpose : Instructs the Battle Entity to disengage from combat
 *  Example : mob:disengage()
 *  Notes   : Used for Mobs and Pets
 ************************************************************************/

int32 CLuaBaseEntity::disengage(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    CBattleEntity* battleEntity = static_cast<CBattleEntity*>(m_PBaseEntity);
    battleEntity->PAI->Disengage();

    return 0;
}

/************************************************************************
 *  Function: timer()
 *  Purpose : Inserts a pre-defined Lua fuction into the queue and executes
 *          : once a defined amount of time has expired (pauses Lua execution)
 *  Example : npc:timer(respawnTime * 1000, function(npc)
 *  Notes   : See scripts/zones/Nyzul_Isle/mobs/Raubahn.lua
 ************************************************************************/

int32 CLuaBaseEntity::timer(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isfunction(L, 2));

    auto ms = (int)lua_tointeger(L, 1);

    m_PBaseEntity->PAI->QueueAction(queueAction_t(ms, false, luautils::register_fp(2)));

    return 0;
}

/************************************************************************
 *  Function: queue()
 *  Purpose : Queues a Lua function
 *  Example :
 *  Notes   : For instance, Sic can be used before a Pet reaches 100% TP.
 *          : Once the Pet reaches 100%, it will use it's Special Ability.
 *          : See scripts/globals/abilities/sic.lua for how the Special
 *          : Ability is delayed until 100% (essentially loops into Action Queue)
 ************************************************************************/

int32 CLuaBaseEntity::queue(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isfunction(L, 2));

    auto ms = (int)lua_tointeger(L, 1);

    m_PBaseEntity->PAI->QueueAction(queueAction_t(ms, true, luautils::register_fp(2)));

    return 0;
}

/************************************************************************
 *  Function: addRecast()
 *  Purpose : Manually adds a cooldown for a particular Ability
 *  Example : automaton:addRecast(RECAST_ABILITY, skill:getID(), 180)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::addRecast(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 3) || !lua_isnumber(L, 3));

    CBattleEntity* PBattleEntity = dynamic_cast<CBattleEntity*>(m_PBaseEntity);
    if (PBattleEntity)
    {
        RECASTTYPE recastContainer = (RECASTTYPE)lua_tointeger(L, 1);
        auto       recastID        = (uint16)lua_tointeger(L, 2);
        auto       duration        = (uint32)lua_tointeger(L, 3);

        PBattleEntity->PRecastContainer->Add(recastContainer, recastID, duration);
        if (PBattleEntity->objtype == TYPE_PC)
        {
            CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;
            PChar->pushPacket(new CCharSkillsPacket(PChar));
            PChar->pushPacket(new CCharRecastPacket(PChar));
        }
    }
    return 0;
}

/************************************************************************
 *  Function: hasRecast()
 *  Purpose : Checks to see if a particular Ability is on cooldown
 *  Example : automaton:hasRecast(RECAST_ABILITY, skill:getID(), recast)
 *  Notes   : Recast parameter is optional to check charges
 ************************************************************************/

inline int32 CLuaBaseEntity::hasRecast(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));

    bool           hasRecast     = false;
    CBattleEntity* PBattleEntity = dynamic_cast<CBattleEntity*>(m_PBaseEntity);
    if (PBattleEntity)
    {
        RECASTTYPE recastContainer = (RECASTTYPE)lua_tointeger(L, 1);
        auto       recastID        = (uint16)lua_tointeger(L, 2);
        uint32     recast          = 0;
        if (!lua_isnil(L, 3) && lua_isnumber(L, 3))
        {
            recast = (uint32)lua_tointeger(L, 3);
        }

        hasRecast = PBattleEntity->PRecastContainer->HasRecast(recastContainer, recastID, recast);
    }
    lua_pushboolean(L, hasRecast);
    return 1;
}

/************************************************************************
 *  Function: resetRecast()
 *  Purpose : Resets the cooldown for a specified Ability to 0
 *  Example : player:resetRecast(RECAST_ABILITY, 231)
 *  Notes   : Must call the particular container (Ability Container in above example)
 *          : I imagine the Magic container can be specified?
 ************************************************************************/

inline int32 CLuaBaseEntity::resetRecast(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));

    // only reset for players
    if (m_PBaseEntity->objtype == TYPE_PC)
    {
        CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

        RECASTTYPE recastContainer = (RECASTTYPE)lua_tointeger(L, 1);
        auto       recastID        = (uint16)lua_tointeger(L, 2);

        if (PChar->PRecastContainer->Has(recastContainer, recastID))
        {
            PChar->PRecastContainer->Del(recastContainer, recastID);
            PChar->PRecastContainer->Add(recastContainer, recastID, 0);
        }

        PChar->pushPacket(new CCharSkillsPacket(PChar));
        PChar->pushPacket(new CCharRecastPacket(PChar));
        return 0;
    }

    return 0;
}

/************************************************************************
 *  Function: resetRecasts()
 *  Purpose : Resets all Ability cooldowns to 0 for an Entity
 *  Example : target:resetRecasts()
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::resetRecasts(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    // only reset for players
    if (m_PBaseEntity->objtype == TYPE_PC)
    {
        CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

        PChar->PRecastContainer->Del(RECAST_MAGIC);
        PChar->PRecastContainer->Del(RECAST_ABILITY);
        PChar->pushPacket(new CCharSkillsPacket(PChar));
        PChar->pushPacket(new CCharRecastPacket(PChar));
        return 0;
    }

    return 0;
}

/************************************************************************
 *  Function: addListener()
 *  Purpose : Instructs the Event Handler to monitor for an Event, then
 *            execute a prepared Lua function once the Event has occurred
 *  Example : See: scripts/mixins/families/chigoe.lua
 *  Notes   : Function along with statements must be passed in L3
 ************************************************************************/

int32 CLuaBaseEntity::addListener(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isstring(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isstring(L, 2));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 3) || !lua_isfunction(L, 3));

    const auto* eventName  = lua_tostring(L, 1);
    const auto* identifier = lua_tostring(L, 2);

    m_PBaseEntity->PAI->EventHandler.addListener(eventName, luautils::register_fp(3), identifier);

    return 0;
}

/************************************************************************
 *  Function: removeListener()
 *  Purpose : Instructs the Event Handler to stop monitoring for an Event
 *  Example : pet:removeListener("AUTO_PATTERN_READER_TICK")
 *  Notes   : Used heavily in Pup Ability scripts
 ************************************************************************/

int32 CLuaBaseEntity::removeListener(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isstring(L, 1));

    const auto* identifier = lua_tostring(L, 1);

    m_PBaseEntity->PAI->EventHandler.removeListener(identifier);

    return 0;
}

/************************************************************************
 *  Function: triggerListener()
 *  Purpose : Instructs Lua to execute an Event Function once a Trigger has
 *            been identified by the Event Handler
 *  Example : mob:triggerListener("AERN_RERAISE", mob, curr_reraise + 1)
 *  Notes   : Manually triggered through Aern scripts for some reason
 ************************************************************************/

int32 CLuaBaseEntity::triggerListener(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isstring(L, 1));

    const auto* eventName = lua_tostring(L, 1);
    auto        top       = lua_gettop(L);

    m_PBaseEntity->PAI->EventHandler.triggerListener(eventName, top - 1);

    return 0;
}

/************************************************************************
 *  Function: getEntity()
 *  Purpose : Returns the object of the Entity targeted
 *  Example : local new_target = mob:getEntity(targetid)
 *  Notes   : Currently used in Assault Missions and some Mobs
 ************************************************************************/

int32 CLuaBaseEntity::getEntity(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    auto* PEntity{ m_PBaseEntity->GetEntity((uint16)lua_tointeger(L, 1)) };
    if (PEntity)
    {
        lua_getglobal(L, "CBaseEntity");
        lua_pushstring(L, "new");
        lua_gettable(L, -2);
        lua_insert(L, -2);
        lua_pushlightuserdata(L, (void*)PEntity);
        lua_pcall(L, 2, 1, 0);
    }
    else
    {
        lua_pushnil(L);
    }
    return 1;
}

/************************************************************************
 *  Function: getNearbyEntities()
 *  Purpose : Returns a Lua table of all Entities surrounding target Entity?
 *  Example : mob:getNearbyEntities(target); player:getNearbyEntities(target)
 *  Notes   : Currently unused in any script
 ************************************************************************/

inline int32 CLuaBaseEntity::getNearbyEntities(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    CCharEntity* iterTarget = (CCharEntity*)(m_PBaseEntity->objtype == TYPE_PC ? m_PBaseEntity : nullptr);

    if (!iterTarget)
    {
        if (m_PBaseEntity->objtype == TYPE_MOB && ((CBattleEntity*)m_PBaseEntity)->GetBattleTarget()->objtype == TYPE_PC)
        {
            iterTarget = (CCharEntity*)((CBattleEntity*)m_PBaseEntity)->GetBattleTarget();
        }
        else if (((CBattleEntity*)m_PBaseEntity)->PMaster && ((CBattleEntity*)m_PBaseEntity)->PMaster->objtype == TYPE_PC)
        {
            iterTarget = (CCharEntity*)((CBattleEntity*)m_PBaseEntity)->PMaster;
        }
    }

    lua_newtable(L);
    int newTable = lua_gettop(L);

    for (auto&& list : { iterTarget->SpawnMOBList, iterTarget->SpawnPCList, iterTarget->SpawnPETList })
    {
        for (auto&& entity : list)
        {
            lua_getglobal(L, "CBaseEntity");
            lua_pushstring(L, "new");
            lua_gettable(L, -2);
            lua_insert(L, -2);
            lua_pushlightuserdata(L, (void*)entity.second);
            lua_pcall(L, 2, 1, 0);
            lua_rawseti(L, newTable, entity.first);
        }
    }

    return 1;
}

/************************************************************************
 *  Function: canChangeState()
 *  Purpose : Returns true if a mob isn't even in it's final form, bro
 *  Example : if mob:canChangeState() then
 *  Notes   : Only used in scripts/mixins/abyssea_nm.lua currently
 ************************************************************************/

int32 CLuaBaseEntity::canChangeState(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    lua_pushboolean(L, m_PBaseEntity->PAI->CanChangeState());
    return 1;
}

/************************************************************************
 *  Function: wakeUp()
 *  Purpose : Removes any Sleep Effect from the Entity's Status Effect Container
 *  Example : target:wakeUp()
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::wakeUp(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    CBattleEntity* PEntity = (CBattleEntity*)m_PBaseEntity;

    // is asleep is not working!
    // if(PEntity->isAsleep())
    // {
    // wake them up!
    PEntity->StatusEffectContainer->DelStatusEffect(EFFECT_SLEEP);
    PEntity->StatusEffectContainer->DelStatusEffect(EFFECT_SLEEP_II);
    PEntity->StatusEffectContainer->DelStatusEffect(EFFECT_LULLABY);
    // }
    return 0;
}

/************************************************************************
 *  Function: recalculateStats()
 *  Purpose : Recalculate the total Stats for a PC (force update)
 *  Example : target:recalculateStats()
 *  Notes   : See scripts/globals/effects/obliviscence.lua
 ************************************************************************/

int32 CLuaBaseEntity::recalculateStats(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    if (m_PBaseEntity->objtype == TYPE_PC)
    {
        auto* PChar{ static_cast<CCharEntity*>(m_PBaseEntity) };
        charutils::BuildingCharSkillsTable(PChar);
        charutils::CalculateStats(PChar);
        charutils::CheckValidEquipment(PChar);
        charutils::BuildingCharAbilityTable(PChar);
        charutils::BuildingCharTraitsTable(PChar);

        PChar->UpdateHealth();

        PChar->pushPacket(new CCharJobsPacket(PChar));
        PChar->pushPacket(new CCharStatsPacket(PChar));
        PChar->pushPacket(new CCharSkillsPacket(PChar));
        PChar->pushPacket(new CCharRecastPacket(PChar));
        PChar->pushPacket(new CCharAbilitiesPacket(PChar));
        PChar->pushPacket(new CCharUpdatePacket(PChar));
        PChar->pushPacket(new CMenuMeritPacket(PChar));
        PChar->pushPacket(new CCharSyncPacket(PChar));
    }
    return 0;
}

/************************************************************************
 *  Function: checkImbuedItems()
 *  Purpose : Returns true if an Imbued Item is found in a PC's inventory
 *  Example : if (v:checkImbuedItems()) then
 *  Notes   : See scripts/zones/Alzadaal_Undersea_Ruins/npcs/_20t.lua
 ************************************************************************/

int32 CLuaBaseEntity::checkImbuedItems(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* PChar{ static_cast<CCharEntity*>(m_PBaseEntity) };

    for (uint8 LocID = 0; LocID < MAX_CONTAINER_ID; ++LocID)
    {
        bool found = false;
        PChar->getStorage(LocID)->ForEachItem([&found](CItem* PItem) {
            if (PItem->getID() >= 5365 && PItem->getID() <= 5384)
            {
                found = true;
            }
        });
        if (found)
        {
            lua_pushboolean(L, true);
            return 1;
        }
    }
    lua_pushboolean(L, false);
    return 1;
}

/************************************************************************
 *  Function: isDualWielding()
 *  Purpose : Returns true if entity is wielding two weapons
 *  Example : if player:isDualWielding() then
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::isDualWielding(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    CBattleEntity* PBattleEntity = dynamic_cast<CBattleEntity*>(m_PBaseEntity);
    if (PBattleEntity)
    {
        lua_pushboolean(L, PBattleEntity->m_dualWield);
        return 1;
    }
    else
    {
        lua_pushboolean(L, false);
        ShowError("lua::isDualWielding :: NPCs don't wield weapons!\n");
        return 1;
    }
}

/************************************************************************
 *  Function: getCE()
 *  Purpose : Returns the current Cumulative Enmity a Mob has against an Entity
 *  Example : local playerCE = target:getCE(player)
 *  Notes   : See Ventriloquy and Atonement
 ************************************************************************/

inline int32 CLuaBaseEntity::getCE(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_MOB);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isuserdata(L, 1));

    CLuaBaseEntity* PEntity = nullptr;

    lua_pushinteger(L, ((CMobEntity*)m_PBaseEntity)->PEnmityContainer->GetCE((CBattleEntity*)PEntity->GetBaseEntity()));
    return 1;
}

/************************************************************************
 *  Function: getVE()
 *  Purpose : Returns the current Volatile Enmity a Mob has against an Entity
 *  Example : local playerVE = target:getVE(player)
 *  Notes   : See Ventriloquy and Atonement
 ************************************************************************/

inline int32 CLuaBaseEntity::getVE(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_MOB);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isuserdata(L, 1));

    CLuaBaseEntity* PEntity = nullptr;

    lua_pushinteger(L, ((CMobEntity*)m_PBaseEntity)->PEnmityContainer->GetVE((CBattleEntity*)PEntity->GetBaseEntity()));
    return 1;
}

/************************************************************************
 *  Function: setCE()
 *  Purpose : Sets a specified amount of Cumulative Enmity against an Entity
 *  Example : target:setCE(player, petCE * petEnmityBonus)
 *  Notes   : Currently only used in scripts/globals/abilities/ventriloquy.lua
 ************************************************************************/

inline int32 CLuaBaseEntity::setCE(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_MOB);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isuserdata(L, 1));

    CLuaBaseEntity* PEntity = nullptr;
    auto            amount  = (uint16)lua_tointeger(L, 2);

    ((CMobEntity*)m_PBaseEntity)->PEnmityContainer->SetCE((CBattleEntity*)PEntity->GetBaseEntity(), amount);
    return 0;
}

/************************************************************************
 *  Function: setVE()
 *  Purpose : Sets a specified amount of Volatile Enmity against an Entity
 *  Example : target:setVE(player, petVE * petEnmityBonus)
 *  Notes   : Currently only used in scripts/globals/abilities/ventriloquy.lua
 ************************************************************************/

inline int32 CLuaBaseEntity::setVE(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_MOB);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isuserdata(L, 1));

    CLuaBaseEntity* PEntity = nullptr;
    auto            amount  = (uint16)lua_tointeger(L, 2);

    ((CMobEntity*)m_PBaseEntity)->PEnmityContainer->SetVE((CBattleEntity*)PEntity->GetBaseEntity(), amount);
    return 0;
}

/************************************************************************
 *  Function: addEnmity()
 *  Purpose : Adds CE and VE Enmity to the Mobs Enmity table against that Entity
 *  Example : target:addEnmity(automaton, 450, 900)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::addEnmity(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 3) || !lua_isnumber(L, 3));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isuserdata(L, 1));

    CLuaBaseEntity* PEntity = nullptr;
    int32           CE      = (int32)lua_tointeger(L, 2);
    int32           VE      = (int32)lua_tointeger(L, 3);

    CMobEntity* PMob = static_cast<CMobEntity*>(m_PBaseEntity);

    if (m_PBaseEntity->objtype == TYPE_PC || m_PBaseEntity->objtype == TYPE_PET || (m_PBaseEntity->objtype == TYPE_MOB && PMob->isCharmed))
    {
        if (PEntity && PEntity->GetBaseEntity() && PEntity->GetBaseEntity()->objtype == TYPE_MOB)
        {
            static_cast<CMobEntity*>(PEntity->GetBaseEntity())->PEnmityContainer->UpdateEnmity(static_cast<CBattleEntity*>(m_PBaseEntity), CE, VE);
        }
    }
    else if (m_PBaseEntity->objtype == TYPE_MOB)
    {
        if (PEntity != nullptr && (CE > 0 || VE > 0) && PEntity->GetBaseEntity()->objtype != TYPE_NPC)
        {
            PMob->PEnmityContainer->UpdateEnmity(static_cast<CBattleEntity*>(PEntity->GetBaseEntity()), CE, VE);
        }
    }
    return 0;
}

/************************************************************************
 *  Function: lowerEnmity()
 *  Purpose : Lowers Enmity towards an Entity by a specified percent
 *  Example : mob:lowerEnmity(target, 45)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::lowerEnmity(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_MOB);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isuserdata(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));

    CLuaBaseEntity* PEntity = nullptr;

    if (PEntity != nullptr && PEntity->GetBaseEntity()->objtype != TYPE_NPC)
    {
        ((CMobEntity*)m_PBaseEntity)->PEnmityContainer->LowerEnmityByPercent((CBattleEntity*)PEntity->GetBaseEntity(), (uint8)lua_tonumber(L, 2), nullptr);
    }
    return 0;
}

/************************************************************************
 *  Function: updateEnmity()
 *  Purpose : Unlike updateClaim(), this function only generates Enmity toward an Entity
 *  Example : SpawnMob(17330334):updateEnmity(target)
 *  Notes   : Mob will aggro an Entity, but be unclaimed until engaged
 ************************************************************************/

inline int32 CLuaBaseEntity::updateEnmity(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_MOB);

    // TPZ_DEBUG_BREAK_IF(lua_gettop(L) > 1);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isuserdata(L, 1));

    CLuaBaseEntity* PEntity = nullptr;

    if (PEntity != nullptr && PEntity->GetBaseEntity()->objtype != TYPE_NPC)
    {
        ((CMobEntity*)m_PBaseEntity)->PEnmityContainer->AddBaseEnmity((CBattleEntity*)PEntity->GetBaseEntity());
    }
    return 0;
}

/************************************************************************
 *  Function: transferEnmity()
 *  Purpose : Used to transfer an amount of Enmity from one Entity to another
 *  Example : target:transferEnmity(player, 50, 20.6)
 *  Notes   : See scripts/globals/abilities/accomplice.lua
 ************************************************************************/

int32 CLuaBaseEntity::transferEnmity(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 3) || !lua_isnumber(L, 3));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));
    TPZ_DEBUG_BREAK_IF(lua_tointeger(L, 2) < 0);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isuserdata(L, 1));

    // TODO:
    CBaseEntity* PEntity = nullptr;
    auto         percent = (uint8)lua_tointeger(L, 2);
    auto         range   = lua_tonumber(L, 3);

    auto* PIterEntity = [&]() -> CCharEntity* {
        if (m_PBaseEntity->objtype == TYPE_PC)
        {
            return static_cast<CCharEntity*>(m_PBaseEntity);
        }
        else if (m_PBaseEntity->objtype == TYPE_PET)
        {
            auto* PMaster = static_cast<CPetEntity*>(m_PBaseEntity)->PMaster;
            if (PMaster->objtype == TYPE_PC)
            {
                return static_cast<CCharEntity*>(PMaster);
            }
        }
        else if (PEntity->objtype == TYPE_PC)
        {
            return static_cast<CCharEntity*>(PEntity);
        }
        else if (PEntity->objtype == TYPE_PET)
        {
            auto* PMaster = static_cast<CPetEntity*>(PEntity)->PMaster;
            if (PMaster->objtype == TYPE_PC)
            {
                return static_cast<CCharEntity*>(PMaster);
            }
        }
        return nullptr;
    }();

    if (PIterEntity)
    {
        for (auto&& mob_pair : PIterEntity->SpawnMOBList)
        {
            if (distanceSquared(mob_pair.second->loc.p, PEntity->loc.p) < (range * range))
            {
                battleutils::TransferEnmity(static_cast<CBattleEntity*>(PEntity), static_cast<CBattleEntity*>(m_PBaseEntity),
                                            static_cast<CMobEntity*>(mob_pair.second), percent);
            }
        }
    }
    return 0;
}

/************************************************************************
 *  Function: updateEnmityFromDamage()
 *  Purpose : Generates Enmity from moves that damage the Mob
 *  Example : target:updateEnmityFromDamage(player,damage)
 *  Notes   : Used in most Weaponskills and damaging abilities scripts
 ************************************************************************/

inline int32 CLuaBaseEntity::updateEnmityFromDamage(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isuserdata(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));

    CLuaBaseEntity* PEntity = nullptr;
    int32           damage  = (int32)lua_tointeger(L, 2);

    auto* PBaseMob = static_cast<CMobEntity*>(m_PBaseEntity);

    // This is a mob attacking a target and losing enmity from doing damage
    if (m_PBaseEntity->objtype == TYPE_PC || m_PBaseEntity->objtype == TYPE_PET || (m_PBaseEntity->objtype == TYPE_MOB && PBaseMob->isCharmed))
    {
        if (PEntity->GetBaseEntity() && PEntity->GetBaseEntity()->objtype == TYPE_MOB)
        {
            static_cast<CMobEntity*>(PEntity->GetBaseEntity())->PEnmityContainer->UpdateEnmityFromAttack(static_cast<CBattleEntity*>(m_PBaseEntity), damage);
        }
    }
    // This is a mob being attacked and gaining enmity on the attacker
    else if (m_PBaseEntity->objtype == TYPE_MOB)
    {
        if (PEntity->GetBaseEntity() && damage > 0 && PEntity->GetBaseEntity()->objtype != TYPE_NPC)
        {
            PBaseMob->PEnmityContainer->UpdateEnmityFromDamage(static_cast<CBattleEntity*>(PEntity->GetBaseEntity()), damage);
        }
    }
    return 0;
}

/************************************************************************
 *  Function: updateEnmityFromCure()
 *  Purpose : Generates Enmity from use of Cure spells/abilities
 *  Example : caster:updateEnmityFromCure(target,final)
 *  Notes   : Used in nearly all Cure scripts and abilities which heal
 ************************************************************************/

inline int32 CLuaBaseEntity::updateEnmityFromCure(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));
    TPZ_DEBUG_BREAK_IF(lua_tointeger(L, 2) < 0);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isuserdata(L, 1));

    CLuaBaseEntity* PEntity = nullptr;
    auto            amount  = (int32)lua_tointeger(L, 2);

    auto* PCurer = [&]() -> CBattleEntity* {
        if (m_PBaseEntity->objtype == TYPE_PC || m_PBaseEntity->objtype == TYPE_TRUST)
        {
            return static_cast<CBattleEntity*>(m_PBaseEntity);
        }
        else if (m_PBaseEntity->objtype == TYPE_PET && static_cast<CPetEntity*>(m_PBaseEntity)->getPetType() != PET_TYPE::AUTOMATON)
        {
            auto* PMaster = static_cast<CPetEntity*>(m_PBaseEntity)->PMaster;
            if (PMaster->objtype == TYPE_PC)
            {
                return static_cast<CCharEntity*>(PMaster);
            }
        }
        return nullptr;
    }();

    if (PEntity != nullptr && PCurer)
    {
        battleutils::GenerateCureEnmity(PCurer, static_cast<CBattleEntity*>(PEntity->GetBaseEntity()), amount);
    }

    return 0;
}

/************************************************************************
 *  Function: resetEnmity()
 *  Purpose : Used to clear all Enmity from the target
 *  Example : mob:resetEnmity(target)
 *  Notes   : Used in Mob special abilities which reset Enmity
 ************************************************************************/

inline int32 CLuaBaseEntity::resetEnmity(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_MOB);

    // TPZ_DEBUG_BREAK_IF(lua_gettop(L) > 1);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isuserdata(L, 1));

    CLuaBaseEntity* PEntity = nullptr;

    if (PEntity != nullptr && PEntity->GetBaseEntity()->objtype != TYPE_NPC)
    {
        ((CMobEntity*)m_PBaseEntity)->PEnmityContainer->LowerEnmityByPercent((CBattleEntity*)PEntity->GetBaseEntity(), 100, nullptr);
    }
    return 0;
}

/************************************************************************
 *  Function: updateClaim()
 *  Purpose : Marks a Mob as claimed once popped by a Player
 *  Example : SpawnMob(17428813):updateClaim(player)
 *  Notes   : Used mostly in QM (???) scripts
 ************************************************************************/

inline int32 CLuaBaseEntity::updateClaim(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == NULL);
    if (m_PBaseEntity->objtype != TYPE_MOB)
    {
        return 0;
    }

    if (lua_isnil(L, 1) || !lua_isuserdata(L, 1))
    {
        static_cast<CMobEntity*>(m_PBaseEntity)->m_OwnerID.clean();
        static_cast<CMobEntity*>(m_PBaseEntity)->updatemask |= UPDATE_STATUS;
        return 0;
    }

    CLuaBaseEntity* PEntity = nullptr;

    if (PEntity != nullptr && PEntity->GetBaseEntity()->objtype != TYPE_NPC)
    {
        battleutils::ClaimMob((CMobEntity*)m_PBaseEntity, (CBattleEntity*)PEntity->GetBaseEntity());
    }
    return 0;
}

/************************************************************************
 *  Function: hasEnmity()
 *  Purpose : Check if a an entity is on any mob's enmity list
 *  Example : if player:hasEnmity() then ...
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::hasEnmity(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    bool hasEnmity = static_cast<CBattleEntity*>(m_PBaseEntity)->PNotorietyContainer->hasEnmity();

    lua_pushboolean(L, hasEnmity);

    return 1;
}

/************************************************************************
 *  Function: getNotorietyList()
 *  Purpose : Return a list of mobs that hold enmity towards the player
 *  Example : local list = player:getNotorietyList()
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getNotorietyList(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    auto& notorietyContainer = static_cast<CBattleEntity*>(m_PBaseEntity)->PNotorietyContainer;

    int size = static_cast<int>(notorietyContainer->size());

    lua_createtable(L, size, 0);
    int i = 1;
    for (auto* entry : *notorietyContainer)
    {
        lua_getglobal(L, "CBaseEntity");
        lua_pushstring(L, "new");
        lua_gettable(L, -2);
        lua_insert(L, -2);
        lua_pushlightuserdata(L, (void*)entry);
        lua_pcall(L, 2, 1, 0);

        lua_rawseti(L, -2, i++);
    }

    return 1;
}

/************************************************************************
 *  Function: addStatusEffect(effect, power, tick, duration)
 *  Purpose : Adds a specified Status Effect to the Entity
 *  Example : target:addStatusEffect(EFFECT_ACCURACY_DOWN,20,3,60)
 *  Notes   :
 ************************************************************************/

bool CLuaBaseEntity::addStatusEffect(sol::object const& arg0, sol::object const& arg1, sol::object const& arg2, sol::object const& arg3,
                                     sol::object const& arg4, sol::object const& arg5, sol::object const& arg6)
{
    auto* PBattleEntity = dynamic_cast<CBattleEntity*>(m_PBaseEntity);
    if (!PBattleEntity)
    {
        return false;
    }

    if (arg0.is<sol::userdata>())
    {
        auto PStatusEffect = arg0.as<CLuaStatusEffect>();
        return PBattleEntity->StatusEffectContainer->AddStatusEffect(new CStatusEffect(*PStatusEffect.GetStatusEffect()));
    }
    else
    {
        CStatusEffect* PEffect = new CStatusEffect(arg0.as<EFFECT>(),                           // Effect ID
                                                   arg0.as<uint16>(),                           // Effect Icon (Associated with ID)
                                                   arg1.as<uint16>(),                           // Power
                                                   arg2.as<uint16>(),                           // Tick
                                                   arg3.as<uint16>(),                           // Duration
                                                   (arg5 != sol::nil) ? arg5.as<uint16>() : 0,  // SubID
                                                   (arg5 != sol::nil) ? arg5.as<uint16>() : 0,  // Sub Power
                                                   (arg6 != sol::nil) ? arg5.as<uint16>() : 0); // Tier

        if (PEffect->GetStatusID() == EFFECT_FOOD)
        {
            int16 durationModifier = PBattleEntity->getMod(Mod::FOOD_DURATION);
            if (durationModifier)
            {
                PEffect->SetDuration((uint32)(PEffect->GetDuration() + PEffect->GetDuration() * (durationModifier / 100.0f)));
            }
        }

        return PBattleEntity->StatusEffectContainer->AddStatusEffect(PEffect);
    }
}

/************************************************************************
 *  Function: addStatusEffectEx()
 *  Purpose : Adds an instance (or 'battle') Status Effect to the Entity
 *  Example : target:addStatusEffectEx(EFFECT_MOUNTED,EFFECT_MOUNTED,0,0,900,true)
 *  Notes   : For instance, Chocobo status, Fireflights, Teleport
 ************************************************************************/

bool CLuaBaseEntity::addStatusEffectEx(sol::object const& arg0, sol::object const& arg1, sol::object const& arg2, sol::object const& arg3,
                                       sol::object const& arg4, sol::object const& arg5, sol::object const& arg6, sol::object const& arg7,
                                       sol::object const& arg8, sol::object const& arg9)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    auto* PBattleEntity = dynamic_cast<CBattleEntity*>(m_PBaseEntity);
    if (!PBattleEntity)
    {
        return false;
    }

    auto isNil = [](auto& item) { return item == sol::nil; };
    std::vector<sol::object> args = { arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9 };
    args.erase(std::remove_if(args.begin(), args.end(), isNil), args.end());

    bool silent = false;
    auto lastElement = args.at(args.size() - 1);
    if (lastElement.is<bool>())
    {
        silent = lastElement.as<bool>();
    }

    CStatusEffect* PEffect =
        new CStatusEffect(arg0.as<EFFECT>(),                           // Effect ID
                          arg1.as<uint16>(),                           // Effect Icon ID
                          arg2.as<uint16>(),                           // Power
                          arg3.as<uint16>(),                           // Tick
                          arg4.as<uint16>(),                           // Duration
                          (arg5 != sol::nil) ? arg5.as<uint16>() : 0,  // Sub Effect ID
                          (arg6 != sol::nil) ? arg5.as<uint16>() : 0,  // Sub Power
                          (arg7 != sol::nil) ? arg5.as<uint16>() : 0,  // Tier
                          (arg8 != sol::nil) ? arg5.as<uint16>() : 0); // Effect Flag (i.e in lua tpz.effectFlag.AURA will make this an aura effect)

    return PBattleEntity->StatusEffectContainer->AddStatusEffect(PEffect, silent);
}

/************************************************************************
 *  Function: getStatusEffect()
 *  Purpose : Returns the Object of a specified Status ID
 *  Example : local debilitation = target:getStatusEffect(EFFECT_DEBILITATION)
 *  Notes   :
 ************************************************************************/

CStatusEffect* CLuaBaseEntity::getStatusEffect(uint16 StatusID, sol::object const& SubID)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    auto* PBattleEntity = dynamic_cast<CBattleEntity*>(m_PBaseEntity);
    if (!PBattleEntity)
    {
        return false;
    }

    CStatusEffect* PStatusEffect = nullptr;
    auto effect_StatusID = static_cast<EFFECT>(StatusID);

    if (SubID != sol::nil)
    {
        auto uint16_SubID = SubID.as<uint16>();
        PStatusEffect = PBattleEntity->StatusEffectContainer->GetStatusEffect(effect_StatusID, uint16_SubID);
    }
    else
    {
        PStatusEffect = PBattleEntity->StatusEffectContainer->GetStatusEffect(effect_StatusID);
    }

    return PStatusEffect;
}

/************************************************************************
 *  Function: getStatusEffects()
 *  Purpose : Returns a Lua table of all Status Effects an Entity has
 *  Example : local effects = caster:getStatusEffects() -- can iterate over table
 *  Notes   : Currently only used to check for Snake Eyes in ability.lua
 ************************************************************************/

sol::table CLuaBaseEntity::getStatusEffects()
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    auto* PBattleEntity = dynamic_cast<CBattleEntity*>(m_PBaseEntity);
    if (!PBattleEntity)
    {
        return false;
    }

    sol::table table;
    static_cast<CBattleEntity*>(m_PBaseEntity)->StatusEffectContainer->ForEachEffect([&](CStatusEffect* PEffect) {
        table.add(CLuaStatusEffect(PEffect));
    });

    return table;
}

/************************************************************************
 *  Function: getStatusEffectElement()
 *  Purpose : Returns the Element associated with a Status Effect
 *  Example : local element = mob:getStatusEffectElement(typeEffect)
 *  Notes   : For instnace, Bind = Ice, Slow = Earth
 ************************************************************************/

int16 CLuaBaseEntity::getStatusEffectElement(uint16 statusId)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    return effects::GetEffectElement(statusId);
}

/************************************************************************
 *  Function: canGainStatusEffect()
 *  Purpose : Returns true if an Entity can gain a Status Effect
 *  Example : if (target:canGainStatusEffect(EFFECT_STR_DOWN)) then
 *  Notes   :
 ************************************************************************/

bool CLuaBaseEntity::canGainStatusEffect(uint16 effect, uint16 power)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    auto* PBattleEntity = dynamic_cast<CBattleEntity*>(m_PBaseEntity);
    if (!PBattleEntity)
    {
        return false;
    }

    CStatusEffect statusEffect(static_cast<EFFECT>(effect), 0, power, 0, 0);

    return PBattleEntity->StatusEffectContainer->CanGainStatusEffect(&statusEffect);
}

/************************************************************************
 *  Function: hasStatusEffect()
 *  Purpose : Returns true if an Entity has a specific Status Effect active
 *  Example : if (player:hasStatusEffect(EFFECT_REFRESH)
 *  Notes   : More specific in scope than hasStatusEffectByFlag()
 ************************************************************************/

bool CLuaBaseEntity::hasStatusEffect(uint16 StatusID, sol::object const& SubID)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    auto* PBattleEntity = dynamic_cast<CBattleEntity*>(m_PBaseEntity);
    if (!PBattleEntity)
    {
        return false;
    }

    bool hasEffect = false;
    auto effect_StatusID = static_cast<EFFECT>(StatusID);

    if (SubID != sol::nil)
    {
        auto uint16_SubID = SubID.as<uint16>();
        hasEffect = PBattleEntity->StatusEffectContainer->HasStatusEffect(effect_StatusID, uint16_SubID);
    }
    else
    {
        hasEffect = PBattleEntity->StatusEffectContainer->HasStatusEffect(effect_StatusID);
    }

    return hasEffect;
}

/************************************************************************
 *  Function: hasStatusEffectByFlag()
 *  Purpose : Returns true if an Entity has a Status Effect of a specified Flag
 *  Example : if (target:hasStatusEffectByFlag(EFFECTFLAG_INVISIBLE)) then
 *  Notes   : More broad in scope than hasStatusEffect()
 ************************************************************************/

uint16 CLuaBaseEntity::hasStatusEffectByFlag(uint16 StatusID)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    auto* PBattleEntity = dynamic_cast<CBattleEntity*>(m_PBaseEntity);
    if (!PBattleEntity)
    {
        return false;
    }

    auto effect_StatusID = static_cast<EFFECT>(StatusID);
    return PBattleEntity->StatusEffectContainer->HasStatusEffectByFlag(effect_StatusID);
}

/************************************************************************
 *  Function: countEffect()
 *  Purpose : Returns the number of Effects an Entity has in their container
 *  Example : if (target:countEffect() > 3) then
 *  Notes   :
 ************************************************************************/

uint8 CLuaBaseEntity::countEffect(uint16 StatusID)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    auto* PBattleEntity = dynamic_cast<CBattleEntity*>(m_PBaseEntity);
    if (!PBattleEntity)
    {
        return false;
    }

    auto effect_StatusID = static_cast<EFFECT>(StatusID);
    return PBattleEntity->StatusEffectContainer->GetEffectsCount(effect_StatusID);
}

/************************************************************************
 *  Function: delStatusEffect()
 *  Purpose : Deletes a specified Effect from the Entity's Status Effect Container
 *  Example : target:delStatusEffect(EFFECT_RERAISE)
 *  Notes   : Can specify Power of the Effect as an option
 ************************************************************************/

bool CLuaBaseEntity::delStatusEffect(uint16 StatusID, sol::object const& SubID)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    auto* PBattleEntity = dynamic_cast<CBattleEntity*>(m_PBaseEntity);
    if (!PBattleEntity)
    {
        return false;
    }

    bool result = false;

    bool hasEffect = false;
    auto effect_StatusID = static_cast<EFFECT>(StatusID);

    if (SubID != sol::nil)
    {
        auto uint16_SubID = SubID.as<uint16>();
        result = PBattleEntity->StatusEffectContainer->DelStatusEffect(effect_StatusID, uint16_SubID);
    }
    else
    {
        result = PBattleEntity->StatusEffectContainer->DelStatusEffect(effect_StatusID);
    }

    return result;
}

/************************************************************************
 *  Function: delStatusEffectsByFlag()
 *  Purpose : Removes all Status Effects of a specified flag
 *  Example : target:delEffectsByFlag(FLAG)
 *  Notes   : Used for removal of multiple effects with matching flag
 ************************************************************************/

void CLuaBaseEntity::delStatusEffectsByFlag(uint16 flag, sol::object const& silent)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    auto* PBattleEntity = dynamic_cast<CBattleEntity*>(m_PBaseEntity);
    if (!PBattleEntity)
    {
        return;
    }

    bool bool_silent = silent.is<bool>() ? silent.as<bool>() : false;

    PBattleEntity->StatusEffectContainer->DelStatusEffectsByFlag(static_cast<EFFECTFLAG>(flag), bool_silent);
}

/************************************************************************
 *  Function: delStatusEffectSilent()
 *  Purpose : Removes a Status Effect from the Entity without showing a message
 *  Example : target:delStatusEffectSilent(EFFECT_SANDSTORM)
 *  Notes   : Used specifically for Status Effects that are not supposed to show a message once worn
 ************************************************************************/

bool CLuaBaseEntity::delStatusEffectSilent(uint16 StatusID)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    auto* PBattleEntity = dynamic_cast<CBattleEntity*>(m_PBaseEntity);
    if (!PBattleEntity)
    {
        return false;
    }

    auto effect_StatusID = static_cast<EFFECT>(StatusID);
    return PBattleEntity->StatusEffectContainer->DelStatusEffect(effect_StatusID);
}

/************************************************************************
 *  Function: eraseStatusEffect()
 *  Purpose : Removes an Erasable Status Effect from the Entity's Status Effect Container
 *  Example : target:eraseStatusEffect()
 *  Notes   : Can specify which type to remove, if Erasable
 ************************************************************************/

uint16 CLuaBaseEntity::eraseStatusEffect()
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    auto* PBattleEntity = dynamic_cast<CBattleEntity*>(m_PBaseEntity);
    if (!PBattleEntity)
    {
        return false;
    }

    auto effect = PBattleEntity->StatusEffectContainer->EraseStatusEffect();
    return static_cast<uint16>(effect);
}

/************************************************************************
 *  Function: eraseAllStatusEffect()
 *  Purpose : Removes an Erasable Status Effect from the Entity's Status Effect Container
 *  Example : target:eraseAllStatusEffect() -- Benediction
 *  Notes   : Can specify which type to remove, if Erasable
 ************************************************************************/

uint8 CLuaBaseEntity::eraseAllStatusEffect()
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    auto* PBattleEntity = dynamic_cast<CBattleEntity*>(m_PBaseEntity);
    if (!PBattleEntity)
    {
        return false;
    }

    return PBattleEntity->StatusEffectContainer->EraseAllStatusEffect();
}

/************************************************************************
 *  Function: dispelStatusEffect()
 *  Purpose : Removes a Dispelable Status Effect from the Entity's Status Effect Container
 *  Example : target:dispelStatusEffect()
 *  Notes   : Can specify which type to remove, if Dispelable
 ************************************************************************/

int32 CLuaBaseEntity::dispelStatusEffect(sol::object const& flagObj)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    auto* PBattleEntity = dynamic_cast<CBattleEntity*>(m_PBaseEntity);
    if (!PBattleEntity)
    {
        return false;
    }

    uint32 flag = flagObj.is<uint32>() ? flagObj.as<uint32>() : EFFECTFLAG_DISPELABLE;

    return PBattleEntity->StatusEffectContainer->DispelStatusEffect(static_cast<EFFECTFLAG>(flag));
}

/************************************************************************
 *  Function: dispelAllStatusEffect()
 *  Purpose : Removes all Status Effects from an Entity
 *  Example : target:dispelAllStatusEffect()
 *  Notes   : Can specify which types to remove, if Dispelable
 ************************************************************************/

uint8 CLuaBaseEntity::dispelAllStatusEffect(sol::object const& flagObj)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    auto* PBattleEntity = dynamic_cast<CBattleEntity*>(m_PBaseEntity);
    if (!PBattleEntity)
    {
        return false;
    }

    uint32 flag = flagObj.is<uint32>() ? flagObj.as<uint32>() : EFFECTFLAG_DISPELABLE;

    return PBattleEntity->StatusEffectContainer->DispelAllStatusEffect(static_cast<EFFECTFLAG>(flag));
}

/************************************************************************
 *  Function: stealStatusEffect()
 *  Purpose : Removes a dispellable status effect from one Entity and transfers it to the other
 *  Example : target:stealStatusEffect()
 *  Notes   : 
 ************************************************************************/

uint16 CLuaBaseEntity::stealStatusEffect(CLuaBaseEntity* PTargetEntity, sol::object const& flagObj)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    auto* PBattleEntity = dynamic_cast<CBattleEntity*>(m_PBaseEntity);
    if (!PBattleEntity)
    {
        return 0;
    }

    auto* PTargetBattleEntity = dynamic_cast<CBattleEntity*>(PTargetEntity->GetBaseEntity());
    if (!PTargetBattleEntity)
    {
        return 0;
    }

    uint32 flag = flagObj.is<uint32>() ? flagObj.as<uint32>() : EFFECTFLAG_DISPELABLE;

    if (CStatusEffect* PStatusEffect = PTargetBattleEntity->StatusEffectContainer->StealStatusEffect(static_cast<EFFECTFLAG>(flag)))
    {
        PBattleEntity->StatusEffectContainer->AddStatusEffect(PStatusEffect);
        return PStatusEffect->GetStatusID();
    }
    else
    {
        return 0;
    }
}

/************************************************************************
 *  Function: addMod()
 *  Purpose : Adds a Mod to the Entity
 *  Example : target:addMod(MOD_INT, 10)
 *  Notes   : If Mod ID already exists, adds the amount to existing amount
 ************************************************************************/

void CLuaBaseEntity::addMod(uint16 type, int16 amount)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    auto* PBattleEntity = dynamic_cast<CBattleEntity*>(m_PBaseEntity);
    if (!PBattleEntity)
    {
        return;
    }

    PBattleEntity->addModifier(static_cast<Mod>(type), amount);
}

/************************************************************************
 *  Function: getMod()
 *  Purpose : Returns the integer value of a specified Mod on the Entity
 *  Example : if (target:getMod(MOD_MND) > 10) then
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getMod(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    lua_pushinteger(L, ((CBattleEntity*)m_PBaseEntity)->getMod(static_cast<Mod>(lua_tointeger(L, 1))));
    return 1;
}

/************************************************************************
 *  Function: setMod()
 *  Purpose : Sets a specified Mod and Amount for the Entity
 *  Example : target:setMod(MOD_STR, 20)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::setMod(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));

    ((CBattleEntity*)m_PBaseEntity)->setModifier(static_cast<Mod>(lua_tointeger(L, 1)), (int16)lua_tointeger(L, 2));
    return 0;
}

/************************************************************************
 *  Function: delMod()
 *  Purpose : Removes a specified Mod and amount from the Entity
 *  Example : target:delMod(MOD_STR,4)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::delMod(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));

    ((CBattleEntity*)m_PBaseEntity)->delModifier(static_cast<Mod>(lua_tointeger(L, 1)), (int16)lua_tointeger(L, 2));
    return 0;
}

/************************************************************************
 *  Function: addLatent()
 *  Purpose : Adds the specified latent to the player
 *  Example : player:addLatent(tpz.latent.LATENT_HP_UNDER_PERCENT, 95, tpz.mod.REGEN, 1)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::addLatent(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 3) || !lua_isnumber(L, 3));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 4) || !lua_isnumber(L, 4));

    LATENT conditionID    = static_cast<LATENT>(lua_tointeger(L, 1));
    uint16 conditionValue = (uint16)lua_tointeger(L, 2);
    Mod    modID          = static_cast<Mod>(lua_tointeger(L, 3));
    int16  modValue       = (int16)lua_tointeger(L, 4);

    ((CCharEntity*)m_PBaseEntity)->PLatentEffectContainer->AddLatentEffect(conditionID, conditionValue, modID, modValue);

    return 0;
}

/************************************************************************
 *  Function: delLatent()
 *  Purpose : Removes the specified latent to the player. Returns if successfully removed or not.
 *  Example : player:delLatent(tpz.latent.LATENT_HP_UNDER_PERCENT, 95, tpz.mod.REGEN, 1)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::delLatent(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 3) || !lua_isnumber(L, 3));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 4) || !lua_isnumber(L, 4));

    LATENT conditionID    = static_cast<LATENT>(lua_tointeger(L, 1));
    uint16 conditionValue = (uint16)lua_tointeger(L, 2);
    Mod    modID          = static_cast<Mod>(lua_tointeger(L, 3));
    int16  modValue       = (int16)lua_tointeger(L, 4);

    lua_pushboolean(L, ((CCharEntity*)m_PBaseEntity)->PLatentEffectContainer->DelLatentEffect(conditionID, conditionValue, modID, modValue));

    return 0;
}

/************************************************************************
 *  Function: fold()
 *  Purpose : Removes the most recent Phantom Roll or Bust effect
 *  Example : target:fold()
 *  Notes   : Calls the Fold member of CStatusEffectContainer for calculation
 ************************************************************************/

inline int32 CLuaBaseEntity::fold(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    CBattleEntity* PEntity = (CBattleEntity*)m_PBaseEntity;
    PEntity->StatusEffectContainer->Fold(PEntity->id);

    return 0;
}

/************************************************************************
 *  Function: doWildCard()
 *  Purpose : Executes the Wild Card two hour for a COR
 *  Example : caster:doWildCard(target,total)
 *  Notes   : Calls the DoWildCardToEntity member of battleutils
 ************************************************************************/

inline int32 CLuaBaseEntity::doWildCard(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isuserdata(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));

    CLuaBaseEntity* PEntity = nullptr;
    battleutils::DoWildCardToEntity(static_cast<CCharEntity*>(m_PBaseEntity), static_cast<CCharEntity*>(PEntity->m_PBaseEntity), (uint8)lua_tointeger(L, 2));

    return 0;
}

/************************************************************************
 *  Function: addCorsairRoll()
 *  Purpose : Adds the Corsair Roll to the Target's Status Effect Container
 *  Example : target:addCorsairRoll(caster:getMainJob(), caster:getMerit(MERIT_BUST_DURATION), EFFECT_CHAOS_ROLL, effectpower, 0, duration, caster:getID(),
 *total, MOD_ATTP) Notes   : Returns true if success (Is range a factor?)
 ************************************************************************/

inline int32 CLuaBaseEntity::addCorsairRoll(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 3) || !lua_isnumber(L, 3));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 4) || !lua_isnumber(L, 4));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 5) || !lua_isnumber(L, 5));

    int32 n = lua_gettop(L);

    auto casterJob    = lua_tointeger(L, 1);
    auto bustDuration = (uint8)lua_tointeger(L, 2);

    CStatusEffect* PEffect  = new CStatusEffect((EFFECT)lua_tointeger(L, 3),                 // Effect ID
                                               (uint16)lua_tointeger(L, 3),                 // Effect Icon (Associated with ID)
                                               (uint16)lua_tointeger(L, 4),                 // Power
                                               (uint32)lua_tointeger(L, 5),                 // Tick
                                               (uint32)lua_tointeger(L, 6),                 // Duration
                                               (n >= 7 ? (uint32)lua_tointeger(L, 7) : 0),  // SubID or 0
                                               (n >= 8 ? (uint16)lua_tointeger(L, 8) : 0),  // SubPower or 0
                                               (n >= 9 ? (uint16)lua_tointeger(L, 9) : 0)); // Tier or 0
    uint8          maxRolls = 2;
    if (casterJob != JOB_COR)
    {
        maxRolls = 1;
    }
    lua_pushboolean(L, ((CBattleEntity*)m_PBaseEntity)->StatusEffectContainer->ApplyCorsairEffect(PEffect, maxRolls, bustDuration));
    return 1;
}

/************************************************************************
 *  Function: hasCorsairEffect()
 *  Purpose : Returns true if the Entity has Corsair Effect
 *  Example : if (target:hasCorsairEffect())
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::hasCorsairEffect(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    CBattleEntity* PEntity = (CBattleEntity*)m_PBaseEntity;

    lua_pushboolean(L, PEntity->StatusEffectContainer->HasCorsairEffect(PEntity->id));

    return 1;
}

/************************************************************************
 *  Function: hasBustEffect()
 *  Purpose : Returns true if an Entity has a Bust Effect of a specified type
 *  Example : if (target:hasBustEffect(EFFECT)) then
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::hasBustEffect(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    bool hasEffect = false;

    hasEffect = ((CBattleEntity*)m_PBaseEntity)->StatusEffectContainer->HasBustEffect((EFFECT)lua_tointeger(L, 1));

    lua_pushboolean(L, hasEffect);
    return 1;
}

/************************************************************************
 *  Function: numBustEffects()
 *  Purpose : Returns the count of how many Bust Effects are in the Entity's container
 *  Example : if (player:numBustEffects() == 2) then
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::numBustEffects(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    uint8 numEffects = ((CBattleEntity*)m_PBaseEntity)->StatusEffectContainer->GetEffectsCount(EFFECT_BUST);

    lua_pushinteger(L, numEffects);
    return 1;
}

/************************************************************************
 *  Function: healingWaltz()
 *  Purpose : Executes the Healing Waltz effect on Status Effect Container
 *  Example : target:healingWaltz()
 *  Notes   : Erases one random effect if it's 'Waltzable'
 ************************************************************************/

inline int32 CLuaBaseEntity::healingWaltz(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    lua_pushinteger(L, ((CBattleEntity*)m_PBaseEntity)->StatusEffectContainer->HealingWaltz());
    return 1;
}

/************************************************************************
 *  Function: addBardSong()
 *  Purpose : Adds a song effect to Player(s') Status Effect Container(s); returns true if sucess
 *  Example : target:addBardSong(caster,EFFECT_BALLAD,power,0,duration,caster:getID(), 0, 1)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::addBardSong(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isuserdata(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 3) || !lua_isnumber(L, 3));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 4) || !lua_isnumber(L, 4));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 5) || !lua_isnumber(L, 5));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 6) || !lua_isnumber(L, 6));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 7) || !lua_isnumber(L, 7));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 8) || !lua_isnumber(L, 8));

    CLuaBaseEntity* PEntity = nullptr; // Caster

    CStatusEffect* PEffect = new CStatusEffect((EFFECT)lua_tointeger(L, 2),  // Effect ID
                                               (uint16)lua_tointeger(L, 2),  // Effect Icon (Associated with ID)
                                               (uint16)lua_tointeger(L, 3),  // Power
                                               (uint16)lua_tointeger(L, 4),  // Tick
                                               (uint16)lua_tointeger(L, 5),  // Duration
                                               (uint16)lua_tointeger(L, 6),  // SubID
                                               (uint16)lua_tointeger(L, 7),  // SubPower
                                               (uint16)lua_tointeger(L, 8)); // Tier

    uint8 maxSongs = 2;

    if (PEntity && PEntity->m_PBaseEntity && PEntity->m_PBaseEntity->objtype == TYPE_PC)
    {
        CCharEntity* PCaster = (CCharEntity*)PEntity->m_PBaseEntity;
        CItemWeapon* PItem   = (CItemWeapon*)PCaster->getEquip(SLOT_RANGED);
        if (PItem == nullptr || PItem->getID() == 65535 ||
            !(PItem->getSkillType() == SKILL_STRING_INSTRUMENT || PItem->getSkillType() == SKILL_WIND_INSTRUMENT))
        {
            maxSongs = 1;
        }
        maxSongs += PCaster->getMod(Mod::MAXIMUM_SONGS_BONUS);
    }
    lua_pushboolean(L, ((CBattleEntity*)m_PBaseEntity)->StatusEffectContainer->ApplyBardEffect(PEffect, maxSongs));
    return 1;
}

/************************************************************************
 *  Function: charm()
 *  Purpose : Charms an entity target
 *  Example : mob:charm(target)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::charm(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isuserdata(L, 1));

    CLuaBaseEntity* PTarget = nullptr;
    battleutils::applyCharm((CBattleEntity*)m_PBaseEntity, (CBattleEntity*)PTarget->GetBaseEntity());

    return 0;
}

/************************************************************************
 *  Function: uncharm()
 *  Purpose : Removes charm from an entity
 *  Example : target:uncharm()
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::uncharm(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    battleutils::unCharm((CBattleEntity*)m_PBaseEntity);

    return 0;
}

/************************************************************************
 *  Function: addBurden()
 *  Purpose : Adds a Burden to a Target
 *  Example : local overload = target:addBurden(ELE_EARTH-1, burden)
 *  Notes   : Used for Automation abilities
 ************************************************************************/

inline int32 CLuaBaseEntity::addBurden(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));

    if (((CBattleEntity*)m_PBaseEntity)->PPet && ((CPetEntity*)((CBattleEntity*)m_PBaseEntity)->PPet)->getPetType() == PET_TYPE::AUTOMATON)
    {
        lua_pushinteger(L, ((CAutomatonEntity*)((CBattleEntity*)m_PBaseEntity)->PPet)->addBurden((uint8)lua_tointeger(L, 1), (int8)lua_tointeger(L, 2)));
    }
    else
    {
        lua_pushinteger(L, 0);
    }
    return 1;
}

/************************************************************************
 *  Function: setStatDebilitation()
 *  Purpose : Updates the private member m_StatsDebilitation in CCharEntity
 *  Example : target:setStatDebilitation(power)
 *  Notes   : Used only through scripts/globals/effects/debilitation.lua
 ************************************************************************/

int32 CLuaBaseEntity::setStatDebilitation(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    if (m_PBaseEntity->objtype == TYPE_PC)
    {
        auto* PChar{ static_cast<CCharEntity*>(m_PBaseEntity) };
        PChar->m_StatsDebilitation = (uint16)lua_tointeger(L, 1);
        PChar->pushPacket(new CCharJobsPacket(PChar));
    }
    return 0;
}

/************************************************************************
 *  Function: getStat()
 *  Purpose : Returns a particular stat for an Entity
 *  Example : caster:getStat(MOD_INT)
 *  Notes   :
 ************************************************************************/

uint16 CLuaBaseEntity::getStat(uint16 statId)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    auto*  PEntity = static_cast<CBattleEntity*>(m_PBaseEntity);
    uint16 value   = 0;

    switch (static_cast<Mod>(statId))
    {
        case Mod::STR:
            value = PEntity->STR();
            break;
        case Mod::DEX:
            value = PEntity->DEX();
            break;
        case Mod::VIT:
            value = PEntity->VIT();
            break;
        case Mod::AGI:
            value = PEntity->AGI();
            break;
        case Mod::INT:
            value = PEntity->INT();
            break;
        case Mod::MND:
            value = PEntity->MND();
            break;
        case Mod::CHR:
            value = PEntity->CHR();
            break;
        case Mod::ATT:
            value = PEntity->ATT();
            break;
        case Mod::DEF:
            value = PEntity->DEF();
            break;
        case Mod::EVA:
            value = PEntity->EVA();
            break;
        // TODO: support getStat for ACC/RACC/RATT
        // case Mod::ACC:  lua_pushinteger(L, PEntity->ACC()); break;
        // case Mod::RACC: lua_pushinteger(L, PEntity->RACC()); break;
        // case Mod::RATT: lua_pushinteger(L, PEntity->RATT()); break;
        default:
            // We should probably show a warning here
            break;
    }

    return value;
}

/************************************************************************
 *  Function: getACC()
 *  Purpose : Returns the  Accuracy of an Entity
 *  Example : player:getACC()
 *  Notes   : Uses the ACC member of CBattleEntity for calculation
 ************************************************************************/

uint16 CLuaBaseEntity::getACC()
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    auto* PEntity = static_cast<CBattleEntity*>(m_PBaseEntity);
    return PEntity->ACC(0, 0); // (attackNumber = 0, offsetAcc = 0)
}

/************************************************************************
 *  Function: getEVA()
 *  Purpose : Returns the Evasion of an Entity
 *  Example : player:getEVA()
 *  Notes   : Uses the EVA member of CBattleEntity for calculation
 ************************************************************************/

uint16 CLuaBaseEntity::getEVA()
{
    auto* PEntity = static_cast<CBattleEntity*>(m_PBaseEntity);
    return PEntity->EVA();
}

/************************************************************************
 *  Function: getRACC()
 *  Purpose : Calculates and returns the Ranged Accuracy of a Weapon euipped in the Ranged slot
 *  Example : player:getRACC()
 *  Notes   : To Do: The calculation is already a public member of battleentity, shouldn't have two calculations, just call (CBattleEntity*)m_PBaseEntity)->RACC
 *and return result
 ************************************************************************/

int CLuaBaseEntity::getRACC()
{
    auto* weapon = dynamic_cast<CItemWeapon*>(static_cast<CBattleEntity*>(m_PBaseEntity)->m_Weapons[SLOT_RANGED]);

    if (weapon == nullptr)
    {
        ShowDebug(CL_CYAN "lua::getRACC weapon in ranged slot is NULL!\n" CL_RESET);
        return 0;
    }

    CBattleEntity* PEntity = static_cast<CBattleEntity*>(m_PBaseEntity);

    int skill = PEntity->GetSkill(weapon->getSkillType());
    int acc   = skill;

    if (skill > 200)
    {
        acc = (int)(200 + (skill - 200) * 0.9);
    }

    acc += PEntity->getMod(Mod::RACC);
    acc += PEntity->AGI() / 2;
    acc = acc + std::min<int16>(((100 + PEntity->getMod(Mod::FOOD_RACCP)) * acc / 100), PEntity->getMod(Mod::FOOD_RACC_CAP));

    return acc;
}

/************************************************************************
 *  Function: getRATT()
 *  Purpose : Returns the Ranged Attack value of an equipped Ranged weapon
 *  Example : player:getRATT()
 *  Notes   : Calls the RATT member function of CBattleEntity for calculation
 ************************************************************************/

uint16 CLuaBaseEntity::getRATT()
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    auto* weapon = dynamic_cast<CItemWeapon*>(static_cast<CBattleEntity*>(m_PBaseEntity)->m_Weapons[SLOT_RANGED]);

    if (weapon == nullptr)
    {
        ShowDebug(CL_CYAN "lua::getRATT weapon in ranged slot is NULL!\n" CL_RESET);
        return 0;
    }

    return static_cast<CBattleEntity*>(m_PBaseEntity)->RATT(weapon->getSkillType(), weapon->getILvlSkill());
}

/************************************************************************
 *  Function: getILvlMacc()
 *  Purpose : Returns the Magic Accuracy value of an equipped Main Weapon
 *  Example : caster:getILvlMacc()
 *  Notes   : Value of m_iLvlMacc (private member of CItemWeapon)
 ************************************************************************/

uint16 CLuaBaseEntity::getILvlMacc()
{
    if (auto* weapon = dynamic_cast<CItemWeapon*>(static_cast<CBattleEntity*>(m_PBaseEntity)->m_Weapons[SLOT_MAIN]))
    {
        return weapon->getILvlMacc();
    }

    return 0;
}

/************************************************************************
 *  Function: isSpellAoE()
 *  Purpose : Returns true if a specified spell is AoE
 *  Example : if (caster:isSpellAoE(spell:getID())) then
 *  Notes   : Only found in scripts/globals/magic.lua
 ************************************************************************/

bool CLuaBaseEntity::isSpellAoE(uint16 spellId)
{
    CBattleEntity* PEntity = static_cast<CBattleEntity*>(m_PBaseEntity);
    CSpell*        PSpell  = spell::GetSpell(static_cast<SpellID>(spellId));

    if (PSpell != nullptr)
    {
        return battleutils::GetSpellAoEType(PEntity, PSpell) > 0;
    }

    return false;
}

/************************************************************************
 *  Function: physicalDmgTaken()
 *  Purpose : Returns the value of Physical Damage taken after calculation
 *  Example : dmg = target:physicalDmgTaken(dmg, damageType)
 *  Notes   : Passes argument to PhysicalDmgTaken member of battleutils.
 *            DamageType is optional and defaults to weapon type if not provided.
 ************************************************************************/

int32 CLuaBaseEntity::physicalDmgTaken(int32 damage, sol::object const& dmgType)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    DAMAGE_TYPE damageType = (dmgType != sol::nil) ? dmgType.as<DAMAGE_TYPE>() : DAMAGE_TYPE::NONE;

    return battleutils::PhysicalDmgTaken(static_cast<CBattleEntity*>(m_PBaseEntity), damage, damageType);
}

/************************************************************************
 *  Function: magicDmgTaken()
 *  Purpose : Returns the value of Magic Damage taken after calculation
 *  Example : dmg = target:magicDmgTaken(dmg)
 *  Notes   : Passes argument to MagicDmgTaken member of battleutils
 ************************************************************************/

inline int32 CLuaBaseEntity::magicDmgTaken(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    if (!lua_isnil(L, 2) && lua_isnumber(L, 2) && lua_tointeger(L, 2) > 0 && lua_tointeger(L, 2) < 9)
    {
        lua_pushinteger(L, battleutils::MagicDmgTaken((CBattleEntity*)m_PBaseEntity, (int32)lua_tointeger(L, 1), (ELEMENT)lua_tointeger(L, 2)));
    }
    else
    {
        lua_pushinteger(L, battleutils::MagicDmgTaken((CBattleEntity*)m_PBaseEntity, (int32)lua_tointeger(L, 1), ELEMENT_NONE));
    }

    return 1;
}

/************************************************************************
 *  Function: rangedDmgTaken()
 *  Purpose : Returns the value of Ranged Damage taken after calculation
 *  Example : dmg = target:rangedDmgTaken(dmg)
 *  Notes   : Passes argument to RangedDmgTaken member of battleutils
 ************************************************************************/

int32 CLuaBaseEntity::rangedDmgTaken(int32 damage, sol::object const& dmgType)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    DAMAGE_TYPE damageType = (dmgType != sol::nil) ? dmgType.as<DAMAGE_TYPE>() : DAMAGE_TYPE::NONE;

    return battleutils::RangedDmgTaken(static_cast<CBattleEntity*>(m_PBaseEntity), damage, damageType);
}

/************************************************************************
 *  Function: breathDmgTaken()
 *  Purpose : Returns the value of Breath Damage taken after calculation
 *  Example : local dmg = target:breathDmgTaken(dmg)
 *  Notes   : Passes argument to BreathDmgTaken member of battleutils
 ************************************************************************/

int32 CLuaBaseEntity::breathDmgTaken(int32 damage)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    return battleutils::BreathDmgTaken(static_cast<CBattleEntity*>(m_PBaseEntity), damage);
}

/************************************************************************
 *  Function: handleAfflatusMiseryDamage()
 *  Purpose : Passes an argument to the HandleAfflatusMiseryDamage member of battleutils
 *  Example : target:handleAfflatusMiseryDamage(dmg)
 *  Notes   :
 ************************************************************************/

void CLuaBaseEntity::handleAfflatusMiseryDamage(int32 damage)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    battleutils::HandleAfflatusMiseryDamage(static_cast<CBattleEntity*>(m_PBaseEntity), damage);
}

/************************************************************************
 *  Function: isWeaponTwoHanded()
 *  Purpose : Returns true if the Weapon in the Main Slot is two-handed
 *  Example : if (player:isWeaponTwoHanded()) then
 *  Notes   :
 ************************************************************************/

bool CLuaBaseEntity::isWeaponTwoHanded()
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    auto* weapon = dynamic_cast<CItemWeapon*>(static_cast<CBattleEntity*>(m_PBaseEntity)->m_Weapons[SLOT_MAIN]);

    if (weapon == nullptr)
    {
        ShowDebug(CL_CYAN "lua::getWeaponDmg weapon in main slot is NULL!\n" CL_RESET);
        return 0;
    }

    return weapon->isTwoHanded();
}

/************************************************************************
 *  Function: getMeleeHitDamage()
 *  Purpose : Calculates and returns total damage for a single hit
 *  Example : getMeleeHitDamage(Attacker,Local Hit Rate)
 *  Notes   : Battleutils calculates hit rate already, so inserting hit rate
 *          : here only increases chance of missing (assuming < 100)?
 *          : Not currently used in any scripts (handled by battleutils) - Is this even needed?
 ************************************************************************/

inline int32 CLuaBaseEntity::getMeleeHitDamage(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isuserdata(L, 1));

    CLuaBaseEntity* PLuaBaseEntity = nullptr;

    CBattleEntity* PAttacker = (CBattleEntity*)m_PBaseEntity;
    CBattleEntity* PDefender = (CBattleEntity*)PLuaBaseEntity->GetBaseEntity();
    auto           hitrate   = battleutils::GetHitRate(PAttacker, PDefender);
    if (!lua_isnil(L, 2) && lua_isnumber(L, 2))
    {
        hitrate = (uint8)lua_tointeger(L, 2);
    }

    if (tpzrand::GetRandomNumber(100) < hitrate)
    {
        float DamageRatio = battleutils::GetDamageRatio(PAttacker, PDefender, false, 0.f);
        int   damage      = (uint16)((PAttacker->GetMainWeaponDmg() + battleutils::GetFSTR(PAttacker, PDefender, SLOT_MAIN)) * DamageRatio);
        lua_pushinteger(L, damage);
        return 1;
    }
    lua_pushinteger(L, -1);
    return 1;
}

/************************************************************************
 *  Function: getWeaponDmg()
 *  Purpose : Returns the real damage value of a Weapon in the Main slot
 *  Example : local weaponDamage = attacker:getWeaponDmg()
 *  Notes   : Also used in Mob damage calculations
 ************************************************************************/

inline int32 CLuaBaseEntity::getWeaponDmg(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    uint16 weapondam = ((CBattleEntity*)m_PBaseEntity)->GetMainWeaponDmg();

    lua_pushinteger(L, weapondam);
    return 1;
}

/************************************************************************
 *  Function: getWeaponDmgRank()
 *  Purpose : Returns the damage rating for the Weapon in the Main slot
 *  Example : attacker:getWeaponDmgRank()
 *  Notes   : Primarily used in fSTR calculation in scripts/globals/weaponskills.lua
 ************************************************************************/

inline int32 CLuaBaseEntity::getWeaponDmgRank(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    uint16 weapondam = ((CBattleEntity*)m_PBaseEntity)->GetMainWeaponRank();

    lua_pushinteger(L, weapondam);
    return 1;
}

/************************************************************************
 *  Function: getOffhandDmg()
 *  Purpose : Returns the damage rating for the Weapon in the Offhand slot
 *  Example : if (player:getOffhandDmg() > 0)
 *  Notes   : Mainly used to add an extra TP Hit in Weaponskills
 ************************************************************************/

inline int32 CLuaBaseEntity::getOffhandDmg(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    uint16 weapondam = ((CBattleEntity*)m_PBaseEntity)->GetSubWeaponDmg();

    lua_pushinteger(L, weapondam);
    return 1;
}

/************************************************************************
 *  Function: getOffhandDmgRank()
 *  Purpose : Returns the damage rank for the weapon in the Offhand slot
 *  Example : player:getOffhandDmgRank()
 *  Notes   : Not currently being used in any script calculation
 ************************************************************************/

inline int32 CLuaBaseEntity::getOffhandDmgRank(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    uint16 weapondam = ((CBattleEntity*)m_PBaseEntity)->GetSubWeaponRank();

    lua_pushinteger(L, weapondam);
    return 1;
}

/************************************************************************
 *  Function: getRangedDmg()
 *  Purpose : Returns the damage rating for the weapon in the Ranged slot
 *  Example : local dmg = (2 * player:getRangedDmg() + player:getAmmoDmg())
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getRangedDmg(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    uint16 weapondam = ((CBattleEntity*)m_PBaseEntity)->GetRangedWeaponDmg();

    lua_pushinteger(L, weapondam);
    return 1;
}

/************************************************************************
 *  Function: getRangedDmgRank()
 *  Purpose : Used in determining fSTR caculcation in weaponskills.lua
 *  Example : attacker:getRangedDmgRank()
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getRangedDmgRank(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    uint16 weaponrank = ((CBattleEntity*)m_PBaseEntity)->GetRangedWeaponRank();

    lua_pushinteger(L, weaponrank);
    return 1;
}

/************************************************************************
 *  Function: getAmmoDmg()
 *  Purpose : Returns the damage rating for the weapon in Ammo slot
 *  Example : local dmg = (2 * player:getRangedDmg() + player:getAmmoDmg()
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getAmmoDmg(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    auto* weapon = dynamic_cast<CItemWeapon*>(((CBattleEntity*)m_PBaseEntity)->m_Weapons[SLOT_AMMO]);

    if (weapon == nullptr)
    {
        ShowDebug(CL_CYAN "lua::getAmmoDmg weapon in ammo slot is NULL!\n" CL_RESET);
        return 0;
    }
    lua_pushinteger(L, weapon->getDamage());
    return 1;
}

/************************************************************************
 *  Function: removeAmmo()
 *  Purpose : Expends one item in the ammo slot (arrow,bullet, etc)
 *  Example : player:removeAmmo()
 *  Notes   : Ammo consumed is calculated in charentity.cpp and passed to battleutils
 ************************************************************************/

int32 CLuaBaseEntity::removeAmmo(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    battleutils::RemoveAmmo(static_cast<CCharEntity*>(m_PBaseEntity));
    return 0;
}

/************************************************************************
 *  Function: getWeaponSkillLevel()
 *  Purpose : Returns the player's skill level for the weapon in a slot
 *  Example : caster:getWeaponSkillLevel(SLOT_RANGED)
 *  Notes   : Mainly used to determine String/Wind level, but can be used for others
 ************************************************************************/

inline int32 CLuaBaseEntity::getWeaponSkillLevel(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    if (m_PBaseEntity->objtype == TYPE_PC)
    {
        uint8 SLOT = (uint8)lua_tointeger(L, 1);

        TPZ_DEBUG_BREAK_IF(SLOT > 3);

        CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

        auto* PWeapon = dynamic_cast<CItemWeapon*>(PChar->m_Weapons[SLOT]);

        if ((PWeapon != nullptr) && PWeapon->isType(ITEM_WEAPON))
        {
            lua_pushinteger(L, PChar->GetSkill(PWeapon->getSkillType()));
            return 1;
        }
    }
    lua_pushinteger(L, 0);
    return 1;
}

/************************************************************************
 *  Function: getWeaponDamageType()
 *  Purpose : Returns the primary type of a weapon in a slot
 *  Example : if (attacker:getWeaponDamageType(SLOT_MAIN) == tpz.damageType.PIERCING)
 *  Notes   : Used to identify which damage type is the weapon
 ************************************************************************/

inline int32 CLuaBaseEntity::getWeaponDamageType(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    if (!lua_isnil(L, 1) && lua_isnumber(L, 1))
    {
        uint8 SLOT = (uint8)lua_tointeger(L, 1);
        if (SLOT > 3)
        {
            lua_pushinteger(L, 0);
            return 1;
        }
        auto* weapon = dynamic_cast<CItemWeapon*>(((CBattleEntity*)m_PBaseEntity)->m_Weapons[SLOT]);
        if (weapon == nullptr)
        {
            lua_pushinteger(L, 0);
            return 1;
        }
        lua_pushinteger(L, static_cast<uint16>(weapon->getDmgType()));
        return 1;
    }
    ShowError(CL_RED "lua::getWeaponDamageType :: Invalid slot specified!" CL_RESET);
    return 0;
}

/************************************************************************
 *  Function: getWeaponSkillType()
 *  Purpose : Returns the primary type of a weapon in a slot
 *  Example : if (attacker:getWeaponSkillType(SLOT_MAIN) == SKILL_HAND_TO_HAND)
 *  Notes   : Used to identify which type of weapon it is (Katana, Sword, etc)
 ************************************************************************/

inline int32 CLuaBaseEntity::getWeaponSkillType(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    if (!lua_isnil(L, 1) && lua_isnumber(L, 1))
    {
        uint8 SLOT = (uint8)lua_tointeger(L, 1);
        if (SLOT > 3)
        {
            lua_pushinteger(L, 0);
            return 1;
        }
        auto* weapon = dynamic_cast<CItemWeapon*>(((CBattleEntity*)m_PBaseEntity)->m_Weapons[SLOT]);
        if (weapon == nullptr)
        {
            lua_pushinteger(L, 0);
            return 1;
        }
        lua_pushinteger(L, weapon->getSkillType());
        return 1;
    }
    ShowError(CL_RED "lua::getWeaponSkillType :: Invalid slot specified!" CL_RESET);
    return 0;
}

/************************************************************************
 *  Function: getWeaponSubSkillType()
 *  Purpose : Returns the integer value of the Weapon's Sub Type
 *  Example : if (player:getWeaponSubSkillType(SLOT_RANGED) == 10)
 *  Notes   : Mainly used to differentiate between ammo and ranged equipment
 ************************************************************************/

inline int32 CLuaBaseEntity::getWeaponSubSkillType(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    if (!lua_isnil(L, 1) && lua_isstring(L, 1))
    {
        uint8 SLOT = (uint8)lua_tointeger(L, 1);
        if (SLOT > 3)
        {
            ShowDebug(CL_CYAN "lua::getWeaponSubskillType slot not a weapon\n" CL_RESET);
            lua_pushinteger(L, 0);
            return 1;
        }
        CItemWeapon* weapon = (CItemWeapon*)((CCharEntity*)m_PBaseEntity)->getEquip((SLOTTYPE)SLOT);

        if (weapon == nullptr)
        {
            lua_pushinteger(L, 0);
            return 1;
        }

        lua_pushinteger(L, weapon->getSubSkillType());
        return 1;
    }
    ShowError(CL_RED "lua::getWeaponSubskillType :: Invalid slot specified!" CL_RESET);
    lua_pushinteger(L, 0);
    return 1;
}

/************************************************************************
*  Function: getWSSkillchainProp()
*  Purpose : For the current weaponskill in use, returns the properties of Primary,
             Secondary, and Tertiary skillchains associated with that weaponskill
*  Example : attacker:getWSSkillchainProp()
*  Notes   : Used in determining Obi/Gorget bonuses (scripts/globals/weaponskills.lua)
************************************************************************/

inline int32 CLuaBaseEntity::getWSSkillchainProp(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    auto* state = dynamic_cast<CWeaponSkillState*>(m_PBaseEntity->PAI->GetCurrentState());

    if (state)
    {
        lua_pushinteger(L, state->GetSkill()->getPrimarySkillchain());
        lua_pushinteger(L, state->GetSkill()->getSecondarySkillchain());
        lua_pushinteger(L, state->GetSkill()->getTertiarySkillchain());

        return 3;
    }

    lua_pushnil(L);
    return 1;
}

/************************************************************************
 *  Function: takeWeaponskillDamage()
 *  Purpose : Calls Battle Utils to calculate final weapon skill damage against a foe
 *  Example : defender:takeWeaponskillDamage(attacker, finaldmg, attackType, damageType, slot, primary, tpHitsLanded, (extraHitsLanded * 10) + bonusTP,
 *targetTPMult) Notes   : Global function of same name in weaponskills.lua, calls this member function from within
 ************************************************************************/

int32 CLuaBaseEntity::takeWeaponskillDamage(lua_State* L)
{
    /*
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isuserdata(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 3) || !lua_isnumber(L, 3));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 4) || !lua_isnumber(L, 4));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 5) || !lua_isnumber(L, 5));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 6) || !lua_isboolean(L, 6));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 7) || !lua_isnumber(L, 7));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 8) || !lua_isnumber(L, 8));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 9) || !lua_isnumber(L, 9));

    auto*       PChar              = static_cast<CCharEntity*>(Lunar<CLuaBaseEntity>::check(L, 1)->m_PBaseEntity);
    auto        damage             = (int32)lua_tointeger(L, 2);
    ATTACK_TYPE attackType         = static_cast<ATTACK_TYPE>(lua_tointeger(L, 3));
    DAMAGE_TYPE damageType         = static_cast<DAMAGE_TYPE>(lua_tointeger(L, 4));
    auto        slot               = (uint8)lua_tointeger(L, 5);
    auto        primary            = lua_toboolean(L, 6);
    auto        tpMultiplier       = (float)lua_tonumber(L, 7);
    auto        bonusTP            = (uint16)lua_tointeger(L, 8);
    auto        targetTPMultiplier = (float)lua_tonumber(L, 9);

    lua_pushinteger(L, (lua_Integer)battleutils::TakeWeaponskillDamage(PChar, static_cast<CBattleEntity*>(m_PBaseEntity), damage, attackType, damageType, slot,
                                                                       primary, tpMultiplier, bonusTP, targetTPMultiplier));
    */
    return 1;
}

/************************************************************************
 *  Function: int32 TakeSpellDamage()
 *  Purpose : Calls Battle Utils to calculate final spell damage against a foe
 *  Example : target:takeSpellDamage(caster, spell, finaldmg, attackType, damageType)
 *  Notes   : Global function of same name in bluemagic.lua, calls this member function from within
 ************************************************************************/

int32 CLuaBaseEntity::takeSpellDamage(lua_State* L)
{
    /*
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isuserdata(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isuserdata(L, 2));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 3) || !lua_isnumber(L, 3));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 3) || !lua_isnumber(L, 4));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 3) || !lua_isnumber(L, 5));

    auto*       PChar      = static_cast<CCharEntity*>(Lunar<CLuaBaseEntity>::check(L, 1)->m_PBaseEntity);
    auto*       PSpell     = Lunar<CLuaSpell>::check(L, 2)->GetSpell();
    auto        damage     = (int32)lua_tointeger(L, 3);
    ATTACK_TYPE attackType = static_cast<ATTACK_TYPE>(lua_tointeger(L, 4));
    DAMAGE_TYPE damageType = static_cast<DAMAGE_TYPE>(lua_tointeger(L, 5));

    lua_pushinteger(L, (lua_Integer)battleutils::TakeSpellDamage(static_cast<CBattleEntity*>(m_PBaseEntity), PChar, PSpell, damage, attackType, damageType));
    */
    return 1;
}

/************************************************************************
 *  Function: spawnPet()
 *  Purpose : Spawns a pet if a few correct conditions are met
 *  Example : caster:spawnPet(PET_CARBUNCLE)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::spawnPet(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    if (m_PBaseEntity->objtype == TYPE_PC)
    {
        if (!lua_isnil(L, 1) && lua_isstring(L, 1))
        {
            auto petId = lua_tointeger(L, 1);
            if (petId == PETID_HARLEQUINFRAME)
            {
                if (((CCharEntity*)m_PBaseEntity)->PAutomaton)
                {
                    petId = PETID_HARLEQUINFRAME + ((CCharEntity*)m_PBaseEntity)->PAutomaton->getFrame() - 0x20;
                }
                else
                {
                    ShowError(CL_RED "CLuaBaseEntity::spawnPet : PetID is NULL\n" CL_RESET);
                    return 0;
                }
            }
            petutils::SpawnPet((CBattleEntity*)m_PBaseEntity, (uint32)lua_tointeger(L, 1), false);
        }
        else
        {
            ShowError(CL_RED "CLuaBaseEntity::spawnPet : PetID is NULL\n" CL_RESET);
        }
    }
    else if (m_PBaseEntity->objtype == TYPE_MOB)
    {
        CMobEntity* PMob = (CMobEntity*)m_PBaseEntity;

        if (PMob->PPet == nullptr)
        {
            ShowError("lua_baseentity::spawnPet PMob (%d) trying to spawn pet but its nullptr\n", PMob->id);
            return 0;
        }

        CMobEntity* PPet = (CMobEntity*)PMob->PPet;

        // if a number is given its an avatar or elemental spawn
        if (!lua_isnil(L, 1) && lua_isstring(L, 1))
        {
            petutils::SpawnMobPet(PMob, (uint32)lua_tointeger(L, 1));
        }

        // always spawn on master
        PPet->m_SpawnPoint = nearPosition(PMob->loc.p, 2.2f, (float)M_PI);

        // setup AI
        PPet->Spawn();
    }
    return 0;
}

/************************************************************************
 *  Function: spawnTrust()
 *  Purpose : Spawns a Trust if a few correct conditions are met
 *  Example : caster:spawnTrust(TRUST_SHANTOTTO)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::spawnTrust(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC); // only PCs can spawn trusts
    if (!lua_isnil(L, 1) && lua_isstring(L, 1))
    {
        uint16 trustId = (uint16)lua_tointeger(L, 1);
        trustutils::SpawnTrust((CCharEntity*)m_PBaseEntity, trustId);
    }
    else
    {
        ShowError(CL_RED "CLuaBaseEntity::spawnTrust : TrustID is NULL\n" CL_RESET);
    }
    return 0;
}

/************************************************************************
 *  Function: clearTrusts()
 *  Purpose :
 *  Example : caster:clearTrusts()
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::clearTrusts(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC); // only PCs can spawn trusts

    static_cast<CCharEntity*>(m_PBaseEntity)->ClearTrusts();

    return 0;
}

/************************************************************************
 *  Function: getTrustID()
 *  Purpose :
 *  Example : trust:getTrustID()
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getTrustID(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_TRUST);

    lua_pushinteger(L, ((CTrustEntity*)m_PBaseEntity)->m_TrustID);
    return 1;
}

/************************************************************************
 *  Function: trustPartyMessage()
 *  Purpose :
 *  Example : mob:trustPartyMessage(message_id)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::trustPartyMessage(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_TRUST);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    auto* PTrust = static_cast<CTrustEntity*>(m_PBaseEntity);

    auto message_id = (uint32)lua_tointeger(L, 1);

    auto* PMaster = static_cast<CCharEntity*>(PTrust->PMaster);
    if (PMaster)
    {
        PMaster->ForParty([&](CBattleEntity* PMember) {
            auto* PCharMember = static_cast<CCharEntity*>(PMember);
            PCharMember->pushPacket(new CMessageCombatPacket(PTrust, PMember, message_id, 0, 711));
        });
    }

    return 0;
}

/************************************************************************
 *  Function: addSimpleGambit()
 *  Purpose :
 *  Example : trust:addSimpleGambit(target, condition, condition_arg, reaction, selector, selector_arg)
 *  Notes   : Adds a behaviour to the gambit system
 ************************************************************************/

inline int32 CLuaBaseEntity::addSimpleGambit(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_TRUST);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 3) || !lua_isnumber(L, 3));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 4) || !lua_isnumber(L, 4));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 5) || !lua_isnumber(L, 5));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 6) || !lua_isnumber(L, 6));

    using namespace gambits;

    auto target        = static_cast<G_TARGET>(lua_tointeger(L, 1));
    auto condition     = static_cast<G_CONDITION>(lua_tointeger(L, 2));
    auto condition_arg = static_cast<uint32>(lua_tointeger(L, 3));

    auto reaction     = static_cast<G_REACTION>(lua_tointeger(L, 4));
    auto selector     = static_cast<G_SELECT>(lua_tointeger(L, 5));
    auto selector_arg = static_cast<uint32>(lua_tointeger(L, 6));

    // Optional
    auto retry_delay = 0;
    if (!lua_isnil(L, 7) && lua_isnumber(L, 7))
    {
        retry_delay = (uint16)lua_tointeger(L, 7);
    }

    Gambit_t g;
    g.predicates.emplace_back(Predicate_t{ target, condition, condition_arg });
    g.actions.emplace_back(Action_t{ reaction, selector, selector_arg });
    g.retry_delay = retry_delay;

    auto* trust      = static_cast<CTrustEntity*>(m_PBaseEntity);
    auto* controller = static_cast<CTrustController*>(trust->PAI->GetController());

    controller->m_GambitsContainer->AddGambit(g);

    return 0;
}

/************************************************************************
 *  Function: addFullGambit()
 *  Purpose :
 *  Example : mob:addGambit(...)
 *  Notes   : Adds a behaviour to the gambit system
 ************************************************************************/

inline int32 CLuaBaseEntity::addFullGambit(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_TRUST);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_istable(L, 1));

    using namespace gambits;

    // ===

    Gambit_t g;

    bool gambit_error = false;

    uint32 stackTop = lua_gettop(L);

    lua_pushvalue(L, 1); // Push main table onto stack

    lua_getfield(L, 1, "predicates"); // Acts as push
    if (lua_istable(L, -1))           // Found
    {
        auto table = lua_gettop(L);
        lua_pushnil(L);
        while (lua_next(L, table) != 0)
        {
            Predicate_t new_predicate;
            auto        sub_table = lua_gettop(L);
            lua_pushnil(L);
            while (lua_next(L, sub_table) != 0)
            {
                auto key   = std::string(lua_tostring(L, -2));
                auto value = static_cast<uint32>(lua_tonumber(L, -1));
                lua_pop(L, 1);

                if (!new_predicate.parseInput(key, value))
                {
                    gambit_error = true;
                }
            }
            g.predicates.emplace_back(new_predicate);
            lua_pop(L, 1);
        }
        lua_pop(L, 1);
    }
    else // No predicates block found!
    {
        gambit_error = true;
    }

    lua_getfield(L, 1, "actions");
    if (lua_istable(L, -1))
    {
        auto table = lua_gettop(L);
        lua_pushnil(L);
        while (lua_next(L, table) != 0)
        {
            Action_t new_action;
            auto     sub_table = lua_gettop(L);
            lua_pushnil(L);
            while (lua_next(L, sub_table) != 0)
            {
                auto key   = std::string(lua_tostring(L, -2));
                auto value = static_cast<uint32>(lua_tonumber(L, -1));
                lua_pop(L, 1);

                if (!new_action.parseInput(key, (uint32)value))
                {
                    gambit_error = true;
                }
            }
            g.actions.emplace_back(new_action);
            lua_pop(L, 1);
        }
        lua_pop(L, 1);
    }
    else // No actions block found!
    {
        gambit_error = true;
    }

    lua_pop(L, 1);

    lua_pop(L, 1); // Main table

    if (gambit_error)
    {
        ShowWarning("Invalid Gambit");
    }

    lua_settop(L, stackTop);

    // ===

    auto* trust      = static_cast<CTrustEntity*>(m_PBaseEntity);
    auto* controller = static_cast<CTrustController*>(trust->PAI->GetController());

    controller->m_GambitsContainer->AddGambit(g);

    return 0;
}

/************************************************************************
 *  Function: setTrustTPSkillSettings(trigger, select)
 *  Purpose :
 *  Example : mob:setTrustTPSkillSettings(ai.tp.ASAP, ai.s.RANDOM)
 *  Notes   :
 ************************************************************************/

int32 CLuaBaseEntity::setTrustTPSkillSettings(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_TRUST);
    TPZ_DEBUG_BREAK_IF(!lua_isnumber(L, 1) || !lua_isnumber(L, 2));

    using namespace gambits;

    auto* trust      = static_cast<CTrustEntity*>(m_PBaseEntity);
    auto* controller = static_cast<CTrustController*>(trust->PAI->GetController());

    controller->m_GambitsContainer->tp_trigger = static_cast<G_TP_TRIGGER>(lua_tointeger(L, 1));
    controller->m_GambitsContainer->tp_select  = static_cast<G_SELECT>(lua_tointeger(L, 2));

    return 0;
}

/************************************************************************
 *  Function: despawnPet()
 *  Purpose : Despawns a Pet Entity
 *  Example : target:despawnPet()
 *  Notes   : Upon death or dismissal or similar
 ************************************************************************/

inline int32 CLuaBaseEntity::despawnPet(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    if (((CBattleEntity*)m_PBaseEntity)->PPet != nullptr)
    {
        petutils::DespawnPet((CBattleEntity*)m_PBaseEntity);
    }
    return 0;
}

/************************************************************************
 *  Function: isJugPet()
 *  Purpose : Returns true if the entity crawled out of a jug after birth
 *  Example : if (pet:isJugPet()) then
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::isJugPet(lua_State* L)
{
    if (m_PBaseEntity != nullptr)
    {
        if (((CBattleEntity*)m_PBaseEntity)->PPet)
        {
            lua_pushboolean(L, ((CPetEntity*)(((CBattleEntity*)m_PBaseEntity)->PPet))->getPetType() == PET_TYPE::JUG_PET);
            return 1;
        }
    }
    return 0;
}

/************************************************************************
 *  Function: hasValidJugPetItem()
 *  Purpose : Returns true if subSkill Type is of sufficient value
 *  Example : if (player:hasValidJugPetItem()) then
 *  Notes   : Solely used for determining Call Beast activation
 ************************************************************************/

inline int32 CLuaBaseEntity::hasValidJugPetItem(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    CItemWeapon* PItem = static_cast<CItemWeapon*>(static_cast<CCharEntity*>(m_PBaseEntity)->getEquip(SLOT_AMMO));

    if (PItem != nullptr && PItem->getSubSkillType() >= SUBSKILL_SHEEP && PItem->getSubSkillType() <= SUBSKILL_TOLOI)
    {
        lua_pushboolean(L, true);
        return 1;
    }
    else
    {
        lua_pushboolean(L, false);
        return 1;
    }
}

/************************************************************************
 *  Function: hasPet()
 *  Purpose : Returns true if an entity has a pet spawned
 *  Example : if (target:hasPet()) then
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::hasPet(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    CBattleEntity* PTarget = (CBattleEntity*)m_PBaseEntity;

    lua_pushboolean(L, (PTarget->PPet != nullptr && PTarget->PPet->status != STATUS_TYPE::DISAPPEAR));
    return 1;
}

/************************************************************************
 *  Function: getPet()
 *  Purpose : Returns the Entity Object of a Pet-type entity
 *  Example : local pet = getPet()
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getPet(lua_State* L)
{
    if (((CBattleEntity*)m_PBaseEntity)->PPet != nullptr)
    {
        // uint32 petid = (uint32);

        CBattleEntity* PPet = ((CBattleEntity*)m_PBaseEntity)->PPet;

        lua_getglobal(L, "CBaseEntity");
        lua_pushstring(L, "new");
        lua_gettable(L, -2);
        lua_insert(L, -2);
        lua_pushlightuserdata(L, (void*)PPet);
        lua_pcall(L, 2, 1, 0);
        return 1;
    }
    lua_pushnil(L);
    return 1;
}

/************************************************************************
 *  Function: getPetID()
 *  Purpose : Returns the Pet ID of an entity
 *  Example : local PetID = pet:getPetID()
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getPetID(lua_State* L)
{
    if (m_PBaseEntity != nullptr)
    {
        if (((CBattleEntity*)m_PBaseEntity)->PPet)
        {
            lua_pushinteger(L, ((CPetEntity*)(((CBattleEntity*)m_PBaseEntity)->PPet))->m_PetID);
            return 1;
        }
    }
    return 0;
}

/************************************************************************
 *  Function: getPetElement()
 *  Purpose : Returns the elemental affinity of a pet entity
 *  Example : pet:getPetElement()
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getPetElement(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    if (((CBattleEntity*)m_PBaseEntity)->PPet)
    {
        lua_pushinteger(L, ((CPetEntity*)((CBattleEntity*)m_PBaseEntity)->PPet)->m_Element);
    }
    else
    {
        lua_pushinteger(L, 0);
    }
    return 1;
}

/************************************************************************
 *  Function: getMaster()
 *  Purpose : Returns the Entity object for a pet's master
 *  Example : local master = pet:petMaster()
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getMaster(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC)

    if (((CBattleEntity*)m_PBaseEntity)->PMaster != nullptr)
    {
        CBaseEntity* PMaster = ((CBattleEntity*)m_PBaseEntity)->PMaster;

        lua_getglobal(L, "CBaseEntity");
        lua_pushstring(L, "new");
        lua_gettable(L, -2);
        lua_insert(L, -2);
        lua_pushlightuserdata(L, (void*)PMaster);
        lua_pcall(L, 2, 1, 0);
        return 1;
    }

    lua_pushnil(L);
    return 1;
}

/************************************************************************
 *  Function: getPetName()
 *  Purpose : Returns the string name of a pet (empty string if not a pet)
 *  Example : local hairball = pet:getPetName()
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getPetName(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    if (((CBattleEntity*)m_PBaseEntity)->PPet)
    {
        lua_pushstring(L, (((CBattleEntity*)m_PBaseEntity)->PPet)->name.c_str());
    }
    else
    {
        lua_pushstring(L, "");
    }
    return 1;
}

/************************************************************************
 *  Function: setPetName()
 *  Purpose : Passes a string to name a new pet
 *  Example : player:setPetName(PET_TYPE_WYVERN,FLUFFI_LUFFIGUS)
 *  Notes   : Updates char_pet.sql
 ************************************************************************/

inline int32 CLuaBaseEntity::setPetName(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));

    int32 n = lua_gettop(L);

    PET_TYPE petType = static_cast<PET_TYPE>(lua_tointeger(L, 1));

    if (n == 2)
    {
        uint16 value = (uint16)lua_tointeger(L, 2);

        if (petType == PET_TYPE::WYVERN)
        {
            Sql_Query(SqlHandle, "INSERT INTO char_pet SET charid = %u, wyvernid = %u ON DUPLICATE KEY UPDATE wyvernid = %u;", m_PBaseEntity->id, value, value);
        }
        else if (petType == PET_TYPE::AUTOMATON)
        {
            Sql_Query(SqlHandle, "INSERT INTO char_pet SET charid = %u, automatonid = %u ON DUPLICATE KEY UPDATE automatonid = %u;", m_PBaseEntity->id, value,
                      value);
            if (((CCharEntity*)m_PBaseEntity)->PAutomaton != nullptr)
            {
                puppetutils::LoadAutomaton((CCharEntity*)m_PBaseEntity);
            }
        }
        /*
        else if (petType == PETTYPE_ADVENTURING_FELLOW)
        {
            Sql_Query(SqlHandle, "INSERT INTO char_pet SET charid = %u, adventuringfellowid = %u ON DUPLICATE KEY UPDATE adventuringfellowid = %u;",
        m_PBaseEntity->id, value, value);
        }
        */
    }
    else if (n == 3)
    {
        if (petType == PET_TYPE::CHOCOBO)
        {
            uint32 chocoboname1 = lua_tointeger(L, 2) & 0x0000FFFF;
            uint32 chocoboname2 = (uint32)(lua_tointeger(L, 3) << 16);

            uint32 value = chocoboname1 + chocoboname2;

            Sql_Query(SqlHandle, "INSERT INTO char_pet SET charid = %u, chocoboid = %u ON DUPLICATE KEY UPDATE chocoboid = %u;", m_PBaseEntity->id, value,
                      value);
        }
    }
    return 0;
}

/************************************************************************
 *  Function: getCharmChance()
 *  Purpose : Returns decimal value of the chances of charming an Entity
 *  Example : player:getCharmChance(target, false)
 *  Notes   : Used for Guage and Maiden's Virelai
 ************************************************************************/

inline int32 CLuaBaseEntity::getCharmChance(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr || m_PBaseEntity->objtype == TYPE_NPC);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isuserdata(L, 1));

    CLuaBaseEntity* PLuaBaseEntity = nullptr;

    auto* PCharmer = static_cast<CBattleEntity*>(m_PBaseEntity);
    auto* PTarget  = static_cast<CBattleEntity*>(PLuaBaseEntity->GetBaseEntity());

    bool includeCharmAffinityAndChanceMods = true;
    if (!lua_isnil(L, 2) && lua_isboolean(L, 2))
    {
        includeCharmAffinityAndChanceMods = lua_toboolean(L, 2);
    };

    float charmChance = battleutils::GetCharmChance(PCharmer, PTarget, includeCharmAffinityAndChanceMods);
    lua_pushnumber(L, charmChance);

    return 1;
}

/************************************************************************
 *  Function: charmPet()
 *  Purpose : Attempts to charm a pet
 *  Example : player:charmPet(target)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::charmPet(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    if (m_PBaseEntity != nullptr)
    {
        if (m_PBaseEntity->objtype != TYPE_MOB)
        {
            CLuaBaseEntity* PEntity = nullptr;
            battleutils::tryToCharm((CBattleEntity*)m_PBaseEntity, (CBattleEntity*)PEntity->GetBaseEntity());
        }
    }
    return 0;
}

/************************************************************************
 *  Function: petAttack()
 *  Purpose : Engages a pet with a target
 *  Example : pet:petAttack(target)
 *  Notes   : Sic
 ************************************************************************/

inline int32 CLuaBaseEntity::petAttack(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isuserdata(L, 1));

    CLuaBaseEntity* PEntity = nullptr;
    if (((CBattleEntity*)m_PBaseEntity)->PPet != nullptr)
    {
        petutils::AttackTarget((CBattleEntity*)m_PBaseEntity, (CBattleEntity*)PEntity->GetBaseEntity());
    }
    return 0;
}

/************************************************************************
 *  Function: petAbility()
 *  Purpose : Manually inserts the use of a pet ability into the queue
 *  Example : pet:petAbility(ABILITY)
 *  Notes   : If I had to guess, it's not coded
 ************************************************************************/

inline int32 CLuaBaseEntity::petAbility(lua_State* L)
{
    return 0;
}

/************************************************************************
 *  Function: petRetreat()
 *  Purpose : Disengages a pet from battle, returns to master
 *  Example : player:petRetreat()
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::petRetreat(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    if (((CBattleEntity*)m_PBaseEntity)->PPet != nullptr)
    {
        petutils::RetreatToMaster((CBattleEntity*)m_PBaseEntity);
    }
    return 0;
}

/************************************************************************
 *  Function: familiar()
 *  Purpose : Increases the power of the entities pet
 *  Example : mob:familiar()
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::familiar(lua_State* L)
{
    if (((CBattleEntity*)m_PBaseEntity)->PPet != nullptr)
    {
        // uint32 petid = (uint32);

        CBattleEntity* PPet = ((CBattleEntity*)m_PBaseEntity)->PPet;

        petutils::Familiar(PPet);
    }

    return 0;
}

/************************************************************************
 *  Function: addPetMod()
 *  Purpose : Adds a specified mod and power to a pet
 *  Example : target:addPetMod(MOD_HP, 20)
 *  Notes   : Adds on top of existing values?
 ************************************************************************/

inline int32 CLuaBaseEntity::addPetMod(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));

    ((CBattleEntity*)m_PBaseEntity)->addPetModifier(static_cast<Mod>(lua_tointeger(L, 1)), PetModType::All, (int16)lua_tointeger(L, 2));
    return 0;
}

/************************************************************************
 *  Function: setPetMod()
 *  Purpose : Sets a specified mod and power for a pet
 *  Example : target:setPetMod(MOD_HP, 20)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::setPetMod(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));

    ((CBattleEntity*)m_PBaseEntity)->setPetModifier(static_cast<Mod>(lua_tointeger(L, 1)), PetModType::All, (int16)lua_tointeger(L, 2));
    return 0;
}

/************************************************************************
 *  Function: delPetMod()
 *  Purpose : Removes a specified mod and power from a pet
 *  Example : target:delPetMod(MOD_HP, 20)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::delPetMod(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));

    ((CBattleEntity*)m_PBaseEntity)->delPetModifier(static_cast<Mod>(lua_tointeger(L, 1)), PetModType::All, (int16)lua_tointeger(L, 2));
    return 0;
}

/************************************************************************
 *  Function: hasAttachment()
 *  Purpose : Returns true if PC has attachment
 *  Example : if (player: hasAttachment()) then
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::hasAttachment(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, -1) || !lua_isnumber(L, -1));

    auto itemID = (uint16)lua_tointeger(L, -1);

    CItem* PItem = itemutils::GetItem(itemID);
    lua_pushboolean(L, puppetutils::HasAttachment((CCharEntity*)m_PBaseEntity, PItem));
    return 1;
}

/************************************************************************
 *  Function: getAutomatonName()
 *  Purpose : Returns the string name of the automation pet
 *  Example : local name = pet:getAutomatonName()
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getAutomatonName(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);
    const char* Query = "SELECT name FROM "
                        "char_pet LEFT JOIN pet_name ON automatonid = id "
                        "WHERE charid = %u;";

    int32 ret = Sql_Query(SqlHandle, Query, m_PBaseEntity->id);

    if (ret != SQL_ERROR && Sql_NumRows(SqlHandle) != 0 && Sql_NextRow(SqlHandle) == SQL_SUCCESS)
    {
        lua_pushstring(L, (const char*)Sql_GetData(SqlHandle, 0));
        return 1;
    }
    return 0;
}

/************************************************************************
 *  Function: getAutomatonFrame()
 *  Purpose : Returns the integer value of frame being used for automation
 *  Example : local frame = pet:getAutomatonFrame()
 *  Notes   :
 ************************************************************************/

int32 CLuaBaseEntity::getAutomatonFrame(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PET || static_cast<CPetEntity*>(m_PBaseEntity)->getPetType() != PET_TYPE::AUTOMATON);

    lua_pushinteger(L, static_cast<CAutomatonEntity*>(m_PBaseEntity)->getFrame());

    return 1;
}

/************************************************************************
 *  Function: getAutomatonHead()
 *  Purpose : Returns the integer value of the (active?) automation head
 *  Example : local head = pet:getAutomatonHead()
 *  Notes   : Currently unscripted
 ************************************************************************/

int32 CLuaBaseEntity::getAutomatonHead(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PET || static_cast<CPetEntity*>(m_PBaseEntity)->getPetType() != PET_TYPE::AUTOMATON);

    lua_pushinteger(L, static_cast<CAutomatonEntity*>(m_PBaseEntity)->getHead());

    return 1;
}

/************************************************************************
 *  Function: unlockAttachment()
 *  Purpose : Makes new attachment frames available to the Puppetmaster
 *  Example : player:unlockAttachment(8224) -- Harlequin Frame
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::unlockAttachment(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, -1) || !lua_isnumber(L, -1));

    auto itemID = (uint16)lua_tointeger(L, -1);

    CItem* PItem = itemutils::GetItem(itemID);
    lua_pushboolean(L, puppetutils::UnlockAttachment((CCharEntity*)m_PBaseEntity, PItem));
    return 1;
}

/************************************************************************
 *  Function: getActiveManeuvers()
 *  Purpose : Get the amount of active maneuvers for an automation
 *  Example : if (target:getActiveManeuvers() == 3) then
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getActiveManeuvers(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    CBattleEntity* PEntity = (CBattleEntity*)m_PBaseEntity;

    lua_pushinteger(L, PEntity->StatusEffectContainer->GetActiveManeuvers());

    return 1;
}

/************************************************************************
 *  Function: removeOldestManeuver()
 *  Purpose : Removes the oldest maneuver in an automation set (FIFO)
 *  Example : target:removeOldestManeuver()
 *  Notes   : Often used if (target:getActiveManeuvers() == 3)
 ************************************************************************/

inline int32 CLuaBaseEntity::removeOldestManeuver(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    CBattleEntity* PEntity = (CBattleEntity*)m_PBaseEntity;

    PEntity->StatusEffectContainer->RemoveOldestManeuver();

    return 0;
}

/************************************************************************
 *  Function: removeAllManeuvers()
 *  Purpose : Removes all maneuvers from an automation
 *  Example : target:removeAllManeuvers()
 *  Notes   : Often used if (overload ~= 0)
 ************************************************************************/

inline int32 CLuaBaseEntity::removeAllManeuvers(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    CBattleEntity* PEntity = (CBattleEntity*)m_PBaseEntity;

    PEntity->StatusEffectContainer->RemoveAllManeuvers();

    return 0;
}

/************************************************************************
 *  Function: updateAttachments()
 *  Purpose : Updates all of the attachments
 *  Example : master:updateAttachments()
 *  Notes   : Called when Optic Fiber has changed.
 ************************************************************************/

inline int32 CLuaBaseEntity::updateAttachments(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    CCharEntity* PEntity = (CCharEntity*)m_PBaseEntity;

    puppetutils::UpdateAttachments(PEntity);

    return 0;
}

/************************************************************************
 *  Function: setMobLevel()
 *  Purpose : Updates the monsters level and recalculates stats
 *  Example : mob:setMobLevel(125)
 *  Notes   : CalculateStats will refill mobs hp/mp as well
 ************************************************************************/

inline int32 CLuaBaseEntity::setMobLevel(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_MOB);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    if (auto* PMob = dynamic_cast<CMobEntity*>(m_PBaseEntity))
    {
        PMob->SetMLevel((uint8)lua_tointeger(L, 1));
        mobutils::CalculateStats(PMob);
        mobutils::GetAvailableSpells(PMob);
    }

    return 0;
}

/************************************************************************
 *  Function: getSystem()
 *  Purpose : Returns integer value of system associated with an Entity
 *  Example : if (pet:getSystem() ~= 5) then -- Not an avatar
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getSystem(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    uint8 system = static_cast<uint8>(((CBattleEntity*)m_PBaseEntity)->m_EcoSystem);

    lua_pushinteger(L, system);
    return 1;
}

/************************************************************************
 *  Function: getFamily()
 *  Purpose : Returns the integer value of the associated Mob Family
 *  Example : if (mob:getFamily() == 123) then
 *  Notes   : To Do: Enumerate Mob Families in global script
 ************************************************************************/

inline int32 CLuaBaseEntity::getFamily(lua_State* L)
{
    auto* entity = dynamic_cast<CMobEntity*>(m_PBaseEntity);
    TPZ_DEBUG_BREAK_IF(!entity);

    uint16 family = entity->m_Family;

    lua_pushinteger(L, family);
    return 1;
}

/************************************************************************
 *  Function: isMobType()
 *  Purpose : Returns true if a Mob is of a specified type (if !Mob->false)
 *  Example : if (mob:isMobType(MOBTYPE_NOTORIOUS)
 *  Notes   : Oddly, this is only being used to check if Mob is NM...?
 *  Notes   : To Do: This isn't the intended function for NM checks...
 ************************************************************************/

inline int32 CLuaBaseEntity::isMobType(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    if (m_PBaseEntity->objtype != TYPE_MOB)
    {
        lua_pushboolean(L, false);
        return 1;
    }

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    CMobEntity* PMob = (CMobEntity*)m_PBaseEntity;

    lua_pushboolean(L, PMob->m_Type & lua_tointeger(L, 1));
    return 1;
}

/************************************************************************
 *  Function: isUndead()
 *  Purpose : Returns true if Entity is Undead
 *  Example : if (target:isUndead()) then
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::isUndead(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    lua_pushboolean(L, ((CBattleEntity*)m_PBaseEntity)->m_EcoSystem == ECOSYSTEM::UNDEAD);
    return 1;
}

/************************************************************************
 *  Function: isNM()
 *  Purpose : Returns true if Mob is a Notorious Monster
 *  Example : if (mob:isNM())
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::isNM(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    if (m_PBaseEntity->objtype == TYPE_MOB && ((CMobEntity*)m_PBaseEntity)->m_Type & MOBTYPE_NOTORIOUS)
    {
        lua_pushboolean(L, true);
    }
    else
    {
        lua_pushboolean(L, false);
    }
    return 1;
}

/************************************************************************
 *  Function: getModelSize()
 *  Purpose : Returns the Model Size of the entity
 *  Example : local size = mob:getModelSize()
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getModelSize(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    CBattleEntity* PEntity = (CBattleEntity*)m_PBaseEntity;

    lua_pushinteger(L, PEntity->m_ModelSize);
    return 1;
}

/************************************************************************
 *  Function: setMobFlags()
 *  Purpose : Manually set Mob flags
 *  Example : Not in use in scripts
 *  Notes   : Currently only used through !setmobflags command
 ************************************************************************/

inline int32 CLuaBaseEntity::setMobFlags(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    if (!lua_isnil(L, 2) && lua_isnumber(L, 2))
    {
        uint32      mobid = (uint32)lua_tointeger(L, 2);
        CMobEntity* PMob  = nullptr;

        if (!lua_isnil(L, 2) && lua_isuserdata(L, 2))
        {
            CLuaInstance* PLuaInstance = nullptr;
            PMob                       = (CMobEntity*)PLuaInstance->GetInstance()->GetEntity(mobid & 0xFFF, TYPE_MOB);
        }
        else
        {
            PMob = (CMobEntity*)zoneutils::GetEntity(mobid, TYPE_MOB);
        }

        if (PMob != nullptr)
        {
            if (!lua_isnil(L, 1) && lua_isnumber(L, 1))
            {
                ((CMobEntity*)PMob)->setEntityFlags((uint32)(lua_tointeger(L, 1)));
                PMob->updatemask |= UPDATE_HP;
            }
        }
    }
    else
    {
        CCharEntity*   PChar   = (CCharEntity*)m_PBaseEntity;
        CBattleEntity* PTarget = (CBattleEntity*)PChar->GetEntity(PChar->m_TargID);

        if (PTarget == nullptr)
        {
            ShowError("Must target a monster to use for setMobFlags \n");
            return 0;
        }
        else if (PTarget->objtype != TYPE_MOB)
        {
            ShowError("Battle target must be a monster to use setMobFlags \n");
            return 0;
        }
        if (!lua_isnil(L, 1) && lua_isnumber(L, 1))
        {
            ((CMobEntity*)PTarget)->setEntityFlags((uint32)(lua_tointeger(L, 1)));
            PTarget->updatemask |= UPDATE_HP;
        }
    }
    return 0;
}

/************************************************************************
 *  Function: getMobFlags()
 *  Purpose : Get Mob flags
 *  Example : Not in use in scripts
 *  Notes   : Currently only used through !getMobFlags command
 ************************************************************************/
inline int32 CLuaBaseEntity::getMobFlags(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_MOB);

    if (auto* PMob = dynamic_cast<CMobEntity*>(m_PBaseEntity))
    {
        lua_pushinteger(L, PMob->getEntityFlags());
        return 1;
    }

    return 0;
}

/************************************************************************
 *  Function: spawn()
 *  Purpose : Forces a mob to spawn with optional Despawn/Respawn values
 *  Example : mob:spawn(60,3600); mob:spawn()
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::spawn(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_MOB);

    CMobEntity* PMob = (CMobEntity*)m_PBaseEntity;

    if (!lua_isnil(L, 1) && lua_isnumber(L, 1))
    {
        PMob->SetDespawnTime(std::chrono::seconds(lua_tointeger(L, 1)));
    }

    if (!lua_isnil(L, 2) && lua_isnumber(L, 2))
    {
        PMob->m_RespawnTime  = (uint32)lua_tointeger(L, 2) * 1000;
        PMob->m_AllowRespawn = true;
    }
    else
    {
        if (!PMob->PAI->IsSpawned())
        {
            PMob->Spawn();
        }
        else
        {
            ShowDebug(CL_CYAN "SpawnMob: %u <%s> is already spawned\n" CL_RESET, PMob->id, PMob->GetName());
        }
    }
    return 0;
}

/************************************************************************
 *  Function: isSpawned()
 *  Purpose : Returns true if a Mob is already spawned
 *  Example : if (mob:isSpawned())
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::isSpawned(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_MOB);

    lua_pushboolean(L, static_cast<CMobEntity*>(m_PBaseEntity)->PAI->IsSpawned());

    return 1;
}

/************************************************************************
 *  Function: getSpawnPos()
 *  Purpose : Returns the spawn position for a Mob in a Lua table
 *  Example : local spawn = mob:getSpawnPos()
 *  Notes   : x = spawn.x; y = spawn.y, etc
 ************************************************************************/

inline int32 CLuaBaseEntity::getSpawnPos(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(!(m_PBaseEntity->objtype & TYPE_MOB));

    CMobEntity* PMob = (CMobEntity*)m_PBaseEntity;

    lua_createtable(L, 4, 0);
    int8 newTable = lua_gettop(L);

    lua_pushnumber(L, PMob->m_SpawnPoint.x);
    lua_setfield(L, newTable, "x");

    lua_pushnumber(L, PMob->m_SpawnPoint.y);
    lua_setfield(L, newTable, "y");

    lua_pushnumber(L, PMob->m_SpawnPoint.z);
    lua_setfield(L, newTable, "z");

    lua_pushnumber(L, PMob->m_SpawnPoint.rotation);
    lua_setfield(L, newTable, "rot");

    return 1;
}

/************************************************************************
 *  Function: setSpawn()
 *  Purpose : Manually set the next spawn position for a Mob
 *  Example : mob:setSpawn(-100,243,0,123)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::setSpawn(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_MOB);

    CMobEntity* PMob = (CMobEntity*)m_PBaseEntity;

    if (lua_isnumber(L, 1))
    {
        if (!lua_isnil(L, 1) && lua_isnumber(L, 1))
        {
            PMob->m_SpawnPoint.x = (float)lua_tonumber(L, 1);
        }
        if (!lua_isnil(L, 2) && lua_isnumber(L, 2))
        {
            PMob->m_SpawnPoint.y = (float)lua_tonumber(L, 2);
        }
        if (!lua_isnil(L, 3) && lua_isnumber(L, 3))
        {
            PMob->m_SpawnPoint.z = (float)lua_tonumber(L, 3);
        }
        if (!lua_isnil(L, 4) && lua_isnumber(L, 4))
        {
            PMob->m_SpawnPoint.rotation = (uint8)lua_tointeger(L, 4);
        }
    }
    else
    {
        // its a table
        lua_rawgeti(L, 1, 1);
        PMob->m_SpawnPoint.x = (float)lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_rawgeti(L, 1, 2);
        PMob->m_SpawnPoint.y = (float)lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_rawgeti(L, 1, 3);
        PMob->m_SpawnPoint.z = (float)lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_rawgeti(L, 1, 4);
        PMob->m_SpawnPoint.rotation = (uint8)lua_tointeger(L, -1);
        lua_pop(L, 1);
    }

    return 0;
}

/************************************************************************
 *  Function: getRespawnTime()
 *  Purpose : Returns the remaining respawn time for a Mob
 *  Example : if (nm:getRespawnTime() == 0)
 *  Notes   : Used in mobs.lua...and directly in Charybdis
 ************************************************************************/

int32 CLuaBaseEntity::getRespawnTime(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_MOB);

    CMobEntity* PMob = static_cast<CMobEntity*>(m_PBaseEntity);

    if (PMob->m_AllowRespawn)
    {
        lua_pushinteger(L, PMob->m_RespawnTime);
        return 1;
    }
    else
    {
        lua_pushinteger(L, 0);
        return 1;
    }
}

/************************************************************************
 *  Function: setRespawnTime()
 *  Purpose : Setting the respawn time for a Mob
 *  Example : mob:setRespawnTime(math.random(3600, 7200))
 *  Notes   : Haven't seen the second argument option being used
 ************************************************************************/

inline int32 CLuaBaseEntity::setRespawnTime(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_MOB);

    CMobEntity* PMob = (CMobEntity*)m_PBaseEntity;

    if (!lua_isnil(L, 1) && lua_isnumber(L, 1))
    {
        PMob->m_RespawnTime = (uint32)(lua_tointeger(L, 1) * 1000);
        if (PMob->PAI->IsCurrentState<CRespawnState>())
        {
            PMob->PAI->GetCurrentState()->ResetEntryTime();
        }

        if (!lua_isnil(L, 2) && lua_isboolean(L, 2) && lua_toboolean(L, 2))
        { // set optional parameter to true to only modify the timer
            return 0;
        }
    }
    else
    {
        ShowWarning("CLuaBaseEntity::setRespawnTime (%d) Tried to set respawn without a time\n", PMob->id);
    }

    PMob->m_AllowRespawn = true;

    return 0;
}

/************************************************************************
 *  Function: instantiateMob()
 *  Purpose : Used for spawning a new mob - is this for Monstrosity prep?
 *  Example : None available
 *  Notes   : Not currently implemented
 ************************************************************************/

inline int32 CLuaBaseEntity::instantiateMob(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    CMobEntity* newMob = mobutils::InstantiateAlly((uint32)lua_tointeger(L, 1), m_PBaseEntity->getZone());

    newMob->loc.p        = m_PBaseEntity->loc.p;
    newMob->m_SpawnPoint = newMob->loc.p;
    newMob->Spawn();

    return 0;
}

/************************************************************************
 *  Function: hasTrait()
 *  Purpose : Returns true if a Mob has an active trait
 *  Example : if (target:hasTrait(15)) -- Double Attack
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::hasTrait(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    if (m_PBaseEntity->objtype != TYPE_PC)
    {
        lua_pushboolean(L, false);
        return 1;
    }

    lua_pushboolean(L, charutils::hasTrait((CCharEntity*)m_PBaseEntity, (uint8)lua_tointeger(L, 1)));
    return 1;
}

/************************************************************************
 *  Function: hasImmunity()
 *  Purpose : Returns true if a Mob is immune to a specified type of spell
 *  Example : if (target:hasImmunity(64)) then
 *  Notes   : Arguments are dec to bin, so powers of 2 (max 256) -- Listed in mobentity.h
 ************************************************************************/

inline int32 CLuaBaseEntity::hasImmunity(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    lua_pushboolean(L, ((CBattleEntity*)m_PBaseEntity)->hasImmunity((uint32)lua_tointeger(L, 1)));
    return 1;
}

/************************************************************************
 *  Function: setAggressive()
 *  Purpose : Toggle a Mob to an aggressive or passive state
 *  Example : mob:setAggressive(1)
 *  Notes   : Different integer values to specify method of aggro?
 ************************************************************************/

inline int32 CLuaBaseEntity::setAggressive(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_MOB);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    ((CMobEntity*)m_PBaseEntity)->m_Aggro = lua_tointeger(L, -1);

    return 0;
}

/************************************************************************
 *  Function: setTrueDetection()
 *  Purpose : Toggle True Detection on or off for a Mob
 *  Example : mob:setTrueDetection(1)
 *  Notes   : Different integer values for True Hearing/Sight?
 ************************************************************************/

inline int32 CLuaBaseEntity::setTrueDetection(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_MOB);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    ((CMobEntity*)m_PBaseEntity)->m_TrueDetection = lua_tointeger(L, -1);

    return 0;
}

/************************************************************************
 *  Function: setUnkillable()
 *  Purpose : Set a Mob to unkillable or not
 *  Example : mob:setUnkillable(true)
 *  Notes   :
 ************************************************************************/

void CLuaBaseEntity::setUnkillable(bool unkillable)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    dynamic_cast<CBattleEntity*>(m_PBaseEntity)->m_unkillable = unkillable;
}

/************************************************************************
 *  Function: untargetable()
 *  Purpose : Returns true if a Mob or NPC is untargetable
 *  Example : if (target:untargetable()) then
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::untargetable(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isboolean(L, 1));
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_MOB && m_PBaseEntity->objtype != TYPE_NPC);

    if (m_PBaseEntity->objtype == TYPE_MOB)
    {
        ((CMobEntity*)m_PBaseEntity)->Untargetable(lua_toboolean(L, 1));
    }
    else if (m_PBaseEntity->objtype == TYPE_NPC)
    {
        ((CNpcEntity*)m_PBaseEntity)->Untargetable(lua_toboolean(L, 1));
    }
    m_PBaseEntity->updatemask |= UPDATE_HP;
    return 0;
}

/************************************************************************
 *  Function: setDelay()
 *  Purpose : Override default delay settings for a Mob
 *  Example : mob:setDelay(2400)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::setDelay(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_MOB);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    ((CItemWeapon*)((CMobEntity*)m_PBaseEntity)->m_Weapons[SLOT_MAIN])->setDelay((uint16)lua_tonumber(L, 1));
    return 0;
}

/************************************************************************
 *  Function: setDamage()
 *  Purpose : Override default damage settings for a Mob
 *  Example : mob:setDamage(40)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::setDamage(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_MOB);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    ((CItemWeapon*)((CMobEntity*)m_PBaseEntity)->m_Weapons[SLOT_MAIN])->setDamage((uint16)lua_tonumber(L, 1));
    return 0;
}

/************************************************************************
 *  Function: hasSpellList()
 *  Purpose : Returns true if a Mob has spells to cast
 *  Example : if (mob:hasSpellList()) then
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::hasSpellList(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_MOB && m_PBaseEntity->objtype != TYPE_PET);

    lua_pushboolean(L, (static_cast<CMobEntity*>(m_PBaseEntity))->SpellContainer->HasSpells());

    return 1;
}

/************************************************************************
 *  Function: setSpellList()
 *  Purpose : Specify a spell list for a Mob to use
 *  Example : mob:setSpellList(118 + DayOfTheWeek)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::setSpellList(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_MOB && m_PBaseEntity->objtype != TYPE_PET);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    mobutils::SetSpellList(static_cast<CMobEntity*>(m_PBaseEntity), (uint16)lua_tonumber(L, 1));

    return 0;
}

/************************************************************************
 *  Function: SetAutoAttackEnabled()
 *  Purpose : Enables/disabled auto-attack for a Mob
 *  Example : mob:SetAutoAttackEnabled(false)
 *  Notes   : See scripts/zones/Throne_Room/mobs/Shadow_Lord.lua
 ************************************************************************/

inline int32 CLuaBaseEntity::SetAutoAttackEnabled(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isboolean(L, 1));

    m_PBaseEntity->PAI->GetController()->SetAutoAttackEnabled(lua_toboolean(L, 1));

    return 0;
}

/************************************************************************
 *  Function: SetMagicCastingEnabled()
 *  Purpose : Used to enable/disable the casting of spells for a Mob
 *  Example : mob:SetMagicCastingEnabled(false)
 *  Notes   : Used primarily for Mob behavior and battle control
 ************************************************************************/

inline int32 CLuaBaseEntity::SetMagicCastingEnabled(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isboolean(L, 1));

    m_PBaseEntity->PAI->GetController()->SetMagicCastingEnabled(lua_toboolean(L, 1));

    return 0;
}

/************************************************************************
 *  Function: SetMobAbilityEnabled()
 *  Purpose : Used primarily to clear queue for special ability to be used
 *  Example : mob:SetMobAbilityEnabled(false)
 *  Notes   : See Bahamut.lua
 ************************************************************************/

inline int32 CLuaBaseEntity::SetMobAbilityEnabled(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isboolean(L, 1));

    m_PBaseEntity->PAI->GetController()->SetWeaponSkillEnabled(lua_toboolean(L, 1));

    return 0;
}

/************************************************************************
 *  Function: SetMobSkillAttack()
 *  Purpose : Used mainly so Mobs don't respawn in flight?
 *  Example : mob:SetMobSkillAttack(0)
 *  Notes   : Used in Ouryu, Jormungand, Tiamat, etc
 ************************************************************************/

inline int32 CLuaBaseEntity::SetMobSkillAttack(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_MOB);

    static_cast<CMobEntity*>(m_PBaseEntity)->setMobMod(MOBMOD_ATTACK_SKILL_LIST, (int16)lua_tointeger(L, 1));

    return 0;
}

/************************************************************************
 *  Function: getMobMod()
 *  Purpose : Returns the power value of a Mob Mod in effect
 *  Example : mob:getMobMod(tpz.mobMod.MUG_GIL)
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getMobMod(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(!(m_PBaseEntity->objtype & TYPE_MOB));

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    lua_pushinteger(L, ((CMobEntity*)m_PBaseEntity)->getMobMod((uint16)lua_tointeger(L, 1)));
    return 1;
}

/************************************************************************
 *  Function: addMobMod()
 *  Purpose : Applies a Mob Mod with a specified amount
 *  Example : mob:addMobMod(tpz.mobMod.MUG_GIL, 100)
 *  Notes   : Currently not being used in any script
 ************************************************************************/

inline int32 CLuaBaseEntity::addMobMod(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    // putting this in here to find elusive bug
    if (!(m_PBaseEntity->objtype & TYPE_MOB))
    {
        // this once broke on an entity (17532673) but it could not be found
        ShowError("CLuaBaseEntity::addMobMod Expected type mob (%d) but its a (%d)\n", m_PBaseEntity->id, m_PBaseEntity->objtype);
        return 0;
    }

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));

    ((CMobEntity*)m_PBaseEntity)->addMobMod((uint16)lua_tointeger(L, 1), (int16)lua_tointeger(L, 2));
    return 0;
}

/************************************************************************
 *  Function: setMobMod()
 *  Purpose : Applies a Mob Mod of a specified magnitude
 *  Example : mob:setMobMod(tpz.mobMod.MUG_GIL, 100)
 *  Notes   : Interesting note - this is being used for superlinking too
 ************************************************************************/

inline int32 CLuaBaseEntity::setMobMod(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    // putting this in here to find elusive bug
    if (!(m_PBaseEntity->objtype & TYPE_MOB))
    {
        // this once broke on an entity (17532673) but it could not be found
        ShowError("CLuaBaseEntity::setMobMod Expected type mob (%d) but its a (%d)\n", m_PBaseEntity->id, m_PBaseEntity->objtype);
        return 0;
    }

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));

    ((CMobEntity*)m_PBaseEntity)->setMobMod((uint16)lua_tointeger(L, 1), (int16)lua_tointeger(L, 2));
    return 0;
}

/************************************************************************
 *  Function: delMobMod()
 *  Purpose : Removes a Mob Mod
 *  Example : mob:delMobMod(tpz.mobMod.MUG_GIL, 100)
 *  Notes   : Currently not being used in any script
 ************************************************************************/

inline int32 CLuaBaseEntity::delMobMod(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    // putting this in here to find elusive bug
    if (!(m_PBaseEntity->objtype & TYPE_MOB))
    {
        // this once broke on an entity (17532673) but it could not be found
        ShowError("CLuaBaseEntity::addMobMod Expected type mob (%d) but its a (%d)\n", m_PBaseEntity->id, m_PBaseEntity->objtype);
        return 0;
    }

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 2) || !lua_isnumber(L, 2));

    ((CMobEntity*)m_PBaseEntity)->addMobMod((uint16)lua_tointeger(L, 1), (int16)(-lua_tointeger(L, 2)));
    return 0;
}

/************************************************************************
 *  Function: getBattleTime()
 *  Purpose : Returns the time the Mob has been engaged in seconds
 *  Example : if (mob:getBattleTime() == 3600) then -- 1 Hour
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getBattleTime(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    lua_pushinteger(L, (lua_Integer)std::chrono::duration_cast<std::chrono::seconds>(((CBattleEntity*)m_PBaseEntity)->GetBattleTime()).count());
    return 1;
}

/************************************************************************
 *  Function: getBehaviour()
 *  Purpose : Returns the current Mob behavior
 *  Example : mob:getBehaviour()
 *  Notes   : Currently used in bitwise calculations for high-tier NM's
 ************************************************************************/

inline int32 CLuaBaseEntity::getBehaviour(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_MOB);

    lua_pushinteger(L, ((CMobEntity*)m_PBaseEntity)->m_Behaviour);

    return 1;
}

/************************************************************************
 *  Function: setBehaviour()
 *  Purpose : Sets a particular behavior for a Mob
 *  Example : mob:setBehaviour(bit.bor(mob:getBehaviour(), BEHAVIOUR_NO_TURN))
 *  Notes   : Currently used in bitwise calculations for high-tier NM's
 ************************************************************************/

inline int32 CLuaBaseEntity::setBehaviour(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_MOB);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    ((CMobEntity*)m_PBaseEntity)->m_Behaviour = (uint16)lua_tointeger(L, 1);

    return 0;
}

/************************************************************************
 *  Function: getTarget()
 *  Purpose : Return available targets as a Lua table to the Mob
 *  Example : mob:getTarget(); pet:getTarget(); if not v:getTarget() then
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getTarget(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    auto* PBattleTarget{ m_PBaseEntity->GetEntity(static_cast<CBattleEntity*>(m_PBaseEntity)->GetBattleTargetID()) };
    if (PBattleTarget)
    {
        lua_getglobal(L, "CBaseEntity");
        lua_pushstring(L, "new");
        lua_gettable(L, -2);
        lua_insert(L, -2);
        lua_pushlightuserdata(L, PBattleTarget);
        lua_pcall(L, 2, 1, 0);
        return 1;
    }
    else
    {
        lua_pushnil(L);
        return 1;
    }
}

/************************************************************************
 *  Function: updateTarget()
 *  Purpose : Direct's Mob's attention to the PC with highest Enmity
 *  Example : mob:updateTarget()
 *  Notes   : Not in scripts, but can be called to force an Enmity check?
 ************************************************************************/

inline int32 CLuaBaseEntity::updateTarget(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_MOB);

    auto* PTarget{ ((CMobEntity*)m_PBaseEntity)->PEnmityContainer->GetHighestEnmity() };

    if (PTarget)
    {
        ((CMobEntity*)m_PBaseEntity)->PAI->ChangeTarget(PTarget->targid);
    }

    return 0;
}

/************************************************************************
 *  Function: getEnmityList()
 *  Purpose : Returns a Lua table list of PC's with active enmity
 *  Example : local targets = mob:getEnmityList()
 *  Notes   : Used in Tame and special Mob abilities
 ************************************************************************/

inline int32 CLuaBaseEntity::getEnmityList(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_MOB);

    EnmityList_t* enmityList = ((CMobEntity*)m_PBaseEntity)->PEnmityContainer->GetEnmityList();

    if (enmityList)
    {
        lua_createtable(L, (int)enmityList->size(), 0);
        int i = 1;
        for (auto member : *enmityList)
        {
            if (member.second.PEnmityOwner)
            {
                lua_createtable(L, 0, 4);
                // push entity
                lua_getglobal(L, "CBaseEntity");
                lua_pushstring(L, "new");
                lua_gettable(L, -2);
                lua_insert(L, -2);
                lua_pushlightuserdata(L, (void*)member.second.PEnmityOwner);
                lua_pcall(L, 2, 1, 0);
                lua_setfield(L, -2, "entity");
                // push ce
                lua_pushinteger(L, member.second.CE);
                lua_setfield(L, -2, "ce");
                // push ve
                lua_pushinteger(L, member.second.VE);
                lua_setfield(L, -2, "ve");
                // push active
                lua_pushboolean(L, member.second.active);
                lua_setfield(L, -2, "active");

                // push tameable
                lua_pushboolean(L, ((CMobEntity*)m_PBaseEntity)->PEnmityContainer->IsTameable());
                lua_setfield(L, -2, "tameable");

                // assign table to key's value
                lua_rawseti(L, -2, i++);
            }
        }
    }
    else
    {
        lua_pushnil(L);
    }
    return 1;
}

/************************************************************************
 *  Function: getTrickAttackChar()
 *  Purpose : Returns the character who is eligible for Trick Attack to be applied against
 *  Example : local taChar = player:getTrickAttackChar(target)
 *  Notes   : For some reason, only used in Jump/High-Jump?
 ************************************************************************/

inline int32 CLuaBaseEntity::getTrickAttackChar(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);

    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isuserdata(L, 1));

    CLuaBaseEntity* PLuaBaseEntity = nullptr;
    CBattleEntity*  PMob           = (CBattleEntity*)PLuaBaseEntity->GetBaseEntity();
    if (PMob != nullptr)
    {
        CBattleEntity* taTarget = battleutils::getAvailableTrickAttackChar((CBattleEntity*)m_PBaseEntity, PMob);
        if (taTarget)
        {
            lua_getglobal(L, "CBaseEntity");
            lua_pushstring(L, "new");
            lua_gettable(L, -2);
            lua_insert(L, -2);
            lua_pushlightuserdata(L, taTarget);
            lua_pcall(L, 2, 1, 0);
            return 1;
        }
    }
    lua_pushnil(L);
    return 1;
}

/************************************************************************
 *  Function: actionQueueEmpty()
 *  Purpose : Returns true if a Mob's action queue is empty
 *  Example : if (mob:actionQueueEmpty() == true) then
 *  Notes   : See: scripts/zones/Temenos/mobs/Proto-Ultima.lua
 ************************************************************************/

inline int32 CLuaBaseEntity::actionQueueEmpty(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    lua_pushboolean(L, m_PBaseEntity->PAI->QueueEmpty());

    return 1;
}

/************************************************************************
 *  Function: castSpell()
 *  Purpose : Prompts an NPC or Mob entity to cast a specified spell
 *  Example : mob:castSpell(spell)
 *  Notes   : Currently only used by a few select mobs
 ************************************************************************/

inline int32 CLuaBaseEntity::castSpell(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    if (lua_isnumber(L, 1))
    {
        SpellID spellid = static_cast<SpellID>(lua_tointeger(L, 1));
        uint16  targid  = 0;

        if (!lua_isnil(L, 2) && lua_isuserdata(L, 2))
        {
            CLuaBaseEntity* PLuaBaseEntity = nullptr;
            if (PLuaBaseEntity->m_PBaseEntity)
            {
                targid = PLuaBaseEntity->m_PBaseEntity->targid;
            }
        }

        m_PBaseEntity->PAI->QueueAction(queueAction_t(0ms, true, [targid, spellid](auto PEntity) {
            if (targid)
            {
                PEntity->PAI->Cast(targid, spellid);
            }
            else if (dynamic_cast<CMobEntity*>(PEntity))
            {
                PEntity->PAI->Cast(static_cast<CMobEntity*>(PEntity)->GetBattleTargetID(), spellid);
            }
        }));
    }
    else
    {
        m_PBaseEntity->PAI->QueueAction(queueAction_t(0ms, true, [](auto PEntity) {
            if (dynamic_cast<CMobEntity*>(PEntity))
            {
                static_cast<CMobController*>(PEntity->PAI->GetController())->TryCastSpell();
            }
        }));
    }
    return 0;
}

/************************************************************************
 *  Function: useJobAbility()
 *  Purpose : Instruct a Mob to use a specified Job Ability
 *  Example : wyvern:useJobAbility(636, wyvern) -- Specifying pet to use
 *  Notes   : Inserts directly into queue stack with 0ms delay
 ************************************************************************/

inline int32 CLuaBaseEntity::useJobAbility(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    if (lua_isnumber(L, 1))
    {
        auto           skillid{ (uint16)lua_tointeger(L, 1) };
        CBattleEntity* PTarget{ nullptr };

        if (!lua_isnil(L, 2) && lua_isuserdata(L, 2))
        {
            CLuaBaseEntity* PLuaBaseEntity = nullptr;
            PTarget                        = (CBattleEntity*)PLuaBaseEntity->m_PBaseEntity;
        }

        m_PBaseEntity->PAI->QueueAction(queueAction_t(0ms, true, [PTarget, skillid](auto PEntity) {
            if (PTarget)
            {
                PEntity->PAI->Ability(PTarget->targid, skillid);
            }
            else if (dynamic_cast<CMobEntity*>(PEntity))
            {
                PEntity->PAI->Ability(static_cast<CMobEntity*>(PEntity)->GetBattleTargetID(), skillid);
            }
        }));
    }

    return 0;
}

/************************************************************************
 *  Function: useMobAbility()
 *  Purpose : Uses a specified Mob Ability or the next one ready in the que
 *  Example : automation:useMobAbility(2132, automation) --Specifying pet
 *  Notes   : Use single variable (Ability ID only) for base entity only
 ************************************************************************/

inline int32 CLuaBaseEntity::useMobAbility(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_TRUST && m_PBaseEntity->objtype != TYPE_MOB && m_PBaseEntity->objtype != TYPE_PET);

    if (lua_isnumber(L, 1))
    {
        auto           skillid{ (uint16)lua_tointeger(L, 1) };
        CBattleEntity* PTarget{ nullptr };
        auto*          PMobSkill{ battleutils::GetMobSkill(skillid) };

        if (!PMobSkill)
        {
            return 0;
        }

        if (!lua_isnil(L, 2) && lua_isuserdata(L, 2))
        {
            CLuaBaseEntity* PLuaBaseEntity = nullptr;
            PTarget                        = (CBattleEntity*)PLuaBaseEntity->m_PBaseEntity;
        }

        m_PBaseEntity->PAI->QueueAction(queueAction_t(0ms, true, [PTarget, skillid, PMobSkill](auto PEntity) {
            if (PTarget)
            {
                PEntity->PAI->MobSkill(PTarget->targid, skillid);
            }
            else if (dynamic_cast<CMobEntity*>(PEntity))
            {
                if (PMobSkill->getValidTargets() & TARGET_ENEMY)
                {
                    PEntity->PAI->MobSkill(static_cast<CMobEntity*>(PEntity)->GetBattleTargetID(), skillid);
                }
                else if (PMobSkill->getValidTargets() & TARGET_SELF)
                {
                    PEntity->PAI->MobSkill(PEntity->targid, skillid);
                }
            }
        }));
    }
    else
    {
        m_PBaseEntity->PAI->QueueAction(queueAction_t(0ms, true, [](auto PEntity) {
            if (dynamic_cast<CMobEntity*>(PEntity))
            {
                static_cast<CMobController*>(PEntity->PAI->GetController())->MobSkill();
            }
        }));
    };

    return 0;
}

/************************************************************************
 *  Function: hasTPMoves()
 *  Purpose : Returns true if a Mob has TP moves in its skill list
 *  Example : if (mob:hasTPMoves()) then
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::hasTPMoves(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF((m_PBaseEntity->objtype == TYPE_NPC) || (m_PBaseEntity->objtype == TYPE_PC));

    uint16 familyID = 0;

    if (m_PBaseEntity->objtype & TYPE_PET)
    {
        familyID = ((CPetEntity*)m_PBaseEntity)->m_Family;
    }
    else if (m_PBaseEntity->objtype & TYPE_MOB)
    {
        familyID = ((CMobEntity*)m_PBaseEntity)->m_Family;
    }
    const std::vector<uint16>& MobSkills = battleutils::GetMobSkillList(familyID);
    lua_pushboolean(L, !MobSkills.empty());
    return 1;
}

/************************************************************************
 *  Function: weaknessTrigger()
 *  Purpose : Triggers the weakness of a mob to an active state
 *  Example : mob:weaknessTrigger(1)
 *  Notes   : Used in scripts/mixins/abyssea_nm.lua
 ************************************************************************/

inline int32 CLuaBaseEntity::weaknessTrigger(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_MOB);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    mobutils::WeaknessTrigger(m_PBaseEntity, (WeaknessType)lua_tointeger(L, 1));

    return 0;
}

/************************************************************************
 *  Function: hasPreventActionEffect()
 *  Purpose : Returns true if a non-NPC entity has a preventative status effect
 *  Example : if (not pet:hasPreventActionEffect()) then
 *  Notes   : Used in scripts/globals/abilities/stay.lua
 ************************************************************************/

inline int32 CLuaBaseEntity::hasPreventActionEffect(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype == TYPE_NPC);

    lua_pushboolean(L, (static_cast<CBattleEntity*>(m_PBaseEntity))->StatusEffectContainer->HasPreventActionEffect());

    return 1;
}

/************************************************************************
 *  Function: stun()
 *  Purpose : Stuns a mob for a specified amount of time (in ms)
 *  Example : mob:stun(5000) -- Stun for 5 seconds
 *  Notes   : To Do: Change to seconds for standardization?
 ************************************************************************/

inline int32 CLuaBaseEntity::stun(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    m_PBaseEntity->PAI->Inactive(std::chrono::milliseconds(lua_tointeger(L, 1)), false);

    return 0;
}

/************************************************************************
 *  Function: getPool()
 *  Purpose : Returns a Mob's Pool ID integer
 *  Example : if (mob:getPool() = 4006) then
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getPool(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);

    if (m_PBaseEntity->objtype != TYPE_MOB)
    {
        lua_pushnil(L);
    }
    else
    {
        CMobEntity* PMob = (CMobEntity*)m_PBaseEntity;
        lua_pushinteger(L, PMob->m_Pool);
    }

    return 1;
}

/************************************************************************
 *  Function: getDropID()
 *  Purpose : Returns the integer Drop ID assigned to a Mob
 *  Example : local DropID = mob:getDropID()
 *  Notes   :
 ************************************************************************/

int32 CLuaBaseEntity::getDropID(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_MOB);

    lua_pushinteger(L, static_cast<CMobEntity*>(m_PBaseEntity)->m_DropID);
    return 1;
}

/************************************************************************
 *  Function: setDropID()
 *  Purpose : Permanently changes the Drop ID assigned to a Mob
 *  Example : target:setDropID(2408)
 *  Notes   : Useful for situations where drops only occur from conditions
 ************************************************************************/

int32 CLuaBaseEntity::setDropID(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_MOB);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    static_cast<CMobEntity*>(m_PBaseEntity)->m_DropID = (uint32)lua_tointeger(L, 1);
    return 0;
}

/************************************************************************
 *  Function: addTreasure()
 *  Purpose : Manually adds treasure to a party's treasure pool
 *  Example : targ:addTreasure(itemId, dropper)
 ************************************************************************/

inline int32 CLuaBaseEntity::addTreasure(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_PC);
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    CCharEntity* PChar = (CCharEntity*)m_PBaseEntity;

    if (PChar->PTreasurePool != nullptr)
    {
        if (!lua_isnil(L, 2) && lua_isuserdata(L, 2))
        {
            uint16 droprate = 1000;
            if (!lua_isnil(L, 3) && lua_isnumber(L, 3))
            {
                droprate = (uint16)lua_tointeger(L, 3);
            }
            // The specified PEntity can be a Mob or NPC
            CLuaBaseEntity* PLuaBaseEntity = nullptr;
            CBaseEntity*    PEntity        = PLuaBaseEntity->GetBaseEntity();
            charutils::DistributeItem(PChar, PEntity, (uint16)lua_tointeger(L, 1), droprate);
        }
        else // Entity can be nullptr - this is intentional
        {
            uint16 droprate = 1000;
            if (!lua_isnil(L, 2) && lua_isnumber(L, 1))
            {
                droprate = (uint16)lua_tointeger(L, 2);
            }
            charutils::DistributeItem(PChar, nullptr, (uint16)lua_tointeger(L, 1), droprate);
        }
    }

    return 0;
}

/************************************************************************
 *  Function: getStealItem()
 *  Purpose : Used to return the Item ID of a mob's item which can be stolen
 *  Example : steamItem = target:getStealItem()
 *  Notes   : Used only in Thief quest and Maat
 ************************************************************************/

inline int32 CLuaBaseEntity::getStealItem(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_MOB);

    CMobEntity* PMob = static_cast<CMobEntity*>(m_PBaseEntity);
    if (PMob)
    {
        DropList_t* PDropList = itemutils::GetDropList(PMob->m_DropID);

        if (PDropList && !PMob->m_ItemStolen)
        {
            for (const DropItem_t& drop : PDropList->Items)
            {
                if (drop.DropType == DROP_STEAL)
                {
                    lua_pushinteger(L, drop.ItemID);
                    return 1;
                }
            }
        }
    }

    lua_pushinteger(L, 0);
    return 1;
}

/************************************************************************
 *  Function: getDespoilItem()
 *  Purpose : Used to return the Item ID of a mob's item which can be despoiled
 *  Example : despoilItem = target:getDespoilItem()
 *  Notes   : Defaults to getStealItem() if no despoil item exists
 ************************************************************************/

inline int32 CLuaBaseEntity::getDespoilItem(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_MOB);

    CMobEntity* PMob = static_cast<CMobEntity*>(m_PBaseEntity);
    if (PMob)
    {
        DropList_t* PDropList = itemutils::GetDropList(PMob->m_DropID);
        if (PDropList && !PMob->m_ItemStolen)
        {
            for (const DropItem_t& drop : PDropList->Items)
            {
                if (drop.DropType == DROP_DESPOIL)
                {
                    lua_pushinteger(L, drop.ItemID);
                    return 1;
                }
            }
        }
    }

    return getStealItem(L);
}

/************************************************************************
 *  Function: getDespoilDebuff()
 *  Purpose : Used to get a status effect id to apply to a mob on successful despoil
 *  Example : effect = player:getDespoilDebuff()
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getDespoilDebuff(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(lua_isnil(L, 1) || !lua_isnumber(L, 1));

    uint16 effectId = luautils::GetDespoilDebuff((uint16)lua_tointeger(L, 1));
    if (effectId > 0)
    {
        lua_pushinteger(L, effectId);
    }
    else
    {
        lua_pushnil(L);
    }

    return 1;
}

/************************************************************************
 *  Function: itemStolen()
 *  Purpose : Flags a mob's item as stolen, returns true upon update
 *  Example : target:itemStolen()
 *  Notes   : Used in scripts/globals/abilities/steal.lua
 ************************************************************************/

inline int32 CLuaBaseEntity::itemStolen(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_MOB);

    ((CMobEntity*)m_PBaseEntity)->m_ItemStolen = true;
    lua_pushboolean(L, 1);
    return 1;
}

/************************************************************************
 *  Function: getTHlevel()
 *  Purpose : Return mob's current Treasure Hunter tier if alive, or its last if dead.
 *  Example : local TH = target:getTHlevel()
 *  Notes   :
 ************************************************************************/

inline int32 CLuaBaseEntity::getTHlevel(lua_State* L)
{
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity == nullptr);
    TPZ_DEBUG_BREAK_IF(m_PBaseEntity->objtype != TYPE_MOB);

    CMobEntity* PMob = (CMobEntity*)m_PBaseEntity;
    lua_pushinteger(L, PMob->isDead() ? PMob->m_THLvl : PMob->PEnmityContainer->GetHighestTH());
    return 1;
}

//==========================================================//

void CLuaBaseEntity::Register()
{
    SOL_USERTYPE("CBaseEntity", CLuaBaseEntity);

    // Messaging System
    // SOL_REGISTER("showText", CLuaBaseEntity::showText);
    // SOL_REGISTER(messageText),
    SOL_REGISTER("PrintToPlayer", CLuaBaseEntity::PrintToPlayer);
    // SOL_REGISTER(PrintToArea),
    // SOL_REGISTER(messageBasic),
    // SOL_REGISTER(messageName),
    // SOL_REGISTER(messagePublic),
    // SOL_REGISTER(messageSpecial),
    // SOL_REGISTER(messageSystem),
    // SOL_REGISTER(messageCombat),

    // Variables
    SOL_REGISTER("getCharVar", CLuaBaseEntity::getCharVar);
    SOL_REGISTER("setCharVar", CLuaBaseEntity::setCharVar);
    SOL_REGISTER("addCharVar", CLuaBaseEntity::addCharVar);
    SOL_REGISTER("getLocalVar", CLuaBaseEntity::getLocalVar);
    SOL_REGISTER("setLocalVar", CLuaBaseEntity::setLocalVar);
    SOL_REGISTER("resetLocalVars", CLuaBaseEntity::resetLocalVars);
    SOL_REGISTER("getLastOnline", CLuaBaseEntity::getLastOnline);

    // Packets, Events, and Flags
    // SOL_REGISTER(injectPacket),
    // SOL_REGISTER(injectActionPacket),
    // SOL_REGISTER(entityVisualPacket),
    // SOL_REGISTER(entityAnimationPacket),

    // SOL_REGISTER(startEvent),
    // SOL_REGISTER(startEventString),
    // SOL_REGISTER(updateEvent),
    // SOL_REGISTER(updateEventString),
    // SOL_REGISTER(getEventTarget),
    // SOL_REGISTER(release),

    // SOL_REGISTER(setFlag),
    // SOL_REGISTER(moghouseFlag),
    // SOL_REGISTER(needToZone),

    // Object Identification
    SOL_REGISTER("getID", CLuaBaseEntity::getID);
    SOL_REGISTER("getShortID", CLuaBaseEntity::getShortID);
    SOL_REGISTER("getCursorTarget", CLuaBaseEntity::getCursorTarget);

    SOL_REGISTER("getObjType", CLuaBaseEntity::getObjType);
    SOL_REGISTER("isPC", CLuaBaseEntity::isPC);
    SOL_REGISTER("isNPC", CLuaBaseEntity::isNPC);
    SOL_REGISTER("isMob", CLuaBaseEntity::isMob);
    SOL_REGISTER("isPet", CLuaBaseEntity::isPet);
    SOL_REGISTER("isAlly", CLuaBaseEntity::isAlly);

    // AI and Control
    SOL_REGISTER("initNpcAi", CLuaBaseEntity::initNpcAi);
    SOL_REGISTER("resetAI", CLuaBaseEntity::resetAI);
    SOL_REGISTER("getStatus", CLuaBaseEntity::getStatus);
    SOL_REGISTER("setStatus", CLuaBaseEntity::setStatus);
    // SOL_REGISTER(getCurrentAction),

    // SOL_REGISTER(lookAt),
    // SOL_REGISTER(clearTargID),

    // SOL_REGISTER(atPoint),
    // SOL_REGISTER(pathTo),
    // SOL_REGISTER(pathThrough),
    // SOL_REGISTER(isFollowingPath),
    // SOL_REGISTER(clearPath),
    // SOL_REGISTER(checkDistance),
    // SOL_REGISTER(wait),

    SOL_REGISTER("openDoor", CLuaBaseEntity::openDoor);
    SOL_REGISTER("closeDoor", CLuaBaseEntity::closeDoor);
    SOL_REGISTER("setElevator", CLuaBaseEntity::setElevator);

    SOL_REGISTER("addPeriodicTrigger", CLuaBaseEntity::addPeriodicTrigger);
    // SOL_REGISTER(showNPC),
    // SOL_REGISTER(hideNPC),
    // SOL_REGISTER(updateNPCHideTime),

    // SOL_REGISTER(getWeather),
    // SOL_REGISTER(setWeather),

    // PC Instructions
    // SOL_REGISTER(ChangeMusic),
    // SOL_REGISTER(sendMenu),
    // SOL_REGISTER(sendGuild),
    // SOL_REGISTER(openSendBox),
    SOL_REGISTER("leaveGame", CLuaBaseEntity::leaveGame),
        // SOL_REGISTER(sendEmote),

        // Location and Positioning
        // SOL_REGISTER(getWorldAngle),
        // SOL_REGISTER(getFacingAngle),
        // SOL_REGISTER(isFacing),
        // SOL_REGISTER(isInfront),
        // SOL_REGISTER(isBehind),
        // SOL_REGISTER(isBeside),

        // SOL_REGISTER(getZone),
        // SOL_REGISTER(getZoneID),
        // SOL_REGISTER(getZoneName),
        // SOL_REGISTER(isZoneVisited),
        // SOL_REGISTER(getPreviousZone),
        // SOL_REGISTER(getCurrentRegion),
        // SOL_REGISTER(getContinentID),
        // SOL_REGISTER(isInMogHouse),

        // SOL_REGISTER(getPos),
        // SOL_REGISTER(showPosition),
        // SOL_REGISTER(getXPos),
        // SOL_REGISTER(getYPos),
        // SOL_REGISTER(getZPos),
        // SOL_REGISTER(getRotPos),
        // SOL_REGISTER(setPos),

        // SOL_REGISTER(warp),
        // SOL_REGISTER(teleport),
        // SOL_REGISTER(addTeleport),
        // SOL_REGISTER(getTeleport),
        // SOL_REGISTER(hasTeleport),
        // SOL_REGISTER(setTeleportMenu),
        // SOL_REGISTER(getTeleportMenu),
        // SOL_REGISTER(setHomePoint),
        // SOL_REGISTER(resetPlayer),

        // SOL_REGISTER(goToEntity),
        // SOL_REGISTER(gotoPlayer),
        // SOL_REGISTER(bringPlayer),

        // Items
        SOL_REGISTER("getEquipID", CLuaBaseEntity::getEquipID);
    // SOL_REGISTER(getEquippedItem),
    // SOL_REGISTER(hasItem),
    // SOL_REGISTER(addItem),
    // SOL_REGISTER(delItem),
    // SOL_REGISTER(addUsedItem),
    // SOL_REGISTER(addTempItem),
    // SOL_REGISTER(hasWornItem),
    // SOL_REGISTER(createWornItem),

    // SOL_REGISTER(createShop),
    // SOL_REGISTER(addShopItem),
    // SOL_REGISTER(getCurrentGPItem),
    // SOL_REGISTER(breakLinkshell),

    // Trading
    // SOL_REGISTER(getContainerSize),
    // SOL_REGISTER(changeContainerSize),
    // SOL_REGISTER(getFreeSlotsCount),
    // SOL_REGISTER(confirmTrade),
    // SOL_REGISTER(tradeComplete),

    // Equipping
    // SOL_REGISTER(canEquipItem),
    // SOL_REGISTER(equipItem),
    // SOL_REGISTER(unequipItem),

    // SOL_REGISTER(setEquipBlock),
    // SOL_REGISTER(lockEquipSlot),
    // SOL_REGISTER(unlockEquipSlot),

    // SOL_REGISTER(getShieldSize),

    SOL_REGISTER("hasGearSetMod", CLuaBaseEntity::hasGearSetMod);
    SOL_REGISTER("addGearSetMod", CLuaBaseEntity::addGearSetMod);
    SOL_REGISTER("clearGearSetMods", CLuaBaseEntity::clearGearSetMods);

    // Storing
    // SOL_REGISTER(getStorageItem),
    // SOL_REGISTER(storeWithPorterMoogle),
    // SOL_REGISTER(getRetrievableItemsForSlip),
    // SOL_REGISTER(retrieveItemFromSlip),

    // Player Appearance
    SOL_REGISTER("getRace", CLuaBaseEntity::getRace);
    SOL_REGISTER("getGender", CLuaBaseEntity::getGender);
    SOL_REGISTER("getName", CLuaBaseEntity::getName);
    SOL_REGISTER("hideName", CLuaBaseEntity::hideName);
    SOL_REGISTER("checkNameFlags", CLuaBaseEntity::checkNameFlags);
    SOL_REGISTER("getModelId", CLuaBaseEntity::getModelId);
    SOL_REGISTER("setModelId", CLuaBaseEntity::setModelId);
    SOL_REGISTER("setCostume", CLuaBaseEntity::setCostume);
    SOL_REGISTER("getCostume", CLuaBaseEntity::getCostume);
    // SOL_REGISTER(costume2),

    // SOL_REGISTER(getAnimation),
    // SOL_REGISTER(setAnimation),
    // SOL_REGISTER(AnimationSub),

    // Player Status
    SOL_REGISTER("getNation", CLuaBaseEntity::getNation);
    SOL_REGISTER("setNation", CLuaBaseEntity::setNation);
    SOL_REGISTER("getAllegiance", CLuaBaseEntity::getAllegiance);
    SOL_REGISTER("setAllegiance", CLuaBaseEntity::setAllegiance);
    SOL_REGISTER("getCampaignAllegiance", CLuaBaseEntity::getCampaignAllegiance);
    SOL_REGISTER("setCampaignAllegiance", CLuaBaseEntity::setCampaignAllegiance);

    SOL_REGISTER("isSeekingParty", CLuaBaseEntity::isSeekingParty);
    SOL_REGISTER("getNewPlayer", CLuaBaseEntity::getNewPlayer);
    // SOL_REGISTER(setNewPlayer),
    // SOL_REGISTER(getMentor),
    // SOL_REGISTER(setMentor),

    // SOL_REGISTER(getGMLevel),
    // SOL_REGISTER(setGMLevel),
    // SOL_REGISTER(getGMHidden),
    // SOL_REGISTER(setGMHidden),

    // SOL_REGISTER(isJailed),
    // SOL_REGISTER(jail),

    // SOL_REGISTER(canUseMisc),

    // SOL_REGISTER(speed),

    // SOL_REGISTER(getPlaytime),
    // SOL_REGISTER(getTimeCreated),

    // Player Jobs and Levels
    SOL_REGISTER("getMainJob", CLuaBaseEntity::getMainJob);
    SOL_REGISTER("getSubJob", CLuaBaseEntity::getSubJob);
    SOL_REGISTER("changeJob", CLuaBaseEntity::changeJob);
    SOL_REGISTER("changesJob", CLuaBaseEntity::changesJob);
    SOL_REGISTER("unlockJob", CLuaBaseEntity::unlockJob);
    SOL_REGISTER("hasJob", CLuaBaseEntity::hasJob);

    SOL_REGISTER("getMainLvl", CLuaBaseEntity::getMainLvl);
    SOL_REGISTER("getSubLvl", CLuaBaseEntity::getSubLvl);
    SOL_REGISTER("getJobLevel", CLuaBaseEntity::getJobLevel);
    SOL_REGISTER("setLevel", CLuaBaseEntity::setLevel);
    // SOL_REGISTER(setsLevel),
    // SOL_REGISTER(levelCap),
    // SOL_REGISTER(levelRestriction),
    // SOL_REGISTER(addJobTraits),

    // Player Titles and Fame
    SOL_REGISTER("getTitle", CLuaBaseEntity::getTitle);
    SOL_REGISTER("hasTitle", CLuaBaseEntity::hasTitle);
    SOL_REGISTER("addTitle", CLuaBaseEntity::addTitle);
    SOL_REGISTER("setTitle", CLuaBaseEntity::setTitle);
    SOL_REGISTER("delTitle", CLuaBaseEntity::delTitle);

    // SOL_REGISTER(getFame),
    // SOL_REGISTER(addFame),
    // SOL_REGISTER(setFame),
    // SOL_REGISTER(getFameLevel),

    SOL_REGISTER("getRank", CLuaBaseEntity::getRank);
    SOL_REGISTER("setRank", CLuaBaseEntity::setRank);
    SOL_REGISTER("getRankPoints", CLuaBaseEntity::getRankPoints);
    SOL_REGISTER("addRankPoints", CLuaBaseEntity::addRankPoints);
    SOL_REGISTER("setRankPoints", CLuaBaseEntity::setRankPoints);

    // SOL_REGISTER(addQuest),
    // SOL_REGISTER(delQuest),
    // SOL_REGISTER(getQuestStatus),
    // SOL_REGISTER(hasCompletedQuest),
    // SOL_REGISTER(completeQuest),

    // SOL_REGISTER(addMission),
    // SOL_REGISTER(delMission),
    // SOL_REGISTER(getCurrentMission),
    // SOL_REGISTER(hasCompletedMission),
    // SOL_REGISTER(completeMission),
    // SOL_REGISTER(setMissionLogEx),
    // SOL_REGISTER(getMissionLogEx),
    // SOL_REGISTER(getEminenceCompleted),
    // SOL_REGISTER(setEminenceCompleted),
    // SOL_REGISTER(getEminenceProgress),
    // SOL_REGISTER(setEminenceProgress),

    // SOL_REGISTER(addAssault),
    // SOL_REGISTER(delAssault),
    // SOL_REGISTER(getCurrentAssault),
    // SOL_REGISTER(hasCompletedAssault),
    // SOL_REGISTER(completeAssault),

    // SOL_REGISTER(addKeyItem),
    // SOL_REGISTER(delKeyItem),
    // SOL_REGISTER(hasKeyItem),
    // SOL_REGISTER(seenKeyItem),
    // SOL_REGISTER(unseenKeyItem),

    // Player Points
    SOL_REGISTER("addExp", CLuaBaseEntity::addExp);
    SOL_REGISTER("delExp", CLuaBaseEntity::delExp);
    SOL_REGISTER("getMerit", CLuaBaseEntity::getMerit);
    SOL_REGISTER("getMeritCount", CLuaBaseEntity::getMeritCount);
    SOL_REGISTER("setMerits", CLuaBaseEntity::setMerits);

    SOL_REGISTER("getGil", CLuaBaseEntity::getGil);
    SOL_REGISTER("addGil", CLuaBaseEntity::addGil);
    SOL_REGISTER("setGil", CLuaBaseEntity::setGil);
    SOL_REGISTER("delGil", CLuaBaseEntity::delGil);

    SOL_REGISTER("getCP", CLuaBaseEntity::getCP);
    SOL_REGISTER("addCP", CLuaBaseEntity::addCP);
    SOL_REGISTER("delCP", CLuaBaseEntity::delCP);

    SOL_REGISTER("getSeals", CLuaBaseEntity::getSeals);
    SOL_REGISTER("addSeals", CLuaBaseEntity::addSeals);
    SOL_REGISTER("delSeals", CLuaBaseEntity::delSeals);

    SOL_REGISTER("getCurrency", CLuaBaseEntity::getCurrency);
    SOL_REGISTER("addCurrency", CLuaBaseEntity::addCurrency);
    SOL_REGISTER("setCurrency", CLuaBaseEntity::setCurrency);
    SOL_REGISTER("delCurrency", CLuaBaseEntity::delCurrency);

    SOL_REGISTER("getAssaultPoint", CLuaBaseEntity::getAssaultPoint);
    SOL_REGISTER("addAssaultPoint", CLuaBaseEntity::addAssaultPoint);
    SOL_REGISTER("delAssaultPoint", CLuaBaseEntity::delAssaultPoint);

    SOL_REGISTER("addGuildPoints", CLuaBaseEntity::addGuildPoints);

    // Health and Status
    SOL_REGISTER("getHP", CLuaBaseEntity::getHP);
    SOL_REGISTER("getHPP", CLuaBaseEntity::getHPP);
    SOL_REGISTER("getMaxHP", CLuaBaseEntity::getMaxHP);
    SOL_REGISTER("getBaseHP", CLuaBaseEntity::getBaseHP);
    SOL_REGISTER("addHP", CLuaBaseEntity::addHP);
    SOL_REGISTER("setHP", CLuaBaseEntity::setHP);
    SOL_REGISTER("restoreHP", CLuaBaseEntity::restoreHP);
    SOL_REGISTER("delHP", CLuaBaseEntity::delHP);
    // SOL_REGISTER(takeDamage),
    // SOL_REGISTER(hideHP),

    SOL_REGISTER("getMP", CLuaBaseEntity::getMP);
    // Got an MPP? Well, I then you don't know me... (+1)
    SOL_REGISTER("getMaxMP", CLuaBaseEntity::getMaxMP);
    SOL_REGISTER("getBaseMP", CLuaBaseEntity::getBaseMP);
    SOL_REGISTER("addMP", CLuaBaseEntity::addMP);
    SOL_REGISTER("setMP", CLuaBaseEntity::setMP);
    SOL_REGISTER("restoreMP", CLuaBaseEntity::restoreMP);
    SOL_REGISTER("delMP", CLuaBaseEntity::delMP);


    SOL_REGISTER("getTP", CLuaBaseEntity::getTP);
    SOL_REGISTER("addTP", CLuaBaseEntity::addTP);
    SOL_REGISTER("setTP", CLuaBaseEntity::setTP);
    SOL_REGISTER("delTP", CLuaBaseEntity::delTP);

    SOL_REGISTER("updateHealth", CLuaBaseEntity::updateHealth);

    // Skills and Abilities
    SOL_REGISTER("capSkill", CLuaBaseEntity::capSkill);
    SOL_REGISTER("capAllSkills", CLuaBaseEntity::capAllSkills);
    SOL_REGISTER("getSkillLevel", CLuaBaseEntity::getSkillLevel);
    SOL_REGISTER("setSkillLevel", CLuaBaseEntity::setSkillLevel);
    SOL_REGISTER("getMaxSkillLevel", CLuaBaseEntity::getMaxSkillLevel);
    SOL_REGISTER("getSkillRank", CLuaBaseEntity::getSkillRank);
    SOL_REGISTER("setSkillRank", CLuaBaseEntity::setSkillRank);
    SOL_REGISTER("getCharSkillLevel", CLuaBaseEntity::getCharSkillLevel);

    SOL_REGISTER("addLearnedWeaponskill", CLuaBaseEntity::addLearnedWeaponskill);
    SOL_REGISTER("hasLearnedWeaponskill", CLuaBaseEntity::hasLearnedWeaponskill);
    SOL_REGISTER("delLearnedWeaponskill", CLuaBaseEntity::delLearnedWeaponskill);

    SOL_REGISTER("addWeaponSkillPoints", CLuaBaseEntity::addWeaponSkillPoints);

    // SOL_REGISTER(addLearnedAbility),
    // SOL_REGISTER(hasLearnedAbility),
    // SOL_REGISTER(canLearnAbility),
    // SOL_REGISTER(delLearnedAbility),

    // SOL_REGISTER(addSpell),
    // SOL_REGISTER(hasSpell),
    // SOL_REGISTER(canLearnSpell),
    // SOL_REGISTER(delSpell),

    // SOL_REGISTER(recalculateSkillsTable),
    // SOL_REGISTER(recalculateAbilitiesTable),

    // Parties and Alliances
    // SOL_REGISTER(getParty),
    // SOL_REGISTER(getPartyWithTrusts),
    // SOL_REGISTER(getPartySize),
    // SOL_REGISTER(hasPartyJob),
    // SOL_REGISTER(getPartyMember),
    // SOL_REGISTER(getPartyLeader),
    // SOL_REGISTER(getLeaderID),
    // SOL_REGISTER(getPartyLastMemberJoinedTime),
    // SOL_REGISTER(forMembersInRange),

    // SOL_REGISTER(addPartyEffect),
    // SOL_REGISTER(hasPartyEffect),
    // SOL_REGISTER(removePartyEffect),

    // SOL_REGISTER(getAlliance),
    // SOL_REGISTER(getAllianceSize),

    // SOL_REGISTER(reloadParty),
    // SOL_REGISTER(disableLevelSync),
    // SOL_REGISTER(isLevelSync),

    // SOL_REGISTER(checkSoloPartyAlliance),

    // SOL_REGISTER(checkKillCredit),

    // Instances
    // SOL_REGISTER(getInstance),
    // SOL_REGISTER(setInstance),
    // SOL_REGISTER(createInstance),
    // SOL_REGISTER(instanceEntry),

    // SOL_REGISTER(getConfrontationEffect),
    // SOL_REGISTER(copyConfrontationEffect),

    // Battlefields
    // SOL_REGISTER(getBattlefield),
    // SOL_REGISTER(getBattlefieldID),
    // SOL_REGISTER(registerBattlefield),
    // SOL_REGISTER(battlefieldAtCapacity),
    // SOL_REGISTER(enterBattlefield),
    // SOL_REGISTER(leaveBattlefield),
    // SOL_REGISTER(isInDynamis),

    //// Battle Utilities
    SOL_REGISTER("isAlive", CLuaBaseEntity::isAlive);
    SOL_REGISTER("isDead", CLuaBaseEntity::isDead);

    SOL_REGISTER("sendRaise", CLuaBaseEntity::sendRaise);
    SOL_REGISTER("sendReraise", CLuaBaseEntity::sendReraise);
    SOL_REGISTER("sendTractor", CLuaBaseEntity::sendTractor);

    // SOL_REGISTER(messageCombat),
    // SOL_REGISTER(countdown),
    // SOL_REGISTER(enableEntities),
    // SOL_REGISTER(independantAnimation),

    // SOL_REGISTER(engage),
    // SOL_REGISTER(isEngaged),
    // SOL_REGISTER(disengage),
    // SOL_REGISTER(timer),
    // SOL_REGISTER(queue),
    // SOL_REGISTER(addRecast),
    // SOL_REGISTER(hasRecast),
    // SOL_REGISTER(resetRecast),
    // SOL_REGISTER(resetRecasts),

    // SOL_REGISTER(addListener),
    // SOL_REGISTER(removeListener),
    // SOL_REGISTER(triggerListener),

    // SOL_REGISTER(getEntity),
    // SOL_REGISTER(getNearbyEntities),
    // SOL_REGISTER(canChangeState),

    // SOL_REGISTER(wakeUp),

    // SOL_REGISTER(recalculateStats),
    // SOL_REGISTER(checkImbuedItems),

    // SOL_REGISTER(isDualWielding),

    // Enmity
    // SOL_REGISTER(getCE),
    // SOL_REGISTER(getVE),
    // SOL_REGISTER(setCE),
    // SOL_REGISTER(setVE),
    // SOL_REGISTER(addEnmity),
    // SOL_REGISTER(lowerEnmity),
    // SOL_REGISTER(updateEnmity),
    // SOL_REGISTER(transferEnmity),
    // SOL_REGISTER(updateEnmityFromDamage),
    // SOL_REGISTER(updateEnmityFromCure),
    // SOL_REGISTER(resetEnmity),
    // SOL_REGISTER(updateClaim),
    // SOL_REGISTER(hasEnmity),
    // SOL_REGISTER(getNotorietyList),

    // Status Effects
    SOL_REGISTER("addStatusEffect", CLuaBaseEntity::addStatusEffect);
    SOL_REGISTER("addStatusEffectEx", CLuaBaseEntity::addStatusEffectEx);
    SOL_REGISTER("getStatusEffect", CLuaBaseEntity::getStatusEffect);
    SOL_REGISTER("getStatusEffects", CLuaBaseEntity::getStatusEffects);
    SOL_REGISTER("getStatusEffectElement", CLuaBaseEntity::getStatusEffectElement);
    SOL_REGISTER("canGainStatusEffect", CLuaBaseEntity::canGainStatusEffect);
    SOL_REGISTER("hasStatusEffect", CLuaBaseEntity::hasStatusEffect);
    SOL_REGISTER("hasStatusEffectByFlag", CLuaBaseEntity::hasStatusEffectByFlag);
    SOL_REGISTER("countEffect", CLuaBaseEntity::countEffect);

    SOL_REGISTER("delStatusEffect", CLuaBaseEntity::delStatusEffect);
    SOL_REGISTER("delStatusEffectsByFlag", CLuaBaseEntity::delStatusEffectsByFlag);
    SOL_REGISTER("delStatusEffectSilent", CLuaBaseEntity::delStatusEffectSilent);
    SOL_REGISTER("eraseStatusEffect", CLuaBaseEntity::eraseStatusEffect);
    SOL_REGISTER("eraseAllStatusEffect", CLuaBaseEntity::eraseAllStatusEffect);
    SOL_REGISTER("dispelStatusEffect", CLuaBaseEntity::dispelStatusEffect);
    SOL_REGISTER("dispelAllStatusEffect", CLuaBaseEntity::dispelAllStatusEffect);
    SOL_REGISTER("stealStatusEffect", CLuaBaseEntity::stealStatusEffect);

    SOL_REGISTER("addMod", CLuaBaseEntity::addMod);
    SOL_REGISTER("getMod", CLuaBaseEntity::getMod);
    SOL_REGISTER("setMod", CLuaBaseEntity::setMod);
    SOL_REGISTER("delMod", CLuaBaseEntity::delMod);

    SOL_REGISTER("delMaddLatentod", CLuaBaseEntity::addLatent);
    SOL_REGISTER("delLatent", CLuaBaseEntity::delLatent);

    // COR
    // SOL_REGISTER(fold),
    // SOL_REGISTER(doWildCard),
    // SOL_REGISTER(addCorsairRoll),
    // SOL_REGISTER(hasCorsairEffect),
    // SOL_REGISTER(hasBustEffect),
    // SOL_REGISTER(numBustEffects),
    // SOL_REGISTER(healingWaltz),
    // SOL_REGISTER(addBardSong),

    // BST
    // SOL_REGISTER(charm),
    // SOL_REGISTER(uncharm),

    // PUP
    // SOL_REGISTER(addBurden),
    // SOL_REGISTER(setStatDebilitation),

    // Damage Calculation
    SOL_REGISTER("getStat", CLuaBaseEntity::getStat);
    SOL_REGISTER("getACC", CLuaBaseEntity::getACC);
    SOL_REGISTER("getEVA", CLuaBaseEntity::getEVA);
    SOL_REGISTER("getRACC", CLuaBaseEntity::getRACC);
    SOL_REGISTER("getRATT", CLuaBaseEntity::getRATT);
    SOL_REGISTER("getILvlMacc", CLuaBaseEntity::getILvlMacc);

    SOL_REGISTER("isSpellAoE", CLuaBaseEntity::isSpellAoE);

    SOL_REGISTER("physicalDmgTaken", CLuaBaseEntity::physicalDmgTaken);
    // SOL_REGISTER(magicDmgTaken),
    SOL_REGISTER("rangedDmgTaken", CLuaBaseEntity::rangedDmgTaken);
    SOL_REGISTER("breathDmgTaken", CLuaBaseEntity::breathDmgTaken);
    SOL_REGISTER("handleAfflatusMiseryDamage", CLuaBaseEntity::handleAfflatusMiseryDamage);

    SOL_REGISTER("isWeaponTwoHanded", CLuaBaseEntity::isWeaponTwoHanded);
    // SOL_REGISTER(getMeleeHitDamage),
    // SOL_REGISTER(getWeaponDmg),
    // SOL_REGISTER(getWeaponDmgRank),
    // SOL_REGISTER(getOffhandDmg),
    // SOL_REGISTER(getOffhandDmgRank),
    // SOL_REGISTER(getRangedDmg),
    // SOL_REGISTER(getRangedDmgRank),
    // SOL_REGISTER(getAmmoDmg),

    // SOL_REGISTER(removeAmmo),

    // SOL_REGISTER(getWeaponSkillLevel),
    // SOL_REGISTER(getWeaponDamageType),
    // SOL_REGISTER(getWeaponSkillType),
    // SOL_REGISTER(getWeaponSubSkillType),
    // SOL_REGISTER(getWSSkillchainProp),

    // SOL_REGISTER(takeWeaponskillDamage),
    // SOL_REGISTER(takeSpellDamage),

    // Pets and Automations
    // SOL_REGISTER(spawnPet),
    // SOL_REGISTER(despawnPet),

    // SOL_REGISTER(isJugPet),
    // SOL_REGISTER(hasValidJugPetItem),

    // SOL_REGISTER(hasPet),
    // SOL_REGISTER(getPet),
    // SOL_REGISTER(getPetID),
    // SOL_REGISTER(getPetElement),
    // SOL_REGISTER(getMaster),

    // SOL_REGISTER(getPetName),
    // SOL_REGISTER(setPetName),

    // SOL_REGISTER(getCharmChance),
    // SOL_REGISTER(charmPet),

    // SOL_REGISTER(petAttack),
    // SOL_REGISTER(petAbility),
    // SOL_REGISTER(petRetreat),
    // SOL_REGISTER(familiar),

    // SOL_REGISTER(addPetMod),
    // SOL_REGISTER(setPetMod),
    // SOL_REGISTER(delPetMod),

    // SOL_REGISTER(hasAttachment),
    // SOL_REGISTER(getAutomatonName),
    // SOL_REGISTER(getAutomatonFrame),
    // SOL_REGISTER(getAutomatonHead),
    // SOL_REGISTER(unlockAttachment),

    // SOL_REGISTER(getActiveManeuvers),
    // SOL_REGISTER(removeOldestManeuver),
    // SOL_REGISTER(removeAllManeuvers),
    // SOL_REGISTER(updateAttachments),

    // Trust related
    // SOL_REGISTER(spawnTrust),
    // SOL_REGISTER(clearTrusts),
    // SOL_REGISTER(getTrustID),
    // SOL_REGISTER(trustPartyMessage),
    // SOL_REGISTER(addSimpleGambit),
    // SOL_REGISTER(addFullGambit),
    // SOL_REGISTER(setTrustTPSkillSettings),

    // Mob Entity-Specific
    // SOL_REGISTER(setMobLevel),
    // SOL_REGISTER(getSystem),
    // SOL_REGISTER(getFamily),
    // SOL_REGISTER(isMobType),
    // SOL_REGISTER(isUndead),
    // SOL_REGISTER(isNM),

    // SOL_REGISTER(getModelSize),
    // SOL_REGISTER(setMobFlags),
    // SOL_REGISTER(getMobFlags),

    // SOL_REGISTER(spawn),
    // SOL_REGISTER(isSpawned),
    // SOL_REGISTER(getSpawnPos),
    // SOL_REGISTER(setSpawn),
    // SOL_REGISTER(getRespawnTime),
    // SOL_REGISTER(setRespawnTime),

    // SOL_REGISTER(instantiateMob),

    // SOL_REGISTER(hasTrait),
    // SOL_REGISTER(hasImmunity),

    // SOL_REGISTER(setAggressive),
    // SOL_REGISTER(setTrueDetection),
    SOL_REGISTER("setUnkillable", CLuaBaseEntity::setUnkillable);
    // SOL_REGISTER(untargetable),

    // SOL_REGISTER(setDelay),
    // SOL_REGISTER(setDamage),
    // SOL_REGISTER(hasSpellList),
    // SOL_REGISTER(setSpellList),
    // SOL_REGISTER(SetAutoAttackEnabled),
    // SOL_REGISTER(SetMagicCastingEnabled),
    // SOL_REGISTER(SetMobAbilityEnabled),
    // SOL_REGISTER(SetMobSkillAttack),

    // SOL_REGISTER(getMobMod),
    // SOL_REGISTER(setMobMod),
    // SOL_REGISTER(addMobMod),
    // SOL_REGISTER(delMobMod),

    // SOL_REGISTER(getBattleTime),

    // SOL_REGISTER(getBehaviour),
    // SOL_REGISTER(setBehaviour),

    // SOL_REGISTER(getTarget),
    // SOL_REGISTER(updateTarget),
    // SOL_REGISTER(getEnmityList),
    // SOL_REGISTER(getTrickAttackChar),

    // SOL_REGISTER(actionQueueEmpty),

    // SOL_REGISTER(castSpell),
    // SOL_REGISTER(useJobAbility),
    // SOL_REGISTER(useMobAbility),
    // SOL_REGISTER(hasTPMoves),

    // SOL_REGISTER(weaknessTrigger),
    // SOL_REGISTER(hasPreventActionEffect),
    // SOL_REGISTER(stun),

    // SOL_REGISTER(getPool),
    // SOL_REGISTER(getDropID),
    // SOL_REGISTER(setDropID),
    // SOL_REGISTER(addTreasure),
    // SOL_REGISTER(getStealItem),
    // SOL_REGISTER(getDespoilItem),
    // SOL_REGISTER(getDespoilDebuff),
    // SOL_REGISTER(itemStolen),
    // SOL_REGISTER(getTHlevel),
    // SOL_REGISTER(getPlayerRegionInZone),
    // SOL_REGISTER(updateToEntireZone),
}

//==========================================================//
