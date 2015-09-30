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

    module:	ansc_packet_binary_l2tp.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    copyright:

        Cisco Systems, Inc., 1997 ~ 2001
        All Rights Reserved.

    ---------------------------------------------------------------

    description:

        This wrapper file defines the data structures that resemble
        the binary format of TCP/IP protocols.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Na Li

    ---------------------------------------------------------------

    revision:

        09/21/01    Xuechen Yang    template in ansc_packet_binary.h
        09/30/01    Na Li           initial revision

**********************************************************************/


#ifndef  _L2TM_ANSC_PACKET_BINARY_
#define  _L2TM_ANSC_PACKET_BINARY_

#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack(1)
#endif

#ifdef  __GNUC__

    #ifndef  _struct_pack_
    #define  _struct_pack_                          __attribute__ ((__packed__))
    #endif

#else

    #ifndef  _struct_pack_
    #define  _struct_pack_
    #endif

#endif


/***********************************************************
                 DATA STRUCTURES USED BY L2TP MESSAGE
***********************************************************/

/*
 * L2TP defines a set of messages sent as UDP data to port 1701. The source port 
 * is assigned to any unused port number. the port definition is
 * moved to aother file
 *
#define  L2TP_PORT                                  1701
 */

/* define the mask for control bits in the L2TP message header
 * if the priority bit is set in the data message, the program sends it out 
 * regardless contestion control
 */
#define  L2TP_CTRL_TYPE                             (0x8000) 
#define  L2TP_CTRL_LENGTH                           (0x4000) 
#define  L2TP_CTRL_RESERVED                         (0x34f0) 
#define  L2TP_CTRL_SEQUENCE                         (0x0800) 
#define  L2TP_CTRL_OFFSET                           (0x0200) 
#define  L2TP_CTRL_PRIORITY                         (0x0100) 
#define  L2TP_CTRL_VERSION                          (0x000f) 


/* define L2tp header. A lot of fields are not included since the second field is optional
 * and therefore, the position of the following fields depends on the value of the control bits
 */
