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

    module:	slap_omo_management.c

        For Service Logic Aggregation Plane Implementation (SLAP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced management functions
        of the Slap Obj Mapper Object.

        *   SlapOmoMakObjBroker
        *   SlapOmoGetObjBroker
        *   SlapOmoGetObjEntity1
        *   SlapOmoGetObjEntity2
        *   SlapOmoAddObjEntity
        *   SlapOmoPopObjEntity
        *   SlapOmoDelObjEntity
        *   SlapOmoGetObjProxy1
        *   SlapOmoGetObjProxy2
        *   SlapOmoAddObjProxy
        *   SlapOmoPopObjProxy
        *   SlapOmoDelObjProxy

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        07/04/03    initial revision.

**********************************************************************/


#include "slap_omo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapOmoMakObjBroker
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hTokenChain
            );

    description:

        This function is called to make an obj broker object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hTokenChain
                Specifies the path token chain of the obj broker.

    return:     obj broker object.

**********************************************************************/

ANSC_HANDLE
SlapOmoMakObjBroker
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTokenChain
    )
{
    PSLAP_OBJ_MAPPER_OBJECT         pMyObject       = (PSLAP_OBJ_MAPPER_OBJECT  )hThisObject;
    PSLAP_OBJ_BROKER_OBJECT         pRootObjBroker  = (PSLAP_OBJ_BROKER_OBJECT  )pMyObject->hRootObjBroker;
    PSLAP_OBJ_BROKER_OBJECT         pThisObjBroker  = (PSLAP_OBJ_BROKER_OBJECT  )pRootObjBroker;
    PSLAP_OBJ_BROKER_OBJECT         pNextObjBroker  = (PSLAP_OBJ_BROKER_OBJECT  )NULL;
    PANSC_TOKEN_CHAIN               pPathTokenChain = (PANSC_TOKEN_CHAIN        )hTokenChain;
    PANSC_STRING_TOKEN              pPathToken      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = NULL;

    pSLinkEntry = AnscQueuePopEntry(&pPathTokenChain->TokensQueue);

    while ( pSLinkEntry && pThisObjBroker )
    {
        pPathToken     = ACCESS_ANSC_STRING_TOKEN(pSLinkEntry);
        pNextObjBroker =
            (PSLAP_OBJ_BROKER_OBJECT)pThisObjBroker->GetObjBroker
                (
                    (ANSC_HANDLE)pThisObjBroker,
                    pPathToken->Name
                );

        if ( pNextObjBroker )
        {
            AnscFreeMemory(pPathToken);
        }
        else
        {
            AnscQueuePushEntryAtFront(&pPathTokenChain->TokensQueue, &pPathToken->Linkage);

            break;
        }

        pThisObjBroker = pNextObjBroker;
        pSLinkEntry    = AnscQueuePopEntry(&pPathTokenChain->TokensQueue);
    }

    if ( !pSLinkEntry )
    {
        return  (ANSC_HANDLE)pThisObjBroker;
    }

    pSLinkEntry = AnscQueuePopEntry(&pPathTokenChain->TokensQueue);

    while ( pSLinkEntry && pThisObjBroker )
    {
        pPathToken     = ACCESS_ANSC_STRING_TOKEN(pSLinkEntry);
        pNextObjBroker =
            (PSLAP_OBJ_BROKER_OBJECT)pThisObjBroker->AddObjBroker
                (
                    (ANSC_HANDLE)pThisObjBroker,
                    pPathToken->Name
                );

        if ( pNextObjBroker )
        {
            AnscFreeMemory(pPathToken);
        }
        else
        {
            AnscQueuePushEntryAtFront(&pPathTokenChain->TokensQueue, &pPathToken->Linkage);

            break;
        }

        pThisObjBroker = pNextObjBroker;
        pSLinkEntry    = AnscQueuePopEntry(&pPathTokenChain->TokensQueue);
    }

    if ( pSLinkEntry )
    {
        return  (ANSC_HANDLE)NULL;
    }

    return  (ANSC_HANDLE)pThisObjBroker;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapOmoGetObjBroker
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hTokenChain
            );

    description:

        This function is called to retrieve an obj broker object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hTokenChain
                Specifies the path token chain of the obj broker.

    return:     obj broker object.

