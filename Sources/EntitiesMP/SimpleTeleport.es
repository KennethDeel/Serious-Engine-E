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

229
%{
  #include "StdH.h"
  #include "EntitiesMP/Player.h"
  #include "EntitiesMP/PlayerWeapons.h"
%}

class CSimpleTeleport: CRationalEntity {
name      "SimpleTeleport";
thumbnail "Thumbnails\\SimpleTeleport.tbn";
features  "HasName", "IsTargetable";

properties:
   1 CTString m_strName "Name" 'N'      = "Simple Teleport",
   3 CTString m_strDescription = "",
   
   4 BOOL m_bActive              "Active" 'A' = TRUE,
   5 BOOL m_bDebugMessages "Debug Messages" = FALSE,

   6 CEntityPointer m_penEnityToTP   "Entity To Teleport" 'E',
   7 CEntityPointer m_penDestination "Destination Target" 'T',

  10 BOOL m_bPenCausedAsEntityToTP  "EntityToTP=penCaused" = FALSE,
  11 BOOL m_bPenCausedAsDestination "Destination=penCaused" = FALSE,
  12 BOOL m_bThisAsDestination      "Destination=this" = FALSE,
   
  25 BOOL m_bForceStop              "Force stop" 'F' = FALSE,
  26 BOOL m_bSaveOrintation         "Save Orientation" = FALSE,
  27 BOOL m_bTelefrag               "Telefrag" 'F' = TRUE,

  30 BOOL m_bSpawnEffect "Spawn Effect" 'X' = FALSE,

components:
  1 model   MODEL_TELEPORTER   "Models\\Editor\\SimpleTeleport.mdl",
  2 texture TEXTURE_TELEPORTER "Models\\Editor\\SimpleTeleport.tex",
  3 class   CLASS_BASIC_EFFECT  "Classes\\BasicEffect.ecl",

functions:
  const CTString &GetDescription(void) const {
    return m_strDescription;
  }

  void DoTeleportation(const ETrigger &eTrigger) {
    CEntity *penEntityToTP = m_penEnityToTP;

    if (m_bPenCausedAsEntityToTP && eTrigger.penCaused) {
      penEntityToTP = eTrigger.penCaused;
    }

    // If no any entity to teleport then stop.
    if (penEntityToTP == NULL) {
      if (m_bDebugMessages) {
        CPrintF(TRANS("%s : Haven't any entity to telport!\n"), m_strName);
      }

      return;
    }
    
    CEntity *penDestination = m_penDestination;

    if (m_bPenCausedAsDestination && eTrigger.penCaused) {
      penDestination = eTrigger.penCaused;
    }

    BOOL bThis = FALSE;

    if (m_bThisAsDestination) {
      penDestination = this;
      bThis = TRUE;
    }

    // If no destination where teleport then stop.
    if (penDestination == NULL) {
      if (m_bDebugMessages) {
        CPrintF(TRANS("%s : Haven't destination to telport!\n"), m_strName);
      }

      return;
    }

    if (m_bDebugMessages) {
      if (bThis) {
        CPrintF(TRANS("%s : Teleporting %s to %s location.\n"), m_strName, penEntityToTP->GetName(), penDestination->GetName());
      } else {
        CPrintF(TRANS("%s : Teleporting %s to this teleporter.\n"), m_strName, penEntityToTP->GetName());
      }
    }

    CPlacement3D pl = penDestination->GetPlacement();
    if (m_bSaveOrintation) {
      pl.pl_OrientationAngle = penEntityToTP->GetPlacement().pl_OrientationAngle;
    }

    // teleport back
    penEntityToTP->Teleport(pl, m_bTelefrag);

    if (m_bForceStop && (penEntityToTP->GetPhysicsFlags()&EPF_MOVABLE) ) {
      ((CMovableEntity*)&*penEntityToTP)->ForceFullStop();
    }

    // spawn teleport effect
    if (m_bSpawnEffect) {
      ESpawnEffect ese;
      ese.colMuliplier = C_WHITE|CT_OPAQUE;
      ese.betType = BET_TELEPORT;
      ese.vNormal = FLOAT3D(0,1,0);
      FLOATaabbox3D box;
      penEntityToTP->GetBoundingBox(box);
      FLOAT fEntitySize = box.Size().MaxNorm()*2;
      ese.vStretch = FLOAT3D(fEntitySize, fEntitySize, fEntitySize);

      CEntityPointer penEffect = CreateEntity(pl, CLASS_BASIC_EFFECT);
      penEffect->Initialize(ese);
    }
  }
  
procedures:
  Main()
  {
    InitAsEditorModel();
    SetPhysicsFlags(EPF_MODEL_IMMATERIAL);
    SetCollisionFlags(ECF_IMMATERIAL);

    // set appearance
    SetModel(MODEL_TELEPORTER);
    SetModelMainTexture(TEXTURE_TELEPORTER);
  
    autowait(0.1f);
  
    while (TRUE) {
      wait(_pTimer->TickQuantum) {
        on (EBegin) : { 
          resume;
        }

        on(ETrigger eTrigger) : {
          if (m_bActive) {
            DoTeleportation(eTrigger);
          }

          resume;
        }

        on (EActivate) : {
          m_bActive = TRUE;
          resume;
        }

        on (EDeactivate) : {
          m_bActive = FALSE;
          resume;
        }

        on(ETimer) : { stop; }
      }
    }
  }
};

