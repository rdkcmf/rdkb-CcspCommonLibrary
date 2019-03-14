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

    module:	sys_definitions.h

        For BroadWay Runtime System Environment (SYS),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This file defines the system- and package-wide runtime
        environmental variables.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        05/02/02    initial revision.

**********************************************************************/


#ifndef  _SYS_DEFINITIONS_
#define  _SYS_DEFINITIONS_


#include "sys_data_types.h"


/***********************************************************
       SYS INFO REPOSITORY ACCESS MECHANISM DEFINITION
***********************************************************/

/*
 * The Sys Info Repository Object is responsible for creating and managing a system-wide central
 * in-memory database. This database/repository is a tree-like structure, which consists of a set
 * of nodes at each level (only one at the root level). Each node may be associated with several
 * properties, which can be one of the pre-defined data types: integer, string, boolean, and
 * binary string.
 */
#define  SYS_REP_FOLDER_TYPE_STORAGE                1           /* simple provides storage for runtime information  */
#define  SYS_REP_FOLDER_TYPE_GUARDED                2           /* there's a dedicated object guarding the folder   */
#define  SYS_REP_FOLDER_TYPE_DYNAMIC                3           /* the runtime information is generated dynamically */
#define  SYS_REP_FOLDER_TYPE_SESSION                4           /* folder-content management is based open session  */

#define  SYS_REP_RECORD_TYPE_SINT                   1           /* signed integer encapsulated in four octets       */
#define  SYS_REP_RECORD_TYPE_UINT                   2           /* unsigned integer encapsulated in four octets     */
#define  SYS_REP_RECORD_TYPE_BOOL                   3           /* boolean value which is either "true" or "false"  */
#define  SYS_REP_RECORD_TYPE_ASTR                   4           /* ascii string, zero can only be used at the end   */
#define  SYS_REP_RECORD_TYPE_BSTR                   5           /* a stream of octets with any arbitrary value      */
#define  SYS_REP_RECORD_TYPE_HCXT                   6           /* context handle, such as interface or function    */
#define  SYS_REP_RECORD_TYPE_ENUM                   7           /* unsigned integer within a range (not supported)  */

#define  SYS_RFO_ACCESS_MODE_READ                   0x00000001  /* caller wants to read records, open sub-folders   */
#define  SYS_RFO_ACCESS_MODE_RETRIEVE               0x00000001  /* caller wants to read records, open sub-folders   */
#define  SYS_RFO_ACCESS_MODE_SYNCHRONIZE            0x00000002  /* caller wants to generate dynamic folder content  */
#define  SYS_RFO_ACCESS_MODE_DELETE                 0x00000004  /* caller may need to delete the current folder     */
#define  SYS_RFO_ACCESS_MODE_FOLDER_ADD             0x00000008  /* caller may need to create new sub-folders        */
#define  SYS_RFO_ACCESS_MODE_FOLDER_DELETE          0x00000010  /* caller may need to delete existing sub-folders   */
#define  SYS_RFO_ACCESS_MODE_RECORD_ADD             0x00000020  /* caller may need to create new records            */
#define  SYS_RFO_ACCESS_MODE_RECORD_DELETE          0x00000040  /* caller may need to delete existing records       */
#define  SYS_RFO_ACCESS_MODE_RECORD_MODIFY          0x00000080  /* caller may need to modify existing records       */
#define  SYS_RFO_ACCESS_MODE_RDWR                   0x00000083  /* need to retrieve, sync, and modify records       */
#define  SYS_RFO_ACCESS_MODE_STANDARD               0x000000F9  /* exclude access for deletion and synchornization  */
#define  SYS_RFO_ACCESS_MODE_ADVANCED               0x000000FD  /* exclude access for content synchronization       */
#define  SYS_RFO_ACCESS_MODE_ALL                    0x000000FF  /* access for everything, except access flags below */
#define  SYS_RFO_ACCESS_MODE_EXCLUSIVE              0x00010000  /* the access to this folder must be exclusive      */
#define  SYS_RFO_ACCESS_MODE_ADAPTIVE               0x00020000  /* access to everything the folder has to offer     */

#define  SYS_RFO_PERMISSION_PROHIBITED              0x00000000  /* doesn't allow any access at all, but why need it */
#define  SYS_RFO_PERMISSION_READ                    0x00000001  /* records can be read, sub-folders can be opened   */
#define  SYS_RFO_PERMISSION_RETRIEVE                0x00000001  /* records can be read, sub-folders can be opened   */
#define  SYS_RFO_PERMISSION_SYNCHRONIZE             0x00000002  /* content can be dynamically generated in runtime  */
#define  SYS_RFO_PERMISSION_DELETE                  0x00000004  /* this folder itself can be deleted                */
#define  SYS_RFO_PERMISSION_FOLDER_ADD              0x00000008  /* allow sub-folders to be created under this one   */
#define  SYS_RFO_PERMISSION_FOLDER_DELETE           0x00000010  /* allow sub-folders to be deleted in general       */
#define  SYS_RFO_PERMISSION_RECORD_ADD              0x00000020  /* allow record objects to be created               */
#define  SYS_RFO_PERMISSION_RECORD_DELETE           0x00000040  /* allow records to be deleted in general           */
#define  SYS_RFO_PERMISSION_RECORD_MODIFY           0x00000080  /* allow records to be modified in general          */
#define  SYS_RFO_PERMISSION_RDWR                    0x00000083  /* allow retrieval, sync, record-modification       */
#define  SYS_RFO_PERMISSION_STANDARD                0x000000F9  /* except permission for delete and synchornization */
#define  SYS_RFO_PERMISSION_ADVANCED                0x000000FD  /* exclude permission for content synchronization   */
#define  SYS_RFO_PERMISSION_ALL                     0x000000FF  /* you can do anything you want with the folder     */

#define  SYS_RRO_ACCESS_MODE_READ                   0x00000001  /* caller may want to read the record value         */
#define  SYS_RRO_ACCESS_MODE_RETRIEVE               0x00000001  /* caller may want to read the record value         */
#define  SYS_RRO_ACCESS_MODE_MODIFY                 0x00000002  /* caller may want to modified the record content   */
#define  SYS_RRO_ACCESS_MODE_DELETE                 0x00000004  /* caller may want to delete the current record     */
#define  SYS_RRO_ACCESS_MODE_RDWR                   0x00000003  /* caller may want to both read and modify content  */
#define  SYS_RRO_ACCESS_MODE_ALL                    0x00000007  /* caller may want to try anything on the record    */

#define  SYS_RRO_PERMISSION_PROHIBITED              0x00000000  /* doesn't allow any access at all, but why need it */
#define  SYS_RRO_PERMISSION_READ                    0x00000001  /* record content, type, attribute can be read      */
#define  SYS_RRO_PERMISSION_RETRIEVE                0x00000001  /* record content, type, attribute can be read      */
#define  SYS_RRO_PERMISSION_MODIFY                  0x00000002  /* record content can be modified                   */
#define  SYS_RRO_PERMISSION_DELETE                  0x00000004  /* record object can be deleted from its folder     */
#define  SYS_RRO_PERMISSION_RDWR                    0x00000003  /* allow record content to be read and modified     */
#define  SYS_RRO_PERMISSION_ALL                     0x00000007  /* you can do anything you want with the record     */

/*
 * Dynamically allocated string versus a statically defined char array has always been a fierce
 * holy battle for any software development. In most cases, we use a statically defined char array
 * since it's easier to manage and have more memory overhead.
 */
#define  SYS_MAX_DEPENDENT_NUMBER                   32
#define  SYS_MAX_MODULE_NAME_SIZE                   128
#define  SYS_MAX_DRIVER_NAME_SIZE                   128
#define  SYS_MAX_FOLDER_NAME_SIZE                   128
#define  SYS_MAX_RECORD_NAME_SIZE                   160
#define  SYS_MAX_ATTRIB_DATA_SIZE                   128


/***********************************************************
       SYS INFO REPOSITORY RENDER ATTRIBUTE DEFINITION
***********************************************************/

