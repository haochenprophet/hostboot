/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/usr/diag/prdf/common/plat/p9/prdfCommonPlugins.C $        */
/*                                                                        */
/* OpenPOWER HostBoot Project                                             */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2016                             */
/* [+] International Business Machines Corp.                              */
/*                                                                        */
/*                                                                        */
/* Licensed under the Apache License, Version 2.0 (the "License");        */
/* you may not use this file except in compliance with the License.       */
/* You may obtain a copy of the License at                                */
/*                                                                        */
/*     http://www.apache.org/licenses/LICENSE-2.0                         */
/*                                                                        */
/* Unless required by applicable law or agreed to in writing, software    */
/* distributed under the License is distributed on an "AS IS" BASIS,      */
/* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or        */
/* implied. See the License for the specific language governing           */
/* permissions and limitations under the License.                         */
/*                                                                        */
/* IBM_PROLOG_END_TAG                                                     */
/** @file  prdfCommonPlugins.C
 *  @brief Contains plugin code that is common for multiple chiplets
 */

#include <prdfGlobal.H>
#include <prdfPluginDef.H>
#include <iipServiceDataCollector.h>
#include <prdfExtensibleChip.H>
#include <prdfPluginMap.H>

namespace PRDF
{

using namespace PlatServices;

namespace CommonPlugins
{


/**
 * @brief   When not in MNFG mode, clear the service call flag so that
 *          thresholding will still be done, but no visible error log committed.
 * @param   i_chip EX chip
 * @param   i_sc   Step code data struct
 * @returns SUCCESS always
 */
int32_t ClearServiceCallFlag( ExtensibleChip * i_chip,
                              STEP_CODE_DATA_STRUCT & i_sc )
{
    if ( i_sc.service_data->IsAtThreshold() && !mfgMode() &&
         (CHECK_STOP != i_sc.service_data->getPrimaryAttnType()) &&
         (!i_sc.service_data->queryFlag(ServiceDataCollector::UNIT_CS)) )
    {
        i_sc.service_data->clearServiceCall();
    }

    return SUCCESS;
}
PRDF_PLUGIN_DEFINE_NS( p9_nimbus, CommonPlugins, ClearServiceCallFlag );
PRDF_PLUGIN_DEFINE_NS( p9_xbus, CommonPlugins, ClearServiceCallFlag );
PRDF_PLUGIN_DEFINE_NS( p9_obus, CommonPlugins, ClearServiceCallFlag );
PRDF_PLUGIN_DEFINE_NS( p9_ex, CommonPlugins, ClearServiceCallFlag );
PRDF_PLUGIN_DEFINE_NS( p9_ec, CommonPlugins, ClearServiceCallFlag );
PRDF_PLUGIN_DEFINE_NS( p9_eq, CommonPlugins, ClearServiceCallFlag );
PRDF_PLUGIN_DEFINE_NS( p9_pec, CommonPlugins, ClearServiceCallFlag );
PRDF_PLUGIN_DEFINE_NS( p9_mcbist, CommonPlugins, ClearServiceCallFlag );
PRDF_PLUGIN_DEFINE_NS( p9_mca, CommonPlugins, ClearServiceCallFlag );
PRDF_PLUGIN_DEFINE_NS( p9_capp, CommonPlugins, ClearServiceCallFlag );
PRDF_PLUGIN_DEFINE_NS( p9_phb, CommonPlugins, ClearServiceCallFlag );
PRDF_PLUGIN_DEFINE_NS( p9_mcs, CommonPlugins, ClearServiceCallFlag );


} // namespace CommonPlugins ends

}// namespace PRDF ends

