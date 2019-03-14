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

    module:	dslh_ifo_ftm.h

        For DSL Home Model Implementation (DSLH),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines the base class data structure and
        interface for the Dslh File Transfer Manager Object.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        09/16/05    initial revision.

**********************************************************************/


#ifndef  _DSLH_IFO_FTM_
#define  _DSLH_IFO_FTM_


/*
 * This object is derived a virtual base object defined by the underlying framework. We include the
 * interface header files of the base object here to shield other objects from knowing the derived
 * relationship between this object and its base class.
 */
#include "ansc_ifo_interface.h"


/***********************************************************
      DSLH FILE TRANSFER MANAGER INTERFACE DEFINITION
***********************************************************/

/*
 * Define some const values that will be used in the os wrapper object definition.
 */
#define  DSLH_FTM_INTERFACE_NAME                    "dslhFileTransferManagerIf"
#define  DSLH_FTM_INTERFACE_ID                      0

/*
 * Since we write all kernel modules in C (due to better performance and lack of compiler support), we
 * have to simulate the C++ object by encapsulating a set of functions inside a data structure.
 */
typedef  ANSC_STATUS
(*PFN_DSLHFTMIF_DOWNLOAD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hDownloadReq,
        int*                        piStatus,
        ANSC_HANDLE*                phStartTime,
        ANSC_HANDLE*                phCompleteTime,
        ANSC_HANDLE*                phSoapFault
    );

typedef  ANSC_STATUS
(*PFN_DSLHFTMIF_UPLOAD)
    (
        ANSC_HANDLE                 hThisObject,
        ANSC_HANDLE                 hUploadReq,
        int*                        piStatus,
        ANSC_HANDLE*                phStartTime,
        ANSC_HANDLE*                phCompleteTime,
        ANSC_HANDLE*                phSoapFault
    );

typedef  ANSC_STATUS
(*PFN_DSLHFTMIF_GET_TRANS)
    (
        ANSC_HANDLE                 hThisObject,
        void**                      ppQueuedTransferArray,
        PULONG                      pulArraySize,
        ANSC_HANDLE*                phSoapFault
    );

/*
 * If the CPE is instructed to perform a file transfer via the Download or Upload request from the
 * ACS, and if the file location is specified as an HTTP URL with the same host name as the ACS,
 * then the CPE MAY choose any of the following approaches in performing the transfer:
 *
 *      - The CPE MAY send the HTTP get/post over the already established connection.
 *        Once the file has been transferred, the CPE MAY then proceed in sending addi-
 *        tional messages to the ACS while continuing to maintain the connection.
 *      - The CPE MAY open a second connection over which to transfer the file, while
 *        maintaining the session to the ACS over which it may continue to send messages.
 *      - The CPE MAY terminate the session tot he ACS and then perform the transfer.
 *
 * If the file location is not an HTTP URL or is not in the same domain as the ACS, then only the
 * latter two options are available to it.
 */
#define  DSLH_FTM_INTERFACE_CLASS_CONTENT                                                   \
    /* duplication of the base object class content */                                      \
    ANSCIFO_CLASS_CONTENT                                                                   \
    /* start of object class content */                                                     \
    PFN_DSLHFTMIF_DOWNLOAD          Download;                                               \
    PFN_DSLHFTMIF_UPLOAD            Upload;                                                 \
    PFN_DSLHFTMIF_GET_TRANS         GetQueuedTransfers;                                     \
    /* end of object class content */                                                       \

typedef  struct
_DSLH_FTM_INTERFACE
{
    DSLH_FTM_INTERFACE_CLASS_CONTENT
}
DSLH_FTM_INTERFACE,  *PDSLH_FTM_INTERFACE;

#define  ACCESS_DSLH_FTM_INTERFACE(p)               \
         ACCESS_CONTAINER(p, DSLH_FTM_INTERFACE, Linkage)


/***********************************************************
      DSLH FILE TRANSFER MANAGER PARAMETERS DEFINITION
***********************************************************/

/*
 * The Download() method may be used by the Server to cause the CPE to download a specified file
 * from the designated location. Three values are currently defined for the 'FileType' argument:
 *
 *      - "1 Firmware Upgrade Image"
 *      - "2 Web Content"
 *      - "3 Vendor Configuration File"
 *
 * The following format is defined to allow the unique definition of vendor-specific file types:
 *
 *      "X <OUI> <Vendor-specific identifier>
 * 
 */
#define  DSLH_FTM_DW_FILE_TYPE_unknown              0
#define  DSLH_FTM_DW_FILE_TYPE_firmwareUpgrade      1
#define  DSLH_FTM_DW_FILE_TYPE_vendorConfigFile     2
#define  DSLH_FTM_DW_FILE_TYPE_application          3

#define  DSLH_FTM_DW_FILE_TYPE_VendorSpecific       0x10000         /* Vendor Specific File Type */


typedef  struct
_DSLH_FTM_DOWNLOAD_REQ
{
    SINGLE_LINK_ENTRY               Linkage;
    ANSC_HANDLE                     hFileManager;
    char*                           CommandKey;
    ULONG                           FileType;
    ULONG                           FileTypeVsOui;
    char*                           FileTypeVsVsi;
    char*                           Url;
    char*                           Username;
    char*                           Password;
    ULONG                           FileSize;
    char*                           TargetFileName;
    ULONG                           DelaySeconds;
    char*                           SuccessUrl;
    char*                           FailureUrl;
}
DSLH_FTM_DOWNLOAD_REQ,  *PDSLH_FTM_DOWNLOAD_REQ;