/*
 * While it may sound weird, you can actually assoicate a content-type with a repository folder
 * and record object. The Web-based Management Module interprets the content-type value and renders
 * the display accordingly (i.e., generate the HTML pages that can best represent the attributes of
 * the data). For example: a repository folder that represents a table should set its content-type
 * to table_2d. Every sub-folder will be rendered as a row, content in each sub-folder fills in the
 * row (the folder owner MUST ensure the consistency of each sub-folder). Below is more detailed
 * specification on the content structure.
 *
 *      $ DEFAULT          - the DEFAULT content type shouldn't be associated with any
 *                           rendering attribute since the absence of rendering attribute
 *                           indicates the DEFAULT content type.
 *
 *      $ CATEGORY         - this content type is currently not defined, but reserved for
 *                           future use.
 *
 *      $ TABLE_2D         - this content type mandates a two-level content structure of
 *                           the associated folder:
 *
 *                              @ a record named "Title" SHOULD be present
 *                                directly under the top folder. If this record
 *                                cannot be found, the folder name is used as
 *                                the title instead.
 *
 *                              @ all the subfolders directly under the current
 *                                folder MUST contain the exact same number of
 *                                records. Any record type and name pair found
 *                                in one subfolder MUST be found in others too.
 *
 *                              @ no subfolder can be found under any directly
 *                                managed subfolder.
 *
 *                              @ the number of subfolders MAY be zero.
 *
 *                              @ the number of records SHOULD NOT be zero.
 *
 *      $ TABLE_3D         - this content type mandates a three-level content structure
 *                           of the associated folder:
 *
 *                              @ a record named "Title" SHOULD be present
 *                                directly under the top folder. If this record
 *                                cannot be found, the folder name is used as
 *                                the title instead.
 *
 *                              @ all the subfolders directly under the current
 *                                folder MUST contain the exact same number of
 *                                subfolders. All those folders MUST share the
 *                                same folder type and content type.
 *
 *                              @ the second-level subfolder MUST contain exact
 *                                same set of records.
 *
 *                              @ the first-level subfolder SHOULD NOT contain
 *                                any records.
 *
 *                              @ the number of subfolders (at any level) MAY
 *                                be zero.
 *
 *                              @ the number of records SHOULD NOT be zero.
 *
 *      $ SIMPLE_CHART_PIE - this content type mandates a two-level content structure of
 *                           the associated folder:
 *
 *                              @ a record named "Title" SHOULD be present
 *                                directly under the top folder. If this record
 *                                cannot be found, the folder name is used as
 *                                the title instead.
 *
 *                              @ all the subfolders directly under the current
 *                                folder MUST contain two records: "Value" and
 *                                "Label"; where "Value" record may be either a
 *                                SINT, UINT, or BOOL type (may be REAL as well
 *                                in the future) while "Label" record MUST be
 *                                displayable string (i.e, ASTR record type).
 *
 *                              @ the rendering engine is responsible for cal-
 *                                culating the percentage for each element. The
 *                                total amount is calculated by adding up all
 *                                the values.
 *
 *      $ SIMPLE_CHART_BAR - this content type mandates a two-level content structure of
 *                           the associated folder:
 *
 *                              @ a record named "Title" SHOULD be present
 *                                directly under the top folder. If this record
 *                                cannot be found, the folder name is used as
 *                                the title instead.
 *
 *                              @ a record named "Y-Axis" SHOULD be present
 *                                directly under the top folder. If this record
 *                                cannot be found, no indication will be given
 *                                for the Y-axis.
 *
 *                              @ all the subfolders directly under the current
 *                                folder MUST contain two records: "Value" and
 *                                "Label"; where "Value" record may be either a
 *                                SINT, UINT, or BOOL type (may be FLAT as well
 *                                in the future) while "Label" record MUST be
 *                                displayable string (i.e, ASTR record type).
 *
 *                              @ the number of first-level subfolders is used
 *                                to indicate the length of X axis while the
 *                                biggest "Value" is used to determine the
 *                                length of Y axis.
 *
 *      $ SIMPLE_GRAPH     - this content type mandates a two-level content structure of
 *                           the associated folder:
 *
 *                              @ a record named "Title" SHOULD be present
 *                                directly under the top folder. If this record
 *                                cannot be found, the folder name is used as
 *                                the title instead.
 *
 *                              @ a record named "Y-Axis" SHOULD be present
 *                                directly under the top folder. If this record
 *                                cannot be found, no indication will be given
 *                                for the Y-axis.
 *
 *                              @ a record named "X-Axis" SHOULD be present
 *                                directly under the top folder. If this record
 *                                cannot be found, no indication will be given
 *                                for the X-axis.
 *
 *                              @ all the subfolders directly under the current
 *                                folder MUST contain two records: "X-Value"
 *                                and "Y-Value".
 *
 *                              @ the length of the X axis is determined by the
 *                                biggest "X-Value" while the length of Y axis
 *                                is determined by the biggest "Y-Value".
 */
#define  SYS_FOLDER_CONTENT_DEFAULT                 0
#define  SYS_FOLDER_CONTENT_CATEGORY                1
#define  SYS_FOLDER_CONTENT_TABLE_2D                2
#define  SYS_FOLDER_CONTENT_TABLE_3D                3
#define  SYS_FOLDER_CONTENT_SIMPLE_CHART_PIE        4
#define  SYS_FOLDER_CONTENT_SIMPLE_CHART_BAR        5
#define  SYS_FOLDER_CONTENT_SIMPLE_GRAPH            6

typedef  struct
_SYS_RFO_RENDER_ATTR
{
    ULONG                           ContentType;
    char*                           Language;
    char*                           Charset;
    char*                           ContentIcon;
    void*                           Template;
}
SYS_RFO_RENDER_ATTR,  *PSYS_RFO_RENDER_ATTR;

#define  SysAllocateRfoRenderAttr()                 (PSYS_RFO_RENDER_ATTR)AnscAllocateMemory(sizeof(SYS_RFO_RENDER_ATTR))
#define  SysRfoRenderAttrGetContentType(p)          ( p->ContentType )
#define  SysRfoRenderAttrGetLanguage(p)             ( p->Language    )
#define  SysRfoRenderAttrGetCharset(p)              ( p->Charset     )
#define  SysRfoRenderAttrGetContentIcon(p)          ( p->ContentIcon )
#define  SysRfoRenderAttrGetTemplate(p)             ( p->Template    )

#define  SysInitRfoRenderAttr(p)                                                            \
         {                                                                                  \
            p->ContentType   = SYS_FOLDER_CONTENT_DEFAULT;                                  \
            p->Language      = NULL;                                                        \
            p->Charset       = NULL;                                                        \
            p->ContentIcon   = NULL;                                                        \
            p->Template      = NULL;                                                        \
         }

#define  SysCleanRfoRenderAttr(p)                                                           \
         {                                                                                  \
            if ( p->Language )                                                              \
            {                                                                               \
                AnscFreeMemory(p->Language);                                                \
                                                                                            \
                p->Language = NULL;                                                         \
            }                                                                               \
                                                                                            \
            if ( p->Charset )                                                               \
            {                                                                               \
                AnscFreeMemory(p->Charset);                                                 \
                                                                                            \
                p->Charset = NULL;                                                          \
            }                                                                               \
                                                                                            \
            if ( p->ContentIcon )                                                           \
            {                                                                               \
                AnscFreeMemory(p->ContentIcon);                                             \
                                                                                            \
                p->ContentIcon = NULL;                                                      \
            }                                                                               \
                                                                                            \
            if ( p->Template )                                                              \
            {                                                                               \
                AnscFreeMemory(p->Template);                                                \
                                                                                            \
                p->Template = NULL;                                                         \
            }                                                                               \
         }

#define  SysFreeRfoRenderAttr(p)                                                            \
         {                                                                                  \
            SysCleanRfoRenderAttr(p);                                                       \
                                                                                            \
            AnscFreeMemory(p);                                                              \
         }

#define  SysRfoRenderAttrSetContentType(p, t)       ( p->ContentType = t )

#define  SysRfoRenderAttrSetLanguage(p, l)                                                  \
         {                                                                                  \
            if ( p->Language )                                                              \
            {                                                                               \
                AnscFreeMemory(p->Language);                                                \
            }                                                                               \
                                                                                            \
            if ( l != NULL )                                                                \
            {                                                                               \
                p->Language = AnscAllocateMemory(AnscSizeOfString(l) + 1);                  \
                                                                                            \
                if ( p->Language )                                                          \
                {                                                                           \
                    AnscCopyString(p->Language, l);                                         \
                }                                                                           \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                p->Language = NULL;                                                         \
            }                                                                               \
         }

