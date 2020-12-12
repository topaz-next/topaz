/*
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

#include <array>
#include <cstring>

#include "../entities/battleentity.h"
#include "../map.h"
#include "itemutils.h"

std::array<CItem*, MAX_ITEMID>      g_pItemList; // global array of pointers to game items
std::array<DropList_t*, MAX_DROPID> g_pDropList; // global array of monster droplist items
std::array<LootList_t*, MAX_LOOTID> g_pLootList; // global array of BCNM lootlist items

CItemWeapon* PUnarmedItem;
CItemWeapon* PUnarmedH2HItem;

DropItem_t::DropItem_t(uint8 DropType, uint16 ItemID, uint16 DropRate)
: DropType(DropType)
, ItemID(ItemID)
, DropRate(DropRate)
{
}

DropGroup_t::DropGroup_t(uint16 GroupRate)
: GroupRate(GroupRate)
{
}

/************************************************************************
 *                                                                       *
 *  Actually methods of working with a global collection of items        *
 *                                                                       *
 ************************************************************************/

namespace itemutils
{
/************************************************************************
 *                                                                       *
 *  Create an empty instance of the item by ID (private method)          *
 *                                                                       *
 ************************************************************************/

CItem* CreateItem(uint16 ItemID)
{
    if ((ItemID >= 0x0200) && (ItemID <= 0x0206))
    {
        return new CItemLinkshell(ItemID);
    }
    if ((ItemID >= 0x01D8) && (ItemID <= 0x0DFF))
    {
        return new CItemGeneral(ItemID);
    }
    if ((ItemID >= 0x0000) && (ItemID <= 0x0FFF))
    {
        return new CItemFurnishing(ItemID);
    }
    if ((ItemID >= 0x1000) && (ItemID <= 0x1FFF))
    {
        return new CItemUsable(ItemID);
    }
    if ((ItemID >= 0x2000) && (ItemID <= 0x27FF))
    {
        return new CItemPuppet(ItemID);
    }
    if ((ItemID >= 0x2800) && (ItemID <= 0x3FFF))
    {
        return new CItemEquipment(ItemID);
    }
    if ((ItemID >= 0x4000) && (ItemID <= 0x5FFF))
    {
        return new CItemWeapon(ItemID);
    }
    if ((ItemID >= 0x6000) && (ItemID <= 0x6FFF))
    {
        return new CItemEquipment(ItemID);
    }
    if ((ItemID >= 0x7000) && (ItemID <= 0x7FFF))
    {
        return new CItemGeneral(ItemID);
    }

    return nullptr;
}

/************************************************************************
 *                                                                       *
 *  Create a new copy of the item ID                                     *
 *                                                                       *
 ************************************************************************/

CItem* GetItem(uint16 ItemID)
{
    if (ItemID == 0xFFFF)
    {
        return new CItemCurrency(ItemID);
    }
    if (ItemID < MAX_ITEMID && g_pItemList[ItemID] != nullptr)
    {
        if ((ItemID >= 0x0200) && (ItemID <= 0x0206))
        {
            return new CItemLinkshell(*(dynamic_cast<CItemLinkshell*>(g_pItemList[ItemID])));
        }
        if ((ItemID >= 0x01D8) && (ItemID <= 0x0DFF))
        {
            return new CItemGeneral(*(dynamic_cast<CItemGeneral*>(g_pItemList[ItemID])));
        }
        if ((ItemID >= 0x0000) && (ItemID <= 0x0FFF))
        {
            return new CItemFurnishing(*(dynamic_cast<CItemFurnishing*>(g_pItemList[ItemID])));
        }
        if ((ItemID >= 0x1000) && (ItemID <= 0x1FFF))
        {
            return new CItemUsable(*(dynamic_cast<CItemUsable*>(g_pItemList[ItemID])));
        }
        if ((ItemID >= 0x2000) && (ItemID <= 0x27FF))
        {
            return new CItemPuppet(*(dynamic_cast<CItemPuppet*>(g_pItemList[ItemID])));
        }
        if (((ItemID >= 0x2800) && (ItemID <= 0x3FFF)))
        {
            return new CItemEquipment(*(dynamic_cast<CItemEquipment*>(g_pItemList[ItemID])));
        }
        if ((ItemID >= 0x4000) && (ItemID <= 0x5FFF))
        {
            return new CItemWeapon(*(dynamic_cast<CItemWeapon*>(g_pItemList[ItemID])));
        }
        if ((ItemID >= 0x6000) && (ItemID <= 0x6FFF))
        {
            return new CItemEquipment(*(dynamic_cast<CItemEquipment*>(g_pItemList[ItemID])));
        }
        if ((ItemID >= 0x7000) && (ItemID <= 0x7FFF))
        {
            return new CItemGeneral(*(dynamic_cast<CItemGeneral*>(g_pItemList[ItemID])));
        }
    }
    return nullptr;
}

/************************************************************************
 *                                                                       *
 *  Create a copy of the item                                            *
 *                                                                       *
 ************************************************************************/

CItem* GetItem(CItem* PItem)
{
    TPZ_DEBUG_BREAK_IF(PItem == nullptr);

    if (PItem->isType(ITEM_WEAPON))
    {
        return new CItemWeapon(*(dynamic_cast<CItemWeapon*>(PItem)));
    }
    if (PItem->isType(ITEM_EQUIPMENT))
    {
        return new CItemEquipment(*(dynamic_cast<CItemEquipment*>(PItem)));
    }
    if (PItem->isType(ITEM_USABLE))
    {
        return new CItemUsable(*(dynamic_cast<CItemUsable*>(PItem)));
    }
    if (PItem->isType(ITEM_LINKSHELL))
    {
        return new CItemLinkshell(*(dynamic_cast<CItemLinkshell*>(PItem)));
    }
    if (PItem->isType(ITEM_FURNISHING))
    {
        return new CItemFurnishing(*(dynamic_cast<CItemFurnishing*>(PItem)));
    }
    if (PItem->isType(ITEM_PUPPET))
    {
        return new CItemPuppet(*(dynamic_cast<CItemPuppet*>(PItem)));
    }
    if (PItem->isType(ITEM_GENERAL))
    {
        return new CItemGeneral(*(dynamic_cast<CItemGeneral*>(PItem)));
    }
    if (PItem->isType(ITEM_CURRENCY))
    {
        return new CItemCurrency(*(dynamic_cast<CItemCurrency*>(PItem)));
    }
    return nullptr;
}

/************************************************************************
 *                                                                       *
 *  Get a pointer to an item (read-only)                                 *
 *                                                                       *
 ************************************************************************/

CItem* GetItemPointer(uint16 ItemID)
{
    if (ItemID < MAX_ITEMID)
    {
        return g_pItemList[ItemID];
    }
    ShowWarning(CL_CYAN "ItemID %u too big\n" CL_RESET, ItemID);
    return nullptr;
}

/************************************************************************
 *                                                                       *
 *  True if pointer points to a read-only g_pItemList array item         *
 *                                                                       *
 ************************************************************************/

bool IsItemPointer(CItem* item)
{
    return g_pItemList[item->getID()] == item;
}

/************************************************************************
 *                                                                       *
 *                                                                       *
 *                                                                       *
 ************************************************************************/

CItemWeapon* GetUnarmedItem()
{
    return PUnarmedItem;
}

CItemWeapon* GetUnarmedH2HItem()
{
    return PUnarmedH2HItem;
}

/************************************************************************
 *                                                                       *
 *  Get the monsters item drop list                                      *
 *                                                                       *
 ************************************************************************/

DropList_t* GetDropList(uint16 DropID)
{
    if (DropID < MAX_DROPID)
    {
        return g_pDropList[DropID];
    }
    ShowWarning(CL_CYAN "DropID %u too big\n" CL_RESET, DropID);
    return nullptr;
}

/************************************************************************
 *                                                                       *
 *                                                                       *
 *                                                                       *
 ************************************************************************/

LootList_t* GetLootList(uint16 LootID)
{
    if (LootID < MAX_LOOTID)
    {
        return g_pLootList[LootID];
    }
    ShowWarning(CL_CYAN "LootID %u too big\n" CL_RESET, LootID);
    return nullptr;
}

/************************************************************************
 *                                                                       *
 *  Load the items                                                       *
 *                                                                       *
 ************************************************************************/

void LoadItemList()
{
    const char* Query = "SELECT "
                        "b.itemId,"    //  0
                        "b.name,"      //  1
                        "b.stackSize," //  2
                        "b.flags,"     //  3
                        "b.aH,"        //  4
                        "b.BaseSell,"  //  5
                        "b.subid,"     //  6

                        "u.validTargets,"  //  7
                        "u.activation,"    //  8
                        "u.animation,"     //  9
                        "u.animationTime," // 10
                        "u.maxCharges,"    // 11
                        "u.useDelay,"      // 12
                        "u.reuseDelay,"    // 13
                        "u.aoe,"           // 14

                        "a.level,"      // 15
                        "a.ilevel,"     // 16
                        "a.jobs,"       // 17
                        "a.MId,"        // 18
                        "a.shieldSize," // 19
                        "a.scriptType," // 20
                        "a.slot,"       // 21
                        "a.rslot,"      // 22

                        "w.skill,"         // 23
                        "w.subskill,"      // 24
                        "w.ilvl_skill,"    // 25
                        "w.ilvl_parry,"    // 26
                        "w.ilvl_macc,"     // 27
                        "w.delay,"         // 28
                        "w.dmg,"           // 29
                        "w.dmgType,"       // 30
                        "w.hit,"           // 31
                        "w.unlock_points," // 32

                        "f.storage,"      // 33
                        "f.moghancement," // 34
                        "f.element,"      // 35
                        "f.aura,"         // 36

                        "p.slot,"    // 37
                        "p.element " // 38
                        "FROM item_basic AS b "
                        "LEFT JOIN item_usable AS u USING (itemId) "
                        "LEFT JOIN item_equipment  AS a USING (itemId) "
                        "LEFT JOIN item_weapon AS w USING (itemId) "
                        "LEFT JOIN item_furnishing AS f USING (itemId) "
                        "LEFT JOIN item_puppet AS p USING (itemId) "
                        "WHERE itemId < %u;";

    int32 ret = Sql_Query(SqlHandle, Query, MAX_ITEMID);

    if (ret != SQL_ERROR && Sql_NumRows(SqlHandle) != 0)
    {
        while (Sql_NextRow(SqlHandle) == SQL_SUCCESS)
        {
            CItem* PItem = CreateItem(Sql_GetUIntData(SqlHandle, 0));

            if (PItem != nullptr)
            {
                PItem->setName(Sql_GetData(SqlHandle, 1));
                PItem->setStackSize(Sql_GetUIntData(SqlHandle, 2));
                PItem->setFlag(Sql_GetUIntData(SqlHandle, 3));
                PItem->setAHCat(Sql_GetUIntData(SqlHandle, 4));
                PItem->setBasePrice(Sql_GetUIntData(SqlHandle, 5));
                PItem->setSubID(Sql_GetUIntData(SqlHandle, 6));

                if (PItem->isType(ITEM_GENERAL))
                {
                }
                if (PItem->isType(ITEM_USABLE))
                {
                    (dynamic_cast<CItemUsable*>(PItem))->setValidTarget(Sql_GetUIntData(SqlHandle, 7));
                    (dynamic_cast<CItemUsable*>(PItem))->setActivationTime(Sql_GetUIntData(SqlHandle, 8) * 1000);
                    (dynamic_cast<CItemUsable*>(PItem))->setAnimationID(Sql_GetUIntData(SqlHandle, 9));
                    (dynamic_cast<CItemUsable*>(PItem))->setAnimationTime(Sql_GetUIntData(SqlHandle, 10) * 1000);
                    (dynamic_cast<CItemUsable*>(PItem))->setMaxCharges(Sql_GetUIntData(SqlHandle, 11));
                    (dynamic_cast<CItemUsable*>(PItem))->setCurrentCharges(Sql_GetUIntData(SqlHandle, 11));
                    (dynamic_cast<CItemUsable*>(PItem))->setUseDelay(Sql_GetUIntData(SqlHandle, 12));
                    (dynamic_cast<CItemUsable*>(PItem))->setReuseDelay(Sql_GetUIntData(SqlHandle, 13));
                    (dynamic_cast<CItemUsable*>(PItem))->setAoE(Sql_GetUIntData(SqlHandle, 14));
                }
                if (PItem->isType(ITEM_PUPPET))
                {
                    (dynamic_cast<CItemPuppet*>(PItem))->setEquipSlot(Sql_GetUIntData(SqlHandle, 37));
                    (dynamic_cast<CItemPuppet*>(PItem))->setElementSlots(Sql_GetUIntData(SqlHandle, 38));
                }
                if (PItem->isType(ITEM_EQUIPMENT))
                {
                    (dynamic_cast<CItemEquipment*>(PItem))->setReqLvl(Sql_GetUIntData(SqlHandle, 15));
                    (dynamic_cast<CItemEquipment*>(PItem))->setILvl(Sql_GetUIntData(SqlHandle, 16));
                    (dynamic_cast<CItemEquipment*>(PItem))->setJobs(Sql_GetUIntData(SqlHandle, 17));
                    (dynamic_cast<CItemEquipment*>(PItem))->setModelId(Sql_GetUIntData(SqlHandle, 18));
                    (dynamic_cast<CItemEquipment*>(PItem))->setShieldSize(Sql_GetUIntData(SqlHandle, 19));
                    (dynamic_cast<CItemEquipment*>(PItem))->setScriptType(Sql_GetUIntData(SqlHandle, 20));
                    (dynamic_cast<CItemEquipment*>(PItem))->setEquipSlotId(Sql_GetUIntData(SqlHandle, 21));
                    (dynamic_cast<CItemEquipment*>(PItem))->setRemoveSlotId(Sql_GetUIntData(SqlHandle, 22));

                    if ((dynamic_cast<CItemEquipment*>(PItem))->getValidTarget() != 0)
                    {
                        (dynamic_cast<CItemEquipment*>(PItem))->setSubType(ITEM_CHARGED);
                    }
                }
                if (PItem->isType(ITEM_WEAPON))
                {
                    (dynamic_cast<CItemWeapon*>(PItem))->setSkillType(Sql_GetUIntData(SqlHandle, 23));
                    (dynamic_cast<CItemWeapon*>(PItem))->setSubSkillType(Sql_GetUIntData(SqlHandle, 24));
                    (dynamic_cast<CItemWeapon*>(PItem))->setILvlSkill(Sql_GetUIntData(SqlHandle, 25));
                    (dynamic_cast<CItemWeapon*>(PItem))->setILvlParry(Sql_GetUIntData(SqlHandle, 26));
                    (dynamic_cast<CItemWeapon*>(PItem))->setILvlMacc(Sql_GetUIntData(SqlHandle, 27));
                    (dynamic_cast<CItemWeapon*>(PItem))->setDelay((Sql_GetIntData(SqlHandle, 28) * 1000) / 60);
                    (dynamic_cast<CItemWeapon*>(PItem))->setDamage(Sql_GetUIntData(SqlHandle, 29));
                    (dynamic_cast<CItemWeapon*>(PItem))->setDmgType(Sql_GetUIntData(SqlHandle, 30));
                    (dynamic_cast<CItemWeapon*>(PItem))->setMaxHit(Sql_GetUIntData(SqlHandle, 31));
                    (dynamic_cast<CItemWeapon*>(PItem))->setUnlockablePoints(Sql_GetUIntData(SqlHandle, 32));
                }
                if (PItem->isType(ITEM_FURNISHING))
                {
                    (dynamic_cast<CItemFurnishing*>(PItem))->setStorage(Sql_GetUIntData(SqlHandle, 33));
                    (dynamic_cast<CItemFurnishing*>(PItem))->setMoghancement(Sql_GetUIntData(SqlHandle, 34));
                    (dynamic_cast<CItemFurnishing*>(PItem))->setElement(Sql_GetUIntData(SqlHandle, 35));
                    (dynamic_cast<CItemFurnishing*>(PItem))->setAura(Sql_GetUIntData(SqlHandle, 36));
                }
                g_pItemList[PItem->getID()] = PItem;
            }
        }
    }

    ret = Sql_Query(SqlHandle,
                    "SELECT itemId, modId, value FROM item_mods WHERE itemId IN (SELECT itemId FROM item_basic LEFT JOIN item_equipment USING (itemId))");

    if (ret != SQL_ERROR && Sql_NumRows(SqlHandle) != 0)
    {
        while (Sql_NextRow(SqlHandle) == SQL_SUCCESS)
        {
            uint16 ItemID = (uint16)Sql_GetUIntData(SqlHandle, 0);
            Mod    modID  = static_cast<Mod>(Sql_GetUIntData(SqlHandle, 1));
            int16  value  = (int16)Sql_GetIntData(SqlHandle, 2);

            if ((g_pItemList[ItemID] != nullptr) && g_pItemList[ItemID]->isType(ITEM_EQUIPMENT))
            {
                (dynamic_cast<CItemEquipment*>(g_pItemList[ItemID]))->addModifier(CModifier(modID, value));
            }
        }
    }

    ret = Sql_Query(
        SqlHandle,
        "SELECT itemId, modId, value, petType FROM item_mods_pet WHERE itemId IN (SELECT itemId FROM item_basic LEFT JOIN item_equipment USING (itemId))");

    if (ret != SQL_ERROR && Sql_NumRows(SqlHandle) != 0)
    {
        while (Sql_NextRow(SqlHandle) == SQL_SUCCESS)
        {
            uint16     ItemID  = (uint16)Sql_GetUIntData(SqlHandle, 0);
            Mod        modID   = static_cast<Mod>(Sql_GetUIntData(SqlHandle, 1));
            int16      value   = (int16)Sql_GetIntData(SqlHandle, 2);
            PetModType petType = static_cast<PetModType>(Sql_GetIntData(SqlHandle, 3));

            if ((g_pItemList[ItemID]) && g_pItemList[ItemID]->isType(ITEM_EQUIPMENT))
            {
                (dynamic_cast<CItemEquipment*>(g_pItemList[ItemID]))->addPetModifier(CPetModifier(modID, petType, value));
            }
        }
    }

    ret = Sql_Query(SqlHandle, "SELECT itemId, modId, value, latentId, latentParam FROM item_latents WHERE itemId IN (SELECT itemId FROM item_basic LEFT JOIN "
                               "item_equipment USING (itemId))");

    if (ret != SQL_ERROR && Sql_NumRows(SqlHandle) != 0)
    {
        while (Sql_NextRow(SqlHandle) == SQL_SUCCESS)
        {
            uint16 ItemID      = (uint16)Sql_GetUIntData(SqlHandle, 0);
            Mod    modID       = static_cast<Mod>(Sql_GetUIntData(SqlHandle, 1));
            int16  value       = (int16)Sql_GetIntData(SqlHandle, 2);
            uint16 latentId    = (uint16)Sql_GetIntData(SqlHandle, 3);
            uint16 latentParam = (uint16)Sql_GetIntData(SqlHandle, 4);

            if ((g_pItemList[ItemID] != nullptr) && g_pItemList[ItemID]->isType(ITEM_EQUIPMENT))
            {
                (dynamic_cast<CItemEquipment*>(g_pItemList[ItemID]))->addLatent((LATENT)latentId, latentParam, modID, value);
            }
        }
    }
}

/************************************************************************
 *                                                                       *
 *  load lists of items monsters drop                                    *
 *                                                                       *
 ************************************************************************/

void LoadDropList()
{
    int32 ret = Sql_Query(SqlHandle, "SELECT dropId, itemId, dropType, itemRate, groupId, groupRate FROM mob_droplist WHERE dropid < %u;", MAX_DROPID);

    if (ret != SQL_ERROR && Sql_NumRows(SqlHandle) != 0)
    {
        while (Sql_NextRow(SqlHandle) == SQL_SUCCESS)
        {
            uint16 DropID = (uint16)Sql_GetUIntData(SqlHandle, 0);

            if (g_pDropList[DropID] == nullptr)
            {
                g_pDropList[DropID] = new DropList_t;
            }

            DropList_t* dropList = g_pDropList[DropID];

            uint16 ItemID   = (uint16)Sql_GetIntData(SqlHandle, 1);
            uint8  DropType = (uint8)Sql_GetIntData(SqlHandle, 2);
            uint16 DropRate = (uint16)Sql_GetIntData(SqlHandle, 3);

            if (DropType == DROP_GROUPED)
            {
                uint8  GroupId   = (uint8)Sql_GetIntData(SqlHandle, 4);
                uint16 GroupRate = (uint16)Sql_GetIntData(SqlHandle, 5);
                while (GroupId >= dropList->Groups.size())
                {
                    dropList->Groups.emplace_back(GroupRate);
                }
                dropList->Groups[GroupId].Items.emplace_back(DropType, ItemID, DropRate);
            }
            else
            {
                dropList->Items.emplace_back(DropType, ItemID, DropRate);
            }
        }
    }
}

/************************************************************************
 *                                                                       *
 *  Handles loot from NPCs that drop things into                         *
 *  the loot pool instead of adding them directly to the inventory       *
 *                                                                       *
 ************************************************************************/

void LoadLootList()
{
}

/************************************************************************
 *                                                                       *
 *  Initialization of the  game objects             bbbb                 *
 *                                                                       *
 ************************************************************************/

void Initialize()
{
    TracyZoneScoped;
    LoadItemList();
    LoadDropList();
    LoadLootList();

    PUnarmedItem = new CItemWeapon(0);

    PUnarmedItem->setDmgType(DAMAGE_NONE);
    PUnarmedItem->setSkillType(SKILL_NONE);
    PUnarmedItem->setDamage(3);

    PUnarmedH2HItem = new CItemWeapon(0);

    PUnarmedH2HItem->setDmgType(DAMAGE_HTH);
    PUnarmedH2HItem->setSkillType(SKILL_HAND_TO_HAND);
    PUnarmedH2HItem->setDamage(3);
}

/************************************************************************
 *                                                                       *
 *  Release the list of items                                            *
 *                                                                       *
 ************************************************************************/

void FreeItemList()
{
    for (int32 ItemID = 0; ItemID < MAX_ITEMID; ++ItemID)
    {
        delete g_pItemList[ItemID];
        g_pItemList[ItemID] = nullptr;
    }

    for (int32 DropID = 0; DropID < MAX_DROPID; ++DropID)
    {
        delete g_pDropList[DropID];
        g_pDropList[DropID] = nullptr;
    }
}
}; // namespace itemutils
