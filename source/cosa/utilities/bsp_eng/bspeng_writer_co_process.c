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

    module:	bspeng_writer_co_process.c

        For BSP Engine Writer Component Object,
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This module implements the advanced operation functions
        of the BSP engine writer Object.

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
#include "safec_lib_common.h"


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateWriterWriteBytes
            (
                ANSC_HANDLE                 hThisObject,
                const char                  *pBuf,
                PULONG                      pCount
            )

    description:

        This function is called to write byte stream.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to this object.

                const char                  *pBuf
                Byte stream buffer.

                PULONG                      pCount
                The number of bytes to be written.

    return:     status of operation.

**********************************************************************/

BOOL
BspTemplateWriterWriteBytes
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pBuf,
        PULONG                      pCount
    )
{
    /* 
     * The "WriteBytes" function of TEMPLATE_WRITER should not be called.
     */
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(pBuf);
    UNREFERENCED_PARAMETER(pCount);
    return FALSE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        BspTemplateWriterWriteString
            (
                ANSC_HANDLE                 hThisObject,
                const char                  *pString
            )

    description:

        This function is called to output given string.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to this object.

                const char                  *pString
                String to be output.

    return:     status of operation.

**********************************************************************/

BOOL
BspTemplateWriterWriteString
    (
        ANSC_HANDLE                 hThisObject,
        const char                  *pString
    )
{
    PBSP_TEMPLATE_WRITER_OBJECT     pMyObject = (PBSP_TEMPLATE_WRITER_OBJECT)hThisObject;
    ULONG                           ulLen;

    if (!pString)
        return TRUE;

    ulLen   = AnscSizeOfString(pString);

    return pMyObject->WriteBytes(hThisObject, pString, &ulLen);
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateWriterWriteChar
            (
                ANSC_HANDLE                 hThisObject,
                char                        ch
            )

    description:

        This function is called to output given character.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to this object.

                char                        ch
                Character to be output.

    return:     handle to this object.

**********************************************************************/

ANSC_HANDLE
BspTemplateWriterWriteChar
    (
        ANSC_HANDLE                 hThisObject,
        char                        ch
    )
{
    PBSP_TEMPLATE_WRITER_OBJECT     pMyObject = (PBSP_TEMPLATE_WRITER_OBJECT)hThisObject;
    ULONG                           ulLen = 1;

    pMyObject->WriteBytes(hThisObject, &ch, &ulLen);

    return hThisObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateWriterWriteLong
            (
                ANSC_HANDLE                 hThisObject,
                LONG                        lNum
            )

    description:

        This function is called to output given LONG number.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to this object.

                LONG                        lNum
                LONG number to be output.

    return:     status of operation.

**********************************************************************/

ANSC_HANDLE
BspTemplateWriterWriteLong
    (
        ANSC_HANDLE                 hThisObject,
        LONG                        lNum
    )
{
    PBSP_TEMPLATE_WRITER_OBJECT     pMyObject = (PBSP_TEMPLATE_WRITER_OBJECT)hThisObject;
    char                            buf[16];
    ULONG                           ulLen;
    errno_t                         rc        = -1;

    rc = sprintf_s(buf,sizeof(buf), "%ld", lNum);
    if(rc < EOK){
        ERR_CHK(rc);
    }
    ulLen = rc;

    pMyObject->WriteBytes(hThisObject, buf, &ulLen);

    return hThisObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateWriterWriteUlong
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulNum
            )

    description:

        This function is called to output given ULONG number.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to this object.

                ULONG                       ulNum
                ULONG number to be output.

    return:     handle to this object.

**********************************************************************/

ANSC_HANDLE
BspTemplateWriterWriteUlong
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulNum
    )
{
    PBSP_TEMPLATE_WRITER_OBJECT     pMyObject = (PBSP_TEMPLATE_WRITER_OBJECT)hThisObject;
    char                            buf[16];
    ULONG                           ulLen;
    errno_t                         rc = -1;

    rc = sprintf_s(buf,sizeof(buf), "%lu", ulNum);
    if(rc < EOK){
        ERR_CHK(rc);
    }
    ulLen = rc;

    pMyObject->WriteBytes(hThisObject, buf, &ulLen);

    return hThisObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateWriterWriteInt
            (
                ANSC_HANDLE                 hThisObject,
                int                         nNum
            )

    description:

        This function is called to output given INT number.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to this object.

                int                         nNum
                INT number to be output.

    return:     handle to this object.

**********************************************************************/

ANSC_HANDLE
BspTemplateWriterWriteInt
    (
        ANSC_HANDLE                 hThisObject,
        int                         nNum
    )
{
    PBSP_TEMPLATE_WRITER_OBJECT     pMyObject = (PBSP_TEMPLATE_WRITER_OBJECT)hThisObject;
    char                            buf[16];
    ULONG                           ulLen;
    errno_t                         rc = -1;

    rc = sprintf_s(buf,sizeof(buf), "%d", nNum);
    if(rc < EOK){
        ERR_CHK(rc);
    }
    ulLen = rc;

    pMyObject->WriteBytes(hThisObject, buf, &ulLen);

    return hThisObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateWriterWriteUint
            (
                ANSC_HANDLE                 hThisObject,
                UINT                        uNum
            )

    description:

        This function is called to output given UINT number.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to this object.

                UINT                        uNum
                UINT number to be output.

    return:     status of operation.

**********************************************************************/

ANSC_HANDLE
BspTemplateWriterWriteUint
    (
        ANSC_HANDLE                 hThisObject,
        UINT                        uNum
    )
{
    PBSP_TEMPLATE_WRITER_OBJECT     pMyObject = (PBSP_TEMPLATE_WRITER_OBJECT)hThisObject;
    char                            buf[16];
    ULONG                           ulLen;
    errno_t                         rc = -1;

    rc = sprintf_s(buf,sizeof(buf), "%u", uNum);
    if(rc < EOK){
        ERR_CHK(rc);
    }
    ulLen = rc;

    pMyObject->WriteBytes(hThisObject, buf, &ulLen);

    return hThisObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateWriterWriteDouble
            (
                ANSC_HANDLE                 hThisObject,
                double                      Value
            )

    description:

        This function is called to output given string.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to this object.

                BSP_TEMPLATE_DOUBLE         Value
                double                      Value
                Double value to be output.

    return:     status of operation.

**********************************************************************/

ANSC_HANDLE
BspTemplateWriterWriteDouble
    (
        ANSC_HANDLE                 hThisObject,
#ifdef   _BSPENG_NO_DOUBLE
        BSP_TEMPLATE_DOUBLE         Value
#else
        double                      Value
#endif
    )
{
    PBSP_TEMPLATE_WRITER_OBJECT     pMyObject = (PBSP_TEMPLATE_WRITER_OBJECT)hThisObject;
    char                            buf[16];
    ULONG                           ulLen;
    errno_t                         rc = -1;

#ifdef   _BSPENG_NO_DOUBLE
    LONG                            lInt;
    ULONG                           ulFrac;

    lInt    = BSP_TEMPLATE_DOUBLE_GET_INT_TRUNK(Value);
    ulFrac  = BSP_TEMPLATE_DOUBLE_GET_FRAC(Value);

    if (ulFrac == 0){
        rc = sprintf_s(buf,sizeof(buf), "%d", (int)lInt);
        if(rc < EOK){
            ERR_CHK(rc);
        }
        ulLen   = rc;
    }
    else{
        rc = sprintf_s(buf,sizeof(buf), BSP_TEMPLATE_DOUBLE_FORMAT, (int)lInt, (int)ulFrac);
        if(rc < EOK){
            ERR_CHK(rc);
        }
        ulLen   = rc;
    }
#else
    rc = sprintf_s(buf,sizeof(buf), "%f", Value);
    if(rc < EOK){
        ERR_CHK(rc);
    }
    ulLen   = rc;
#endif

    pMyObject->WriteBytes(hThisObject, buf, &ulLen);
    
    return hThisObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateWriterWriteHex
            (
                ANSC_HANDLE                 hThisObject,
                ULONG                       ulNum
            )

    description:

        This function is called to output given HEX number.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to this object.

                ULONG                       ulNum
                HEX number to be output.

    return:     handle to this object.

**********************************************************************/

ANSC_HANDLE
BspTemplateWriterWriteHex
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       ulNum
    )
{
    PBSP_TEMPLATE_WRITER_OBJECT     pMyObject = (PBSP_TEMPLATE_WRITER_OBJECT)hThisObject;
    char                            buf[16];
    ULONG                           ulLen;
    errno_t                         rc = -1;

    rc = sprintf_s(buf,sizeof(buf), "0x%.8x", (UINT)ulNum);
    if(rc < EOK){
        ERR_CHK(rc);
    }
    ulLen = rc;

    pMyObject->WriteBytes(hThisObject, buf, &ulLen);

    return hThisObject;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE
        BspTemplateWriterWrite
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hVar
            )

    description:

        This function is called to output given variable.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to this object.

                ANSC_HANDLE                 hVar
                Variable to be output.

    return:     handle to this object.

**********************************************************************/

ANSC_HANDLE
BspTemplateWriterWrite
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hVar
    )
{
    PBSP_TEMPLATE_WRITER_OBJECT     pMyObject = (PBSP_TEMPLATE_WRITER_OBJECT)hThisObject;
    PBSP_TEMPLATE_VAR_OBJECT        pVar      = (PBSP_TEMPLATE_VAR_OBJECT)hVar;

    switch (pVar->Type)
    {
    case BspVar_String:
        if (pVar->Size)
        {
            ULONG                   ulLen   = pVar->Size;
            char                    *pStr   = (char *)pVar->Value.str;

            if (pStr[ulLen - 1] == 0)
            {
                /* don't output NULL character */
                pMyObject->WriteString(hThisObject, pVar->Value.str);
            }
            else
            {
                pMyObject->WriteBytes(hThisObject, pVar->Value.str, &ulLen);
            }
        }
        else
        {
            pMyObject->WriteString(hThisObject, pVar->Value.str);
        }
        break;

    case BspVar_Number:
        pMyObject->WriteInt(hThisObject, pVar->Value.num);
        break;

    case BspVar_UNumber:
        pMyObject->WriteUlong(hThisObject, pVar->Value.num);
        break;

    case BspVar_Real:
        pMyObject->WriteDouble(hThisObject, pVar->Value.real);
        break;

    case BspVar_Object:
        pMyObject->WriteHex(hThisObject, (ULONG)pVar->Value.obj);
        break;

    default:
        pMyObject->WriteString(hThisObject, "Can't write content of given var\r\n");
        break;
    }

    return hThisObject;
}
    

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BspTemplateWriterOpenExternal
            (
                ANSC_HANDLE                 hThisObject,
                char                        *pName
            )

    description:

        This function is called to open destination resource for
        output.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to this object.

                char                        *pName
                The name of destination resource.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BspTemplateWriterOpenExternal
    (
        ANSC_HANDLE                 hThisObject,
        char                        *pName
    )
{
    /* 
     * The 'OpenExternal' of base object 'Writer' should not be called.
     */
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(pName);
    return ANSC_STATUS_FAILURE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        BspTemplateWriterOpenInternal
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to open internal resource for output.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to this object.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
BspTemplateWriterOpenInternal
    (
        ANSC_HANDLE                 hThisObject
    )
{
    /* 
     * The 'OpenInternal' of base object 'Writer' should not be called.
     */
    UNREFERENCED_PARAMETER(hThisObject);
    return ANSC_STATUS_FAILURE;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        void
        BspTemplateWriterClose
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function is called to close the writer object.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to this object.

    return:     void.

**********************************************************************/

void
BspTemplateWriterClose
    (
        ANSC_HANDLE                 hThisObject
    )
{
    UNREFERENCED_PARAMETER(hThisObject);
    /* 
     * The 'Close' of base object 'Writer' should not be called.
     */
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        char *
        BspTemplateWriterGetContent
            (
                ANSC_HANDLE                 hThisObject,
                PULONG                      pulContentSize
            )

    description:

        This function is called to get output buffer.

    argument:   ANSC_HANDLE                 hThisObject
                Handle to this object.

                PULONG                      pulContentSize
                The size of output content buffer.

    return:     the content buffer.

**********************************************************************/

char *
BspTemplateWriterGetContent
    (
        ANSC_HANDLE                 hThisObject,
        PULONG                      pulContentSize
    )
{
    /* 
     * The 'GetContent' of base object 'Writer' should not be called.
     */
    UNREFERENCED_PARAMETER(hThisObject);
    UNREFERENCED_PARAMETER(pulContentSize);
    return NULL;
}



