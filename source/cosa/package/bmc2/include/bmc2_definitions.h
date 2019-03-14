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

    module:	bmc2_definitions.h

        For Broadway Management Console v2.0 Implementation (BMC2),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This file defines the configuration parameters that can be
        applied to the BMC2 Systems.

    ---------------------------------------------------------------

    environment:

        platform independent

    ---------------------------------------------------------------

    author:

        Xuechen Yang

    ---------------------------------------------------------------

    revision:

        06/20/05    initial revision.

**********************************************************************/


#ifndef  _BMC2_DEFINITIONS_
#define  _BMC2_DEFINITIONS_


#include "slap_definitions.h"

/***********************************************************
         BMC2 STANDARD ELEMENT PARAMETERS DEFINITION
***********************************************************/

#define  BMC2_CMD_ARG_TYPE_NAME_nameless            "NN"
#define  BMC2_CMD_ARG_TYPE_NAME_named               "NA"
#define  BMC2_CMD_ARG_TYPE_NAME_dependent           "$%u"
#define  BMC2_CMD_ARG_FIELD_SEPARATOR               '#'
#define  BMC2_CMD_ARG_HELP_SEPARATOR                '|'
#define  BMC2_CMD_ARG_DVALUE_SEPARATOR              '~'
#define  BMC2_CMD_ARG_HELP_DESC_START_SEPARATOR     '{'
#define  BMC2_CMD_ARG_HELP_DESC_END_SEPARATOR       '}'
#define  BMC2_CMD_ARG_DEP_SEP                       '\n'
#define  BMC2_CMD_ARG_DEP_CONDITION_SEP             '='
#define  BMC2_CMD_ARG_SYNTAX_STRING_SEPARATOR       '!'
#define  BMC2_CMD_ARG_DEP_TYPE_OPTARGN_REQUIRED     '+'
#define  BMC2_CMD_ARG_DEP_TYPE_OPTARGN_OPTIONAL     '?'
#define  BMC2_CMD_ARG_VALUE_LENGTH_START            '('
#define  BMC2_CMD_ARG_VALUE_LENGTH_END              ')'
#define  BMC2_CMD_ARG_VALUE_QSTRING_SEP             '"'
#define	 BMC2_CMD_ARG_STRING_VALUE_DUP_OK			'*'

#define  BMC2_CMD_ARG_TYPE_unknown                  0
#define  BMC2_CMD_ARG_TYPE_nameless                 1
#define  BMC2_CMD_ARG_TYPE_named                    2
#define  BMC2_CMD_ARG_TYPE_dependent                3

#define  BMC2_CMD_ARG_VTYPE_NAME_int                "int"
#define  BMC2_CMD_ARG_VTYPE_NAME_uint               "uint"
#define  BMC2_CMD_ARG_VTYPE_NAME_double             "double"
#define  BMC2_CMD_ARG_VTYPE_NAME_string             "string"
#define  BMC2_CMD_ARG_VTYPE_NAME_qstring            "qstring"
#define  BMC2_CMD_ARG_VTYPE_NAME_hex                "hex"
#define  BMC2_CMD_ARG_VTYPE_NAME_ip4addr            "ip4addr"
#define  BMC2_CMD_ARG_VTYPE_NAME_macaddr            "macaddr"
#define  BMC2_CMD_ARG_VTYPE_NAME_ip6addr            "ip6addr"
#define  BMC2_CMD_ARG_VTYPE_NAME_ip6prefix          "ip6prefix"

#define  BMC2_CMD_ARG_VTYPE_unknown                 0
#define  BMC2_CMD_ARG_VTYPE_int                     1
#define  BMC2_CMD_ARG_VTYPE_uint                    2
#define  BMC2_CMD_ARG_VTYPE_double                  3
#define  BMC2_CMD_ARG_VTYPE_string                  4
#define  BMC2_CMD_ARG_VTYPE_qstring                 5
#define  BMC2_CMD_ARG_VTYPE_hex                     6
#define  BMC2_CMD_ARG_VTYPE_ip4addr                 7
#define  BMC2_CMD_ARG_VTYPE_macaddr                 8
#define  BMC2_CMD_ARG_VTYPE_ip6addr                 9
#define  BMC2_CMD_ARG_VTYPE_ip6prefix               10

#define  BMC2_CMD_DEP_ARG_DEP_required              1       /* dep argument depends on value of one of required argument */
#define  BMC2_CMD_DEP_ARG_DEP_opt_arg_name          2       /* dep argument must exist if the depending optional argument exists */
#define  BMC2_CMD_DEP_ARG_DEP_opt_arg_name_optional 3       /* dep argument may or may not exist if the depending optional argument exists */

