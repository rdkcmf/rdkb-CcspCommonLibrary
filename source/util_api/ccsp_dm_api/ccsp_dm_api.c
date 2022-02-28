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

/**
 * Unified API to access CCSP Data Model.
 * It can be used for both local and remote process.
 *
 * Remote access is implemented with "baseIf" API.
 * Local access is implemented with "CcMbi"
 * which is BaseIf-like (MPA wrapper).
 *
 * leichen2@cisco.com
 * 2013.11 - Initailize
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define __CDM_LOCAL_ACCESS

#include "ansc_platform.h"
#include "ccsp_message_bus.h"
#include "ccsp_base_api.h"
#include "ccsp_trace.h"
#include "ccsp_memory.h"
#ifdef __CDM_LOCAL_ACCESS
#include "messagebus_interface_helper.h"
#endif
#include "ansc_string_util.h"
#include "ccsp_dm_api.h"
#include "safec_lib_common.h"

#define CDM_DEF_CR              "com.cisco.spvtg.ccsp.CR"
#define CDM_DEF_SUBSYS          "eRT."
#define CDM_DEF_COMPID          "com.cisco.spvtg.ccsp.cdm"
#define CDM_DEF_CONF            "/tmp/ccsp_msg.cfg"

#define CDM_ID_SZ               128

struct CdmObj {
    void        *busHdl;
    char        crId[CDM_ID_SZ];
    char        subSys[CDM_ID_SZ];
    char        compId[CDM_ID_SZ];
    char        cnfFil[CDM_ID_SZ];
    int         destory; /* busHdl need destory when term */
};

struct CdmComp {
#ifdef __CDM_LOCAL_ACCESS
    int         isLocal;
#endif
    char        *compId;
    char        *dbusPath;
    int         isPath; /* union is path or parameter */
    int         paramCnt;
    union {
        parameterValStruct_t *paramVals;
        char    **paths;
    };
};

static struct CdmObj cdm;

/* e.g., Device.IP.Interface.1.Address.1.IPv4Address */
static int IsPathParam(const char *path)
{
    if (strlen(path) && path[strlen(path) - 1] != '.')
        return 1;
    else
        return 0;
}

/* e.g., Device.IP.Interface.1. */
static int IsPathIns(const char *path)
{
    const char *cp;

    if (strlen(path) < 2 || path[strlen(path) - 1] != '.')
        return 0;

    for (cp = &path[strlen(path) - 2]; cp != path && (*cp != '.'); cp--)
        if (!isdigit(*cp))
            return 0;

    if (cp != path && *cp == '.' && isdigit(cp[1]))
        return 1;
    else
        return 0;
}

/* e.g., Device.IP.Interface. */
static int IsPathObj(const char *path)
{
    return strlen(path) && !IsPathParam(path) && !IsPathIns(path);
}

static DmParamVal_t *CloneParamVal(const DmParamVal_t *orig)
{
    DmParamVal_t *new = NULL;

    if (!orig)
        return NULL;

    new = (void *)AnscAllocateMemory(sizeof(DmParamVal_t));
    if (new == NULL)
        return NULL;

    new->type = orig->type;
    new->len = orig->len;

    switch (orig->type) {
    case DM_PT_STR:
        new->v_str = (void *)AnscAllocateMemory(orig->len + 1);
        if (new->v_str == NULL) {
            AnscFreeMemory(new);
            return NULL;
        }
        memcpy(new->v_str, orig->v_str, orig->len);
        new->v_str[orig->len] = '\0';
        break;
    case DM_PT_DATETIME:
        new->v_datetime = (void *)AnscDupString((PUCHAR)orig->v_datetime);
        if (new->v_datetime == NULL) {
            AnscFreeMemory(new);
            return NULL;
        }
        break;
    case DM_PT_BASE64:
        new->v_base64 = (void *)AnscDupString((PUCHAR)orig->v_base64);
        if (new->v_base64 == NULL) {
            AnscFreeMemory(new);
            return NULL;
        }
        break;
    case DM_PT_BIN:
        new->v_bin = (void *)AnscAllocateMemory(orig->len + 1);
        if (new->v_bin == NULL) {
            AnscFreeMemory(new);
            return NULL;
        }
        memcpy(new->v_bin, orig->v_bin, orig->len);
        new->v_bin[orig->len] = '\0'; // harmless and may useful
        break;
    case DM_PT_BOOL:
        new->v_bool = orig->v_bool;
        break;
    case DM_PT_INT:
        new->v_int = orig->v_int;
        break;
    case DM_PT_UINT:
        new->v_uint = orig->v_uint;
        break;
    case DM_PT_LONG:
        new->v_long = orig->v_long;
        break;
    case DM_PT_ULONG:
        new->v_ulong = orig->v_ulong;
        break;
    case DM_PT_FLOAT:
        new->v_float = orig->v_float;
        break;
    case DM_PT_DOUBLE:
        new->v_double = orig->v_double;
        break;
    default:
        AnscFreeMemory(new);
        return NULL;
    }

    return new;
}

