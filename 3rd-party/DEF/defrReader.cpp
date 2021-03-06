// *****************************************************************************
// *****************************************************************************
// Copyright 2013 - 2014, Cadence Design Systems
// 
// This  file  is  part  of  the  Cadence  LEF/DEF  Open   Source
// Distribution,  Product Version 5.8. 
// 
// Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
// 
//        http://www.apache.org/licenses/LICENSE-2.0
// 
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
//    implied. See the License for the specific language governing
//    permissions and limitations under the License.
// 
// For updates, support, or to become part of the LEF/DEF Community,
// check www.openeda.org for details.
// 
//  $Author: dell $
//  $Revision: #4 $
//  $Date: 2014/06/05 $
//  $State:  $
// *****************************************************************************
// *****************************************************************************

#include "defrReader.hpp"
#include "defiProp.hpp"
#include "defiPropType.hpp"
#include "defrCallBacks.hpp"
#include "defiDebug.hpp"
#include "defiMisc.hpp"
#include "defrData.hpp"
#include "defrSettings.hpp"

#include <stdlib.h>
#include <string.h>
#include "lex.h"
#include "defiUtil.hpp"
#include "defrCallBacks.hpp"

#define NODEFMSG 4013     // (9012 + 1) - 5000, def msg starts at 5000

#if _DEBUG
# define ASSERT_INIT assert_def_init(__FILE__, __LINE__)
#else
# define ASSERT_INIT 1
#endif

BEGIN_LEFDEF_PARSER_NAMESPACE

void
assert_def_init(const char  *file,
                int         line)
{
    if (defSettings && defCallbacks) {
        return;
    }

    fprintf(stderr, "ERROR: Attempt to configure DEF parser before defrInit() call: %s %d\n", file, line);
    exit(100);
}


int
defrCountUnused(defrCallbackType_e  e,
                void                *v,
                defiUserData        d)
{
    ASSERT_INIT;
    int i;
    if (defiDebug(23))
        printf("Count %d, 0x%p, 0x%p\n", (int) e, v, d);
    i = (int) e;
    if (i <= 0 || i >= CBMAX) {
        return 1;
    }
    defSettings->UnusedCallbacks[i] += 1;

    return 0;
}


const char *
typeToString(defrCallbackType_e num)
{
    switch ((int) num) {

    case defrUnspecifiedCbkType:
        return "Unspecified";
    case defrDesignStartCbkType:
        return "Design Start";
    case defrTechNameCbkType:
        return "Tech Name";
    case defrPropCbkType:
        return "Property";
    case defrPropDefEndCbkType:
        return "Property Definitions Section End";
    case defrPropDefStartCbkType:
        return "Property Definitions Section Start";
    case defrFloorPlanNameCbkType:
        return "FloorPlanName";
    case defrArrayNameCbkType:
        return "Array Name";
    case defrUnitsCbkType:
        return "Units";
    case defrDividerCbkType:
        return "Divider";
    case defrBusBitCbkType:
        return "BusBit Character";
    case defrSiteCbkType:
        return "Site";
    case defrComponentMaskShiftLayerCbkType:
        return "ComponentMaskShiftLayer";
    case defrComponentStartCbkType:
        return "Components Section Start";
    case defrComponentCbkType:
        return "Component";
    case defrComponentEndCbkType:
        return "Components Section End";
    case defrNetStartCbkType:
        return "Nets Section Start";
    case defrNetCbkType:
        return "Net";
    case defrNetNameCbkType:
        return "Net Name";
    case defrNetNonDefaultRuleCbkType:
        return "Net Nondefaultrule";
    case defrNetSubnetNameCbkType:
        return "Net Subnet Name";
    case defrNetEndCbkType:
        return "Nets Section End";
    case defrPathCbkType:
        return "Path";
    case defrVersionCbkType:
        return "Version";
    case defrVersionStrCbkType:
        return "Version";
    case defrComponentExtCbkType:
        return "Component User Extention";
    case defrPinExtCbkType:
        return "Pin User Extension";
    case defrViaExtCbkType:
        return "Via User Extension";
    case defrNetConnectionExtCbkType:
        return "NetConnection User Extention";
    case defrNetExtCbkType:
        return "Net User Extension";
    case defrGroupExtCbkType:
        return "Group User Extension";
    case defrScanChainExtCbkType:
        return "ScanChain User Extension";
    case defrIoTimingsExtCbkType:
        return "IoTimings User Extension";
    case defrPartitionsExtCbkType:
        return "Partitions User Extension";
    case defrHistoryCbkType:
        return "History";
    case defrDieAreaCbkType:
        return "DieArea";
    case defrCanplaceCbkType:
        return "Canplace";
    case defrCannotOccupyCbkType:
        return "CannotOccupy";
    case defrPinCapCbkType:
        return "PinCap";
    case defrDefaultCapCbkType:
        return "DefaultCap";
    case defrStartPinsCbkType:
        return "Start Pins Section";
    case defrPinCbkType:
        return "Pin";
    case defrPinEndCbkType:
        return "End Pins Section";
    case defrRowCbkType:
        return "Row";
    case defrTrackCbkType:
        return "Track";
    case defrGcellGridCbkType:
        return "GcellGrid";
    case defrViaStartCbkType:
        return "Start Vias Section";
    case defrViaCbkType:
        return "Via";
    case defrViaEndCbkType:
        return "End Vias Section";
    case defrRegionStartCbkType:
        return "Region Section Start";
    case defrRegionCbkType:
        return "Region";
    case defrRegionEndCbkType:
        return "Region Section End";
    case defrSNetStartCbkType:
        return "Special Net Section Start";
    case defrSNetCbkType:
        return "Special Net";
    case defrSNetEndCbkType:
        return "Special Net Section End";
    case defrGroupsStartCbkType:
        return "Groups Section Start";
    case defrGroupNameCbkType:
        return "Group Name";
    case defrGroupMemberCbkType:
        return "Group Member";
    case defrGroupCbkType:
        return "Group";
    case defrGroupsEndCbkType:
        return "Groups Section End";
    case defrAssertionsStartCbkType:
        return "Assertions Section Start";
    case defrAssertionCbkType:
        return "Assertion";
    case defrAssertionsEndCbkType:
        return "Assertions Section End";
    case defrConstraintsStartCbkType:
        return "Constraints Section Start";
    case defrConstraintCbkType:
        return "Constraint";
    case defrConstraintsEndCbkType:
        return "Constraints Section End";
    case defrScanchainsStartCbkType:
        return "Scanchains Section Start";
    case defrScanchainCbkType:
        return "Scanchain";
    case defrScanchainsEndCbkType:
        return "Scanchains Section End";
    case defrIOTimingsStartCbkType:
        return "IOTimings Section Start";
    case defrIOTimingCbkType:
        return "IOTiming";
    case defrIOTimingsEndCbkType:
        return "IOTimings Section End";
    case defrFPCStartCbkType:
        return "Floor Plan Constraints Section Start";
    case defrFPCCbkType:
        return "Floor Plan Constraint";
    case defrFPCEndCbkType:
        return "Floor Plan Constraints Section End";
    case defrTimingDisablesStartCbkType:
        return "TimingDisables Section Start";
    case defrTimingDisableCbkType:
        return "TimingDisable";
    case defrTimingDisablesEndCbkType:
        return "TimingDisables Section End";
    case defrPartitionsStartCbkType:
        return "Partitions Section Start";
    case defrPartitionCbkType:
        return "Partition";
    case defrPartitionsEndCbkType:
        return "Partitions Section End";
    case defrPinPropStartCbkType:
        return "PinProp Section Start";
    case defrPinPropCbkType:
        return "PinProp";
    case defrPinPropEndCbkType:
        return "PinProp Section End";
    case defrCaseSensitiveCbkType:
        return "CaseSensitive";
    case defrBlockageStartCbkType:
        return "Blockage Section Start";
    case defrBlockageCbkType:
        return "Blockage";
    case defrBlockageEndCbkType:
        return "Blockage Section End";
    case defrSlotStartCbkType:
        return "Slots Section Start";
    case defrSlotCbkType:
        return "Slots";
    case defrSlotEndCbkType:
        return "Slots Section End";
    case defrFillStartCbkType:
        return "Fills Section Start";
    case defrFillCbkType:
        return "Fills";
    case defrFillEndCbkType:
        return "Fills Section End";
    case defrNonDefaultStartCbkType:
        return "NonDefaultRule Section Start";
    case defrNonDefaultCbkType:
        return "NonDefault";
    case defrNonDefaultEndCbkType:
        return "NonDefaultRule Section End";
    case defrStylesStartCbkType:
        return "Styles Section Start";
    case defrStylesCbkType:
        return "Styles";
    case defrStylesEndCbkType:
        return "Styles Section End";
    case defrExtensionCbkType:
        return "Extension";

        // NEW CALLBACK - If you created a new callback then add the
        // type enums that you created here for debug printing.     

    case defrDesignEndCbkType:
        return "DesignEnd";
    default:
        break;
    }
    return "BOGUS";
}

