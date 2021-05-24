/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2016 RDK Management
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

#include <stdio.h>
#include <string.h>
#include <stddef.h>

#ifndef SAFEC_DUMMY_API
#include "safe_str_lib.h"
#include "safe_mem_lib.h"

/* Macro is defined for non clobbering of the safec secure string API strcpy_s & memcpy_s function*/
/* strcpy_s overwrites the old value and nulls the dest when encounters an error*/
#ifndef STRCPY_S_NOCLOBBER
 #define STRCPY_S_NOCLOBBER(dst,dmax,src)   ((src != NULL) ? (strlen(src) < dmax ?  strcpy_s(dst,dmax,src) : ESNOSPC):ESNULLP)
#endif
#define MEMCPY_S_NOCLOBBER(dst,dmax,src,len)   ((src != NULL) ? (len <= dmax ?  memcpy_s(dst,dmax,src,len) : ESNOSPC):ESNULLP)
#endif

/*
 * SAFECLIB Error Handling Logging APIs
 */
#define RDK_SAFECLIB_ERR(rc)  printf("safeclib error at rc - %d %s %s:%d", rc,  __FILE__, __FUNCTION__, __LINE__)

#define ERR_CHK(rc)                                             \
    if(rc !=EOK) {                                              \
        RDK_SAFECLIB_ERR(rc);                                   \
    }

#ifdef SAFEC_DUMMY_API
#include <stdarg.h>
typedef int errno_t;
#define EOK 0
#define ESNULLP          400        /* null ptr                    */
#define ESLEMAX          403       /* length exceeds RSIZE_MAX    */
#define ESNOSPC          406       /* not enough space for dest     */

#define strcpy_s(dst,max,src) (src != NULL)?((max > strlen(src))?EOK:ESLEMAX):ESNULLP; \
 if((src != NULL) && (max > strlen(src))) strcpy(dst,src);

#define strncpy_s(dst,max,src,len) (src != NULL)?((len <= max)?EOK:ESLEMAX):ESNULLP; \
 if((src != NULL) && (len <= max)) strncpy(dst,src,len);

#define memset_s(dst,max_1,c,max) EOK; \
 memset(dst,c,max);

#define strcat_s(dst,max,src) (src != NULL)?((max > strlen(src))?EOK:ESLEMAX):ESNULLP; \
 if((src != NULL) && (max > strlen(src))) strcat(dst,src);

#define strncat_s(dst,max,src,len) (src != NULL)?((len <= max)?EOK:ESLEMAX):ESNULLP; \
 if((src != NULL) && (len <= max)) strncat(dst,src,len);

#define memcpy_s(dst,max,src,len)  (src != NULL)?((len <= max)?EOK:ESLEMAX):ESNULLP; \
 if((src != NULL) && (len <= max)) memcpy(dst,src,len);

#ifndef STRCPY_S_NOCLOBBER
 #define STRCPY_S_NOCLOBBER(dst,max,src) (src != NULL)?((max > strlen(src))?EOK:ESLEMAX):ESNULLP; \
  if((src != NULL) && (strlen(src) < max)) strcpy(dst, src);
#endif

#define MEMCPY_S_NOCLOBBER(dst,max,src,len) (src != NULL) ? ((len <= max)?EOK:ESLEMAX):ESNULLP; \
  if((src != NULL) && (len <= max)) memcpy(dst, src, len);

#define sscanf_s( buffer, fmt, ...)  EOK; \
  sscanf( buffer, fmt, ##__VA_ARGS__ );

#define strtok_s(dest, dmax, delim, ptr) strtok_r(dest, delim, ptr)

#define sprintf_s( dst, max, fmt, ... ) parseFormat(dst, max, fmt, ##__VA_ARGS__)

static inline int parseFormat(const char *dst, int max, const char *fmt, ...)
{
    va_list argp;
    int len = 0;

    if((fmt == NULL) || (dst == NULL) || (max == 0))
    {
        return -ESNULLP;
    }

    va_start(argp, fmt);

    len = vsnprintf((char *)dst, (size_t)max, fmt, argp);

    va_end(argp);

    return (max > len) ? len : -ESNOSPC;
}

static inline int strcmp_s(const char *dst, int dmax, const char *src, int *r) {
        if((src ==  NULL) || (dst == NULL) || (dmax == 0))
            return ESNULLP;

        *r = strcmp(dst, src);
        return EOK;
}

static inline int strcasecmp_s(const char *dst, int dmax, const char *src, int *r) {
         if((src ==  NULL) || (dst == NULL) || (dmax == 0))
            return ESNULLP;

         *r = strcasecmp(dst, src);
         return EOK;
}
#endif