#define  ACCESS_DSLH_FTM_DOWNLOAD_REQ(p)            \
         ACCESS_CONTAINER(p, DSLH_FTM_DOWNLOAD_REQ, Linkage)

#define  DslhFtmCleanDownloadReq(download_req)                                              \
         {                                                                                  \
            if ( download_req->CommandKey )                                                 \
            {                                                                               \
                AnscFreeMemory(download_req->CommandKey);                                   \
                                                                                            \
                download_req->CommandKey = NULL;                                            \
            }                                                                               \
                                                                                            \
            if ( download_req->FileTypeVsVsi )                                              \
            {                                                                               \
                AnscFreeMemory(download_req->FileTypeVsVsi);                                \
                                                                                            \
                download_req->FileTypeVsVsi = NULL;                                         \
            }                                                                               \
                                                                                            \
            if ( download_req->Url )                                                        \
            {                                                                               \
                AnscFreeMemory(download_req->Url);                                          \
                                                                                            \
                download_req->Url = NULL;                                                   \
            }                                                                               \
                                                                                            \
            if ( download_req->Username )                                                   \
            {                                                                               \
                AnscFreeMemory(download_req->Username);                                     \
                                                                                            \
                download_req->Username = NULL;                                              \
            }                                                                               \
                                                                                            \
            if ( download_req->Password )                                                   \
            {                                                                               \
                AnscFreeMemory(download_req->Password);                                     \
                                                                                            \
                download_req->Password = NULL;                                              \
            }                                                                               \
                                                                                            \
            if ( download_req->TargetFileName )                                             \
            {                                                                               \
                AnscFreeMemory(download_req->TargetFileName);                               \
                                                                                            \
                download_req->TargetFileName = NULL;                                        \
            }                                                                               \
                                                                                            \
            if ( download_req->SuccessUrl )                                                 \
            {                                                                               \
                AnscFreeMemory(download_req->SuccessUrl);                                   \
                                                                                            \
                download_req->SuccessUrl = NULL;                                            \
            }                                                                               \
                                                                                            \
            if ( download_req->FailureUrl )                                                 \
            {                                                                               \
                AnscFreeMemory(download_req->FailureUrl);                                   \
                                                                                            \
                download_req->FailureUrl = NULL;                                            \
            }                                                                               \
         }

#define  DslhFtmFreeDownloadReq(download_req)                                               \
         {                                                                                  \
            DslhFtmCleanDownloadReq(download_req)                                           \
            AnscFreeMemory         (download_req);                                          \
         }

/*
 * The Upload() method may be used by the SErver to cause the CPE to upload a specified file to the
 * designated location. Two values are currently defined for the 'FileType' argument:
 *
 *      - "1 Vendor Configuration File"
 *      - "2 Vendor Log File"
 *
 * The following format is defined to allow the unique definition of vendor-specific file types:
 *
 *      "X <OUI> <Vendor-specific identifier>
 */
#define  DSLH_FTM_UP_FILE_TYPE_vendorConfigFile     101
#define  DSLH_FTM_UP_FILE_TYPE_vendorLogFile        102

#define  DSLH_FTM_UP_FILE_TYPE_VendorSpecific       0x10000


typedef  struct
_DSLH_FTM_UPLOAD_REQ
{
    SINGLE_LINK_ENTRY               Linkage;
    ANSC_HANDLE                     hFileManager;
    char*                           CommandKey;
    ULONG                           FileType;
    ULONG                           FileTypeVsOui;
    char*                           FileTypeVsVsi;
    char*                           Url;
    char*                           Username;
    char*                           Password;
    ULONG                           DelaySeconds;
}
DSLH_FTM_UPLOAD_REQ,  *PDSLH_FTM_UPLOAD_REQ;

#define  ACCESS_DSLH_FTM_UPLOAD_REQ(p)              \
         ACCESS_CONTAINER(p, DSLH_FTM_UPLOAD_REQ, Linkage)

#define  DslhFtmCleanUploadReq(upload_req)                                                  \
         {                                                                                  \
            if ( upload_req->CommandKey )                                                   \
            {                                                                               \
                AnscFreeMemory(upload_req->CommandKey);                                     \
                                                                                            \
                upload_req->CommandKey = NULL;                                              \
            }                                                                               \
                                                                                            \
            if ( upload_req->FileTypeVsVsi )                                                \
            {                                                                               \
                AnscFreeMemory(upload_req->FileTypeVsVsi);                                  \
                                                                                            \
                upload_req->FileTypeVsVsi = NULL;                                           \
            }                                                                               \
                                                                                            \
            if ( upload_req->Url )                                                          \
            {                                                                               \
                AnscFreeMemory(upload_req->Url);                                            \
                                                                                            \
                upload_req->Url = NULL;                                                     \
            }                                                                               \
                                                                                            \
            if ( upload_req->Username )                                                     \
            {                                                                               \
                AnscFreeMemory(upload_req->Username);                                       \
                                                                                            \
                upload_req->Username = NULL;                                                \
            }                                                                               \
                                                                                            \
            if ( upload_req->Password )                                                     \
            {                                                                               \
                AnscFreeMemory(upload_req->Password);                                       \
                                                                                            \
                upload_req->Password = NULL;                                                \
            }                                                                               \
         }

#define  DslhFtmFreeUploadReq(upload_req)                                                   \
         {                                                                                  \
            DslhFtmCleanUploadReq(upload_req)                                               \
            AnscFreeMemory       (upload_req);                                              \
         }


#endif