static DmErr_t TransParamStru(const parameterValStruct_t *from, DmParam_t *to, int raw)
{
    DmParamVal_t *val;

    if (!from || !to)
        return CCSP_ERR_INVALID_ARGUMENTS;

    snprintf(to->name, sizeof(to->name), "%s", from->parameterName);
    val = to->value = (void *)AnscAllocateMemory(sizeof(DmParamVal_t));
    if (val == NULL)
        return CCSP_ERR_MEMORY_ALLOC_FAIL;

    if (raw) {
        switch (from->type) {
        case ccsp_string:
            val->type = DM_PT_STR;
            break;
        case ccsp_int:
            val->type = DM_PT_INT;
            break;
        case ccsp_unsignedInt:
            val->type = DM_PT_UINT;
            break;
        case ccsp_boolean:
            val->type = DM_PT_BOOL;
            break;
        case ccsp_long:
            val->type = DM_PT_LONG;
            break;
        case ccsp_unsignedLong:
            val->type = DM_PT_ULONG;
            break;
        case ccsp_dateTime:
            val->type = DM_PT_DATETIME;
            break;
        case ccsp_base64:
            val->type = DM_PT_BASE64;
            break;
        case ccsp_float:
            val->type = DM_PT_FLOAT;
            break;
        case ccsp_double:
            val->type = DM_PT_FLOAT;
            break;
        case ccsp_byte:
            // XXX: swith from HEX string to binary ?
            AnscFreeMemory(to->value);
            return CCSP_ERR_NOT_SUPPORT;
        case ccsp_none:
        default:
            return CCSP_ERR_INVALID_PARAMETER_TYPE;
        }

        val->v_raw = (void *)AnscDupString((PUCHAR)from->parameterValue);
        if (val->v_raw == NULL) {
            AnscFreeMemory(to->value);
            return CCSP_ERR_MEMORY_ALLOC_FAIL;
        }
        val->len = strlen(val->v_raw);

        return CCSP_SUCCESS;
    }

    switch (from->type) {
    case ccsp_string:
        val->type = DM_PT_STR;
        /* baseif only support ASCII string */
        val->v_str = (void *)AnscDupString((PUCHAR)from->parameterValue);
        if (val->v_str == NULL) {
            AnscFreeMemory(to->value);
            return CCSP_ERR_MEMORY_ALLOC_FAIL;
        }
        val->len = strlen(val->v_str);
        break;
    case ccsp_int:
        val->type = DM_PT_INT;
        val->len = sizeof(int);
        val->v_int = atoi(from->parameterValue);
        break;
    case ccsp_unsignedInt:
        val->type = DM_PT_UINT;
        val->len = sizeof(unsigned);
        sscanf(from->parameterValue, "%u", &val->v_uint);
        break;
    case ccsp_boolean:
        val->type = DM_PT_BOOL;
        val->len = sizeof(bool);
        if (strcmp(from->parameterValue, "1") == 0
                || strcasecmp(from->parameterValue, "true") == 0)
            val->v_bool = true;
        else
            val->v_bool = false;
        break;
    case ccsp_long:
        val->type = DM_PT_LONG;
        val->len = sizeof(long);
        val->v_int = atol(from->parameterValue);
        break;
    case ccsp_unsignedLong:
        val->type = DM_PT_ULONG;
        val->len = sizeof(unsigned long);
        sscanf(from->parameterValue, "%lu", &val->v_ulong);
        break;
    case ccsp_dateTime:
        val->type = DM_PT_DATETIME;
        val->v_datetime = (void *)AnscDupString((PUCHAR)from->parameterValue);
        if (val->v_datetime == NULL) {
            AnscFreeMemory(to->value);
            return CCSP_ERR_MEMORY_ALLOC_FAIL;
        }
        val->len = strlen(val->v_str);
        break;
    case ccsp_base64:
        val->type = DM_PT_BASE64;
        val->v_base64 = (void *)AnscDupString((PUCHAR)from->parameterValue);
        if (val->v_base64 == NULL) {
            AnscFreeMemory(to->value);
            return CCSP_ERR_MEMORY_ALLOC_FAIL;
        }
        val->len = strlen(val->v_str);
        break;
    case ccsp_float:
        val->type = DM_PT_FLOAT;
        val->len = sizeof(float);
        sscanf(from->parameterValue, "%f", &val->v_float);
        break;
    case ccsp_double:
        val->type = DM_PT_FLOAT;
        val->len = sizeof(double);
        sscanf(from->parameterValue, "%lf", &val->v_double);
        break;
    case ccsp_byte:
        // XXX: swith from HEX string to binary ?
        AnscFreeMemory(to->value);
        return CCSP_ERR_NOT_SUPPORT;
    case ccsp_none:
    default:
        return CCSP_ERR_INVALID_PARAMETER_TYPE;
    }

    return CCSP_SUCCESS;
}

static DmErr_t TransParam(const DmParam_t *from, parameterValStruct_t *to, int raw)
{
    char buf[64];
    DmErr_t err = CCSP_ERR_MEMORY_ALLOC_FAIL;
    DmParamVal_t *fVal;

    if (!from || !to || !from->value)
        return CCSP_ERR_INVALID_ARGUMENTS;
    fVal = from->value;

    to->parameterName = to->parameterValue = NULL;
    to->parameterName = (void *)AnscDupString((PUCHAR)from->name);
    if (to->parameterName == NULL)
        return CCSP_ERR_MEMORY_ALLOC_FAIL;

    if (raw) {
        switch (fVal->type) {
        case DM_PT_STR: 
            to->type = ccsp_string;
            break;
        case DM_PT_INT: 
            to->type = ccsp_int;
            break;
        case DM_PT_UINT: 
            to->type = ccsp_unsignedInt;
            break;
        case DM_PT_BOOL: 
            to->type = ccsp_boolean;
            break;
        case DM_PT_DATETIME:
            to->type = ccsp_dateTime;
            break;
        case DM_PT_BASE64:
            to->type = ccsp_base64;
            break;
        case DM_PT_LONG: 
            to->type = ccsp_long;
            break;
        case DM_PT_ULONG: 
            to->type = ccsp_unsignedLong;
            break;
        case DM_PT_FLOAT: 
            to->type = ccsp_float;
            break;
        case DM_PT_DOUBLE:
            to->type = ccsp_double;
            break;
        case DM_PT_BIN:
            err =  CCSP_ERR_NOT_SUPPORT;
            goto errout;
        default:
            err =  CCSP_ERR_INVALID_PARAMETER_TYPE;
            goto errout;
        }

        to->parameterValue = (void *)AnscDupString((PUCHAR)fVal->v_raw);
        if (to->parameterValue == NULL)
            goto errout;

        return CCSP_SUCCESS;
    }

    switch (fVal->type) {
    case DM_PT_STR: 
        to->type = ccsp_string;
        to->parameterValue = (void *)AnscDupString((PUCHAR)fVal->v_str);
        break;
    case DM_PT_INT: 
        to->type = ccsp_int;
        snprintf(buf, sizeof(buf), "%d", fVal->v_int);
        to->parameterValue = (void *)AnscDupString((PUCHAR)buf);
        break;
    case DM_PT_UINT: 
        to->type = ccsp_unsignedInt;
        snprintf(buf, sizeof(buf), "%u", fVal->v_uint);
        to->parameterValue = (void *)AnscDupString((PUCHAR)buf);
        break;
    case DM_PT_BOOL: 
        to->type = ccsp_boolean;
        if (fVal->v_bool)
            to->parameterValue = (void *)AnscDupString((PUCHAR)"true");
        else
            to->parameterValue = (void *)AnscDupString((PUCHAR)"false");
        break;
    case DM_PT_DATETIME:
        to->type = ccsp_dateTime;
        to->parameterValue = (void *)AnscDupString((PUCHAR)fVal->v_datetime);
        break;
    case DM_PT_BASE64:
        to->type = ccsp_base64;
        to->parameterValue = (void *)AnscDupString((PUCHAR)fVal->v_base64);
        break;
    case DM_PT_LONG: 
        to->type = ccsp_long;
        snprintf(buf, sizeof(buf), "%ld", fVal->v_long);
        to->parameterValue = (void *)AnscDupString((PUCHAR)buf);
        break;
    case DM_PT_ULONG: 
        to->type = ccsp_unsignedLong;
        snprintf(buf, sizeof(buf), "%lu", fVal->v_ulong);
        to->parameterValue = (void *)AnscDupString((PUCHAR)buf);
        break;
    case DM_PT_FLOAT: 
        to->type = ccsp_float;
        snprintf(buf, sizeof(buf), "%f", fVal->v_float);
        to->parameterValue = (void *)AnscDupString((PUCHAR)buf);
        break;
    case DM_PT_DOUBLE:
        to->type = ccsp_double;
        snprintf(buf, sizeof(buf), "%f", fVal->v_double);
        to->parameterValue = (void *)AnscDupString((PUCHAR)buf);
        break;
    case DM_PT_BIN:
        err =  CCSP_ERR_NOT_SUPPORT;
        goto errout;
    default:
        err =  CCSP_ERR_INVALID_PARAMETER_TYPE;
        goto errout;
    }

    if (to->parameterValue == NULL)
        goto errout;

    return CCSP_SUCCESS;

errout:
    if (to->parameterName)
        AnscFreeMemory(to->parameterName);
    if (to->parameterValue)
        AnscFreeMemory(to->parameterValue);
    return err;
}