#define  BMC2_CMD_ARG_TYPE_UINT_min                 ((ULONG)0)
#define  BMC2_CMD_ARG_TYPE_UINT_max                 ((ULONG)-1)

#define  BMC2_CMD_ARG_TYPE_INT_min                  (int)(1<<31)
#define  BMC2_CMD_ARG_TYPE_INT_max                  (int)((((ULONG)1)<<31) - 1)

#define  BMC2_CMD_ARG_TYPE_DOUBLE_min               -1.797693e+308
#define  BMC2_CMD_ARG_TYPE_DOUBLE_max               1.797693e+308
#define  BMC_CMD_ARG_TYPE_DOUBLE_exp_max            308
#define  BMC_CMD_ARG_TYPE_DOUBLE_max_digits         "1797693"

#define  BMC2_CMD_REQ_ARG_MAX_COUNT                 32
#define  BMC2_CMD_OPT_ARG_MAX_COUNT                 32

typedef  struct
_BMC2_CMD_ARG_IP6_PREFIX
{
    UCHAR                           Ip6Addr[16];
    UCHAR                           Length;
}
BMC2_CMD_ARG_IP6_PREFIX, *PBMC2_CMD_ARG_IP6_PREFIX;

typedef  struct
_BMC2_CMD_ARG_VALUE
{
    union
    {
        int                         varInt;
        ULONG                       varUint;
        DOUBLE                      varDouble;
        char*                       varString;
        UCHAR                       varMacAddr[6];
        UCHAR                       varIp6Addr[16];
        BMC2_CMD_ARG_IP6_PREFIX     varIp6Prefix;
    } Val;
}
BMC2_CMD_ARG_VALUE, *PBMC2_CMD_ARG_VALUE;


typedef  struct
_BMC2_CMD_ARG_HELP
{
    char*                           pItem;
    char*                           pDescription;
}
BMC2_CMD_ARG_HELP, *PBMC2_CMD_ARG_HELP;

typedef  struct
_BMC2_CMD_ARG_VRANGE
{
    union
    {
        int                         varInt;
        ULONG                       varUint;
        DOUBLE                      varDouble;
        UCHAR                       varIp6Addr[16];
    } Min;

    union
    {
        int                         varInt;
        ULONG                       varUint;
        DOUBLE                      varDouble;
        UCHAR                       varIp6Addr[16];
    } Max;

    char**                          pStringArray;       /* Min.varUint or Max.varUint indicates the number of items in array */
    ULONG                           ulStringCount;      /* number of strings in the array above, used when data type is uint/int */
}
BMC2_CMD_ARG_VRANGE, *PBMC2_CMD_ARG_VRANGE;

#define  Bmc2CreateCmdArgValueRange(pCmdArgVrange)                                  \
    do {                                                                            \
        pCmdArgVrange = (PBMC2_CMD_ARG_VRANGE)AnscAllocateMemory                    \
                            (                                                       \
                                sizeof(BMC2_CMD_ARG_VRANGE)                         \
                            );                                                      \
        if ( pCmdArgVrange )                                                        \
        {                                                                           \
            pCmdArgVrange->Min.varUint  = 0;                                        \
            pCmdArgVrange->Max.varUint  = (ULONG)-1;                                \
            pCmdArgVrange->pStringArray = NULL;                                     \
        }                                                                           \
    } while (0)


typedef  struct
_BMC2_CMD_SIMPLE_ARG
{
    ULONG                           ValueType;
    ULONG                           ValueMinCount;  /* default 1 */
    ULONG                           ValueMaxCount;  /* default 1 */
    ULONG                           ValueMinLength; /* minimum value length - meaningful to string, hex etc. */
    ULONG                           ValueMaxLength; /* maximum value length */
	BOOL							bStrDupAllowed; /* TRUE means user may input duplicate string defined in string value range */

    char*                           DisplayName;    /* NULL if it's not specified in grammar */
    char*                           DisplayNameHelp;/* help on name of a named argument */

    ULONG                           ValueCount;     /* notes: value list must be a comma separated string */
    PBMC2_CMD_ARG_VALUE             pValue;

    PBMC2_CMD_ARG_VRANGE            pValueRange;    /* mainly for validation purpose */

    ULONG                           HelpItemCount;
    PBMC2_CMD_ARG_HELP              pHelpItems;
}
BMC2_CMD_SIMPLE_ARG, *PBMC2_CMD_SIMPLE_ARG;

typedef  struct
_BMC2_CMD_DEP_ARG_BRANCH
{
    ULONG                           ulArgDepOn;     /* type of dependency - on required argument value, optional argument name etc. */
    ULONG                           ulArgIndex;     /* the index to the argument this arg depends on, 1 based */
    char*                           pArgValue;      /* the value of depending argument, NULL meaning any value matches */

    ULONG                           Type;
    BMC2_CMD_SIMPLE_ARG             SimpleArg;
}
BMC2_CMD_DEP_ARG_BRANCH, *PBMC2_CMD_DEP_ARG_BRANCH;