int
defrCatchAll(defrCallbackType_e typ,
             void               *data,
             defiUserData       ud)
{
    ASSERT_INIT;
    if (defSettings->Debug[3])
        printf("CatchAll(%d, 0x%p, 0x%p)\n", typ, data, ud);

    if ((int) typ >= 0 && (int) typ < CBMAX) {
        defSettings->UnusedCallbacks[(int) typ] += 1;
    } else {
        defSettings->UnusedCallbacks[0] += 1;
        return 1;
    }

    return 0;
}

// *****************************************************************
// Wrapper functions.
//
// These functions provide access to the class member functions
// for compatibility with previous parser kits.
// *****************************************************************
int
defrInit()
{
    defrCallbacks::reset();
    defrSettings::reset();

    defSettings->UserData = 0;


    for (int i = 0; i < CBMAX; i++)
        defSettings->UnusedCallbacks[i] = 0;

    return 0;
}

// obsoleted now 
int
defrReset()
{
    return 0;
}

int 
defrClear()
{
    delete defData;
    defData = NULL;

    delete defCallbacks;
    defCallbacks = NULL;

    delete defSettings;
    defSettings = NULL;

    return 0;
}


void
defrSetRegisterUnusedCallbacks()
{
    ASSERT_INIT;
    defrSetUnusedCallbacks(defrCountUnused);
}

void
defrSetUnusedCallbacks(defrVoidCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->SetUnusedCallbacks(f);
}

void
defrUnsetCallbacks()
{
    ASSERT_INIT;
    defrCallbacks::reset();
}

void
defrPrintUnusedCallbacks(FILE *log)
{
    int i;
    int first = 1;

    for (i = 0; i < CBMAX; i++) {
        if (defSettings->UnusedCallbacks[i]) {
            if (first) {
                fprintf(log,
                        "WARNING (DEFPARS-5001): DEF statement found in the def file with no callback set.\n");
                first = 0;
            }
            fprintf(log, "%5d %s\n", defSettings->UnusedCallbacks[i],
                    typeToString((defrCallbackType_e) i));
        }
    }
}

// obsoleted
int
defrReleaseNResetMemory()
{
    return 0;
}

void
defrUnsetArrayNameCbk()
{
    ASSERT_INIT;
    defCallbacks->ArrayNameCbk = NULL;
    ;
}

void
defrUnsetAssertionCbk()
{
    ASSERT_INIT;
    defCallbacks->AssertionCbk = NULL;
    ;
}

void
defrUnsetAssertionsStartCbk()
{
    ASSERT_INIT;
    defCallbacks->AssertionsStartCbk = NULL;
    ;
}

void
defrUnsetAssertionsEndCbk()
{
    ASSERT_INIT;
    defCallbacks->AssertionsEndCbk = NULL;
    ;
}

void
defrUnsetBlockageCbk()
{
    ASSERT_INIT;
    defCallbacks->BlockageCbk = NULL;
    ;
}

void
defrUnsetBlockageStartCbk()
{
    ASSERT_INIT;
    defCallbacks->BlockageStartCbk = NULL;
    ;
}

void
defrUnsetBlockageEndCbk()
{
    ASSERT_INIT;
    defCallbacks->BlockageEndCbk = NULL;
    ;
}

void
defrUnsetBusBitCbk()
{
    ASSERT_INIT;
    defCallbacks->BusBitCbk = NULL;
    ;
}

void
defrUnsetCannotOccupyCbk()
{
    ASSERT_INIT;
    defCallbacks->CannotOccupyCbk = NULL;
    ;
}

void
defrUnsetCanplaceCbk()
{
    ASSERT_INIT;
    defCallbacks->CanplaceCbk = NULL;
    ;
}