static void FreeCompList(struct CdmComp *comps, int compCnt)
{
    int i, j;

    if (!comps)
        return;

    for (i = 0; i < compCnt; i++) {
        if (comps[i].compId)
            AnscFreeMemory(comps[i].compId);
        if (comps[i].dbusPath)
            AnscFreeMemory(comps[i].dbusPath);

        if (comps[i].isPath && comps[i].paths) {
           for (j = 0; j < comps[i].paramCnt; j++) {
               if(comps[i].paths[j])
                   AnscFreeMemory(comps[i].paths[j]);
           }
           AnscFreeMemory(comps[i].paths);
        } else if (!comps[i].isPath && comps[i].paramVals) {
            for (j = 0; j < comps[i].paramCnt; j++) {
                if (comps[i].paramVals[j].parameterName)
                    AnscFreeMemory(comps[i].paramVals[j].parameterName);
                if (comps[i].paramVals[j].parameterValue)
                    AnscFreeMemory(comps[i].paramVals[j].parameterValue);
            }

            AnscFreeMemory(comps[i].paramVals);
        }
    }
    AnscFreeMemory(comps);
}

/* 
 * classifying paths for different components 
 * and also checks if it's local.
 */
static DmErr_t LoadCompPathList(const char *paths[], int cnt, 
        struct CdmComp **comps, int *compCnt)
{
    struct CdmComp      *compList = NULL;
    int                 compListSize;
    int                 i, j, k, offset;
    struct CdmComp      *comp;
    DmErr_t             err;
    void                *ptr;
    componentStruct_t   **compStru = NULL;
    int                 compStruNum = 0;

    if (!paths || !comps || !compCnt)
        return CCSP_ERR_INVALID_ARGUMENTS;

    /* let's alloc *cnt number of CdmComp{} first, 
     * it may need to expend if not enough. */
    offset = 0, compListSize = cnt;
    compList = (void *)AnscAllocateMemory(compListSize * sizeof(struct CdmComp));
    if (compList == NULL)
        return CCSP_ERR_MEMORY_ALLOC_FAIL;

    /* for each path we got */
    for (i = 0; i < cnt; i++) {
        err = CcspBaseIf_discComponentSupportingNamespace(cdm.busHdl, cdm.crId,
                paths[i], cdm.subSys, &compStru, &compStruNum);
        if (err != CCSP_SUCCESS)
            continue;

        /* 
         * for each compStru contains that path, 
         * we need record it to CdmComp{} List.
         */
        for (j = 0; j < compStruNum; j++) {
            /* travelling current CdmComp{} list */
            for (k = 0; k < offset; k++) {
                comp = &compList[k];

                /* adding new path to existing CdmComp{} */
                if (strcmp(comp->compId, compStru[j]->componentName) == 0) {
                    ptr = (void *)AnscReallocMemory(comp->paths, sizeof(const char *) * (comp->paramCnt),
                            sizeof(const char *) * (comp->paramCnt + 1));
                    if (ptr == NULL)
                        goto nomem;
                    comp->paths = ptr;

                    comp->paths[comp->paramCnt] = (void *)AnscDupString((PUCHAR)paths[i]);
                    if (comp->paths[comp->paramCnt] == NULL)
                        goto nomem;
                    comp->paramCnt++;

                    break;
                }
            }

            /* 
             * adding new CdmComp{} to list,
             * expend the list if need
             */
            if (k == offset) {
                if (offset == compListSize) {
                    /* expend the complist first */
                    compListSize += 10;
                    ptr = (void *)AnscReallocMemory(compList, sizeof(struct CdmComp) * offset, 
                            sizeof(struct CdmComp) * compListSize);
                    if (ptr == NULL)
                        goto nomem;
                    compList = ptr;
                }

                comp = &compList[offset++];
                comp->isPath = 1;
#ifdef __CDM_LOCAL_ACCESS
                if (strcmp(compStru[j]->componentName, cdm.compId) == 0)
                    comp->isLocal = 1;
                else
                    comp->isLocal = 0;
#endif
                comp->compId = (void *)AnscDupString((PUCHAR)compStru[j]->componentName);
                comp->dbusPath = (void *)AnscDupString((PUCHAR)compStru[j]->dbusPath);
                comp->paths = (void *)AnscAllocateMemory(sizeof(const char *) * 1);
                if (comp->paths != NULL)
                    comp->paths[0] = (void *)AnscDupString((PUCHAR)paths[i]);

                if (comp->compId == NULL || comp->dbusPath == NULL
                        || comp->paths == NULL || comp->paths[0] == NULL)
                    goto nomem;
                comp->paramCnt = 1;
            }
        }

        free_componentStruct_t(cdm.busHdl, compStruNum, compStru);
    }

    *comps = compList;
    *compCnt = offset;
    return CCSP_SUCCESS;

nomem:
    if (compStru && compStruNum)
        free_componentStruct_t(cdm.busHdl, compStruNum, compStru);
    FreeCompList(compList, offset);
    return CCSP_ERR_MEMORY_ALLOC_FAIL;
}