#define  SysRfoRenderAttrSetCharset(p, c)                                                   \
         {                                                                                  \
            if ( p->Charset )                                                               \
            {                                                                               \
                AnscFreeMemory(p->Charset);                                                 \
            }                                                                               \
                                                                                            \
            if ( c != NULL )                                                                \
            {                                                                               \
                p->Charset = AnscAllocateMemory(AnscSizeOfString(c) + 1);                   \
                                                                                            \
                if ( p->Charset )                                                           \
                {                                                                           \
                    AnscCopyString(p->Charset, c);                                          \
                }                                                                           \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                p->Charset = NULL;                                                          \
            }                                                                               \
         }

#define  SysRfoRenderAttrSetContentIcon(p, c)                                               \
         {                                                                                  \
            if ( p->ContentIcon )                                                           \
            {                                                                               \
                AnscFreeMemory(p->ContentIcon);                                             \
            }                                                                               \
                                                                                            \
            if ( c != NULL )                                                                \
            {                                                                               \
                p->ContentIcon = AnscAllocateMemory(AnscSizeOfString(c) + 1);               \
                                                                                            \
                if ( p->ContentIcon )                                                       \
                {                                                                           \
                    AnscCopyString(p->ContentIcon, c);                                      \
                }                                                                           \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                p->ContentIcon = NULL;                                                      \
            }                                                                               \
         }

#define  SysRfoRenderAttrSetTemplate(p, t)                                                  \
         {                                                                                  \
            ULONG                   template_size = 0;                                      \
                                                                                            \
            if ( p->Template )                                                              \
            {                                                                               \
                AnscFreeMemory(p->Template);                                                \
            }                                                                               \
                                                                                            \
            if ( t != NULL )                                                                \
            {                                                                               \
                template_size  = AnscUlongFromNToH(AnscReadUlong(t));                       \
                template_size += sizeof(ULONG);                                             \
                p->Template    = AnscAllocateMemory(template_size);                         \
                                                                                            \
                if ( p->Template )                                                          \
                {                                                                           \
                    AnscCopyMemory(p->Template, t, template_size);                          \
                }                                                                           \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                p->Template = NULL;                                                         \
            }                                                                               \
         }

#define  SysRfoRenderAttrSetTemplate2(p, t)                                                 \
         {                                                                                  \
            ULONG                   template_size = 0;                                      \
                                                                                            \
            if ( p->Template )                                                              \
            {                                                                               \
                AnscFreeMemory(p->Template);                                                \
            }                                                                               \
                                                                                            \
            if ( t != NULL )                                                                \
            {                                                                               \
                p->Template = t;                                                            \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                p->Template = NULL;                                                         \
            }                                                                               \
         }

#define  SysRfoCopyRenderAttr(dst_attr, src_attr)                                           \
         {                                                                                  \
            SysRfoRenderAttrSetContentType(dst_attr, src_attr->ContentType);                \
            SysRfoRenderAttrSetLanguage   (dst_attr, src_attr->Language   );                \
            SysRfoRenderAttrSetCharset    (dst_attr, src_attr->Charset    );                \
            SysRfoRenderAttrSetContentIcon(dst_attr, src_attr->ContentIcon);                \
            SysRfoRenderAttrSetTemplate   (dst_attr, src_attr->Template   );                \
         }

/*
 * While it may sound weird, you can actually assoicate a content-type with a repository folder
 * and record object. The Web-based Management Module interprets the content-type value and renders
 * the display accordingly (i.e., generate the HTML pages that can best represent the attributes of
 * the data). For example: a repository folder that represents a table should set its content-type
 * to table_2d. Every sub-folder will be rendered as a row, content in each sub-folder fills in the
 * row (the folder owner MUST ensure the consistency of each sub-folder).
 */
#define  SYS_RECORD_CONTENT_DEFAULT                 0
#define  SYS_RECORD_CONTENT_MAPPED                  1   /* applies to UINT and ENUM, rendered by the Name Mapper         */
#define  SYS_RECORD_CONTENT_IP4_ADDR                2   /* applies to UINT and BSTR, rendered as decimal dotted string   */
#define  SYS_RECORD_CONTENT_MAC_ADDR                3   /* applies to ASTR and BSTR, rendered as hex-decimal dotted      */
#define  SYS_RECORD_CONTENT_IP4_ADDR_LIST           4   /* applies to BSTR, rendered as an array of IP addresses         */
#define  SYS_RECORD_CONTENT_MAC_ADDR_LIST           5   /* applies to BSTR, rendered as an array of MAC addresses        */
#define  SYS_RECORD_CONTENT_TCPUDP_PORT             6   /* applies to UINT, mandate the range checking on the value      */
#define  SYS_RECORD_CONTENT_SECONDS                 7   /* applies to UINT, rendered as an integer followed by "seconds" */
#define  SYS_RECORD_CONTENT_MINUTES                 8   /* applies to UINT, rendered as an integer followed by "minutes" */
#define  SYS_RECORD_CONTENT_HOURS                   9   /* applies to UINT, rendered as an integer followed by "hours"   */
#define  SYS_RECORD_CONTENT_CALENDAR_TIME           10  /* applies to BSTR, data is structured as ANSC_UNIVERSAL_TIME    */
#define  SYS_RECORD_CONTENT_PASSWORD                11  /* applies to ASTR, all the characters should be masked          */
#define  SYS_RECORD_CONTENT_MASKED                  12  /* applies to UINT, rendered as a set of checkboxes              */
#define  SYS_RECORD_CONTENT_BUTTON                  13  /* applies to BOOL, rendered as a standalone button              */
#define  SYS_RECORD_CONTENT_SWITCH                  14  /* applies to SINT, rendered as a standalone switch              */
#define  SYS_RECORD_CONTENT_SINT_LIST               15  /* applies to BSTR, rendered as an array of integers             */
#define  SYS_RECORD_CONTENT_UINT_LIST               16  /* applies to BSTR, rendered as an array of unsigned integers    */

#define  SYS_RECORD_IFORMAT_DEFAULT                 0
#define  SYS_RECORD_IFORMAT_HIDDEN                  1
#define  SYS_RECORD_IFORMAT_ASCII                   2
#define  SYS_RECORD_IFORMAT_ALPHANUMERIC            3
#define  SYS_RECORD_IFORMAT_DECIMAL                 4
#define  SYS_RECORD_IFORMAT_HEX                     5

#define  SYS_RECORD_IVERIFY_DEFAULT                 0
#define  SYS_RECORD_IVERIFY_LEN_MIN                 0x00000001
#define  SYS_RECORD_IVERIFY_LEN_MAX                 0x00000002
#define  SYS_RECORD_IVERIFY_LEN_EXACT               0x00000004
#define  SYS_RECORD_IVERIFY_VAL_MIN                 0x00000008
#define  SYS_RECORD_IVERIFY_VAL_MAX                 0x00000010
#define  SYS_RECORD_IVERIFY_VAL_EXACT               0x00000020
#define  SYS_RECORD_IVERIFY_FORMAT                  0x00000040
#define  SYS_RECORD_IVERIFY_CHARSET                 0x00000080

typedef  ANSC_STATUS
(*PFN_SYS_RRO_ACTION_CALL)
    (
        ANSC_HANDLE                 hActionContext,
        int                         recordValue
    );

typedef  struct
_SYS_RRO_RENDER_ATTR
{
    ULONG                           ContentType;
    ULONG                           IFormat;
    ULONG                           IVerify;
    union
    {
        ULONG                       Length;
        ULONG                       ValueU;
        int                         ValueI;
    }MinLV;
    union
    {
        ULONG                       Length;
        ULONG                       ValueU;
        int                         ValueI;
    }MaxLV;
    ULONG                           MaxLength;
    PFN_SYS_RRO_ACTION_CALL         ActionCall;
    ANSC_HANDLE                     ActionContext;

    char*                           Language;
    char*                           Charset;
    char*                           ContentIcon;
    char*                           NameMapper;
}
SYS_RRO_RENDER_ATTR,  *PSYS_RRO_RENDER_ATTR;

