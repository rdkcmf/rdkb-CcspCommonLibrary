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

    module: ansc_asn1_string.c

        For ASN.1 STRING object

    ---------------------------------------------------------------

    description:

        This file implements the all the functions
        provided by all the ASN.1 string classes

        *   AnscAsn1StringCreate
        *   AnscAsn1OctetStringCreate
        *   AnscAsn1UTF8StringCreate
        *   AnscAsn1UniversalStringCreate
        *   AnscAsn1TeletexStringCreate
        *   AnscAsn1IA5StringCreate
        *   AnscAsn1PrintableStringCreate
        *   AnscAsn1VideotexStringCreate
        *   AnscAsn1VisibleStringCreate
        *   AnscAsn1NumericStringCreate
        *   AnscAsn1GeneralStringCreate
        *   AnscAsn1GraphicStringCreate
        *   AnscAsn1BMPStringCreate
        *   AnscAsn1MSStringCreate

        *   AnscAsn1StringFree
        *   AnscAsn1StringInitialize
        *   AnscAsn1StringClone
        *   AnscAsn1StringCopyTo
        *   AnscAsn1StringEqualsTo
        *   AnscAsn1StringGetSizeOfEncoded
        *   AnscAsn1StringDecodingData
        *   AnscAsn1StringEncodingData
        *   AnscAsn1StringTraceObject
        *   AnscAsn1StringDumpObject

        *   AnscAsn1StringSetStringValue
        *   AnscAsn1StringGetStringValue
        *   AnscAsn1StringGetValueBuffer

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Bin Zhu

    ---------------------------------------------------------------

    revision:

        03/18/2002    initial revision.
        06/14/2002    PFN_ASN1_GET_VALUE_BUFFER was added for Integer  
                      and STRING objects.

**********************************************************************/

#include "ansc_asn1_local.h"
#include "safec_lib_common.h"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscAsn1StringCreate
            (
                ANSC_HANDLE                 hContainerContext,
                ULONG                       uType
            );

    description:

        This function constructs an ASN.1 object and
        initializes the member variables and functions.

    argument:   ANSC_HANDLE                 hContainerContext
                This handle is used by the container object to interact
                with the outside world. It could be the real container
                or an target object.

                ULONG                       uType,
                The object type of the string object.

    return:     newly created container object.

**********************************************************************/

ANSC_HANDLE
AnscAsn1StringCreate
    (
        ANSC_HANDLE                 hContainerContext,
        ULONG                       uType
    )
{
    switch ( uType )
    {
        case ASN1_OCTETSTRING_TYPE:

                return AnscAsn1OctetStringCreate
                            (
                                hContainerContext
                            );

        case ASN1_UTF8STRING_TYPE:

                return AnscAsn1UTF8StringCreate
                            (
                                hContainerContext
                            );

        case ASN1_NUMERICSTRING_TYPE:

                return AnscAsn1NumericStringCreate
                            (
                                hContainerContext
                            );

        case ASN1_PRINTABLESTRING_TYPE:

                return AnscAsn1PrintableStringCreate
                            (
                                hContainerContext
                            );

        case ASN1_TELETEXSTRING_TYPE:

                return AnscAsn1TeletexStringCreate
                            (
                                hContainerContext
                            );

        case ASN1_VIDEOTEXSTRING_TYPE:

                return AnscAsn1VideotexStringCreate
                            (
                                hContainerContext
                            );

        case ASN1_IA5STRING_TYPE:

                return AnscAsn1IA5StringCreate
                            (
                                hContainerContext
                            );

        case ASN1_GRAPHICSTRING_TYPE:

                return AnscAsn1GraphicStringCreate
                            (
                                hContainerContext
                            );

        case ASN1_VISIBLESTRING_TYPE:

                return AnscAsn1VisibleStringCreate
                            (
                                hContainerContext
                            );

        case ASN1_GENERALSTRING_TYPE:

                return AnscAsn1GeneralStringCreate
                            (
                                hContainerContext
                            );

        case ASN1_UNIVERSALSTRING_TYPE:

                return AnscAsn1UniversalStringCreate
                            (
                                hContainerContext
                            );
        
        case ASN1_BMPSTRING_TYPE:

                return AnscAsn1BMPStringCreate
                            (
                                hContainerContext
                            );

        case ASN1_MSSTRING_TYPE:

                return AnscAsn1MSStringCreate
                            (
                                hContainerContext
                            );
        default:

                return NULL;
    }
}
/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscAsn1OctetStringCreate
            (
                ANSC_HANDLE                 hContainerContext
            );

    description:

        This function constructs an OctetString object.

    argument:   ANSC_HANDLE                 hContainerContext
                This handle is used by the container object to interact
                with the outside world. It could be the real container
                or an target object.

    return:     newly created container object.