/* 
 * classifying params for different components 
 * and also marking if it's local.
 * also transfer the format form DmParam_t{} to parameterValStruct_t{}
 */
static DmErr_t LoadCompParamList(const DmParam_t params[], int cnt,
        struct CdmComp **comps, int *compCnt, int raw)
{
    struct CdmComp      *compList = NULL;
    struct CdmComp      *comp = NULL;
    int                 i=0, j=0, offset=0;
    componentStruct_t   **compStru = NULL;
    int                 compStruNum =0;
    DmErr_t             err = 0;
    parameterValStruct_t *paramVal = NULL;
    void                *ptr = NULL;

    if (!params || !comps || !compCnt)
        return CCSP_ERR_INVALID_ARGUMENTS;

    /* 
     * it's not possible a param belongs to more than one comp.
     * so, the wrost case is each param holded by different comp.
     */
    offset = 0;
    compList = (void *)AnscAllocateMemory(cnt * sizeof(struct CdmComp));
    if (compList == NULL)
        return CCSP_ERR_MEMORY_ALLOC_FAIL;

    /* for each DmParam_t{} we got */
    for (i = 0; i < cnt; i++) {
        if (!IsPathParam(params[i].name))
            continue;

        err = CcspBaseIf_discComponentSupportingNamespace(cdm.busHdl, cdm.crId,
                params[i].name, cdm.subSys, &compStru, &compStruNum);
        if ((err != CCSP_SUCCESS) || (compStru == NULL)) /* RDKB-6129, CID-33395, null check before use*/
            continue;
        if (compStruNum != 1) {
            free_componentStruct_t(cdm.busHdl, compStruNum, compStru);
            continue;
        }

        /* travelling current CdmComp{} list */
        for (j = 0; j < offset; j++) {
            comp = &compList[j];

            /* adding new param to existing component */
            if (strcmp(comp->compId, compStru[0]->componentName) == 0) {
                ptr = (void *)AnscReallocMemory(comp->paramVals, sizeof(parameterValStruct_t) * (comp->paramCnt), 
                        sizeof(parameterValStruct_t) * (comp->paramCnt + 1));
                if (ptr == NULL) {
                    goto nomem;
                }
                comp->paramVals = ptr;

                paramVal = &comp->paramVals[comp->paramCnt];
                comp->paramCnt++;

                /* need clear the buffer for AnscFreeMemory in case ->value is not empty */
                memset(paramVal, 0, sizeof(parameterValStruct_t));
                err = TransParam(&params[i], paramVal, raw);
                if (err != CCSP_SUCCESS)
                    goto nomem;

                break;
            }
        }

        /* adding new component */
        if (j == offset && offset < cnt) {
            comp = &compList[offset++];
#ifdef __CDM_LOCAL_ACCESS
            if (strcmp(compStru[0]->componentName, cdm.compId) == 0)
                comp->isLocal = 1;
            else
                comp->isLocal = 0;
#endif
            comp->compId = (void *)AnscDupString((PUCHAR)compStru[0]->componentName);
            comp->dbusPath = (void *)AnscDupString((PUCHAR)compStru[0]->dbusPath);
            if (comp->compId == NULL || comp->dbusPath == NULL)
                goto nomem;
            comp->paramVals = (void *)AnscAllocateMemory(sizeof(parameterValStruct_t));
            if (comp->paramVals == NULL)
                goto nomem;

            paramVal = &comp->paramVals[0];
            comp->paramCnt = 1;

            err = TransParam(&params[i], paramVal, raw);
            if (err != CCSP_SUCCESS)
                goto nomem;
        }

        free_componentStruct_t(cdm.busHdl, compStruNum, compStru);
        compStru = NULL, compStruNum = 0;
    }

    *comps = compList;
    *compCnt = offset;
    return CCSP_SUCCESS;

nomem:
    if (compStruNum && compStru)
        free_componentStruct_t(cdm.busHdl, compStruNum, compStru);
    FreeCompList(compList, offset);
    return CCSP_ERR_MEMORY_ALLOC_FAIL;
}

static DmErr_t AddDelObj(const char *obj, int *ins, int add)
{
    componentStruct_t **compStru = NULL;
    int compNum = 0;
    DmErr_t err;

    if (!obj || (add && !ins))
        return CCSP_ERR_INVALID_ARGUMENTS;

    err = CcspBaseIf_discComponentSupportingNamespace(cdm.busHdl, 
            cdm.crId, obj, cdm.subSys, &compStru, &compNum);
    if (err != CCSP_SUCCESS)
        return err;

    /* object can be added/deleted should not separated */
    if (compNum != 1) {
        free_componentStruct_t(cdm.busHdl, compNum, compStru);
        return CCSP_ERR_INTERNAL_ERROR;
    }

#ifdef __CDM_LOCAL_ACCESS
    if (strcmp(compStru[0]->componentName, cdm.compId) == 0) {
        if (add)
            err = CcspCcMbi_AddTblRow(0, (char *)obj, ins, NULL);
        else
            err = CcspCcMbi_DeleteTblRow(0, (char *)obj, NULL);
    } else {
#endif
        if (add) {
            err = CcspBaseIf_AddTblRow(cdm.busHdl, compStru[0]->componentName, 
                    compStru[0]->dbusPath, 0, (char *)obj, ins);
        } else {
            err = CcspBaseIf_DeleteTblRow(cdm.busHdl, compStru[0]->componentName, 
                    compStru[0]->dbusPath, 0, (char *)obj);
        }
#ifdef __CDM_LOCAL_ACCESS
    }
#endif

    free_componentStruct_t(cdm.busHdl, compNum, compStru);
    return err;
}

static unsigned CompIdGetWriteId(const char *compId)
{
    /**
     * is not good to copy the code below:
     *
     *   if ( _ansc_strcmp(bus_info->component_id, "ccsp.busclient" ) == 0 )
     *   {
     *       writeID = DSLH_MPA_ACCESS_CONTROL_CLIENTTOOL;
     *   }
     *   ... ...
     *
     * "BaseIf_Get" use compId, while "BaseIf_Set" using writeId.
     * Every baseIf caller have to do compid/writeid translation.
     * we should change the BaseIf_Set API to use compId.
     * moreover, writeId is not passed to DML.
     */
    UNREFERENCED_PARAMETER(compId);
    return 0;
}