typedef  struct
_L2TP_MSG_HEADER
{
    USHORT                          ControlBits;
    USHORT                          Fields[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
L2TP_MSG_HEADER,  *PL2TP_MSG_HEADER;

/*
 * the fixed fields of the L2TP message occupy 6 bytes
 */ 
#define  L2TP_MSG_MAXIMUM_SIZE                      1023
#define  L2TP_MSG_MINIMUM_HEADER_SIZE               6
#define  L2TP_MSG_FIELD_SIZE                        2
#define  L2TP_MSG_ZLB_SIZE                          12


/* define macro to access header fields. for optional fields, the caller has to check if the
 * field exist before getting or setting the value
 */
#define  AnscL2tpIsData(p)                          ( (AnscUshortFromNToH(AnscReadUshort(&p->ControlBits)) & L2TP_CTRL_TYPE) == 0 )
#define  AnscL2tpIsControl(p)                       ( (AnscUshortFromNToH(AnscReadUshort(&p->ControlBits)) & L2TP_CTRL_TYPE) != 0 )
#define  AnscL2tpHasLen(p)                          ( (AnscUshortFromNToH(AnscReadUshort(&p->ControlBits)) & L2TP_CTRL_LENGTH) != 0 )
#define  AnscL2tpIsReservedValid(p)                 ( (AnscUshortFromNToH(AnscReadUshort(&p->ControlBits)) & L2TP_CTRL_RESERVED) == 0 )
#define  AnscL2tpHasSeq(p)                          ( (AnscUshortFromNToH(AnscReadUshort(&p->ControlBits)) & L2TP_CTRL_SEQUENCE) != 0 )
#define  AnscL2tpHasOffset(p)                       ( (AnscUshortFromNToH(AnscReadUshort(&p->ControlBits)) & L2TP_CTRL_OFFSET) != 0 )
#define  AnscL2tpHighPriority(p)                    ( (AnscUshortFromNToH(AnscReadUshort(&p->ControlBits)) & L2TP_CTRL_PRIORITY) != 0 )
#define  AnscL2tpMsgVersion(p)                      (  AnscUshortFromNToH(AnscReadUshort(&p->ControlBits)) & L2TP_CTRL_VERSION )  

#define  AnscL2tpGetControlBits(p)                  AnscUshortFromNToH(AnscReadUshort(&p->ControlBits)) 
#define  AnscL2tpSetControlBits(p, value)           AnscWriteUshort(&p->ControlBits, AnscUshortFromHToN(value))

#define  AnscL2tpGetLength(p)                       ( AnscL2tpHasLen(p)? AnscUshortFromNToH(AnscReadUshort(&p->Fields[0])) : 0 )
#define  AnscL2tpSetLength(p, value)                AnscWriteUshort(&p->Fields[0], AnscUshortFromHToN(value))

#define  AnscL2tmIsZeroLengthBodyMsg(p)             \
         ( AnscL2tpGetLength(p) <= L2TP_MSG_ZLB_SIZE? TRUE : FALSE )

#define  AnscL2tpGetIndexTunnelID(p)                ( AnscL2tpHasLen(p)? 1 : 0 )       
#define  AnscL2tpGetTunnelID(p)                     \
            AnscUshortFromNToH(AnscReadUshort(&p->Fields[AnscL2tpGetIndexTunnelID(p)]))  
#define  AnscL2tpSetTunnelID(p, value)              \
            AnscWriteUshort(&p->Fields[AnscL2tpGetIndexTunnelID(p)], AnscUshortFromHToN(value))          

#define  AnscL2tpGetIndexSessionID(p)               ( AnscL2tpGetIndexTunnelID(p) + 1 )       
#define  AnscL2tpGetSessionID(p)                    \
            AnscUshortFromNToH(AnscReadUshort(&p->Fields[AnscL2tpGetIndexSessionID(p)]))
#define  AnscL2tpSetSessionID(p, value)             \
            AnscWriteUshort(&p->Fields[AnscL2tpGetIndexSessionID(p)], AnscUshortFromHToN(value))          

#define  AnscL2tpGetIndexNs(p)                      ( AnscL2tpGetIndexSessionID(p) + 1 )       
#define  AnscL2tpGetNs(p)                           \
            AnscUshortFromNToH(AnscReadUshort(&p->Fields[AnscL2tpGetIndexNs(p)]))
#define  AnscL2tpSetNs(p, value)                    \
            AnscWriteUshort(&p->Fields[AnscL2tpGetIndexNs(p)], AnscUshortFromHToN(value))          

#define  AnscL2tpGetIndexNr(p)                      ( AnscL2tpGetIndexNs(p) + 1 )       
#define  AnscL2tpGetNr(p)                           \
            AnscUshortFromNToH(AnscReadUshort(&p->Fields[AnscL2tpGetIndexNr(p)]))
#define  AnscL2tpSetNr(p, value)                    \
            AnscWriteUshort(&p->Fields[AnscL2tpGetIndexNr(p)], AnscUshortFromHToN(value))          

#define  AnscL2tpGetIndexOffsetSize(p)              \
            ( AnscL2tpHasSeq(p)?  AnscL2tpGetIndexNr(p) + 1 : AnscL2tpGetIndexSessionID(p) + 1  )
       
#define  AnscL2tpGetOffsetSize(p)                   \
            AnscUshortFromNToH(AnscReadUshort(&p->Fields[AnscL2tpGetIndexOffsetSize(p)]))
  
#define  AnscL2tpSetOffsetSize(p, value)            \
            AnscWriteUshort(&p->Fields[AnscL2tpGetIndexNr(p)], AnscUshortFromHToN(value))          


/***********************************************************
                 DATA STRUCTURES USED BY L2TP AVP
***********************************************************/

#define  L2TP_AVP_CTRL_MANDATORY                    (0x8000) 
#define  L2TP_AVP_CTRL_HIDE                         (0x4000) 
#define  L2TP_AVP_CTRL_RESERVED                     (0x3c00)
#define  L2TP_AVP_CTRL_LENGTH                       (0x03ff)  

/*
 * define the Attribute Value Pair (AVP) header
 */
typedef  struct
_L2TP_AVP_HEADER
{
    USHORT                          ControlBits;
    USHORT                          VendorID;
    USHORT                          AttributeType;
    UCHAR                           AvpFields[ANSC_ZERO_ARRAY_SIZE];
}_struct_pack_
L2TP_AVP_HEADER,  *PL2TP_AVP_HEADER;

/*
 * the fixed fields of the L2TP AVP occupy 6 bytes
 */ 
#define  L2TP_AVP_MAXIMUM_SIZE                      1023
#define  L2TP_AVP_MINIMUM_SIZE                      6
#define  L2TP_AVP_FIELD_SIZE                        2


/*
 * define the macro to access header fields
 */

#define  AnscL2tpAvpGetControlBits(p)               AnscUshortFromNToH(AnscReadUshort(&p->ControlBits)) 
#define  AnscL2tpAvpSetControlBits(p, value)        AnscWriteUshort(&p->ControlBits, AnscUshortFromHToN(value))

#define  AnscL2tpAvpIsMandatory(p)                  \
            ( (AnscUshortFromNToH(AnscReadUshort(&p->ControlBits)) & L2TP_AVP_CTRL_MANDATORY) != 0 )

#define  AnscL2tpAvpSetMandatory(p)                 \
            AnscWriteUshort(&p->ControlBits, AnscReadUshort(&p->ControlBits) | AnscUshortFromHToN(L2TP_AVP_CTRL_MANDATORY))

#define  AnscL2tpAvpClearMandatory(p)               \
            AnscWriteUshort(&p->ControlBits, AnscReadUshort(&p->ControlBits) & AnscUshortFromHToN(~L2TP_AVP_CTRL_MANDATORY) )

#define  AnscL2tpAvpIsHide(p)                       \
            ( (AnscUshortFromNToH(AnscReadUshort(&p->ControlBits)) & L2TP_AVP_CTRL_HIDE) != 0 )

#define  AnscL2tpAvpSetHide(p)                      \
            AnscWriteUshort(&p->ControlBits, AnscReadUshort(&p->ControlBits) | AnscUshortFromHToN(L2TP_AVP_CTRL_HIDE))

#define  AnscL2tpAvpClearHide(p)                    \
            AnscWriteUshort(&p->ControlBits, AnscReadUshort(&p->ControlBits) & AnscUshortFromHToN(~L2TP_AVP_CTRL_HIDE) )

#define  AnscL2tpAvpIsReservedValid(p)               \
            ( (AnscUshortFromNToH(AnscReadUshort(&p->ControlBits)) & L2TP_AVP_CTRL_RESERVED) == 0 )

#define  AnscL2tpAvpGetLength(p)                    \
            ( AnscUshortFromNToH(AnscReadUshort(&p->ControlBits)) & L2TP_AVP_CTRL_LENGTH )
  
#define  AnscL2tpAvpSetLength(p, value)             \
            AnscWriteUshort( &p->ControlBits,       \
                             AnscUshortFromHToN((USHORT)((AnscL2tpAvpGetControlBits(p) & (USHORT)(~L2TP_AVP_CTRL_LENGTH)) | (USHORT)value)))

#define  AnscL2tpAvpGetVendorID(p)                  AnscUshortFromNToH(AnscReadUshort(&p->VendorID))
#define  AnscL2tpAvpSetVendorID(p, value)           AnscWriteUshort(&p->VendorID, AnscUshortFromHToN(value))         

#define  AnscL2tpAvpGetAttributeType(p)             AnscUshortFromNToH(AnscReadUshort(&p->AttributeType))
#define  AnscL2tpAvpSetAttributeType(p, value)      AnscWriteUshort(&p->AttributeType, AnscUshortFromHToN(value))
#define  AnscL2tpAvpGetAttrHandle(p)                ( p->AvpFields )

#define  AnscL2tpAvpGetAttrValueUshort(p, fi)        AnscUshortFromNToH(AnscReadUshort(&((p)->AvpFields[fi])) )
#define  AnscL2tpAvpGetAttrValueUlong(p, fi)         AnscUlongFromNToH (AnscReadUlong (&((p)->AvpFields[fi])) )




#ifdef   _ANSC_ENABLE_PRAGMA_
#pragma  pack()
#endif


#endif
