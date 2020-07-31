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

    module:	web_rlo_management.c

        For Web Server/Client/Application Implementation (WEB),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the Web Resource Locator Object.

        *   WebRloMakUriPath
        *   WebRloGetUriPath
        *   WebRloGetResourceOwner1
        *   WebRloGetResourceOwner2
        *   WebRloAddResourceOwner
        *   WebRloPopResourceOwner
        *   WebRloDelResourceOwner

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/12/02    initial revision.

**********************************************************************/


#include "web_rlo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        WebRloMakUriPath
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hTokenChain
            );

    description:

        This function is called to make a uri path object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hTokenChain
                Specifies the path token chain of the uri path.

    return:     uri path object.

**********************************************************************/

ANSC_HANDLE
WebRloMakUriPath
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTokenChain
    )
{
    PWEB_RESOURCE_LOCATOR_OBJECT    pMyObject       = (PWEB_RESOURCE_LOCATOR_OBJECT  )hThisObject;
    PWEB_URI_PATH_OBJECT            pRootUriPath    = (PWEB_URI_PATH_OBJECT          )pMyObject->hRootUriPath;
    PWEB_URI_PATH_OBJECT            pThisUriPath    = (PWEB_URI_PATH_OBJECT          )pRootUriPath;
    PWEB_URI_PATH_OBJECT            pNextUriPath    = (PWEB_URI_PATH_OBJECT          )NULL;
    PANSC_TOKEN_CHAIN               pPathTokenChain = (PANSC_TOKEN_CHAIN             )hTokenChain;
    PANSC_STRING_TOKEN              pPathToken      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = NULL;

    pSLinkEntry = AnscQueuePopEntry(&pPathTokenChain->TokensQueue);

    while ( pSLinkEntry && pThisUriPath )
    {
        pPathToken   = ACCESS_ANSC_STRING_TOKEN(pSLinkEntry);
        pNextUriPath =
            (PWEB_URI_PATH_OBJECT)pThisUriPath->GetUriPath
                (
                    (ANSC_HANDLE)pThisUriPath,
                    pPathToken->Name
                );

        if ( pNextUriPath )
        {
            AnscFreeMemory(pPathToken);
        }
        else
        {
            AnscQueuePushEntryAtFront(&pPathTokenChain->TokensQueue, &pPathToken->Linkage);

            break;
        }

        pThisUriPath = pNextUriPath;
        pSLinkEntry  = AnscQueuePopEntry(&pPathTokenChain->TokensQueue);
    }

    if ( !pSLinkEntry )
    {
        return  (ANSC_HANDLE)pThisUriPath;
    }

    pSLinkEntry = AnscQueuePopEntry(&pPathTokenChain->TokensQueue);

    while ( pSLinkEntry && pThisUriPath )
    {
        pPathToken   = ACCESS_ANSC_STRING_TOKEN(pSLinkEntry);
        pNextUriPath =
            (PWEB_URI_PATH_OBJECT)pThisUriPath->AddUriPath
                (
                    (ANSC_HANDLE)pThisUriPath,
                    pPathToken->Name
                );

        if ( pNextUriPath )
        {
            AnscFreeMemory(pPathToken);
        }
        else
        {
            AnscQueuePushEntryAtFront(&pPathTokenChain->TokensQueue, &pPathToken->Linkage);

            break;
        }

        pThisUriPath = pNextUriPath;
        pSLinkEntry  = AnscQueuePopEntry(&pPathTokenChain->TokensQueue);
    }

    if ( pSLinkEntry )
    {
        return  (ANSC_HANDLE)NULL;
    }

    return  (ANSC_HANDLE)pThisUriPath;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        WebRloGetUriPath
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hTokenChain
            );

    description:

        This function is called to retrieve a uri path object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hTokenChain
                Specifies the path token chain of the uri path.

    return:     uri path object.

**********************************************************************/

