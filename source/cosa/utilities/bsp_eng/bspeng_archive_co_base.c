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

    module:	bspeng_archive_co_base.c

        For BSP Engine Reader Component Object,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the basic component object functions
        of the string Component Object.

        *   BspTemplateArchiveCOCreate
        *   BspTemplateArchiveCORemove

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
        BspTemplateArchiveCOCreate
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
BspTemplateArchiveCOCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    )
{
    UNREFERENCED_PARAMETER(hContainerContext);
    UNREFERENCED_PARAMETER(hOwnerContext);
    UNREFERENCED_PARAMETER(hAnscReserved);
    PBSP_TEMPLATE_ARCHIVE_OBJECT    pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    pMyObject = (PBSP_TEMPLATE_ARCHIVE_OBJECT)AnscAllocateMemory(sizeof(BSP_TEMPLATE_ARCHIVE_OBJECT));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    pMyObject->WriteInteger                 = BspTemplateArchiveWriteInteger;
    pMyObject->WriteUlong                   = BspTemplateArchiveWriteUlong;
    pMyObject->WriteDouble                  = BspTemplateArchiveWriteDouble;
    pMyObject->WriteWord                    = BspTemplateArchiveWriteWord;
    pMyObject->WriteBoolean                 = BspTemplateArchiveWriteBoolean;
    pMyObject->WriteByte                    = BspTemplateArchiveWriteByte;
    pMyObject->WriteString                  = BspTemplateArchiveWriteString;
    pMyObject->WriteBinary                  = BspTemplateArchiveWriteBinary;
    pMyObject->ReadInteger                  = BspTemplateArchiveReadInteger;
    pMyObject->ReadUlong                    = BspTemplateArchiveReadUlong;
    pMyObject->ReadDouble                   = BspTemplateArchiveReadDouble;
    pMyObject->ReadWord                     = BspTemplateArchiveReadWord;
    pMyObject->ReadBoolean                  = BspTemplateArchiveReadBoolean;
    pMyObject->ReadByte                     = BspTemplateArchiveReadByte;
    pMyObject->ReadString                   = BspTemplateArchiveReadString;
    pMyObject->ReadBinary                   = BspTemplateArchiveReadBinary;
    pMyObject->EnlargeStorage               = BspTemplateArchiveEnlargeStorage;
    pMyObject->BigEnough                    = BspTemplateArchiveBigEnough;
    pMyObject->GetRemainSize                = BspTemplateArchiveGetRemainSize;
    pMyObject->SetStorage                   = BspTemplateArchiveSetStorage;
    pMyObject->ReleaseStorage               = BspTemplateArchiveReleaseStorage;
    pMyObject->PrepareLoading               = BspTemplateArchivePrepareLoading;
    pMyObject->SaveToFile                   = BspTemplateArchiveSaveToFile;
    pMyObject->ExtractContent               = BspTemplateArchiveExtractContent;

    pMyObject->Remove                       = BspTemplateArchiveCORemove;

    pMyObject->pStorage     = 
        (PUCHAR)AnscAllocateMemory(sizeof(UCHAR) * BSP_TEMPLATE_ARCHIVE_OBJECT_STORAGE_GROW);

    pMyObject->StorageLen   = BSP_TEMPLATE_ARCHIVE_OBJECT_STORAGE_GROW;
    pMyObject->NeedRelease  = TRUE;
    pMyObject->CurPos       = 0;
    pMyObject->ContentLen   = 0;

    return  (ANSC_HANDLE)pMyObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BspTemplateArchiveCORemove
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
BspTemplateArchiveCORemove
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PBSP_TEMPLATE_ARCHIVE_OBJECT    pMyObject    = (PBSP_TEMPLATE_ARCHIVE_OBJECT)hThisObject;

    pMyObject->ReleaseStorage(hThisObject);

    AnscFreeMemory(pMyObject);

    return  ANSC_STATUS_SUCCESS;
}


