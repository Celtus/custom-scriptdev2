/* ScriptData
SDName: Item_Phase2
SD%Complete: 100
SDComment: Used to change player to phasemask 2
SDCategory: Items
EndScriptData */

#include "precompiled.h"
#include <cstring>

bool ItemUse_item_phase2(Player *player, Item* _Item, SpellCastTargets const& targets)
{
    // Declare variables
    uint32 phase = 2;
    player->SetPhaseMask(phase, true);
    return true;
}

void AddSC_item_phase2()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name="item_phase2";
    newscript->pItemUse = &ItemUse_item_phase2;
    newscript->RegisterSelf();
}