typedef  struct
_BMC2_CMD_DEP_ARG
{
    ULONG                           ulArgBranchCount;
    PBMC2_CMD_DEP_ARG_BRANCH        pArgBranches;
}
BMC2_CMD_DEP_ARG, *PBMC2_CMD_DEP_ARG;


typedef   struct
_BMC2_CMD_ARGUMENT
{
    ULONG                           Type;

    union
    {
        BMC2_CMD_SIMPLE_ARG         Simple;
        BMC2_CMD_DEP_ARG            Dependent;
    } Arg;
}
BMC2_CMD_ARGUMENT, *PBMC2_CMD_ARGUMENT;


typedef  struct
_BMC2_CMD_HELP_SYNTAX
{
    ULONG                           ulReqArgCount;
    PBMC2_CMD_ARGUMENT              pReqArgs;

    ULONG                           ulOptArgCount;
    PBMC2_CMD_ARGUMENT              pOptArgs;
}
BMC2_CMD_HELP_SYNTAX, *PBMC2_CMD_HELP_SYNTAX;

#define  Bmc2CreateCmdHelpSyntax(pCmdHelpSyntax)                                    \
    do {                                                                            \
        pCmdHelpSyntax =                                                            \
            (PBMC2_CMD_HELP_SYNTAX)AnscAllocateMemory                               \
                (                                                                   \
                    sizeof(BMC2_CMD_HELP_SYNTAX)                                    \
                );                                                                  \
        if ( pCmdHelpSyntax )                                                       \
        {                                                                           \
            (pCmdHelpSyntax)->ulReqArgCount = 0;                                    \
            (pCmdHelpSyntax)->pReqArgs      = NULL;                                 \
            (pCmdHelpSyntax)->ulOptArgCount = 0;                                    \
            (pCmdHelpSyntax)->pOptArgs      = NULL;                                 \
        }                                                                           \
    } while (0)



/*
 * For each CLI command, there's a corresponding SCP (Shell Command Page). Such mapping is usually
 * done automatically by appending ".scp" to the command to construct the SCP file name. However,
 * it's a good idea to create a mapping layer so we can easily support features like Alias command.
 * In addition, we need to maintain an ACL (Access Control List) for each command.
 */
#define  BMC2_DEF_CLI_SEPARATORS                    " "

typedef  struct
_BMC2_COMMAND_PROPERTY
{
    SINGLE_LINK_ENTRY               Linkage;
    /* '*' may be included in domain name pattern, e.g. 'pvc*' to match domains 'pvc-1', 'pvc-2' etc. 
     * A single '*' or NULL means the command applies to all domains.
     * Currently we only support prefix matching that we think this is sufficient. So overall,
     * we should only see the following possible scenarios.
     *  1. NULL - to match any domains
     *  2. *    - to match any domains as well
     *  3. specific domain name, such as 'adsl', 'auth' etc.
     *  4. prefixed domain, e.g. 'pvc-*' to match 'pvc-1', 'pvc-2' and etc.
     */
    char*                           DomainNamePattern;  
    char*                           CommandName;
    char*                           ScpPageName;
    BOOL                            bReadOnly;
    BOOL                            bAuthorized;
    ULONG                           Permissions;

    /* help syntax - used to provide inline help */
    PBMC2_CMD_HELP_SYNTAX           pHelpSyntax;

    BOOL                            bDisabled;      /* a command can be disabled temporarily */
}
BMC2_COMMAND_PROPERTY,  *PBMC2_COMMAND_PROPERTY;

#define  ACCESS_BMC2_COMMAND_PROPERTY(p)            \
         ACCESS_CONTAINER(p, BMC2_COMMAND_PROPERTY, Linkage)


/*
 * SLAP maintains all the system-created object containers in a one dimensional table sorted by
 * containers' names; while external modules may dynamically create containers in runtime, but
 * these containers may be isolated from the internal management. BMC2 creates several different
 * type of containers, each of them has a different scope.
 */
#define  BMC2_SLAP_CONTAINER_APP                    "Bmc2.Application"
#define  BMC2_SLAP_CONTAINER_SERVER                 "Bmc2.Server"
#define  BMC2_SLAP_CONTAINER_TERMINAL               "Bmc2.Terminal"
#define  BMC2_SLAP_CONTAINER_DOMAIN                 "Bmc2.Domain"
#define  BMC2_SLAP_CONTAINER_REQUEST                "Bmc2.Request"


#endif
