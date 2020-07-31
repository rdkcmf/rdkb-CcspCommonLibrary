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

    module:	bmc2_comto_management.c

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the Bmc2 Com Terminal Object.

        *   Bmc2ComtoGetDomainCount
        *   Bmc2ComtoGetCurComDomain
        *   Bmc2ComtoAddComDomain
        *   Bmc2ComtoDelCurComDomain
        *   Bmc2ComtoDelComDomain
        *   Bmc2ComtoDelAllComDomains

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/27/05    initial revision.

**********************************************************************/


#include "bmc2_comto_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        Bmc2ComtoGetDomainCount
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the domain count.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     domain count.

**********************************************************************/

ULONG
Bmc2ComtoGetDomainCount
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_COM_TERMINAL_OBJECT       pMyObject    = (PBMC2_COM_TERMINAL_OBJECT  )hThisObject;

    return  AnscSListQueryDepth(&pMyObject->ComdoSList);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        Bmc2ComtoGetCurComDomain
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve the current com domain.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     com domain.

**********************************************************************/

ANSC_HANDLE
Bmc2ComtoGetCurComDomain
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_COM_TERMINAL_OBJECT       pMyObject      = (PBMC2_COM_TERMINAL_OBJECT  )hThisObject;
    PBMC2_COM_DOMAIN_OBJECT         pBmc2ComDomain = (PBMC2_COM_DOMAIN_OBJECT    )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry    = (PSINGLE_LINK_ENTRY         )NULL;

    AnscAcquireLock(&pMyObject->ComdoSListLock);
    pSLinkEntry = AnscSListGetFirstEntry(&pMyObject->ComdoSList);
    AnscReleaseLock(&pMyObject->ComdoSListLock);

    if ( pSLinkEntry )
    {
        pBmc2ComDomain = ACCESS_BMC2_COM_DOMAIN_OBJECT(pSLinkEntry);
    }

    return  pBmc2ComDomain;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ComtoAddComDomain
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hComDomain
            );

    description:

        This function is called to add a com domain.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hComDomain
                Specifies the com domain to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ComtoAddComDomain
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hComDomain
    )
{
    PBMC2_COM_TERMINAL_OBJECT       pMyObject      = (PBMC2_COM_TERMINAL_OBJECT  )hThisObject;
    PBMC2_COM_DOMAIN_OBJECT         pBmc2ComDomain = (PBMC2_COM_DOMAIN_OBJECT    )hComDomain;

    AnscAcquireLock   (&pMyObject->ComdoSListLock);
    AnscSListPushEntry(&pMyObject->ComdoSList, &pBmc2ComDomain->Linkage);
    AnscReleaseLock   (&pMyObject->ComdoSListLock);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ComtoDelCurComDomain
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to delete the current com domain.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ComtoDelCurComDomain
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_COM_TERMINAL_OBJECT       pMyObject      = (PBMC2_COM_TERMINAL_OBJECT  )hThisObject;
    PBMC2_COM_DOMAIN_OBJECT         pBmc2ComDomain = (PBMC2_COM_DOMAIN_OBJECT    )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry    = (PSINGLE_LINK_ENTRY         )NULL;

    AnscAcquireLock(&pMyObject->ComdoSListLock);
    pSLinkEntry = AnscSListPopEntry(&pMyObject->ComdoSList);
    AnscReleaseLock(&pMyObject->ComdoSListLock);

    if ( pSLinkEntry )
    {
        pBmc2ComDomain = ACCESS_BMC2_COM_DOMAIN_OBJECT(pSLinkEntry);

        pBmc2ComDomain->Cancel((ANSC_HANDLE)pBmc2ComDomain);
        pBmc2ComDomain->Remove((ANSC_HANDLE)pBmc2ComDomain);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ComtoDelComDomain
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hComDomain
            );

    description:

        This function is called to delete the specified com domain.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hComDomain
                Specifies the com domain to be deleted.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ComtoDelComDomain
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hComDomain
    )
{
    PBMC2_COM_TERMINAL_OBJECT       pMyObject      = (PBMC2_COM_TERMINAL_OBJECT  )hThisObject;
    PBMC2_COM_DOMAIN_OBJECT         pBmc2ComDomain = (PBMC2_COM_DOMAIN_OBJECT    )hComDomain;

    AnscAcquireLock(&pMyObject->ComdoSListLock);
    AnscSListPopEntryByLink(&pMyObject->ComdoSList, &pBmc2ComDomain->Linkage);
    AnscReleaseLock(&pMyObject->ComdoSListLock);

    pBmc2ComDomain->Cancel((ANSC_HANDLE)pBmc2ComDomain);
    pBmc2ComDomain->Remove((ANSC_HANDLE)pBmc2ComDomain);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ComtoDelAllComDomains
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to delete all com domains.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ComtoDelAllComDomains
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_COM_TERMINAL_OBJECT       pMyObject      = (PBMC2_COM_TERMINAL_OBJECT  )hThisObject;
    PBMC2_COM_DOMAIN_OBJECT         pBmc2ComDomain = (PBMC2_COM_DOMAIN_OBJECT    )NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry    = (PSINGLE_LINK_ENTRY         )NULL;

    AnscAcquireLock(&pMyObject->ComdoSListLock);

    pSLinkEntry = AnscSListPopEntry(&pMyObject->ComdoSList);

    while ( pSLinkEntry )
    {
        pBmc2ComDomain = ACCESS_BMC2_COM_DOMAIN_OBJECT(pSLinkEntry);
        pSLinkEntry    = AnscSListPopEntry(&pMyObject->ComdoSList);

        pBmc2ComDomain->Cancel((ANSC_HANDLE)pBmc2ComDomain);
        pBmc2ComDomain->Remove((ANSC_HANDLE)pBmc2ComDomain);
    }

    AnscReleaseLock(&pMyObject->ComdoSListLock);

    return  ANSC_STATUS_SUCCESS;
}