DmErr_t Cdm_Init(const void *busHdl, const char *subSys, const char *cnfFil,
                const char *crId, const char *compId)
{
    DmErr_t err;
    errno_t rc = -1;

    memset(&cdm, 0, sizeof(cdm));

    cdm.busHdl   = (void *)busHdl;
    cdm.destory  = 0;

    snprintf(cdm.subSys, sizeof(cdm.subSys), "%s", subSys ? subSys : CDM_DEF_SUBSYS);
    snprintf(cdm.cnfFil, sizeof(cdm.cnfFil), "%s", cnfFil ? cnfFil : CDM_DEF_CONF);
    rc = sprintf_s(cdm.crId, sizeof(cdm.crId), "%s%s", cdm.subSys, crId ? crId : CDM_DEF_CR);
    if(rc < EOK) {
        ERR_CHK(rc);
    }
    rc = sprintf_s(cdm.compId, sizeof(cdm.compId), "%s%s", cdm.subSys, compId ? compId : CDM_DEF_COMPID);
    if(rc < EOK) {
        ERR_CHK(rc);
    }
  
    cdm.subSys[0] = '\0'; /* baseif/ccmbi do not need subsys */

    if (cdm.busHdl == NULL) {
        err = CCSP_Message_Bus_Init(cdm.compId, cdm.cnfFil, &cdm.busHdl, 
                (CCSP_MESSAGE_BUS_MALLOC)Ansc_AllocateMemory_Callback, 
                (CCSP_MESSAGE_BUS_FREE)Ansc_FreeMemory_Callback);
        if (err != 0)
            return CCSP_ERR_INTERNAL_ERROR;
        cdm.destory = 1;
    }

    return CCSP_SUCCESS;
}

DmErr_t Cdm_Term(void)
{
    if (!cdm.busHdl)
        return CCSP_ERR_INTERNAL_ERROR;

    if (cdm.destory)
        CCSP_Message_Bus_Exit(cdm.busHdl);

    return CCSP_SUCCESS;
}

DmErr_t Cdm_GetParamBool(const char *param, bool *val)
{
    DmErr_t err = 0;
    DmParamVal_t *dmVal = NULL;

    if (!param || !val)
        return CCSP_ERR_INVALID_ARGUMENTS;

    err = Cdm_GetParam(param, &dmVal);
    if (err != CCSP_SUCCESS)
        return err;

    if (dmVal->type != DM_PT_BOOL) {
        Cdm_FreeParam(dmVal);
        return CCSP_ERR_INVALID_PARAMETER_TYPE;
    }

    *val = dmVal->v_bool;
    Cdm_FreeParam(dmVal);
    return CCSP_SUCCESS;
}

DmErr_t Cdm_GetParamInt(const char *param, int *val)
{
    DmErr_t err = 0;
    DmParamVal_t *dmVal = NULL; 

    if (!param || !val)
        return CCSP_ERR_INVALID_ARGUMENTS;

    err = Cdm_GetParam(param, &dmVal);
    if (err != CCSP_SUCCESS)
        return err;

    if (dmVal->type != DM_PT_INT) {
        Cdm_FreeParam(dmVal);
        return CCSP_ERR_INVALID_PARAMETER_TYPE;
    }

    *val = dmVal->v_int;
    Cdm_FreeParam(dmVal);
    return CCSP_SUCCESS;
}

DmErr_t Cdm_GetParamString(const char *param, char *val, size_t size)
{
    DmErr_t err = 0;
    DmParamVal_t *dmVal = NULL;

    if (!param || !val)
        return CCSP_ERR_INVALID_ARGUMENTS;

    err = Cdm_GetParam(param, &dmVal);
    /*CID: 63401 Dereference before null check*/
    if (err != CCSP_SUCCESS || dmVal == NULL)
        return err;

    if (dmVal->type != DM_PT_STR) {
		err =  CCSP_ERR_INVALID_PARAMETER_TYPE;
        goto errout;
    }

    if (size <= dmVal->len) {
		err =  CCSP_ERR_RESOURCE_EXCEEDED;
        goto errout;
    }

    /* although it's not true for now,
     * let's assume Cdm_GetParam() can return not only ASCII string,
     * so snprintf() is not used here. */
    memcpy(val, dmVal->v_str, dmVal->len);
    val[dmVal->len] = '\0'; /* this API is used for nil tailed str */

	err = CCSP_SUCCESS;

errout:
	if(dmVal->v_str)
		AnscFreeMemory(dmVal->v_str);
	if(dmVal)
		AnscFreeMemory(dmVal);

    return err;
}

DmErr_t Cdm_GetParamUlong(const char *param, unsigned long *val)
{
    DmErr_t err = 0;
    DmParamVal_t *dmVal = NULL;

    if (!param || !val)
        return CCSP_ERR_INVALID_ARGUMENTS;

    err = Cdm_GetParam(param, &dmVal);
    if (err != CCSP_SUCCESS)
        return err;

    if (dmVal->type != DM_PT_ULONG) {
        Cdm_FreeParam(dmVal);
        return CCSP_ERR_INVALID_PARAMETER_TYPE;
    }

    *val = dmVal->v_ulong;
    Cdm_FreeParam(dmVal);
    return CCSP_SUCCESS;
}

DmErr_t Cdm_GetParamUint(const char *param, unsigned int *val)
{
    DmErr_t err = 0;
    DmParamVal_t *dmVal = NULL;

    if (!param || !val)
        return CCSP_ERR_INVALID_ARGUMENTS;

    err = Cdm_GetParam(param, &dmVal);
    if (err != CCSP_SUCCESS || (dmVal == NULL))
        return err;

    if (dmVal->type != DM_PT_UINT) {
		err = CCSP_ERR_INVALID_PARAMETER_TYPE;
        goto lbl;
    }

	*val = dmVal->v_uint;
    err = CCSP_SUCCESS;
lbl:
	AnscFreeMemory(dmVal);
    return err;
}

DmErr_t Cdm_SetParamBool(const char *param, bool val, int commit)
{
    DmParamVal_t dmVal;

    if (!param)
        return CCSP_ERR_INVALID_ARGUMENTS;

    dmVal.type = DM_PT_BOOL;
    dmVal.len = sizeof(dmVal.v_bool);
    dmVal.v_bool = val;
    return Cdm_SetParam(param, &dmVal, commit);
}

DmErr_t Cdm_SetParamInt(const char *param, int val, int commit)
{
    DmParamVal_t dmVal;

    if (!param)
        return CCSP_ERR_INVALID_ARGUMENTS;

    dmVal.type = DM_PT_INT;
    dmVal.len = sizeof(dmVal.v_int);
    dmVal.v_int = val;
    return Cdm_SetParam(param, &dmVal, commit);
}

