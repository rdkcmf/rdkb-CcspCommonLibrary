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

    module: ansc_asn1_utils.h

        For Abstract Syntax Notation One (ASN.1)
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        Utility functions for ASN1 implementation

        *   IsObjectChoiceType
        *   IsObjectStringType
        *   IsObjectPrimitiveType
        *   IsObjectContextType
        *   IsObjectUserDefined
        *   GetVisibleStringFromBMPString
        *   GetBMPStringFromVisibleString
        *   GetSizeOfEncodedLength
        *   EncodeLength
        *   ASN1WriteUlong
        *   GetPureEncodedLength
        *   GetLengthFromBuffer
        *   GetIntegerValueFromBuffer
        *   LookingForInfiniteEnd
        *   TraceBinaryData
        *   TraceOIDValue
        *   ParsingOIDStringValue
        *   ExportOIDValueToString
        *   TraceBMPString
        *   TraceErrorMessage
        *   TraceTabs
        *   ASN1Type2String
        *   AnscGotoNextTag
        *   AnscGetTLVLength

        *   ASN1WriteBinaryToFile
        *   ASN1LoadBinaryFromFile
        *   ASN1WriteASN1HandleToFile
        *   ASN1CreateTLV
        *   AnscAsn1GetMD5FingerPrint

        *   PKIInitDHKeyParameter
        *   PKIInitDHKeyParameterQ
        *   PKIIsDHParameterAcceptable

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        03/12/02    initial revision.
        03/14/02    function added: LookingForInfiniteEnd()
        03/18/02    function added: ParsingOIDStringValue()
                    function added: TraceOIDValue()
                    function added: ExportOIDValueToString()
        03/19/02    function added: TraceBMPString()
        03/26/02    function added: TraceErrorMessage()
        03/28/02    function added: TraceTabs()
        04/05/02    function added: ASN1WriteUlong()
        04/14/02    function added: ASN1Type2String()
        07/28/02    function added: ASN1WriteBinaryToFile()
        08/15/02    function added: AnscGotoNextTag() (Ignore the TL)
        09/12/02    function added: ASN1LoadBinaryFromFile()
        09/12/02    function added: ASN1WriteASN1HandleToFile()
        10/02/02    function added: AnscGetTLVLength()
        11/12/02    add DH key implementation;
        12/13/02    add function to create TLV;
        03/28/03    add GetIntegerValueFromBuffer();
        12/20/03    add function AnscCheckAsn1Validity()

**********************************************************************/


#ifndef  _ANSC_ASN1_UTILS_
#define  _ANSC_ASN1_UTILS_


/*
 *  define the maxi trace binary length
 */
#define MAXI_TRACE_BIANRY_LENGTH                    16
#define MAXI_TRACE_BMP_LENGTH                       32

/***********************************************************
        FUNCTIONS TO CHECK THE OBJECT TYPE
        IMPLEMENTED IN ANSC_ASN1_UTILS.C
***********************************************************/
BOOLEAN
IsObjectChoiceType
    (
        LONG                        uType
    );

BOOLEAN
IsObjectStringType
    (
        LONG                        uType
    );

BOOLEAN
IsObjectPrimitiveType
    (
        LONG                        uType
    );


BOOLEAN
IsObjectContextType
    (
        LONG                        uType
    );

PCHAR
ASN1Type2String
    (
        LONG                        uType
    );

BOOLEAN
IsObjectUserDefined
    (
        LONG                        uType
    );

BOOLEAN
GetVisibleStringFromBMPString
    (
        PUCHAR                      pUData,
        ULONG                       length,
        PCHAR                       pBuffer,
        PULONG                      pNewLength
    );

BOOLEAN
GetBMPStringFromVisibleString
    (
        PCHAR                       pData,
        ULONG                       length,
        PUCHAR                      pBuffer,
        PULONG                      pNewLength
    );

ULONG
GetSizeOfEncodedLength
    (
        ULONG                       ulLength
    );

ANSC_STATUS
EncodeLength
    (
        PVOID*                      ppEncodedData,
        ULONG                       ulLength
    );

ULONG
ASN1WriteUlong
    (
        PUCHAR                      pData,
        ULONG                       length
    );

LONG
GetPureEncodedLength
    (
        ULONG                       TotalLength
    );

