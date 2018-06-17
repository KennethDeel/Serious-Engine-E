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
#include "MNetworkStart.h"

extern INDEX sam_bMentalActivated;

extern void UpdateNetworkLevel(INDEX iDummy);

// --------------------------------------------------------------------------------------
// Intializes network start menu.
// --------------------------------------------------------------------------------------
void CNetworkStartMenu::Initialize_t(void)
{
  // Initialize title label.
  gm_mgTitle.mg_boxOnScreen = BoxTitle();
  gm_mgTitle.mg_strText = TRANS("START SERVER");

  // Initialize "Session name" edit box.
  gm_mgSessionName.mg_strText = _pGame->gam_strSessionName;
  gm_mgSessionName.mg_strLabel = TRANS("Session name:");
  gm_mgSessionName.mg_ctMaxStringLen = 25;
  gm_mgSessionName.mg_pstrToChange = &_pGame->gam_strSessionName;
  gm_mgSessionName.mg_boxOnScreen = BoxMediumRow(1);
  gm_mgSessionName.mg_bfsFontSize = BFS_MEDIUM;
  gm_mgSessionName.mg_iCenterI = -1;
  gm_mgSessionName.mg_pmgUp = &gm_mgStart;
  gm_mgSessionName.mg_pmgDown = &gm_mgGameType;
  gm_mgSessionName.mg_strTip = TRANS("name the session to start");
  
  // Add components.
  AddChild(&gm_mgTitle);
  AddChild(&gm_mgSessionName);

  // Initialize "Game type" trigger.
  TRIGGER_MG(gm_mgGameType, 2,
    gm_mgSessionName, gm_mgDifficulty, TRANS("Game type:"), astrGameTypeRadioTexts);
  gm_mgGameType.mg_ctTexts = ctGameTypeRadioTexts;
  gm_mgGameType.mg_strTip = TRANS("choose type of multiplayer game");

  // Initialize "Difficulty" trigger.
  TRIGGER_MG(gm_mgDifficulty, 3,
    gm_mgGameType, gm_mgLevel, TRANS("Difficulty:"), astrDifficultyRadioTexts);
  gm_mgDifficulty.mg_strTip = TRANS("choose difficulty level");

  // Initialize "Level" button.
  gm_mgLevel.mg_strText = "";
  gm_mgLevel.mg_strLabel = TRANS("Level:");
  gm_mgLevel.mg_boxOnScreen = BoxMediumRow(4);
  gm_mgLevel.mg_bfsFontSize = BFS_MEDIUM;
  gm_mgLevel.mg_iCenterI = -1;
  gm_mgLevel.mg_pmgUp = &gm_mgDifficulty;
  gm_mgLevel.mg_pmgDown = &gm_mgMaxPlayers;
  gm_mgLevel.mg_strTip = TRANS("choose the level to start");
  AddChild(&gm_mgLevel);
  
  // Initialize "Min players" trigger.
  TRIGGER_MG(gm_mgMinPlayers, 5,
    gm_mgLevel, gm_mgMaxPlayers, TRANS("Min players:"), astrMinPlayersRadioTexts);
  gm_mgMinPlayers.mg_strTip = TRANS("choose minimum needed number of players");

  // Initialize "Max players" trigger.
  TRIGGER_MG(gm_mgMaxPlayers, 6,
    gm_mgMinPlayers, gm_mgVisible, TRANS("Max players:"), astrMaxPlayersRadioTexts);
  gm_mgMaxPlayers.mg_strTip = TRANS("choose maximum allowed number of players");

  // Initialize "Wait all players" trigger.
  //TRIGGER_MG(gm_mgWaitAllPlayers, 6,
  //  gm_mgMaxPlayers, gm_mgVisible, TRANS("Wait for all players:"), astrNoYes);
  //gm_mgWaitAllPlayers.mg_strTip = TRANS("if on, game won't start until all players have joined");

  // Initialize "Server visible" trigger.
  TRIGGER_MG(gm_mgVisible, 7,
    gm_mgMaxPlayers, gm_mgGameOptions, TRANS("Server visible:"), astrNoYes);
  gm_mgVisible.mg_strTip = TRANS("invisible servers are not listed, cleints have to join manually");

  // Initialize "Game options" button.
  gm_mgGameOptions.mg_strText = TRANS("Game options");
  gm_mgGameOptions.mg_boxOnScreen = BoxMediumRow(8);
  gm_mgGameOptions.mg_bfsFontSize = BFS_MEDIUM;
  gm_mgGameOptions.mg_iCenterI = 0;
  gm_mgGameOptions.mg_pmgUp = &gm_mgVisible;
  gm_mgGameOptions.mg_pmgDown = &gm_mgGameMutators;
  gm_mgGameOptions.mg_strTip = TRANS("adjust game rules");
  
  // Initialize "Game mutators" button.
  gm_mgGameMutators.mg_strText = TRANS("Game mutators");
  gm_mgGameMutators.mg_boxOnScreen = BoxMediumRow(9);
  gm_mgGameMutators.mg_bfsFontSize = BFS_MEDIUM;
  gm_mgGameMutators.mg_iCenterI = 0;
  gm_mgGameMutators.mg_pmgUp = &gm_mgGameOptions;
  gm_mgGameMutators.mg_pmgDown = &gm_mgStart;
  gm_mgGameMutators.mg_strTip = TRANS("adjust game rules");

  // Initialize "Start" button.
  gm_mgStart.mg_bfsFontSize = BFS_LARGE;
  gm_mgStart.mg_boxOnScreen = BoxBigRow(7.5);
  gm_mgStart.mg_pmgUp = &gm_mgGameMutators;
  gm_mgStart.mg_pmgDown = &gm_mgSessionName;
  gm_mgStart.mg_strText = TRANS("START");
  
  // Add components.
  AddChild(&gm_mgGameOptions);
  AddChild(&gm_mgGameMutators);
  AddChild(&gm_mgStart);
}