**********************************************************************/
ANSC_HANDLE
AnscAsn1OctetStringCreate
    (
        ANSC_HANDLE                 hContainerContext
    )
{
    PANSC_ASN1_STRING               pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    if( hContainerContext != NULL)
    {
        pMyObject = (PANSC_ASN1_STRING)AnscAllocateMemory((ULONG)hContainerContext);
    }
    else
    {
        pMyObject = (PANSC_ASN1_STRING)AnscAllocateMemory(sizeof(ANSC_ASN1_STRING));
    }    

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     *  reset the SAttrList;
     */
    AnscSListInitializeHeader(&pMyObject->sAttrList);

    pMyObject->hContainerContext   = hContainerContext;
    pMyObject->hOwnerContext       = NULL;
    pMyObject->Oid                 = ANSC_ASN1_OCTETSTRING_ID;
    pMyObject->uType               = ASN1_OCTETSTRING_TYPE;
    pMyObject->bOptional           = FALSE;
    pMyObject->bCanBeOptional      = FALSE;
    pMyObject->Create              = AnscAsn1OctetStringCreate;
    pMyObject->AsnFree             = AnscAsn1StringFree;
    
    AnscAsn1StringInitialize(pMyObject);

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->SetName(pMyObject,ANSC_ASN1_OCTETSTRING_NAME);
    pMyObject->SetClassName(pMyObject,ANSC_ASN1_OCTETSTRING_CLASS_NAME);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscAsn1UTF8StringCreate
            (
                ANSC_HANDLE                 hContainerContext
            );

    description:

        This function constructs a UTF8String object.

    argument:   ANSC_HANDLE                 hContainerContext
                This handle is used by the container object to interact
                with the outside world. It could be the real container
                or an target object.

    return:     newly created container object.

**********************************************************************/
ANSC_HANDLE
AnscAsn1UTF8StringCreate
    (
        ANSC_HANDLE                 hContainerContext
    )
{
    PANSC_ASN1_STRING               pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    if( hContainerContext != NULL)
    {
        pMyObject = (PANSC_ASN1_STRING)AnscAllocateMemory((ULONG)hContainerContext);
    }
    else
    {
        pMyObject = (PANSC_ASN1_STRING)AnscAllocateMemory(sizeof(ANSC_ASN1_STRING));
    }    

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     *  reset the SAttrList;
     */
    AnscSListInitializeHeader(&pMyObject->sAttrList);

    pMyObject->hContainerContext   = hContainerContext;
    pMyObject->hOwnerContext       = NULL;
    pMyObject->Oid                 = ANSC_ASN1_UTF8STRING_ID;
    pMyObject->uType               = ASN1_UTF8STRING_TYPE;
    pMyObject->bOptional           = FALSE;
    pMyObject->bCanBeOptional      = FALSE;
    pMyObject->Create              = AnscAsn1UTF8StringCreate;
    pMyObject->AsnFree             = AnscAsn1StringFree;
    
    AnscAsn1StringInitialize(pMyObject);

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->SetName(pMyObject,ANSC_ASN1_UTF8STRING_NAME);
    pMyObject->SetClassName(pMyObject,ANSC_ASN1_UTF8STRING_CLASS_NAME);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscAsn1UniversalStringCreate
            (
                ANSC_HANDLE                 hContainerContext
            );

    description:

        This function constructs a UniversalString object.

    argument:   ANSC_HANDLE                 hContainerContext
                This handle is used by the container object to interact
                with the outside world. It could be the real container
                or an target object.

    return:     newly created container object.

**********************************************************************/
ANSC_HANDLE
AnscAsn1UniversalStringCreate
    (
        ANSC_HANDLE                 hContainerContext
    )
{
    PANSC_ASN1_STRING               pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    if( hContainerContext != NULL)
    {
        pMyObject = (PANSC_ASN1_STRING)AnscAllocateMemory((ULONG)hContainerContext);
    }
    else
    {
        pMyObject = (PANSC_ASN1_STRING)AnscAllocateMemory(sizeof(ANSC_ASN1_STRING));
    }    

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     *  reset the SAttrList;
     */
    AnscSListInitializeHeader(&pMyObject->sAttrList);

    pMyObject->hContainerContext   = hContainerContext;
    pMyObject->hOwnerContext       = NULL;
    pMyObject->Oid                 = ANSC_ASN1_UNIVERSALSTRING_ID;
    pMyObject->uType               = ASN1_UNIVERSALSTRING_TYPE;
    pMyObject->bOptional           = FALSE;
    pMyObject->bCanBeOptional      = FALSE;
    pMyObject->Create              = AnscAsn1UniversalStringCreate;
    pMyObject->AsnFree             = AnscAsn1StringFree;
    
    AnscAsn1StringInitialize(pMyObject);

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->SetName(pMyObject,ANSC_ASN1_UNIVERSALSTRING_NAME);
    pMyObject->SetClassName(pMyObject,ANSC_ASN1_UNIVERSALSTRING_CLASS_NAME);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscAsn1TeletexStringCreate
            (
                ANSC_HANDLE                 hContainerContext
            );

    description:

        This function constructs a TeletexString object.

    argument:   ANSC_HANDLE                 hContainerContext
                This handle is used by the container object to interact
                with the outside world. It could be the real container
                or an target object.

    return:     newly created container object.

**********************************************************************/
ANSC_HANDLE
AnscAsn1TeletexStringCreate
    (
        ANSC_HANDLE                 hContainerContext
    )
{
    PANSC_ASN1_STRING               pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    if( hContainerContext != NULL)
    {
        pMyObject = (PANSC_ASN1_STRING)AnscAllocateMemory((ULONG)hContainerContext);
    }
    else
    {
        pMyObject = (PANSC_ASN1_STRING)AnscAllocateMemory(sizeof(ANSC_ASN1_STRING));
    }    

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     *  reset the SAttrList;
     */
    AnscSListInitializeHeader(&pMyObject->sAttrList);

    pMyObject->hContainerContext   = hContainerContext;
    pMyObject->hOwnerContext       = NULL;
    pMyObject->Oid                 = ANSC_ASN1_TELETEXSTRING_ID;
    pMyObject->uType               = ASN1_TELETEXSTRING_TYPE;
    pMyObject->bOptional           = FALSE;
    pMyObject->bCanBeOptional      = FALSE;
    pMyObject->Create              = AnscAsn1TeletexStringCreate;
    pMyObject->AsnFree             = AnscAsn1StringFree;
    
    AnscAsn1StringInitialize(pMyObject);

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->SetName(pMyObject,ANSC_ASN1_TELETEXSTRING_NAME);
    pMyObject->SetClassName(pMyObject,ANSC_ASN1_TELETEXSTRING_CLASS_NAME);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscAsn1IA5StringCreate
            (
                ANSC_HANDLE                 hContainerContext
            );

    description:

        This function constructs an IA5String object.

    argument:   ANSC_HANDLE                 hContainerContext
                This handle is used by the container object to interact
                with the outside world. It could be the real container
                or an target object.

    return:     newly created container object.

**********************************************************************/
ANSC_HANDLE
AnscAsn1IA5StringCreate
    (
        ANSC_HANDLE                 hContainerContext
    )
{
    PANSC_ASN1_STRING               pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    if( hContainerContext != NULL)
    {
        pMyObject = (PANSC_ASN1_STRING)AnscAllocateMemory((ULONG)hContainerContext);
    }
    else
    {
        pMyObject = (PANSC_ASN1_STRING)AnscAllocateMemory(sizeof(ANSC_ASN1_STRING));
    }    

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     *  reset the SAttrList;
     */
    AnscSListInitializeHeader(&pMyObject->sAttrList);

    pMyObject->hContainerContext   = hContainerContext;
    pMyObject->hOwnerContext       = NULL;
    pMyObject->Oid                 = ANSC_ASN1_IA5STRING_ID;
    pMyObject->uType               = ASN1_IA5STRING_TYPE;
    pMyObject->bOptional           = FALSE;
    pMyObject->bCanBeOptional      = FALSE;
    pMyObject->Create              = AnscAsn1IA5StringCreate;
    pMyObject->AsnFree             = AnscAsn1StringFree;
    
    AnscAsn1StringInitialize(pMyObject);

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->SetName(pMyObject,ANSC_ASN1_IA5STRING_NAME);
    pMyObject->SetClassName(pMyObject,ANSC_ASN1_IA5STRING_CLASS_NAME);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscAsn1PrintableStringCreate
            (
                ANSC_HANDLE                 hContainerContext
            );

    description:

        This function constructs a PrintableString object.

    argument:   ANSC_HANDLE                 hContainerContext
                This handle is used by the container object to interact
                with the outside world. It could be the real container
                or an target object.

    return:     newly created container object.

**********************************************************************/
ANSC_HANDLE
AnscAsn1PrintableStringCreate
    (
        ANSC_HANDLE                 hContainerContext
    )
{
    PANSC_ASN1_STRING               pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    if( hContainerContext != NULL)
    {
        pMyObject = (PANSC_ASN1_STRING)AnscAllocateMemory((ULONG)hContainerContext);
    }
    else
    {
        pMyObject = (PANSC_ASN1_STRING)AnscAllocateMemory(sizeof(ANSC_ASN1_STRING));
    }    

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     *  reset the SAttrList;
     */
    AnscSListInitializeHeader(&pMyObject->sAttrList);

    pMyObject->hContainerContext   = hContainerContext;
    pMyObject->hOwnerContext       = NULL;
    pMyObject->Oid                 = ANSC_ASN1_PRINTABLESTRING_ID;
    pMyObject->uType               = ASN1_PRINTABLESTRING_TYPE;
    pMyObject->bOptional           = FALSE;
    pMyObject->bCanBeOptional      = FALSE;
    pMyObject->Create              = AnscAsn1PrintableStringCreate;
    pMyObject->AsnFree             = AnscAsn1StringFree;
    
    AnscAsn1StringInitialize(pMyObject);

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->SetName(pMyObject,ANSC_ASN1_PRINTABLESTRING_NAME);
    pMyObject->SetClassName(pMyObject,ANSC_ASN1_PRINTABLESTRING_CLASS_NAME);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscAsn1VideotexStringCreate
            (
                ANSC_HANDLE                 hContainerContext
            );

    description:

        This function constructs a VideotexString object.

    argument:   ANSC_HANDLE                 hContainerContext
                This handle is used by the container object to interact
                with the outside world. It could be the real container
                or an target object.

    return:     newly created container object.

**********************************************************************/
ANSC_HANDLE
AnscAsn1VideotexStringCreate
    (
        ANSC_HANDLE                 hContainerContext
    )
{
    PANSC_ASN1_STRING               pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    if( hContainerContext != NULL)
    {
        pMyObject = (PANSC_ASN1_STRING)AnscAllocateMemory((ULONG)hContainerContext);
    }
    else
    {
        pMyObject = (PANSC_ASN1_STRING)AnscAllocateMemory(sizeof(ANSC_ASN1_STRING));
    }    

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     *  reset the SAttrList;
     */
    AnscSListInitializeHeader(&pMyObject->sAttrList);

    pMyObject->hContainerContext   = hContainerContext;
    pMyObject->hOwnerContext       = NULL;
    pMyObject->Oid                 = ANSC_ASN1_VIDEOTEXSTRING_ID;
    pMyObject->uType               = ASN1_VIDEOTEXSTRING_TYPE;
    pMyObject->bOptional           = FALSE;
    pMyObject->bCanBeOptional      = FALSE;
    pMyObject->Create              = AnscAsn1VideotexStringCreate;
    pMyObject->AsnFree             = AnscAsn1StringFree;
    
    AnscAsn1StringInitialize(pMyObject);

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->SetName(pMyObject,ANSC_ASN1_VIDEOTEXSTRING_NAME);
    pMyObject->SetClassName(pMyObject,ANSC_ASN1_VIDEOTEXSTRING_CLASS_NAME);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscAsn1VisibleStringCreate
            (
                ANSC_HANDLE                 hContainerContext
            );

    description:

        This function constructs a VisibleString object.

    argument:   ANSC_HANDLE                 hContainerContext
                This handle is used by the container object to interact
                with the outside world. It could be the real container
                or an target object.

    return:     newly created container object.

**********************************************************************/
ANSC_HANDLE
AnscAsn1VisibleStringCreate
    (
        ANSC_HANDLE                 hContainerContext
    )
{
    PANSC_ASN1_STRING               pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    if( hContainerContext != NULL)
    {
        pMyObject = (PANSC_ASN1_STRING)AnscAllocateMemory((ULONG)hContainerContext);
    }
    else
    {
        pMyObject = (PANSC_ASN1_STRING)AnscAllocateMemory(sizeof(ANSC_ASN1_STRING));
    }    

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     *  reset the SAttrList;
     */
    AnscSListInitializeHeader(&pMyObject->sAttrList);

    pMyObject->hContainerContext   = hContainerContext;
    pMyObject->hOwnerContext       = NULL;
    pMyObject->Oid                 = ANSC_ASN1_VISIBLESTRING_ID;
    pMyObject->uType               = ASN1_VISIBLESTRING_TYPE;
    pMyObject->bOptional           = FALSE;
    pMyObject->bCanBeOptional      = FALSE;
    pMyObject->Create              = AnscAsn1VisibleStringCreate;
    pMyObject->AsnFree             = AnscAsn1StringFree;
    
    AnscAsn1StringInitialize(pMyObject);

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->SetName(pMyObject,ANSC_ASN1_VISIBLESTRING_NAME);
    pMyObject->SetClassName(pMyObject,ANSC_ASN1_VISIBLESTRING_CLASS_NAME);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscAsn1NumericStringCreate
            (
                ANSC_HANDLE                 hContainerContext
            );

    description:

        This function constructs an OctetString object.

    argument:   ANSC_HANDLE                 hContainerContext
                This handle is used by the container object to interact
                with the outside world. It could be the real container
                or an target object.

    return:     newly created container object.

**********************************************************************/
ANSC_HANDLE
AnscAsn1NumericStringCreate
    (
        ANSC_HANDLE                 hContainerContext
    )
{
    PANSC_ASN1_STRING               pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    if( hContainerContext != NULL)
    {
        pMyObject = (PANSC_ASN1_STRING)AnscAllocateMemory((ULONG)hContainerContext);
    }
    else
    {
        pMyObject = (PANSC_ASN1_STRING)AnscAllocateMemory(sizeof(ANSC_ASN1_STRING));
    }    

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     *  reset the SAttrList;
     */
    AnscSListInitializeHeader(&pMyObject->sAttrList);

    pMyObject->hContainerContext   = hContainerContext;
    pMyObject->hOwnerContext       = NULL;
    pMyObject->Oid                 = ANSC_ASN1_NUMERICSTRING_ID;
    pMyObject->uType               = ASN1_NUMERICSTRING_TYPE;
    pMyObject->bOptional           = FALSE;
    pMyObject->bCanBeOptional      = FALSE;
    pMyObject->Create              = AnscAsn1NumericStringCreate;
    pMyObject->AsnFree             = AnscAsn1StringFree;
    
    AnscAsn1StringInitialize(pMyObject);

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->SetName(pMyObject,ANSC_ASN1_NUMERICSTRING_NAME);
    pMyObject->SetClassName(pMyObject,ANSC_ASN1_NUMERICSTRING_CLASS_NAME);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscAsn1GeneralStringCreate
            (
                ANSC_HANDLE                 hContainerContext
            );

    description:

        This function constructs a GeneralString object.

    argument:   ANSC_HANDLE                 hContainerContext
                This handle is used by the container object to interact
                with the outside world. It could be the real container
                or an target object.

    return:     newly created container object.

**********************************************************************/
ANSC_HANDLE
AnscAsn1GeneralStringCreate
    (
        ANSC_HANDLE                 hContainerContext
    )
{
    PANSC_ASN1_STRING               pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    if( hContainerContext != NULL)
    {
        pMyObject = (PANSC_ASN1_STRING)AnscAllocateMemory((ULONG)hContainerContext);
    }
    else
    {
        pMyObject = (PANSC_ASN1_STRING)AnscAllocateMemory(sizeof(ANSC_ASN1_STRING));
    }    

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     *  reset the SAttrList;
     */
    AnscSListInitializeHeader(&pMyObject->sAttrList);

    pMyObject->hContainerContext   = hContainerContext;
    pMyObject->hOwnerContext       = NULL;
    pMyObject->Oid                 = ANSC_ASN1_GENERALSTRING_ID;
    pMyObject->uType               = ASN1_GENERALSTRING_TYPE;
    pMyObject->bOptional           = FALSE;
    pMyObject->bCanBeOptional      = FALSE;
    pMyObject->Create              = AnscAsn1GeneralStringCreate;
    pMyObject->AsnFree             = AnscAsn1StringFree;
    
    AnscAsn1StringInitialize(pMyObject);

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->SetName(pMyObject,ANSC_ASN1_GENERALSTRING_NAME);
    pMyObject->SetClassName(pMyObject,ANSC_ASN1_GENERALSTRING_CLASS_NAME);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscAsn1GraphicStringCreate
            (
                ANSC_HANDLE                 hContainerContext
            );

    description:

        This function constructs a GraphicString object.

    argument:   ANSC_HANDLE                 hContainerContext
                This handle is used by the container object to interact
                with the outside world. It could be the real container
                or an target object.

    return:     newly created container object.

**********************************************************************/
ANSC_HANDLE
AnscAsn1GraphicStringCreate
    (
        ANSC_HANDLE                 hContainerContext
    )
{
    PANSC_ASN1_STRING               pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    if( hContainerContext != NULL)
    {
        pMyObject = (PANSC_ASN1_STRING)AnscAllocateMemory((ULONG)hContainerContext);
    }
    else
    {
        pMyObject = (PANSC_ASN1_STRING)AnscAllocateMemory(sizeof(ANSC_ASN1_STRING));
    }    

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     *  reset the SAttrList;
     */
    AnscSListInitializeHeader(&pMyObject->sAttrList);

    pMyObject->hContainerContext   = hContainerContext;
    pMyObject->hOwnerContext       = NULL;
    pMyObject->Oid                 = ANSC_ASN1_GRAPHICSTRING_ID;
    pMyObject->uType               = ASN1_GRAPHICSTRING_TYPE;
    pMyObject->bOptional           = FALSE;
    pMyObject->Create              = AnscAsn1GraphicStringCreate;
    pMyObject->AsnFree             = AnscAsn1StringFree;
    
    AnscAsn1StringInitialize(pMyObject);

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->SetName(pMyObject,ANSC_ASN1_GRAPHICSTRING_NAME);
    pMyObject->SetClassName(pMyObject,ANSC_ASN1_GRAPHICSTRING_CLASS_NAME);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscAsn1BMPStringCreate
            (
                ANSC_HANDLE                 hContainerContext
            );

    description:

        This function constructs a BMPString object.

    argument:   ANSC_HANDLE                 hContainerContext
                This handle is used by the container object to interact
                with the outside world. It could be the real container
                or an target object.

    return:     newly created container object.

**********************************************************************/
ANSC_HANDLE
AnscAsn1BMPStringCreate
    (
        ANSC_HANDLE                 hContainerContext
    )
{
    PANSC_ASN1_STRING               pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    if( hContainerContext != NULL)
    {
        pMyObject = (PANSC_ASN1_STRING)AnscAllocateMemory((ULONG)hContainerContext);
    }
    else
    {
        pMyObject = (PANSC_ASN1_STRING)AnscAllocateMemory(sizeof(ANSC_ASN1_STRING));
    }    

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     *  reset the SAttrList;
     */
    AnscSListInitializeHeader(&pMyObject->sAttrList);

    pMyObject->hContainerContext   = hContainerContext;
    pMyObject->hOwnerContext       = NULL;
    pMyObject->Oid                 = ANSC_ASN1_BMPSTRING_ID;
    pMyObject->uType               = ASN1_BMPSTRING_TYPE;
    pMyObject->bOptional           = FALSE;
    pMyObject->Create              = AnscAsn1BMPStringCreate;
    pMyObject->AsnFree             = AnscAsn1StringFree;
    
    AnscAsn1StringInitialize(pMyObject);

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->SetName(pMyObject,ANSC_ASN1_BMPSTRING_NAME);
    pMyObject->SetClassName(pMyObject,ANSC_ASN1_BMPSTRING_CLASS_NAME);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscAsn1MSStringCreate
            (
                ANSC_HANDLE                 hContainerContext
            );

    description:

        This function constructs a MSString object.

    argument:   ANSC_HANDLE                 hContainerContext
                This handle is used by the container object to interact
                with the outside world. It could be the real container
                or an target object.

    return:     newly created container object.

**********************************************************************/
ANSC_HANDLE
AnscAsn1MSStringCreate
    (
        ANSC_HANDLE                 hContainerContext
    )
{
    PANSC_ASN1_STRING               pMyObject    = NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */
    if( hContainerContext != NULL)
    {
        pMyObject = (PANSC_ASN1_STRING)AnscAllocateMemory((ULONG)hContainerContext);
    }
    else
    {
        pMyObject = (PANSC_ASN1_STRING)AnscAllocateMemory(sizeof(ANSC_ASN1_STRING));
    }    

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }

    /*
     *  reset the SAttrList;
     */
    AnscSListInitializeHeader(&pMyObject->sAttrList);

    pMyObject->hContainerContext   = hContainerContext;
    pMyObject->hOwnerContext       = NULL;
    pMyObject->Oid                 = ANSC_ASN1_MSSTRING_ID;
    pMyObject->uType               = ASN1_MSSTRING_TYPE;
    pMyObject->bOptional           = FALSE;
    pMyObject->Create              = AnscAsn1MSStringCreate;
    pMyObject->AsnFree             = AnscAsn1StringFree;
    
    AnscAsn1StringInitialize(pMyObject);

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->SetName(pMyObject,ANSC_ASN1_MSSTRING_NAME);
    pMyObject->SetClassName(pMyObject,ANSC_ASN1_MSSTRING_CLASS_NAME);

    return  (ANSC_HANDLE)pMyObject;
}
/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1StringFree
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function destroys the object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscAsn1StringFree
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_STRING               pMyObject    = (PANSC_ASN1_STRING)hThisObject;

    if( pMyObject != NULL)
    {
        if( pMyObject->bIsDynamic)
        {
            AnscFreeMemory( pMyObject->pStringBuffer);
        }
    }

    AnscAsn1Free(pMyObject);

    return  ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1StringInitialize
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function first calls the initialization member function
        of the base class object to set the common member fields
        inherited from the base class. It then initializes the member
        fields that are specific to this object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     status of operation.

**********************************************************************/

ANSC_STATUS
AnscAsn1StringInitialize
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_STRING               pMyObject    = (PANSC_ASN1_STRING)hThisObject;

    /*
     *  Call base initialization first
     */
    AnscAsn1Initialize((ANSC_HANDLE)pMyObject);

    /* set other functions */
    pMyObject->Clone                = AnscAsn1StringClone;
    pMyObject->CopyTo               = AnscAsn1StringCopyTo;
    pMyObject->EqualsTo             = AnscAsn1StringEqualsTo;
    pMyObject->DecodingData         = AnscAsn1StringDecodingData;
    pMyObject->EncodingData         = AnscAsn1StringEncodingData;
    pMyObject->GetSizeOfEncoded     = AnscAsn1StringGetSizeOfEncoded;
    pMyObject->DumpObject           = AnscAsn1StringDumpObject;

    pMyObject->GetStringValue       = AnscAsn1StringGetStringValue;
    pMyObject->SetStringValue       = AnscAsn1StringSetStringValue;
    pMyObject->GetValueBuffer       = AnscAsn1StringGetValueBuffer;

    pMyObject->bIsDynamic           = FALSE;
    pMyObject->pStringBuffer        = NULL;
    pMyObject->uLength              = 0;

    AnscZeroMemory((PVOID)&pMyObject->pString, MAXIMUM_OCTETSTRING_OCTETS);
    
    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        BOOLEAN
        AnscAsn1StringCopyTo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hDestObject
            );

    description:

        This function copies the value to the other one;

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hDestObject
                the dest object;

    return:     succeeded or not;

**********************************************************************/
BOOLEAN
AnscAsn1StringCopyTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDestObject
    )
{
    PANSC_ASN1_STRING               pMyObject    = (PANSC_ASN1_STRING)hThisObject;
    PANSC_ASN1_STRING               pNewObject   = (PANSC_ASN1_STRING)hDestObject;

    if( pNewObject == NULL || pMyObject == NULL)
    {
        return FALSE;
    }

    /*
     *  Set the value;
     */
    pNewObject->bIsDynamic          = pMyObject->bIsDynamic;
    pNewObject->uLength             = pMyObject->uLength;

    if( pNewObject->bIsDynamic)
    {
        pNewObject->pStringBuffer   = (PVOID)AnscAllocateMemory(pMyObject->uLength + 1);

        if( pNewObject->pStringBuffer == NULL)
        {
            return FALSE;
        }

        AnscCopyMemory
            ( 
                pNewObject->pStringBuffer, 
                pMyObject->pStringBuffer, 
                pMyObject->uLength
            );
    }
    else
    {
        if( pMyObject->uLength > 0)
        {
            AnscCopyMemory
                ( 
                    pNewObject->pString, 
                    pMyObject->pString,
                    pMyObject->uLength
                );
        }
    }
    
   
    /*
     * Copy the optional state;
     */
    pNewObject->bOptional       = pMyObject->bOptional;
    pNewObject->bCanBeOptional  = pMyObject->bCanBeOptional;

    return TRUE;
}

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE
        AnscAsn1StringClone
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

        This function clones a same ASN.1 object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     The cloned container object.

**********************************************************************/
ANSC_HANDLE
AnscAsn1StringClone
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_STRING               pMyObject    = (PANSC_ASN1_STRING)hThisObject;
    PANSC_ASN1_STRING               pNewObject   = NULL;

    pNewObject = (PANSC_ASN1_STRING)
                    pMyObject->Create
                        (
                            pMyObject->hContainerContext
                        );

    if( pNewObject == NULL)
    {
        AnscTrace("Failed to clone at AnscAsn1StringClone()\n");

        return NULL;
    }

    /*
     *  Copy all the attributes;
     */
    /* AttrListCopyAllAttributes( &pNewObject->sAttrList, &pMyObject->sAttrList);*/
    pNewObject->SetName(pNewObject, pMyObject->Name);

    if( ! pMyObject->CopyTo(pMyObject, pNewObject))
    {
        pNewObject->AsnFree(pNewObject);

        return NULL;
    }

    return pNewObject;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1StringEqualsTo
            (
                ANSC_HANDLE                 hThisObject,
                ANSC_HANDLE                 hOtherObject,
                BOOLEAN                     bValueOnly
            );

    description:

        This function compares the value of the 2 ASN.1 objects.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                ANSC_HANDLE                 hOtherObject
                This handle is the other object will be compared to;

                BOOLEAN                     bValueOnly
                Check value only or not;

    return:     If they have the same type and value, return TRUE;
                otherwise return FALSE.

**********************************************************************/
BOOLEAN
AnscAsn1StringEqualsTo
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hOtherObject,
        BOOLEAN                     bValueOnly
    )
{
    PANSC_ASN1_STRING               pMyObject  = (PANSC_ASN1_STRING)hThisObject;
    PANSC_ASN1_STRING               pNewObject = (PANSC_ASN1_STRING)hOtherObject;

    /* check the pointer first */
    if( hThisObject == hOtherObject)
    {
        return TRUE;
    }

    if( hThisObject == NULL || hOtherObject == NULL)
    {
        AnscTrace("One of the parameter in AnscAsn1StringEqualsTo() is NULL pointer.\n");

        return FALSE;
    }

    /* check the type */
    if( pMyObject->uType != pNewObject->uType)
    {
        AnscTrace("Different type in AnscAsn1StringEqualsTo().\n");

        return FALSE;
    }

    /* check the optional state */
    if( pMyObject->bOptional != pNewObject->bOptional)
    {
        return FALSE;
    }

    /* check the attribute list */
    if( !bValueOnly)
    {
        if(!AttrListCompareAttributeList(&pMyObject->sAttrList, &pNewObject->sAttrList))
        {
            return FALSE;
        }
    }

    /* check the value */
    if(!pMyObject->bOptional)
    {
        if( pMyObject->bIsDynamic != pNewObject->bIsDynamic ||
            pMyObject->uLength    != pNewObject->uLength
          )
        {
            return FALSE;
        }

        if( pMyObject->bIsDynamic)
        {
            return AnscEqualMemory
                        (
                            pMyObject->pStringBuffer,
                            pNewObject->pStringBuffer,
                            pMyObject->uLength
                        );            
        }
        else
        {
            /* case insensitive in printable and ia5  strings */
            if( pMyObject->uType == ASN1_PRINTABLESTRING_TYPE ||
                pMyObject->uType == ASN1_IA5STRING_TYPE)
            {
                return ( strcmp((PCHAR)pMyObject->pString,(PCHAR)pNewObject->pString) == 0 );            
            }
            else
            {
                return AnscEqualMemory
                            (
                                pMyObject->pString,
                                pNewObject->pString,
                                pMyObject->uLength
                            );            
            }
        }
    }

    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        LONG
        AnscAsn1StringGetSizeOfEncoded
            (
                ANSC_HANDLE                 hThisObject
            )

    description:

        This function returns the encoding size of the 
        ASN.1 object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     The encoding size of the ASN.1 object.
                If the object is not ready-to-encode, -1 will be 
                returned.

**********************************************************************/
LONG
AnscAsn1StringGetSizeOfEncoded
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_STRING               pMyObject    = (PANSC_ASN1_STRING)hThisObject;
    ULONG                           ulSize       = 0;

    /*
     * If it's optional, don't need encode
     */
    if( pMyObject->bOptional)
    {
        return 0;
    }

    if( !pMyObject->ReadyToEncode(pMyObject))
    {
        AnscTrace("The value of the bitstring object is not ready to encode.\n");

        return -1;
    }

    /*
     * The data is string values;
     */
    ulSize              = pMyObject->uLength;

    /*
     *  check the attribute list, from the end of the list;
     *  the tag length value is 1.
     */ 
    return AttrListGetSizeOfEncoded(&pMyObject->sAttrList, ulSize, 1);
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1StringDecodingData
            (
                ANSC_HANDLE                 hThisObject,
                PVOID*                      ppEncoding
            );

    description:

        This function is decoding data for ASN.1 object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID*                      ppEncoding
                The pointer of the input binary data

    return:     status of decoding operation

**********************************************************************/
static ANSC_STATUS
decodingConstructedString
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding,
        LONG                        length
    )
{
    PANSC_ASN1_STRING               pMyObject    = (PANSC_ASN1_STRING   )hThisObject;
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_ASN1_STRING               pNewObject;
    LONG                            actualLength;

    /*
     * pointer to the start of unexamined raw encoding data
     */
    PUCHAR                          pEndBuffer   = NULL;
    PUCHAR                          pEncodingUnit, pBackBuffer;

    pEncodingUnit                   = (PUCHAR)*ppEncoding;

    /* Free the previous value */
    if( pMyObject->bIsDynamic)
    {
        AnscFreeMemory(pMyObject->pStringBuffer);
        pMyObject->uLength          = 0;
        pMyObject->bIsDynamic       = FALSE;
    }

    actualLength    = length;

    /* check the infinite mode first */
    if( length < 0 ) /* infinite mode */
    {
        pEndBuffer  =
            LookingForInfiniteEnd
                (
                    pEncodingUnit,
                    4096
                );

        if( pEndBuffer == NULL)
        {
            AnscTrace("Failed to find the end of infinite mode in AnscAsn1StringDecodingWithoutTag().\n");

            return ANSC_ASN1_INFINITE_END_MISSED;
        }

        actualLength    =  pEndBuffer - pEncodingUnit;           
    }

    /*
     *  allocate memory first, it's no more than actualLength;
     */
    pMyObject->pStringBuffer = AnscAllocateMemory( actualLength + 1);

    if( pMyObject->pStringBuffer == NULL)
    {
        AnscTrace("Failed to allocate memory in constructed decoding\n");

        return ANSC_STATUS_RESOURCES;
    }

    pMyObject->bIsDynamic   = TRUE;
    pBackBuffer             = pEncodingUnit;

    /*
     * Sample constructed encoding of IA5String :
     * 36 13     constructed encoding: "test1" + "@" + "rsa.com"
     * 16 05 74 65 73 74 31
     * 16 01 40
     * 16 07 72 73 61 2e 63 6f 6d
     *
     * Sample constructed encoding of OctetString :
     * 24 0c     constructed encoding: 01 ... 67 + 89 ... ef
     * 04 04 01 23 45 67
     * 04 04 89 ab cd ef
     */

    while( actualLength > 0)
    {
        pNewObject = 
            (PANSC_ASN1_STRING)
                pMyObject->Create
                    (
                        pMyObject->hContainerContext
                    );

        if( pNewObject == NULL)
        {
            AnscTrace("Failed to create a STRING in constructed decoding\n");

            return ANSC_STATUS_RESOURCES;
        }

        returnStatus = 
            pNewObject->DecodingData
                (
                   pNewObject,
                   (PVOID*)&pEncodingUnit
                );

        if( returnStatus != ANSC_STATUS_SUCCESS)
        {
            AnscTrace("Failed to decode the STRING in constructed decoding\n");

            pNewObject->AsnFree(pNewObject);

            return returnStatus;
        }

        /*
         *  recalculate the length;
         */
        actualLength     -=  (pEncodingUnit - pBackBuffer);
        pBackBuffer      =   pEncodingUnit;

        /*
         *  Copy the string value;
         */
        AnscCopyMemory
            (
                (PVOID)((PUCHAR)pMyObject->pStringBuffer + pMyObject->uLength),
                pNewObject->pStringBuffer,
                pNewObject->uLength
            );

        pMyObject->uLength          += pNewObject->uLength;

        /*
         * don't forget to remove the sub object.
         */
        pNewObject->AsnFree(pNewObject);
    }

    /* 
     * don't forget to add the last 2 '0x00' in
     * infinite mode
     */
    if( length < 0 ) /* infinite mode */
    {
        pEncodingUnit               += 2;
    }

    *ppEncoding                     = (PVOID)pEncodingUnit;

    /*
     *  Set the optional state;
     */
    pMyObject->bOptional            = FALSE;

    return ANSC_STATUS_SUCCESS;
}

ANSC_STATUS
AnscAsn1StringDecodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    )
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PANSC_ASN1_STRING               pMyObject    = (PANSC_ASN1_STRING)hThisObject;
    PANSC_TAGINFO_OBJECT            pTagInfo     = NULL;
    BOOLEAN                         bTagIncluded = TRUE;
    ULONG                           uLeftSize,uInfiniteTags;
    PUCHAR                          pEndBuffer   = NULL;
    ULONG                           uValueLength;

    /*
     * pointer to the start of unexamined raw encoding data
     */
    PUCHAR                          pEncodingUnit;

    /* check the parameters */
    if( hThisObject == NULL  || ppEncoding == NULL)
    {
        return ANSC_ASN1_BAD_PARAMETER;
    }

    /*
     *  check before decoding;
     */
    if( pMyObject->BeforeDecoding != NULL)
    {
        returnStatus = pMyObject->BeforeDecoding(pMyObject, ppEncoding);

        if( ANSC_STATUS_SUCCESS != returnStatus)
        {
            return returnStatus;
        }
    }

    /* parsing the data */
    pEncodingUnit   = (PUCHAR)*ppEncoding;

    /*
     * Decode the attrlist first;
     */
    returnStatus = 
        AttrListDecodingData
            (
                &pMyObject->sAttrList,
                (PVOID*)&pEncodingUnit,
                pMyObject->IsConstructive(pMyObject),
                &bTagIncluded,
                &uLeftSize,
                &uInfiniteTags
            );
    
    if( ANSC_STATUS_SUCCESS != returnStatus)
    {
        return returnStatus;
    }

    /* if tag included, decode the tag */
    if( bTagIncluded)
    {
        pTagInfo        = (PANSC_TAGINFO_OBJECT)AnscTagInfoCreate();

        if( pTagInfo == NULL)
        {
            return ANSC_STATUS_RESOURCES;
        }

        returnStatus    = pTagInfo->ParsingData((ANSC_HANDLE)pTagInfo, pEncodingUnit);
        
        if( returnStatus != ANSC_STATUS_SUCCESS)
        {
            pTagInfo->Remove((ANSC_HANDLE)pTagInfo);

            return returnStatus;
        }

        /* check the tag matched or not */
        if( pTagInfo->FirstOctetOfTag
                != pMyObject->GetFirstOctet((ANSC_HANDLE)pMyObject))
        {
            /* sometimes, the user cannot differenciate the MSString and BMPString, we have to consider 
             * them the same here
             */
            if( pTagInfo->FirstOctetOfTag == ASN1_MSSTRING_TYPE         && 
                pMyObject->GetFirstOctet((ANSC_HANDLE)pMyObject) == ASN1_BMPSTRING_TYPE
              )
            {
                goto GOAHEAD;
            }

            if( pTagInfo->FirstOctetOfTag == ASN1_BMPSTRING_TYPE         && 
                pMyObject->GetFirstOctet((ANSC_HANDLE)pMyObject) == ASN1_MSSTRING_TYPE
              )
            {
                goto GOAHEAD;
            }

            if( pTagInfo->FirstOctetOfTag == 
                (pMyObject->GetFirstOctet((ANSC_HANDLE)pMyObject) | CONSTRUCTED_FORM))
            {
                AnscTrace("**** Constructed Encoding was used in STRING encoding, try to decode it.\n");
            
                pEncodingUnit   += 1 + pTagInfo->GetSizeOfLength(pTagInfo);

                returnStatus    = 
                    decodingConstructedString
                        (
                          hThisObject,
                          (PVOID*)&pEncodingUnit,
                          pTagInfo->ulContentLength
                        );

                pTagInfo->Remove((ANSC_HANDLE)pTagInfo);

                AnscTrace("**** The result of decoding constructed STRING is");
                TraceErrorMessage(returnStatus);

                goto EXIT;
            }

            /*
            AnscTrace
                (
                    "Unexpected tag in ASN1_STRING - '0x%.2X', expected - '0x%.2X'\n",
                    pTagInfo->FirstOctetOfTag,
                    pMyObject->GetFirstOctet((ANSC_HANDLE)pMyObject)
                );
            */
            pTagInfo->Remove((ANSC_HANDLE)pTagInfo);

            return ANSC_ASN1_UNEXPECTED_TAG;
        }

GOAHEAD:

        if( pTagInfo->GetSizeOfTag(pTagInfo) != 1 )
        {
            AnscTrace("The length of BITSTRING tag is unacceptable.\n");

            pTagInfo->Remove((ANSC_HANDLE)pTagInfo);

            return ANSC_ASN1_UNACCEPTABLE_VALUE_SIZE;
        }

        /* check the tag */
        if( pTagInfo->bIsInfinite)
        {
            uLeftSize       = 0;
            uInfiniteTags   ++;
        }
        else
        {
            uLeftSize       = pTagInfo->ulContentLength;
        }

        pEncodingUnit   += 1 + pTagInfo->GetSizeOfLength(pTagInfo);

        pTagInfo->Remove((ANSC_HANDLE)pTagInfo);
    }

    /**************************************************
     *
     *  Decode the value;
     *
     **************************************************/
    /* Free the previous value */
    if( pMyObject->bIsDynamic)
    {
        AnscFreeMemory(pMyObject->pStringBuffer);
        pMyObject->uLength          = 0;
        pMyObject->bIsDynamic       = FALSE;
    }

    uValueLength    = uLeftSize;

    if( uValueLength == 0 && uInfiniteTags > 0)
    {
        /* infinite mode */
        pEndBuffer  =
            LookingForInfiniteEnd
                (
                    pEncodingUnit,
                    4096
                );

        if( pEndBuffer == NULL)
        {
            AnscTrace("Failed to find the end of infinite mode in AnscAsn1StringDecoding().\n");

            return ANSC_ASN1_INFINITE_END_MISSED;
        }

        uValueLength = pEndBuffer - pEncodingUnit;
    }

    /* 
     * zhubin updated here on 09/16/2004 
     *
     * To protect from ASN.1 attack
     */
    if( uValueLength > 20000)
    {
        AnscTrace("The length of string is unreasonable '%lu'\n", uValueLength);

        pMyObject->uLength = 0;

        return ANSC_ASN1_UNACCEPTABLE_VALUE_SIZE;
    }

    pMyObject->uLength          = uValueLength;

    if( pMyObject->uLength < MAXIMUM_OCTETSTRING_OCTETS)
    {
        pMyObject->bIsDynamic   = FALSE;

        if(pMyObject->uLength > 0)
        {
            AnscCopyMemory( pMyObject->pString, pEncodingUnit, pMyObject->uLength);
        }
    }
    else
    {   
        pMyObject->pStringBuffer = (PUCHAR)AnscAllocateMemory(pMyObject->uLength + 1);

        if( pMyObject->pStringBuffer == NULL)
        {
            return ANSC_STATUS_RESOURCES;
        }

        pMyObject->bIsDynamic   = TRUE;
        AnscCopyMemory( pMyObject->pStringBuffer, pEncodingUnit, pMyObject->uLength);
    }

    pEncodingUnit += pMyObject->uLength;

EXIT:

    /*
     *  if succeeded, set the new location.
     */
    if( returnStatus == ANSC_STATUS_SUCCESS)
    {
        /* Don't forget adding the end of those infinite forms */
        while( uInfiniteTags > 0)
        {
            if( pEncodingUnit[0] != 0x00 || pEncodingUnit[1] != 0x00)
            {
                /*
                AnscTrace
                    (
                        "Unexpected End Of Indefinite Content : [%.2X %.2X %.2X %.2X] !\n",
                        pEncodingUnit[0],
                        pEncodingUnit[1],
                        pEncodingUnit[2],
                        pEncodingUnit[3] 
                    );
                */
                returnStatus = ANSC_ASN1_INFINITE_END_MISSED;

                break;                
            }

            pEncodingUnit += 2;
            uInfiniteTags --;
        }
    }

    /*
     *  check after decoding;
     */
    if( returnStatus == ANSC_STATUS_SUCCESS)
    {
        if( pMyObject->AfterDecoding != NULL)
        {
            returnStatus = pMyObject->AfterDecoding(pMyObject, (PVOID*)&pEncodingUnit);

            if( ANSC_STATUS_SUCCESS != returnStatus)
            {
                return returnStatus;
            }
        }

        *ppEncoding             = (PVOID)pEncodingUnit;
        pMyObject->bOptional    = FALSE;
    }

    return returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1StringEncodingData
            (
                ANSC_HANDLE                 hThisObject,
                PVOID*                      ppEncoding
            );

    description:

        This function is decoding data for ASN.1 object.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PVOID*                      ppEncoding
                The pointer of the output binary data

    return:     status of encoding operation

**********************************************************************/
ANSC_STATUS
AnscAsn1StringEncodingData
    (
        ANSC_HANDLE                 hThisObject,
        PVOID*                      ppEncoding
    )
{
    ANSC_STATUS                     returnStatus    = ANSC_STATUS_SUCCESS;
    PANSC_ASN1_STRING               pMyObject       = (PANSC_ASN1_STRING)hThisObject;
    BOOLEAN                         bTagIncluded    = TRUE;
    LONG                            uSizeOfEncoded;
    ULONG                           uLeftSize;

    /*
     * shortcut pointer to a char array
     */
    PUCHAR                          pCharData       = (PUCHAR)*ppEncoding;
    PUCHAR                          pBackData       = pCharData;

    /*
     * If it's optional, don't need encode
     */
    if( pMyObject->bOptional)
    {
        return ANSC_STATUS_SUCCESS;
    }

    /*
     *  check before encoding;
     */
    if( pMyObject->BeforeEncoding != NULL)
    {
        returnStatus = pMyObject->BeforeEncoding(pMyObject, ppEncoding);

        if( ANSC_STATUS_SUCCESS != returnStatus)
        {
            return returnStatus;
        }
    }

    uSizeOfEncoded = pMyObject->GetSizeOfEncoded(pMyObject);

    if( uSizeOfEncoded < 0)
    {
        return ANSC_ASN1_NOT_READY_TO_ENCODE;
    }

    /*
     * Encode all attrlist first; 
     */
    returnStatus = 
        AttrListEncodingData
            (
                &pMyObject->sAttrList,
                (PVOID*)&pCharData,
                (ULONG)uSizeOfEncoded,
                pMyObject->IsConstructive(pMyObject),
                &bTagIncluded,
                &uLeftSize
            );

    if( ANSC_STATUS_SUCCESS != returnStatus)
    {
        return returnStatus;
    }

    /*
     *  Encode the tag first;
     */
    if ( bTagIncluded)
    {
        *pCharData                  = pMyObject->GetFirstOctet(pMyObject);
        pCharData++;

        EncodeLength((PVOID*)&pCharData, pMyObject->uLength);
    }

   /*
    *   Encode the value;
    */
    if( pMyObject->uLength > 0)
    {
        if( pMyObject->bIsDynamic)
        {
            AnscCopyMemory
                (
                    pCharData,
                    pMyObject->pStringBuffer,
                    pMyObject->uLength
                );
        }
        else
        {
            AnscCopyMemory
                (
                    pCharData,
                    pMyObject->pString,
                    pMyObject->uLength
                );
        }
    }
    
    /*
     *  Set the buffer before exit;
     */
    pCharData                       += pMyObject->uLength;

    /*
     *  Set the buffer before exit;
     */
    if( ANSC_STATUS_SUCCESS == returnStatus)
    { 
        if( pCharData - pBackData != uSizeOfEncoded)
        {
            /*
            AnscTrace
                (
                    "Warning in Encoding STRING '%s', the size of encoding is expected to be %d, but it's %d.\n",
                    pMyObject->Name,
                    uSizeOfEncoded,
                    (pCharData - pBackData)
                );
            */
        }

        /*
         *  check after encoding;
         */
        if( pMyObject->AfterEncoding != NULL)
        {
            returnStatus = pMyObject->AfterEncoding(pMyObject, (PVOID*)&pCharData);

            if( ANSC_STATUS_SUCCESS != returnStatus)
            {
                return returnStatus;
            }
        }
        
        *ppEncoding                     =  (PVOID)pCharData;
    }

    return returnStatus;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1StringDumpObject
            (
                ANSC_HANDLE                 hThisObject,
                PCHAR                       pBuffer,
                PULONG                      pLength,
                BOOLEAN                     bIsRoot,
                BOOLEAN                     bShowValue
            );

    description:

        This function dump out the values in a tree structure.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PCHAR                       pBuffer,
                The output buffer;

                PULONG                      pLength,
                The pointer of buffer size;

                BOOLEAN                     bIsRoot,
                The object is the root or not;

                BOOLEAN                     bShowValue
                Specifies showing the value or not;

    return:     status of operation.

**********************************************************************/

static void
writeStringToBuffer
    (
        PCHAR                       pEncoding,
        ULONG                       length,
        PCHAR                       pOutput   
    )
{
    PCHAR                           pBuffer;
    
    if( length <= 32)
    {
        AnscCopyMemory
            (
                pOutput,
                pEncoding,
                length
            );
    }
    else
    {
        AnscCopyMemory
            (
                pOutput,
                pEncoding,
                32
            );

        pBuffer = (PCHAR)(pOutput + 32);

        pBuffer[0] = '.';
        pBuffer[1] = '.';
        pBuffer[2] = '.';
        pBuffer[3] = 0;
    }
}

static void
writeBinaryToBuffer
    (
        PUCHAR                      pEncoding,
        ULONG                       length,
        PCHAR                       pOutput   
    )
{
    PCHAR                           pBuffer;
    
    if( length <= 4)
    {
        AnscFromHexToString
            (
                pEncoding,
                pOutput,
                length,
                FALSE
            );
    }
    else
    {
        AnscFromHexToString
            (
                pEncoding,
                pOutput,
                4,
                FALSE
            );


        pBuffer = (PCHAR)(pOutput + 4*2);

        pBuffer[0] = '.';
        pBuffer[1] = '.';
        pBuffer[2] = '.';
        pBuffer[3] = 0;
    }
}

BOOLEAN
AnscAsn1StringDumpObject
    (
        ANSC_HANDLE                 hThisObject,
        PCHAR                       pBuffer,
        PULONG                      pLength,
        BOOLEAN                     bIsRoot,
        BOOLEAN                     bShowValue
    )
{
    PANSC_ASN1_STRING               pMyObject       = (PANSC_ASN1_STRING)hThisObject;
    CHAR                            pAttrBuffer[512]= { 0 };
    ULONG                           attrLength      = 512;
    PCHAR                           pName;
    CHAR                            pValueBuf[128]  = { 0 };
    PUCHAR                          pMyBuffer;
    errno_t                         rc              = -1;

    if( pBuffer == NULL || pLength == NULL)
    {
        return FALSE;
    }

    AttrListDump( &pMyObject->sAttrList, pAttrBuffer, &attrLength);

    if( bIsRoot)
    {
        pName   = pMyObject->ClassName;
    }
    else
    {
        pName   = pMyObject->Name;
    }

    if( pName == NULL)
    {
        return FALSE;
    }

    if( !bShowValue)
    {
        rc =
            sprintf_s
                (
                    pBuffer,
                    *pLength,
                    "%s ::=%s %s",
                    pName,
                    pAttrBuffer,
                    ASN1Type2String(pMyObject->uType)
                );
        if(rc < EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }
        *pLength = rc;
    }
    else if( pMyObject->bOptional)
    {
        rc =
            sprintf_s
                (
                    pBuffer,
                    *pLength,
                    "%s ::=%s %s (Optional)",
                    pName,
                    pAttrBuffer,
                    ASN1Type2String(pMyObject->uType)
                );
        if(rc < EOK)
        {
            ERR_CHK(rc);
            return FALSE;
        }
        *pLength = rc;
    }
    else
    {
        pMyBuffer = pMyObject->GetValueBuffer(pMyObject);

        if( pMyObject->uLength == 0)
        {
            rc =
                sprintf_s
                    (
                        pBuffer,
                        *pLength,
                        "%s ::=%s %s (L0)",
                        pName,
                        pAttrBuffer,
                        ASN1Type2String(pMyObject->uType)
                    );
            if(rc < EOK)
            {
                ERR_CHK(rc);
                return FALSE;
            }
            *pLength = rc;
        }
        else if( AnscIsReadableString(pMyBuffer, pMyObject->uLength))
        {
            writeStringToBuffer((PCHAR)pMyBuffer, pMyObject->uLength, pValueBuf);

            rc =
                sprintf_s
                    (
                        pBuffer,
                        *pLength,
                        "%s ::=%s %s (L%d, '%s')",
                        pName,
                        pAttrBuffer,
                        ASN1Type2String(pMyObject->uType),
                        (int)pMyObject->uLength,
                        pValueBuf
                    );
            if(rc < EOK)
            {
                ERR_CHK(rc);
                return FALSE;
            }
            *pLength = rc;
        }
        else
        {
            writeBinaryToBuffer(pMyBuffer, pMyObject->uLength, pValueBuf);

            rc =
                sprintf_s
                    (
                        pBuffer,
                        *pLength,
                        "%s ::=%s %s (L%d, 0x%s)",
                        pName,
                        pAttrBuffer,
                        ASN1Type2String(pMyObject->uType),
                        (int)pMyObject->uLength,
                        pValueBuf
                    );
            if(rc < EOK)
            {
                ERR_CHK(rc);
                return FALSE;
            }
            *pLength = rc;
        }
    }

    return  TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOLEAN
        AnscAsn1StringTraceObject
            (
                ANSC_HANDLE                 hThisObject,
                LONG                        layer,
                BOOLEAN                     bShowValue,
                BOOLEAN                     bRecursive
            );

    description:

        This function dump out the values in a tree structure.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                LONG                        layer,
                Specifies the layer of current object.

                BOOLEAN                     bShowValue
                Specifies showing the value or not;

                BOOLEAN                     bRecursive
                Trace recursive or not;

    return:     status of operation.

**********************************************************************/
BOOLEAN
AnscAsn1StringTraceObject
    (
        ANSC_HANDLE                 hThisObject,
        LONG                        layer,
        BOOLEAN                     bShowValue,
        BOOLEAN                     bRecursive
    )
{
    UNREFERENCED_PARAMETER(bRecursive);
    PANSC_ASN1_STRING               pMyObject    = (PANSC_ASN1_STRING)hThisObject;
    CHAR                            pAttrBuffer[512] = { 0 };
    ULONG                           attrLength       = 512;
    CHAR                            pValueBuf[128]   = { 0 };
    PUCHAR                          pMyBuffer;
    ULONG                           type;
    PCHAR                           pName;

    TraceTabs(layer);
    AttrListDump( &pMyObject->sAttrList, pAttrBuffer, &attrLength);

    if( layer  == 0)
    {
        pName   = pMyObject->ClassName;
    }
    else
    {
        pName   = pMyObject->Name;
    }

    if( pName == NULL)
    {
        return FALSE;
    }

    if(bShowValue)
    {
        if( pMyObject->bOptional)
        {
            AnscTrace
                (
                    "%s ::=%s %s (Optional)\n", 
                    pName,
                    pAttrBuffer,
                    ASN1Type2String(pMyObject->uType)
                );
        }
        else if( pMyObject->uLength == 0)
        {
            AnscTrace
                (
                    "%s ::=%s %s (L0)\n",
                    pName,
                    pAttrBuffer,
                    ASN1Type2String(pMyObject->uType)
                );
        }
        else
        {
            pMyBuffer = pMyObject->GetValueBuffer(pMyObject);

            type = pMyObject->uType;

            if( type  == ASN1_BMPSTRING_TYPE ||
                type  == ASN1_MSSTRING_TYPE)
            {
                AnscTrace
                    (
                        "%s ::=%s %s (L%lu,",
                        pName,
                        pAttrBuffer,
                        ASN1Type2String(pMyObject->uType),
                        pMyObject->uLength
                    );
                TraceBMPString( pMyBuffer, pMyObject->uLength);

                AnscTrace("\n");
            }
            else if( AnscIsReadableString(pMyBuffer, pMyObject->uLength))
            {
                writeStringToBuffer((PCHAR)pMyBuffer, pMyObject->uLength, pValueBuf);

                AnscTrace
                    (
                        "%s ::=%s %s (L%lu, '%s')\n",
                        pName,
                        pAttrBuffer,
                        ASN1Type2String(pMyObject->uType),
                        pMyObject->uLength,
                        pValueBuf
                    );
            }
            else
            {
                writeBinaryToBuffer(pMyBuffer, pMyObject->uLength, pValueBuf);

                AnscTrace
                    (
                        "%s ::=%s %s (L%lu, 0x%s)\n",
                        pName,
                        pAttrBuffer,
                        ASN1Type2String(pMyObject->uType),
                        pMyObject->uLength,
                        pValueBuf
                    );
            }

        }
    }
    else
    {
        AnscTrace
            (
                "%s ::=%s %s \n", 
                pName,
                pAttrBuffer,
                ASN1Type2String(pMyObject->uType)
            );
    }

    return  TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1StringSetStringValue
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR                      pData,
                ULONG                       uSize
            );

    description:

        This function set the string value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR                      pData,
                The input binary data value;

                ULONG                       uSize
                The size of the buffer;

    return:     the status of the operation

**********************************************************************/
ANSC_STATUS
AnscAsn1StringSetStringValue
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR                      pData,
        ULONG                       uSize
    )
{

    PANSC_ASN1_STRING               pMyObject    = (PANSC_ASN1_STRING)hThisObject;

    /* Free the previous value */
    if( pMyObject->bIsDynamic)
    {
        AnscFreeMemory(pMyObject->pStringBuffer);
    }

    pMyObject->uLength          = 0;
    pMyObject->bIsDynamic       = FALSE;

    AnscZeroMemory(pMyObject->pString, MAXIMUM_OCTETSTRING_OCTETS);

    if( pData == NULL || uSize == 0)
    {
        return ANSC_STATUS_SUCCESS;
    }

    pMyObject->uLength              = uSize;

    if( pMyObject->uLength < MAXIMUM_OCTETSTRING_OCTETS)
    {
        pMyObject->bIsDynamic       = FALSE;
        AnscCopyMemory( pMyObject->pString, pData, pMyObject->uLength);
    }
    else
    {   
        pMyObject->pStringBuffer = (PUCHAR)AnscAllocateMemory(pMyObject->uLength + 1);

        if( pMyObject->pStringBuffer == NULL)
        {
            pMyObject->uLength      = 0;

            return ANSC_STATUS_RESOURCES;
        }

        pMyObject->bIsDynamic       = TRUE;
        AnscCopyMemory( pMyObject->pStringBuffer, pData, pMyObject->uLength);
    }

    /*
     *  set as no-optional
     */
    pMyObject->bOptional            = FALSE;

    return ANSC_STATUS_SUCCESS;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_STATUS
        AnscAsn1StringGetStringValue
            (
                ANSC_HANDLE                 hThisObject,
                PUCHAR*                     ppData,
                PULONG                      pSize
            );

    description:

         get the string value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

                PUCHAR*                     ppData
                The buffer of the output data;
                if the output data is NULL, it will allocate memory.
                The caller is responsible to release it.
                

                PULONG                       pSize
                The buffer of the output size;

    return:     the status of the operation

**********************************************************************/
ANSC_STATUS
AnscAsn1StringGetStringValue
    (
        ANSC_HANDLE                 hThisObject,
        PUCHAR*                     ppData,
        PULONG                      pSize
    )
{
    PANSC_ASN1_STRING               pMyObject    = (PANSC_ASN1_STRING)hThisObject;
    PUCHAR                          pOutput      = (PUCHAR)*ppData;

    /* 
     * If the caller already allocated the memory, check it if it's
     * big enough here.
     */
    if( pOutput != NULL && pSize != NULL && *pSize < pMyObject->uLength)
    {
        AnscTrace("The output size is not big enough: %lu\n", *pSize);

        return ANSC_ASN1_NO_ENOUGH_SIZE;
    }

    if( pSize != NULL)
    {
        *pSize                          = pMyObject->uLength;
    }

    if( pOutput == NULL)
    {
        pOutput = (PUCHAR)AnscAllocateMemory( pMyObject->uLength + 8);

        *ppData = pOutput;
    }

    if( pOutput == NULL)
    {
        return ANSC_STATUS_RESOURCES;
    }

    if( pMyObject->bIsDynamic)
    {
        AnscCopyMemory( pOutput, pMyObject->pStringBuffer, pMyObject->uLength);
    }
    else
    {
        AnscCopyMemory( pOutput, pMyObject->pString, pMyObject->uLength);
    }

    return ANSC_STATUS_SUCCESS;
}


/**********************************************************************

    caller:     owner of this object

    prototype:

        PUCHAR
        AnscAsn1StringGetValueBuffer
            (
                ANSC_HANDLE                 hThisObject
            );

    description:

         get the string value.

    argument:   ANSC_HANDLE                 hThisObject
                This handle is actually the pointer of this object
                itself.

    return:     the string buffer;

**********************************************************************/
PUCHAR
AnscAsn1StringGetValueBuffer
    (
        ANSC_HANDLE                 hThisObject
    )
{
    PANSC_ASN1_STRING               pMyObject    = (PANSC_ASN1_STRING)hThisObject;


    if( pMyObject == NULL)
    {
        return NULL;
    }

    if( pMyObject->bIsDynamic)
    {
        return pMyObject->pStringBuffer;
    }
    else
    {
        return pMyObject->pString;
    }
}