#define  SysAllocateRroRenderAttr()                 (PSYS_RRO_RENDER_ATTR)AnscAllocateMemory(sizeof(SYS_RRO_RENDER_ATTR))
#define  SysRroRenderAttrGetContentType(p)          ( p->ContentType   )
#define  SysRroRenderAttrGetIFormat(p)              ( p->IFormat       )
#define  SysRroRenderAttrGetIVerify(p)              ( p->IVerify       )
#define  SysRroRenderAttrGetMinLength(p)            ( p->MinLV.Length  )
#define  SysRroRenderAttrGetMinValueU(p)            ( p->MinLV.ValueU  )
#define  SysRroRenderAttrGetMinValueI(p)            ( p->MinLV.ValueI  )
#define  SysRroRenderAttrGetMaxLength(p)            ( p->MaxLV.Length  )
#define  SysRroRenderAttrGetMaxValueU(p)            ( p->MaxLV.ValueU  )
#define  SysRroRenderAttrGetMaxValueI(p)            ( p->MaxLV.ValueI  )
#define  SysRroRenderAttrGetActionCall(p)           ( p->ActionCall    )
#define  SysRroRenderAttrGetActionContext(p)        ( p->ActionContext )
#define  SysRroRenderAttrGetLanguage(p)             ( p->Language      )
#define  SysRroRenderAttrGetCharset(p)              ( p->Charset       )
#define  SysRroRenderAttrGetContentIcon(p)          ( p->ContentIcon   )
#define  SysRroRenderAttrGetNameMapper(p)           ( p->NameMapper    )

#define  SysInitRroRenderAttr(p)                                                            \
         {                                                                                  \
            p->ContentType   = SYS_RECORD_CONTENT_DEFAULT;                                  \
            p->IFormat       = SYS_RECORD_IFORMAT_DEFAULT;                                  \
            p->IVerify       = SYS_RECORD_IVERIFY_DEFAULT;                                  \
            p->MinLV.Length  = 0;                                                           \
            p->MaxLV.Length  = 0;                                                           \
            p->ActionCall    = NULL;                                                        \
            p->ActionContext = (ANSC_HANDLE)NULL;                                           \
                                                                                            \
            p->Language      = NULL;                                                        \
            p->Charset       = NULL;                                                        \
            p->ContentIcon   = NULL;                                                        \
            p->NameMapper    = NULL;                                                        \
         }

#define  SysCleanRroRenderAttr(p)                                                           \
         {                                                                                  \
            if ( p->Language )                                                              \
            {                                                                               \
                AnscFreeMemory(p->Language);                                                \
                                                                                            \
                p->Language = NULL;                                                         \
            }                                                                               \
                                                                                            \
            if ( p->Charset )                                                               \
            {                                                                               \
                AnscFreeMemory(p->Charset);                                                 \
                                                                                            \
                p->Charset = NULL;                                                          \
            }                                                                               \
                                                                                            \
            if ( p->ContentIcon )                                                           \
            {                                                                               \
                AnscFreeMemory(p->ContentIcon);                                             \
                                                                                            \
                p->ContentIcon = NULL;                                                      \
            }                                                                               \
                                                                                            \
            if ( p->NameMapper )                                                            \
            {                                                                               \
                AnscFreeMemory(p->NameMapper);                                              \
                                                                                            \
                p->NameMapper = NULL;                                                       \
            }                                                                               \
         }

#define  SysFreeRroRenderAttr(p)                                                            \
         {                                                                                  \
            SysCleanRroRenderAttr(p);                                                       \
                                                                                            \
            AnscFreeMemory(p);                                                              \
         }

#define  SysRroRenderAttrSetContentType(p, t)       ( p->ContentType   = t )
#define  SysRroRenderAttrSetIFormat(p, f)           ( p->IFormat       = f )
#define  SysRroRenderAttrSetIVerify(p, v)           ( p->IVerify       = v )
#define  SysRroRenderAttrSetMinLength(p, l)         ( p->MinLV.Length  = l )
#define  SysRroRenderAttrSetMinValueU(p, v)         ( p->MinLV.ValueU  = v )
#define  SysRroRenderAttrSetMinValueI(p, v)         ( p->MinLV.ValueI  = v )
#define  SysRroRenderAttrSetMaxLength(p, l)         ( p->MaxLV.Length  = l )
#define  SysRroRenderAttrSetMaxValueU(p, v)         ( p->MaxLV.ValueU  = v )
#define  SysRroRenderAttrSetMaxValueI(p, v)         ( p->MaxLV.ValueI  = v )
#define  SysRroRenderAttrSetActionCall(p, c)        ( p->ActionCall    = c )
#define  SysRroRenderAttrSetActionContext(p, c)     ( p->ActionContext = c )

#define  SysRroRenderAttrSetLanguage(p, l)                                                  \
         {                                                                                  \
            if ( p->Language )                                                              \
            {                                                                               \
                AnscFreeMemory(p->Language);                                                \
            }                                                                               \
                                                                                            \
            if ( l != NULL )                                                                \
            {                                                                               \
                p->Language = AnscAllocateMemory(AnscSizeOfString(l) + 1);                  \
                                                                                            \
                if ( p->Language )                                                          \
                {                                                                           \
                    AnscCopyString(p->Language, l);                                         \
                }                                                                           \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                p->Language = NULL;                                                         \
            }                                                                               \
         }

#define  SysRroRenderAttrSetCharset(p, c)                                                   \
         {                                                                                  \
            if ( p->Charset )                                                               \
            {                                                                               \
                AnscFreeMemory(p->Charset);                                                 \
            }                                                                               \
                                                                                            \
            if ( c != NULL )                                                                \
            {                                                                               \
                p->Charset = AnscAllocateMemory(AnscSizeOfString(c) + 1);                   \
                                                                                            \
                if ( p->Charset )                                                           \
                {                                                                           \
                    AnscCopyString(p->Charset, c);                                          \
                }                                                                           \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                p->Charset = NULL;                                                          \
            }                                                                               \
         }

#define  SysRroRenderAttrSetContentIcon(p, c)                                               \
         {                                                                                  \
            if ( p->ContentIcon )                                                           \
            {                                                                               \
                AnscFreeMemory(p->ContentIcon);                                             \
            }                                                                               \
                                                                                            \
            if ( c != NULL )                                                                \
            {                                                                               \
                p->ContentIcon = AnscAllocateMemory(AnscSizeOfString(c) + 1);               \
                                                                                            \
                if ( p->ContentIcon )                                                       \
                {                                                                           \
                    AnscCopyString(p->ContentIcon, c);                                      \
                }                                                                           \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                p->ContentIcon = NULL;                                                      \
            }                                                                               \
         }

#define  SysRroRenderAttrSetNameMapper(p, n)                                                \
         {                                                                                  \
            if ( p->NameMapper )                                                            \
            {                                                                               \
                AnscFreeMemory(p->NameMapper);                                              \
            }                                                                               \
                                                                                            \
            if ( n != NULL )                                                                \
            {                                                                               \
                p->NameMapper = AnscAllocateMemory(AnscSizeOfString(n) + 1);                \
                                                                                            \
                if ( p->NameMapper )                                                        \
                {                                                                           \
                    AnscCopyString(p->NameMapper, n);                                       \
                }                                                                           \
            }                                                                               \
            else                                                                            \
            {                                                                               \
                p->NameMapper = NULL;                                                       \
            }                                                                               \
         }

#define  SysRroCopyRenderAttr(dst_attr, src_attr)                                           \
         {                                                                                  \
            SysRroRenderAttrSetContentType  (dst_attr, src_attr->ContentType  );            \
            SysRroRenderAttrSetIFormat      (dst_attr, src_attr->IFormat      );            \
            SysRroRenderAttrSetIVerify      (dst_attr, src_attr->IVerify      );            \
            SysRroRenderAttrSetMinLength    (dst_attr, src_attr->MinLV.Length );            \
            SysRroRenderAttrSetMaxLength    (dst_attr, src_attr->MaxLV.Length );            \
            SysRroRenderAttrSetActionCall   (dst_attr, src_attr->ActionCall   );            \
            SysRroRenderAttrSetActionContext(dst_attr, src_attr->ActionContext);            \
                                                                                            \
            SysRroRenderAttrSetLanguage     (dst_attr, src_attr->Language     );            \
            SysRroRenderAttrSetCharset      (dst_attr, src_attr->Charset      );            \
            SysRroRenderAttrSetContentIcon  (dst_attr, src_attr->ContentIcon  );            \
            SysRroRenderAttrSetNameMapper   (dst_attr, src_attr->NameMapper   );            \
         }


