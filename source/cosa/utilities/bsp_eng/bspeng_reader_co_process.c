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

    module:	bspeng_reader_co_process.c

        For BSP Engine Reader Component Object,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the BSP engine reader Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        05/14/03    initial revision.

    ---------------------------------------------------------------

    last modified:  

**********************************************************************/


#include "bspeng_co_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        BspTemplateReaderRead
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       pBuffer,
                ULONG                       ulSize
            )

    description:

        This function is called to read specified amount of data.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       pBuffer
                Buffer to hold data.

                ULONG                       ulSize
                The number of bytes to read.

    return:     the number of bytes read.

**********************************************************************/

ULONG
BspTemplateReaderRead
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       pBuffer,
        ULONG                       ulSize
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(pBuffer);
    UNREFERENCED_PARAMETER(ulSize);
    /* 
     * The virtual function "Read" of object TEMPLATE_READER_OBJECT 
     * should not be called. 
     */

    return 0;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BspTemplateReaderClose
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to close the reader object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BspTemplateReaderClose
    (
        ANSC_HANDLE                 hThisObject
    )
{
    /* 
     * The virtual function "Close" of object TEMPLATE_READER_OBJECT 
     * should not be called.
     */
    UNREFERENCED_PARAMETER(hThisObject);
    return ANSC_STATUS_FAILURE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateListOpenExternal
            (
                ANSC_HANDLE                 hThisObject,
                char                        *pName
            )

    description:

        This function is called to open an external template.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char                        *pName
                Resource name.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BspTemplateReaderOpenExternal
    (
        ANSC_HANDLE                 hThisObject,
        char                        *pName
    )
{
    /* 
     * The virtual function "OpenExternal" of object TEMPLATE_READER_OBJECT 
     * should not be called.
     */
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(pName);
    return ANSC_STATUS_FAILURE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateListOpenInternal
            (
                ANSC_HANDLE                 hThisObject,
                char                        *pName,
                char                        *pContent,
                ULONG                       ulContentLen
            )

    description:

        This function is called to open a resource from given buffer.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char                        *pName
                Template name.

                char                        *pContent
                Template content.

                ULONG                       ulContentLen
                Template content length.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BspTemplateReaderOpenInternal
    (
        ANSC_HANDLE                 hThisObject,
        char                        *pName,
        char                        *pContent,
        ULONG                       ulContentLen
    )
{
    /* 
     * The virtual function "OpenInternal" of object TEMPLATE_READER_OBJECT 
     * should not be called.
     */
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(pName);
    UNREFERENCED_PARAMETER(pContent);
    UNREFERENCED_PARAMETER(ulContentLen);
    return ANSC_STATUS_FAILURE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char *
        BspTemplateListGetTemplateName
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to get template name.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     template name.

**********************************************************************/

char *
BspTemplateReaderGetTemplateName
    (
        ANSC_HANDLE                 hThisObject
    )
{
    /* 
     * The virtual function "GetTemplateName" of object TEMPLATE_READER_OBJECT 
     * should not be called.
     */
    UNREFERENCED_PARAMETER(hThisObject);
    return NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BspTemplateListSetExtPath
            (
                ANSC_HANDLE                 hThisObject,
                char                        *pExtPath
            )

    description:

        This function is called to set external path.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                char                        *pExtPath
                External path.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BspTemplateReaderSetExtPath
    (
        ANSC_HANDLE                 hThisObject,
        char                        *pExtPath
    )
{
    PBSP_TEMPLATE_READER_OBJECT     pMyObject   = (PBSP_TEMPLATE_READER_OBJECT)hThisObject;

    if (pExtPath)
    {
        ULONG                       ulLen, ulCopySize;

        ulLen   = AnscSizeOfString(pExtPath);

        if (ulLen >= TEMPLATE_READER_EXT_FILE_PATH_LEN - 1)
            ulCopySize  = TEMPLATE_READER_EXT_FILE_PATH_LEN - 1;
        else
            ulCopySize  = ulLen;

        AnscCopyMemory(pMyObject->ExtPath, pExtPath, ulCopySize);
        pMyObject->ExtPath[ulCopySize] = 0;
    }

    return ANSC_STATUS_SUCCESS;
}


