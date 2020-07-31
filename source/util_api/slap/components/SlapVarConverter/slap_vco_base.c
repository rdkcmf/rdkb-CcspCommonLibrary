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

    module:	slap_vco_base.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic container object functions
        of the Slap Var Converter Object.

        *   SlapVcoCreate
        *   SlapVcoRemove
        *   SlapVcoEnrollObjects
        *   SlapVcoInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Bin Zhu

    ---------------------------------------------------------------

    revision:

        07/13/2003    initial revision.
        08/09/2010    Bin added ipv6 support
        09/30/2010    Bin added ipv6_list support.

**********************************************************************/


#include "slap_vco_global.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        SlapVcoCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the Slap Var Converter Object and
        initializes the member variables and functions.

    argument:   ANSC_HANDLE                 hContainerContext
                This handle is used by the container object to interact
                with the outside world. It could be the real container
                or an target object.

                ANSC_HANDLE                 hOwnerContext
                This handle is passed in by the owner of this object.

                ANSC_HANDLE                 hAnscReserved
                This handle is passed in by the owner of this object.

    return:     newly created container object.

**********************************************************************/

ANSC_HANDLE
SlapVcoCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_LIGHT_COMPONENT_OBJECT    pBaseObject  = NULL;
    PSLAP_VAR_CONVERTER_OBJECT      pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PSLAP_VAR_CONVERTER_OBJECT)AnscAllocateMemory(sizeof(SLAP_VAR_CONVERTER_OBJECT));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pBaseObject = (PANSC_LIGHT_COMPONENT_OBJECT)pMyObject;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = SLAP_VAR_CONVERTER_OID;
    pBaseObject->Create            = SlapVcoCreate;
    pBaseObject->Remove            = SlapVcoRemove;
    pBaseObject->EnrollObjects     = SlapVcoEnrollObjects;
    pBaseObject->Initialize        = SlapVcoInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapVcoRemove
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function destroys the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapVcoRemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_VAR_CONVERTER_OBJECT      pMyObject    = (PSLAP_VAR_CONVERTER_OBJECT)hThisObject;

    SlapScoStdRemove((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapVcoEnrollObjects
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function enrolls all the objects required by this object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapVcoEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_VAR_CONVERTER_OBJECT      pMyObject    = (PSLAP_VAR_CONVERTER_OBJECT)hThisObject;

    SlapScoStdEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapVcoInitialize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function first calls the initialization member function
        of the base class object to set the common member fields
        inherited from the base class. It then initializes the member
        fields that are specific to this object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapVcoInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PSLAP_VAR_CONVERTER_OBJECT      pMyObject    = (PSLAP_VAR_CONVERTER_OBJECT)hThisObject;

    /*
     * Until you have to simulate C++ object-oriented programming style with standard C, you don't
     * appreciate all the nice little things come with C++ language and all the dirty works that
     * have been done by the C++ compilers. Member initialization is one of these things. While in
     * C++ you don't have to initialize all the member fields inherited from the base class since
     * the compiler will do it for you, such is not the case with C.
     */
    SlapScoStdInitialize((ANSC_HANDLE)pMyObject);

    /*
     * Although we have initialized some of the member fields in the "create" member function, we
     * repeat the work here for completeness. While this simulation approach is pretty stupid from
     * a C++/Java programmer perspective, it's the best we can get for universal embedded network
     * programming. Before we develop our own operating system (don't expect that to happen any
     * time soon), this is the way things gonna be.
     */
    pMyObject->Oid                      = SLAP_VAR_CONVERTER_OID;
    pMyObject->Create                   = SlapVcoCreate;
    pMyObject->Remove                   = SlapVcoRemove;
    pMyObject->EnrollObjects            = SlapVcoEnrollObjects;
    pMyObject->Initialize               = SlapVcoInitialize;

    pMyObject->Reset                    = SlapVcoReset;

    pMyObject->ConvertVariable          = SlapVcoConvertVariable;

    pMyObject->BoolToString             = SlapVcoBoolToString;
    pMyObject->IntToString              = SlapVcoIntToString;
    pMyObject->UcharArrayToString       = SlapVcoUcharArrayToString;
    pMyObject->UcharArrayToBase64String = SlapVcoUcharArrayToBase64String;
    pMyObject->UcharArrayToString2      = SlapVcoUcharArrayToString2;
    pMyObject->Uint32ToString           = SlapVcoUint32ToString;
    pMyObject->Ip4AddrToString          = SlapVcoIp4AddrToString;
    pMyObject->Ip4AddrToString2         = SlapVcoIp4AddrToString2;
    pMyObject->Ip4AddrListToString      = SlapVcoIp4AddrListToString;
    pMyObject->Ip6AddrListToString      = SlapVcoIp6AddrListToString;
    pMyObject->Ip6AddrToString          = SlapVcoIp6AddrToString;
    pMyObject->MacAddrToString          = SlapVcoMacAddrToString;
    pMyObject->MacAddrToString2         = SlapVcoMacAddrToString2;
    pMyObject->MacAddrListToString      = SlapVcoMacAddrListToString;
    pMyObject->OidListToString          = SlapVcoOidListToString;
    pMyObject->CalendarTimeToString     = SlapVcoCalendarTimeToString;
    pMyObject->Uint32ToHexString        = SlapVcoUint32ToHexString;

    pMyObject->StringToBool             = SlapVcoStringToBool;
    pMyObject->StringToInt              = SlapVcoStringToInt;
    pMyObject->StringToUcharArray       = SlapVcoStringToUcharArray;
    pMyObject->Base64StringToUcharArray = SlapVcoBase64StringToUcharArray;
    pMyObject->StringToUcharArray2      = SlapVcoStringToUcharArray2;
    pMyObject->StringToUint32           = SlapVcoStringToUint32;
    pMyObject->StringToIp4Addr          = SlapVcoStringToIp4Addr;
    pMyObject->StringToIp4AddrList      = SlapVcoStringToIp4AddrList;
    pMyObject->StringToIp6AddrList      = SlapVcoStringToIp6AddrList;
    pMyObject->StringToIp6Addr          = SlapVcoStringToIp6Addr;
    pMyObject->StringToMacAddr          = SlapVcoStringToMacAddr;
    pMyObject->StringToMacAddrList      = SlapVcoStringToMacAddrList;
    pMyObject->StringToOidList          = SlapVcoStringToOidList;
    pMyObject->StringToCalendarTime     = SlapVcoStringToCalendarTime;
    pMyObject->HexStringToUint32        = SlapVcoHexStringToUint32;
    pMyObject->HexStringToDecimal       = SlapVcoHexStringToDecimal;

    pMyObject->Uint32ArrayToMacAddr     = SlapVcoUint32ArrayToMacAddr;

    pMyObject->Uint32ToUcharArray       = SlapVcoUint32ToUcharArray;
    pMyObject->Uint32ToUchar            = SlapVcoUint32ToUchar;
    pMyObject->UcharArrayToUint32       = SlapVcoUcharArrayToUint32;

    return  ANSC_STATUS_SUCCESS;
}