void
defrUnsetCaseSensitiveCbk()
{
    ASSERT_INIT;
    defCallbacks->CaseSensitiveCbk = NULL;
    ;
}

void
defrUnsetComponentCbk()
{
    ASSERT_INIT;
    defCallbacks->ComponentCbk = NULL;
    ;
}

void
defrUnsetComponentExtCbk()
{
    ASSERT_INIT;
    defCallbacks->ComponentExtCbk = NULL;
    ;
}

void
defrUnsetComponentStartCbk()
{
    ASSERT_INIT;
    defCallbacks->ComponentStartCbk = NULL;
    ;
}

void
defrUnsetComponentEndCbk()
{
    ASSERT_INIT;
    defCallbacks->ComponentEndCbk = NULL;
    ;
}

void
defrUnsetConstraintCbk()
{
    ASSERT_INIT;
    defCallbacks->ConstraintCbk = NULL;
    ;
}

void
defrUnsetConstraintsStartCbk()
{
    ASSERT_INIT;
    defCallbacks->ConstraintsStartCbk = NULL;
    ;
}

void
defrUnsetConstraintsEndCbk()
{
    ASSERT_INIT;
    defCallbacks->ConstraintsEndCbk = NULL;
    ;
}

void
defrUnsetDefaultCapCbk()
{
    ASSERT_INIT;
    defCallbacks->DefaultCapCbk = NULL;
    ;
}

void
defrUnsetDesignCbk()
{
    ASSERT_INIT;
    defCallbacks->DesignCbk = NULL;
    ;
}

void
defrUnsetDesignEndCbk()
{
    ASSERT_INIT;
    defCallbacks->DesignEndCbk = NULL;
    ;
}

void
defrUnsetDieAreaCbk()
{
    ASSERT_INIT;
    defCallbacks->DieAreaCbk = NULL;
    ;
}

void
defrUnsetDividerCbk()
{
    ASSERT_INIT;
    defCallbacks->DividerCbk = NULL;
    ;
}

void
defrUnsetExtensionCbk()
{
    ASSERT_INIT;
    defCallbacks->ExtensionCbk = NULL;
    ;
}

void
defrUnsetFillCbk()
{
    ASSERT_INIT;
    defCallbacks->FillCbk = NULL;
    ;
}

void
defrUnsetFillStartCbk()
{
    ASSERT_INIT;
    defCallbacks->FillStartCbk = NULL;
    ;
}

void
defrUnsetFillEndCbk()
{
    ASSERT_INIT;
    defCallbacks->FillEndCbk = NULL;
    ;
}

void
defrUnsetFPCCbk()
{
    ASSERT_INIT;
    defCallbacks->FPCCbk = NULL;
    ;
}

void
defrUnsetFPCStartCbk()
{
    ASSERT_INIT;
    defCallbacks->FPCStartCbk = NULL;
    ;
}

void
defrUnsetFPCEndCbk()
{
    ASSERT_INIT;
    defCallbacks->FPCEndCbk = NULL;
    ;
}

void
defrUnsetFloorPlanNameCbk()
{
    ASSERT_INIT;
    defCallbacks->FloorPlanNameCbk = NULL;
    ;
}

void
defrUnsetGcellGridCbk()
{
    ASSERT_INIT;
    defCallbacks->GcellGridCbk = NULL;
    ;
}

void
defrUnsetGroupCbk()
{
    ASSERT_INIT;
    defCallbacks->GroupCbk = NULL;
    ;
}

void
defrUnsetGroupExtCbk()
{
    ASSERT_INIT;
    defCallbacks->GroupExtCbk = NULL;
    ;
}

void
defrUnsetGroupMemberCbk()
{
    ASSERT_INIT;
    defCallbacks->GroupMemberCbk = NULL;
    ;
}

void
defrUnsetComponentMaskShiftLayerCbk()
{
    ASSERT_INIT;
    defCallbacks->ComponentMaskShiftLayerCbk = NULL;
    ;
}

void
defrUnsetGroupNameCbk()
{
    ASSERT_INIT;
    defCallbacks->GroupNameCbk = NULL;
    ;
}

void
defrUnsetGroupsStartCbk()
{
    ASSERT_INIT;
    defCallbacks->GroupsStartCbk = NULL;
    ;
}

void
defrUnsetGroupsEndCbk()
{
    ASSERT_INIT;
    defCallbacks->GroupsEndCbk = NULL;
    ;
}

void
defrUnsetHistoryCbk()
{
    ASSERT_INIT;
    defCallbacks->HistoryCbk = NULL;
    ;
}

void
defrUnsetIOTimingCbk()
{
    ASSERT_INIT;
    defCallbacks->IOTimingCbk = NULL;
    ;
}

void
defrUnsetIOTimingsStartCbk()
{
    ASSERT_INIT;
    defCallbacks->IOTimingsStartCbk = NULL;
    ;
}

void
defrUnsetIOTimingsEndCbk()
{
    ASSERT_INIT;
    defCallbacks->IOTimingsEndCbk = NULL;
    ;
}

void
defrUnsetIOTimingsExtCbk()
{
    ASSERT_INIT;
    defCallbacks->IoTimingsExtCbk = NULL;
    ;
}

void
defrUnsetNetCbk()
{
    ASSERT_INIT;
    defCallbacks->NetCbk = NULL;
    ;
}

void
defrUnsetNetNameCbk()
{
    ASSERT_INIT;
    defCallbacks->NetNameCbk = NULL;
    ;
}

void
defrUnsetNetNonDefaultRuleCbk()
{
    ASSERT_INIT;
    defCallbacks->NetNonDefaultRuleCbk = NULL;
    ;
}

void
defrUnsetNetConnectionExtCbk()
{
    ASSERT_INIT;
    defCallbacks->NetConnectionExtCbk = NULL;
    ;
}

void
defrUnsetNetExtCbk()
{
    ASSERT_INIT;
    defCallbacks->NetExtCbk = NULL;
    ;
}

void
defrUnsetNetPartialPathCbk()
{
    ASSERT_INIT;
    defCallbacks->NetPartialPathCbk = NULL;
    ;
}

void
defrUnsetNetSubnetNameCbk()
{
    ASSERT_INIT;
    defCallbacks->NetSubnetNameCbk = NULL;
    ;
}

void
defrUnsetNetStartCbk()
{
    ASSERT_INIT;
    defCallbacks->NetStartCbk = NULL;
    ;
}

