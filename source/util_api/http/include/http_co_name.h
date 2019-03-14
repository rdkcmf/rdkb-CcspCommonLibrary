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

    module:	http_co_name.h

        For HyperText Transfer Protocol Implementation (HTTP),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the object names for the Http
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


#ifndef  _HTTP_CO_NAME_
#define  _HTTP_CO_NAME_


/***********************************************************
            GENERAL HTTP FEATURE OBJECTS DEFINITION
***********************************************************/

/*
 * Define the object names for all the Feature Objects that cannot be categorized. Feature Objects
 * are the objects that encapsulate certain features and provide services.
 */
#define  HTTP_SIMPLE_CLIENT_NAME                    "simpleClient"
#define  HTTP_SIMPLE_SERVER_NAME                    "simpleServer"
#define  HTTP_SIMPLE_PROXY_NAME                     "simpleProxy"
#define  HTTP_PROXY_SESSION_NAME                    "proxySession"

#define  HTTP_ADVANCED_PROXY_NAME                   "advancedProxy"
#define  HTTP_PSO_VER2_NAME                         "psoVer2"
#define  HTTP_TRANS_RECORD_NAME                     "transRecord"

#define  HTTP_WEBS_SESSION_NAME                     "websSession"
#define  HTTP_WEBS_TRANS_NAME                       "websTrans"
#define  HTTP_WEBC_SESSION_NAME                     "webcSession"
#define  HTTP_WEBC_TRANS_NAME                       "webcTrans"

#define  HTTP_BASIC_MESSAGE_NAME                    "basicMessage"
#define  HTTP_BMO_REQ_NAME                          "bmoReq"
#define  HTTP_BMO_REP_NAME                          "bmoRep"

#define  HTTP_FORM_CONTENT_NAME                     "formContent"
#define  HTTP_MESSAGE_BODY_NAME                     "messageBody"
#define  HTTP_MBO_IDENTITY_NAME                     "mboIdentity"
#define  HTTP_MBO_CHUNKED_NAME                      "mboChunked"
#define  HTTP_MBO_GZIPPED_NAME                      "mboGzipped"
#define  HTTP_MBO_COMPRESSED_NAME                   "mboCompressed"
#define  HTTP_MBO_DEFLATED_NAME                     "mboDeflated"
#define  HTTP_MBO_FILE_NAME                         "mboFile"

#define  HTTP_ATO_STATUS_NAME                       "atoStatus"
#define  HTTP_ATO_HEADER_NAME                       "atoHeader"
#define  HTTP_ATO_METHOD_NAME                       "atoMethod"
#define  HTTP_ATO_CGIENV_NAME                       "atoCgiEnv"
#define  HTTP_ATO_MCTYPE_NAME                       "atoMcType"
#define  HTTP_ATO_SCTYPE_NAME                       "atoScType"
#define  HTTP_ATO_FXCAT1_NAME                       "atoFxCat1"
#define  HTTP_ATO_FXCAT2_NAME                       "atoFxCat2"

#define  HTTP_AUTHO_NAME                            "httpAutho"
#define  HTTP_AUTHCO_NAME                           "httpAuthco"
#define  HTTP_AUTHSO_NAME                           "httpAuthso"


#endif
