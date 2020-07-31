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

    module:	http_hco_feature.c

        For Real-time Transport Protocol Component (HTTPC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Http Helper Container Object.

        *   HttpHcoEnrollFeatureObjects
        *   HttpHcoManufactureFeatureObjects
        *   HttpHcoDestroyFeatureObjects

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        03/14/02    initial revision.

**********************************************************************/


#include "http_hco_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpHcoEnrollFeatureObjects
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to enroll all the internal objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpHcoEnrollFeatureObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_HELPER_CONTAINER_OBJECT   pMyObject             = (PHTTP_HELPER_CONTAINER_OBJECT)hThisObject;
    PANSC_OBJECT_MAPPER_OBJECT      pInternalObjectMapper = (PANSC_OBJECT_MAPPER_OBJECT   )pMyObject->hInternalObjectMapper;


    /******************************************************************
                  REGISTER ALL THE DEFAULT FEATURE OBJECTS
    ******************************************************************/

    /*
     * Feature objects are visible to the external modules, and can be manufactured and/or
     * destroyed by either external modules or the container. The Ato Status Object is such
     * an object.
     */
    
    pInternalObjectMapper->RegisterObject
            (
                (ANSC_HANDLE)pInternalObjectMapper,
                HTTP_ATO_STATUS_NAME,
                HTTP_ATO_STATUS_OID,
                ANSC_ATOM_TABLE_NAME,
                ANSC_ATOM_TABLE_OID,
                HTTP_ATO_STATUS_TYPE,
                ANSC_OBJECT_OID_NULL,
                HttpAtoStatusCreate,
                HttpAtoStatusRemove
            );

    /*
     * Feature objects are visible to the external modules, and can be manufactured and/or
     * destroyed by either external modules or the container. The Ato Header Object is such
     * an object.
     */
    
    pInternalObjectMapper->RegisterObject
            (
                (ANSC_HANDLE)pInternalObjectMapper,
                HTTP_ATO_HEADER_NAME,
                HTTP_ATO_HEADER_OID,
                ANSC_ATOM_TABLE_NAME,
                ANSC_ATOM_TABLE_OID,
                HTTP_ATO_HEADER_TYPE,
                ANSC_OBJECT_OID_NULL,
                HttpAtoHeaderCreate,
                HttpAtoHeaderRemove
            );

    /*
     * Feature objects are visible to the external modules, and can be manufactured and/or
     * destroyed by either external modules or the container. The Ato Method Object is such
     * an object.
     */
    
    pInternalObjectMapper->RegisterObject
            (
                (ANSC_HANDLE)pInternalObjectMapper,
                HTTP_ATO_METHOD_NAME,
                HTTP_ATO_METHOD_OID,
                ANSC_ATOM_TABLE_NAME,
                ANSC_ATOM_TABLE_OID,
                HTTP_ATO_METHOD_TYPE,
                ANSC_OBJECT_OID_NULL,
                HttpAtoMethodCreate,
                HttpAtoMethodRemove
            );

    /*
     * Feature objects are visible to the external modules, and can be manufactured and/or
     * destroyed by either external modules or the container. The Ato CgiEnv Object is such
     * an object.
     */
    
    pInternalObjectMapper->RegisterObject
            (
                (ANSC_HANDLE)pInternalObjectMapper,
                HTTP_ATO_CGIENV_NAME,
                HTTP_ATO_CGIENV_OID,
                ANSC_ATOM_TABLE_NAME,
                ANSC_ATOM_TABLE_OID,
                HTTP_ATO_CGIENV_TYPE,
                ANSC_OBJECT_OID_NULL,
                HttpAtoCgiEnvCreate,
                HttpAtoCgiEnvRemove
            );

    /*
     * Feature objects are visible to the external modules, and can be manufactured and/or
     * destroyed by either external modules or the container. The Ato FxCat1 Object is such
     * an object.
     */
    
    pInternalObjectMapper->RegisterObject
            (
                (ANSC_HANDLE)pInternalObjectMapper,
                HTTP_ATO_FXCAT1_NAME,
                HTTP_ATO_FXCAT1_OID,
                ANSC_ATOM_TABLE_NAME,
                ANSC_ATOM_TABLE_OID,
                HTTP_ATO_FXCAT1_TYPE,
                ANSC_OBJECT_OID_NULL,
                HttpAtoFxCat1Create,
                HttpAtoFxCat1Remove
            );

    /*
     * Feature objects are visible to the external modules, and can be manufactured and/or
     * destroyed by either external modules or the container. The Ato FxCat2 Object is such
     * an object.
     */
    
    pInternalObjectMapper->RegisterObject
            (
                (ANSC_HANDLE)pInternalObjectMapper,
                HTTP_ATO_FXCAT2_NAME,
                HTTP_ATO_FXCAT2_OID,
                ANSC_ATOM_TABLE_NAME,
                ANSC_ATOM_TABLE_OID,
                HTTP_ATO_FXCAT2_TYPE,
                ANSC_OBJECT_OID_NULL,
                HttpAtoFxCat2Create,
                HttpAtoFxCat2Remove
            );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpHcoManufactureFeatureObjects
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to manufacture all the internal objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpHcoManufactureFeatureObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_HELPER_CONTAINER_OBJECT   pMyObject    = (PHTTP_HELPER_CONTAINER_OBJECT)hThisObject;
    PHTTP_ATO_STATUS_OBJECT         pAtoStatus   = (PHTTP_ATO_STATUS_OBJECT      )pMyObject->hAtoStatus;
    PHTTP_ATO_HEADER_OBJECT         pAtoHeader   = (PHTTP_ATO_HEADER_OBJECT      )pMyObject->hAtoHeader;
    PHTTP_ATO_METHOD_OBJECT         pAtoMethod   = (PHTTP_ATO_METHOD_OBJECT      )pMyObject->hAtoMethod;
    PHTTP_ATO_CGIENV_OBJECT         pAtoCgiEnv   = (PHTTP_ATO_CGIENV_OBJECT      )pMyObject->hAtoCgiEnv;
    PHTTP_ATO_FXCAT1_OBJECT         pAtoFxCat1   = (PHTTP_ATO_FXCAT1_OBJECT      )pMyObject->hAtoFxCat1;
    PHTTP_ATO_FXCAT2_OBJECT         pAtoFxCat2   = (PHTTP_ATO_FXCAT2_OBJECT      )pMyObject->hAtoFxCat2;
    //PANSC_SMART_CLOCK_OBJECT        pSmartClock  = (PANSC_SMART_CLOCK_OBJECT     )pMyObject->hSmartClock;
    
    pAtoStatus =
        (PHTTP_ATO_STATUS_OBJECT)pMyObject->CreateObjectByOid
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pMyObject,
                HTTP_ATO_STATUS_OID
            );

    if ( !pAtoStatus )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pMyObject->hAtoStatus = (ANSC_HANDLE)pAtoStatus;
    }

    pAtoHeader =
        (PHTTP_ATO_HEADER_OBJECT)pMyObject->CreateObjectByOid
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pMyObject,
                HTTP_ATO_HEADER_OID
            );

    if ( !pAtoHeader )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pMyObject->hAtoHeader = (ANSC_HANDLE)pAtoHeader;
    }

    pAtoMethod =
        (PHTTP_ATO_METHOD_OBJECT)pMyObject->CreateObjectByOid
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pMyObject,
                HTTP_ATO_METHOD_OID
            );

    if ( !pAtoMethod )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pMyObject->hAtoMethod = (ANSC_HANDLE)pAtoMethod;
    }

    pAtoCgiEnv =
        (PHTTP_ATO_CGIENV_OBJECT)pMyObject->CreateObjectByOid
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pMyObject,
                HTTP_ATO_CGIENV_OID
            );

    if ( !pAtoCgiEnv )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pMyObject->hAtoCgiEnv = (ANSC_HANDLE)pAtoCgiEnv;
    }

    pAtoFxCat1 =
        (PHTTP_ATO_FXCAT1_OBJECT)pMyObject->CreateObjectByOid
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pMyObject,
                HTTP_ATO_FXCAT1_OID
            );

    if ( !pAtoFxCat1 )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pMyObject->hAtoFxCat1 = (ANSC_HANDLE)pAtoFxCat1;
    }

    pAtoFxCat2 =
        (PHTTP_ATO_FXCAT2_OBJECT)pMyObject->CreateObjectByOid
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pMyObject,
                HTTP_ATO_FXCAT2_OID
            );

    if ( !pAtoFxCat2 )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pMyObject->hAtoFxCat2 = (ANSC_HANDLE)pAtoFxCat2;
    }

    // Assume no smart clock support.
    /*
    pSmartClock =
        (PANSC_SMART_CLOCK_OBJECT)AnscCreateSmartClock
            (
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)pMyObject,
                (ANSC_HANDLE)NULL
            );

    if ( !pSmartClock )
    {
        return  ANSC_STATUS_RESOURCES;
    }
    else
    {
        pMyObject->hSmartClock = (ANSC_HANDLE)pSmartClock;

        pSmartClock->Start((ANSC_HANDLE)pSmartClock);
    }*/

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        HttpHcoDestroyFeatureObjects
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to destroy all the internal objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
HttpHcoDestroyFeatureObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PHTTP_HELPER_CONTAINER_OBJECT   pMyObject    = (PHTTP_HELPER_CONTAINER_OBJECT)hThisObject;
    PHTTP_ATO_STATUS_OBJECT         pAtoStatus   = (PHTTP_ATO_STATUS_OBJECT      )pMyObject->hAtoStatus;
    PHTTP_ATO_HEADER_OBJECT         pAtoHeader   = (PHTTP_ATO_HEADER_OBJECT      )pMyObject->hAtoHeader;
    PHTTP_ATO_METHOD_OBJECT         pAtoMethod   = (PHTTP_ATO_METHOD_OBJECT      )pMyObject->hAtoMethod;
    PHTTP_ATO_CGIENV_OBJECT         pAtoCgiEnv   = (PHTTP_ATO_CGIENV_OBJECT      )pMyObject->hAtoCgiEnv;
    PHTTP_ATO_FXCAT1_OBJECT         pAtoFxCat1   = (PHTTP_ATO_FXCAT1_OBJECT      )pMyObject->hAtoFxCat1;
    PHTTP_ATO_FXCAT2_OBJECT         pAtoFxCat2   = (PHTTP_ATO_FXCAT2_OBJECT      )pMyObject->hAtoFxCat2;
    PANSC_SMART_CLOCK_OBJECT        pSmartClock  = (PANSC_SMART_CLOCK_OBJECT     )pMyObject->hSmartClock;

    if ( pAtoStatus )
    {
        pAtoStatus->Remove((ANSC_HANDLE)pAtoStatus);

        pMyObject->hAtoStatus = (ANSC_HANDLE)NULL;
    }

    if ( pAtoHeader )
    {
        pAtoHeader->Remove((ANSC_HANDLE)pAtoHeader);

        pMyObject->hAtoHeader = (ANSC_HANDLE)NULL;
    }

    if ( pAtoMethod )
    {
        pAtoMethod->Remove((ANSC_HANDLE)pAtoMethod);

        pMyObject->hAtoMethod = (ANSC_HANDLE)NULL;
    }

    if ( pAtoCgiEnv )
    {
        pAtoCgiEnv->Remove((ANSC_HANDLE)pAtoCgiEnv);

        pMyObject->hAtoCgiEnv = (ANSC_HANDLE)NULL;
    }

    if ( pAtoFxCat1 )
    {
        pAtoFxCat1->Remove((ANSC_HANDLE)pAtoFxCat1);

        pMyObject->hAtoFxCat1 = (ANSC_HANDLE)NULL;
    }

    if ( pAtoFxCat2 )
    {
        pAtoFxCat2->Remove((ANSC_HANDLE)pAtoFxCat2);

        pMyObject->hAtoFxCat2 = (ANSC_HANDLE)NULL;
    }

    if ( pSmartClock )
    {
        pSmartClock->Stop  ((ANSC_HANDLE)pSmartClock);
        pSmartClock->Remove((ANSC_HANDLE)pSmartClock);

        pMyObject->hSmartClock = (ANSC_HANDLE)NULL;
    }

    return  ANSC_STATUS_SUCCESS;
}