/***********************************************************
         SYS SERVICE MODULE RUNTIME STATE DEFINITION
***********************************************************/

/*
 * The system model modules usually provide an interface for other system modules (even external
 * modules) to manage the core services in runtime. To reflect such control mechanism via a typical
 * GUI, we need to define a set of states that a service module may be in.
 */
#define  SYS_SERVICE_STATE_NOT_LOADED               1
#define  SYS_SERVICE_STATE_LOADED                   2
#define  SYS_SERVICE_STATE_SWITCHED                 3

#define  SYS_SERVICE_SDESP_NOT_LOADED               "Not Loaded"
#define  SYS_SERVICE_SDESP_LOADED                   "Stopped"
#define  SYS_SERVICE_SDESP_SWITCHED                 "Running"


/***********************************************************
         SYS REPOSITORY FOLDER KEY OBJECT DEFINITION
***********************************************************/

/*
 * We have to provide some sort of resource-sharing mechanism to synchronize the access to the same
 * Repository Folder Object simultaneously by multiple components. To achieve, we allocate a small
 * data structure, known as "key", every time when a component asks for access the folder. This key
 * encapsulates the access mode required by caller and binds itself to the original folder. At any
 * time, we always allow unlimited read access, but only allow a single write/create/sync access.
 */
#define  SYS_RFO_CLOSE_FLAG_SYNC                    0x00000001
#define  SYS_RFO_CLOSE_FLAG_CLEAN                   0x00000002

typedef  struct
_SYS_RFO_KEY
{
    SINGLE_LINK_ENTRY               Linkage;
    ANSC_HANDLE                     hRepFolder;
    ULONG                           Permission;
    ULONG                           FolderType;
    ULONG                           ContentType;
    ULONG                           AccessMode;
    ULONG                           CloseFlags;
}
SYS_RFO_KEY,  *PSYS_RFO_KEY;

#define  ACCESS_SYS_RFO_KEY(p)                      \
         ACCESS_CONTAINER(p, SYS_RFO_KEY, Linkage)


/***********************************************************
     SYS REPOSITORY FOLDER SORTING MECHANISM DEFINITION
***********************************************************/

/*
 * The Repository Folder Object maintains all the sub-folders and records in hash tables. By nature,
 * hash table doesn't preserve the adding order of the hashed item. This causes problems for many
 * management front-end modules. For example: when web management engine renders a TABLE_2D folder,
 * end user will expect to see a organized table view. To address this requirement, the Repository
 * Folder Object provides a sorting mechanism.
 */
#define  SYS_REPOSITORY_SORTING_BYTIME              1
#define  SYS_REPOSITORY_SORTING_BYNAME              2


/***********************************************************
          SYS INTERNAL COMPONENT MODULE DEFINITION
***********************************************************/

/*
 * It's always been pain in the ass trying to maintain several different versions of a software
 * component and manage the migrating process. For example, we have to maintain the legacy firewall
 * implementation, known as "CyberWall", for some existing customers, while we have already started
 * working on a newer and better firewall version. Although we may simply encapsulate both versions
 * with the same set of modules (at least the module name can be shared), it actually can be quite
 * confusing to other controller modules.
 */
#define  SYS_MODULE_NAME_NAT_GATEWAY                SYS_MODULE_NAME_SM_NAT
#define  SYS_MODULE_NAME_WEB_SERVICE                SYS_MODULE_NAME_SB_WEB_SERVER
#define  SYS_MODULE_NAME_CYFW                       SYS_MODULE_NAME_FIREWALL

/*
 * Where to draw the lines between objects, modules, containers, and packages is a hard question to
 * answer since there's no differences conceptually. We consider "module" as a pack of objects that
 * work together to deliver a set of well-understood but integrated functions. By "well-understood"
 * we meant that the function of the module has been studied and specified by certain standards. By
 * "integrated" we meant that the set of the functions shouldn't be separated in a deployment.
 */
#define  SYS_MODULE_NAME_NULL                       "Module-Null"
#define  SYS_MODULE_NAME_NET_DRIVER_LIB             "Module-Net-Driver-Lib"
#define  SYS_MODULE_NAME_SOCKET_LIB                 "Module-Socket-Lib"
#define  SYS_MODULE_NAME_BSS                        "Module-Bss"
#define  SYS_MODULE_NAME_GNPS                       "Module-Gnps"
#define  SYS_MODULE_NAME_IP4_STACK                  "Module-Ip4-Stack"
#define  SYS_MODULE_NAME_TCP_STACK                  "Module-Tcp-Stack"
#define  SYS_MODULE_NAME_UDP_STACK                  "Module-Udp-Stack"
#define  SYS_MODULE_NAME_GTP_STACK                  "Module-Gtp-Stack"
#define  SYS_MODULE_NAME_PPTP_PNS                   "Module-Pptp-Pns"
#define  SYS_MODULE_NAME_PPTP_PAC                   "Module-Pptp-Pac"
#define  SYS_MODULE_NAME_L2TP_LNS                   "Module-L2tp-Lns"
#define  SYS_MODULE_NAME_L2TP_LAC                   "Module-L2tp-Lac"
#define  SYS_MODULE_NAME_RIP2                       "Module-Rip2"
#define  SYS_MODULE_NAME_OSPF                       "Module-Ospf"
#define  SYS_MODULE_NAME_DHCP_SERVER                "Module-Dhcp-Server"
#define  SYS_MODULE_NAME_DHCP_SERVER2               "Module-Dhcp-Server2"
#define  SYS_MODULE_NAME_DNS_REDIRECTOR             "Module-Dns-Redirector"
#define  SYS_MODULE_NAME_WEB_PROXY                  "Module-Web-Proxy"
#define  SYS_MODULE_NAME_WEB_SERVER                 "Module-Web-Server"
#define  SYS_MODULE_NAME_CYBERWALL                  "Module-CyberWall"
#define  SYS_MODULE_NAME_IPSEC_VPN                  "Module-Ipsec-Vpn"
#define  SYS_MODULE_NAME_SNMP_AGENT                 "Module-Snmp-Agent"
#define  SYS_MODULE_NAME_NTP                        "Module-Ntp"
#define  SYS_MODULE_NAME_BMW                        "Module-Bmw"
#define  SYS_MODULE_NAME_WCP                        "Module-Wcp"
#define  SYS_MODULE_NAME_HTTP_HFP                   "Module-Http-Hfp"
#define  SYS_MODULE_NAME_RGWM                       "Module-Rgwm"
#define  SYS_MODULE_NAME_RAM                        "Module-Ram"
#define  SYS_MODULE_NAME_SNMP                       "Module-Snmp"
#define  SYS_MODULE_NAME_MIB2                       "Module-Mib2"
#define  SYS_MODULE_NAME_MOAM                       "Module-Moam"
#define  SYS_MODULE_NAME_FIREWALL                   "Module-Firewall"
#define  SYS_MODULE_NAME_NAT                        "Module-Nat"
#define  SYS_MODULE_NAME_SNMP                       "Module-Snmp"
#define  SYS_MODULE_NAME_MIB2                       "Module-Mib2"
#define  SYS_MODULE_NAME_MOAM                       "Module-Moam"
#define  SYS_MODULE_NAME_KB5C                       "Module-Kb5-Client"
#define  SYS_MODULE_NAME_KB5S                       "Module-Kb5-Server"
#define  SYS_MODULE_NAME_KB5A                       "Module-Kb5-AppServer"
#define  SYS_MODULE_NAME_KBKMC                      "Module-Kbkm-Client"
#define  SYS_MODULE_NAME_KBKMS                      "Module-Kbkm-Server"
#define  SYS_MODULE_NAME_CBHM10                     "Module-Cbhm10"
#define  SYS_MODULE_NAME_GFWM                       "Module-Gfwm"
#define  SYS_MODULE_NAME_GNAT                       "Module-Gnat"
#define  SYS_MODULE_NAME_CTP                        "Module-Ctp"
#define  SYS_MODULE_NAME_IPSEC                      "Module-Ipsec"
#define  SYS_MODULE_NAME_CCGM                       "Module-Ccgm"
#define  SYS_MODULE_NAME_BEEP                       "Module-Beep"
#define  SYS_MODULE_NAME_BREE                       "Module-Bree"
#define  SYS_MODULE_NAME_BWRM                       "Module-Bwrm"
#define  SYS_MODULE_NAME_BWSP                       "Module-Bwsp"
#define  SYS_MODULE_NAME_SLAP                       "Module-Slap"
#define  SYS_MODULE_NAME_TLS                        "Module-Tls"

