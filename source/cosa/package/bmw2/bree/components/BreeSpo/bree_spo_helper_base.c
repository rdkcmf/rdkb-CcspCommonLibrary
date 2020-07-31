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

    module:	bree_spo_helper.c

        For BSP Run-time Execution Engine (BREE),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the BSP Run-time Execution Engine (BREE).

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        07/22/03    initial revision.

**********************************************************************/

#include "bree_spo_global.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        BreeSpoReaderCOCreate
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
BreeSpoReaderCOCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hContainerContext);
    UNREFERENCED_PARAMETER(hOwnerContext);
    UNREFERENCED_PARAMETER(hAnscReserved);
    PBSP_TEMPLATE_READER_OBJECT     pBaseObject  = NULL;
    PBREESPO_READER_OBJECT          pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PBREESPO_READER_OBJECT)AnscAllocateMemory(sizeof(BREESPO_READER_OBJECT));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pBaseObject = (PBSP_TEMPLATE_READER_OBJECT)pMyObject;
    }

    pBaseObject->Remove            = BreeSpoReaderCORemove;
    pBaseObject->Initialize        = BreeSpoReaderCOInitialize;

    pBaseObject->Initialize   ((ANSC_HANDLE)pBaseObject);

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BreeSpoReaderCORemove
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
BreeSpoReaderCORemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBREESPO_READER_OBJECT          pMyObject    = (PBREESPO_READER_OBJECT)hThisObject;

    pMyObject->Close((ANSC_HANDLE)pMyObject);

    BspTemplateReaderCORemove((ANSC_HANDLE)pMyObject  );

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BreeSpoReaderCOInitialize
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
BreeSpoReaderCOInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBREESPO_READER_OBJECT             pMyObject     = (PBREESPO_READER_OBJECT)hThisObject;

    /*
     * Until you have to simulate C++ object-oriented programming style with standard C, you don't
     * appreciate all the nice little things come with C++ language and all the dirty works that
     * have been done by the C++ compilers. Member initialization is one of these things. While in
     * C++ you don't have to initialize all the member fields inherited from the base class since
     * the compiler will do it for you, such is not the case with C.
     */
    BspTemplateReaderCOInitialize((ANSC_HANDLE)pMyObject);

    /*
     * Although we have initialized some of the member fields in the "create" member function, we
     * repeat the work here for completeness. While this simulation approach is pretty stupid from
     * a C++/Java programmer perspective, it's the best we can get for universal embedded network
     * programming. Before we develop our own operating system (don't expect that to happen any
     * time soon), this is the way things gonna be.
     */
    pMyObject->Remove                       = BreeSpoReaderCORemove;
    pMyObject->Initialize                   = BreeSpoReaderCOInitialize;

    pMyObject->Read                         = BreeSpoReaderRead;
    pMyObject->Valid                        = BreeSpoReaderValid;

    pMyObject->Close                        = BreeSpoReaderClose;
    pMyObject->OpenExternal                 = BreeSpoReaderOpenExternal;
    pMyObject->OpenInternal                 = BreeSpoReaderOpenInternal;
    pMyObject->GetTemplateName              = BreeSpoReaderGetTemplateName;

    return  ANSC_STATUS_SUCCESS;
}