void
defrUnsetNetEndCbk()
{
    ASSERT_INIT;
    defCallbacks->NetEndCbk = NULL;
    ;
}

void
defrUnsetNonDefaultCbk()
{
    ASSERT_INIT;
    defCallbacks->NonDefaultCbk = NULL;
    ;
}

void
defrUnsetNonDefaultStartCbk()
{
    ASSERT_INIT;
    defCallbacks->NonDefaultStartCbk = NULL;
    ;
}

void
defrUnsetNonDefaultEndCbk()
{
    ASSERT_INIT;
    defCallbacks->NonDefaultEndCbk = NULL;
    ;
}

void
defrUnsetPartitionCbk()
{
    ASSERT_INIT;
    defCallbacks->PartitionCbk = NULL;
    ;
}

void
defrUnsetPartitionsExtCbk()
{
    ASSERT_INIT;
    defCallbacks->PartitionsExtCbk = NULL;
    ;
}

void
defrUnsetPartitionsStartCbk()
{
    ASSERT_INIT;
    defCallbacks->PartitionsStartCbk = NULL;
    ;
}

void
defrUnsetPartitionsEndCbk()
{
    ASSERT_INIT;
    defCallbacks->PartitionsEndCbk = NULL;
    ;
}

void
defrUnsetPathCbk()
{
    ASSERT_INIT;
    defCallbacks->PathCbk = NULL;
    ;
}

void
defrUnsetPinCapCbk()
{
    ASSERT_INIT;
    defCallbacks->PinCapCbk = NULL;
    ;
}

void
defrUnsetPinCbk()
{
    ASSERT_INIT;
    defCallbacks->PinCbk = NULL;
    ;
}

void
defrUnsetPinEndCbk()
{
    ASSERT_INIT;
    defCallbacks->PinEndCbk = NULL;
    ;
}

void
defrUnsetPinExtCbk()
{
    ASSERT_INIT;
    defCallbacks->PinExtCbk = NULL;
    ;
}

void
defrUnsetPinPropCbk()
{
    ASSERT_INIT;
    defCallbacks->PinPropCbk = NULL;
    ;
}

void
defrUnsetPinPropStartCbk()
{
    ASSERT_INIT;
    defCallbacks->PinPropStartCbk = NULL;
    ;
}

void
defrUnsetPinPropEndCbk()
{
    ASSERT_INIT;
    defCallbacks->PinPropEndCbk = NULL;
    ;
}

void
defrUnsetPropCbk()
{
    ASSERT_INIT;
    defCallbacks->PropCbk = NULL;
    ;
}

void
defrUnsetPropDefEndCbk()
{
    ASSERT_INIT;
    defCallbacks->PropDefEndCbk = NULL;
    ;
}

void
defrUnsetPropDefStartCbk()
{
    ASSERT_INIT;
    defCallbacks->PropDefStartCbk = NULL;
    ;
}

void
defrUnsetRegionCbk()
{
    ASSERT_INIT;
    defCallbacks->RegionCbk = NULL;
    ;
}

void
defrUnsetRegionStartCbk()
{
    ASSERT_INIT;
    defCallbacks->RegionStartCbk = NULL;
    ;
}

void
defrUnsetRegionEndCbk()
{
    ASSERT_INIT;
    defCallbacks->RegionEndCbk = NULL;
    ;
}

void
defrUnsetRowCbk()
{
    ASSERT_INIT;
    defCallbacks->RowCbk = NULL;
    ;
}

void
defrUnsetScanChainExtCbk()
{
    ASSERT_INIT;
    defCallbacks->ScanChainExtCbk = NULL;
    ;
}

void
defrUnsetScanchainCbk()
{
    ASSERT_INIT;
    defCallbacks->ScanchainCbk = NULL;
    ;
}

void
defrUnsetScanchainsStartCbk()
{
    ASSERT_INIT;
    defCallbacks->ScanchainsStartCbk = NULL;
    ;
}

void
defrUnsetScanchainsEndCbk()
{
    ASSERT_INIT;
    defCallbacks->ScanchainsEndCbk = NULL;
    ;
}

void
defrUnsetSiteCbk()
{
    ASSERT_INIT;
    defCallbacks->SiteCbk = NULL;
    ;
}

void
defrUnsetSlotCbk()
{
    ASSERT_INIT;
    defCallbacks->SlotCbk = NULL;
    ;
}

void
defrUnsetSlotStartCbk()
{
    ASSERT_INIT;
    defCallbacks->SlotStartCbk = NULL;
    ;
}

void
defrUnsetSlotEndCbk()
{
    ASSERT_INIT;
    defCallbacks->SlotEndCbk = NULL;
    ;
}

void
defrUnsetSNetWireCbk()
{
    ASSERT_INIT;
    defCallbacks->SNetWireCbk = NULL;
    ;
}

void
defrUnsetSNetCbk()
{
    ASSERT_INIT;
    defCallbacks->SNetCbk = NULL;
    ;
}

void
defrUnsetSNetStartCbk()
{
    ASSERT_INIT;
    defCallbacks->SNetStartCbk = NULL;
    ;
}

void
defrUnsetSNetEndCbk()
{
    ASSERT_INIT;
    defCallbacks->SNetEndCbk = NULL;
    ;
}

void
defrUnsetSNetPartialPathCbk()
{
    ASSERT_INIT;
    defCallbacks->SNetPartialPathCbk = NULL;
    ;
}

void
defrUnsetStartPinsCbk()
{
    ASSERT_INIT;
    defCallbacks->StartPinsCbk = NULL;
    ;
}

void
defrUnsetStylesCbk()
{
    ASSERT_INIT;
    defCallbacks->StylesCbk = NULL;
    ;
}

void
defrUnsetStylesStartCbk()
{
    ASSERT_INIT;
    defCallbacks->StylesStartCbk = NULL;
    ;
}

void
defrUnsetStylesEndCbk()
{
    ASSERT_INIT;
    defCallbacks->StylesEndCbk = NULL;
    ;
}

void
defrUnsetTechnologyCbk()
{
    ASSERT_INIT;
    defCallbacks->TechnologyCbk = NULL;
    ;
}

void
defrUnsetTimingDisableCbk()
{
    ASSERT_INIT;
    defCallbacks->TimingDisableCbk = NULL;
    ;
}