#define  SYS_MODULE_NAME_AM_NET_DRIVER_LIB          "Module-Am-Net-Driver-Lib"
#define  SYS_MODULE_NAME_AM_SOCKET_LIB              "Module-Am-Socket-Lib"
#define  SYS_MODULE_NAME_AM_BSS                     "Module-Am-Bss"
#define  SYS_MODULE_NAME_AM_GNPS                    "Module-Am-Gnps"
#define  SYS_MODULE_NAME_AM_IP4_STACK               "Module-Am-Ip4-Stack"
#define  SYS_MODULE_NAME_AM_TCP_STACK               "Module-Am-Tcp-Stack"
#define  SYS_MODULE_NAME_AM_UDP_STACK               "Module-Am-Udp-Stack"
#define  SYS_MODULE_NAME_AM_GTP_STACK               "Module-Am-Gtp-Stack"
#define  SYS_MODULE_NAME_AM_PPTP_PNS                "Module-Am-Pptp-Pns"
#define  SYS_MODULE_NAME_AM_PPTP_PAC                "Module-Am-Pptp-Pac"
#define  SYS_MODULE_NAME_AM_L2TP_LNS                "Module-Am-L2tp-Lns"
#define  SYS_MODULE_NAME_AM_L2TP_LAC                "Module-Am-L2tp-Lac"
#define  SYS_MODULE_NAME_AM_RIP2                    "Module-Am-Rip2"
#define  SYS_MODULE_NAME_AM_OSPF                    "Module-Am-Ospf"
#define  SYS_MODULE_NAME_AM_DHCP_SERVER             "Module-Am-Dhcp-Server"
#define  SYS_MODULE_NAME_AM_DHCP_SERVER2            "Module-Am-Dhcp-Server2"
#define  SYS_MODULE_NAME_AM_DNS_REDIRECTOR          "Module-Am-Dns-Redirector"
#define  SYS_MODULE_NAME_AM_WEB_PROXY               "Module-Am-Web-Proxy"
#define  SYS_MODULE_NAME_AM_WEB_SERVER              "Module-Am-Web-Server"
#define  SYS_MODULE_NAME_AM_CYBERWALL               "Module-Am-CyberWall"
#define  SYS_MODULE_NAME_AM_IPSEC_VPN               "Module-Am-Ipsec-Vpn"
#define  SYS_MODULE_NAME_AM_SNMP_AGENT              "Module-Am-Snmp-Agent"
#define  SYS_MODULE_NAME_AM_NTP                     "Module-Am-Ntp"
#define  SYS_MODULE_NAME_AM_BMW                     "Module-Am-Bmw"
#define  SYS_MODULE_NAME_AM_WCP                     "Module-Am-Wcp"
#define  SYS_MODULE_NAME_AM_HTTP_HFP                "Module-Am-Http-Hfp"
#define  SYS_MODULE_NAME_AM_RGWM                    "Module-Am-Rgwm"
#define  SYS_MODULE_NAME_AM_RAM                     "Module-Am-Ram"
#define  SYS_MODULE_NAME_AM_SNMP                    "Module-Am-Snmp"
#define  SYS_MODULE_NAME_AM_MIB2                    "Module-Am-Mib2"
#define  SYS_MODULE_NAME_AM_MOAM                    "Module-Am-Moam"
#define  SYS_MODULE_NAME_AM_FIREWALL                "Module-Am-Firewall"
#define  SYS_MODULE_NAME_AM_NAT                     "Module-Am-Nat"
#define  SYS_MODULE_NAME_AM_SNMP                    "Module-Am-Snmp"
#define  SYS_MODULE_NAME_AM_MIB2                    "Module-Am-Mib2"
#define  SYS_MODULE_NAME_AM_MOAM                    "Module-Am-Moam"
#define  SYS_MODULE_NAME_AM_KB5C                    "Module-Am-Kb5-Client"
#define  SYS_MODULE_NAME_AM_KB5S                    "Module-Am-Kb5-Server"
#define  SYS_MODULE_NAME_AM_KB5A                    "Module-Am-Kb5-AppServer"
#define  SYS_MODULE_NAME_AM_KBKMC                   "Module-Am-Kbkm-Client"
#define  SYS_MODULE_NAME_AM_KBKMS                   "Module-Am-Kbkm-Server"
#define  SYS_MODULE_NAME_AM_CBHM10                  "Module-Am-Cbhm10"
#define  SYS_MODULE_NAME_AM_GFWM                    "Module-Am-Gfwm"
#define  SYS_MODULE_NAME_AM_GNAT                    "Module-Am-Gnat"
#define  SYS_MODULE_NAME_AM_CTP                     "Module-Am-Ctp"
#define  SYS_MODULE_NAME_AM_IPSEC                   "Module-Am-Ipsec"
#define  SYS_MODULE_NAME_AM_CCGM                    "Module-Am-Ccgm"
#define  SYS_MODULE_NAME_AM_BEEP                    "Module-Am-Beep"
#define  SYS_MODULE_NAME_AM_BREE                    "Module-Am-Bree"
#define  SYS_MODULE_NAME_AM_BWRM                    "Module-Am-Bwrm"
#define  SYS_MODULE_NAME_AM_BWSP                    "Module-Am-Bwsp"
#define  SYS_MODULE_NAME_AM_SLAP                    "Module-Am-Slap"
#define  SYS_MODULE_NAME_AM_TLS                     "Module-Am-Tls"

