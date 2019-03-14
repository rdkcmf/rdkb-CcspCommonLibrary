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

    module:	sys_smo_utility.c

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced utilities functions
        of the Sys Setup Manager Object.

        *   SysSmoGetRroTmplSizeByRrtd
        *   SysSmoGetRfoTmplSizeByRftd
        *   SysSmoWriteRroTmplByRrtd
        *   SysSmoAllocRfoTmplByRftd
        *   SysSmoAllocRftdByRfoTmpl
        *   SysSmoWriteRrtdByRroTmpl
        *   SysSmoCleanRftDescriptor
        *   SysSmoCleanRrtDescriptor

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        08/21/02    initial revision.

**********************************************************************/


#include "sys_smo_global.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SysSmoGetRroTmplSizeByRrtd
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hRrtd
            );

    description:

        This function is called to calculate the size for a record
        template that is specified by the RRT descriptor.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hRrtd
                Specifies the RRT descriptor to be calculated.

    return:     template size.

**********************************************************************/

ULONG
SysSmoGetRroTmplSizeByRrtd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRrtd
    )
{
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;
    PSYS_RRT_DESCRIPTOR             pRrtd                = (PSYS_RRT_DESCRIPTOR)hRrtd;
    ULONG                           ulCtlvSizeTotal      = 0;
    ULONG                           ulCtlvSizeName       = 0;
    ULONG                           ulCtlvSizePermission = 0;
    ULONG                           ulCtlvSizeType       = 0;
    ULONG                           ulCtlvSizeValue      = 0;
    ULONG                           ulCtlvSizeAsFolder   = 0;
    ULONG                           ulCtlvSizeContent    = 0;
    ULONG                           ulCtlvSizeFormat     = 0;
    ULONG                           ulCtlvSizeVerify     = 0;
    ULONG                           ulCtlvSizeMinLen     = 0;
    ULONG                           ulCtlvSizeMaxLen     = 0;
    ULONG                           ulCtlvSizeCall       = 0;
    ULONG                           ulCtlvSizeContext    = 0;
    ULONG                           ulCtlvSizeLanguage   = 0;
    ULONG                           ulCtlvSizeCharset    = 0;
    ULONG                           ulCtlvSizeIcon       = 0;
    ULONG                           ulCtlvSizeMapper     = 0;
    ULONG                           ulCtlvSizeEnd        = 0;

    if ( pRrtd->Name )
    {
        ulCtlvSizeName = sizeof(SYS_CTLV_TUPLE) + AnscSizeOfString(pRrtd->Name);
    }

    if ( TRUE )
    {
        ulCtlvSizePermission = sizeof(SYS_CTLV_TUPLE) + sizeof(ULONG);
    }

    if ( TRUE )
    {
        ulCtlvSizeType = sizeof(SYS_CTLV_TUPLE) + sizeof(ULONG);
    }

    if ( pRrtd->Value )
    {
        ulCtlvSizeValue = sizeof(SYS_CTLV_TUPLE) + AnscSizeOfString(pRrtd->Value);
    }

    if ( TRUE )
    {
        ulCtlvSizeAsFolder = sizeof(SYS_CTLV_TUPLE) + sizeof(ULONG);
    }

    if ( TRUE )
    {
        ulCtlvSizeContent = sizeof(SYS_CTLV_TUPLE) + sizeof(ULONG);
    }

    if ( TRUE )
    {
        ulCtlvSizeFormat = sizeof(SYS_CTLV_TUPLE) + sizeof(ULONG);
    }

    if ( TRUE )
    {
        ulCtlvSizeVerify = sizeof(SYS_CTLV_TUPLE) + sizeof(ULONG);
    }

    if ( TRUE )
    {
        ulCtlvSizeMinLen = sizeof(SYS_CTLV_TUPLE) + sizeof(ULONG);
    }

    if ( TRUE )
    {
        ulCtlvSizeMaxLen = sizeof(SYS_CTLV_TUPLE) + sizeof(ULONG);
    }

    if ( TRUE )
    {
        ulCtlvSizeCall = sizeof(SYS_CTLV_TUPLE) + sizeof(ULONG);
    }

    if ( TRUE )
    {
        ulCtlvSizeContext = sizeof(SYS_CTLV_TUPLE) + sizeof(ULONG);
    }

    if ( pRrtd->Language )
    {
        ulCtlvSizeLanguage = sizeof(SYS_CTLV_TUPLE) + AnscSizeOfString(pRrtd->Language);
    }

    if ( pRrtd->Charset )
    {
        ulCtlvSizeCharset = sizeof(SYS_CTLV_TUPLE) + AnscSizeOfString(pRrtd->Charset);
    }

    if ( pRrtd->Icon )
    {
        ulCtlvSizeIcon = sizeof(SYS_CTLV_TUPLE) + AnscSizeOfString(pRrtd->Icon);
    }

    if ( pRrtd->Mapper )
    {
        ulCtlvSizeMapper = sizeof(SYS_CTLV_TUPLE) + AnscSizeOfString(pRrtd->Mapper);
    }

    if ( TRUE )
    {
        ulCtlvSizeEnd = sizeof(SYS_CTLV_TUPLE);
    }

    ulCtlvSizeTotal = ulCtlvSizeName     + ulCtlvSizePermission + ulCtlvSizeType    +
                      ulCtlvSizeValue    + ulCtlvSizeAsFolder   + ulCtlvSizeContent +
                      ulCtlvSizeFormat   + ulCtlvSizeVerify     + ulCtlvSizeMinLen  +
                      ulCtlvSizeMaxLen   + ulCtlvSizeCall       + ulCtlvSizeContext +
                      ulCtlvSizeLanguage + ulCtlvSizeCharset    + ulCtlvSizeIcon    +
                      ulCtlvSizeMapper   + ulCtlvSizeEnd;

    return  ulCtlvSizeTotal;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG
        SysSmoGetRfoTmplSizeByRftd
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hRftd
            );

    description:

        This function is called to calculate the size for a record
        template that is specified by the RFT descriptor.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hRftd
                Specifies the RFT descriptor to be calculated.

    return:     template size.

**********************************************************************/

