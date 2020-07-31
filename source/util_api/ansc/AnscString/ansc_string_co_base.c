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

    module:	ansc_string_co_base.c

        For string,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic component object functions
        of the string Component Object.

        *   AnscStringCOCreate
        *   AnscStringCORemove
        *   AnscStringCOEnrollObjects
        *   AnscStringCOInitialize

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        03/13/02    initial revision.

**********************************************************************/


#include "ansc_string_co_global.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscStringCOCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ANSC_HANDLE                 hOwnerContext,
                ANSC_HANDLE                 hAnscReserved
            );

    description:

        This function constructs the String Component object and
        initializes the member variables and functions.

    argument:   ANSC_HANDLE                 hContainerContext
                This handle is used by the container object to interact
                with the outside world. It could be the real container
                or an target object.

                ANSC_HANDLE                 hOwnerContext
                This handle is passed in by the owner of this object.

                ANSC_HANDLE                 hAnscReserved
                This handle is passed in by the owner of this object.

    return:     newly created string component object.

**********************************************************************/

ANSC_HANDLE
AnscStringCOCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hAnscReserved);
    PANSC_COMPONENT_OBJECT          pBaseObject  = NULL;
    PANSC_STRING_OBJECT             pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PANSC_STRING_OBJECT)AnscAllocateMemory(sizeof(ANSC_STRING_OBJECT));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pBaseObject = (PANSC_COMPONENT_OBJECT)pMyObject;
    }

    /*
     * Initialize the common variables and functions for a component object.
     */
    AnscCopyString(pBaseObject->Name, ANSC_STRING_COMPONENT_NAME);

    pBaseObject->hContainerContext = hContainerContext;
    pBaseObject->hOwnerContext     = hOwnerContext;
    pBaseObject->Oid               = ANSC_STRING_COMPONENT_OID;
    pBaseObject->Create            = AnscStringCOCreate;
    pBaseObject->Remove            = AnscStringCORemove;
    pBaseObject->EnrollObjects     = AnscStringCOEnrollObjects;
    pBaseObject->Initialize        = AnscStringCOInitialize;

    pBaseObject->EnrollObjects((ANSC_HANDLE)pBaseObject);
    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscStringCORemove
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
AnscStringCORemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_STRING_OBJECT     pMyObject    = (PANSC_STRING_OBJECT)hThisObject;

    /* Destroy value buffer no matter if the value length is ZERO */
    if (pMyObject->pValue)
    {
        AnscFreeMemory(pMyObject->pValue);
    }

    AnscCoRemove((ANSC_HANDLE)pMyObject  );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscStringCOEnrollObjects
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
AnscStringCOEnrollObjects
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_STRING_OBJECT             pMyObject    = (PANSC_STRING_OBJECT)hThisObject;

    AnscCoEnrollObjects((ANSC_HANDLE)pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscStringCOInitialize
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
AnscStringCOInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_STRING_OBJECT             pMyObject     = (PANSC_STRING_OBJECT)hThisObject;

    /*
     * Until you have to simulate C++ object-oriented programming style with standard C, you don't
     * appreciate all the nice little things come with C++ language and all the dirty works that
     * have been done by the C++ compilers. Member initialization is one of these things. While in
     * C++ you don't have to initialize all the member fields inherited from the base class since
     * the compiler will do it for you, such is not the case with C.
     */
    AnscCoInitialize((ANSC_HANDLE)pMyObject);

    /*
     * Although we have initialized some of the member fields in the "create" member function, we
     * repeat the work here for completeness. While this simulation approach is pretty stupid from
     * a C++/Java programmer perspective, it's the best we can get for universal embedded network
     * programming. Before we develop our own operating system (don't expect that to happen any
     * time soon), this is the way things gonna be.
     */
    pMyObject->Oid                          = ANSC_STRING_COMPONENT_OID;
    pMyObject->Create                       = AnscStringCOCreate;
    pMyObject->Remove                       = AnscStringCORemove;
    pMyObject->EnrollObjects                = AnscStringCOEnrollObjects;
    pMyObject->Initialize                   = AnscStringCOInitialize;

    pMyObject->SetString                    = AnscStringCOSetString;
    pMyObject->SetBuffer                    = AnscStringCOSetBuffer;
    pMyObject->Copy                         = AnscStringCOCopy;
    pMyObject->GetLength                    = AnscStringCOGetLength;
    pMyObject->IsEmpty                      = AnscStringCOIsEmpty;
    pMyObject->Empty                        = AnscStringCOEmpty;
    pMyObject->GetAt                        = AnscStringCOGetAt;
    pMyObject->SetAt                        = AnscStringCOSetAt;
    pMyObject->GetString                    = AnscStringCOGetString;
    pMyObject->Equal                        = AnscStringCOEqual;
    pMyObject->EqualNoCase                  = AnscStringCOEqualNoCase;
    pMyObject->Left                         = AnscStringCOLeft;
    pMyObject->Right                        = AnscStringCORight;
    pMyObject->FindChar                     = AnscStringCOFindChar;
    pMyObject->FindString                   = AnscStringCOFindString;
    pMyObject->Upper                        = AnscStringCOUpper;
    pMyObject->Lower                        = AnscStringCOLower;
    pMyObject->Resize                       = AnscStringCOResize;
    pMyObject->ReplaceChar                  = AnscStringCOReplaceChar;
    pMyObject->ReplaceString                = AnscStringCOReplaceString;
    pMyObject->AppendBuffer                 = AnscStringCOAppendBuffer;
    pMyObject->AppendString                 = AnscStringCOAppendString;
    pMyObject->Concat                       = AnscStringCOConcat;
    pMyObject->ExtractBuffer                = AnscStringCOExtractBuffer;
    pMyObject->AppendStringN                = AnscStringCOAppendStringN;

    return  ANSC_STATUS_SUCCESS;
}