void
defrUnsetTimingDisablesStartCbk()
{
    ASSERT_INIT;
    defCallbacks->TimingDisablesStartCbk = NULL;
    ;
}

void
defrUnsetTimingDisablesEndCbk()
{
    ASSERT_INIT;
    defCallbacks->TimingDisablesEndCbk = NULL;
    ;
}

void
defrUnsetTrackCbk()
{
    ASSERT_INIT;
    defCallbacks->TrackCbk = NULL;
    ;
}

void
defrUnsetUnitsCbk()
{
    ASSERT_INIT;
    defCallbacks->UnitsCbk = NULL;
    ;
}

void
defrUnsetVersionCbk()
{
    ASSERT_INIT;
    defCallbacks->VersionCbk = NULL;
    ;
}

void
defrUnsetVersionStrCbk()
{
    ASSERT_INIT;
    defCallbacks->VersionStrCbk = NULL;
    ;
}

void
defrUnsetViaCbk()
{
    ASSERT_INIT;
    defCallbacks->ViaCbk = NULL;
    ;
}

void
defrUnsetViaExtCbk()
{
    ASSERT_INIT;
    defCallbacks->ViaExtCbk = NULL;
    ;
}

void
defrUnsetViaStartCbk()
{
    ASSERT_INIT;
    defCallbacks->ViaStartCbk = NULL;
    ;
}


void
defrUnsetViaEndCbk()
{
    ASSERT_INIT;
    defCallbacks->ViaEndCbk = NULL;
    ;
}

int *
defUnusedCallbackCount()
{
    ASSERT_INIT;
    return defSettings->UnusedCallbacks;
}


const char *
defrFName()
{
    ASSERT_INIT;
    return defSettings->FileName;
}


int
defrRead(FILE           *f,
         const char     *fName,
         defiUserData   uData,
         int            case_sensitive)
{

    int status;

    defrData::reset();

    // Propagate Settings parameter to Data.
    if (defSettings->reader_case_sensitive_set) {
        defData->names_case_sensitive = defSettings->reader_case_sensitive;
    }

    defSettings->FileName = (char*) fName;
    defSettings->File = f;
    defSettings->UserData = uData;
    defSettings->reader_case_sensitive = case_sensitive;

    // Create a path pointer that is all ready to go just in case
    // we need it later.

    defData->NeedPathData = (
        ((defCallbacks->NetCbk || defCallbacks->SNetCbk) && defSettings->AddPathToNet) || defCallbacks->PathCbk) ? 1 : 0;
    if (defData->NeedPathData) {
        defData->PathObj.Init();
    }

    status = defyyparse();

    return status;
}

void
defrSetUserData(defiUserData d)
{
    ASSERT_INIT;
    defSettings->UserData = d;
}


defiUserData
defrGetUserData()
{
    ASSERT_INIT;
    return defSettings->UserData;
}


void
defrSetDesignCbk(defrStringCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->DesignCbk = f;
    ;
}


void
defrSetTechnologyCbk(defrStringCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->TechnologyCbk = f;
    ;
}


void
defrSetDesignEndCbk(defrVoidCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->DesignEndCbk = f;
    ;
}


void
defrSetPropCbk(defrPropCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->PropCbk = f;
    ;
}


void
defrSetPropDefEndCbk(defrVoidCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->PropDefEndCbk = f;
    ;
}


void
defrSetPropDefStartCbk(defrVoidCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->PropDefStartCbk = f;
    ;
}


void
defrSetArrayNameCbk(defrStringCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->ArrayNameCbk = f;
    ;
}


void
defrSetFloorPlanNameCbk(defrStringCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->FloorPlanNameCbk = f;
    ;
}


void
defrSetUnitsCbk(defrDoubleCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->UnitsCbk = f;
    ;
}


void
defrSetVersionCbk(defrDoubleCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->VersionCbk = f;
    ;
}


void
defrSetVersionStrCbk(defrStringCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->VersionStrCbk = f;
    ;
}


void
defrSetDividerCbk(defrStringCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->DividerCbk = f;
    ;
}


void
defrSetBusBitCbk(defrStringCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->BusBitCbk = f;
    ;
}


void
defrSetSiteCbk(defrSiteCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->SiteCbk = f;
    ;
}


void
defrSetCanplaceCbk(defrSiteCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->CanplaceCbk = f;
    ;
}


void
defrSetCannotOccupyCbk(defrSiteCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->CannotOccupyCbk = f;
    ;
}


void
defrSetComponentStartCbk(defrIntegerCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->ComponentStartCbk = f;
    ;
}


void
defrSetComponentEndCbk(defrVoidCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->ComponentEndCbk = f;
    ;
}

void
defrSetComponentCbk(defrComponentCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->ComponentCbk = f;
    ;
}

void
defrSetNetStartCbk(defrIntegerCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->NetStartCbk = f;
    ;
}

void
defrSetNetEndCbk(defrVoidCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->NetEndCbk = f;
    ;
}

void
defrSetNetCbk(defrNetCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->NetCbk = f;
    ;
}

void
defrSetNetNameCbk(defrStringCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->NetNameCbk = f;
    ;
}

void
defrSetNetSubnetNameCbk(defrStringCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->NetSubnetNameCbk = f;
    ;
}

void
defrSetNetNonDefaultRuleCbk(defrStringCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->NetNonDefaultRuleCbk = f;
    ;
}

void
defrSetNetPartialPathCbk(defrNetCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->NetPartialPathCbk = f;
    ;
}

void
defrSetSNetStartCbk(defrIntegerCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->SNetStartCbk = f;
    ;
}

void
defrSetSNetEndCbk(defrVoidCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->SNetEndCbk = f;
    ;
}

void
defrSetSNetCbk(defrNetCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->SNetCbk = f;
    ;
}

void
defrSetSNetPartialPathCbk(defrNetCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->SNetPartialPathCbk = f;
    ;
}

void
defrSetSNetWireCbk(defrNetCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->SNetWireCbk = f;
    ;
}

void
defrSetPathCbk(defrPathCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->PathCbk = f;
    ;
}

void
defrSetAddPathToNet()
{
    ASSERT_INIT;
    defSettings->AddPathToNet = 1;
}

void
defrSetAllowComponentNets()
{
    ASSERT_INIT;
    defSettings->AllowComponentNets = 1;
}

int
defrGetAllowComponentNets()
{
    ASSERT_INIT;
    return defSettings->AllowComponentNets;
}