ANSC_HANDLE
WebRloGetUriPath
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTokenChain
    )
{
    PWEB_RESOURCE_LOCATOR_OBJECT    pMyObject       = (PWEB_RESOURCE_LOCATOR_OBJECT  )hThisObject;
    PWEB_URI_PATH_OBJECT            pRootUriPath    = (PWEB_URI_PATH_OBJECT          )pMyObject->hRootUriPath;
    PWEB_URI_PATH_OBJECT            pThisUriPath    = (PWEB_URI_PATH_OBJECT          )pRootUriPath;
    PWEB_URI_PATH_OBJECT            pNextUriPath    = (PWEB_URI_PATH_OBJECT          )NULL;
    PANSC_TOKEN_CHAIN               pPathTokenChain = (PANSC_TOKEN_CHAIN             )hTokenChain;
    PANSC_STRING_TOKEN              pPathToken      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = NULL;

    pSLinkEntry = AnscQueuePopEntry(&pPathTokenChain->TokensQueue);

    while ( pSLinkEntry && pThisUriPath )
    {
        pPathToken   = ACCESS_ANSC_STRING_TOKEN(pSLinkEntry);
        pNextUriPath =
            (PWEB_URI_PATH_OBJECT)pThisUriPath->GetUriPath
                (
                    (ANSC_HANDLE)pThisUriPath,
                    pPathToken->Name
                );

        if ( pNextUriPath )
        {
            AnscFreeMemory(pPathToken);
        }
        else
        {
            AnscQueuePushEntryAtFront(&pPathTokenChain->TokensQueue, &pPathToken->Linkage);

            break;
        }

        pThisUriPath = pNextUriPath;
        pSLinkEntry  = AnscQueuePopEntry(&pPathTokenChain->TokensQueue);
    }

    if ( pSLinkEntry )
    {
        return  (ANSC_HANDLE)NULL;
    }

    return  (ANSC_HANDLE)pThisUriPath;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        WebRloGetResourceOwner1
            (
                ANSC_HANDLE                 hThisObject,
                char*                       path
            );

    description:

        This function is called to retrieve a resource owner object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       path
                Specifies the path name of the resource owner to be
                matched.

    return:     resource owner object.

**********************************************************************/

ANSC_HANDLE
WebRloGetResourceOwner1
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PWEB_RESOURCE_LOCATOR_OBJECT    pMyObject       = (PWEB_RESOURCE_LOCATOR_OBJECT  )hThisObject;
    PWEB_RESOURCE_OWNER_OBJECT      pResourceOwner  = NULL;
    PANSC_TOKEN_CHAIN               pPathTokenChain = NULL;

    pPathTokenChain = (PANSC_TOKEN_CHAIN)AnscAllocateMemory(sizeof(ANSC_TOKEN_CHAIN));

    if ( !pPathTokenChain )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        returnStatus =
            AnscSetTokenChain
                (
                    (ANSC_HANDLE)pPathTokenChain,
                    path,
                    WEB_RLO_PATH_SEPARATORS
                );
    }

    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        pResourceOwner =
            pMyObject->GetResourceOwner2
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pPathTokenChain
                );
    }
    else
    {
        pResourceOwner = (ANSC_HANDLE)NULL;
    }

    AnscDelTokenChain((ANSC_HANDLE)pPathTokenChain);
    AnscFreeMemory   ((ANSC_HANDLE)pPathTokenChain);

    return  (ANSC_HANDLE)pResourceOwner;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        WebRloGetResourceOwner2
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hTokenChain
            );

    description:

        This function is called to retrieve a resource owner object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hTokenChain
                Specifies the path token chain of the resource owner to
                be matched.

    return:     resource owner object.

**********************************************************************/

