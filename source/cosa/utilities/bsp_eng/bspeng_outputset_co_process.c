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

    module:	bspeng_outputset_co_process.c

        For BSP Engine Reader Component Object,
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

        05/14/03    initial revision.

    ---------------------------------------------------------------

    last modified:  

**********************************************************************/


#include "bspeng_co_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateListOutputSet
            (
                ANSC_HANDLE                 hThisObject,
                const char                  *pName,
                ANSC_HANDLE                 hWriter,
                ANSC_HANDLE                 hArgs,
                ULONG                       ulArgs
            )

    description:

        This function is called to set output destination.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                const char                  *pName
                Name of output.

                ANSC_HANDLE                 hWriter
                Writer object.

                ANSC_HANDLE                 hArgs
                Arguments.

                ULONG                       ulArgs
                Number of arguments.

    return:     handle to output object.

**********************************************************************/

ANSC_HANDLE
BspTemplateOutputSet
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
    /*
     * This virtual function shouldn't be called.
     */

    return NULL;
}