void
defrSetComponentExtCbk(defrStringCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->ComponentExtCbk = f;
    ;
}

void
defrSetPinExtCbk(defrStringCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->PinExtCbk = f;
    ;
}

void
defrSetViaExtCbk(defrStringCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->ViaExtCbk = f;
    ;
}

void
defrSetNetConnectionExtCbk(defrStringCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->NetConnectionExtCbk = f;
    ;
}

void
defrSetNetExtCbk(defrStringCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->NetExtCbk = f;
    ;
}

void
defrSetGroupExtCbk(defrStringCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->GroupExtCbk = f;
    ;
}

void
defrSetScanChainExtCbk(defrStringCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->ScanChainExtCbk = f;
    ;
}

void
defrSetIoTimingsExtCbk(defrStringCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->IoTimingsExtCbk = f;
    ;
}

void
defrSetPartitionsExtCbk(defrStringCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->PartitionsExtCbk = f;
    ;
}

void
defrSetHistoryCbk(defrStringCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->HistoryCbk = f;
    ;
}

void
defrSetDieAreaCbk(defrBoxCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->DieAreaCbk = f;
    ;
}

void
defrSetPinCapCbk(defrPinCapCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->PinCapCbk = f;
    ;
}

void
defrSetPinEndCbk(defrVoidCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->PinEndCbk = f;
    ;
}

void
defrSetStartPinsCbk(defrIntegerCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->StartPinsCbk = f;
    ;
}

void
defrSetDefaultCapCbk(defrIntegerCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->DefaultCapCbk = f;
    ;
}

void
defrSetPinCbk(defrPinCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->PinCbk = f;
    ;
}

void
defrSetRowCbk(defrRowCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->RowCbk = f;
    ;
}

void
defrSetTrackCbk(defrTrackCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->TrackCbk = f;
    ;
}

void
defrSetGcellGridCbk(defrGcellGridCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->GcellGridCbk = f;
    ;
}

void
defrSetViaStartCbk(defrIntegerCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->ViaStartCbk = f;
    ;
}

void
defrSetViaEndCbk(defrVoidCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->ViaEndCbk = f;
    ;
}

void
defrSetViaCbk(defrViaCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->ViaCbk = f;
    ;
}

void
defrSetRegionStartCbk(defrIntegerCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->RegionStartCbk = f;
    ;
}

void
defrSetRegionEndCbk(defrVoidCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->RegionEndCbk = f;
    ;
}

void
defrSetRegionCbk(defrRegionCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->RegionCbk = f;
    ;
}

void
defrSetGroupsStartCbk(defrIntegerCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->GroupsStartCbk = f;
    ;
}

void
defrSetGroupsEndCbk(defrVoidCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->GroupsEndCbk = f;
    ;
}

void
defrSetGroupNameCbk(defrStringCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->GroupNameCbk = f;
    ;
}

void
defrSetGroupMemberCbk(defrStringCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->GroupMemberCbk = f;
    ;
}

void
defrSetComponentMaskShiftLayerCbk(defrComponentMaskShiftLayerCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->ComponentMaskShiftLayerCbk = f;
    ;
}

void
defrSetGroupCbk(defrGroupCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->GroupCbk = f;
    ;
}

void
defrSetAssertionsStartCbk(defrIntegerCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->AssertionsStartCbk = f;
    ;
}

void
defrSetAssertionsEndCbk(defrVoidCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->AssertionsEndCbk = f;
    ;
}

void
defrSetAssertionCbk(defrAssertionCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->AssertionCbk = f;
    ;
}

void
defrSetConstraintsStartCbk(defrIntegerCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->ConstraintsStartCbk = f;
    ;
}

void
defrSetConstraintsEndCbk(defrVoidCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->ConstraintsEndCbk = f;
    ;
}

void
defrSetConstraintCbk(defrAssertionCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->ConstraintCbk = f;
    ;
}

void
defrSetScanchainsStartCbk(defrIntegerCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->ScanchainsStartCbk = f;
    ;
}

void
defrSetScanchainsEndCbk(defrVoidCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->ScanchainsEndCbk = f;
    ;
}

void
defrSetScanchainCbk(defrScanchainCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->ScanchainCbk = f;
    ;
}

void
defrSetIOTimingsStartCbk(defrIntegerCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->IOTimingsStartCbk = f;
    ;
}

void
defrSetIOTimingsEndCbk(defrVoidCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->IOTimingsEndCbk = f;
    ;
}

void
defrSetIOTimingCbk(defrIOTimingCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->IOTimingCbk = f;
    ;
}

void
defrSetFPCStartCbk(defrIntegerCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->FPCStartCbk = f;
    ;
}

void
defrSetFPCEndCbk(defrVoidCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->FPCEndCbk = f;
    ;
}

void
defrSetFPCCbk(defrFPCCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->FPCCbk = f;
    ;
}

void
defrSetTimingDisablesStartCbk(defrIntegerCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->TimingDisablesStartCbk = f;
    ;
}

void
defrSetTimingDisablesEndCbk(defrVoidCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->TimingDisablesEndCbk = f;
    ;
}

void
defrSetTimingDisableCbk(defrTimingDisableCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->TimingDisableCbk = f;
    ;
}

void
defrSetPartitionsStartCbk(defrIntegerCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->PartitionsStartCbk = f;
    ;
}

void
defrSetPartitionsEndCbk(defrVoidCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->PartitionsEndCbk = f;
    ;
}

void
defrSetPartitionCbk(defrPartitionCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->PartitionCbk = f;
    ;
}

void
defrSetPinPropStartCbk(defrIntegerCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->PinPropStartCbk = f;
    ;
}

void
defrSetPinPropEndCbk(defrVoidCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->PinPropEndCbk = f;
    ;
}

void
defrSetPinPropCbk(defrPinPropCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->PinPropCbk = f;
    ;
}

void
defrSetCaseSensitiveCbk(defrIntegerCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->CaseSensitiveCbk = f;
    ;
}

void
defrSetBlockageStartCbk(defrIntegerCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->BlockageStartCbk = f;
    ;
}

void
defrSetBlockageEndCbk(defrVoidCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->BlockageEndCbk = f;
    ;
}

void
defrSetBlockageCbk(defrBlockageCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->BlockageCbk = f;
    ;
}

void
defrSetSlotStartCbk(defrIntegerCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->SlotStartCbk = f;
    ;
}