ANSC_HANDLE
WebRloGetResourceOwner2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTokenChain
    )
{
    PWEB_RESOURCE_LOCATOR_OBJECT    pMyObject       = (PWEB_RESOURCE_LOCATOR_OBJECT  )hThisObject;
    PWEB_URI_PATH_OBJECT            pRootUriPath    = (PWEB_URI_PATH_OBJECT          )pMyObject->hRootUriPath;
    PWEB_URI_PATH_OBJECT            pThisUriPath    = (PWEB_URI_PATH_OBJECT          )pRootUriPath;
    PWEB_URI_PATH_OBJECT            pNextUriPath    = (PWEB_URI_PATH_OBJECT          )NULL;
    PWEB_RESOURCE_OWNER_OBJECT      pResourceOwner  = (PWEB_RESOURCE_OWNER_OBJECT    )NULL;
    PANSC_TOKEN_CHAIN               pPathTokenChain = (PANSC_TOKEN_CHAIN             )hTokenChain;
    PANSC_STRING_TOKEN              pPathToken      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = NULL;

    if ( AnscQueueQueryDepth(&pPathTokenChain->TokensQueue) == 0 )
    {
        pResourceOwner = (PWEB_RESOURCE_OWNER_OBJECT)pMyObject->hDefRoo;
    }
    else
    {
        /*RDKB-6312, CID-24369, "pThisUriPath"/"pRootUriPath" may null, vallidate before use */
        if(pThisUriPath)
        {
            pResourceOwner  = (PWEB_RESOURCE_OWNER_OBJECT    )pThisUriPath->GetResourceOwner((ANSC_HANDLE)pThisUriPath);
        }

        pSLinkEntry = AnscQueuePopEntry(&pPathTokenChain->TokensQueue);

        while ( pSLinkEntry && pThisUriPath )
        {
            pPathToken   = ACCESS_ANSC_STRING_TOKEN(pSLinkEntry);
            pNextUriPath =
                (PWEB_URI_PATH_OBJECT)pThisUriPath->GetUriPath
                    (
                        (ANSC_HANDLE)pThisUriPath,
                        pPathToken->Name
                    );

            if ( pNextUriPath )
            {
                if ( pNextUriPath->hResourceOwner )
                {
                    pResourceOwner = (PWEB_RESOURCE_OWNER_OBJECT)pNextUriPath->hResourceOwner;
                }

                AnscFreeMemory(pPathToken);
            }
            else
            {
                AnscQueuePushEntryAtFront(&pPathTokenChain->TokensQueue, &pPathToken->Linkage);

                break;
            }

            pThisUriPath = pNextUriPath;
            pSLinkEntry  = AnscQueuePopEntry(&pPathTokenChain->TokensQueue);
        }
    }

    return  (ANSC_HANDLE)pResourceOwner;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRloAddResourceOwner
            (
                ANSC_HANDLE                 hThisObject,
                char*                       path,
                ANSC_HANDLE                 hRoo
            );

    description:

        This function is called to add a resource owner object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       path
                Specifies the path name of the resource owner object.

                ANSC_HANDLE                 hRoo
                Specifies the resource owner to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebRloAddResourceOwner
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path,
        ANSC_HANDLE                 hRoo
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PWEB_RESOURCE_LOCATOR_OBJECT    pMyObject       = (PWEB_RESOURCE_LOCATOR_OBJECT  )hThisObject;
    PWEB_RESOURCE_OWNER_OBJECT      pResourceOwner  = (PWEB_RESOURCE_OWNER_OBJECT    )hRoo;
    PWEB_URI_PATH_OBJECT            pThisUriPath    = NULL;
    PANSC_TOKEN_CHAIN               pPathTokenChain = NULL;

    pPathTokenChain = (PANSC_TOKEN_CHAIN)AnscAllocateMemory(sizeof(ANSC_TOKEN_CHAIN));

    if ( !pPathTokenChain )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        returnStatus =
            AnscSetTokenChain
                (
                    (ANSC_HANDLE)pPathTokenChain,
                    path,
                    WEB_RLO_PATH_SEPARATORS
                );
    }

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        AnscDelTokenChain((ANSC_HANDLE)pPathTokenChain);
        AnscFreeMemory   ((ANSC_HANDLE)pPathTokenChain);

        return  returnStatus;
    }
    else
    {
        pThisUriPath =
            (PWEB_URI_PATH_OBJECT)pMyObject->MakUriPath
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pPathTokenChain
                );

        AnscDelTokenChain((ANSC_HANDLE)pPathTokenChain);
        AnscFreeMemory   ((ANSC_HANDLE)pPathTokenChain);
    }

    if ( pThisUriPath )
    {
        if ( pThisUriPath->GetResourceOwner((ANSC_HANDLE)pThisUriPath) != (ANSC_HANDLE)NULL )
        {
            return  ANSC_STATUS_COLLISION;
        }
        else
        {
            pThisUriPath->SetResourceOwner((ANSC_HANDLE)pThisUriPath, (ANSC_HANDLE)pResourceOwner);
        }
    }
    else
    {
        return  ANSC_STATUS_INTERNAL_ERROR;
    }

    if ( pResourceOwner->IsDefaultOwner((ANSC_HANDLE)pResourceOwner) )
    {
        pMyObject->hDefRoo = (ANSC_HANDLE)pResourceOwner;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRloPopResourceOwner
            (
                ANSC_HANDLE                 hThisObject,
                char*                       path
            );

    description:

        This function is called to detach a resource owner object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       path
                Specifies the path name of the resource owner object.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebRloPopResourceOwner
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PWEB_RESOURCE_LOCATOR_OBJECT    pMyObject       = (PWEB_RESOURCE_LOCATOR_OBJECT  )hThisObject;

    returnStatus =
        pMyObject->DelResourceOwner
            (
                (ANSC_HANDLE)pMyObject,
                path,
                FALSE
            );

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        WebRloDelResourceOwner
            (
                ANSC_HANDLE                 hThisObject,
                char*                       path,
                BOOL                        bFreeRoo
            );

    description:

        This function is called to delete a resource owner object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       path
                Specifies the path name of the resource owner object.

                BOOL                        bFreeRoo
                Specifies whether the resource owner object should be
                freed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
WebRloDelResourceOwner
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path,
        BOOL                        bFreeRoo
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PWEB_RESOURCE_LOCATOR_OBJECT    pMyObject       = (PWEB_RESOURCE_LOCATOR_OBJECT  )hThisObject;
    PWEB_URI_PATH_OBJECT            pRootUriPath    = (PWEB_URI_PATH_OBJECT          )pMyObject->hRootUriPath;
    PWEB_RESOURCE_OWNER_OBJECT      pResourceOwner  = NULL;
    PWEB_URI_PATH_OBJECT            pThisUriPath    = NULL;
    PWEB_URI_PATH_OBJECT            pNextUriPath    = NULL;
    PANSC_TOKEN_CHAIN               pPathTokenChain = NULL;

    pPathTokenChain = (PANSC_TOKEN_CHAIN)AnscAllocateMemory(sizeof(ANSC_TOKEN_CHAIN));

    if ( !pPathTokenChain )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        returnStatus =
            AnscSetTokenChain
                (
                    (ANSC_HANDLE)pPathTokenChain,
                    path,
                    WEB_RLO_PATH_SEPARATORS
                );
    }

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        AnscDelTokenChain((ANSC_HANDLE)pPathTokenChain);
        AnscFreeMemory   ((ANSC_HANDLE)pPathTokenChain);

        return  returnStatus;
    }
    else
    {
        pThisUriPath =
            (PWEB_URI_PATH_OBJECT)pMyObject->GetUriPath
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pPathTokenChain
                );

        AnscDelTokenChain((ANSC_HANDLE)pPathTokenChain);
        AnscFreeMemory   ((ANSC_HANDLE)pPathTokenChain);
    }

    if ( pThisUriPath )
    {
        pResourceOwner = (PWEB_RESOURCE_OWNER_OBJECT)pThisUriPath->GetResourceOwner((ANSC_HANDLE)pThisUriPath);

        if ( pResourceOwner )
        {
            pThisUriPath->SetResourceOwner((ANSC_HANDLE)pThisUriPath, (ANSC_HANDLE)NULL);

            if ( pResourceOwner->IsDefaultOwner((ANSC_HANDLE)pResourceOwner) )
            {
                pMyObject->hDefRoo = (ANSC_HANDLE)NULL;
            }

            if ( bFreeRoo )
            {
                pResourceOwner->Remove((ANSC_HANDLE)pResourceOwner);
            }
        }
    }
    else
    {
        return  ANSC_STATUS_CANT_FIND;
    }

    /*
     * After removing the associated Resource Owner Object, the Uri Path Object may have become
     * removeable if there's no more Path Objects currently associated with it. We shall trim all
     * the unnecessary branches from the bottom to the top.
     */
    while ( pThisUriPath && (pThisUriPath != pRootUriPath) && pThisUriPath->IsRemoveable((ANSC_HANDLE)pThisUriPath) )
    {
        pNextUriPath = pThisUriPath;
        pThisUriPath = (PWEB_URI_PATH_OBJECT)pNextUriPath->hOwnerContext;
        /*RDKB-6312, CID-24197, Null check to avoid crash updated "pThisUriPath" before use*/
        if(pThisUriPath)
        {
            returnStatus =
                pThisUriPath->DelUriPath
                    (
                        (ANSC_HANDLE)pThisUriPath,
                        pNextUriPath->GetPathName((ANSC_HANDLE)pNextUriPath)
                    );
        }
    }

    return  ANSC_STATUS_SUCCESS;
}
