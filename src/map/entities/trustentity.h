﻿/*
===========================================================================

  Copyright (c) 2018 Darkstar Dev Teams

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

#ifndef _CTRUSTENTITY_H
#define _CTRUSTENTITY_H

#include "mobentity.h"

// PTrust->m_MovementType is read from 'behaviour' in a trust's mob_pool entry
enum TRUST_MOVEMENT_TYPE
{
    MELEE_RANGE        = 0, // Default
    NO_MOVE            = 1,
    MID_RANGE          = 2,
    LONG_RANGE         = 3,
    LAST_MOVEMENT_TYPE = 4,
};

class CCharEntity;
class CAbilityState;
class CRangeState;
class CDespawnState;
class CMagicState;
class CMobSkillState;
class CWeaponSkillState;

class CTrustEntity : public CMobEntity
{
public:
    explicit CTrustEntity(CCharEntity*);
    ~CTrustEntity() override = default;

    void PostTick() override;
    void FadeOut() override;
    void Die() override;
    void Spawn() override;
    void OnAbility(CAbilityState&, action_t&) override;
    void OnRangedAttack(CRangeState&, action_t&) override;
    bool ValidTarget(CBattleEntity* PInitiator, uint16 targetFlags) override;
    void OnDespawn(CDespawnState&) override;

    void OnCastFinished(CMagicState& state, action_t& action) override;
    void OnMobSkillFinished(CMobSkillState& state, action_t& action) override;
    void OnWeaponSkillFinished(CWeaponSkillState& state, action_t& action) override;

    uint32              m_TrustID{};
    TRUST_MOVEMENT_TYPE m_MovementType;
};

#endif
