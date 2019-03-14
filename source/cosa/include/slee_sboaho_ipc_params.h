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

    module:	slee_sboaho_ipc_params.h

        For Service Logic Execution Environment Container (SLEEC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the input and output structures
        for communications between Sce and Nsm.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/09/01    initial revision.

**********************************************************************/


#ifndef  _SLEE_SBOAHO_IPC_PARAMS_
#define  _SLEE_SBOAHO_IPC_PARAMS_


/***********************************************************
   DEFFINITIONS FOR CONFIGURE PARAMS AND RETURN STRUCTURES
***********************************************************/

/*
 * The trade-off between the simplicity and the finer control over the runtime objects has always
 * been the endless battle inside developer's mind. I have set a ground rule for Slee objects: if
 * the design and structure of objects are well modulized and finely devided, the configuration
 * interface may be implemented loosely.
 */
#define  SLEE_SBOAHO_CFG_CODE_BASE                  0x00000000
#define  SLEE_SBOAHO_CFG_CODE_SETUP                 SLEE_SBOAHO_CFG_CODE_BASE  + 0x0001

/*
 * The Ipc param for any configuration channel command requires the runtime object id as the
 * identifier. The rid in the general header specifies the targeted Ipc receiver. However, most
 * configuration commands also include another rid, which usually identifies the related object
 * that current request refers to.
 */
#define  SLEE_SBOAHO_IPCP_CFG_CONTENT                                                       \
    /* duplication of the base object class content */                                      \
    NSM_IPC_PARAMS_CONTENT                                                                  \
    /* start of object class content */                                                     \
    ULONG                           CfgSize;                                                \
    ULONG                           CfgCode;                                                \
    ULONG                           TrunkCount;                                             \
    ANSC_AH_TRUNK                   TrunkArray[ANSC_ZERO_ARRAY_SIZE];                       \
    /* end of object class content */                                                       \

typedef  struct
_SLEE_SBOAHO_IPCP_CFG
{
    SLEE_SBOAHO_IPCP_CFG_CONTENT
}
SLEE_SBOAHO_IPCP_CFG,  *PSLEE_SBOAHO_IPCP_CFG;

#define  SLEE_SBOAHO_IPCR_CFG_CONTENT                                                       \
    /* duplication of the base object class content */                                      \
    NSM_IPC_RETURN_CONTENT                                                                  \
    /* start of object class content */                                                     \
    /* end of object class content */                                                       \

typedef  struct
_SLEE_SBOAHO_IPCR_CFG
{
    SLEE_SBOAHO_IPCR_CFG_CONTENT
}
SLEE_SBOAHO_IPCR_CFG,  *PSLEE_SBOAHO_IPCR_CFG;


#endif
