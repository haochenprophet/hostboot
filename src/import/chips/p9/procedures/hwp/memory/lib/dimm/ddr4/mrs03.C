/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/import/chips/p9/procedures/hwp/memory/lib/dimm/ddr4/mrs03.C $ */
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

///
/// @file mrs03.C
/// @brief Run and manage the DDR4 DDR4 loading
///
// *HWP HWP Owner: Brian Silver <bsilver@us.ibm.com>
// *HWP HWP Backup: Andre Marin <aamarin@us.ibm.com>
// *HWP Team: Memory
// *HWP Level: 1
// *HWP Consumed by: FSP:HB

#include <fapi2.H>

#include <mss.H>
#include <lib/dimm/ddr4/mrs_load_ddr4.H>

using fapi2::TARGET_TYPE_MCBIST;
using fapi2::TARGET_TYPE_DIMM;

using fapi2::FAPI2_RC_SUCCESS;

namespace mss
{

namespace ddr4
{

///
/// @brief mrs03_data ctor
/// @param[in] a fapi2::TARGET_TYPE_DIMM target
/// @param[out] fapi2::ReturnCode FAPI2_RC_SUCCESS iff ok
///
mrs03_data::mrs03_data( const fapi2::Target<fapi2::TARGET_TYPE_DIMM>& i_target, fapi2::ReturnCode& o_rc ):
    iv_mpr_mode(fapi2::ENUM_ATTR_EFF_MPR_MODE_DISABLE),
    iv_mpr_page(fapi2::ENUM_ATTR_EFF_MPR_PAGE_PG0),
    iv_geardown(0),
    iv_pda(fapi2::ENUM_ATTR_EFF_PER_DRAM_ACCESS_DISABLE),
    iv_crc_wr_latency(0),
    iv_temp_readout(fapi2::ENUM_ATTR_EFF_TEMP_READOUT_DISABLE),
    iv_fine_refresh(0),
    iv_read_format(fapi2::ENUM_ATTR_EFF_MPR_RD_FORMAT_SERIAL)
{
    FAPI_TRY( mss::eff_mpr_mode(i_target, iv_mpr_mode) );
    FAPI_TRY( mss::eff_mpr_page(i_target, iv_mpr_page) );
    FAPI_TRY( mss::eff_geardown_mode(i_target, iv_geardown) );
    FAPI_TRY( mss::eff_per_dram_access(i_target, iv_pda) );
    FAPI_TRY( mss::eff_temp_readout(i_target, iv_temp_readout) );
    FAPI_TRY( mss::mrw_fine_refresh_mode(iv_fine_refresh) );
    FAPI_TRY( mss::eff_crc_wr_latency(i_target, iv_crc_wr_latency) );
    FAPI_TRY( mss::eff_mpr_rd_format(i_target, iv_read_format) );

    FAPI_INF("MR3 attributes: MPR_MODE: 0x%x, MPR_PAGE: 0x%x, GD: 0x%x, PDA: 0x%x, "
             "TEMP: 0x%x FR: 0x%x, CRC_WL: 0x%x, RF: 0x%x",
             iv_mpr_mode, iv_mpr_page, iv_geardown, iv_pda, iv_temp_readout,
             iv_fine_refresh, iv_crc_wr_latency, iv_read_format);

    o_rc = fapi2::FAPI2_RC_SUCCESS;
    return;

fapi_try_exit:
    o_rc = fapi2::current_err;
    FAPI_ERR("unable to get attributes for mrs0");
    return;
}

///
/// @brief Configure the ARR0 of the CCS instruction for mrs03
/// @param[in] i_target a fapi2::Target<TARGET_TYPE_DIMM>
/// @param[in,out] io_inst the instruction to fixup
/// @param[in] i_rank the rank in question
/// @return FAPI2_RC_SUCCESS iff OK
///
fapi2::ReturnCode mrs03(const fapi2::Target<TARGET_TYPE_DIMM>& i_target,
                        ccs::instruction_t<TARGET_TYPE_MCBIST>& io_inst,
                        const uint64_t i_rank)
{
    // Check to make sure our ctor worked ok
    mrs03_data l_data( i_target, fapi2::current_err );
    FAPI_TRY( fapi2::current_err, "Unable to construct MRS03 data from attributes");
    FAPI_TRY( mrs03(i_target, l_data, io_inst, i_rank) );

fapi_try_exit:
    return fapi2::current_err;
}

///
/// @brief Configure the ARR0 of the CCS instruction for mrs03, data object as input
/// @param[in] i_target a fapi2::Target<fapi2::TARGET_TYPE_DIMM>
/// @param[in] i_data an mrs00_data object, filled in
/// @param[in,out] io_inst the instruction to fixup
/// @param[in] i_rank the rank in question
/// @return FAPI2_RC_SUCCESS iff OK
///
fapi2::ReturnCode mrs03(const fapi2::Target<fapi2::TARGET_TYPE_DIMM>& i_target,
                        const mrs03_data& i_data,
                        ccs::instruction_t<fapi2::TARGET_TYPE_MCBIST>& io_inst,
                        const uint64_t i_rank)
{
    constexpr uint64_t LOWEST_WL = 4;
    constexpr uint64_t WL_COUNT = 3;
    //                                                 4  5  6
    constexpr uint8_t crc_wr_latency_map[WL_COUNT] = { 0, 1, 2 };

    fapi2::buffer<uint8_t> l_crc_wr_latency_buffer;

    FAPI_ASSERT((i_data.iv_crc_wr_latency >= LOWEST_WL) && (i_data.iv_crc_wr_latency < (LOWEST_WL + WL_COUNT)),
                fapi2::MSS_BAD_MR_PARAMETER()
                .set_MR_NUMBER(3)
                .set_PARAMETER(WRITE_CMD_LATENCY)
                .set_PARAMETER_VALUE(i_data.iv_crc_wr_latency)
                .set_DIMM_IN_ERROR(i_target),
                "Bad value for Write CMD Latency: %d (%s)", i_data.iv_crc_wr_latency, mss::c_str(i_target));

    l_crc_wr_latency_buffer = crc_wr_latency_map[i_data.iv_crc_wr_latency - LOWEST_WL];

    mss::swizzle<A0, 2, 7>(fapi2::buffer<uint8_t>(i_data.iv_mpr_mode), io_inst.arr0);
    io_inst.arr0.writeBit<A2>(i_data.iv_mpr_page);
    io_inst.arr0.writeBit<A3>(i_data.iv_geardown);
    io_inst.arr0.writeBit<A4>(i_data.iv_pda);
    io_inst.arr0.writeBit<A5>(i_data.iv_temp_readout);

    mss::swizzle<A6 , 3, 7>(fapi2::buffer<uint8_t>(i_data.iv_fine_refresh), io_inst.arr0);
    mss::swizzle<A9 , 2, 7>(l_crc_wr_latency_buffer, io_inst.arr0);
    mss::swizzle<A11, 2, 7>(fapi2::buffer<uint8_t>(i_data.iv_read_format), io_inst.arr0);

    FAPI_INF("MR3: 0x%016llx", uint64_t(io_inst.arr0));

    return fapi2::FAPI2_RC_SUCCESS;

fapi_try_exit:
    return fapi2::current_err;
}

///
/// @brief Given a CCS instruction which contains address bits with an encoded MRS3,
/// decode and trace the contents
/// @param[in] i_inst the CCS instruction
/// @param[in] i_rank ths rank in question
/// @return void
///
fapi2::ReturnCode mrs03_decode(const ccs::instruction_t<TARGET_TYPE_MCBIST>& i_inst,
                               const uint64_t i_rank)
{
    fapi2::buffer<uint8_t> l_mpr_mode;

    fapi2::buffer<uint8_t> l_fine_refresh;
    fapi2::buffer<uint8_t> l_crc_wr_latency_buffer;
    fapi2::buffer<uint8_t> l_read_format;

    uint8_t l_mpr_page = i_inst.arr0.getBit<A2>();
    uint8_t l_geardown = i_inst.arr0.getBit<A3>();
    uint8_t l_pda = i_inst.arr0.getBit<A4>();
    uint8_t l_temp_readout = i_inst.arr0.getBit<A5>();

    mss::swizzle<6, 2, A1>(i_inst.arr0, l_mpr_mode);
    mss::swizzle<5, 3, A7>(i_inst.arr0, l_fine_refresh);
    mss::swizzle<6, 2, A10>(i_inst.arr0, l_crc_wr_latency_buffer);
    mss::swizzle<6, 2, A12>(i_inst.arr0, l_read_format);

    FAPI_INF("MR3 rank %d decode: MPR_MODE: 0x%x, MPR_PAGE: 0x%x, GD: 0x%x, PDA: 0x%x, "
             "TEMP: 0x%x FR: 0x%x, CRC_WL: 0x%x, RF: 0x%x", i_rank,
             uint8_t(l_mpr_mode), l_mpr_page, l_geardown, l_pda, uint8_t(l_temp_readout),
             uint8_t(l_fine_refresh), uint8_t(l_crc_wr_latency_buffer), uint8_t(l_read_format));

    return FAPI2_RC_SUCCESS;
}

fapi2::ReturnCode (*mrs03_data::make_ccs_instruction)(const fapi2::Target<fapi2::TARGET_TYPE_DIMM>& i_target,
        const mrs03_data& i_data,
        ccs::instruction_t<fapi2::TARGET_TYPE_MCBIST>& io_inst,
        const uint64_t i_rank) = &mrs03;

fapi2::ReturnCode (*mrs03_data::decode)(const ccs::instruction_t<fapi2::TARGET_TYPE_MCBIST>& i_inst,
                                        const uint64_t i_rank) = &mrs03_decode;

} // ns ddr4
} // ns mss
