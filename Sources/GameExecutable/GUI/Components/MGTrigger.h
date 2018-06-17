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

#ifndef SE_INCL_MENU_GADGET_TRIGGER_H
#define SE_INCL_MENU_GADGET_TRIGGER_H
#ifdef PRAGMA_ONCE
  #pragma once
#endif

#include "MenuGadget.h"

// --------------------------------------------------------------------------------------
// Class that provides clickable label with multiple states.
// --------------------------------------------------------------------------------------
class CMGTrigger : public CMenuGadget
{
  public:
    CTString mg_strLabel;   // Text label.
    CTString mg_strValue;   // Current selected text.
    CTString *mg_astrTexts; // List of variants.
    INDEX mg_ctTexts;       // Number of variants.
    INDEX mg_iSelected;     // Selected index.
    INDEX mg_iCenterI;
    BOOL mg_bVisual; // Used for togglable textures.

  public:
    // Default constructor. Constructs component without text.
    CMGTrigger(void);
    
    // Constructs component with the text.
    CMGTrigger(const CTString &strText);

    void ApplyCurrentSelection(void);
    void OnSetNextInList(int iVKey);
    BOOL OnKeyDown(int iVKey); // return TRUE if handled
    void Render(CDrawPort *pdp);
};

#endif  /* include-once check. */