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

    module:	ansc_popular_definitions.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines some of the popular object and
        data structures used across Ansc platform.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/22/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_POPULAR_DEFINITIONS_
#define  _ANSC_POPULAR_DEFINITIONS_


/***********************************************************
        GENERAL OBJECT EMPLOYMENT CONTRACT DEFINITION
***********************************************************/

/*
 * Define some constant values that will be used by different modules. The reason we define the
 * maximum number items to limit the size of certain arrays (instead of using linked list) is that
 * most of the data structures will have to be packed into an Ipc call.
 */
#define  ANSC_CONTRACT_PARAMX_MASK                  0x00000001
#define  ANSC_CONTRACT_PARAMY_MASK                  0x00000002
#define  ANSC_CONTRACT_PARAMZ_MASK                  0x00000004

#define  ANSC_CONTRACT_TYPE_PERMANENT               1
#define  ANSC_CONTRACT_TYPE_TEMPORARY               2
#define  ANSC_CONTRACT_TYPE_ALTERNATE               3

#define  ANSC_CONTRACT_PARAM_DEFAULT                0

#define  ANSC_CONTRACT_PARAM_INGRESS                1
#define  ANSC_CONTRACT_PARAM_EGRESS                 2

#define  ANSC_CONTRACT_PARAM_MAC_UNICAST            0x00000001
#define  ANSC_CONTRACT_PARAM_MAC_MULTICAST          0x00000002
#define  ANSC_CONTRACT_PARAM_MAC_BROADCAST          0x00000004

#define  ANSC_CONTRACT_PARAM_NET_UNICAST            0x00000001
#define  ANSC_CONTRACT_PARAM_NET_MULTICAST          0x00000002
#define  ANSC_CONTRACT_PARAM_NET_BROADCAST          0x00000004

#define  ANSC_CONTRACT_PARAM_TSP_CLIENT             0x00000001
#define  ANSC_CONTRACT_PARAM_TSP_SERVER             0x00000002
#define  ANSC_CONTRACT_PARAM_TSP_SIGNAL             0x00000004

#define  ANSC_CONTRACT_PARAM_APP_CLIENT             0x00000001
#define  ANSC_CONTRACT_PARAM_APP_SERVER             0x00000002

#define  MAX_CONDITIONS_NUMBER                      8
#define  MAX_EXCEPTIONS_NUMBER                      8

#define  ANSC_CONDITION_UNKNOWN                     0
#define  ANSC_CONDITION_ANY                         1
#define  ANSC_CONDITION_NONE                        2

#define  ANSC_EXCEPTION_UNKNOWN                     0
#define  ANSC_EXCEPTION_ANY                         1
#define  ANSC_EXCEPTION_NONE                        2

/*
 * The employment/commitment relationship among Ansc component objects has grown from the original
 * one-dimensional linkage to a fully dynamic object management schema. While the contract-based
 * object management sounds great and does enable many possible ways for objects to interact with
 * each other, the real benefit may be a little bit unclear due to the fact that the component
 * objects can encapsulate that type of interaction into the object without any help from contract
 * based model. As a general rule in software architecture design, any pattern extraction and
 * behavior generalization is to reduce the repeative work that otherwise has to be done for each
 * individual component.
 */
typedef  struct
_ANSC_COEC_CONDITION
{
    ULONG                           Code;
    char                            Text[ANSC_OBJECT_NAME_SIZE];
}
ANSC_COEC_CONDITION,  *PANSC_COEC_CONDITION;

typedef  struct
_ANSC_COEC_EXCEPTION
{
    ULONG                           Code;
    char                            Text[ANSC_OBJECT_NAME_SIZE];
}
ANSC_COEC_EXCEPTION,  *PANSC_COEC_EXCEPTION;

typedef  struct
_ANSC_COEC_CONTRACT
{
    ULONG                           EmployerOid;
    ULONG                           EmployeeOid;

    ULONG                           TypeCode;
    char                            TypeText[ANSC_OBJECT_NAME_SIZE];
    ULONG                           ParamXCode;
    char                            ParamXText[ANSC_OBJECT_NAME_SIZE];
    ULONG                           ParamYCode;
    char                            ParamYText[ANSC_OBJECT_NAME_SIZE];
    ULONG                           ParamZCode;
    char                            ParamZText[ANSC_OBJECT_NAME_SIZE];
    ULONG                           ParamsMask;

    ULONG                           ConditionCount;
    ANSC_COEC_CONDITION             Conditions[MAX_CONDITIONS_NUMBER];
    ULONG                           ExceptionCount;
    ANSC_COEC_EXCEPTION             Exceptions[MAX_EXCEPTIONS_NUMBER];
}
ANSC_COEC_CONTRACT,  *PANSC_COEC_CONTRACT;


#endif