void CNetworkStartMenu::StartMenu(void)
{
  gm_mgDifficulty.mg_ctTexts = sam_bMentalActivated ? 7 : 5;

  gm_mgGameType.mg_iSelected = Clamp(_pShell->GetINDEX("gam_iStartMode"), 0L, ctGameTypeRadioTexts - 1L);
  gm_mgGameType.ApplyCurrentSelection();
  gm_mgDifficulty.mg_iSelected = _pShell->GetINDEX("gam_iStartDifficulty") + 1;
  gm_mgDifficulty.ApplyCurrentSelection();

  _pShell->SetINDEX("gam_iStartMode", CSessionProperties::GM_COOPERATIVE);

  INDEX ctMaxPlayers = _pShell->GetINDEX("gam_ctMaxPlayers");
  if (ctMaxPlayers<2 || ctMaxPlayers>16) {
    ctMaxPlayers = 2;
    _pShell->SetINDEX("gam_ctMaxPlayers", ctMaxPlayers);
  }

  gm_mgMaxPlayers.mg_iSelected = ctMaxPlayers - 2;
  gm_mgMaxPlayers.ApplyCurrentSelection();

  //gm_mgWaitAllPlayers.mg_iSelected = Clamp(_pShell->GetINDEX("gam_bWaitAllPlayers"), 0L, 1L);
  //gm_mgWaitAllPlayers.ApplyCurrentSelection();

  gm_mgVisible.mg_iSelected = _pShell->GetINDEX("ser_bEnumeration");
  gm_mgVisible.ApplyCurrentSelection();

  UpdateNetworkLevel(0);

  CGameMenu::StartMenu();
}

void CNetworkStartMenu::EndMenu(void)
{
  _pShell->SetINDEX("gam_iStartDifficulty", gm_mgDifficulty.mg_iSelected - 1);
  _pShell->SetINDEX("gam_iStartMode", gm_mgGameType.mg_iSelected);
  //_pShell->SetINDEX("gam_bWaitAllPlayers", gm_mgWaitAllPlayers.mg_iSelected);
  _pShell->SetINDEX("gam_ctMaxPlayers", gm_mgMaxPlayers.mg_iSelected + 2);
  _pShell->SetINDEX("ser_bEnumeration", gm_mgVisible.mg_iSelected);

  CGameMenu::EndMenu();
}

// --------------------------------------------------------------------------------------
// [SSE]
// Returns TRUE if event was handled.
// --------------------------------------------------------------------------------------
BOOL CNetworkStartMenu::OnEvent(const SEvent& event)
{
  if (event.EventType == EET_GUI_EVENT)
  {
    if (event.GuiEvent.EventType == EGET_TRIGGERED) {
      if (event.GuiEvent.Caller == &gm_mgLevel) {
        StartSelectLevelFromNetwork();
        return TRUE;

      } else if (event.GuiEvent.Caller == &gm_mgGameOptions) {
        StartGameOptionsFromNetwork();
        return TRUE;

      } else if (event.GuiEvent.Caller == &gm_mgGameMutators) {
        StartVarGameMutators();
        return TRUE;

      } else if (event.GuiEvent.Caller == &gm_mgStart) {
        StartSelectPlayersMenuFromNetwork();
        return TRUE;
      }
      
    } else if (event.GuiEvent.EventType == EGET_CHANGED) {

      if (event.GuiEvent.Caller == &gm_mgGameType) {
        UpdateNetworkLevel(-1);
        return TRUE; 
      }
    }
  }
  
  if (CGameMenu::OnEvent(event)) {
    return TRUE;
  }
  
  return m_pParent ? m_pParent->OnEvent(event) : FALSE;
}
