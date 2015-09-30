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

/*
* 
*  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*  !!!                                                                     !!!
*  !!!  NOTE:                                                              !!!
*  !!!                                                                     !!!
*  !!!  The contents of this document constitute CONFIDENTIAL INFORMATION  !!!
*  !!!  which is the property of Hi/fn, Inc.                           !!!
*  !!!                                                                     !!!
*  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*
*/
/*  TITLE   LZSC.H  -  Function prototypes and constants for DCS221C
*
*$Header: /Product/BroadWay/Bisga/wrapper/AnscCrypto/crypto/lzs/lzsc.h 2     8/05/05 3:34p Zhubin $
*
******************************************************************************
*
* Copyright 1988-92 Hi/fn, Inc., San Jose, California.
* All rights reserved.  This code is Stac confidential property, and the
* algorithms and methods used herein may not be disclosed to any party without
* express written consent from Hi/fn, Inc.
*
******************************************************************************
*
*
* Filename:          LZSC.H
*
* Project:           DCS221-C
*
* Functional Block:  DCS221-C
*
* Comments:          The following variable types are assumed to have the
*                    sizes listed below:
*
*                    char   =  8 bits
*                    short  = 16 bits
*                    long   = 32 bits
*
*
* Contents:     Symbolic Constants
*               Performance Tuning Declarations
*               Processor Specific Macros
*               Function Prototypes
*
*****************************************************************************
*
* Version Control Information:
*
*    Rev 3.01  01 Dec 1992 17:38:00   beb
*     Added GetABit function to speed up LZS_Decompress by 20%.
*
*    Rev 3.00   11 Nov 1992 16:48:40   lap
*     Added performance tuning parameters and removed indirection for
*     greater speed performance.
*
*    Rev 1.00   01 Apr 1992 14:07:04   beb
*     Initial revision.
*
*
*
****************************************************************************/

/*

******************************************************************************
*
* Block Name:        Target Processor Identification Macro
*
* Functional Block:  DCS221-C
*
* Special Notes:     User should select a compiler type from this list
*
*****************************************************************************/

#define LZS_ANSI            1       /* February 1990 ANSI */
#define LZS_INTEL_86        2       /* Borland C++ 3.1 */

/*#define   LZS_TARGET          LZS_INTEL_86*/  /* selected from the list above */
#define LZS_TARGET          LZS_ANSI            /* change this to eliminate the obsolete keyword "far" */

/*

****************************************************************************
*
* Block Name:        Processor Specific Data Types
*
* Functional Block:  DCS221-C
*
* Special Notes:
*
*****************************************************************************/

#if     LZS_TARGET == LZS_ANSI

#define LZS_FAR
#define LZS_HANDLE          * *

#elif   LZS_TARGET == LZS_INTEL_86

#define LZS_FAR             far
#define LZS_HANDLE          far * far *

#endif


/*

****************************************************************************
*
* Block Name:   Shorthand Declarations
*
* Functional Block: DCS221-C
*
* Special Notes:    
*
*****************************************************************************/

/* shorthand declarations */            /* size assumptions */

/*

****************************************************************************
*
* Block Name:        Symbolic Constants
*
* Functional Block:  DCS221-C
*
* Special Notes:
*
*****************************************************************************/


/* Bit definitions for the flags parameter */

#define     LZS_NO_CLEAR            0x04
#define     LZS_DEST_FLUSH          0x02
#define     LZS_SOURCE_FLUSH        0x01

#define     LZS_RESET               0x01


/* Return codes */

#define LZS_INVALID                 0
#define LZS_SOURCE_EXHAUSTED        1
#define LZS_DEST_EXHAUSTED          2
#define LZS_SOURCE_DEST_EXHAUSTED   3
#define LZS_FLUSHED                 4   /* flushed at end of compression    */
#define LZS_END_MARKER              4   /* hit eocd during decompression    */


/* Other */

#define LZS_DEST_MIN                15

#define LZS_HISTORY_SIZE            24752   /* Size of history scratch RAM */


/*

****************************************************************************
*
* Block Name:        Performance Tuning Declarations
*
* Functional Block:  DCS221-C
*
* Special Notes:     The speed and compression ratio of the LZS_Compress
*                    function can be tuned by adjusting the values of the
*                    performance parameter and the LZS_PERFORMANCE_MODE
*                    bits of the flags parameter.  Valid values for the
*                    performance parameter are 0 thru 255.  Valid values
*                    for the LZS_PERFORMANCE_MODE BITS are 0 thru 2.
*                    Setting these parameters to the lowest possible values
*                    results in the fastest speed.  Setting these parameters
*                    to the highest possible values results in the
*                    greatest compression ratios.
*
*****************************************************************************/

/* Possible settings for the performance mode flag bits */

#define LZS_PERF_MODE_0         0X0000
#define LZS_PERF_MODE_1         0x0008
#define LZS_PERF_MODE_2         0x0010

#define LZS_PERF_SHIFT          3   /* # of positions to shift performance  */
                                    /* mode bits for right justification    */

#define LZS_PERF_MASK           0X0018  /* mask for extracting performance  */
                                        /* mode bits                        */


/*

*****************************************************************************
*
* Block Name:        Function Prototypes
*
* Functional Block:  DCS221-C
*
* Special Notes:
*
*****************************************************************************/

void LZS_FAR LZS_InitHistory(
                            void LZS_FAR    *scratch
                            );


unsigned short LZS_FAR  LZS_Compress(
                                    unsigned char   LZS_HANDLE   sourceHandle,
                                    unsigned char   LZS_HANDLE   destHandle,
                                    unsigned long   LZS_FAR     *sourceCount,
                                    unsigned long   LZS_FAR     *destCount,
                                    void            LZS_FAR     *scratch,
                                    unsigned short               flags,
                                    unsigned short               performance
                                    );


unsigned short LZS_FAR  LZS_Decompress(
                                      unsigned char  LZS_HANDLE  sourceHandle,
                                      unsigned char  LZS_HANDLE  destHandle,
                                      unsigned long  LZS_FAR    *sourceCount,
                                      unsigned long  LZS_FAR    *destCount,
                                      void           LZS_FAR    *scratch,
                                      unsigned short             flags
                                      );


/****************************************************************************
*                           End - of - File                                 *
****************************************************************************/

