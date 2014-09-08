#include "precompiled.h"

struct npc_areaguardAI : public Scripted_NoMovementAI
{
    npc_areaguardAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
    {
        pCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        pCreature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_NORMAL, true);
        pCreature->ApplySpellImmune(0, IMMUNITY_DAMAGE, SPELL_SCHOOL_MASK_MAGIC, true);
    }

    void Reset(){}

    void Aggro(Unit* /*pWho*/){}

    void AttackStart(Unit* /*pWho*/){}

    void MoveInLineOfSight(Unit *pWho, Creature* pCreature)
    {

        // Return if Distance is greater than maximum (65 ft) - saves data reads
        if (!pCreature->IsWithinDist(pWho, 65.0f, false))
            return;

        extern DatabaseType WorldDatabase;
        uint32 creatureID = pCreature->GetEntry();
        uint32 guardType;
        uint32 triggerDist;
        uint32 reqValue1, reqValue2;
        uint32 teleMap;
        float teleX,teleY,teleZ,teleO;

        QueryResult* result = WorldDatabase.PQuery("SELECT `guard_type`, `trigger_distance`, `req_value1`,`req_value2`,`tele_map`,`tele_x`,`tele_y`,`tele_z`,`tele_o` FROM `npc_areaguard` WHERE `creature_entry` = %i LIMIT 1",creatureID);
        if (result)
        {
            Field *fields = result->Fetch();
            guardType = fields[0].GetUInt32();
            triggerDist = fields[1].GetFloat();
            reqValue1 = fields[2].GetUInt32();
            reqValue2 = fields[3].GetUInt32();
            teleMap = fields[4].GetUInt32();
            teleX = fields[5].GetFloat();
            teleY = fields[6].GetFloat();
            teleZ = fields[7].GetFloat();
            teleO = fields[8].GetFloat();
            delete result;
        } else {
            return;
        }

        if (!pWho || !pWho->IsInWorld())
            return;

        // Return if distance is greater than the trigger_distance
        if (!pCreature->IsWithinDist(pWho, triggerDist, false))
            return;

        Player *pPlayer = pWho->GetCharmerOrOwnerPlayerOrPlayerItself();

        // Return if player has GM flag on or is in process of teleport
        if (!pPlayer || pPlayer->isGameMaster() || pPlayer->IsBeingTeleported())
            return;

        switch (guardType)
        {
            case 0:
                // Action on all players without GM flag on
                pPlayer->TeleportTo(teleMap,teleX,teleY,teleZ,teleO);
                break;
            case 1:
                {
                // Action based on Team
                if (reqValue1 == 1) // Alliance Required 
                    if (pPlayer->GetTeam() != 469) // Player is Horde
                        pPlayer->TeleportTo(teleMap,teleX,teleY,teleZ,teleO);
                if (reqValue1 == 2) // Horde Required
                    if (pPlayer->GetTeam() != 67) // Player is Alliance
                        pPlayer->TeleportTo(teleMap,teleX,teleY,teleZ,teleO);
                }
                break;
            case 2:
                {
                // Action based on GM Level
                if (pPlayer->GetSession()->GetSecurity() < reqValue1)
                    pPlayer->TeleportTo(teleMap,teleX,teleY,teleZ,teleO);
                }
                break;
            case 3:
                {
                // Action based on Player Level
                if (pPlayer->getLevel() < reqValue1 || pPlayer->getLevel() > reqValue2)
                    pPlayer->TeleportTo(teleMap,teleX,teleY,teleZ,teleO);
                }
                break;
            case 4:
                {
                // Action based on Guild ID
                if (pPlayer->GetGuildId() != reqValue1)
                    pPlayer->TeleportTo(teleMap,teleX,teleY,teleZ,teleO);
                }
                break;
            case 5:
                {
                // Action based on Player GUID
                if (pPlayer->GetGUIDLow() != reqValue1)
                    pPlayer->TeleportTo(teleMap,teleX,teleY,teleZ,teleO);
                }
                break;
        }
 
        return;
    }

    void UpdateAI(const uint32 /*diff*/){}
};

CreatureAI* GetAI_npc_areaguard(Creature* pCreature)
{
    return new npc_areaguardAI(pCreature);
}

void AddSC_npc_areaguard()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "npc_areaguard";
    newscript->GetAI = &GetAI_npc_areaguard;
    newscript->RegisterSelf();
}