**********************************************************************/

ANSC_HANDLE
SlapOmoGetObjBroker
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTokenChain
    )
{
    PSLAP_OBJ_MAPPER_OBJECT         pMyObject       = (PSLAP_OBJ_MAPPER_OBJECT  )hThisObject;
    PSLAP_OBJ_BROKER_OBJECT         pRootObjBroker  = (PSLAP_OBJ_BROKER_OBJECT  )pMyObject->hRootObjBroker;
    PSLAP_OBJ_BROKER_OBJECT         pThisObjBroker  = (PSLAP_OBJ_BROKER_OBJECT  )pRootObjBroker;
    PSLAP_OBJ_BROKER_OBJECT         pNextObjBroker  = (PSLAP_OBJ_BROKER_OBJECT  )NULL;
    PANSC_TOKEN_CHAIN               pPathTokenChain = (PANSC_TOKEN_CHAIN        )hTokenChain;
    PANSC_STRING_TOKEN              pPathToken      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = NULL;

    pSLinkEntry = AnscQueuePopEntry(&pPathTokenChain->TokensQueue);

    while ( pSLinkEntry && pThisObjBroker )
    {
        pPathToken     = ACCESS_ANSC_STRING_TOKEN(pSLinkEntry);
        pNextObjBroker =
            (PSLAP_OBJ_BROKER_OBJECT)pThisObjBroker->GetObjBroker
                (
                    (ANSC_HANDLE)pThisObjBroker,
                    pPathToken->Name
                );

        if ( pNextObjBroker )
        {
            AnscFreeMemory(pPathToken);
        }
        else
        {
            AnscQueuePushEntryAtFront(&pPathTokenChain->TokensQueue, &pPathToken->Linkage);

            break;
        }

        pThisObjBroker = pNextObjBroker;
        pSLinkEntry    = AnscQueuePopEntry(&pPathTokenChain->TokensQueue);
    }

    if ( pSLinkEntry )
    {
        return  (ANSC_HANDLE)NULL;
    }

    return  (ANSC_HANDLE)pThisObjBroker;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapOmoGetObjEntity1
            (
                ANSC_HANDLE                 hThisObject,
                char*                       path
            );

    description:

        This function is called to retrieve an obj entity object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       path
                Specifies the path name of the obj entity to be
                matched.

    return:     obj entity object.

**********************************************************************/

ANSC_HANDLE
SlapOmoGetObjEntity1
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PSLAP_OBJ_MAPPER_OBJECT         pMyObject       = (PSLAP_OBJ_MAPPER_OBJECT  )hThisObject;
    PSLAP_OBJ_ENTITY_OBJECT         pObjEntity      = NULL;
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
                    SLAP_OBJ_PATH_SEPARATORS
                );
    }

    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        pObjEntity =
            pMyObject->GetObjEntity2
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pPathTokenChain
                );
    }
    else
    {
        pObjEntity = (ANSC_HANDLE)NULL;
    }

    AnscDelTokenChain((ANSC_HANDLE)pPathTokenChain);
    AnscFreeMemory   ((ANSC_HANDLE)pPathTokenChain);

    return  (ANSC_HANDLE)pObjEntity;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapOmoGetObjEntity2
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hTokenChain
            );

    description:

        This function is called to retrieve an obj entity object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hTokenChain
                Specifies the path token chain of the obj entity to
                be matched.

    return:     obj entity object.

**********************************************************************/

