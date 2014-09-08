/* ScriptData
SDName: Item_Phase4
SD%Complete: 100
SDComment: Used to change player to phasemask 4
SDCategory: Items
EndScriptData */

#include "precompiled.h"
#include <cstring>

bool ItemUse_item_phase4(Player *player, Item* _Item, SpellCastTargets const& targets)
{
    // Declare variables
    uint32 phase = 4;
    player->SetPhaseMask(phase, true);
    return true;
}

void AddSC_item_phase4()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="item_phase4";
    newscript->pItemUse = &ItemUse_item_phase4;
    newscript->RegisterSelf();
}

