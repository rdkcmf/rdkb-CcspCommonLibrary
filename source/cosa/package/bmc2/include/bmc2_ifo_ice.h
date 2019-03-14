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

    module:	bmc2_ifo_ice.h

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Bmc2 Interactive Console Environment (ICE)
        Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/21/05    initial revision.

**********************************************************************/


#ifndef  _BMC2_IFO_ICE_
#define  _BMC2_IFO_ICE_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/***********************************************************
  BMC2 INTERACTIVE CONSOLE ENVIRONMENT INTERFACE DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  BMC2_ICE_INTERFACE_NAME                    "bmc2InteractiveConsoleEnvironmentIf"
#define  BMC2_ICE_INTERFACE_ID                      0

#define  BMC2_ICE_INPUT_TYPE_unknown                0
#define  BMC2_ICE_INPUT_TYPE_text                   1
#define  BMC2_ICE_INPUT_TYPE_nextControl            2
#define  BMC2_ICE_INPUT_TYPE_prevControl            3

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_BMC2ICEIF_WRITE)
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pContent        /* may or may not include "\r\n" */
    );

typedef  ANSC_STATUS
(*PFN_BMC2ICEIF_READ)
    (
        ANSC_HANDLE                 hThisObject,
        char**                      pTextInput
    );

typedef  ANSC_STATUS
(*PFN_BMC2ICEIF_READ2)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTextBoxInfo,
        char**                      pTextInput
    );

typedef  ANSC_STATUS
(*PFN_BMC2ICEIF_CLEAR)
    (
        ANSC_HANDLE                 hThisObject
    );

/*
 * The BMC2_ICE interface is provided by the shell program when creating a Virtual Terminal. This
 * interface provides API to allow Terminal to read user input and write output to the console in
 * an orderly fashion. Because those APIs will only be called within pBmc2SccIf->ExecuteCommand(),
 * shell developer must ensure there's no deadlock situation.
 */
#define  BMC2_ICE_INTERFACE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_BMC2ICEIF_WRITE             Write;                                                  \
    PFN_BMC2ICEIF_READ              ReadInput;                                              \
    PFN_BMC2ICEIF_READ2             ReadInput2;                                             \
    PFN_BMC2ICEIF_CLEAR             ClearScreen;                                            \
    /* end of object class content */                                                       \

typedef  struct
_BMC2_ICE_INTERFACE
{
    BMC2_ICE_INTERFACE_CLASS_CONTENT
}
BMC2_ICE_INTERFACE,  *PBMC2_ICE_INTERFACE;

#define  ACCESS_BMC2_ICE_INTERFACE(p)               \
         ACCESS_CONTAINER(p, BMC2_ICE_INTERFACE, Linkage)


/***********************************************************
      BMC2 ICE-SPECIFIC INPUT/OUTPUT CONTROL DEFINITION
***********************************************************/

/*
 * To build sophisticated CLI interface, we need the capability to display various input controls
 * on the console and read user's input from them. Here, we define a base input control element:
 * Text Box. A Text Box is a dedicated area on the console where user can input parameters during
 * the execution of a CLI command.
 */
typedef  struct
_BMC2_ICE_TEXTBOX_INFO
{
    char*                           Name;           /* used to identify a particular TextBox control, reserved for future use */
    char*                           DefaultValue;   /* default value to be displayed in the Text Box, could be NULL           */
    BOOL                            bSizeFixed;     /* if TRUE, user is not allowed to input more characters than the 'Width' */
    BOOL                            bMaskInput;     /* if TRUE, '*' is echoed back to the user instead of the real character  */
    ULONG                           Width;          /* number of columns covered by the Text Box, not counting the brackets   */
}
BMC2_ICE_TEXTBOX_INFO, *PBMC2_ICE_TEXTBOX_INFO;


#endif
