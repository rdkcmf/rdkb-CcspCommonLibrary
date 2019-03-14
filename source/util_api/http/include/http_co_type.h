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

    module:	http_co_type.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the object types for the Http
        Component Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        02/20/02    initial revision.

**********************************************************************/


#ifndef  _HTTP_CO_TYPE_
#define  _HTTP_CO_TYPE_


/***********************************************************
            GENERAL HTTP FEATURE OBJECTS DEFINITION
***********************************************************/

/*
 * Define the object names for all the Feature Objects that cannot be categorized. Feature Objects
 * are the objects that encapsulate certain features and provide services.
 */
#define  HTTP_SIMPLE_CLIENT_TYPE                    ANSC_OBJECT_TYPE_NULL
#define  HTTP_SIMPLE_SERVER_TYPE                    ANSC_OBJECT_TYPE_NULL
#define  HTTP_SIMPLE_PROXY_TYPE                     ANSC_OBJECT_TYPE_NULL
#define  HTTP_PROXY_SESSION_TYPE                    ANSC_OBJECT_TYPE_NULL

#define  HTTP_ADVANCED_PROXY_TYPE                   ANSC_OBJECT_TYPE_NULL
#define  HTTP_PSO_VER2_TYPE                         ANSC_OBJECT_TYPE_NULL
#define  HTTP_TRANS_RECORD_TYPE                     ANSC_OBJECT_TYPE_NULL

#define  HTTP_WEBS_SESSION_TYPE                     ANSC_OBJECT_TYPE_NULL
#define  HTTP_WEBS_TRANS_TYPE                       ANSC_OBJECT_TYPE_NULL
#define  HTTP_WEBC_SESSION_TYPE                     ANSC_OBJECT_TYPE_NULL
#define  HTTP_WEBC_TRANS_TYPE                       ANSC_OBJECT_TYPE_NULL

#define  HTTP_BASIC_MESSAGE_TYPE                    ANSC_OBJECT_TYPE_NULL
#define  HTTP_BMO_REQ_TYPE                          "req"
#define  HTTP_BMO_REP_TYPE                          "rep"

#define  HTTP_FORM_CONTENT_TYPE                     ANSC_OBJECT_TYPE_NULL
#define  HTTP_MESSAGE_BODY_TYPE                     ANSC_OBJECT_TYPE_NULL
#define  HTTP_MBO_IDENTITY_TYPE                     "identity"
#define  HTTP_MBO_CHUNKED_TYPE                      "chunked"
#define  HTTP_MBO_GZIPPED_TYPE                      "gzipped"
#define  HTTP_MBO_COMPRESSED_TYPE                   "compressed"
#define  HTTP_MBO_DEFLATED_TYPE                     "deflated"
#define  HTTP_MBO_FILE_TYPE                         "file"

#define  HTTP_ATO_STATUS_TYPE                       "status"
#define  HTTP_ATO_HEADER_TYPE                       "header"
#define  HTTP_ATO_METHOD_TYPE                       "method"
#define  HTTP_ATO_CGIENV_TYPE                       "cgiEnv"
#define  HTTP_ATO_MCTYPE_TYPE                       "mcType"
#define  HTTP_ATO_SCTYPE_TYPE                       "scType"
#define  HTTP_ATO_FXCAT1_TYPE                       "fxCat1"
#define  HTTP_ATO_FXCAT2_TYPE                       "fxCat2"


#endif
