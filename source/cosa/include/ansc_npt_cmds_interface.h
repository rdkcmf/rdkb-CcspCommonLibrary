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

    module:	ansc_npt_cmds_interface.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines all the platform-independent
        functions and macros for variable Npt (Network Packet
        Transformation) Command Objects.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/11/01    initial revision.

**********************************************************************/


#ifndef  _ANSC_NPT_CMDS_INTERFACE_
#define  _ANSC_NPT_CMDS_INTERFACE_


/***********************************************************
     PLATFORM INDEPENDENT NPT COMMAND OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the policy node object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */

/*
 * Different packet transformation command may require different parameters associated with it. The
 * parameters may come from different places as well: for byte/word/dword replacement and insertion,
 * the values can be specified within the command object while the string-related operation have to
 * use the scratch pad to do business. We're not trying to design a full-fledged packet-processing
 * silicon chip, but this architecture does give us the flexibility to adapt our technology into
 * the higher end platforms.
 */
#define  ANSC_NPT_CMD_NULL_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    ANSC_NPT_CMD_CLASS_CONTENT                                                              \
    /* start of object class content */                                                     \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_NPT_CMD_NULL_OBJECT
{
    ANSC_NPT_CMD_NULL_CLASS_CONTENT
}
ANSC_NPT_CMD_NULL_OBJECT,  *PANSC_NPT_CMD_NULL_OBJECT;

#define  ACCESS_ANSC_NPT_CMD_NULL_OBJECT(p)         \
         ACCESS_CONTAINER(p, ANSC_NPT_CMD_NULL_OBJECT, Linkage)


/***********************************************************
     PLATFORM INDEPENDENT NPT COMMAND OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the policy node object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ULONG
(*PFN_NCOINSB_GET_OFFSET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_NCOINSB_SET_OFFSET)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       offset
    );

typedef  UCHAR
(*PFN_NCOINSB_GET_BYTE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_NCOINSB_SET_BYTE)
    (
        ANSC_HANDLE                 hThisObject,
        UCHAR                       byte
    );

/*
 * Different packet transformation command may require different parameters associated with it. The
 * parameters may come from different places as well: for byte/word/dword replacement and insertion,
 * the values can be specified within the command object while the string-related operation have to
 * use the scratch pad to do business. We're not trying to design a full-fledged packet-processing
 * silicon chip, but this architecture does give us the flexibility to adapt our technology into
 * the higher end platforms.
 */
#define  ANSC_NPT_CMD_INSB_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    ANSC_NPT_CMD_CLASS_CONTENT                                                              \
    /* start of object class content */                                                     \
    ULONG                           Offset;                                                 \
    UCHAR                           Byte;                                                   \
                                                                                            \
    PFN_NCOINSB_GET_OFFSET          GetOffset;                                              \
    PFN_NCOINSB_SET_OFFSET          SetOffset;                                              \
    PFN_NCOINSB_GET_BYTE            GetByte;                                                \
    PFN_NCOINSB_SET_BYTE            SetByte;                                                \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_NPT_CMD_INSB_OBJECT
{
    ANSC_NPT_CMD_INSB_CLASS_CONTENT
}
ANSC_NPT_CMD_INSB_OBJECT,  *PANSC_NPT_CMD_INSB_OBJECT;

#define  ACCESS_ANSC_NPT_CMD_INSB_OBJECT(p)         \
         ACCESS_CONTAINER(p, ANSC_NPT_CMD_INSB_OBJECT, Linkage)


/***********************************************************
     PLATFORM INDEPENDENT NPT COMMAND OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the policy node object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_NCOGENP_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_NCOGENP_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

typedef  ULONG
(*PFN_NCOGENP_GET_OFFSET)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_NCOGENP_SET_OFFSET)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       offset
    );

typedef  ULONG
(*PFN_NCOGENP_GET_SIZE)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_NCOGENP_SET_SIZE)
    (
        ANSC_HANDLE                 hThisObject,
        ULONG                       size
    );

/*
 * Different packet transformation command may require different parameters associated with it. The
 * parameters may come from different places as well: for byte/word/dword replacement and insertion,
 * the values can be specified within the command object while the string-related operation have to
 * use the scratch pad to do business. We're not trying to design a full-fledged packet-processing
 * silicon chip, but this architecture does give us the flexibility to adapt our technology into
 * the higher end platforms.
 */
#define  ANSC_NPT_CMD_GENP_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    ANSC_NPT_CMD_CLASS_CONTENT                                                              \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hSpaDescriptor;                                         \
    ULONG                           Offset;                                                 \
    ULONG                           Size;                                                   \
                                                                                            \
    PFN_NCOGENP_GET_CONTEXT         GetSpaDescriptor;                                       \
    PFN_NCOGENP_SET_CONTEXT         SetSpaDescriptor;                                       \
    PFN_NCOGENP_GET_OFFSET          GetOffset;                                              \
    PFN_NCOGENP_SET_OFFSET          SetOffset;                                              \
    PFN_NCOGENP_GET_SIZE            GetSize;                                                \
    PFN_NCOGENP_SET_SIZE            SetSize;                                                \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_NPT_CMD_GENP_OBJECT
{
    ANSC_NPT_CMD_GENP_CLASS_CONTENT
}
ANSC_NPT_CMD_GENP_OBJECT,  *PANSC_NPT_CMD_GENP_OBJECT;

#define  ACCESS_ANSC_NPT_CMD_GENP_OBJECT(p)         \
         ACCESS_CONTAINER(p, ANSC_NPT_CMD_GENP_OBJECT, Linkage)


/***********************************************************
     PLATFORM INDEPENDENT NPT COMMAND OBJECT DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the policy node object definition.
 */

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support),
 * we have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_HANDLE
(*PFN_NCOMOVP_GET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject
    );

typedef  ANSC_STATUS
(*PFN_NCOMOVP_SET_CONTEXT)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hContext
    );

/*
 * Different packet transformation command may require different parameters associated with it. The
 * parameters may come from different places as well: for byte/word/dword replacement and insertion,
 * the values can be specified within the command object while the string-related operation have to
 * use the scratch pad to do business. We're not trying to design a full-fledged packet-processing
 * silicon chip, but this architecture does give us the flexibility to adapt our technology into
 * the higher end platforms.
 */
#define  ANSC_NPT_CMD_MOVP_CLASS_CONTENT                                                    \
    /* duplication of the base object class content */                                      \
    ANSC_NPT_CMD_CLASS_CONTENT                                                              \
    /* start of object class content */                                                     \
    ANSC_HANDLE                     hPqaDescriptor;                                         \
                                                                                            \
    PFN_NCOMOVP_GET_CONTEXT         GetPqaDescriptor;                                       \
    PFN_NCOMOVP_SET_CONTEXT         SetPqaDescriptor;                                       \
    /* end of object class content */                                                       \

typedef  struct
_ANSC_NPT_CMD_MOVP_OBJECT
{
    ANSC_NPT_CMD_MOVP_CLASS_CONTENT
}
ANSC_NPT_CMD_MOVP_OBJECT,  *PANSC_NPT_CMD_MOVP_OBJECT;

#define  ACCESS_ANSC_NPT_CMD_MOVP_OBJECT(p)         \
         ACCESS_CONTAINER(p, ANSC_NPT_CMD_MOVP_OBJECT, Linkage)


#endif
