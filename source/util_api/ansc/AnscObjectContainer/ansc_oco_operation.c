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

    module:	ansc_oco_operation.c

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2001
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the Ansc Object Container Object.

        *   AnscOcoEngage
        *   AnscOcoCancel
        *   AnscOcoSubEngage
        *   AnscOcoSubCancel
        *   AnscOcoEnrollAllObjects
        *   AnscOcoManufactureAllObjects
        *   AnscOcoDestroyAllObjects

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        10/15/01    initial revision.

**********************************************************************/


#include "ansc_oco_global.h"


#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  warning(disable: 4101)                     /* unreferenced local variable */
#pragma  warning(disable: 4102)                     /* unreferenced lable */
#endif


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscOcoEngage
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to engage the container operations.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscOcoEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus          = ANSC_STATUS_SUCCESS;
    PANSC_OBJECT_CONTAINER_OBJECT   pMyObject             = (PANSC_OBJECT_CONTAINER_OBJECT)hThisObject;
    PANSC_OBJECT_CONTAINER_OBJECT   pParentContainer      = (PANSC_OBJECT_CONTAINER_OBJECT)pMyObject->hContainerContext;
    PANSC_OBJECT_MAPPER_OBJECT      pExternalObjectMapper = NULL;
    PANSC_OBJECT_MAPPER_OBJECT      pInternalObjectMapper = NULL;
    PANSC_TIMER_SCHEDULER_OBJECT    pTimerScheduler       = NULL;

    /*
     * To better manage all the objects in this environment, we create a object mapper object to
     * do the job for us. The object mapper object takes over the general object management,
     * object creation and removal from the container and unifies the implementations of different
     * containers.
     */
    if ( pMyObject->OpMask & ANSC_OCO_OPMASK_OMO_EXTERNAL )
    {
        pExternalObjectMapper = (PANSC_OBJECT_MAPPER_OBJECT)AnscCreateObjectMapper((ANSC_HANDLE)pMyObject);

        if ( !pExternalObjectMapper )
        {
            returnStatus = ANSC_STATUS_RESOURCES;

            goto  EXIT1;
        }
        else
        {
            pExternalObjectMapper->SetContainerContext
                (
                    (ANSC_HANDLE)pExternalObjectMapper,
                    (ANSC_HANDLE)pMyObject
                );
        }

        pMyObject->hExternalObjectMapper = (ANSC_HANDLE)pExternalObjectMapper;
    }

    /*
     * To better manage all the objects in this environment, we create a object mapper object to
     * do the job for us. The object mapper object takes over the general object management,
     * object creation and removal from the container and unifies the implementations of different
     * containers.
     */
    if ( pMyObject->OpMask & ANSC_OCO_OPMASK_OMO_INTERNAL )
    {
        pInternalObjectMapper = (PANSC_OBJECT_MAPPER_OBJECT)AnscCreateObjectMapper((ANSC_HANDLE)pMyObject);

        if ( !pInternalObjectMapper )
        {
            returnStatus = ANSC_STATUS_RESOURCES;

            goto  EXIT2;
        }
        else
        {
            pInternalObjectMapper->SetContainerContext
                (
                    (ANSC_HANDLE)pInternalObjectMapper,
                    (ANSC_HANDLE)pMyObject
                );
        }

        pMyObject->hInternalObjectMapper = (ANSC_HANDLE)pInternalObjectMapper;
    }

    /*
     * To better manage all the timers in this environment, we create a timer scheduler object to
     * do the job for us. The timer scheduler object takes over the general timer object management,
     * object creation and removal from the container and unifies the implementations of different
     * containers.
     */
    if (  ((pMyObject->OpMask & ANSC_OCO_OPMASK_TSO     )                    ) &&
         !((pMyObject->OpMask & ANSC_OCO_OPMASK_COLLAPSE) && pParentContainer) )
    {
        pTimerScheduler =
            (PANSC_TIMER_SCHEDULER_OBJECT)AnscCreateTimerScheduler
                (
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)pMyObject,
                    (ANSC_HANDLE)NULL
                );

        if ( !pTimerScheduler )
        {
            returnStatus = ANSC_STATUS_RESOURCES;

            goto  EXIT3;
        }
        else
        {
            pMyObject->hTimerScheduler = (ANSC_HANDLE)pTimerScheduler;
        }
    }

    /*
     * We have to give the derived Container Objects a chance to engage some advanced components
     * before creating all the feature and extension objects.
     */
    returnStatus = pMyObject->SubEngage((ANSC_HANDLE)pMyObject);

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        goto  EXIT4;
    }

    /*
     * Manufacture all the predefined container objects, namingly all the controller objects. One
     * of the painful experience for embedded software development (or kernel software development
     * in general) is that most time we have no luxury of free object naming, dynamic loading, and
     * binary-level component reuse.
     */
    returnStatus = pMyObject->EnrollAllObjects     ((ANSC_HANDLE)pMyObject);
    returnStatus = pMyObject->ManufactureAllObjects((ANSC_HANDLE)pMyObject);

    if ( returnStatus != ANSC_STATUS_SUCCESS )
    {
        goto  EXIT5;
    }
    else
    {
        pMyObject->bEngaged = TRUE;

        goto  EXIT1;
    }


    /******************************************************************
                GRACEFUL ROLLBACK PROCEDURES AND EXIT DOORS
    ******************************************************************/