ULONG
SysSmoGetRfoTmplSizeByRftd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRftd
    )
{
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;
    PSYS_RFT_DESCRIPTOR             pRftd                = (PSYS_RFT_DESCRIPTOR)hRftd;
    ULONG                           ulCtlvSizeTotal      = 0;
    ULONG                           ulCtlvSizeMoniker    = 0;
    ULONG                           ulCtlvSizePermission = 0;
    ULONG                           ulCtlvSizeType       = 0;
    ULONG                           ulCtlvSizeNamingMtd  = 0;
    ULONG                           ulCtlvSizeContent    = 0;
    ULONG                           ulCtlvSizeLanguage   = 0;
    ULONG                           ulCtlvSizeCharset    = 0;
    ULONG                           ulCtlvSizeIcon       = 0;
    ULONG                           ulCtlvSizeTemplate   = 0;
    ULONG                           ulCtlvSizeRdArray    = 0;
    ULONG                           ulCtlvSizeRdEntry    = 0;
    ULONG                           ulCtlvSizeEnd        = 0;
    ULONG                           ulRrtdIndex          = 0;

    if ( pRftd->Moniker )
    {
        ulCtlvSizeMoniker = sizeof(SYS_CTLV_TUPLE) + AnscSizeOfString(pRftd->Moniker);
    }

    if ( TRUE )
    {
        ulCtlvSizePermission = sizeof(SYS_CTLV_TUPLE) + sizeof(ULONG);
    }

    if ( TRUE )
    {
        ulCtlvSizeType = sizeof(SYS_CTLV_TUPLE) + sizeof(ULONG);
    }

    if ( TRUE )
    {
        ulCtlvSizeNamingMtd = sizeof(SYS_CTLV_TUPLE) + sizeof(ULONG);
    }

    if ( TRUE )
    {
        ulCtlvSizeContent = sizeof(SYS_CTLV_TUPLE) + sizeof(ULONG);
    }

    if ( pRftd->Language )
    {
        ulCtlvSizeLanguage = sizeof(SYS_CTLV_TUPLE) + AnscSizeOfString(pRftd->Language);
    }

    if ( pRftd->Charset )
    {
        ulCtlvSizeCharset = sizeof(SYS_CTLV_TUPLE) + AnscSizeOfString(pRftd->Charset);
    }

    if ( pRftd->Icon )
    {
        ulCtlvSizeIcon = sizeof(SYS_CTLV_TUPLE) + AnscSizeOfString(pRftd->Icon);
    }

    if ( pRftd->Template )
    {
        ulCtlvSizeTemplate = sizeof(SYS_CTLV_TUPLE) + AnscSizeOfString(pRftd->Template);
    }

    while ( pRftd->RrtdArray[ulRrtdIndex].Name != NULL )
    {
        ulCtlvSizeRdEntry  = sizeof(SYS_CTLV_TUPLE);
        ulCtlvSizeRdEntry +=
            SysSmoGetRroTmplSizeByRrtd
                (
                    hThisObject,
                    (ANSC_HANDLE)&pRftd->RrtdArray[ulRrtdIndex]
                );

        ulCtlvSizeRdArray += ulCtlvSizeRdEntry;

        ulRrtdIndex++;
    }

    if ( TRUE )
    {
        ulCtlvSizeEnd = sizeof(SYS_CTLV_TUPLE);
    }

    ulCtlvSizeTotal = ulCtlvSizeMoniker   + ulCtlvSizePermission + ulCtlvSizeType     +
                      ulCtlvSizeNamingMtd + ulCtlvSizeContent    + ulCtlvSizeLanguage +
                      ulCtlvSizeCharset   + ulCtlvSizeIcon       + ulCtlvSizeTemplate +
                      ulCtlvSizeRdArray   + ulCtlvSizeEnd;

    return  ulCtlvSizeTotal;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysSmoWriteRroTmplByRrtd
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hRrtd,
                PVOID                       buffer,
                PULONG                      pulSize
            );

    description:

        This function is called to fill out the template description
        data stream based on the RRT descriptor.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hRrtd
                Specifies the RRT descriptor to examined.

                PVOID                       buffer
                Specifies the buffer to hold the serialized template.

                PULONG                      pulSize
                Specifies the size of serialized template.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysSmoWriteRroTmplByRrtd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRrtd,
        PVOID                       buffer,
        PULONG                      pulSize
    )
{
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;
    PSYS_RRT_DESCRIPTOR             pRrtd                = (PSYS_RRT_DESCRIPTOR)hRrtd;
    PSYS_CTLV_TUPLE                 pCtlvTupleNext       = (PSYS_CTLV_TUPLE    )buffer;
    PSYS_CTLV_TUPLE                 pCtlvTupleName       = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTuplePermission = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleType       = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleValue      = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleAsFolder   = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleContent    = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleFormat     = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleVerify     = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleMinLen     = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleMaxLen     = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleCall       = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleContext    = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleLanguage   = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleCharset    = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleIcon       = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleMapper     = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleEnd        = (PSYS_CTLV_TUPLE    )NULL;
    ULONG                           ulTotalSize          = 0;

    if ( pRrtd->Name )
    {
        pCtlvTupleName = pCtlvTupleNext;

        SysCtlvSetClass    (pCtlvTupleName, SYS_RFT_CTLV_CLASS_RECORD_NAME       );
        SysCtlvSetType     (pCtlvTupleName, SYS_RFT_CTLV_TYPE_RECORD_NAME        );
        SysCtlvSetLength   (pCtlvTupleName, (USHORT)AnscSizeOfString(pRrtd->Name));
        SysCtlvSetValueCstr(pCtlvTupleName, pRrtd->Name                          );

        ulTotalSize   += SysCtlvGetSize     (pCtlvTupleName);
        pCtlvTupleNext = SysCtlvGetNextTuple(pCtlvTupleName);
    }

    if ( TRUE )
    {
        pCtlvTuplePermission = pCtlvTupleNext;

        SysCtlvSetClass     (pCtlvTuplePermission, SYS_RFT_CTLV_CLASS_RECORD_PERMISSION);
        SysCtlvSetType      (pCtlvTuplePermission, SYS_RFT_CTLV_TYPE_RECORD_PERMISSION );
        SysCtlvSetLength    (pCtlvTuplePermission, (USHORT)sizeof(ULONG)               );
        SysCtlvSetValueUlong(pCtlvTuplePermission, pRrtd->Permission                   );

        ulTotalSize   += SysCtlvGetSize     (pCtlvTuplePermission);
        pCtlvTupleNext = SysCtlvGetNextTuple(pCtlvTuplePermission);
    }

    if ( TRUE )
    {
        pCtlvTupleType = pCtlvTupleNext;

        SysCtlvSetClass     (pCtlvTupleType, SYS_RFT_CTLV_CLASS_RECORD_TYPE);
        SysCtlvSetType      (pCtlvTupleType, SYS_RFT_CTLV_TYPE_RECORD_TYPE );
        SysCtlvSetLength    (pCtlvTupleType, (USHORT)sizeof(ULONG)         );
        SysCtlvSetValueUlong(pCtlvTupleType, pRrtd->Type                   );

        ulTotalSize   += SysCtlvGetSize     (pCtlvTupleType);
        pCtlvTupleNext = SysCtlvGetNextTuple(pCtlvTupleType);
    }

    if ( pRrtd->Value )
    {
        pCtlvTupleValue = pCtlvTupleNext;

        SysCtlvSetClass    (pCtlvTupleValue, SYS_RFT_CTLV_CLASS_RECORD_VALUE       );
        SysCtlvSetType     (pCtlvTupleValue, SYS_RFT_CTLV_TYPE_RECORD_VALUE        );
        SysCtlvSetLength   (pCtlvTupleValue, (USHORT)AnscSizeOfString(pRrtd->Value));
        SysCtlvSetValueCstr(pCtlvTupleValue, pRrtd->Value                          );

        ulTotalSize   += SysCtlvGetSize     (pCtlvTupleValue);
        pCtlvTupleNext = SysCtlvGetNextTuple(pCtlvTupleValue);
    }

    if ( TRUE )
    {
        pCtlvTupleAsFolder = pCtlvTupleNext;

        SysCtlvSetClass     (pCtlvTupleAsFolder, SYS_RFT_CTLV_CLASS_RECORD_AS_FOLDER);
        SysCtlvSetType      (pCtlvTupleAsFolder, SYS_RFT_CTLV_TYPE_RECORD_AS_FOLDER );
        SysCtlvSetLength    (pCtlvTupleAsFolder, (USHORT)sizeof(ULONG)              );
        SysCtlvSetValueUlong(pCtlvTupleAsFolder, pRrtd->AsFolder                    );

        ulTotalSize   += SysCtlvGetSize     (pCtlvTupleAsFolder);
        pCtlvTupleNext = SysCtlvGetNextTuple(pCtlvTupleAsFolder);
    }

    if ( TRUE )
    {
        pCtlvTupleContent = pCtlvTupleNext;

        SysCtlvSetClass     (pCtlvTupleContent, SYS_RFT_CTLV_CLASS_RECORD_CONTENT);
        SysCtlvSetType      (pCtlvTupleContent, SYS_RFT_CTLV_TYPE_RECORD_CONTENT );
        SysCtlvSetLength    (pCtlvTupleContent, (USHORT)sizeof(ULONG)            );
        SysCtlvSetValueUlong(pCtlvTupleContent, pRrtd->Content                   );

        ulTotalSize   += SysCtlvGetSize     (pCtlvTupleContent);
        pCtlvTupleNext = SysCtlvGetNextTuple(pCtlvTupleContent);
    }

    if ( TRUE )
    {
        pCtlvTupleFormat = pCtlvTupleNext;

        SysCtlvSetClass     (pCtlvTupleFormat, SYS_RFT_CTLV_CLASS_RECORD_FORMAT);
        SysCtlvSetType      (pCtlvTupleFormat, SYS_RFT_CTLV_TYPE_RECORD_FORMAT );
        SysCtlvSetLength    (pCtlvTupleFormat, (USHORT)sizeof(ULONG)           );
        SysCtlvSetValueUlong(pCtlvTupleFormat, pRrtd->Format                   );

        ulTotalSize   += SysCtlvGetSize     (pCtlvTupleFormat);
        pCtlvTupleNext = SysCtlvGetNextTuple(pCtlvTupleFormat);
    }

    if ( TRUE )
    {
        pCtlvTupleVerify = pCtlvTupleNext;

        SysCtlvSetClass     (pCtlvTupleVerify, SYS_RFT_CTLV_CLASS_RECORD_VERIFY);
        SysCtlvSetType      (pCtlvTupleVerify, SYS_RFT_CTLV_TYPE_RECORD_VERIFY );
        SysCtlvSetLength    (pCtlvTupleVerify, (USHORT)sizeof(ULONG)           );
        SysCtlvSetValueUlong(pCtlvTupleVerify, pRrtd->Verify                   );

        ulTotalSize   += SysCtlvGetSize     (pCtlvTupleVerify);
        pCtlvTupleNext = SysCtlvGetNextTuple(pCtlvTupleVerify);
    }

    if ( TRUE )
    {
        pCtlvTupleMinLen = pCtlvTupleNext;

        SysCtlvSetClass     (pCtlvTupleMinLen, SYS_RFT_CTLV_CLASS_RECORD_MINLEN);
        SysCtlvSetType      (pCtlvTupleMinLen, SYS_RFT_CTLV_TYPE_RECORD_MINLEN );
        SysCtlvSetLength    (pCtlvTupleMinLen, (USHORT)sizeof(ULONG)           );
        SysCtlvSetValueUlong(pCtlvTupleMinLen, pRrtd->MinLen                   );

        ulTotalSize   += SysCtlvGetSize     (pCtlvTupleMinLen);
        pCtlvTupleNext = SysCtlvGetNextTuple(pCtlvTupleMinLen);
    }

    if ( TRUE )
    {
        pCtlvTupleMaxLen = pCtlvTupleNext;

        SysCtlvSetClass     (pCtlvTupleMaxLen, SYS_RFT_CTLV_CLASS_RECORD_MAXLEN);
        SysCtlvSetType      (pCtlvTupleMaxLen, SYS_RFT_CTLV_TYPE_RECORD_MAXLEN );
        SysCtlvSetLength    (pCtlvTupleMaxLen, (USHORT)sizeof(ULONG)           );
        SysCtlvSetValueUlong(pCtlvTupleMaxLen, pRrtd->MaxLen                   );

        ulTotalSize   += SysCtlvGetSize     (pCtlvTupleMaxLen);
        pCtlvTupleNext = SysCtlvGetNextTuple(pCtlvTupleMaxLen);
    }

    if ( TRUE )
    {
        pCtlvTupleCall = pCtlvTupleNext;

        SysCtlvSetClass     (pCtlvTupleCall, SYS_RFT_CTLV_CLASS_RECORD_CALL);
        SysCtlvSetType      (pCtlvTupleCall, SYS_RFT_CTLV_TYPE_RECORD_CALL );
        SysCtlvSetLength    (pCtlvTupleCall, (USHORT)sizeof(ULONG)         );
        SysCtlvSetValueUlong(pCtlvTupleCall, (ULONG )pRrtd->Call           );

        ulTotalSize   += SysCtlvGetSize     (pCtlvTupleCall);
        pCtlvTupleNext = SysCtlvGetNextTuple(pCtlvTupleCall);
    }

    if ( TRUE )
    {
        pCtlvTupleContext = pCtlvTupleNext;

        SysCtlvSetClass     (pCtlvTupleContext, SYS_RFT_CTLV_CLASS_RECORD_CONTEXT);
        SysCtlvSetType      (pCtlvTupleContext, SYS_RFT_CTLV_TYPE_RECORD_CONTEXT );
        SysCtlvSetLength    (pCtlvTupleContext, (USHORT)sizeof(ULONG)            );
        SysCtlvSetValueUlong(pCtlvTupleContext, (ULONG )pRrtd->Context           );

        ulTotalSize   += SysCtlvGetSize     (pCtlvTupleContext);
        pCtlvTupleNext = SysCtlvGetNextTuple(pCtlvTupleContext);
    }

    if ( pRrtd->Language )
    {
        pCtlvTupleLanguage = pCtlvTupleNext;

        SysCtlvSetClass    (pCtlvTupleLanguage, SYS_RFT_CTLV_CLASS_RECORD_LANGUAGE       );
        SysCtlvSetType     (pCtlvTupleLanguage, SYS_RFT_CTLV_TYPE_RECORD_LANGUAGE        );
        SysCtlvSetLength   (pCtlvTupleLanguage, (USHORT)AnscSizeOfString(pRrtd->Language));
        SysCtlvSetValueCstr(pCtlvTupleLanguage, pRrtd->Language                          );

        ulTotalSize   += SysCtlvGetSize     (pCtlvTupleLanguage);
        pCtlvTupleNext = SysCtlvGetNextTuple(pCtlvTupleLanguage);
    }

    if ( pRrtd->Charset )
    {
        pCtlvTupleCharset = pCtlvTupleNext;

        SysCtlvSetClass    (pCtlvTupleCharset, SYS_RFT_CTLV_CLASS_RECORD_CHARSET       );
        SysCtlvSetType     (pCtlvTupleCharset, SYS_RFT_CTLV_TYPE_RECORD_CHARSET        );
        SysCtlvSetLength   (pCtlvTupleCharset, (USHORT)AnscSizeOfString(pRrtd->Charset));
        SysCtlvSetValueCstr(pCtlvTupleCharset, pRrtd->Charset                          );

        ulTotalSize   += SysCtlvGetSize     (pCtlvTupleCharset);
        pCtlvTupleNext = SysCtlvGetNextTuple(pCtlvTupleCharset);
    }

    if ( pRrtd->Icon )
    {
        pCtlvTupleIcon = pCtlvTupleNext;

        SysCtlvSetClass    (pCtlvTupleIcon, SYS_RFT_CTLV_CLASS_RECORD_ICON       );
        SysCtlvSetType     (pCtlvTupleIcon, SYS_RFT_CTLV_TYPE_RECORD_ICON        );
        SysCtlvSetLength   (pCtlvTupleIcon, (USHORT)AnscSizeOfString(pRrtd->Icon));
        SysCtlvSetValueCstr(pCtlvTupleIcon, pRrtd->Icon                          );

        ulTotalSize   += SysCtlvGetSize     (pCtlvTupleIcon);
        pCtlvTupleNext = SysCtlvGetNextTuple(pCtlvTupleIcon);
    }

    if ( pRrtd->Mapper )
    {
        pCtlvTupleMapper = pCtlvTupleNext;

        SysCtlvSetClass    (pCtlvTupleMapper, SYS_RFT_CTLV_CLASS_RECORD_MAPPER       );
        SysCtlvSetType     (pCtlvTupleMapper, SYS_RFT_CTLV_TYPE_RECORD_MAPPER        );
        SysCtlvSetLength   (pCtlvTupleMapper, (USHORT)AnscSizeOfString(pRrtd->Mapper));
        SysCtlvSetValueCstr(pCtlvTupleMapper, pRrtd->Mapper                          );

        ulTotalSize   += SysCtlvGetSize     (pCtlvTupleMapper);
        pCtlvTupleNext = SysCtlvGetNextTuple(pCtlvTupleMapper);
    }

    if ( TRUE )
    {
        pCtlvTupleEnd = pCtlvTupleNext;

        SysCtlvSetClass (pCtlvTupleEnd, SYS_CTLV_CLASS_RESERVED);
        SysCtlvSetType  (pCtlvTupleEnd, SYS_CTLV_TYPE_END      );
        SysCtlvSetLength(pCtlvTupleEnd, 0                      );

        ulTotalSize   += SysCtlvGetSize     (pCtlvTupleEnd);
        pCtlvTupleNext = SysCtlvGetNextTuple(pCtlvTupleEnd);
    }

    *pulSize = ulTotalSize;

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        PVOID
        SysSmoAllocRfoTmplByRftd
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hRftd
            );

    description:

        This function is called to allocate a template description
        data stream based on the RFT descriptor.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hRftd
                Specifies the RFT descriptor to be examined.

    return:     template description stream.