DmErr_t Cdm_SetParamString(const char *param, const char *val, int commit)
{
    DmErr_t err = CCSP_ERR_INTERNAL_ERROR;
    DmParamVal_t dmVal;

    if (!param || !val)
        return CCSP_ERR_INVALID_ARGUMENTS;

    dmVal.type = DM_PT_STR;
    dmVal.len = strlen(val);
    dmVal.v_str = (void *)AnscAllocateMemory(dmVal.len + 1);
    if (dmVal.v_str == NULL)
        return CCSP_ERR_MEMORY_ALLOC_FAIL;

    snprintf(dmVal.v_str, dmVal.len + 1, "%s", val);
    err = Cdm_SetParam(param, &dmVal, commit);

    AnscFreeMemory(dmVal.v_str);
    return err;
}

DmErr_t Cdm_SetParamUlong(const char *param, unsigned long val, int commit)
{
    DmParamVal_t dmVal;

    if (!param)
        return CCSP_ERR_INVALID_ARGUMENTS;

    dmVal.type = DM_PT_ULONG;
    dmVal.len = sizeof(dmVal.v_ulong);
    dmVal.v_ulong = val;
    return Cdm_SetParam(param, &dmVal, commit);
}

DmErr_t Cdm_SetParamUint(const char *param, unsigned int val, int commit)
{
    DmParamVal_t dmVal;

    if (!param)
        return CCSP_ERR_INVALID_ARGUMENTS;

    dmVal.type = DM_PT_UINT;
    dmVal.len = sizeof(dmVal.v_uint);
    dmVal.v_uint = val;
    return Cdm_SetParam(param, &dmVal, commit);
}

DmErr_t Cdm_GetParam(const char *param, DmParamVal_t **val)
{
    const char *paths[1] = {NULL};
    DmParam_t *params = NULL;
    int cnt =0;
    DmErr_t err = {0};

    if (!param || !val || !IsPathParam(param))
        return CCSP_ERR_INVALID_ARGUMENTS;

    paths[0] = param;
    err = Cdm_GetParamGrp(paths, 1, &params, &cnt);
    if (err != CCSP_SUCCESS)
        goto errout;

    /* if more than one param got which one should we return ? */
    if (cnt != 1) {
		err = CCSP_ERR_INTERNAL_ERROR;
		goto errout;
    }

    *val = CloneParamVal(params[0].value);
    if (*val == NULL) {
		err = CCSP_ERR_MEMORY_ALLOC_FAIL;
		goto errout;
    }
	err = CCSP_SUCCESS;

errout:
    Cdm_FreeParamGrp(params, cnt);
    return err;

}

DmErr_t Cdm_SetParam(const char *param, const DmParamVal_t *val, int commit)
{
    DmParam_t dmParam;
    DmErr_t err;

    if (!param || !val || !IsPathParam(param))
        return CCSP_ERR_INVALID_ARGUMENTS;

    snprintf(dmParam.name, sizeof(dmParam.name), "%s", param);

    dmParam.value = CloneParamVal(val);
    if (dmParam.value == NULL)
        return CCSP_ERR_MEMORY_ALLOC_FAIL;

    err =  Cdm_SetParamGrp(&dmParam, 1, commit);

    Cdm_FreeParam(dmParam.value);
    return err;
}

static void FreeParam(DmParamVal_t *val, int raw)
{
    void *ptr = NULL;

    if (val == NULL)
        return;

    if (raw)
        ptr = val->v_raw;
    else {
        switch (val->type) {
        case DM_PT_STR:
            ptr = val->v_str;
            break;
        case DM_PT_BASE64:
            ptr = val->v_base64;
            break;
        case DM_PT_DATETIME:
            ptr = val->v_datetime;
            break;
        case DM_PT_BIN:
            ptr = val->v_bin;
            break;
        default:
            break;
        }
    }

    if (ptr)
        AnscFreeMemory(ptr);
    AnscFreeMemory(val);
}

void Cdm_FreeParam(DmParamVal_t *val)
{
    FreeParam(val, 0);
}

DmErr_t Cdm_AddObj(const char *obj, int *ins)
{
    if (!obj || !ins || !IsPathObj(obj))
        return CCSP_ERR_INVALID_ARGUMENTS;

    return AddDelObj(obj, ins, 1);
}

DmErr_t Cdm_DelObj(const char *obj, int ins)
{
    char path[257];

    if (!obj || (!IsPathObj(obj) && !IsPathIns(obj)))
        return CCSP_ERR_INVALID_ARGUMENTS;

    if (IsPathObj(obj)) {
        if (ins <= 0)
            return CCSP_ERR_INVALID_ARGUMENTS;
        snprintf(path, sizeof(path), "%s%d.", obj, ins);
        obj = path;
    }

    return AddDelObj(obj, NULL, 0);
}

DmErr_t Cdm_GetInsNums(const char *obj, int *insList[], int *cnt)
{
    componentStruct_t       **compStru = NULL;
    int                     compNum = 0;
    DmErr_t                 err = CCSP_SUCCESS;

    if (!obj || !insList || !cnt || !IsPathObj(obj))
        return CCSP_ERR_INVALID_ARGUMENTS;

    err = CcspBaseIf_discComponentSupportingNamespace(cdm.busHdl, 
            cdm.crId, obj, cdm.subSys, &compStru, &compNum);
    if (err != CCSP_SUCCESS)
        return err;

    /* object has mutli instance should not separated */
    if (compNum != 1) {
        free_componentStruct_t(cdm.busHdl, compNum, compStru);
        return CCSP_ERR_INTERNAL_ERROR;
    }

#ifdef __CDM_LOCAL_ACCESS
    if (strcmp(compStru[0]->componentName, cdm.compId) == 0) {
        // TODO: 
        free_componentStruct_t(cdm.busHdl, compNum, compStru);
        return CCSP_ERR_NOT_SUPPORT;
    }
#endif

    err = CcspBaseIf_GetNextLevelInstances(cdm.busHdl, 
            compStru[0]->componentName, compStru[0]->dbusPath, 
            (char *)obj, (unsigned int *)cnt, (unsigned int **)insList);

    free_componentStruct_t(cdm.busHdl, compNum, compStru);
    return err;
}

void Cdm_FreeInsNums(int insList[])
{
    if (insList)
        AnscFreeMemory(insList);
}

