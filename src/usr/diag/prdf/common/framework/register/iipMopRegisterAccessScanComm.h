/* IBM_PROLOG_BEGIN_TAG                                                   */
/* This is an automatically generated prolog.                             */
/*                                                                        */
/* $Source: src/usr/diag/prdf/common/framework/register/iipMopRegisterAccessScanComm.h $ */
/*                                                                        */
/* OpenPOWER HostBoot Project                                             */
/*                                                                        */
/* COPYRIGHT International Business Machines Corp. 1996,2014              */
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

#ifndef iipMopRegisterAccessScanComm_h
#define iipMopRegisterAccessScanComm_h

// Class Specification *************************************************
//
// Class name:   MopRegisterAccessScanComm
// Parent class: MopRegisterAccess.
//
// Summary: This class provides access to hardware register data via
//          a MOP Scan Comm routine.
//
// Cardinality: 0
//
// Performance/Implementation:
//   Space Complexity: Constant
//   Time Complexity:  All member functions constant unless otherwise
//                     stated.
//
// Usage Examples:
//
//
//
// End Class Specification *********************************************

// Includes

#pragma interface

#ifndef iipMopRegisterAccess_h
#include <iipMopRegisterAccess.h>
#endif

namespace PRDF
{

// Forward References
class MopRegisterAccessScanComm : public MopRegisterAccess
{
public:

  // Function Specification ********************************************
  //
  // Purpose:      CTOR
  // Parameters:   None
  // Returns:      No value returned.
  // Requirements: None.
  // Promises:     All data members are initialized.
  // Exceptions:   None.
  // Concurrency:  N/A
  // Note:         Multiple chip IDs are for chips that MOPs must
  //               access at the same time when performing a Scan
  //               Comm operation (ie STINGER & ARROW chips)
  //
  // End Function Specification //////////////////////////////////////

  //  MopRegisterAccessScanComm(const MopRegisterAccessScanComm & scr);
  // Function Specification ********************************************
  //
  // Purpose:      Copy
  // Parameters:   scr: Reference to instance to copy
  // Returns:      No value returned.
  // Requirements: None.
  // Promises:     All data members will be copied (Deep copy).
  // Exceptions:   None.
  // Concurrency:  N/A.
  // Notes:  This constructor is not declared.  This compiler generated
  //         default definition is sufficient.
  //
  // End Function Specification ****************************************

  //   virtual ~MopRegisterAccessScanComm(void);
  // Function Specification ********************************************
  //
  // Purpose:      Destruction
  // Parameters:   None.
  // Returns:      No value returned
  // Requirements: None.
  // Promises:     None.
  // Exceptions:   None.
  // Concurrency:  N/A
  // Notes:  This destructor is not declared.  This compiler generated
  //         default definition is sufficient.
  //
  // End Function Specification ****************************************

  //  MopRegisterAccessScanComm & operator=(const MopRegisterAccessScanComm & scr);
  // Function Specification ********************************************
  //
  // Purpose:      Assigment
  // Parameters:   d: Reference to instance to assign from
  // Returns:      Reference to this instance
  // Requirements: None.
  // Promises:     All data members are assigned to
  // Exceptions:   None.
  // Concurrency:  N/A.
  // Notes:  This assingment operator is not declared.  The compiler
  //         generated default definition is sufficient.
  //
  // End Function Specification ****************************************

  virtual uint32_t Access(BIT_STRING_CLASS & bs,
                          uint32_t registerId,
                          Operation operation) const;
  // Function Specification ********************************************
  //
  // Purpose:      This function reads or writes the hardware according
  //               to the specified operation.
  // Parameters:   bs: Bit string to retrieve(for write) or store data
  //               (from read)
  //               registerId: ScanComm register address
  //               operation: Indicates either read or write operation
  // Returns:      Hardware OPs return code
  // Requirements: bs.Length() == long enough
  // Promises:     For read operation, bs is modified to reflect hardware
  //               register state
  // Exceptions:   None.
  // Concurrency:  Nonreentrant.
  // Note:         The first bs.Length() bits from the Hardware OPs read
  //               are set/reset in bs (from left to right)
  //               For a write, the first bs.Length() bits are written
  //               to the hardware register with right padded 0's if
  //               needed
  //
  // End Function Specification ****************************************


private: // DATA

};

} // end namespace PRDF

#endif
