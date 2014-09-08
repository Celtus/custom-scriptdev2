/* ScriptData
SDName: Item_Phase1
SD%Complete: 100
SDComment: Used to change player to phasemask 1
SDCategory: Items
EndScriptData */

#include "precompiled.h"
#include <cstring>

bool ItemUse_item_phase1(Player *player, Item* _Item, SpellCastTargets const& targets)
{
    // Declare variables
    uint32 phase = 1;
    player->SetPhaseMask(phase, true);
    return true;
}

void AddSC_item_phase1()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="item_phase1";
    newscript->pItemUse = &ItemUse_item_phase1;
    newscript->RegisterSelf();
}