EXIT5:

    pMyObject->DestroyAllObjects((ANSC_HANDLE)pMyObject);

EXIT4:

    if ( pTimerScheduler )
    {
        pTimerScheduler->Remove((ANSC_HANDLE)pTimerScheduler);

        pMyObject->hTimerScheduler = (ANSC_HANDLE)NULL;
    }

EXIT3:

    if ( pInternalObjectMapper )
    {
        pInternalObjectMapper->Remove((ANSC_HANDLE)pInternalObjectMapper);

        pMyObject->hInternalObjectMapper = (ANSC_HANDLE)NULL;
    }

EXIT2:

    if ( pExternalObjectMapper )
    {
        pExternalObjectMapper->Remove((ANSC_HANDLE)pExternalObjectMapper);

        pMyObject->hExternalObjectMapper = (ANSC_HANDLE)NULL;
    }

EXIT1:

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscOcoCancel
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to cancel the container operations.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscOcoCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus          = ANSC_STATUS_SUCCESS;
    PANSC_OBJECT_CONTAINER_OBJECT   pMyObject             = (PANSC_OBJECT_CONTAINER_OBJECT)hThisObject;
    PANSC_OBJECT_MAPPER_OBJECT      pExternalObjectMapper = (PANSC_OBJECT_MAPPER_OBJECT   )pMyObject->hExternalObjectMapper;
    PANSC_OBJECT_MAPPER_OBJECT      pInternalObjectMapper = (PANSC_OBJECT_MAPPER_OBJECT   )pMyObject->hInternalObjectMapper;
    PANSC_TIMER_SCHEDULER_OBJECT    pTimerScheduler       = (PANSC_TIMER_SCHEDULER_OBJECT )pMyObject->hTimerScheduler;

    /*
     * Stop all the async tasks and wait for a while before go on.
     */
    pMyObject->bEngaged = FALSE;

    /*
     * Destory the all the container objects we have created during initialization and free the
     * memory block we have allocated for ourselves. Each container object will in turn clean its
     * internal states and allocate memory buffers.
     */
    pMyObject->DestroyAllObjects((ANSC_HANDLE)pMyObject);

    /*
     * We have to give the derived Container Objects a chance to cancel some advanced components
     * before destroying all the basic operational objects.
     */
    returnStatus = pMyObject->SubCancel((ANSC_HANDLE)pMyObject);

    /*
     * The object mapper object has gone through his wholly useful life, now it's time for him to
     * take a long break.
     */
    if ( pExternalObjectMapper )
    {
        pExternalObjectMapper->Remove((ANSC_HANDLE)pExternalObjectMapper);

        pMyObject->hExternalObjectMapper = (ANSC_HANDLE)NULL;
    }

    if ( pInternalObjectMapper )
    {
        pInternalObjectMapper->Remove((ANSC_HANDLE)pInternalObjectMapper);

        pMyObject->hInternalObjectMapper = (ANSC_HANDLE)NULL;
    }

    /*
     * The timer scheduler object has gone through his wholly useful life, now it's time for him to
     * take a long break.
     */
    if ( pTimerScheduler )
    {
        pTimerScheduler->Remove((ANSC_HANDLE)pTimerScheduler);

        pMyObject->hTimerScheduler = (ANSC_HANDLE)NULL;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscOcoSubEngage
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to engage the container operations.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscOcoSubEngage
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PANSC_OBJECT_CONTAINER_OBJECT   pMyObject        = (PANSC_OBJECT_CONTAINER_OBJECT)hThisObject;
    PANSC_OBJECT_CONTAINER_OBJECT   pParentContainer = (PANSC_OBJECT_CONTAINER_OBJECT)pMyObject->hContainerContext;

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscOcoSubCancel
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to engage the container operations.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscOcoSubCancel
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus     = ANSC_STATUS_SUCCESS;
    PANSC_OBJECT_CONTAINER_OBJECT   pMyObject        = (PANSC_OBJECT_CONTAINER_OBJECT)hThisObject;
    PANSC_OBJECT_CONTAINER_OBJECT   pParentContainer = (PANSC_OBJECT_CONTAINER_OBJECT)pMyObject->hContainerContext;

    return  returnStatus;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscOcoEnrollAllObjects
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
AnscOcoEnrollAllObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_OBJECT_CONTAINER_OBJECT   pMyObject    = (PANSC_OBJECT_CONTAINER_OBJECT)hThisObject;

    pMyObject->EnrollExtensionObjects((ANSC_HANDLE)pMyObject);
    pMyObject->EnrollFeatureObjects  ((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscOcoManufactureAllObjects
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
AnscOcoManufactureAllObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_OBJECT_CONTAINER_OBJECT   pMyObject    = (PANSC_OBJECT_CONTAINER_OBJECT)hThisObject;

    pMyObject->ManufactureExtensionObjects((ANSC_HANDLE)pMyObject);
    pMyObject->ManufactureFeatureObjects  ((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscOcoDestroyAllObjects
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
AnscOcoDestroyAllObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_OBJECT_CONTAINER_OBJECT   pMyObject    = (PANSC_OBJECT_CONTAINER_OBJECT)hThisObject;

    pMyObject->DestroyExtensionObjects((ANSC_HANDLE)pMyObject);
    pMyObject->DestroyFeatureObjects  ((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
