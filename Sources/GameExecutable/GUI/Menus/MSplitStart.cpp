/* Copyright (c) 2002-2012 Croteam Ltd.
This program is free software; you can redistribute it and/or modify
it under the terms of version 2 of the GNU General Public License as published by
the Free Software Foundation


This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA. */

#include "StdH.h"
#include <Engine/CurrentVersion.h>
#include "MenuPrinting.h"
#include "LevelInfo.h"
#include "MenuStuff.h"
#include "MenuStarters.h"
#include "MSplitStart.h"

extern void UpdateSplitLevel(INDEX iDummy);

// --------------------------------------------------------------------------------------
// Intializes split screen start menu.
// --------------------------------------------------------------------------------------
void CSplitStartMenu::Initialize_t(void)
{
  // Initialize title label.
  gm_mgTitle.mg_boxOnScreen = BoxTitle();
  gm_mgTitle.mg_strText = TRANS("START SPLIT SCREEN");
  AddChild(&gm_mgTitle);

  // Initialize "Game type" trigger.
  TRIGGER_MG(gm_mgGameType, 0,
    gm_mgStart, gm_mgDifficulty, TRANS("Game type:"), astrGameTypeRadioTexts);
  gm_mgGameType.mg_ctTexts = ctGameTypeRadioTexts;
  gm_mgGameType.mg_strTip = TRANS("choose type of multiplayer game");

  // Initialize "Difficulty" trigger.
  TRIGGER_MG(gm_mgDifficulty, 1,
    gm_mgGameType, gm_mgLevel, TRANS("Difficulty:"), astrDifficultyRadioTexts);
  gm_mgDifficulty.mg_strTip = TRANS("choose difficulty level");

  // Initialize level name
  gm_mgLevel.mg_strText = "";
  gm_mgLevel.mg_strLabel = TRANS("Level:");
  gm_mgLevel.mg_boxOnScreen = BoxMediumRow(2);
  gm_mgLevel.mg_bfsFontSize = BFS_MEDIUM;
  gm_mgLevel.mg_iCenterI = -1;
  gm_mgLevel.mg_pmgUp = &gm_mgDifficulty;
  gm_mgLevel.mg_pmgDown = &gm_mgGameOptions;
  gm_mgLevel.mg_strTip = TRANS("choose the level to start");

  // Initialize "Game Options" button
  gm_mgGameOptions.mg_strText = TRANS("Game options");
  gm_mgGameOptions.mg_boxOnScreen = BoxMediumRow(3);
  gm_mgGameOptions.mg_bfsFontSize = BFS_MEDIUM;
  gm_mgGameOptions.mg_iCenterI = 0;
  gm_mgGameOptions.mg_pmgUp = &gm_mgLevel;
  gm_mgGameOptions.mg_pmgDown = &gm_mgGameMutators;
  gm_mgGameOptions.mg_strTip = TRANS("adjust game rules");
  
  // Initialize "Game Mutators" button.
  gm_mgGameMutators.mg_strText = TRANS("Game mutators");
  gm_mgGameMutators.mg_boxOnScreen = BoxMediumRow(4);
  gm_mgGameMutators.mg_bfsFontSize = BFS_MEDIUM;
  gm_mgGameMutators.mg_iCenterI = 0;
  gm_mgGameMutators.mg_pmgUp = &gm_mgGameOptions;
  gm_mgGameMutators.mg_pmgDown = &gm_mgStart;
  gm_mgGameMutators.mg_strTip = TRANS("adjust game rules");

  // Initialize "Start" button
  gm_mgStart.mg_bfsFontSize = BFS_LARGE;
  gm_mgStart.mg_boxOnScreen = BoxBigRow(4.5);
  gm_mgStart.mg_pmgUp = &gm_mgGameMutators;
  gm_mgStart.mg_pmgDown = &gm_mgGameType;
  gm_mgStart.mg_strText = TRANS("START");
  
  // Add components.
  AddChild(&gm_mgLevel);
  AddChild(&gm_mgGameOptions);
  AddChild(&gm_mgGameMutators);
  AddChild(&gm_mgStart);
}

void CSplitStartMenu::StartMenu(void)
{
  extern INDEX sam_bMentalActivated;
  gm_mgDifficulty.mg_ctTexts = sam_bMentalActivated ? 7 : 5;

  gm_mgGameType.mg_iSelected = Clamp(_pShell->GetINDEX("gam_iStartMode"), 0L, ctGameTypeRadioTexts - 1L);
  gm_mgGameType.ApplyCurrentSelection();
  gm_mgDifficulty.mg_iSelected = _pShell->GetINDEX("gam_iStartDifficulty") + 1;
  gm_mgDifficulty.ApplyCurrentSelection();

  // clamp maximum number of players to at least 4
  _pShell->SetINDEX("gam_ctMaxPlayers", ClampDn(_pShell->GetINDEX("gam_ctMaxPlayers"), 4L));

  UpdateSplitLevel(0);
  CGameMenu::StartMenu();
}

void CSplitStartMenu::EndMenu(void)
{
  _pShell->SetINDEX("gam_iStartDifficulty", gm_mgDifficulty.mg_iSelected - 1);
  _pShell->SetINDEX("gam_iStartMode", gm_mgGameType.mg_iSelected);

  CGameMenu::EndMenu();
}

// --------------------------------------------------------------------------------------
// [SSE]
// Returns TRUE if event was handled.
// --------------------------------------------------------------------------------------
BOOL CSplitStartMenu::OnEvent(const SEvent& event)
{
  if (event.EventType == EET_GUI_EVENT)
  {
    if (event.GuiEvent.EventType == EGET_TRIGGERED) {
      if (event.GuiEvent.Caller == &gm_mgLevel) {
        StartSelectLevelFromSplit();
        return TRUE;

      } else if (event.GuiEvent.Caller == &gm_mgGameOptions) {
        StartGameOptionsFromSplitScreen();
        return TRUE;
        
      } else if (event.GuiEvent.Caller == &gm_mgGameMutators) {
        StartVarGameMutators();
        return TRUE;

      } else if (event.GuiEvent.Caller == &gm_mgStart) {
        StartSelectPlayersMenuFromSplit();
        return TRUE;
      }

    } else if (event.GuiEvent.EventType == EGET_CHANGED) {

      if (event.GuiEvent.Caller == &gm_mgGameType) {
        UpdateSplitLevel(-1);
        return TRUE; 
      }
    }
  }
  
  if (CGameMenu::OnEvent(event)) {
    return TRUE;
  }
  
  return m_pParent ? m_pParent->OnEvent(event) : FALSE;
}