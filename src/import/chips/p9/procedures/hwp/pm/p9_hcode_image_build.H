/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/pm/p9_hcode_image_build.H $ */
/*                                                                        */
/* OpenPOWER HostBoot Project                                             */
/*                                                                        */
/* Contributors Listed Below - COPYRIGHT 2015,2016                        */
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
#ifndef __HCODE_IMG_BUILD_H_
#define __HCODE_IMG_BUILD_H_

#include <stdint.h>

///
/// @file   p9_hcode_image_build.H
/// @brief  describes HWP interface that builds the STOP image.
///
/// *HWP HWP Owner:      Greg Still <stillgs@us.ibm.com>
/// *HWP FW Owner:       Prem S Jha <premjha2@in.ibm.com>
/// *HWP Team:           PM
/// *HWP Level:          2
/// *HWP Consumed by:    Hostboot:Phyp
//

//--------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------
#include <fapi2.H>

extern "C"
{
    typedef  const fapi2::Target< fapi2::TARGET_TYPE_PROC_CHIP > CONST_FAPI2_PROC;

    /**
     * @brief   phases of system associated for hcode image build.
     */
    enum SysPhase_t
    {
        PHASE_NA        =   0,
        PHASE_IPL       =   1,
        PHASE_REBUILD   =   2,
        PHASE_END       =   3,
    };

    /**
     * @brief   constants used for hcode image build.
     */
    enum
    {
        HW_IMG_RING_SIZE   =  1024 * 1024,
    };

    /**
     * @brief image types to be handled by the hcode image build HWP.
     * @note  structure below summarizes all the components that needs to be
     *        included during STOP image build. Setting a field to 1 means that
     *        it needs to be part of STOP image in HOMER. It can be used to
     *        build partial STOP image in HOMER.
     */
    struct ImageType_t
    {
        uint32_t selfRestoreBuild: 1;       // for self restore code inclusion
        uint32_t cmeHcodeBuild: 1;           // for cme hcode inclusion
        uint32_t cmeCommonRingBuild: 1;      // for core ring inclusion
        uint32_t cmeCoreSpecificRingBuild: 1; // for core specific scan ring inclusion
        uint32_t sgpeHcodeBuild: 1;          // for sgpe inclusion
        uint32_t sgpeCommonRingBuild: 1;     // for sgpe common scan ring inclusion
        uint32_t sgpeCacheSpecificRingBuild: 1; // for cache specific ring inclusion
        uint32_t pgpeImageBuild: 1;      // pgpe platform inclusion in image build
        uint32_t pgpePstateParmBlockBuild: 1; //pgpe P-State Parameter Block Build
        uint32_t reserve: 23;

        ImageType_t( )
        {
            memset( this , 0, sizeof (*this) );
            this->selfRestoreBuild |= 0x01;
            this->cmeHcodeBuild |= 0x01;
            this->cmeCommonRingBuild |= 0x01;
            this->cmeCoreSpecificRingBuild |= 0x01;
            this->sgpeHcodeBuild |= 0x01;
            this->sgpeCommonRingBuild |= 0x01;
            this->sgpeCacheSpecificRingBuild |= 0x01;
            this->pgpeImageBuild |= 0x01;
            this->pgpePstateParmBlockBuild |= 0x01;
        }

        bool isBuildValid()
        {
            bool buildValid = false;

            if ( ( this->selfRestoreBuild ) ||
                 ( this->cmeHcodeBuild ) ||
                 ( this->cmeCommonRingBuild ) ||
                 ( this->cmeCoreSpecificRingBuild ) ||
                 ( this->sgpeHcodeBuild ) ||
                 ( this->sgpeCommonRingBuild ) ||
                 ( this->sgpeCacheSpecificRingBuild ) ||
                 ( this->pgpeImageBuild ) ||
                 ( this->pgpePstateParmBlockBuild ) )
            {
                buildValid = true;
            }

            return buildValid;
        }
    };

    typedef fapi2::ReturnCode( *p9_hcode_image_build_FP_t )
    ( CONST_FAPI2_PROC& i_procTgt,
      void* const   i_pImageIn,
      void*         i_pHomerImage,
      void* const   i_pRingOverride,
      SysPhase_t    i_phase,
      ImageType_t i_imgType,
      void*  const    i_pBuf1,
      const  uint32_t i_sizeBuf1,
      void*  const    i_pBuf2,
      const  uint32_t  i_sizeBuf2 );

    /**
    * @brief   builds a STOP image using a refrence image as input.
    * @param   i_procTgt        fapi2 target for processor chip.
    * @param   i_pImageIn       points to memory mapped refrence image in PNOR.
    * @param   i_pHomerImage    pointer to the beginning of the HOMER image buffer.
    * @param   i_pRingOverride  pointer to the location of override ring. NULL means override not available.
    * @param   i_phase          phase of the system i.e. IPL or Hypervisor/rebuild mode.
    * @param   i_imgType        image type to be built.
    * @param   i_pBuf1          pointer to a work buffer1.
    * @param   i_sizeBuf1       size of work buffer1. Minimum size expected HW_IMG_RING_SIZE.
    * @param   i_pBuf2          pointer to a work buffer2. Minimum size expected HW_IMG_RING_SIZE.
    * @param   i_sizeBuf2       size of work buffer2
    * @note    needs attribute ATTR_EC
    */
    fapi2::ReturnCode p9_hcode_image_build( CONST_FAPI2_PROC& i_procTgt,
                                            void* const     i_pImageIn,
                                            void*           i_pHomerImage,
                                            void* const     i_pRingOverride,
                                            SysPhase_t      i_phase,
                                            ImageType_t     i_imgType,
                                            void*  const    i_pBuf1,
                                            const  uint32_t i_sizeBuf1,
                                            void* const    i_pBuf2,
                                            const uint32_t  i_sizeBuf2
                                          );
} // extern C
#endif //__HCODE_IMG_BUILD_H_
