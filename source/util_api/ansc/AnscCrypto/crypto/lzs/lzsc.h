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

