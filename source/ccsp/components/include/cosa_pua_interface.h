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

    module:	cosa_pua_interface.h

        For COSA Plugin Universal Acccess (PUA) interface
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the COSA Plugin Universal Access interface.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        01/10/2011  initial revision.

**********************************************************************/


#ifndef  _COSA_PUAI_INTERFACE_API_
#define  _COSA_PUAI_INTERFACE_API_


typedef  void*
(*PFN_COSAPUAIF_ACQ_POAM_OBJ)
    (
        void*                       pPoamObjDescr
    );

typedef  int
(*PFN_COSAPUAIF_REL_POAM_OBJ)
    (
        void*                       pPoamObject
    );


/*
 * SLAP_POA interface is built on top of SLAP_GOA. It allows the upper control/management modules
 * to access any proxied Service Logic Object via the regular object member functions instead of
 * the mandatory Invoke() entry point. SLAP_POA should be used to access SLAP_LOA objects only, but
 * there's no limitation against using it to access SLAP_UOA objects.
 */
#define  COSA_PUA_INTERFACE_CLASS_CONTENT                                                   \
    /* start of object class content */                                                     \
    PFN_COSAPUAIF_ACQ_POAM_OBJ      AcqPoamObjAccess;                                       \
    PFN_COSAPUAIF_REL_POAM_OBJ      RelPoamObjAccess;                                       \
    /* end of object class content */                                                       \

typedef  struct
_COSA_PUA_INTERFACE
{
    COSA_PUA_INTERFACE_CLASS_CONTENT
}
COSA_PUA_INTERFACE,  *PCOSA_PUA_INTERFACE;


#endif
