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

    module:	moam_definitions.h

        For MIB Object Access Management Implementation (MOAM),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This file defines the Moam element parameters that can be
        applied to the Moam Systems.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/30/02    initial revision.

**********************************************************************/


#ifndef  _MOAM_DEFINITIONS_
#define  _MOAM_DEFINITIONS_


/***********************************************************
         MOAM STANDARD ELEMENT PARAMETERS DEFINITION
***********************************************************/

/*
 * Unlike obj_type defined in Mib2 modules, Moam objects are not defined by the standard SMIv2
 * notation. Instead, we use following simplified data types to differentiate between different
 * types of data manipulation. Note these type definitions are consistent with the Mib2 ROM
 * interface definition.
 */
#define  MOAM_OBJ_TYPE_RESERVED                     0
#define  MOAM_OBJ_TYPE_SINT                         1
#define  MOAM_OBJ_TYPE_UINT                         2
#define  MOAM_OBJ_TYPE_LINT                         3
#define  MOAM_OBJ_TYPE_BSTR                         4
#define  MOAM_OBJ_TYPE_OID                          5
#define  MOAM_OBJ_TYPE_TBL                          6
#define  MOAM_OBJ_TYPE_BUFF                         7


/***********************************************************
   MOAM STANDARD OBJECT AND VARIABLE DESCRIPTOR DEFINITION
***********************************************************/

/*
 * Instead of defining a separate Obj Entity Object for every MIB object that is needed, we resuse
 * the base Obj Entity Objects for all the registered MIB objects while the real MIB implementation
 * is encapsulated in traditional C-style modules. The benefits of such design are:
 *
 *      (1) the API interface is easier-to-understand to non-C++ programmers,
 *      (2) smaller code-size since no object-construction code is needed.
 *      (3) a much simpler C interface can be used for third-party extensions
 *
 * However, there're disadvantages as well associated with such approach:
 *
 *      (1) most benefits of object-oriented programming is no longer available
 *      (2) no extension or any functional enhancement is possible
 */
typedef  struct
_MOAM_VAR_VALUE_DESCRIPTOR
{
    char*                           ObjName;
    ULONG                           ObjType;

    void*                           MapInstance;
    void*                           ClrInstance;
    void*                           GetInsCount;
    void*                           GetInsArray;
    void*                           GetNextIns;

    void*                           Commit;
    void*                           Undo;
    void*                           GetThrough;
    void*                           TstThrough;
    void*                           SetThrough;

    void*                           GetValue;
    void*                           TstValue;
    void*                           SetValue;
}
MOAM_VAR_VALUE_DESCRIPTOR,  *PMOAM_VAR_VALUE_DESCRIPTOR;

typedef  struct
_MOAM_VAR_TABLE_DESCRIPTOR
{
    char*                           ObjName;
    ULONG                           ObjType;

    void*                           MapInstance;
    void*                           ClrInstance;
    void*                           GetInsCount;
    void*                           GetInsArray;
    void*                           GetNextIns;

    void*                           Commit;
    void*                           Undo;
    void*                           GetThrough;
    void*                           TstThrough;
    void*                           SetThrough;

    void*                           DefRow;
    void*                           AddRow;
    void*                           DelRow;
    void*                           MovRow;
    void*                           TstRow;
}
MOAM_VAR_TABLE_DESCRIPTOR,  *PMOAM_VAR_TABLE_DESCRIPTOR;

/*
 * Every MIB object used in the system MUST be associated with a unique MIB Object Descriptor data
 * structure. This descriptor retains the type, name, oid, and syntax of the corresponding MIB
 * object. When system initializes, a MIB Object Tree will be constructed based on the information
 * provided by the Object Descriptors.
 */
typedef  struct
_MOAM_OBJ_ENTITY_DESCRIPTOR
{
    char*                           ObjEntityName;
    ULONG                           ObjEntityType;
    char*                           ObjEntitySyntax;
    void*                           VarEntityDesp;
}
MOAM_OBJ_ENTITY_DESCRIPTOR,  *PMOAM_OBJ_ENTITY_DESCRIPTOR;


/***********************************************************
         MOAM STANDARD OBJ GROUP DESCRIPTOR DEFINITION
***********************************************************/

/*
 * As part of the effort to make the MOAM module extensible and scalable, we pre-organize all the
 * MIB objects into different object groups. Each Moam Obj Group Object is responsible for loading
 * and creating all its Obj Entry Objects during initialization. For any newly defined MIB objects,
 * a corresponding Obj Group Object must be provided.
 */
#define  MOAM_MODULE_REG_TRIGGER_AUTOMATIC          0
#define  MOAM_MODULE_REG_TRIGGER_LOADING            1
#define  MOAM_MODULE_REG_TRIGGER_SWITCHING          2
#define  MOAM_MODULE_REG_TRIGGER_DIRECT             3

typedef  struct
_MOAM_OBJ_MODULE_DESCRIPTOR
{
    char*                           ObjModuleName;
    char*                           ObjTargetName;
    char*                           ObjDstMicName;
    void*                           ObjDstMicIf;
    ULONG                           RegistrationTrigger;
    void*                           Constructor;
    void*                           ModuleVarMap;
    BOOL                            bAccessible;
}
MOAM_OBJ_MODULE_DESCRIPTOR,  *PMOAM_OBJ_MODULE_DESCRIPTOR;


#endif
