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

    module: dslh_dmagnt_mwsif.c

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the some platform-dependent and
        DslhMwsIf functions in Dslh DataModelAgent Object;

        *   COSAMwsIfGetResource

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        03/03/2011    initial revision.

**********************************************************************/


#include "dslh_dmagnt_global.h"
#include "safec_lib_common.h"


/**********************************************************************

    caller:     component objects

    prototype:

    char*
    COSAMwsIfGetResource
        (
            ANSC_HANDLE                 hThisObject,
            char*                       pUrlPath,      
            char*                       pMediaType,    
            PULONG                      pulMediaTypeLength,
            PULONG                      pulResourceLen     
        );

    description:

        This function is called to retrieve the content specified by url path;

    argument:   ANSC_HANDLE                 hThisObject,
                This component object;

                char*                       pUrlPath,           
                absolute URL path to web root;

                char*                       pMediaType,         
                The output of media type;

                PULONG                      pulMediaTypeLength, 
                The buffer of length of media type;

                PULONG                      pulResourceLen      
                The buffer of length of resource;

    return:     content of the URL;

**********************************************************************/
static char                   s_DataModel_TR106_XML[] =
"<?xml version=\"1.0\" encoding=\"utf-8\" ?> \
<documentation><feature name=\"NAT\" />\n<feature name=\"Firewall\" />\n</documentation>\n";

char*
COSAMwsIfGetResource
    (
        ANSC_HANDLE                 hThisObject,
        char*                       pUrlPath,           /* absolute URL path to web root */
        char*                       pMediaType,         /* OUT - media type associated with the URL, used to indicate content-type */
        PULONG                      pulMediaTypeLength, /* IN/OUT */
        PULONG                      pulResourceLen      /* OUT - length of resource */
    )
{
    errno_t                         rc              = -1;
    UNREFERENCED_PARAMETER(hThisObject);
    if( pUrlPath == NULL || !strcasecmp(pUrlPath, _DATA_MODEL_XML_PATH) == 0 )
    {
        return NULL;
    }

    rc = strcpy_s(pMediaType, *pulMediaTypeLength, "text/xml");
    if(rc != EOK)
    {
       ERR_CHK(rc);
       return NULL;
    }

    *pulMediaTypeLength  = AnscSizeOfString("text/xml");

    *pulResourceLen      = AnscSizeOfString(s_DataModel_TR106_XML);

    return AnscCloneString(s_DataModel_TR106_XML);
}
