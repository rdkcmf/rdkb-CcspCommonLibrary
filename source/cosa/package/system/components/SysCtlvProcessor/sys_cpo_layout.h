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

    module: sys_cpo_layout.h

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file defines the CTLV file layout that is
        provisioned/serialized by the Sys Ctlv Processor Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Ding Hua

    ---------------------------------------------------------------

    revision:

        01/13/06    initial revision.

**********************************************************************/


#ifndef  _SYS_CPO_LAYOUT_
#define  _SYS_CPO_LAYOUT_


/***********************************************************
            SYS CPO CTLV LAYOUT DEFINITION
***********************************************************/

/*
 *  System Repository folders/records will be converted into a giant CTLV (Class/Type/Length/Value)
 *  tuple stream.
 *
 *  The introduction of "composite" CTLV tuple makes the parsing and generating of the
 *  CTLV tuple stream similar to the tree-based Repository operation.
 *
 *  Below is the overall layout of CTLV file format for System Repository:
 *
    CLASS_CSTR              TYPE_FILE_TYPE_NAME LENGTH      "Ctlv-Sys-Repository"
    CLASS_UNIT              TYPE_CFG_FILE_VER   LENGTH

    CLASS_COMPOSITE         TYPE_FOLDER         LENGTH
        CLASS_CSTR          TYPE_FOLDER_NAME    LENGTH      Folder Name, e.g. "IpsController"
        
        CLASS_COMPOSITE     TYPE_RECORD         LENGTH
            CLASS_CSTR      TYPE_RECORD_NAME    LENGTH      Attribute Name
            CLASS_XYZ       TYPE_RECORD_DATA    LENGTH      Attribute Data

        CLASS_COMPOSITE     TYPE_RECORD         LENGTH
            CLASS_CSTR      TYPE_RECORD_NAME    LENGTH      Attribute Name
            CLASS_XYZ       TYPE_RECORD_DATA    LENGTH      Attribute Data

        CLASS_COMPOSITE     TYPE_FOLDER         LENGTH      (Sub-Folder)
            CLASS_CSTR      TYPE_FOLDER_NAME    LENGTH      Sub-Folder Name, e.g. "LinkTable"

            CLASS_COMPOSITE     TYPE_RECORD         LENGTH
                CLASS_CSTR      TYPE_RECORD_NAME    LENGTH      Attribute Name
                CLASS_XYZ       TYPE_RECORD_DATA    LENGTH      Attribute Data

            CLASS_COMPOSITE     TYPE_FOLDER         LENGTH      (Sub-Folder)
                CLASS_CSTR      TYPE_FOLDER_NAME    LENGTH      Sub-Folder Name, e.g. "WAN"
            ...

        CLASS_COMPOSITE     TYPE_FOLDER         LENGTH      (Sub-Folder)
            CLASS_CSTR      TYPE_FOLDER_NAME    LENGTH      Sub-Folder Name, e.g. "ZoneTable"
            ...

    CLASS_COMPOSITE         TYPE_FOLDER         LENGTH

        CLASS_CSTR          TYPE_FOLDER_NAME    LENGTH      Folder Name, e.g. "SecController"
        
        CLASS_COMPOSITE     TYPE_RECORD         LENGTH
            CLASS_CSTR      TYPE_RECORD_NAME    LENGTH      Attribute Name
            CLASS_XYZ       TYPE_RECORD_DATA    LENGTH      Attribute Data
            
        CLASS_COMPOSITE     TYPE_FOLDER         LENGTH      (Sub-Folder)
            CLASS_CSTR      TYPE_FOLDER_NAME    LENGTH      Sub-Folder Name
            ...
 *
 */

/*
 *  The definitions of value/class/type
 */
#define  SYS_CPO_LAYOUT_fileTypeName                        "Ctlv-Sys-Repository"
#define  SYS_CPO_LAYOUT_version                             1

#define  SYS_CPO_CLASS_fileTypeName                         SYS_CTLV_CLASS_CSTR
#define  SYS_CPO_CLASS_version                              SYS_CTLV_CLASS_UINT

#define  SYS_CPO_TYPE_fileTypeName                          16
#define  SYS_CPO_TYPE_version                               17

#define  SYS_CPO_TYPE_folder                                20
#define  SYS_CPO_TYPE_folderName                            21
#define  SYS_CPO_TYPE_record                                22
#define  SYS_CPO_TYPE_recordName                            23
#define  SYS_CPO_TYPE_recordData                            24


#endif

