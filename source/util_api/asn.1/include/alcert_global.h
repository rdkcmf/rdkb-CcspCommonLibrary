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

    MODULE: alcert_global.h

        For AL Certificate Library
        by Cisco Systems, Inc.

    ---------------------------------------------------------------

    FUNCTIONAL DESCRIPTION:

        This header file defines some common data types.

    ---------------------------------------------------------------

    ENVIRONMENT:

        Platform Independent.

    ---------------------------------------------------------------

    AUTHOR:

        Bin Zhu

    ---------------------------------------------------------------

    REVISION HISTORY:

        *   09/25/02    initial revision

**********************************************************************/


#ifndef     _AL_CERT_GLOBAL_H
#define     _AL_CERT_GLOBAL_H

/*
 * in driver, AL_STATUS is actually NT_STATUS (four bytes) and AL_HANDLE is actually NDIS_HANDLE (four bytes); in application,
 * AL_STATUS is actually unsigned integer (four bytes), so does AL_HANDLE;
 */
#ifndef  _AL_DEPENDENT_

typedef  ULONG                                      AL_STATUS,  *PAL_STATUS;
typedef  PVOID                                      AL_HANDLE,  *PAL_HANDLE;
typedef  AL_HANDLE                                  AL_LOCK,    *PAL_LOCK;
typedef  AL_HANDLE                                  AL_EVENT,   *PAL_EVENT;

#define  SINGLE_LIST_ENTRY                          SINGLE_LINK_ENTRY

#define  AL_STATUS_FAILURE                          0xC0000001
#define  AL_STATUS_SUCCESS                          0

#endif /* _AL_DEPENDENT_ */

#ifndef CERT_TRACE
#define CERT_TRACE                                  AnscTrace
#endif

#ifndef ALCertFreeMemory
#define ALCertFreeMemory                            AnscFreeMemory
#endif

#ifndef MAXIMUM_HASH_RESULT
#define MAXIMUM_HASH_RESULT                         32
#endif

#endif /* AL_CERT_GLOBAL_H */
