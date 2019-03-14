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

    module:	bspeng_co_exported_api.h

        For BSP Engine Component Object,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This header file contains the prototype definition for all
        the exported functions provided by the string component
        object.

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


#ifndef  _BSPENG_CO_EXPORTED_API_
#define  _BSPENG_CO_EXPORTED_API_

/***********************************************************
   FUNCTIONS IMPLEMENTED IN TMPLENG_READER_CO_INTERFACE.C
***********************************************************/

ANSC_HANDLE
CreateBspEngReaderComponent
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );


ANSC_HANDLE
BspTemplateReaderCOCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );

ANSC_STATUS
BspTemplateReaderCORemove
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BspTemplateReaderCOInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

/***********************************************************
   FUNCTIONS IMPLEMENTED IN TMPLENG_ACCESS_CO_INTERFACE.C
***********************************************************/

ANSC_HANDLE
CreateBspEngAccessComponent
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );


ANSC_HANDLE
BspTemplateAccessCOCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );

ANSC_STATUS
BspTemplateAccessCORemove
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BspTemplateAccessCOInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

/***********************************************************
   FUNCTIONS IMPLEMENTED IN TMPLENG_LIST_CO_INTERFACE.C
***********************************************************/

ANSC_HANDLE
CreateBspEngListComponent
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );


ANSC_HANDLE
BspTemplateListCOCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );

ANSC_STATUS
BspTemplateListCORemove
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BspTemplateListCOInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

/***********************************************************
   FUNCTIONS IMPLEMENTED IN TMPLENG_OUTPUTSET_CO_INTERFACE.C
***********************************************************/

ANSC_HANDLE
CreateBspEngOutputSetComponent
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );


ANSC_HANDLE
BspTemplateOutputSetCOCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );

ANSC_STATUS
BspTemplateOutputSetCORemove
    (
        ANSC_HANDLE                 hThisObject
    );


ANSC_STATUS
BspTemplateOutputSetCOInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

/***********************************************************
   FUNCTIONS IMPLEMENTED IN TMPLENG_VAR_CO_INTERFACE.C
***********************************************************/

ANSC_HANDLE
CreateBspEngVarComponent
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );


ANSC_HANDLE
BspTemplateVarCOCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );

ANSC_STATUS
BspTemplateVarCORemove
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BspTemplateVarCOInitialize
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
   FUNCTIONS IMPLEMENTED IN TMPLENG_BRANCH_CO_INTERFACE.C
***********************************************************/

ANSC_HANDLE
CreateBspEngBranchComponent
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );


ANSC_HANDLE
BspTemplateBranchCOCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );

ANSC_STATUS
BspTemplateBranchCORemove
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BspTemplateBranchCOInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

/***********************************************************
   FUNCTIONS IMPLEMENTED IN TMPLENG_TEMP_CO_INTERFACE.C
***********************************************************/

ANSC_HANDLE
CreateBspEngTempComponent
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );


ANSC_HANDLE
BspTemplateTempCOCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );

ANSC_STATUS
BspTemplateTempCORemove
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BspTemplateTempCOInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

/***********************************************************
   FUNCTIONS IMPLEMENTED IN TMPLENG_STACK_CO_INTERFACE.C
***********************************************************/

ANSC_HANDLE
CreateBspEngStackComponent
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );


ANSC_HANDLE
BspTemplateStackCOCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );

ANSC_STATUS
BspTemplateStackCORemove
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BspTemplateStackCOInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

/***********************************************************
   FUNCTIONS IMPLEMENTED IN TMPLENG_RUNTIME_CO_INTERFACE.C
***********************************************************/

ANSC_HANDLE
CreateBspEngRuntimeComponent
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );


ANSC_HANDLE
BspTemplateRuntimeCOCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );

ANSC_STATUS
BspTemplateRuntimeCORemove
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BspTemplateRuntimeCOInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

/***********************************************************
   FUNCTIONS IMPLEMENTED IN TMPLENG_WRITER_CO_INTERFACE.C
***********************************************************/

ANSC_HANDLE
CreateBspEngWriterComponent
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );


ANSC_HANDLE
BspTemplateWriterCOCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );

ANSC_STATUS
BspTemplateWriterCORemove
    (
        ANSC_HANDLE                 hThisObject
    );

ANSC_STATUS
BspTemplateWriterCOInitialize
    (
        ANSC_HANDLE                 hThisObject
    );

/***********************************************************
   FUNCTIONS IMPLEMENTED IN TMPLENG_CO_INTERFACE.C
***********************************************************/

ANSC_HANDLE
CreateBspEngComponent
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );


ANSC_HANDLE
BspEngCOCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );

ANSC_STATUS
BspEngCORemove
    (
        ANSC_HANDLE                 hThisObject
    );


/***********************************************************
   FUNCTIONS IMPLEMENTED IN TMPLENG_CO_INTERFACE.C
***********************************************************/

ANSC_HANDLE
CreateBspEngArchiveComponent
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );


ANSC_HANDLE
BspTemplateArchiveCOCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ANSC_HANDLE                 hOwnerContext,
        ANSC_HANDLE                 hAnscReserved
    );

ANSC_STATUS
BspTemplateArchiveCORemove
    (
        ANSC_HANDLE                 hThisObject
    );


#endif
