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

    module:	tmpleng_access_co_process.c

        For Template Engine Access Component Object,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the string Component Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Kang Quan

    ---------------------------------------------------------------

    revision:

        05/30/02    initial revision.

    ---------------------------------------------------------------

    last modified:  

**********************************************************************/


#include "bspeng_co_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateAccessOutput
            (
                ANSC_HANDLE                 hThisObject,
                const char                  *pName,
                ANSC_HANDLE                 hWriter,
                ANSC_HANDLE                 hArgs,
                ULONG                       ulArgs
            )

    description:

        This function is called to output given variable.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                const char                  *pName
                Output variable name.

                ANSC_HANDLE                 hWriter
                Writer object.

                ANSC_HANDLE                 hArgs
                Argument list.

                ULONG                       ulArgs
                Number of arguments.

    return:     void.

**********************************************************************/

void
BspTemplateAccessOutput
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pName,
        ANSC_HANDLE                 hWriter,
        ANSC_HANDLE                 hArgs,
        ULONG                       ulArgs
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(pName);
    UNREFERENCED_PARAMETER(hWriter);
    UNREFERENCED_PARAMETER(hArgs);
    UNREFERENCED_PARAMETER(ulArgs);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateAccessMember
            (
                ANSC_HANDLE                 hThisObject,
                const char                  *pName,
                ANSC_HANDLE                 hWriter,
                ANSC_HANDLE                 hArgs,
                ULONG                       ulArgs
            )

    description:

        This function is called to get member of given item.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                const char                  *pName
                Variable name.

                ANSC_HANDLE                 hWriter
                Writer object.

                ANSC_HANDLE                 hArgs
                Argument list.

                ULONG                       ulArgs
                Number of arguments.

    return:     handle to member object.

**********************************************************************/

ANSC_HANDLE
BspTemplateAccessMember
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pName,
        ANSC_HANDLE                 hWriter,
        ANSC_HANDLE                 hArgs,
        ULONG                       ulArgs
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(pName);
    UNREFERENCED_PARAMETER(hWriter);
    UNREFERENCED_PARAMETER(hArgs);
    UNREFERENCED_PARAMETER(ulArgs);
    return NULL;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateAccessOutput
            (
                ANSC_HANDLE                 hThisObject,
                const char                  *pName,
                ANSC_HANDLE                 hWriter,
                ANSC_HANDLE                 hArgs,
                ULONG                       ulArgs
            )

    description:

        This function is called to get value of given item.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                const char                  *pName
                Member variable name.

                ANSC_HANDLE                 hWriter
                Writer object.

                ANSC_HANDLE                 hArgs
                Argument list.

                ULONG                       ulArgs
                Number of arguments.

    return:     handle to value object.

**********************************************************************/

ANSC_HANDLE
BspTemplateAccessValue
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pName,
        ANSC_HANDLE                 hWriter,
        ANSC_HANDLE                 hArgs,
        ULONG                       ulArgs
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(pName);
    UNREFERENCED_PARAMETER(hWriter);
    UNREFERENCED_PARAMETER(hArgs);
    UNREFERENCED_PARAMETER(ulArgs);
    return NULL;
}

