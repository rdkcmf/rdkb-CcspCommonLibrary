/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2015 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

/**********************************************************************
   Copyright [2014] [Cisco Systems, Inc.]
 
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
 
       http://www.apache.org/licenses/LICENSE-2.0
 
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
**********************************************************************/

/**********************************************************************

    module:	ssd_ifo_gbi.h

        For CCSP Secure Software Download

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Genenral Bootloader Interface.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Tom Chang

    ---------------------------------------------------------------

    revision:

        06/14/11    initial revision.

**********************************************************************/

#ifndef  _SSD_IFO_GBI_
#define  _SSD_IFO_GBI_

/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/***********************************************************
  SSD GENERAL BOOTLOADER INTERFACE OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  SSD_GBI_INTERFACE_NAME                    "ssdGeneralBootloaderIf"
#define  SSD_GBI_INTERFACE_ID                      0

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */

typedef  ANSC_STATUS
(*PFN_SSDGBIF_ACQ_ACCESS)
    (
        ANSC_HANDLE                         hThisObject,
        ANSC_HANDLE                         hReserved
    );

typedef  ANSC_STATUS
(*PFN_SSDGBIF_REL_ACCESS)
    (
        ANSC_HANDLE                         hThisObject,
        ANSC_HANDLE                         hReserved
    );

typedef  ANSC_STATUS
(*PFN_SSDGBIF_AUTH_SW)
    (
        ANSC_HANDLE                         hThisObject,
        char*                               pLocation
    );

typedef  ANSC_STATUS
(*PFN_SSDGBIF_CHECK_SW)
    (
        ANSC_HANDLE                         hThisObject,
        char*                               pLoacation
    );

typedef  ANSC_STATUS
(*PFN_SSDGBIF_FLASH_IMAGE)
    (
        ANSC_HANDLE                         hThisObject,
        ANSC_HANDLE                         pHeader,
        ULONG                               ulImageSize,
        ULONG                               FlashMode,
        ULONG                               BankdingMode
    );

/*
 * The SSD mainly relies on Bootloader apis to do firmware authentication, verification and flashing. 
 * Bootloader features like single bank management or double bank management and the difference between 
 * various bootloader implementations may be transparent to the SSD. 
 */
#define  SSD_GBI_INTERFACE_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_SSDGBIF_ACQ_ACCESS                  AcquireAccess;                                  \
    PFN_SSDGBIF_REL_ACCESS                  ReleaseAccess;                                  \
    PFN_SSDGBIF_AUTH_SW                     AuthSoftware;                                   \
    PFN_SSDGBIF_CHECK_SW                    CheckSoftware;                                  \
    PFN_SSDGBIF_FLASH_IMAGE                 FlashImage;                                     \
    /* end of object class content */                                                       \

typedef  struct
_SSD_GB_INTERFACE
{
    SSD_GBI_INTERFACE_CLASS_CONTENT
}
SSD_GB_INTERFACE,  *PSSD_GB_INTERFACE;

#define  ACCESS_SSD_GB_INTERFACE(p)               \
         ACCESS_CONTAINER(p, SSD_GB_INTERFACE, Linkage)

#endif