ULONG
GetLengthFromBuffer
    (
        PUCHAR                      pBuffer,
        ULONG                       ulLength
    );

LONG
GetIntegerValueFromBuffer
    (
        PUCHAR                      pBuffer,
        ULONG                       ulLength
    );

PUCHAR
LookingForInfiniteEnd
    (
        PUCHAR                      pBuffer,
        ULONG                       uMaxLength
    );

void
TraceBinaryData
    (
        PUCHAR                      pBuffer,
        ULONG                       uLength,
        ULONG                       uMaxiLength
    );

BOOLEAN
ExportOIDValueToString
    (
        PULONG                      pArray,
        ULONG                       uLength,
        PCHAR                       pBuffer
    );

void
TraceOIDValue
    (
        PULONG                      pArray,
        ULONG                       uLength
    );

BOOLEAN
ParsingOIDStringValue
    (
        PCHAR                       pInput,
        PULONG                      pArray,
        PULONG                      pLength
    );

void
TraceBMPString
    (
        PUCHAR                      pBuffer,
        ULONG                       uLength
    );

void
TraceErrorMessage
    (   
        ANSC_STATUS                 anscStatus
    );

void
TraceTabs
    (   
        ULONG                       number
    );

/**************************************************
 *
 *  Some times the encoding data is not exactly what
 *  is required.
 *   
 *  This function is implemented to get the begining
 *  of the next tag which means ignore the (TL) and goto
 *  value directly
 *
 *  PUCHAR
 *  AnscGotoNextTag
 *      (
 *          PUCHAR                      pEncoding, (IN)
 *          ULONG                       length,    (IN)
 *          PULONG                      pNewLength (OUT)
 *      )
 *
 *  return: The place of the coming tag.
 *          If no valid place, return NUL;
 *
 **************************************************/
PUCHAR
AnscGotoNextTag
    (
        PUCHAR                      pEncoding,
        ULONG                       length,
        PULONG                      pNewLength
    );

/**************************************************
 *
 *  If it's valid TLV and indefinite encoding binary
 *  return the length of the bianry.
 *  Otherwise, return 0;
 *   
 *  ULONG
 *  AnscGetTLVLength
 *      (
 *          PUCHAR                      pEncoding
 *       );
 *
 **************************************************/
ULONG
AnscGetTLVLength
    (
        PUCHAR                      pEncoding
    );


BOOL
AnscCheckAsn1Validity
    (
        PUCHAR                      pEncoding,
        ULONG                       uLength    
    );

/**********************************************************************

    BOOLEAN
    ASN1WriteBinaryToFile
        (
            PCHAR                       pFileName,
            PUCHAR                      pEncoding,
            ULONG                       length
        )

    For debugging purpose, we can write the encoding data to file.

    argument:   PCHAR                   pFileName
                the  output file name;

                PUCHAR                      pEncoding,
                the binary encoded data;

                ULONG                       length
                the length of the binary;

      return:   succeeded or not;

**********************************************************************/
BOOLEAN
ASN1WriteBinaryToFile
    (
        PCHAR                       pFileName,
        PUCHAR                      pEncoding,
        ULONG                       length
    );

/************************************************************
 *
 * Load the binary string from the given file.
 *
 * The caller is responsible to release the returned memeory
 *

    PUCHAR
    ASN1LoadBinaryFromFile
        (
            PCHAR                       pFileName,
            PULONG                      pLength
        )

    Load the binary from the file

    argument:   PCHAR                   pFileName
                the  input file name;

                PULONG                  pLength
                The buffer of returned length;

      return:   The binary returned.

************************************************************/
PUCHAR
ASN1LoadBinaryFromFile
    (
        PCHAR                       pFileName,
        PULONG                      pLength
    );

/**********************************************************************

    BOOLEAN
    ASN1WriteBinaryToFile
        (
            PCHAR                       pFileName,
            PUCHAR                      pEncoding,
            ULONG                       length
        )

    For debugging purpose, we can write the encoding data to file.

    argument:   PCHAR                   pFileName
                the  output file name;

                PUCHAR                      pEncoding,
                the binary encoded data;

                ULONG                       length
                the length of the binary;

      return:   succeeded or not;

**********************************************************************/
BOOLEAN
ASN1WriteBinaryToFile
    (
        PCHAR                       pFileName,
        PUCHAR                      pEncoding,
        ULONG                       length
    );

