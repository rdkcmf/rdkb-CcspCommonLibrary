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
 */
#ifndef _CDM_API_H
#define _CDM_API_H
#include <ctype.h>
#include <stdbool.h>
#include "ccsp_base_api.h"

#define CDM_PATH_SZ     257 /* max CWMP path + 1 */

/* compatible with CCSP_SUCCESS/... in ccsp_base_api.h */
typedef int DmErr_t;

/* complex data type */
typedef enum DmParmType {
    DM_PT_STR,
    DM_PT_INT,
    DM_PT_UINT,
    DM_PT_BOOL,
    DM_PT_DATETIME,
    DM_PT_BASE64,
    DM_PT_LONG,
    DM_PT_ULONG,
    DM_PT_FLOAT,
    DM_PT_DOUBLE,
    DM_PT_BIN, /* opaque */
} DmParamType_t;

typedef struct DmParamVal {
    DmParamType_t       type;
    size_t              len;
    union {
        char            *v_str;
        int             v_int;
        unsigned        v_uint;
        bool            v_bool;
        float           v_float;
        double          v_double;
        long            v_long;
        unsigned long   v_ulong;
        char            *v_datetime; /* UTC */
        char            *v_base64;
        unsigned char   *v_bin;

        char            *v_raw;
    };
} DmParamVal_t;

typedef struct DmParam {
    /* Why we use array instead of point ?
     * it may used to save fault param when set */
    char                name[CDM_PATH_SZ];
    DmParamVal_t        *value;
} DmParam_t;

/**
 * Cdm_Init - initialize DM access API, call it once.
 * Params:
 *   @busHdl,   Dbus Handler, set to NULL to create internall.,
 *   @subSys,   subsystem, NULL for default, e.g, "eRT",
 *   @cnfFil,   config file, useful only when @busHdl is NULL.
 *   @crId,     CR ID, NULL for default, e.g: "com.cisco.spvtg.ccsp.CR"
 *   @compId,   MUST be set, used to identify the user.
 *
 * Note:
 *   This function could be used to collect the info needed, but
 *   if @busHdl is NULL, it will be initialized internally with 
 *   @cnfFil, @subSys, @crId and @compId or thire default values.
 */
DmErr_t Cdm_Init(const void *busHdl, const char *subSys, const char *cnfFil,
                const char *crId, const char *compId);
DmErr_t Cdm_Term(void);
const char *Cdm_StrError(DmErr_t err);

/**
 * These APIs can be used to get/set value in a simple way.
 */
DmErr_t Cdm_GetParamBool(const char *param, bool *val);
DmErr_t Cdm_GetParamInt(const char *param, int *val);
DmErr_t Cdm_GetParamString(const char *param, char *val, size_t size); /* nil tailed str */
DmErr_t Cdm_GetParamUlong(const char *param, unsigned long *val);
DmErr_t Cdm_GetParamUint(const char *param, unsigned int *val);

DmErr_t Cdm_SetParamBool(const char *param, bool val, int commit);
DmErr_t Cdm_SetParamInt(const char *param, int val, int commit);
DmErr_t Cdm_SetParamString(const char *param, const char *val, int commit); /* nil tailed str */
DmErr_t Cdm_SetParamUlong(const char *param, unsigned long val, int commit);
DmErr_t Cdm_SetParamUint(const char *param, unsigned int val, int commit);

/* in case the caller don't know the type of a param.
 * all param will be handled as ASCII (nil terminated) string.
 * these two funcion is more slower, since it takes time to 
 * determine the type internally. */
DmErr_t Cdm_GetParamAsStr(const char *param, char *val, size_t size);
DmErr_t Cdm_SetParamAsStr(const char *param, const char *val, int commit);

/**
 * Fully functional APIs.
 */

/* it depends on caller to free @val by Cdm_FreeParam(), if return success */
DmErr_t Cdm_GetParam(const char *param, DmParamVal_t **val);
DmErr_t Cdm_SetParam(const char *param, const DmParamVal_t *val, int commit);
void Cdm_FreeParam(DmParamVal_t *val);

/* object operations */
DmErr_t Cdm_AddObj(const char *obj, int *ins);
DmErr_t Cdm_DelObj(const char *obj, int ins); /* if @ins is zero, @obj contains ins num */

/* it depends on caller to free @insList by Cdm_FreeInsNums(), if return success */
DmErr_t Cdm_GetInsNums(const char *obj, int *insList[], int *cnt);
void Cdm_FreeInsNums(int insList[]);

/* it depends on caller to free @names by Cdm_FreeNames(), if return success */
DmErr_t Cdm_GetNames(const char *path, int recursive,
                    char (**names)[CDM_PATH_SZ], int *cnt);
void Cdm_FreeNames(char (*names)[CDM_PATH_SZ]);

/**
 * Group get - get multiple params or params of objects at the same time.
 * Parameters:
 *   @paths [in], could be complete path name (param) or object name.
 *   @npath [in], number of paths.
 *   @params [out], parameters got.
 *   @cnt [out], number of parameterss got.
 * Note:
 *   it depends on caller to free @params by Cdm_FreeParamGrp(), if success.
 */
DmErr_t Cdm_GetParamGrp(const char *paths[], int npath, DmParam_t *params[], int *cnt);
void Cdm_FreeParamGrp(DmParam_t params[], int cnt);

/* 
 * @params [in-out], when return error, first invalid 
 * param will be saved in @param[0], except @cnt <= zero.
 */
DmErr_t Cdm_SetParamGrp(DmParam_t params[] /* in-out */, int cnt, int commit);

/** 
 * for those user handles all param as raw format (ascii string)
 */

/*
 * same with Cdm_GetParamGrp() except that 
 * 1. the format of param[] got will be Raw Ascii String. 
 *    Pls check the DmParam_t->Value->v_raw for value
 * 2. If return success, param[] should be free by Cdm_FreeParamGrpRaw();
 */
DmErr_t Cdm_GetParamGrpRaw(const char *paths[], int npath, DmParam_t *params[], int *cnt);
void Cdm_FreeParamGrpRaw(DmParam_t params[], int cnt);

/*
 * same with Cdm_SetParamGrp() except that the format of param[] to set
 * are Raw Ascii String (DmParam_t->Value->v_raw).
 */
DmErr_t Cdm_SetParamGrpRaw(DmParam_t params[] /* in-out */, int cnt, int commit);

#endif /* _CDM_API_H */