#define  SYS_MODULE_NAME_SM_NET_DRIVER_LIB          "Module-Sm-Net-Driver-Lib"
#define  SYS_MODULE_NAME_SM_SOCKET_LIB              "Module-Sm-Socket-Lib"
#define  SYS_MODULE_NAME_SM_BSS                     "Module-Sm-Bss"
#define  SYS_MODULE_NAME_SM_GNPS                    "Module-Sm-Gnps"
#define  SYS_MODULE_NAME_SM_IP4_STACK               "Module-Sm-Ip4-Stack"
#define  SYS_MODULE_NAME_SM_TCP_STACK               "Module-Sm-Tcp-Stack"
#define  SYS_MODULE_NAME_SM_UDP_STACK               "Module-Sm-Udp-Stack"
#define  SYS_MODULE_NAME_SM_GTP_STACK               "Module-Sm-Gtp-Stack"
#define  SYS_MODULE_NAME_SM_PPTP_PNS                "Module-Sm-Pptp-Pns"
#define  SYS_MODULE_NAME_SM_PPTP_PAC                "Module-Sm-Pptp-Pac"
#define  SYS_MODULE_NAME_SM_L2TP_LNS                "Module-Sm-L2tp-Lns"
#define  SYS_MODULE_NAME_SM_L2TP_LAC                "Module-Sm-L2tp-Lac"
#define  SYS_MODULE_NAME_SM_RIP2                    "Module-Sm-Rip2"
#define  SYS_MODULE_NAME_SM_OSPF                    "Module-Sm-Ospf"
#define  SYS_MODULE_NAME_SM_DHCP_SERVER             "Module-Sm-Dhcp-Server"
#define  SYS_MODULE_NAME_SM_DHCP_SERVER2            "Module-Sm-Dhcp-Server2"
#define  SYS_MODULE_NAME_SM_DNS_REDIRECTOR          "Module-Sm-Dns-Redirector"
#define  SYS_MODULE_NAME_SM_WEB_PROXY               "Module-Sm-Web-Proxy"
#define  SYS_MODULE_NAME_SM_WEB_SERVER              "Module-Sm-Web-Server"
#define  SYS_MODULE_NAME_SM_CYBERWALL               "Module-Sm-CyberWall"
#define  SYS_MODULE_NAME_SM_IPSEC_VPN               "Module-Sm-Ipsec-Vpn"
#define  SYS_MODULE_NAME_SM_SNMP_AGENT              "Module-Sm-Snmp-Agent"
#define  SYS_MODULE_NAME_SM_NTP                     "Module-Sm-Ntp"
#define  SYS_MODULE_NAME_SM_BMW                     "Module-Sm-Bmw"
#define  SYS_MODULE_NAME_SM_WCP                     "Module-Sm-Wcp"
#define  SYS_MODULE_NAME_SM_HTTP_HFP                "Module-Sm-Http-Hfp"
#define  SYS_MODULE_NAME_SM_RGWM                    "Module-Sm-Rgwm"
#define  SYS_MODULE_NAME_SM_RAM                     "Module-Sm-Ram"
#define  SYS_MODULE_NAME_SM_SNMP                    "Module-Sm-Snmp"
#define  SYS_MODULE_NAME_SM_MIB2                    "Module-Sm-Mib2"
#define  SYS_MODULE_NAME_SM_MOAM                    "Module-Sm-Moam"
#define  SYS_MODULE_NAME_SM_FIREWALL                "Module-Sm-Firewall"
#define  SYS_MODULE_NAME_SM_NAT                     "Module-Sm-Nat"
#define  SYS_MODULE_NAME_SM_SNMP                    "Module-Sm-Snmp"
#define  SYS_MODULE_NAME_SM_MIB2                    "Module-Sm-Mib2"
#define  SYS_MODULE_NAME_SM_MOAM                    "Module-Sm-Moam"
#define  SYS_MODULE_NAME_SM_KB5C                    "Module-Sm-Kb5-Client"
#define  SYS_MODULE_NAME_SM_KB5S                    "Module-Sm-Kb5-Server"
#define  SYS_MODULE_NAME_SM_KB5A                    "Module-Sm-Kb5-AppServer"
#define  SYS_MODULE_NAME_SM_KBKMC                   "Module-Sm-Kbkm-Client"
#define  SYS_MODULE_NAME_SM_KBKMS                   "Module-Sm-Kbkm-Server"
#define  SYS_MODULE_NAME_SM_CBHM10                  "Module-Sm-Cbhm10"
#define  SYS_MODULE_NAME_SM_GFWM                    "Module-Sm-Gfwm"
#define  SYS_MODULE_NAME_SM_GNAT                    "Module-Sm-Gnat"
#define  SYS_MODULE_NAME_SM_CTP                     "Module-Sm-Ctp"
#define  SYS_MODULE_NAME_SM_IPSEC                   "Module-Sm-Ipsec"
#define  SYS_MODULE_NAME_SM_CCGM                    "Module-Sm-Ccgm"
#define  SYS_MODULE_NAME_SM_BEEP                    "Module-Sm-Beep"
#define  SYS_MODULE_NAME_SM_BREE                    "Module-Sm-Bree"
#define  SYS_MODULE_NAME_SM_BWRM                    "Module-Sm-Bwrm"
#define  SYS_MODULE_NAME_SM_BWSP                    "Module-Sm-Bwsp"
#define  SYS_MODULE_NAME_SM_SLAP                    "Module-Sm-Slap"
#define  SYS_MODULE_NAME_SM_TLS                     "Module-Sm-Tls"

#define  SYS_MODULE_NAME_PM_NET_DRIVER_LIB          "Module-Pm-Net-Driver-Lib"
#define  SYS_MODULE_NAME_PM_SOCKET_LIB              "Module-Pm-Socket-Lib"
#define  SYS_MODULE_NAME_PM_BSS                     "Module-Pm-Bss"
#define  SYS_MODULE_NAME_PM_GNPS                    "Module-Pm-Gnps"
#define  SYS_MODULE_NAME_PM_IP4_STACK               "Module-Pm-Ip4-Stack"
#define  SYS_MODULE_NAME_PM_TCP_STACK               "Module-Pm-Tcp-Stack"
#define  SYS_MODULE_NAME_PM_UDP_STACK               "Module-Pm-Udp-Stack"
#define  SYS_MODULE_NAME_PM_GTP_STACK               "Module-Pm-Gtp-Stack"
#define  SYS_MODULE_NAME_PM_PPTP_PNS                "Module-Pm-Pptp-Pns"
#define  SYS_MODULE_NAME_PM_PPTP_PAC                "Module-Pm-Pptp-Pac"
#define  SYS_MODULE_NAME_PM_L2TP_LNS                "Module-Pm-L2tp-Lns"
#define  SYS_MODULE_NAME_PM_L2TP_LAC                "Module-Pm-L2tp-Lac"
#define  SYS_MODULE_NAME_PM_RIP2                    "Module-Pm-Rip2"
#define  SYS_MODULE_NAME_PM_OSPF                    "Module-Pm-Ospf"
#define  SYS_MODULE_NAME_PM_DHCP_SERVER             "Module-Pm-Dhcp-Server"
#define  SYS_MODULE_NAME_PM_DHCP_SERVER2            "Module-Pm-Dhcp-Server2"
#define  SYS_MODULE_NAME_PM_DNS_REDIRECTOR          "Module-Pm-Dns-Redirector"
#define  SYS_MODULE_NAME_PM_WEB_PROXY               "Module-Pm-Web-Proxy"
#define  SYS_MODULE_NAME_PM_WEB_SERVER              "Module-Pm-Web-Server"
#define  SYS_MODULE_NAME_PM_CYBERWALL               "Module-Pm-CyberWall"
#define  SYS_MODULE_NAME_PM_IPSEC_VPN               "Module-Pm-Ipsec-Vpn"
#define  SYS_MODULE_NAME_PM_SNMP_AGENT              "Module-Pm-Snmp-Agent"
#define  SYS_MODULE_NAME_PM_NTP                     "Module-Pm-Ntp"
#define  SYS_MODULE_NAME_PM_BMW                     "Module-Pm-Bmw"
#define  SYS_MODULE_NAME_PM_WCP                     "Module-Pm-Wcp"
#define  SYS_MODULE_NAME_PM_HTTP_HFP                "Module-Pm-Http-Hfp"
#define  SYS_MODULE_NAME_PM_RGWM                    "Module-Pm-Rgwm"
#define  SYS_MODULE_NAME_PM_RAM                     "Module-Pm-Ram"
#define  SYS_MODULE_NAME_PM_SNMP                    "Module-Pm-Snmp"
#define  SYS_MODULE_NAME_PM_MIB2                    "Module-Pm-Mib2"
#define  SYS_MODULE_NAME_PM_MOAM                    "Module-Pm-Moam"
#define  SYS_MODULE_NAME_PM_FIREWALL                "Module-Pm-Firewall"
#define  SYS_MODULE_NAME_PM_NAT                     "Module-Pm-Nat"
#define  SYS_MODULE_NAME_PM_SNMP                    "Module-Pm-Snmp"
#define  SYS_MODULE_NAME_PM_MIB2                    "Module-Pm-Mib2"
#define  SYS_MODULE_NAME_PM_MOAM                    "Module-Pm-Moam"
#define  SYS_MODULE_NAME_PM_KB5C                    "Module-Pm-Kb5-Client"
#define  SYS_MODULE_NAME_PM_KB5S                    "Module-Pm-Kb5-Server"
#define  SYS_MODULE_NAME_PM_KB5A                    "Module-Pm-Kb5-AppServer"
#define  SYS_MODULE_NAME_PM_KBKMC                   "Module-Pm-Kbkm-Client"
#define  SYS_MODULE_NAME_PM_KBKMS                   "Module-Pm-Kbkm-Server"
#define  SYS_MODULE_NAME_PM_CBHM10                  "Module-Pm-Cbhm10"
#define  SYS_MODULE_NAME_PM_GFWM                    "Module-Pm-Gfwm"
#define  SYS_MODULE_NAME_PM_GNAT                    "Module-Pm-Gnat"
#define  SYS_MODULE_NAME_PM_CTP                     "Module-Pm-Ctp"
#define  SYS_MODULE_NAME_PM_IPSEC                   "Module-Pm-Ipsec"
#define  SYS_MODULE_NAME_PM_CCGM                    "Module-Pm-Ccgm"
#define  SYS_MODULE_NAME_PM_BEEP                    "Module-Pm-Beep"
#define  SYS_MODULE_NAME_PM_BREE                    "Module-Pm-Bree"
#define  SYS_MODULE_NAME_PM_BWRM                    "Module-Pm-Bwrm"
#define  SYS_MODULE_NAME_PM_BWSP                    "Module-Pm-Bwsp"
#define  SYS_MODULE_NAME_PM_SLAP                    "Module-Pm-Slap"
#define  SYS_MODULE_NAME_PM_TLS                     "Module-Pm-Tls"