ANSC_HANDLE
SlapOmoGetObjEntity2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTokenChain
    )
{
    PSLAP_OBJ_MAPPER_OBJECT         pMyObject       = (PSLAP_OBJ_MAPPER_OBJECT  )hThisObject;
    PSLAP_OBJ_BROKER_OBJECT         pRootObjBroker  = (PSLAP_OBJ_BROKER_OBJECT  )pMyObject->hRootObjBroker;
    PSLAP_OBJ_BROKER_OBJECT         pThisObjBroker  = (PSLAP_OBJ_BROKER_OBJECT  )pRootObjBroker;
    PSLAP_OBJ_BROKER_OBJECT         pNextObjBroker  = (PSLAP_OBJ_BROKER_OBJECT  )NULL;
    PSLAP_OBJ_ENTITY_OBJECT         pObjEntity      = (PSLAP_OBJ_ENTITY_OBJECT  )NULL;
    PANSC_TOKEN_CHAIN               pPathTokenChain = (PANSC_TOKEN_CHAIN        )hTokenChain;
    PANSC_STRING_TOKEN              pPathToken      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = NULL;

    if(pThisObjBroker) /*RDKB-6244, CID-24460, null check before use*/
    {
        pObjEntity      = (PSLAP_OBJ_ENTITY_OBJECT  )pThisObjBroker->hSlapObjEntity;
    }

    if ( AnscQueueQueryDepth(&pPathTokenChain->TokensQueue) == 0 )
    {
        pObjEntity = (PSLAP_OBJ_ENTITY_OBJECT)NULL;
    }
    else
    {
        pSLinkEntry = AnscQueuePopEntry(&pPathTokenChain->TokensQueue);

        while ( pSLinkEntry && pThisObjBroker )
        {
            pPathToken     = ACCESS_ANSC_STRING_TOKEN(pSLinkEntry);
            pNextObjBroker =
                (PSLAP_OBJ_BROKER_OBJECT)pThisObjBroker->GetObjBroker
                    (
                        (ANSC_HANDLE)pThisObjBroker,
                        pPathToken->Name
                    );

            if ( pNextObjBroker )
            {
                if ( pNextObjBroker->hSlapObjEntity )
                {
                    pObjEntity = (PSLAP_OBJ_ENTITY_OBJECT)pNextObjBroker->hSlapObjEntity;
                }

                AnscFreeMemory(pPathToken);
            }
            else
            {
                AnscQueuePushEntryAtFront(&pPathTokenChain->TokensQueue, &pPathToken->Linkage);

                break;
            }

            pThisObjBroker = pNextObjBroker;
            pSLinkEntry    = AnscQueuePopEntry(&pPathTokenChain->TokensQueue);
        }

        if ( AnscQueueQueryDepth(&pPathTokenChain->TokensQueue) > 0 )
        {
            pObjEntity = (PSLAP_OBJ_ENTITY_OBJECT)NULL;
        }
    }

    return  (ANSC_HANDLE)pObjEntity;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOmoAddObjEntity
            (
                ANSC_HANDLE                 hThisObject,
                char*                       path,
                ANSC_HANDLE                 hObjEntity
            );

    description:

        This function is called to add an obj entity object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       path
                Specifies the path name of the obj entity object.

                ANSC_HANDLE                 hObjEntity
                Specifies the obj entity to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOmoAddObjEntity
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path,
        ANSC_HANDLE                 hObjEntity
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PSLAP_OBJ_MAPPER_OBJECT         pMyObject       = (PSLAP_OBJ_MAPPER_OBJECT  )hThisObject;
    PSLAP_OBJ_ENTITY_OBJECT         pObjEntity      = (PSLAP_OBJ_ENTITY_OBJECT  )hObjEntity;
    PSLAP_OBJ_BROKER_OBJECT         pThisObjBroker  = NULL;
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
                    SLAP_OBJ_PATH_SEPARATORS
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
        pThisObjBroker =
            (PSLAP_OBJ_BROKER_OBJECT)pMyObject->MakObjBroker
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pPathTokenChain
                );

        AnscDelTokenChain((ANSC_HANDLE)pPathTokenChain);
        AnscFreeMemory   ((ANSC_HANDLE)pPathTokenChain);
    }

    if ( pThisObjBroker )
    {
        if ( pThisObjBroker->hSlapObjEntity != (ANSC_HANDLE)NULL )
        {
            return  ANSC_STATUS_COLLISION;
        }
        else
        {
            pThisObjBroker->hSlapObjEntity = (ANSC_HANDLE)pObjEntity;
        }
    }
    else
    {
        return  ANSC_STATUS_INTERNAL_ERROR;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOmoPopObjEntity
            (
                ANSC_HANDLE                 hThisObject,
                char*                       path
            );

    description:

        This function is called to detach an obj entity object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       path
                Specifies the path name of the obj entity object.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOmoPopObjEntity
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PSLAP_OBJ_MAPPER_OBJECT         pMyObject      = (PSLAP_OBJ_MAPPER_OBJECT  )hThisObject;

    returnStatus =
        pMyObject->DelObjEntity
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
        SlapOmoDelObjEntity
            (
                ANSC_HANDLE                 hThisObject,
                char*                       path,
                BOOL                        bFreeObjEntity
            );

    description:

        This function is called to delete an obj entity object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       path
                Specifies the path name of the obj entity object.

                BOOL                        bFreeObjEntity
                Specifies whether the obj entity object should be
                freed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOmoDelObjEntity
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path,
        BOOL                        bFreeObjEntity
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PSLAP_OBJ_MAPPER_OBJECT         pMyObject       = (PSLAP_OBJ_MAPPER_OBJECT  )hThisObject;
    PSLAP_OBJ_BROKER_OBJECT         pRootObjBroker  = (PSLAP_OBJ_BROKER_OBJECT  )pMyObject->hRootObjBroker;
    PSLAP_OBJ_ENTITY_OBJECT         pObjEntity      = NULL;
    PSLAP_OBJ_BROKER_OBJECT         pThisObjBroker  = NULL;
    PSLAP_OBJ_BROKER_OBJECT         pNextObjBroker  = NULL;
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
                    SLAP_OBJ_PATH_SEPARATORS
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
        pThisObjBroker =
            (PSLAP_OBJ_BROKER_OBJECT)pMyObject->GetObjBroker
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pPathTokenChain
                );

        AnscDelTokenChain((ANSC_HANDLE)pPathTokenChain);
        AnscFreeMemory   ((ANSC_HANDLE)pPathTokenChain);
    }

    if ( pThisObjBroker )
    {
        pObjEntity = (PSLAP_OBJ_ENTITY_OBJECT)pThisObjBroker->hSlapObjEntity;

        if ( pObjEntity )
        {
            pThisObjBroker->hSlapObjEntity = (ANSC_HANDLE)NULL;

            if ( bFreeObjEntity )
            {
                pObjEntity->Remove((ANSC_HANDLE)pObjEntity);
            }
        }
    }
    else
    {
        return  ANSC_STATUS_CANT_FIND;
    }

    /*
     * After removing the associated Obj Entity Object, the Obj Broker Object may have become
     * removeable if there's no more Broker Objects currently associated with it. We shall trim all
     * the unnecessary branches from the bottom to the top.
     */
    while ( pThisObjBroker && (pThisObjBroker != pRootObjBroker) && pThisObjBroker->IsRemoveable((ANSC_HANDLE)pThisObjBroker) )
    {
        pNextObjBroker = pThisObjBroker;
        pThisObjBroker = (PSLAP_OBJ_BROKER_OBJECT)pNextObjBroker->hOwnerContext;

        if(pThisObjBroker) /*RDKB-6244, CID-24348, null check before use*/
        {
            returnStatus   =
                pThisObjBroker->DelObjBroker
                    (
                        (ANSC_HANDLE)pThisObjBroker,
                        pNextObjBroker->PathName
                    );
        }
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapOmoGetObjProxy1
            (
                ANSC_HANDLE                 hThisObject,
                char*                       path
            );

    description:

        This function is called to retrieve an obj proxy object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       path
                Specifies the path name of the obj proxy to be
                matched.

    return:     obj proxy object.

**********************************************************************/

ANSC_HANDLE
SlapOmoGetObjProxy1
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PSLAP_OBJ_MAPPER_OBJECT         pMyObject       = (PSLAP_OBJ_MAPPER_OBJECT  )hThisObject;
    PSLAP_OBJ_PROXY_OBJECT          pObjProxy       = NULL;
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
                    SLAP_OBJ_PATH_SEPARATORS
                );
    }

    if ( returnStatus == ANSC_STATUS_SUCCESS )
    {
        pObjProxy =
            pMyObject->GetObjProxy2
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pPathTokenChain
                );
    }
    else
    {
        pObjProxy = (ANSC_HANDLE)NULL;
    }

    AnscDelTokenChain((ANSC_HANDLE)pPathTokenChain);
    AnscFreeMemory   ((ANSC_HANDLE)pPathTokenChain);

    return  (ANSC_HANDLE)pObjProxy;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SlapOmoGetObjProxy2
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hTokenChain
            );

    description:

        This function is called to retrieve an obj proxy object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hTokenChain
                Specifies the path token chain of the obj proxy to
                be matched.

    return:     obj proxy object.