void
defrSetSlotEndCbk(defrVoidCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->SlotEndCbk = f;
    ;
}

void
defrSetSlotCbk(defrSlotCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->SlotCbk = f;
    ;
}

void
defrSetFillStartCbk(defrIntegerCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->FillStartCbk = f;
    ;
}

void
defrSetFillEndCbk(defrVoidCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->FillEndCbk = f;
    ;
}

void
defrSetFillCbk(defrFillCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->FillCbk = f;
    ;
}

void
defrSetNonDefaultStartCbk(defrIntegerCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->NonDefaultStartCbk = f;
    ;
}

void
defrSetNonDefaultEndCbk(defrVoidCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->NonDefaultEndCbk = f;
    ;
}

void
defrSetNonDefaultCbk(defrNonDefaultCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->NonDefaultCbk = f;
    ;
}

void
defrSetStylesStartCbk(defrIntegerCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->StylesStartCbk = f;
    ;
}

void
defrSetStylesEndCbk(defrVoidCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->StylesEndCbk = f;
    ;
}

void
defrSetStylesCbk(defrStylesCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->StylesCbk = f;
    ;
}

void
defrSetExtensionCbk(defrStringCbkFnType f)
{
    ASSERT_INIT;
    defCallbacks->ExtensionCbk = f;
    ;
}

// NEW CALLBACK - Put the set functions for the new callbacks here. 

void
defrSetAssertionWarnings(int warn)
{
    ASSERT_INIT;
    defSettings->AssertionWarnings = warn;
    ;
}


void
defrSetBlockageWarnings(int warn)
{
    ASSERT_INIT;
    defSettings->BlockageWarnings = warn;
    ;
}


void
defrSetCaseSensitiveWarnings(int warn)
{
    ASSERT_INIT;
    defSettings->CaseSensitiveWarnings = warn;
    ;
}


void
defrSetComponentWarnings(int warn)
{
    ASSERT_INIT;
    defSettings->ComponentWarnings = warn;
    ;
}


void
defrSetConstraintWarnings(int warn)
{
    ASSERT_INIT;
    defSettings->ConstraintWarnings = warn;
    ;
}


void
defrSetDefaultCapWarnings(int warn)
{
    ASSERT_INIT;
    defSettings->DefaultCapWarnings = warn;
    ;
}


void
defrSetGcellGridWarnings(int warn)
{
    ASSERT_INIT;
    defSettings->GcellGridWarnings = warn;
    ;
}


void
defrSetIOTimingWarnings(int warn)
{
    ASSERT_INIT;
    defSettings->IOTimingWarnings = warn;
    ;
}


void
defrSetNetWarnings(int warn)
{
    ASSERT_INIT;
    defSettings->NetWarnings = warn;
    ;
}


void
defrSetNonDefaultWarnings(int warn)
{
    ASSERT_INIT;
    defSettings->NonDefaultWarnings = warn;
    ;
}


void
defrSetPinExtWarnings(int warn)
{
    ASSERT_INIT;
    defSettings->PinExtWarnings = warn;
    ;
}


void
defrSetPinWarnings(int warn)
{
    ASSERT_INIT;
    defSettings->PinWarnings = warn;
    ;
}


void
defrSetRegionWarnings(int warn)
{
    ASSERT_INIT;
    defSettings->RegionWarnings = warn;
    ;
}


void
defrSetRowWarnings(int warn)
{
    ASSERT_INIT;
    defSettings->RowWarnings = warn;
    ;
}


void
defrSetScanchainWarnings(int warn)
{
    ASSERT_INIT;
    defSettings->ScanchainWarnings = warn;
    ;
}


void
defrSetSNetWarnings(int warn)
{
    ASSERT_INIT;
    defSettings->SNetWarnings = warn;
    ;
}


void
defrSetStylesWarnings(int warn)
{
    ASSERT_INIT;
    defSettings->StylesWarnings = warn;
    ;
}


void
defrSetTrackWarnings(int warn)
{
    ASSERT_INIT;
    defSettings->TrackWarnings = warn;
    ;
}


void
defrSetUnitsWarnings(int warn)
{
    ASSERT_INIT;
    defSettings->UnitsWarnings = warn;
    ;
}


void
defrSetVersionWarnings(int warn)
{
    ASSERT_INIT;
    defSettings->VersionWarnings = warn;
    ;
}


void
defrSetViaWarnings(int warn)
{
    ASSERT_INIT;
    defSettings->ViaWarnings = warn;
    ;
}


void
defrDisableParserMsgs(int   nMsg,
                      int   *msgs)
{
    ASSERT_INIT;
    int i, j;
    int *tmp;

    if (defSettings->nDDMsgs == 0) {
        defSettings->nDDMsgs = nMsg;
        defSettings->disableDMsgs = (int*) defMalloc(sizeof(int) * nMsg);
        for (i = 0; i < nMsg; i++)
            defSettings->disableDMsgs[i] = msgs[i];
    } else {  // add the list to the existing list 
        // 1st check if the msgId is already on the list before adding it on 
        tmp = (int*) defMalloc(sizeof(int) * (nMsg + defSettings->nDDMsgs));
        for (i = 0; i < defSettings->nDDMsgs; i++)  // copy the existing to the new list 
            tmp[i] = defSettings->disableDMsgs[i];
        defFree((int*) (defSettings->disableDMsgs));
        defSettings->disableDMsgs = tmp;           // set disableDMsgs to the new list 
        for (i = 0; i < nMsg; i++) { // merge the new list with the existing 
            for (j = 0; j < defSettings->nDDMsgs; j++) {
                if (defSettings->disableDMsgs[j] == msgs[i])
                    break;             // msgId already on the list 
            }
            if (j == defSettings->nDDMsgs)           // msgId not on the list, add it on 
                defSettings->disableDMsgs[defSettings->nDDMsgs++] = msgs[i];
        }
    }
    return;
}