/************************************************************

    BOOLEAN
    ASN1WriteASN1HandleToFile
        (
            PCHAR                       pFileName,
            ANSC_HANDLE                 hObject
        );

    Encode the ASN1 Object and write to file

************************************************************/
BOOLEAN
ASN1WriteASN1HandleToFile
    (
        PCHAR                       pFileName,
        ANSC_HANDLE                 hObject
    );

/************************************************************

    PUCHAR
    ASN1CreateTLV
        (
            UCHAR                       uTag,
            ULONG                       length,
            PUCHAR                      pData,
            PULONG                      pNewLength
        );

    Generate the new asn.1 object with given tag and value,
    the caller is responsible to release the returned buffer if
    it's not empty.

    Parameters:
            UCHAR                       uTag,
            The tag value, such as "0x28";

            ULONG                       length,
            The length of the data;

            PUCHAR                      pData
            The value of the data;

            PULONG                      pNewLength
            The buffer of the new length;

    return:

            The new created TLV encoding;

************************************************************/
PUCHAR
ASN1CreateTLV
    (
        UCHAR                           uTag,
        ULONG                           length,
        PUCHAR                          pData,
        PULONG                          pNewLength
    );


/**********************************************************************

    prototype:

        ANSC_STATUS
        AnscAsn1GetMD5FingerPrint
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hHashObject
            );

    description:

        This function returns the MD5 finger printer of this object

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hHashObject
                The hash object which is an output buffer

    return:     the state of the operation

**********************************************************************/
ANSC_STATUS
AnscAsn1GetMD5FingerPrint
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHashObject
    );

/**********************************************************************

    prototype:

        ANSC_STATUS
        AnscAsn1GetSHA1FingerPrint
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hHashObject
            );

    description:

        This function returns the SHA1 finger printer of this object

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hHashObject
                The hash object which is an output buffer

    return:     the state of the operation

**********************************************************************/
ANSC_STATUS
AnscAsn1GetSHA1FingerPrint
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hHashObject
    );

/**********************************************************************

  PKINIT states that DH parameters SHOULD be taken from the first or 
  second Oakley groups. Additionally, this specification requires that 
  DH groups are used exactly as defined in RFC2409.

  Because RFC2409 does not give values of q ( the p-1 factor) for the groups,
  and these are necessary in order to encode the dhPublicNumber type used 
  in the subjectPublicKeyInfo data structure in PKINIT.
    
 **********************************************************************/

/**********************************************************************

  Function:

    ANSC_STATUS
    PKIInitDHKeyParameter
        (
            ANSC_HANDLE                 hDHParameter,    (output)
            UINT                        length
        );

  Only 96 and 128 bits are supported here. 
  If the length is invalid, 128 bits will be used.

  *******************************************************************/

ANSC_STATUS
PKIInitDHKeyParameter
    (
        ANSC_HANDLE                 hDHParameter,  /* output */
        UINT                        length
    );

/**********************************************************************

  Function:

    ANSC_STATUS
    PKIInitDHKeyParameterQ
        (
            ANSC_HANDLE                 hDHParameter, ( input )
            PUCHAR                      pBufferQ,
            PULONG                      pLength
        );

  This function will check the input DHParameter. If it's the first and
  second Oakley group, it outputs the value of ParameterQ respectively;
  Otherwise, it returns status failure.

  *******************************************************************/
ANSC_STATUS
PKIInitDHKeyParameterQ
    (
        ANSC_HANDLE                 hDHParameter, /* input */
        PUCHAR                      pBufferQ,
        PULONG                      pLength
    );

/**********************************************************************

  Function:

    BOOLEAN
    PKIIsDHParameterAcceptable
        (
            ANSC_HANDLE                 hDHParameter
        );

  This function will check the input DHParameter. If it's the first and
  second Oakley group, it returns TRUE;  otherwise returns FALSE.

  *******************************************************************/
BOOLEAN
PKIIsDHParameterAcceptable
    (
        ANSC_HANDLE                 hDHParameter
    );


#endif /*_ANSC_ASN1_UTILS*/
