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

    module:	bspeng_reader_co_base.c

        For BSP Engine Reader Component Object,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic component object functions
        of the string Component Object.

        *   BspTemplateReaderCOCreate
        *   BspTemplateReaderCORemove

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        05/14/03    initial revision.

**********************************************************************/


#include "bspeng_co_global.h"


/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        BspTemplateReaderCOCreate
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
BspTemplateReaderCOCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hContainerContext);
    UNREFERENCED_PARAMETER(hOwnerContext);
    UNREFERENCED_PARAMETER(hAnscReserved);
    PBSP_TEMPLATE_READER_OBJECT     pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PBSP_TEMPLATE_READER_OBJECT)AnscAllocateMemory(sizeof(BSP_TEMPLATE_READER_OBJECT));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    pMyObject->Read                         = BspTemplateReaderRead;
    pMyObject->Close                        = BspTemplateReaderClose;
    pMyObject->OpenExternal                 = BspTemplateReaderOpenExternal;
    pMyObject->OpenInternal                 = BspTemplateReaderOpenInternal;
    pMyObject->GetTemplateName              = BspTemplateReaderGetTemplateName;
    pMyObject->SetExtPath                   = BspTemplateReaderSetExtPath;
    pMyObject->Initialize                   = BspTemplateReaderCOInitialize;
    pMyObject->Remove                       = BspTemplateReaderCORemove;
    
    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BspTemplateReaderCORemove
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
BspTemplateReaderCORemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_READER_OBJECT     pMyObject    = (PBSP_TEMPLATE_READER_OBJECT)hThisObject;

    AnscFreeMemory(pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


ANSC_STATUS
BspTemplateReaderCOInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_READER_OBJECT     pMyObject    = (PBSP_TEMPLATE_READER_OBJECT)hThisObject;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    if ( !pMyObject )
    {
        return ANSC_STATUS_FAILURE;
    }

    pMyObject->Read                         = BspTemplateReaderRead;
    pMyObject->Close                        = BspTemplateReaderClose;
    pMyObject->OpenExternal                 = BspTemplateReaderOpenExternal;
    pMyObject->OpenInternal                 = BspTemplateReaderOpenInternal;
    pMyObject->GetTemplateName              = BspTemplateReaderGetTemplateName;
    pMyObject->SetExtPath                   = BspTemplateReaderSetExtPath;

    pMyObject->Remove                       = BspTemplateReaderCORemove;
    pMyObject->Initialize                   = BspTemplateReaderCOInitialize;

    return  ANSC_STATUS_SUCCESS;
}


