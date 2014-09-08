/* ScriptData
SDName: item_summonnpc
SD%Complete: 100
SDComment: Summon NPC
SDCategory: Items
EndScriptData */

#include "precompiled.h"
#include <cstring>

bool ItemUse_item_summonnpc(Player* pPlayer, Item* pItem, const SpellCastTargets &pTargets)
{

    // Declare Variables
    uint32 itemId = pItem->GetEntry();
    uint32 npcId;
    uint32 despawnSec;
    extern DatabaseType WorldDatabase;

if ((pPlayer->isInCombat()) || (!pPlayer->isAlive()))
{
      pPlayer->SendEquipError(EQUIP_ERR_NOT_IN_COMBAT, pItem, NULL);
        return false;
}

if (pPlayer->IsMounted()) // Is player mounted
{
      pPlayer->SendEquipError(EQUIP_ERR_CANT_DO_RIGHT_NOW, pItem, NULL);
        return true;
}

{
        // Fetch the NPC Id
        QueryResult *result = WorldDatabase.PQuery("SELECT `creature_entry`, `despawn` FROM `item_summonnpc` WHERE `item_entry` = '%i' LIMIT 1",itemId);
    if(result)
    {
        Field *fields = result->Fetch();
        // Read NPC Id from the query result
        npcId = fields[0].GetInt32();
        // Read time before despawn
        despawnSec = fields[1].GetUInt32() * 1000;
        // Summon the NPC
        pPlayer->SummonCreature(npcId,pPlayer->GetPositionX() ,pPlayer->GetPositionY()-2, pPlayer->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, despawnSec);
        return true;
    } else {
        // Summons failed - show info in log
        outstring_log ("Item_SummonNPC Failed: %i)",itemId);
    }
    delete result;
}
        return false;
}
void AddSC_item_summonnpc()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="item_summonnpc";
    newscript->pItemUse = &ItemUse_item_summonnpc;
    newscript->RegisterSelf();
}

