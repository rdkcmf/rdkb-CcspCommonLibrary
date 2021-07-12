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

    module:	bmc2_comto_states.c

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced state-access functions
        of the Bmc2 Com Terminal Object.

        *   Bmc2ComtoGetBmc2IceIf
        *   Bmc2ComtoSetBmc2IceIf
        *   Bmc2ComtoGetBmc2EnvController
        *   Bmc2ComtoSetBmc2EnvController
        *   Bmc2ComtoGetUserIdentifier
        *   Bmc2ComtoSetUserIdentifier
        *   Bmc2ComtoGetUserPermission
        *   Bmc2ComtoSetUserPermission
        *   Bmc2ComtoGetMaxLineNumber
        *   Bmc2ComtoSetMaxLineNumber
        *   Bmc2ComtoGetMaxColumnNumber
        *   Bmc2ComtoSetMaxColumnNumber
        *   Bmc2ComtoGetReadOnly
        *   Bmc2ComtoSetReadOnly
        *   Bmc2ComtoGetProperty
        *   Bmc2ComtoSetProperty
        *   Bmc2ComtoResetProperty
        *   Bmc2ComtoReset

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
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        Bmc2ComtoGetBmc2IceIf
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
Bmc2ComtoGetBmc2IceIf
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_COM_TERMINAL_OBJECT       pMyObject    = (PBMC2_COM_TERMINAL_OBJECT  )hThisObject;

    return  pMyObject->hBmc2IceIf;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ComtoSetBmc2IceIf
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hInterface
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hInterface
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ComtoSetBmc2IceIf
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hInterface
    )
{
    PBMC2_COM_TERMINAL_OBJECT       pMyObject    = (PBMC2_COM_TERMINAL_OBJECT  )hThisObject;

    pMyObject->hBmc2IceIf = hInterface;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        Bmc2ComtoGetBmc2EnvController
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ANSC_HANDLE
Bmc2ComtoGetBmc2EnvController
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_COM_TERMINAL_OBJECT       pMyObject    = (PBMC2_COM_TERMINAL_OBJECT  )hThisObject;

    return  pMyObject->hBmc2EnvController;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ComtoSetBmc2EnvController
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hEnvController
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hEnvController
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ComtoSetBmc2EnvController
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hEnvController
    )
{
    PBMC2_COM_TERMINAL_OBJECT       pMyObject    = (PBMC2_COM_TERMINAL_OBJECT  )hThisObject;

    pMyObject->hBmc2EnvController = hEnvController;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char*
        Bmc2ComtoGetUserIdentifier
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

char*
Bmc2ComtoGetUserIdentifier
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_COM_TERMINAL_OBJECT       pMyObject    = (PBMC2_COM_TERMINAL_OBJECT  )hThisObject;
    PBMC2_COM_TERMINAL_PROPERTY     pProperty    = (PBMC2_COM_TERMINAL_PROPERTY)&pMyObject->Property;

    return  pProperty->UserIdentifier;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ComtoSetUserIdentifier
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pUserIdentifier
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pUserIdentifier
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ComtoSetUserIdentifier
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pUserIdentifier
    )
{
    PBMC2_COM_TERMINAL_OBJECT       pMyObject    = (PBMC2_COM_TERMINAL_OBJECT  )hThisObject;
    PBMC2_COM_TERMINAL_PROPERTY     pProperty    = (PBMC2_COM_TERMINAL_PROPERTY)&pMyObject->Property;
    errno_t    rc   = -1;

    AnscZeroMemory(pProperty->UserIdentifier, BMC2_MAX_USER_ID_SIZE);

    if ( pUserIdentifier )
    {
        rc = STRCPY_S_NOCLOBBER(pProperty->UserIdentifier, sizeof(pProperty->UserIdentifier), pUserIdentifier);
        ERR_CHK(rc);
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ComtoSetPseudoTermDevName
            (
                ANSC_HANDLE                 hThisObject,
                char*                       pPseudoTermDevName
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char*                       pSeudoTermDevName
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ComtoSetPseudoTermDevName
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pPseudoTermDevName
    )
{
    PBMC2_COM_TERMINAL_OBJECT       pMyObject    = (PBMC2_COM_TERMINAL_OBJECT  )hThisObject;

    if ( pMyObject->pPseudoTermDevName )
    {
        AnscFreeMemory(pMyObject->pPseudoTermDevName);
    }

    pMyObject->pPseudoTermDevName = AnscCloneString(pPseudoTermDevName);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        Bmc2ComtoGetUserPermission
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ULONG
Bmc2ComtoGetUserPermission
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_COM_TERMINAL_OBJECT       pMyObject    = (PBMC2_COM_TERMINAL_OBJECT  )hThisObject;
    PBMC2_COM_TERMINAL_PROPERTY     pProperty    = (PBMC2_COM_TERMINAL_PROPERTY)&pMyObject->Property;

    return  pProperty->UserPermission;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ComtoSetUserPermission
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulPermission
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulPermission
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ComtoSetUserPermission
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulPermission
    )
{
    PBMC2_COM_TERMINAL_OBJECT       pMyObject    = (PBMC2_COM_TERMINAL_OBJECT  )hThisObject;
    PBMC2_COM_TERMINAL_PROPERTY     pProperty    = (PBMC2_COM_TERMINAL_PROPERTY)&pMyObject->Property;

    pProperty->UserPermission = ulPermission;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        Bmc2ComtoGetMaxLineNumber
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ULONG
Bmc2ComtoGetMaxLineNumber
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_COM_TERMINAL_OBJECT       pMyObject    = (PBMC2_COM_TERMINAL_OBJECT  )hThisObject;
    PBMC2_COM_TERMINAL_PROPERTY     pProperty    = (PBMC2_COM_TERMINAL_PROPERTY)&pMyObject->Property;

    return  pProperty->MaxLineNumber;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ComtoSetMaxLineNumber
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulLineNumber
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulLineNumber
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ComtoSetMaxLineNumber
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulLineNumber
    )
{
    PBMC2_COM_TERMINAL_OBJECT       pMyObject    = (PBMC2_COM_TERMINAL_OBJECT  )hThisObject;
    PBMC2_COM_TERMINAL_PROPERTY     pProperty    = (PBMC2_COM_TERMINAL_PROPERTY)&pMyObject->Property;

    pProperty->MaxLineNumber = ulLineNumber;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        Bmc2ComtoGetMaxColumnNumber
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

ULONG
Bmc2ComtoGetMaxColumnNumber
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_COM_TERMINAL_OBJECT       pMyObject    = (PBMC2_COM_TERMINAL_OBJECT  )hThisObject;
    PBMC2_COM_TERMINAL_PROPERTY     pProperty    = (PBMC2_COM_TERMINAL_PROPERTY)&pMyObject->Property;

    return  pProperty->MaxColumnNumber;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ComtoSetMaxColumnNumber
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulColumnNumber
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ULONG                       ulColumnNumber
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ComtoSetMaxColumnNumber
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulColumnNumber
    )
{
    PBMC2_COM_TERMINAL_OBJECT       pMyObject    = (PBMC2_COM_TERMINAL_OBJECT  )hThisObject;
    PBMC2_COM_TERMINAL_PROPERTY     pProperty    = (PBMC2_COM_TERMINAL_PROPERTY)&pMyObject->Property;

    pProperty->MaxColumnNumber = ulColumnNumber;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        Bmc2ComtoGetReadOnly
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to retrieve object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     object state.

**********************************************************************/

BOOL
Bmc2ComtoGetReadOnly
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_COM_TERMINAL_OBJECT       pMyObject    = (PBMC2_COM_TERMINAL_OBJECT  )hThisObject;
    PBMC2_COM_TERMINAL_PROPERTY     pProperty    = (PBMC2_COM_TERMINAL_PROPERTY)&pMyObject->Property;

    return  pProperty->bReadOnly;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ComtoSetReadOnly
            (
                ANSC_HANDLE                 hThisObject,
                BOOL                        bEnabled
            );

    description:

        This function is called to configure object state.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                BOOL                        bEnabled
                Specifies the object state to be configured.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ComtoSetReadOnly
    (
        ANSC_HANDLE                 hThisObject,
        BOOL                        bEnabled
    )
{
    PBMC2_COM_TERMINAL_OBJECT       pMyObject    = (PBMC2_COM_TERMINAL_OBJECT  )hThisObject;
    PBMC2_COM_TERMINAL_PROPERTY     pProperty    = (PBMC2_COM_TERMINAL_PROPERTY)&pMyObject->Property;

    pProperty->bReadOnly = bEnabled;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ComtoGetProperty
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hProperty
            );

    description:

        This function is called to retrieve object property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hProperty
                Specifies the property data structure to be filled.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ComtoGetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PBMC2_COM_TERMINAL_OBJECT       pMyObject    = (PBMC2_COM_TERMINAL_OBJECT  )hThisObject;
    PBMC2_COM_TERMINAL_PROPERTY     pProperty    = (PBMC2_COM_TERMINAL_PROPERTY)&pMyObject->Property;

    *(PBMC2_COM_TERMINAL_PROPERTY)hProperty = *pProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ComtoSetProperty
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hProperty
            );

    description:

        This function is called to configure object property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hProperty
                Specifies the property data structure to be copied.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ComtoSetProperty
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hProperty
    )
{
    PBMC2_COM_TERMINAL_OBJECT       pMyObject    = (PBMC2_COM_TERMINAL_OBJECT  )hThisObject;
    PBMC2_COM_TERMINAL_PROPERTY     pProperty    = (PBMC2_COM_TERMINAL_PROPERTY)&pMyObject->Property;

    *pProperty = *(PBMC2_COM_TERMINAL_PROPERTY)hProperty;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ComtoResetProperty
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to reset object property.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ComtoResetProperty
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_COM_TERMINAL_OBJECT       pMyObject    = (PBMC2_COM_TERMINAL_OBJECT  )hThisObject;
    PBMC2_COM_TERMINAL_PROPERTY     pProperty    = (PBMC2_COM_TERMINAL_PROPERTY)&pMyObject->Property;

    AnscZeroMemory(pProperty, sizeof(BMC2_COM_TERMINAL_PROPERTY));

    pProperty->UserPermission  = 0;
    pProperty->MaxLineNumber   = 45;
    pProperty->MaxColumnNumber = 80;
    pProperty->bReadOnly       = FALSE;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        Bmc2ComtoReset
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function is called to reset object states.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
Bmc2ComtoReset
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBMC2_COM_TERMINAL_OBJECT       pMyObject    = (PBMC2_COM_TERMINAL_OBJECT  )hThisObject;

    pMyObject->DelAllComDomains((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}