DmErr_t Cdm_GetNames(const char *path, int recursive,
                    char (**names)[CDM_PATH_SZ], int *cnt)
{
    componentStruct_t       **compStru = NULL;
    int                     compNum = 0;
    int                     compIdx = 0, infoIdx = 0;
    parameterInfoStruct_t   **infoStru = NULL;
    int                     infoNum = 0;
    int                     offset = 0;
    char                    (*list)[CDM_PATH_SZ];
    void                    *ptr = NULL;
    DmErr_t                 err = 0;

    if (!path || !names || !cnt)
        return CCSP_ERR_INVALID_ARGUMENTS;

    err = CcspBaseIf_discComponentSupportingNamespace(cdm.busHdl, cdm.crId, 
                path, cdm.subSys, &compStru, &compNum);
    if (err != CCSP_SUCCESS)
        return err;

    *cnt = offset = 0;
    list = NULL;

    /* handle all components found */
    for (compIdx = 0; compIdx < compNum; compIdx++) {
#ifdef __CDM_LOCAL_ACCESS
        if (strcmp(compStru[compIdx]->componentName, cdm.compId) == 0) {
            err = CcspCcMbi_GetParameterNames((char *)path, !recursive, 
                    &infoNum, &infoStru, NULL);
        } else {
#endif
            err = CcspBaseIf_getParameterNames(cdm.busHdl, 
                        compStru[compIdx]->componentName, 
                        compStru[compIdx]->dbusPath,
                        (char *)path, !recursive, &infoNum, &infoStru);
#ifdef __CDM_LOCAL_ACCESS
        }
#endif
        if (err != CCSP_SUCCESS) {
            free_componentStruct_t(cdm.busHdl, compNum, compStru);
            if (list)/*RDKB-6129, CID-33493  Free list(ptr) incase of error*/
                AnscFreeMemory(list);
            return err;
        }

        *cnt += infoNum;
        ptr = AnscReallocMemory(list, ((*cnt)-infoNum) * CDM_PATH_SZ, (*cnt) * CDM_PATH_SZ);
        if (ptr == NULL) {
            err = CCSP_ERR_MEMORY_ALLOC_FAIL;
            free_parameterInfoStruct_t(cdm.busHdl, infoNum, infoStru);
            free_componentStruct_t(cdm.busHdl, compNum, compStru);
            if (list)
                AnscFreeMemory(list);
            return err;
        }
        list = ptr;

        for (infoIdx = 0; infoIdx < infoNum; infoIdx++) {
            if (offset >= *cnt) { /* should not happen */
                free_parameterInfoStruct_t(cdm.busHdl, infoNum, infoStru);
                goto exit; /* not error */
            }

            snprintf(list[offset], CDM_PATH_SZ, "%s", infoStru[infoIdx]->parameterName);
            offset++;
        }

        free_parameterInfoStruct_t(cdm.busHdl, infoNum, infoStru);
    }

exit:
    *names = list;
    *cnt = offset; /* redundant */
    free_componentStruct_t(cdm.busHdl, compNum, compStru);

    return CCSP_SUCCESS;
}

void Cdm_FreeNames(char (*names)[CDM_PATH_SZ])
{
    if (names)
        AnscFreeMemory(names);
}

static DmErr_t GetParamGrp(const char *paths[], int npath, DmParam_t *params[], int *cnt, int raw)
{
    DmErr_t         err = {0};
    int             compCnt, i, j;
    struct CdmComp  *compList = NULL;
    int             valNum = 0;
    parameterValStruct_t **valStru = NULL;
    int             paramCnt = 0;
    DmParam_t       *paramBuf = NULL;
    void            *ptr = NULL;

    if (!paths || !params || !cnt)
        return CCSP_ERR_INVALID_ARGUMENTS;

    /* classify these paths to Compoent->Param */
    err = LoadCompPathList(paths, npath, &compList, &compCnt);
    if (err != CCSP_SUCCESS)
        return err;

    for (i = 0; i < compCnt; i++) {
#ifdef __CDM_LOCAL_ACCESS
        if (compList[i].isLocal) {
            err = CcspCcMbi_GetParameterValues(
                    CompIdGetWriteId(cdm.compId),
                    compList[i].paths, compList[i].paramCnt,
                    &valNum, &valStru, NULL);
        } else {
#endif
            err = CcspBaseIf_getParameterValues(cdm.busHdl,
                    compList[i].compId, compList[i].dbusPath,
                    compList[i].paths, compList[i].paramCnt,
                    &valNum, &valStru);
#ifdef __CDM_LOCAL_ACCESS
        }
#endif

        if (err != CCSP_SUCCESS) {
            valStru = NULL, valNum = 0;
            goto errout;
        }

        ptr = AnscReallocMemory(paramBuf, sizeof(DmParam_t) * paramCnt, sizeof(DmParam_t) * (paramCnt + valNum));
        if (ptr == NULL) {
            err = CCSP_ERR_MEMORY_ALLOC_FAIL;
            goto errout;
        }
        paramBuf = ptr;

        for (j = 0; j < valNum; j++) {
            err = TransParamStru(valStru[j], &paramBuf[paramCnt], raw);
            if (err != CCSP_SUCCESS)
                goto errout;

            paramCnt++;
        }

        free_parameterValStruct_t(cdm.busHdl, valNum, valStru);
        valStru = NULL, valNum = 0;
    }

    FreeCompList(compList, paramCnt);
    *params = paramBuf;
    *cnt = paramCnt;
    return CCSP_SUCCESS;

errout:
    if (valStru)
        free_parameterValStruct_t(cdm.busHdl, valNum, valStru);
    if (paramBuf)
        AnscFreeMemory(paramBuf);
    FreeCompList(compList, paramCnt);
    return err;
}

DmErr_t Cdm_GetParamGrp(const char *paths[], int npath, DmParam_t *params[], int *cnt)
{
    return GetParamGrp(paths, npath, params, cnt, 0);
}

DmErr_t Cdm_GetParamGrpRaw(const char *paths[], int npath, DmParam_t *params[], int *cnt)
{
    return GetParamGrp(paths, npath, params, cnt, 1);
}

void Cdm_FreeParamGrp(DmParam_t params[], int cnt)
{
    int i;

    if (!params || cnt <= 0)
        return;

    for (i = 0; i < cnt; i++)
    {
		switch(params[i].value->type)
		{
			case DM_PT_STR:
					if(params[i].value->v_str)
						AnscFreeMemory(params[i].value->v_str);
					break;

			case DM_PT_DATETIME:
					if(params[i].value->v_datetime)
						AnscFreeMemory(params[i].value->v_datetime);
					break;

			case DM_PT_BASE64:
					if(params[i].value->v_base64)
						AnscFreeMemory(params[i].value->v_base64);
					break;

			case DM_PT_BIN:
					if(params[i].value->v_bin)
						AnscFreeMemory(params[i].value->v_bin);
					break;

			default:
					break;
		}

		if(params[i].value)
			AnscFreeMemory(params[i].value);
    }
    AnscFreeMemory(params);
}

