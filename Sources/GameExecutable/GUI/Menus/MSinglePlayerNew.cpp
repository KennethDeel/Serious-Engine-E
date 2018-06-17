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
#include "MSinglePlayerNew.h"

// --------------------------------------------------------------------------------------
// Intializes single player new menu.
// --------------------------------------------------------------------------------------
void CSinglePlayerNewMenu::Initialize_t(void)
{
  // Initialize title label.
  gm_mgTitle.mg_strText = TRANS("NEW GAME");
  gm_mgTitle.mg_boxOnScreen = BoxTitle();

  // Initialize "Tourist" button.
  gm_mgTourist.mg_strText = TRANS("TOURIST");
  gm_mgTourist.mg_bfsFontSize = BFS_LARGE;
  gm_mgTourist.mg_boxOnScreen = BoxBigRow(0.0f);
  gm_mgTourist.mg_strTip = TRANS("for non-FPS players");
  gm_mgTourist.mg_pmgUp = &gm_mgSerious;
  gm_mgTourist.mg_pmgDown = &gm_mgEasy;

  // Initialize "Easy" button.
  gm_mgEasy.mg_strText = TRANS("EASY");
  gm_mgEasy.mg_bfsFontSize = BFS_LARGE;
  gm_mgEasy.mg_boxOnScreen = BoxBigRow(1.0f);
  gm_mgEasy.mg_strTip = TRANS("for unexperienced FPS players");
  gm_mgEasy.mg_pmgUp = &gm_mgTourist;
  gm_mgEasy.mg_pmgDown = &gm_mgMedium;

  // Initialize "Medium" button.
  gm_mgMedium.mg_strText = TRANS("NORMAL");
  gm_mgMedium.mg_bfsFontSize = BFS_LARGE;
  gm_mgMedium.mg_boxOnScreen = BoxBigRow(2.0f);
  gm_mgMedium.mg_strTip = TRANS("for experienced FPS players");
  gm_mgMedium.mg_pmgUp = &gm_mgEasy;
  gm_mgMedium.mg_pmgDown = &gm_mgHard;

  // Initialize "Hard" button.
  gm_mgHard.mg_strText = TRANS("HARD");
  gm_mgHard.mg_bfsFontSize = BFS_LARGE;
  gm_mgHard.mg_boxOnScreen = BoxBigRow(3.0f);
  gm_mgHard.mg_strTip = TRANS("for experienced Serious Sam players");
  gm_mgHard.mg_pmgUp = &gm_mgMedium;
  gm_mgHard.mg_pmgDown = &gm_mgSerious;

  // Initialize "Serious" button.
  gm_mgSerious.mg_strText = TRANS("SERIOUS");
  gm_mgSerious.mg_bfsFontSize = BFS_LARGE;
  gm_mgSerious.mg_boxOnScreen = BoxBigRow(4.0f);
  gm_mgSerious.mg_strTip = TRANS("are you serious?");
  gm_mgSerious.mg_pmgUp = &gm_mgHard;
  gm_mgSerious.mg_pmgDown = &gm_mgTourist;

  // Initialize "Mental" button.
  gm_mgMental.mg_strText = TRANS("MENTAL");
  gm_mgMental.mg_bfsFontSize = BFS_LARGE;
  gm_mgMental.mg_boxOnScreen = BoxBigRow(5.0f);
  gm_mgMental.mg_strTip = TRANS("you are not serious!");
  gm_mgMental.mg_pmgUp = &gm_mgSerious;
  gm_mgMental.mg_pmgDown = &gm_mgTourist;
  gm_mgMental.mg_bBlinking = FALSE;
  
  // Initialize "Serious Mental" button.
  gm_mgSeriousMental.mg_strText = TRANS("^cFF0000SERIOUS MENTAL");
  gm_mgSeriousMental.mg_bfsFontSize = BFS_LARGE;
  gm_mgSeriousMental.mg_boxOnScreen = BoxBigRow(6.0f);
  gm_mgSeriousMental.mg_strTip = TRANS("you are too serious!");
  gm_mgSeriousMental.mg_pmgUp = &gm_mgMental;
  gm_mgSeriousMental.mg_pmgDown = &gm_mgTourist;
  gm_mgSeriousMental.mg_bBlinking = TRUE;
  
  // Add components.
  AddChild(&gm_mgTitle);
  AddChild(&gm_mgTourist);
  AddChild(&gm_mgEasy);
  AddChild(&gm_mgMedium);
  AddChild(&gm_mgHard);
  AddChild(&gm_mgSerious);
  AddChild(&gm_mgMental);
  AddChild(&gm_mgSeriousMental);
}

void CSinglePlayerNewMenu::StartMenu(void)
{
  CGameMenu::StartMenu();
  extern INDEX sam_bMentalActivated;

  if (sam_bMentalActivated) {
    gm_mgMental.Appear();
    gm_mgSerious.mg_pmgDown = &gm_mgMental;
    gm_mgTourist.mg_pmgUp = &gm_mgMental;
    
    // [SSE] Serious Mental
    gm_mgMental.mg_pmgDown = &gm_mgSeriousMental;
    gm_mgTourist.mg_pmgUp = &gm_mgSeriousMental;
    //
    
  } else {
    gm_mgMental.Disappear();
    gm_mgSeriousMental.Disappear(); // [SSE] Serious Mental

    gm_mgSerious.mg_pmgDown = &gm_mgTourist;
    gm_mgTourist.mg_pmgUp = &gm_mgSerious;
  }
}

// [SSE]
BOOL CSinglePlayerNewMenu::OnEvent(const SEvent& event)
{
  if (event.EventType == EET_GUI_EVENT)
  {
    INDEX iDifficulty = -666;
    
    if (event.GuiEvent.Caller == &gm_mgTourist) {
      iDifficulty = CSessionProperties::GD_TOURIST;

    } else if (event.GuiEvent.Caller == &gm_mgEasy) {
      iDifficulty = CSessionProperties::GD_EASY;

    } else if (event.GuiEvent.Caller == &gm_mgMedium) {
      iDifficulty = CSessionProperties::GD_NORMAL;

    } else if (event.GuiEvent.Caller == &gm_mgHard) {
      iDifficulty = CSessionProperties::GD_HARD;

    } else if (event.GuiEvent.Caller == &gm_mgSerious) {
      iDifficulty = CSessionProperties::GD_EXTREME;
    
    } else if (event.GuiEvent.Caller == &gm_mgMental) {
      iDifficulty = CSessionProperties::GD_EXTREME + 1;

    } else if (event.GuiEvent.Caller == &gm_mgSeriousMental) {
      iDifficulty = CSessionProperties::GD_EXTREME + 2;
    }

    if (iDifficulty >= CSessionProperties::GD_TOURIST) {
      _pShell->SetINDEX("gam_iStartDifficulty", iDifficulty);
      _pShell->SetINDEX("gam_iStartMode", CSessionProperties::GM_COOPERATIVE);
      
      extern void StartSinglePlayerGame(void);
      StartSinglePlayerGame();
      
      return TRUE;
    }
  }
  
  if (CGameMenu::OnEvent(event)) {
    return TRUE;
  }
  
  return m_pParent ? m_pParent->OnEvent(event) : FALSE;
}