**********************************************************************/

PVOID
SysSmoAllocRfoTmplByRftd
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRftd
    )
{
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;
    PSYS_RFT_DESCRIPTOR             pRftd                = (PSYS_RFT_DESCRIPTOR)hRftd;
    PSYS_CTLV_TUPLE                 pCtlvTupleNext       = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleMoniker    = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTuplePermission = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleType       = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleNamingMtd  = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleContent    = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleLanguage   = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleCharset    = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleIcon       = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleTemplate   = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleRecord     = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleEnd        = (PSYS_CTLV_TUPLE    )NULL;
    ULONG                           ulRecordsSize        = 0;
    ULONG                           ulTotalSize          = 0;
    ULONG                           ulValueSize          = 0;
    void*                           pTmplStream          = NULL;
    ULONG                           ulRrtdIndex          = 0;

    ulTotalSize =
        SysSmoGetRfoTmplSizeByRftd
            (
                hThisObject,
                (ANSC_HANDLE)pRftd
            );

    if ( ulTotalSize == 0 )
    {
        return  NULL;
    }
    else
    {
        pTmplStream = AnscAllocateMemory(ulTotalSize + sizeof(ULONG));
    }

    if ( !pTmplStream )
    {
        return  NULL;
    }
    else
    {
        AnscWriteUlong(pTmplStream, AnscUlongFromHToN(ulTotalSize));

        pCtlvTupleNext = (PSYS_CTLV_TUPLE)((ULONG)pTmplStream + sizeof(ULONG));
        ulTotalSize    = 0;
    }

    if ( pRftd->Moniker )
    {
        pCtlvTupleMoniker = pCtlvTupleNext;

        SysCtlvSetClass    (pCtlvTupleMoniker, SYS_RFT_CTLV_CLASS_FOLDER_MONIKER       );
        SysCtlvSetType     (pCtlvTupleMoniker, SYS_RFT_CTLV_TYPE_FOLDER_MONIKER        );
        SysCtlvSetLength   (pCtlvTupleMoniker, (USHORT)AnscSizeOfString(pRftd->Moniker));
        SysCtlvSetValueCstr(pCtlvTupleMoniker, pRftd->Moniker                          );

        ulTotalSize   += SysCtlvGetSize     (pCtlvTupleMoniker);
        pCtlvTupleNext = SysCtlvGetNextTuple(pCtlvTupleMoniker);
    }

    if ( TRUE )
    {
        pCtlvTuplePermission = pCtlvTupleNext;

        SysCtlvSetClass     (pCtlvTuplePermission, SYS_RFT_CTLV_CLASS_FOLDER_PERMISSION);
        SysCtlvSetType      (pCtlvTuplePermission, SYS_RFT_CTLV_TYPE_FOLDER_PERMISSION );
        SysCtlvSetLength    (pCtlvTuplePermission, (USHORT)sizeof(ULONG)               );
        SysCtlvSetValueUlong(pCtlvTuplePermission, pRftd->Permission                   );

        ulTotalSize   += SysCtlvGetSize     (pCtlvTuplePermission);
        pCtlvTupleNext = SysCtlvGetNextTuple(pCtlvTuplePermission);
    }

    if ( TRUE )
    {
        pCtlvTupleType = pCtlvTupleNext;

        SysCtlvSetClass     (pCtlvTupleType, SYS_RFT_CTLV_CLASS_FOLDER_TYPE);
        SysCtlvSetType      (pCtlvTupleType, SYS_RFT_CTLV_TYPE_FOLDER_TYPE );
        SysCtlvSetLength    (pCtlvTupleType, (USHORT)sizeof(ULONG)         );
        SysCtlvSetValueUlong(pCtlvTupleType, pRftd->Type                   );

        ulTotalSize   += SysCtlvGetSize     (pCtlvTupleType);
        pCtlvTupleNext = SysCtlvGetNextTuple(pCtlvTupleType);
    }

    if ( TRUE )
    {
        pCtlvTupleNamingMtd = pCtlvTupleNext;

        SysCtlvSetClass     (pCtlvTupleNamingMtd, SYS_RFT_CTLV_CLASS_FOLDER_NAMING);
        SysCtlvSetType      (pCtlvTupleNamingMtd, SYS_RFT_CTLV_TYPE_FOLDER_NAMING );
        SysCtlvSetLength    (pCtlvTupleNamingMtd, (USHORT)sizeof(ULONG)           );
        SysCtlvSetValueUlong(pCtlvTupleNamingMtd, pRftd->NamingMtd                );

        ulTotalSize   += SysCtlvGetSize     (pCtlvTupleNamingMtd);
        pCtlvTupleNext = SysCtlvGetNextTuple(pCtlvTupleNamingMtd);
    }

    if ( TRUE )
    {
        pCtlvTupleContent = pCtlvTupleNext;

        SysCtlvSetClass     (pCtlvTupleContent, SYS_RFT_CTLV_CLASS_FOLDER_CONTENT);
        SysCtlvSetType      (pCtlvTupleContent, SYS_RFT_CTLV_TYPE_FOLDER_CONTENT );
        SysCtlvSetLength    (pCtlvTupleContent, (USHORT)sizeof(ULONG)            );
        SysCtlvSetValueUlong(pCtlvTupleContent, pRftd->Content                   );

        ulTotalSize   += SysCtlvGetSize     (pCtlvTupleContent);
        pCtlvTupleNext = SysCtlvGetNextTuple(pCtlvTupleContent);
    }

    if ( pRftd->Language )
    {
        pCtlvTupleLanguage = pCtlvTupleNext;

        SysCtlvSetClass    (pCtlvTupleLanguage, SYS_RFT_CTLV_CLASS_FOLDER_LANGUAGE       );
        SysCtlvSetType     (pCtlvTupleLanguage, SYS_RFT_CTLV_TYPE_FOLDER_LANGUAGE        );
        SysCtlvSetLength   (pCtlvTupleLanguage, (USHORT)AnscSizeOfString(pRftd->Language));
        SysCtlvSetValueCstr(pCtlvTupleLanguage, pRftd->Language                          );

        ulTotalSize   += SysCtlvGetSize     (pCtlvTupleLanguage);
        pCtlvTupleNext = SysCtlvGetNextTuple(pCtlvTupleLanguage);
    }

    if ( pRftd->Charset )
    {
        pCtlvTupleCharset = pCtlvTupleNext;

        SysCtlvSetClass    (pCtlvTupleCharset, SYS_RFT_CTLV_CLASS_FOLDER_CHARSET       );
        SysCtlvSetType     (pCtlvTupleCharset, SYS_RFT_CTLV_TYPE_FOLDER_CHARSET        );
        SysCtlvSetLength   (pCtlvTupleCharset, (USHORT)AnscSizeOfString(pRftd->Charset));
        SysCtlvSetValueCstr(pCtlvTupleCharset, pRftd->Charset                          );

        ulTotalSize   += SysCtlvGetSize     (pCtlvTupleCharset);
        pCtlvTupleNext = SysCtlvGetNextTuple(pCtlvTupleCharset);
    }

    if ( pRftd->Icon )
    {
        pCtlvTupleIcon = pCtlvTupleNext;

        SysCtlvSetClass    (pCtlvTupleIcon, SYS_RFT_CTLV_CLASS_FOLDER_ICON       );
        SysCtlvSetType     (pCtlvTupleIcon, SYS_RFT_CTLV_TYPE_FOLDER_ICON        );
        SysCtlvSetLength   (pCtlvTupleIcon, (USHORT)AnscSizeOfString(pRftd->Icon));
        SysCtlvSetValueCstr(pCtlvTupleIcon, pRftd->Icon                          );

        ulTotalSize   += SysCtlvGetSize     (pCtlvTupleIcon);
        pCtlvTupleNext = SysCtlvGetNextTuple(pCtlvTupleIcon);
    }

    if ( pRftd->Template )
    {
        pCtlvTupleTemplate = pCtlvTupleNext;

        SysCtlvSetClass    (pCtlvTupleTemplate, SYS_RFT_CTLV_CLASS_FOLDER_TEMPLATE       );
        SysCtlvSetType     (pCtlvTupleTemplate, SYS_RFT_CTLV_TYPE_FOLDER_TEMPLATE        );
        SysCtlvSetLength   (pCtlvTupleTemplate, (USHORT)AnscSizeOfString(pRftd->Template));
        SysCtlvSetValueCstr(pCtlvTupleTemplate, pRftd->Template                          );

        ulTotalSize   += SysCtlvGetSize     (pCtlvTupleTemplate);
        pCtlvTupleNext = SysCtlvGetNextTuple(pCtlvTupleTemplate);
    }

    while ( pRftd->RrtdArray[ulRrtdIndex].Name != NULL )
    {
        pCtlvTupleRecord = pCtlvTupleNext;

        SysCtlvSetClass (pCtlvTupleRecord, SYS_RFT_CTLV_CLASS_FOLDER_RECORD);
        SysCtlvSetType  (pCtlvTupleRecord, SYS_RFT_CTLV_TYPE_FOLDER_RECORD );
        SysCtlvSetLength(pCtlvTupleRecord, 0                               );

        ulValueSize  = 0;
        returnStatus =
            SysSmoWriteRroTmplByRrtd
                (
                    hThisObject,
                    (ANSC_HANDLE)&pRftd->RrtdArray[ulRrtdIndex],
                    (PVOID      )SysCtlvGetValue(pCtlvTupleRecord),
                    &ulValueSize
                );

        SysCtlvSetLength(pCtlvTupleRecord, (USHORT)ulValueSize);

        ulRrtdIndex++;

        ulTotalSize   += SysCtlvGetSize     (pCtlvTupleRecord);
        pCtlvTupleNext = SysCtlvGetNextTuple(pCtlvTupleRecord);
    }

    if ( TRUE )
    {
        pCtlvTupleEnd = pCtlvTupleNext;

        SysCtlvSetClass (pCtlvTupleEnd, SYS_CTLV_CLASS_RESERVED);
        SysCtlvSetType  (pCtlvTupleEnd, SYS_CTLV_TYPE_END      );
        SysCtlvSetLength(pCtlvTupleEnd, 0                      );

        ulTotalSize   += SysCtlvGetSize     (pCtlvTupleEnd);
        pCtlvTupleNext = SysCtlvGetNextTuple(pCtlvTupleEnd);
    }

    return  pTmplStream;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        SysSmoAllocRftdByRfoTmpl
            (
                ANSC_HANDLE                 hThisObject,
                PVOID                       pRfoTmpl
            );

    description:

        This function is called to allocate a RFT descriptor based on
        the template description data stream.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID                       pRfoTmpl
                Specifies the template descripion data to be examined.

    return:     RFT descriptor.

**********************************************************************/

ANSC_HANDLE
SysSmoAllocRftdByRfoTmpl
    (
        ANSC_HANDLE                 hThisObject,
        PVOID                       pRfoTmpl
    )
{
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;
    PSYS_RFT_DESCRIPTOR             pRftd                = (PSYS_RFT_DESCRIPTOR)NULL;
    PSYS_RRT_DESCRIPTOR             pRrtd                = (PSYS_RRT_DESCRIPTOR)NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleNext       = (PSYS_CTLV_TUPLE    )((ULONG)pRfoTmpl + sizeof(ULONG));
    PSYS_CTLV_TUPLE                 pCtlvTupleMoniker    = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTuplePermission = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleType       = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleNamingMtd  = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleContent    = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleLanguage   = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleCharset    = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleIcon       = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleTemplate   = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleRecord     = (PSYS_CTLV_TUPLE    )NULL;
    ULONG                           ulTotalSize          = AnscUlongFromNToH(AnscReadUlong(pRfoTmpl));
    ULONG                           ulRrtdCount          = 0;
    ULONG                           ulRrtdIndex          = 0;
    ULONG                           i                    = 0;

    if ( ulTotalSize <= sizeof(SYS_CTLV_TUPLE) )
    {
        return  (ANSC_HANDLE)NULL;
    }
    else
    {
        pRftd = (PSYS_RFT_DESCRIPTOR)AnscAllocateMemory(sizeof(SYS_RFT_DESCRIPTOR));

        if ( !pRftd )
        {
            return  (ANSC_HANDLE)NULL;
        }
        else
        {
            SysInitRftd(pRftd);
        }
    }

    SysCtlvGetTupleCount
        (
            pCtlvTupleNext,
            SYS_RFT_CTLV_TYPE_FOLDER_RECORD,
            ulRrtdCount
        );

    if ( ulRrtdCount > 0 )
    {
        ulRrtdCount += 1;
        pRrtd        = (PSYS_RRT_DESCRIPTOR)AnscAllocateMemory(sizeof(SYS_RRT_DESCRIPTOR) * ulRrtdCount);

        if ( !pRrtd )
        {
            AnscFreeMemory(pRftd);

            return  (ANSC_HANDLE)NULL;
        }
        else
        {
            pRftd->RrtdArray = pRrtd;
        }

        for ( i = 0; i < ulRrtdCount; i++ )
        {
            SysInitRrtd((&pRrtd[i]));
        }
    }

    while ( pCtlvTupleNext && (SysCtlvGetType(pCtlvTupleNext) != SYS_CTLV_TYPE_END) )
    {
        switch ( SysCtlvGetType(pCtlvTupleNext) )
        {
            case    SYS_RFT_CTLV_TYPE_FOLDER_MONIKER :

                    pCtlvTupleMoniker = pCtlvTupleNext;

                    if ( TRUE )
                    {
                        pRftd->Moniker = AnscAllocateMemory(SysCtlvGetLength(pCtlvTupleMoniker) + 1);

                        if ( pRftd->Moniker )
                        {
                            SysCtlvCopyValue(pCtlvTupleMoniker, pRftd->Moniker);
                        }
                    }

                    break;

            case    SYS_RFT_CTLV_TYPE_FOLDER_PERMISSION :

                    pCtlvTuplePermission = pCtlvTupleNext;

                    if ( TRUE )
                    {
                        pRftd->Permission = SysCtlvGetValueUlong(pCtlvTuplePermission);
                    }

                    break;

            case    SYS_RFT_CTLV_TYPE_FOLDER_TYPE :

                    pCtlvTupleType = pCtlvTupleNext;

                    if ( TRUE )
                    {
                        pRftd->Type = SysCtlvGetValueUlong(pCtlvTupleType);
                    }

                    break;

            case    SYS_RFT_CTLV_TYPE_FOLDER_NAMING :

                    pCtlvTupleNamingMtd = pCtlvTupleNext;

                    if ( TRUE )
                    {
                        pRftd->NamingMtd = SysCtlvGetValueUlong(pCtlvTupleNamingMtd);
                    }

                    break;

            case    SYS_RFT_CTLV_TYPE_FOLDER_CONTENT :

                    pCtlvTupleContent = pCtlvTupleNext;

                    if ( TRUE )
                    {
                        pRftd->Content = SysCtlvGetValueUlong(pCtlvTupleContent);
                    }

                    break;

            case    SYS_RFT_CTLV_TYPE_FOLDER_LANGUAGE :

                    pCtlvTupleLanguage = pCtlvTupleNext;

                    if ( TRUE )
                    {
                        pRftd->Language = AnscAllocateMemory(SysCtlvGetLength(pCtlvTupleLanguage) + 1);

                        if ( pRftd->Language )
                        {
                            SysCtlvCopyValue(pCtlvTupleLanguage, pRftd->Language);
                        }
                    }

                    break;

            case    SYS_RFT_CTLV_TYPE_FOLDER_CHARSET :

                    pCtlvTupleCharset = pCtlvTupleNext;

                    if ( TRUE )
                    {
                        pRftd->Charset = AnscAllocateMemory(SysCtlvGetLength(pCtlvTupleCharset) + 1);

                        if ( pRftd->Charset )
                        {
                            SysCtlvCopyValue(pCtlvTupleCharset, pRftd->Charset);
                        }
                    }

                    break;

            case    SYS_RFT_CTLV_TYPE_FOLDER_ICON :

                    pCtlvTupleIcon = pCtlvTupleNext;

                    if ( TRUE )
                    {
                        pRftd->Icon = AnscAllocateMemory(SysCtlvGetLength(pCtlvTupleIcon) + 1);

                        if ( pRftd->Icon )
                        {
                            SysCtlvCopyValue(pCtlvTupleIcon, pRftd->Icon);
                        }
                    }

                    break;

            case    SYS_RFT_CTLV_TYPE_FOLDER_TEMPLATE :

                    pCtlvTupleTemplate = pCtlvTupleNext;

                    if ( TRUE )
                    {
                        pRftd->Template = AnscAllocateMemory(SysCtlvGetLength(pCtlvTupleTemplate) + 1);

                        if ( pRftd->Template )
                        {
                            SysCtlvCopyValue(pCtlvTupleTemplate, pRftd->Template);
                        }
                    }

                    break;

            case    SYS_RFT_CTLV_TYPE_FOLDER_RECORD :

                    pCtlvTupleRecord = pCtlvTupleNext;
                    returnStatus     =
                        SysSmoWriteRrtdByRroTmpl
                            (
                                hThisObject,
                                (ANSC_HANDLE)&pRrtd[ulRrtdIndex++],
                                (PVOID      )SysCtlvGetValue (pCtlvTupleRecord),
                                (ULONG      )SysCtlvGetLength(pCtlvTupleRecord)
                            );

                    break;

            case    SYS_CTLV_TYPE_END :

                    break;

            default :

                    break;
        }

        pCtlvTupleNext = SysCtlvGetNextTuple(pCtlvTupleNext);
    }

    return  (ANSC_HANDLE)pRftd;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysSmoWriteRrtdByRroTmpl
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hRrtd,
                PVOID                       pRroTmpl,
                ULONG                       ulSize
            );

    description:

        This function is called to fill out a RRT descriptor based on
        the template description data stream.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hRrtd
                Specifies the RRT descriptor to be filled out.

                PVOID                       pRroTmpl
                Specifies the template descripion data to be examined.

                ULONG                       ulSize
                Specifies the size of the template description data.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysSmoWriteRrtdByRroTmpl
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRrtd,
        PVOID                       pRroTmpl,
        ULONG                       ulSize
    )
{
    ANSC_STATUS                     returnStatus         = ANSC_STATUS_SUCCESS;
    PSYS_RRT_DESCRIPTOR             pRrtd                = (PSYS_RRT_DESCRIPTOR)hRrtd;
    PSYS_CTLV_TUPLE                 pCtlvTupleNext       = (PSYS_CTLV_TUPLE    )pRroTmpl;
    PSYS_CTLV_TUPLE                 pCtlvTupleName       = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTuplePermission = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleType       = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleValue      = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleAsFolder   = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleContent    = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleFormat     = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleVerify     = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleMinLen     = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleMaxLen     = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleCall       = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleContext    = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleLanguage   = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleCharset    = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleIcon       = (PSYS_CTLV_TUPLE    )NULL;
    PSYS_CTLV_TUPLE                 pCtlvTupleMapper     = (PSYS_CTLV_TUPLE    )NULL;

    SysCtlvGetTupleByType(pCtlvTupleNext, SYS_RFT_CTLV_TYPE_RECORD_NAME,       pCtlvTupleName      );
    SysCtlvGetTupleByType(pCtlvTupleNext, SYS_RFT_CTLV_TYPE_RECORD_PERMISSION, pCtlvTuplePermission);
    SysCtlvGetTupleByType(pCtlvTupleNext, SYS_RFT_CTLV_TYPE_RECORD_TYPE,       pCtlvTupleType      );
    SysCtlvGetTupleByType(pCtlvTupleNext, SYS_RFT_CTLV_TYPE_RECORD_VALUE,      pCtlvTupleValue     );
    SysCtlvGetTupleByType(pCtlvTupleNext, SYS_RFT_CTLV_TYPE_RECORD_AS_FOLDER,  pCtlvTupleAsFolder  );
    SysCtlvGetTupleByType(pCtlvTupleNext, SYS_RFT_CTLV_TYPE_RECORD_CONTENT,    pCtlvTupleContent   );
    SysCtlvGetTupleByType(pCtlvTupleNext, SYS_RFT_CTLV_TYPE_RECORD_FORMAT,     pCtlvTupleFormat    );
    SysCtlvGetTupleByType(pCtlvTupleNext, SYS_RFT_CTLV_TYPE_RECORD_VERIFY,     pCtlvTupleVerify    );
    SysCtlvGetTupleByType(pCtlvTupleNext, SYS_RFT_CTLV_TYPE_RECORD_MINLEN,     pCtlvTupleMinLen    );
    SysCtlvGetTupleByType(pCtlvTupleNext, SYS_RFT_CTLV_TYPE_RECORD_MAXLEN,     pCtlvTupleMaxLen    );
    SysCtlvGetTupleByType(pCtlvTupleNext, SYS_RFT_CTLV_TYPE_RECORD_CALL,       pCtlvTupleCall      );
    SysCtlvGetTupleByType(pCtlvTupleNext, SYS_RFT_CTLV_TYPE_RECORD_CONTEXT,    pCtlvTupleContext   );
    SysCtlvGetTupleByType(pCtlvTupleNext, SYS_RFT_CTLV_TYPE_RECORD_LANGUAGE,   pCtlvTupleLanguage  );
    SysCtlvGetTupleByType(pCtlvTupleNext, SYS_RFT_CTLV_TYPE_RECORD_CHARSET,    pCtlvTupleCharset   );
    SysCtlvGetTupleByType(pCtlvTupleNext, SYS_RFT_CTLV_TYPE_RECORD_ICON,       pCtlvTupleIcon      );
    SysCtlvGetTupleByType(pCtlvTupleNext, SYS_RFT_CTLV_TYPE_RECORD_MAPPER,     pCtlvTupleMapper    );

    if ( pCtlvTupleName )
    {
        pRrtd->Name = AnscAllocateMemory(SysCtlvGetLength(pCtlvTupleName) + 1);

        if ( pRrtd->Name )
        {
            SysCtlvCopyValue(pCtlvTupleName, pRrtd->Name);
        }
    }

    if ( pCtlvTuplePermission )
    {
        pRrtd->Permission = SysCtlvGetValueUlong(pCtlvTuplePermission);
    }

    if ( pCtlvTupleType )
    {
        pRrtd->Type = SysCtlvGetValueUlong(pCtlvTupleType);
    }

    if ( pCtlvTupleValue )
    {
        pRrtd->Value = AnscAllocateMemory(SysCtlvGetLength(pCtlvTupleValue) + 1);

        if ( pRrtd->Value )
        {
            SysCtlvCopyValue(pCtlvTupleValue, pRrtd->Value);
        }
    }

    if ( pCtlvTupleAsFolder )
    {
        pRrtd->AsFolder = SysCtlvGetValueUlong(pCtlvTupleAsFolder);
    }

    if ( pCtlvTupleContent )
    {
        pRrtd->Content = SysCtlvGetValueUlong(pCtlvTupleContent);
    }

    if ( pCtlvTupleFormat )
    {
        pRrtd->Format = SysCtlvGetValueUlong(pCtlvTupleFormat);
    }

    if ( pCtlvTupleVerify )
    {
        pRrtd->Verify = SysCtlvGetValueUlong(pCtlvTupleVerify);
    }

    if ( pCtlvTupleMinLen )
    {
        pRrtd->MinLen = SysCtlvGetValueUlong(pCtlvTupleMinLen);
    }

    if ( pCtlvTupleMaxLen )
    {
        pRrtd->MaxLen = SysCtlvGetValueUlong(pCtlvTupleMaxLen);
    }

    if ( pCtlvTupleCall )
    {
        pRrtd->Call = (PFN_SYS_RRO_ACTION_CALL)SysCtlvGetValueUlong(pCtlvTupleCall);
    }

    if ( pCtlvTupleContext )
    {
        pRrtd->Context = (ANSC_HANDLE)SysCtlvGetValueUlong(pCtlvTupleContext);
    }

    if ( pCtlvTupleLanguage )
    {
        pRrtd->Language = AnscAllocateMemory(SysCtlvGetLength(pCtlvTupleLanguage) + 1);

        if ( pRrtd->Language )
        {
            SysCtlvCopyValue(pCtlvTupleLanguage, pRrtd->Language);
        }
    }

    if ( pCtlvTupleCharset )
    {
        pRrtd->Charset = AnscAllocateMemory(SysCtlvGetLength(pCtlvTupleCharset) + 1);

        if ( pRrtd->Charset )
        {
            SysCtlvCopyValue(pCtlvTupleCharset, pRrtd->Charset);
        }
    }

    if ( pCtlvTupleIcon )
    {
        pRrtd->Icon = AnscAllocateMemory(SysCtlvGetLength(pCtlvTupleIcon) + 1);

        if ( pRrtd->Icon )
        {
            SysCtlvCopyValue(pCtlvTupleIcon, pRrtd->Icon);
        }
    }

    if ( pCtlvTupleMapper )
    {
        pRrtd->Mapper = AnscAllocateMemory(SysCtlvGetLength(pCtlvTupleMapper) + 1);

        if ( pRrtd->Mapper )
        {
            SysCtlvCopyValue(pCtlvTupleMapper, pRrtd->Mapper);
        }
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysSmoCleanRftDescriptor
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hRftd
            );

    description:

        This function is called to clean up a RFT descriptor.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hRftd
                Specifies the RFT descriptor to be cleaned.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysSmoCleanRftDescriptor
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRftd
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_RFT_DESCRIPTOR             pRftd        = (PSYS_RFT_DESCRIPTOR)hRftd;
    ULONG                           ulRrtdIndex  = 0;

    if ( pRftd->Moniker )
    {
        AnscFreeMemory(pRftd->Moniker);

        pRftd->Moniker = NULL;
    }

    if ( pRftd->Language )
    {
        AnscFreeMemory(pRftd->Language);

        pRftd->Language = NULL;
    }

    if ( pRftd->Charset )
    {
        AnscFreeMemory(pRftd->Charset);

        pRftd->Charset = NULL;
    }

    if ( pRftd->Icon )
    {
        AnscFreeMemory(pRftd->Icon);

        pRftd->Icon = NULL;
    }

    if ( pRftd->Template )
    {
        AnscFreeMemory(pRftd->Template);

        pRftd->Template = NULL;
    }

    if ( pRftd->RrtdArray )
    {
        while ( pRftd->RrtdArray[ulRrtdIndex].Name != NULL )
        {
            returnStatus =
                SysSmoCleanRrtDescriptor
                    (
                        hThisObject,
                        (ANSC_HANDLE)&pRftd->RrtdArray[ulRrtdIndex]
                    );

            ulRrtdIndex++;
        }

        AnscFreeMemory(pRftd->RrtdArray);

        pRftd->RrtdArray = NULL;
    }

    return  ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        SysSmoCleanRrtDescriptor
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hRrtd
            );

    description:

        This function is called to clean up a RRT descriptor.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hRrtd
                Specifies the RRT descriptor to be cleaned.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
SysSmoCleanRrtDescriptor
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hRrtd
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PSYS_RRT_DESCRIPTOR             pRrtd        = (PSYS_RRT_DESCRIPTOR)hRrtd;

    if ( pRrtd->Name )
    {
        AnscFreeMemory(pRrtd->Name);

        pRrtd->Name = NULL;
    }

    if ( pRrtd->Value )
    {
        AnscFreeMemory(pRrtd->Value);

        pRrtd->Value = NULL;
    }

    if ( pRrtd->Language )
    {
        AnscFreeMemory(pRrtd->Language);

        pRrtd->Language = NULL;
    }

    if ( pRrtd->Charset )
    {
        AnscFreeMemory(pRrtd->Charset);

        pRrtd->Charset = NULL;
    }

    if ( pRrtd->Icon )
    {
        AnscFreeMemory(pRrtd->Icon);

        pRrtd->Icon = NULL;
    }

    if ( pRrtd->Mapper )
    {
        AnscFreeMemory(pRrtd->Mapper);

        pRrtd->Mapper = NULL;
    }

    return  ANSC_STATUS_SUCCESS;
}