void Cdm_FreeParamGrpRaw(DmParam_t params[], int cnt)
{
    int i;

    if (!params || cnt <= 0)
        return;

    for (i = 0; i < cnt; i++)
        FreeParam(params[i].value, 1);
    AnscFreeMemory(params);
}

/* 
 * We have same issue with objects/params group get, 
 * 1. it's no enough just spliting them to local and remote.
 * 2. we need know which component they belongs to.
 *
 * More issue than "group get",
 * if we set param one-by-one to avoid the classifying, 
 * then "commit" will be a big problem since param from same 
 * component may random distributed and param from 
 * different component may interleaved.
 * Note commit is "component level"
 *
 * At a result, we have to classify param for not only local/remote 
 * but also different remote components.
 *
 * 1. local params
 * 2. remote-comp-A's params
 * 3. remote-comp-B's Params
 * 4. ...
 */
static DmErr_t SetParamGrp(DmParam_t params[] /* in-out */, int cnt, int commit, int raw)
{
    struct CdmComp *compList = NULL;
    int compCnt=0, i=0;
    DmErr_t err = 0;
    char *faultParam = NULL; /*RDKB-6129, CID-33194; initializing the variable*/

    if (!params || cnt <= 0)
        return CCSP_ERR_INVALID_ARGUMENTS;

    /* 
     * classifying params for different components 
     * and also marking if it's local.
     * the wrost case is each param holded by different comp.
     */
    err = LoadCompParamList(params, cnt, &compList, &compCnt, raw);
    if (err != CCSP_SUCCESS)
        return err;

    if (compCnt == 0) {
        FreeCompList(compList, compCnt); /* still need to free */
        return CCSP_ERR_NOT_EXIST;
    }

    for (i = 0; i < compCnt; i++) {
#ifdef __CDM_LOCAL_ACCESS
        if (compList[i].isLocal) {
            err = CcspCcMbi_SetParameterValues(
                    0, CompIdGetWriteId(cdm.compId),
                    compList[i].paramVals, compList[i].paramCnt,
                    0 /* not commit */, &faultParam, NULL);
        } else {
#endif
            err = CcspBaseIf_setParameterValues(cdm.busHdl, 
                    compList[i].compId, compList[i].dbusPath,
                    0, CompIdGetWriteId(cdm.compId), /* session-id, write-id */
                    compList[i].paramVals, compList[i].paramCnt, 
                    0 /* not commit now */, &faultParam);
#ifdef __CDM_LOCAL_ACCESS
        }
#endif
        if (err != CCSP_SUCCESS) {
            if (faultParam) {
                snprintf(params[0].name, sizeof(params[0].name), 
                        "!%s", faultParam);
                AnscFreeMemory(faultParam);
            }
            goto rollback;
        }
    }

    /* commit is component level, according to 
     * CCSP design, commit always success. - -b
     * so if some comp commit fail we do not rollback */
    if (commit) {
        for (i = 0; i < compCnt; i++) {
#ifdef __CDM_LOCAL_ACCESS
            if (compList[i].isLocal) {
                err = CcspCcMbi_SetCommit(0, CompIdGetWriteId(cdm.compId), 1, NULL);
            } else {
#endif
                err = CcspBaseIf_setCommit(cdm.busHdl, 
                        compList[i].compId, compList[i].dbusPath, 
                        0, CompIdGetWriteId(cdm.compId), 1);
#ifdef __CDM_LOCAL_ACCESS
            }
#endif
            if (err != CCSP_SUCCESS)
                goto errout;
        }
    }

    FreeCompList(compList, compCnt);
    return CCSP_SUCCESS;

rollback:
    for (; i >= 0; i--) {
#ifdef __CDM_LOCAL_ACCESS
        if (compList[i].isLocal) {
            CcspCcMbi_SetCommit(0, CompIdGetWriteId(cdm.compId), 0, NULL);
        } else {
#endif
            CcspBaseIf_setCommit(cdm.busHdl, 
                    compList[i].compId, compList[i].dbusPath, 
                    0, CompIdGetWriteId(cdm.compId), 0);
#ifdef __CDM_LOCAL_ACCESS
        }
#endif
    }

errout:
    FreeCompList(compList, compCnt);
    return err;
}

DmErr_t Cdm_SetParamGrp(DmParam_t params[] /* in-out */, int cnt, int commit)
{
    return SetParamGrp(params, cnt, commit, 0);
}

DmErr_t Cdm_SetParamGrpRaw(DmParam_t params[] /* in-out */, int cnt, int commit)
{
    return SetParamGrp(params, cnt, commit, 1);
}

const char *Cdm_StrError(DmErr_t err)
{
    static struct {
        const char *desc;
        DmErr_t err;
    } errTbl[] = {
        { "Success",            CCSP_SUCCESS, },
        { "Internal Error",     CCSP_ERR_INTERNAL_ERROR, },
        { "Invalid Arguments",  CCSP_ERR_INVALID_ARGUMENTS, },
        { "Invalid Type",       CCSP_ERR_INVALID_PARAMETER_TYPE, },
        { "No Memory",          CCSP_ERR_MEMORY_ALLOC_FAIL, },
        { "Not Support",        CCSP_ERR_NOT_SUPPORT, },
        { "Too Big",            CCSP_ERR_RESOURCE_EXCEEDED, },
        { "Not Exist",          CCSP_ERR_NOT_EXIST, },
        { NULL, },
    };
    int i;

    for (i = 0; errTbl[i].desc != NULL; i++) {
        if (err ==  errTbl[i].err)
            return errTbl[i].desc;
    }

    return "Other Error";
}

DmErr_t Cdm_GetParamAsStr(const char *param, char *val, size_t size)
{
    // TODO:
    UNREFERENCED_PARAMETER(param);
    UNREFERENCED_PARAMETER(val);
    UNREFERENCED_PARAMETER(size);
    return CCSP_ERR_NOT_SUPPORT;
}

DmErr_t Cdm_SetParamAsStr(const char *param, const char *val, int commit)
{
    // TODO:
    UNREFERENCED_PARAMETER(param);
    UNREFERENCED_PARAMETER(val);
    UNREFERENCED_PARAMETER(commit);
    return CCSP_ERR_NOT_SUPPORT;
}