**********************************************************************/

ANSC_HANDLE
SlapOmoGetObjProxy2
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hTokenChain
    )
{
    PSLAP_OBJ_MAPPER_OBJECT         pMyObject       = (PSLAP_OBJ_MAPPER_OBJECT  )hThisObject;
    PSLAP_OBJ_BROKER_OBJECT         pRootObjBroker  = (PSLAP_OBJ_BROKER_OBJECT  )pMyObject->hRootObjBroker;
    PSLAP_OBJ_BROKER_OBJECT         pThisObjBroker  = (PSLAP_OBJ_BROKER_OBJECT  )pRootObjBroker;
    PSLAP_OBJ_BROKER_OBJECT         pNextObjBroker  = (PSLAP_OBJ_BROKER_OBJECT  )NULL;
    PSLAP_OBJ_PROXY_OBJECT          pObjProxy       = (PSLAP_OBJ_PROXY_OBJECT   )NULL;
    PANSC_TOKEN_CHAIN               pPathTokenChain = (PANSC_TOKEN_CHAIN        )hTokenChain;
    PANSC_STRING_TOKEN              pPathToken      = NULL;
    PSINGLE_LINK_ENTRY              pSLinkEntry     = NULL;

    if(pThisObjBroker) /*RDKB-6244, CID-24128, null check before use*/
    {
        pObjProxy = (PSLAP_OBJ_PROXY_OBJECT	)pThisObjBroker->hSlapObjProxy;
    }

    if ( AnscQueueQueryDepth(&pPathTokenChain->TokensQueue) == 0 )
    {
        pObjProxy = (PSLAP_OBJ_PROXY_OBJECT)NULL;
    }
    else
    {
        pSLinkEntry = AnscQueuePopEntry(&pPathTokenChain->TokensQueue);

        while ( pSLinkEntry && pThisObjBroker )
        {
            pPathToken     = ACCESS_ANSC_STRING_TOKEN(pSLinkEntry);
            pNextObjBroker =
                (PSLAP_OBJ_BROKER_OBJECT)pThisObjBroker->GetObjBroker
                    (
                        (ANSC_HANDLE)pThisObjBroker,
                        pPathToken->Name
                    );

            if ( pNextObjBroker )
            {
                if ( pNextObjBroker->hSlapObjProxy )
                {
                    pObjProxy = (PSLAP_OBJ_PROXY_OBJECT)pNextObjBroker->hSlapObjProxy;
                }

                AnscFreeMemory(pPathToken);
            }
            else
            {
                AnscQueuePushEntryAtFront(&pPathTokenChain->TokensQueue, &pPathToken->Linkage);

                break;
            }

            pThisObjBroker = pNextObjBroker;
            pSLinkEntry    = AnscQueuePopEntry(&pPathTokenChain->TokensQueue);
        }

        if ( AnscQueueQueryDepth(&pPathTokenChain->TokensQueue) > 0 )
        {
            pObjProxy = (PSLAP_OBJ_PROXY_OBJECT)NULL;
        }
    }

    return  (ANSC_HANDLE)pObjProxy;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOmoAddObjProxy
            (
                ANSC_HANDLE                 hThisObject,
                char*                       path,
                ANSC_HANDLE                 hObjProxy
            );

    description:

        This function is called to add an obj proxy object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       path
                Specifies the path name of the obj proxy object.

                ANSC_HANDLE                 hObjProxy
                Specifies the obj proxy to be added.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOmoAddObjProxy
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path,
        ANSC_HANDLE                 hObjProxy
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PSLAP_OBJ_MAPPER_OBJECT         pMyObject       = (PSLAP_OBJ_MAPPER_OBJECT  )hThisObject;
    PSLAP_OBJ_PROXY_OBJECT          pObjProxy       = (PSLAP_OBJ_PROXY_OBJECT   )hObjProxy;
    PSLAP_OBJ_BROKER_OBJECT         pThisObjBroker  = NULL;
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
                    SLAP_OBJ_PATH_SEPARATORS
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
        pThisObjBroker =
            (PSLAP_OBJ_BROKER_OBJECT)pMyObject->MakObjBroker
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pPathTokenChain
                );

        AnscDelTokenChain((ANSC_HANDLE)pPathTokenChain);
        AnscFreeMemory   ((ANSC_HANDLE)pPathTokenChain);
    }

    if ( pThisObjBroker )
    {
        if ( pThisObjBroker->hSlapObjProxy != (ANSC_HANDLE)NULL )
        {
            return  ANSC_STATUS_COLLISION;
        }
        else
        {
            pThisObjBroker->hSlapObjProxy = (ANSC_HANDLE)pObjProxy;
        }
    }
    else
    {
        return  ANSC_STATUS_INTERNAL_ERROR;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SlapOmoPopObjProxy
            (
                ANSC_HANDLE                 hThisObject,
                char*                       path
            );

    description:

        This function is called to detach an obj proxy object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       path
                Specifies the path name of the obj proxy object.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOmoPopObjProxy
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path
    )
{
    ANSC_STATUS                     returnStatus   = ANSC_STATUS_SUCCESS;
    PSLAP_OBJ_MAPPER_OBJECT         pMyObject      = (PSLAP_OBJ_MAPPER_OBJECT  )hThisObject;

    returnStatus =
        pMyObject->DelObjProxy
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
        SlapOmoDelObjProxy
            (
                ANSC_HANDLE                 hThisObject,
                char*                       path,
                BOOL                        bFreeObjProxy
            );

    description:

        This function is called to delete an obj proxy object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       path
                Specifies the path name of the obj proxy object.

                BOOL                        bFreeObjProxy
                Specifies whether the obj proxy object should be
                freed.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SlapOmoDelObjProxy
    (
        ANSC_HANDLE                 hThisObject,
        char*                       path,
        BOOL                        bFreeObjProxy
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PSLAP_OBJ_MAPPER_OBJECT         pMyObject       = (PSLAP_OBJ_MAPPER_OBJECT  )hThisObject;
    PSLAP_OBJ_BROKER_OBJECT         pRootObjBroker  = (PSLAP_OBJ_BROKER_OBJECT  )pMyObject->hRootObjBroker;
    PSLAP_OBJ_PROXY_OBJECT          pObjProxy       = NULL;
    PSLAP_OBJ_BROKER_OBJECT         pThisObjBroker  = NULL;
    PSLAP_OBJ_BROKER_OBJECT         pNextObjBroker  = NULL;
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
                    SLAP_OBJ_PATH_SEPARATORS
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
        pThisObjBroker =
            (PSLAP_OBJ_BROKER_OBJECT)pMyObject->GetObjBroker
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pPathTokenChain
                );

        AnscDelTokenChain((ANSC_HANDLE)pPathTokenChain);
        AnscFreeMemory   ((ANSC_HANDLE)pPathTokenChain);
    }

    if ( pThisObjBroker )
    {
        pObjProxy = (PSLAP_OBJ_PROXY_OBJECT)pThisObjBroker->hSlapObjProxy;

        if ( pObjProxy )
        {
            pThisObjBroker->hSlapObjProxy = (ANSC_HANDLE)NULL;

            if ( bFreeObjProxy )
            {
                pObjProxy->Remove((ANSC_HANDLE)pObjProxy);
            }
        }
    }
    else
    {
        return  ANSC_STATUS_CANT_FIND;
    }

    /*
     * After removing the associated Obj Proxy Object, the Obj Broker Object may have become
     * removeable if there's no more Broker Objects currently associated with it. We shall trim all
     * the unnecessary branches from the bottom to the top.
     */
    while ( pThisObjBroker && (pThisObjBroker != pRootObjBroker) && pThisObjBroker->IsRemoveable((ANSC_HANDLE)pThisObjBroker) )
    {
        pNextObjBroker = pThisObjBroker;
        pThisObjBroker = (PSLAP_OBJ_BROKER_OBJECT)pNextObjBroker->hOwnerContext;

        if(pThisObjBroker) /*RDKB-6244, CID-24290, null check before use*/
        {
            returnStatus   =
                pThisObjBroker->DelObjBroker
                    (
                        (ANSC_HANDLE)pThisObjBroker,
                        pNextObjBroker->PathName
                    );
        }
    }

    return  ANSC_STATUS_SUCCESS;
}