#define  SYS_MODULE_NAME_RM_NET_DRIVER_LIB          "Module-Rm-Net-Driver-Lib"
#define  SYS_MODULE_NAME_RM_SOCKET_LIB              "Module-Rm-Socket-Lib"
#define  SYS_MODULE_NAME_RM_BSS                     "Module-Rm-Bss"
#define  SYS_MODULE_NAME_RM_GNPS                    "Module-Rm-Gnps"
#define  SYS_MODULE_NAME_RM_IP4_STACK               "Module-Rm-Ip4-Stack"
#define  SYS_MODULE_NAME_RM_TCP_STACK               "Module-Rm-Tcp-Stack"
#define  SYS_MODULE_NAME_RM_UDP_STACK               "Module-Rm-Udp-Stack"
#define  SYS_MODULE_NAME_RM_GTP_STACK               "Module-Rm-Gtp-Stack"
#define  SYS_MODULE_NAME_RM_PPTP_PNS                "Module-Rm-Pptp-Pns"
#define  SYS_MODULE_NAME_RM_PPTP_PAC                "Module-Rm-Pptp-Pac"
#define  SYS_MODULE_NAME_RM_L2TP_LNS                "Module-Rm-L2tp-Lns"
#define  SYS_MODULE_NAME_RM_L2TP_LAC                "Module-Rm-L2tp-Lac"
#define  SYS_MODULE_NAME_RM_RIP2                    "Module-Rm-Rip2"
#define  SYS_MODULE_NAME_RM_OSPF                    "Module-Rm-Ospf"
#define  SYS_MODULE_NAME_RM_DHCP_SERVER             "Module-Rm-Dhcp-Server"
#define  SYS_MODULE_NAME_RM_DHCP_SERVER2            "Module-Rm-Dhcp-Server2"
#define  SYS_MODULE_NAME_RM_DNS_REDIRECTOR          "Module-Rm-Dns-Redirector"
#define  SYS_MODULE_NAME_RM_WEB_PROXY               "Module-Rm-Web-Proxy"
#define  SYS_MODULE_NAME_RM_WEB_SERVER              "Module-Rm-Web-Server"
#define  SYS_MODULE_NAME_RM_CYBERWALL               "Module-Rm-CyberWall"
#define  SYS_MODULE_NAME_RM_IPSEC_VPN               "Module-Rm-Ipsec-Vpn"
#define  SYS_MODULE_NAME_RM_SNMP_AGENT              "Module-Rm-Snmp-Agent"
#define  SYS_MODULE_NAME_RM_NTP                     "Module-Rm-Ntp"
#define  SYS_MODULE_NAME_RM_BMW                     "Module-Rm-Bmw"
#define  SYS_MODULE_NAME_RM_WCP                     "Module-Rm-Wcp"
#define  SYS_MODULE_NAME_RM_HTTP_HFP                "Module-Rm-Http-Hfp"
#define  SYS_MODULE_NAME_RM_RGWM                    "Module-Rm-Rgwm"
#define  SYS_MODULE_NAME_RM_RAM                     "Module-Rm-Ram"
#define  SYS_MODULE_NAME_RM_SNMP                    "Module-Rm-Snmp"
#define  SYS_MODULE_NAME_RM_MIB2                    "Module-Rm-Mib2"
#define  SYS_MODULE_NAME_RM_MOAM                    "Module-Rm-Moam"
#define  SYS_MODULE_NAME_RM_FIREWALL                "Module-Rm-Firewall"
#define  SYS_MODULE_NAME_RM_NAT                     "Module-Rm-Nat"
#define  SYS_MODULE_NAME_RM_SNMP                    "Module-Rm-Snmp"
#define  SYS_MODULE_NAME_RM_MIB2                    "Module-Rm-Mib2"
#define  SYS_MODULE_NAME_RM_MOAM                    "Module-Rm-Moam"
#define  SYS_MODULE_NAME_RM_KB5C                    "Module-Rm-Kb5-Client"
#define  SYS_MODULE_NAME_RM_KB5S                    "Module-Rm-Kb5-Server"
#define  SYS_MODULE_NAME_RM_KB5A                    "Module-Rm-Kb5-AppServer"
#define  SYS_MODULE_NAME_RM_KBKMC                   "Module-Rm-Kbkm-Client"
#define  SYS_MODULE_NAME_RM_KBKMS                   "Module-Rm-Kbkm-Server"
#define  SYS_MODULE_NAME_RM_CBHM10                  "Module-Rm-Cbhm10"
#define  SYS_MODULE_NAME_RM_GFWM                    "Module-Rm-Gfwm"
#define  SYS_MODULE_NAME_RM_GNAT                    "Module-Rm-Gnat"
#define  SYS_MODULE_NAME_RM_CTP                     "Module-Rm-Ctp"
#define  SYS_MODULE_NAME_RM_IPSEC                   "Module-Rm-Ipsec"
#define  SYS_MODULE_NAME_RM_CCGM                    "Module-Rm-Ccgm"
#define  SYS_MODULE_NAME_RM_BEEP                    "Module-Rm-Beep"
#define  SYS_MODULE_NAME_RM_BREE                    "Module-Rm-Bree"
#define  SYS_MODULE_NAME_RM_BWRM                    "Module-Rm-Bwrm"
#define  SYS_MODULE_NAME_RM_BWSP                    "Module-Rm-Bwsp"
#define  SYS_MODULE_NAME_RM_SLAP                    "Module-Rm-Slap"
#define  SYS_MODULE_NAME_RM_TLS                     "Module-Rm-Tls"

#define  SYS_MODULE_NAME_NM_APP_PORT                "Module-Nm-App-Port"
#define  SYS_MODULE_NAME_NM_PROTOCOL                "Module-Nm-Protocol"
#define  SYS_MODULE_NAME_NM_LINK_TYPE               "Module-Nm-Link-Type"
#define  SYS_MODULE_NAME_NM_SRV_STATE               "Module-Nm-Srv-State"
#define  SYS_MODULE_NAME_NM_VPN_PROTO               "Module-Nm-Vpn-Proto"
#define  SYS_MODULE_NAME_NM_KB5C                    "Module-Nm-Kb5-Client"
#define  SYS_MODULE_NAME_NM_KB5S                    "Module-Nm-Kb5-Server"
#define  SYS_MODULE_NAME_NM_KB5A                    "Module-Nm-Kb5-AppServer"
#define  SYS_MODULE_NAME_NM_KBKMC                   "Module-Nm-Kbkm-Client"
#define  SYS_MODULE_NAME_NM_KBKMS                   "Module-Nm-Kbkm-Server"
#define  SYS_MODULE_NAME_NM_IPSEC                   "Module-Nm-Ipsec"
#define  SYS_MODULE_NAME_NM_CYFW                    "Module-Nm-Cyfw"
#define  SYS_MODULE_NAME_NM_FIREWALL                "Module-Nm-Cyfw"

#define  SYS_MODULE_NAME_SB_IP4_STACK               "Module-Sb-Ip4-Stack"
#define  SYS_MODULE_NAME_SB_WEB_SERVER              "Module-Sb-Web-Server"

/*
 * During runtime environment initialization, the Module Loader Object must go through a list of
 * registered modules (along with the pointer to object constructor function) to construct the
 * dependency tree, and load each module in certain order defined by the tree. To make the module
 * information known to Module Loader Object, all the required modules must be registered during
 * environment setup, we use following data structure to store such information. All the module
 * information must be provided in a separate config file in such format.
 */
#define  SYS_MODULE_SWITCH_MODE_LOADING             1
#define  SYS_MODULE_SWITCH_MODE_CONTROL             2
#define  SYS_MODULE_SWITCH_MODE_PROVISIONING        3
#define  SYS_MODULE_SWITCH_MODE_MANUAL              4

typedef  struct
_SYS_MODULE_DESCRIPTOR
{
    char                            ModuleName[SYS_MAX_MODULE_NAME_SIZE];
    char                            BrokerName[SYS_MAX_MODULE_NAME_SIZE];
    void*                           ModuleConstrutor;
    ULONG                           SwitchMode;
}
SYS_MODULE_DESCRIPTOR,  *PSYS_MODULE_DESCRIPTOR;


#endif
