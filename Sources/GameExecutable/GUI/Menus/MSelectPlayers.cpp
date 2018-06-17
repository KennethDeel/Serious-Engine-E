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
#include "MenuStuff.h"
#include "MenuStarters.h"
#include "MenuStartersAF.h"
#include "MenuManager.h"
#include "MSelectPlayers.h"

#define ADD_GADGET( gd, box, up, dn, lf, rt, txt) \
  gd.mg_boxOnScreen = box; \
  gd.mg_pmgUp = up; \
  gd.mg_pmgDown = dn; \
  gd.mg_pmgLeft = lf; \
  gd.mg_pmgRight = rt; \
  gd.mg_strText = txt; \
  AddChild(&gd);

extern CTString astrNoYes[2];
extern CTString astrSplitScreenRadioTexts[4];
extern void SelectPlayersFillMenu(void);
extern void SelectPlayersApplyMenu(void);

// --------------------------------------------------------------------------------------
// Intializes players selection menu.
// --------------------------------------------------------------------------------------
void CSelectPlayersMenu::Initialize_t(void)
{
  // Initialize title label.
  gm_mgTitle.mg_boxOnScreen = BoxTitle();
  gm_mgTitle.mg_strText = TRANS("SELECT PLAYERS");
  AddChild(&gm_mgTitle);

  // Initialize "Dedicated" trigger.
  TRIGGER_MG(gm_mgDedicated, 0, gm_mgStart, gm_mgObserver, TRANS("Dedicated:"), astrNoYes);
  gm_mgDedicated.mg_strTip = TRANS("select to start dedicated server");

  // Initialize "Observer" trigger.
  TRIGGER_MG(gm_mgObserver, 1, gm_mgDedicated, gm_mgSplitScreenCfg, TRANS("Observer:"), astrNoYes);
  gm_mgObserver.mg_strTip = TRANS("select to join in for observing, not for playing");

  // Initialize "Number of players" trigger.
  TRIGGER_MG(gm_mgSplitScreenCfg, 2, gm_mgObserver, gm_mgPlayer0Change, TRANS("Number of players:"), astrSplitScreenRadioTexts);
  gm_mgSplitScreenCfg.mg_strTip = TRANS("choose more than one player to play in split screen");

  // Initialize players selection buttons.
  gm_mgPlayer0Change.mg_iCenterI = -1;
  gm_mgPlayer1Change.mg_iCenterI = -1;
  gm_mgPlayer2Change.mg_iCenterI = -1;
  gm_mgPlayer3Change.mg_iCenterI = -1;

  gm_mgPlayer0Change.mg_boxOnScreen = BoxMediumMiddle(4);
  gm_mgPlayer1Change.mg_boxOnScreen = BoxMediumMiddle(5);
  gm_mgPlayer2Change.mg_boxOnScreen = BoxMediumMiddle(6);
  gm_mgPlayer3Change.mg_boxOnScreen = BoxMediumMiddle(7);

  gm_mgPlayer0Change.mg_strTip = gm_mgPlayer1Change.mg_strTip = gm_mgPlayer2Change.mg_strTip = gm_mgPlayer3Change.mg_strTip = TRANS("select profile for this player");

  gm_mgNotes.mg_boxOnScreen = BoxMediumRow(9.0);
  gm_mgNotes.mg_bfsFontSize = BFS_MEDIUM;
  gm_mgNotes.mg_iCenterI = -1;
  gm_mgNotes.SetEnabled(FALSE);
  gm_mgNotes.mg_bLabel = TRUE;
  gm_mgNotes.mg_strText = "";

  // Add components.
  AddChild(&gm_mgPlayer0Change);
  AddChild(&gm_mgPlayer1Change);
  AddChild(&gm_mgPlayer2Change);
  AddChild(&gm_mgPlayer3Change);
  AddChild(&gm_mgNotes);

  ADD_GADGET(gm_mgStart, BoxMediumRow(11), &gm_mgSplitScreenCfg, &gm_mgPlayer0Change, NULL, NULL, TRANS("START"));
  gm_mgStart.mg_bfsFontSize = BFS_LARGE;
  gm_mgStart.mg_iCenterI = 0;
}

extern void SelectPlayersFillMenu(void);

static void SelectPlayersApplyMenu(void)
{
  CSelectPlayersMenu &gmCurrent = _pGUIM->gmSelectPlayersMenu;

  if (gmCurrent.gm_bAllowDedicated && gmCurrent.gm_mgDedicated.mg_iSelected) {
    _pGame->gm_MenuSplitScreenCfg = CGame::SSC_DEDICATED;
    return;
  }

  if (gmCurrent.gm_bAllowObserving && gmCurrent.gm_mgObserver.mg_iSelected) {
    _pGame->gm_MenuSplitScreenCfg = CGame::SSC_OBSERVER;
    return;
  }

  _pGame->gm_MenuSplitScreenCfg = (enum CGame::SplitScreenCfg) gmCurrent.gm_mgSplitScreenCfg.mg_iSelected;
}

void CSelectPlayersMenu::StartMenu(void)
{
  CGameMenu::StartMenu();
  SelectPlayersFillMenu();
  SelectPlayersApplyMenu();
}

void CSelectPlayersMenu::EndMenu(void)
{
  SelectPlayersApplyMenu();
  CGameMenu::EndMenu();
}

static void UpdateSelectPlayers(INDEX i)
{
  SelectPlayersApplyMenu();
  SelectPlayersFillMenu();
}

extern void StartSplitScreenGame(void);
extern void StartNetworkGame(void);
extern void JoinNetworkGame(void);

// --------------------------------------------------------------------------------------
// [SSE]
// Returns TRUE if event was handled.
// --------------------------------------------------------------------------------------
BOOL CSelectPlayersMenu::OnEvent(const SEvent& event)
{
  if (event.EventType == EET_GUI_EVENT)
  {
    if (event.GuiEvent.EventType == EGET_TRIGGERED)
    {
      if (event.GuiEvent.Caller == &gm_mgStart)
      {
        if (gm_pgmParentMenu == &_pGUIM->gmSplitStartMenu) {
          StartSplitScreenGame();

        } else if (gm_pgmParentMenu == &_pGUIM->gmNetworkStartMenu) {
          StartNetworkGame();

        } else if (gm_pgmParentMenu == &_pGUIM->gmLoadSaveMenu) {
          if (_pGUIM->gmLoadSaveMenu.gm_pgmParentMenu == &_pGUIM->gmSplitScreenMenu) {
            StartSplitScreenGameLoad();
          } else {
            StartNetworkLoadGame();
          }

        } else if (gm_pgmParentMenu == &_pGUIM->gmNetworkOpenMenu || gm_pgmParentMenu == &_pGUIM->gmServersMenu) {
          JoinNetworkGame();
        }
        
        return TRUE;
      }
    
    } else if (event.GuiEvent.EventType == EGET_CHANGED) {

      INDEX iNewValue = event.GuiEvent.IntValue;
      
      if (event.GuiEvent.Caller == &gm_mgDedicated) {
        UpdateSelectPlayers(iNewValue);
        return TRUE;

      } else if (event.GuiEvent.Caller == &gm_mgObserver) {
        UpdateSelectPlayers(iNewValue);
        return TRUE;

      } else if (event.GuiEvent.Caller == &gm_mgSplitScreenCfg) {
        UpdateSelectPlayers(iNewValue);
        return TRUE;
      }
    }
  }
  
  if (CGameMenu::OnEvent(event)) {
    return TRUE;
  }
  
  return m_pParent ? m_pParent->OnEvent(event) : FALSE;
}