void
defrEnableParserMsgs(int    nMsg,
                     int    *msgs)
{
    ASSERT_INIT;
    int i, j;

    if (defSettings->nDDMsgs == 0)
        return;                       // list is empty, nothing to remove 

    for (i = 0; i < nMsg; i++) {     // loop through the given list 
        for (j = 0; j < defSettings->nDDMsgs; j++) {
            if (defSettings->disableDMsgs[j] == msgs[i]) {
                defSettings->disableDMsgs[j] = -1;    // temp assign a -1 on that slot 
                break;
            }
        }
    }
    // fill up the empty slot with the next non -1 msgId 
    for (i = 0; i < defSettings->nDDMsgs; i++) {
        if (defSettings->disableDMsgs[i] == -1) {
            j = i + 1;
            while (j < defSettings->nDDMsgs) {
                if (defSettings->disableDMsgs[j] != -1)
                    defSettings->disableDMsgs[i++] = defSettings->disableDMsgs[j++];
            }
            break;     // break out the for loop, the list should all moved 
        }
    }
    // Count how many messageId left and change all -1 to 0 
    for (j = i; j < defSettings->nDDMsgs; j++) {
        defSettings->disableDMsgs[j] = 0;     // set to 0 
    }
    defSettings->nDDMsgs = i;
    return;
}


void
defrEnableAllMsgs()
{
    ASSERT_INIT;
    defSettings->nDDMsgs = 0;
    defFree((int*) (defSettings->disableDMsgs));
}


void
defrSetTotalMsgLimit(int totNumMsgs)
{
    ASSERT_INIT;
    defSettings->totalDefMsgLimit = totNumMsgs;
}


void
defrSetLimitPerMsg(int  msgId,
                   int  numMsg)
{
    ASSERT_INIT;
    char msgStr[10];

    if ((msgId <= 0) || ((msgId - 5000) >= NODEFMSG)) {   // Def starts at 5000
        sprintf(msgStr, "%d", msgId);
        defError(204, msgStr);
        return;
    }
    defSettings->MsgLimit[msgId - 5000] = numMsg;
    return;
}


// *****************************************************************
// Utility functions
//
// These are utility functions. Note: this part still contains some
// global variables. Ideally they would be part of the main class.
// *****************************************************************

void
defrSetMagicCommentFoundFunction(DEFI_MAGIC_COMMENT_FOUND_FUNCTION f)
{
    ASSERT_INIT;
    defSettings->MagicCommentFoundFunction = f;
}


void
defrSetMagicCommentString(char *s)
{
    ASSERT_INIT;
    free(defData->magic);
    defData->magic = strdup(s);
}

void
defrSetLogFunction(DEFI_LOG_FUNCTION f)
{
    ASSERT_INIT;
    defSettings->ErrorLogFunction = f;
}

void
defrSetWarningLogFunction(DEFI_WARNING_LOG_FUNCTION f)
{
    ASSERT_INIT;
    defSettings->WarningLogFunction = f;
}

void
defrSetMallocFunction(DEFI_MALLOC_FUNCTION f)
{
    ASSERT_INIT;
    defSettings->MallocFunction = f;
}

void
defrSetReallocFunction(DEFI_REALLOC_FUNCTION f)
{
    ASSERT_INIT;
    defSettings->ReallocFunction = f;
}

void
defrSetFreeFunction(DEFI_FREE_FUNCTION f)
{
    ASSERT_INIT;
    defSettings->FreeFunction = f;
}

void
defrSetLineNumberFunction(DEFI_LINE_NUMBER_FUNCTION f)
{
    ASSERT_INIT;
    defSettings->LineNumberFunction = f;
}

void
defrSetLongLineNumberFunction(DEFI_LONG_LINE_NUMBER_FUNCTION f)
{
    ASSERT_INIT;
    defSettings->LongLineNumberFunction = f;
}


void
defrSetDeltaNumberLines(int numLines)
{
    ASSERT_INIT;
    defSettings->defiDeltaNumberLines = numLines;
}


void
defrSetCommentChar(char c)
{
    ASSERT_INIT;
    defSettings->CommentChar = c;
}

void
defrSetCaseSensitivity(int caseSense)
{
    ASSERT_INIT;
    defSettings->reader_case_sensitive_set = 1;
    defSettings->reader_case_sensitive = caseSense;
}


void
defrAddAlias(const char *key,
             const char *value,
             int        marked)
{
    // Since the alias data is stored in the hash table, the hash table
    // only takes the key and the data, the marked data will be stored
    // at the end of the value data

    char    *k1;
    char    *v1;
    int     len = strlen(key) + 1;
    k1 = (char*) malloc(len);
    strcpy(k1, key);
    len = strlen(value) + 1 + 1;   // 1 for the marked
    v1 = (char*) malloc(len);
    //strcpy(v1, value);
    if (marked != 0)
        marked = 1;                 // make sure only 1 digit
    sprintf(v1, "%d%s", marked, value);

    defData->def_alias_set[k1] = v1;
}

void
defrSetOpenLogFileAppend()
{
    ASSERT_INIT;
    defSettings->LogFileAppend = TRUE;
}

void
defrUnsetOpenLogFileAppend()
{
    ASSERT_INIT;
    defSettings->LogFileAppend = FALSE;
}


void
defrSetReadFunction(DEFI_READ_FUNCTION f)
{
    ASSERT_INIT;
    defSettings->ReadFunction = f;
}

void
defrUnsetReadFunction()
{
    ASSERT_INIT;
    defSettings->ReadFunction = 0;
}

void
defrDisablePropStrProcess()
{
    ASSERT_INIT;
    defSettings->DisPropStrProcess = 1;
}

void
defrSetNLines(long long n)
{
    defData->nlines = n;
}

void
pathIsDone(int  shield,
           int  reset,
           int  netOsnet,
           int  *needCbk)
{
    if ((defCallbacks->NetCbk || defCallbacks->SNetCbk) && defSettings->AddPathToNet) {
        //defData->PathObj.reverseOrder();
        if (defData->Subnet) {
            // if (shield)
            //    defrSubnet->addShieldPath(defrPath);
            // else 
            defData->Subnet->addWirePath(&defData->PathObj, reset, netOsnet,
                                         needCbk);

        } else {
            if (shield)
                defData->Net.addShieldPath(&defData->PathObj, reset, netOsnet, needCbk);
            else
                defData->Net.addWirePath(&defData->PathObj, reset, netOsnet, needCbk);

        }

    } else if (defCallbacks->PathCbk) {
        //defrPath->reverseOrder();
        (*defCallbacks->PathCbk)(defrPathCbkType, &defData->PathObj, defSettings->UserData);
        defData->PathObj.Destroy();
        free((char*) &defData->PathObj);
    }

    defData->PathObj.Init();
}

END_LEFDEF_PARSER_NAMESPACE

