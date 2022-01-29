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

    module:	kernel_file_io.h

        For Advanced Networking Service Container (ANSC),
        BroadWay Service Delivery System

    ---------------------------------------------------------------

    description:

        This wrapper file defines some basic data types and
        structures on a particular platform.

    ---------------------------------------------------------------

    environment:

        Linux kernel

    ---------------------------------------------------------------

    author:

        Xuechen Yang
        Geng Yang
        Ying Lou

    ---------------------------------------------------------------

    revision:

        01/19/01    initial revision.
        09/14/01    adapted to linux kernel by Geng Yang
        04/03/02    adapted to linux user by Ying Lou

**********************************************************************/

#include <string.h>
#include <stdlib.h>
#include "ansc_global.h"

#include "user_base.h"
#include "user_file_io.h"

extern inline int
user_rename_file(char* old_file_name, char* new_file_name);

extern inline int
user_create_directory(char* dir_name);

extern inline  int
user_get_file_stat(char* file_name, void* h_file_property);

extern inline int
user_move_file(char* srcFile, char* dstFile);

extern inline int
user_copy_directory(char* srcDir, char* dstDir);

extern inline int
user_delete_directory(char* dir);

extern inline int
user_delete_file(char *filename);

extern inline int
user_get_file_size(PVOID h);

extern inline  int
user_find_first_file
    (
        char*                       dir_name,
        char*                       tar_file_name,
        void**                      ph_find_context,
        char*                       pb_directory,    /* 0 file, !0 dir */
        char*                       first_file_name
    );

extern inline  int
user_find_next_file
    (
        void*                       h_find_context,
        char*                       pb_directory,
        char*                       next_file_name
    );

extern inline  void
user_find_close
    (
        void*                      h_find_context
    );

/*
 S ! " #  $ % & '  ( ) * +  , - . /  0 1 2 3  4 5 6 7  8 9 : ;  < = > ?
 1 1 1 0  0 0 1 1  1 1 1 0  0 0 0 0                    0 0 0 1  1 1 1 1
 @ A B C  D E F G  H I J K  L M N O  P Q R S  T U V W  X Y Z [  \ ] ^ _
                                                                1 0 0 0
 ` a b c  d e f g  h i j k  l m n o  p q r s  t u v w  x y z {  | } ~ DEL
 1 0 0 0                                                        1 0 0 0
*/
static const unsigned int esc_mask[8] = {
   0, 0xE3E0001B, 0x08, 0x80000008,
   0, 0, 0, 0
};

static
int
escape_char
    (
        const char *                filename,
        char *                      buf
    )
{
    int                             n = 0;
    const unsigned char *           p;

    for( p = (const unsigned char *)filename; *p; p ++ )
    {
         if( esc_mask[*p >> 5] & (((unsigned int)0x80000000) >> (*p & 31)) )
         {
             if( buf )
                 buf[n] = '\\';
             n ++;
         }
         if( buf )
             buf[n] = *p;
         n ++;
    }
    if( buf )
        buf[n] = ' ';
    n ++;

    return n;
}


typedef struct COMMOND_ARG{
    const char *        arg;
    unsigned long       flags;
}
COMMOND_ARG, *PCOMMOND_ARG;

/*
 * make command
 */
static
char *
make_command
    (
        int                         arg_count,
        COMMOND_ARG                 args[]
    )
{
    int                             i;
    size_t                          len;
    char                            *buffer, *p;

    for( i = 0, len = 0; i < arg_count; i ++ )
    {
        len += args[i].flags ? (size_t)escape_char(args[i].arg, NULL) : (size_t)(strlen(args[i].arg) + 1);
# if 0
        len += strlen(args[i].arg)+ 1;
        if( args[i].flags )
        {
            /* doule quote for file name */
            len += 2;
        }
#endif
    }

    if( ! (buffer = (char*)AnscAllocateMemory(++len)) )
    {
        return NULL;
    }

    for( i = 0, p = buffer; i < arg_count; i ++ )
    {
        int len_printed;

        /*len_printed = snprintf(p, len, args[i].flags ? "\"%s\" " : "%s ", args[i].arg);
        */
        len_printed = args[i].flags ?
            escape_char(args[i].arg, p) : snprintf(p, len, "%s ", args[i].arg);
        if( len_printed < 0 || len_printed == (int)len )
        {
            /* this should never happen */
            AnscFreeMemory(buffer);
            return NULL;
        }

        p += len_printed;
        len -= len_printed;
    }

    *p = '\0';

    return buffer;
}


static
inline
int
exec_command
    (
        int                         arg_count,
        COMMOND_ARG                 args[]
    )
{
    char *                          cmd;
    int                             ret;

    cmd = make_command(arg_count, args);
    if( ! cmd )
    {
        return 0;
    }
    /* printf("$$$$$$$$$$$$$$$$$$$\n Exec command <%s>\n$$$$$$$$$$$$$$$$\n", cmd); */
    ret = system(cmd);

    AnscFreeMemory((void*)cmd);

    return !ret;  /* need 1 as sucess */

}


inline int
user_rename_file
    (
        char*                       old_file_name,
        char*                       new_file_name
    )
{
    return  !rename(old_file_name, new_file_name); /* changed since 0 is success */
}

inline int
user_create_directory(char* dir_name)
{
    COMMOND_ARG                     args[2] = {{"mkdir", 0}, {NULL, 1}};

    args[1].arg = dir_name;

    return exec_command(2, args);
}

inline  int
user_get_file_stat
    (
        char*                       file_name,
        void*                       h_file_property
    )
{
    PUSER_FILE_PROPERTY             pFileProperty = (PUSER_FILE_PROPERTY)h_file_property;
    struct stat                     fst;
    int                             ret;
    struct tm*                      tm;

    ret = stat(file_name, &fst);

    if ( ret == 0 )
    {
        tm = localtime(&fst.st_atime);
        pFileProperty->AccessTime.Year                  = (USHORT)(tm->tm_year + 1900);
        pFileProperty->AccessTime.Month                 = (USHORT)(tm->tm_mon + 1);
        pFileProperty->AccessTime.DayOfMonth            = (USHORT)tm->tm_mday;
        pFileProperty->AccessTime.Hour                  = (USHORT)tm->tm_hour;
        pFileProperty->AccessTime.Minute                = (USHORT)tm->tm_min;
        pFileProperty->AccessTime.Second                = (USHORT)tm->tm_sec;
        pFileProperty->AccessTime.MilliSecond           = (USHORT)0;
        pFileProperty->AccessTime.DayOfWeek             = (USHORT)tm->tm_wday;
        pFileProperty->AccessTime.bDayLightSaving       = tm->tm_isdst;

        tm = localtime(&fst.st_mtime);
        pFileProperty->ModificationTime.Year            = (USHORT)(tm->tm_year + 1900);
        pFileProperty->ModificationTime.Month           = (USHORT)(tm->tm_mon + 1);
        pFileProperty->ModificationTime.DayOfMonth      = (USHORT)tm->tm_mday;
        pFileProperty->ModificationTime.Hour            = (USHORT)tm->tm_hour;
        pFileProperty->ModificationTime.Minute          = (USHORT)tm->tm_min;
        pFileProperty->ModificationTime.Second          = (USHORT)tm->tm_sec;
        pFileProperty->ModificationTime.MilliSecond     = (USHORT)0;
        pFileProperty->ModificationTime.DayOfWeek       = (USHORT)tm->tm_wday;
        pFileProperty->ModificationTime.bDayLightSaving = tm->tm_isdst;

        tm = localtime(&fst.st_ctime);
        pFileProperty->CreationTime.Year                = (USHORT)(tm->tm_year + 1900);
        pFileProperty->CreationTime.Month               = (USHORT)(tm->tm_mon + 1);
        pFileProperty->CreationTime.DayOfMonth          = (USHORT)tm->tm_mday;
        pFileProperty->CreationTime.Hour                = (USHORT)tm->tm_hour;
        pFileProperty->CreationTime.Minute              = (USHORT)tm->tm_min;
        pFileProperty->CreationTime.Second              = (USHORT)tm->tm_sec;
        pFileProperty->CreationTime.MilliSecond         = (USHORT)0;
        pFileProperty->CreationTime.DayOfWeek           = (USHORT)tm->tm_wday;
        pFileProperty->CreationTime.bDayLightSaving     = tm->tm_isdst;

        pFileProperty->Size         = fst.st_size;
        pFileProperty->Attributes   = 0;

        if ( fst.st_mode & S_IFDIR )
        {
            pFileProperty->Attributes |= USER_FILE_ATTRIBUTE_Directory;
        }

        if ( fst.st_mode & S_IFREG )
        {
            pFileProperty->Attributes |= USER_FILE_ATTRIBUTE_Normal;
        }

        if ( fst.st_mode & S_IRUSR && fst.st_mode & S_IRGRP && fst.st_mode & S_IROTH )
        {
            pFileProperty->Attributes |= USER_FILE_ATTRIBUTE_ReadOnly;
        }
    }

    return  (!ret);
}


inline  int
user_move_file
    (
        char*                       srcFile,
        char*                       dstFile
    )
{
    /*return MoveFile(srcFile, dstFile);*/
    COMMOND_ARG                     args[3] = {{"mv", 0}, {NULL, 1}, {NULL, 1}};

    args[1].arg = srcFile;
    args[2].arg = dstFile;

    return exec_command(3, args);
}


inline  int
user_copy_directory
    (
        char*                       srcDir,
        char*                       dstDir
    )
{
    COMMOND_ARG                     args[4] = {{"cp", 0}, {NULL, 1}, {NULL, 1}, {"-r", 0}};

    args[1].arg = srcDir;
    args[2].arg = dstDir;

    return exec_command(4, args);
}


inline  int
user_delete_directory
    (
        char*                       dir
    )
{
    COMMOND_ARG                     args[3] = {{"rm", 0}, {NULL, 1}, {"-r", 0}};

    args[1].arg = dir;

    return exec_command(3, args);
}

inline int
user_delete_file(char *filename)
{
    if ( ! unlink(filename) )
    {
        return 1;
    }

	return 0;
}

inline int
user_get_file_size(PVOID h)
{
	int length = 0;
#ifdef _64BIT_ARCH_SUPPORT_
	length = lseek((int)(ULONG)h, 0, SEEK_END);
	lseek((int)(ULONG)h, 0, SEEK_SET);
#else
	length = lseek((int)h, 0, SEEK_END);
	lseek((int)h, 0, SEEK_SET);
#endif
	return length;
}

/*
 * Note: return 1 if success, 0 if failure.
 */
int
user_copy_file(char *src, char *dst, int fail_if_exists)
{
    int src_fd, dst_fd;
    unsigned char buf[1024];
    int rsz, wsz;

    src_fd = open(src, O_RDONLY);
    if (src_fd < 0)
    {
        return 0;
    }
    dst_fd = open(dst, O_WRONLY | O_CREAT | O_TRUNC | (fail_if_exists ? O_EXCL : 0), S_IRUSR | S_IWUSR);
    if (dst_fd < 0)
    {
        close(src_fd);
        return 0;
    }

    do
    {
        rsz = read(src_fd, buf, sizeof(buf));
        for(wsz=0; wsz < rsz;)
        {
            int sz;
            sz = write(dst_fd, buf+wsz, rsz - wsz);
            if (sz <= 0)
            {
                goto fail;
            }
            wsz += sz;
        }
    }
    while(rsz > 0);

    if (rsz < 0)
    {
        goto fail;
    }

    close(src_fd);
    close(dst_fd);
    return 1;

fail:
    close(src_fd);
    close(dst_fd);
    unlink(dst);

    return 0;
}

int
check_legal_filename_char
    (
        char                         ch
    )
{
    return (ch != '*' && ch != '?' && ch != '\0');
}

/*------------------------------------------------------------------------------------
 *
 * Same as SIO_MatchFileName:
 *        This function is called to match filename with wildcard char *, ?
 *        This function is not necessary since upper caller has already implemented it
 *
 * return:  Ansc status.
 *
 *-------------------------------------------------------------------------------------*/

int
match_wildcard_file_name
    (
        char*                  pattern,
        char*                  name,
        ULONG                  name_len
    )
{
    char                            legal_string[_LINUX_MAX_FNAME];
    ULONG                           len;
    char*                           max_name = name + name_len;

    /*
     * check parameter:
     */
    if( ! name || name_len <= 0)
        return 0;

    /*
     * let NULL pattern match everything:
     */
    if( ! pattern )
    {
        return 1;
    }

    while( name < max_name && (*pattern || *name) )
    {
        switch ( *pattern )
        {
        case '*':

            if( '\0' == *(++pattern) )
                return 1;

            for( len = 0; check_legal_filename_char(pattern[len]); len ++ )
                ;
            if( len > _LINUX_MAX_FNAME - 1 )
                len = _LINUX_MAX_FNAME - 1;

            memcpy(legal_string, pattern, len);
            legal_string[len] = 0;

            name = strstr(name, legal_string);
            if( ! name )
                return 0; /* not match */

            pattern += len;
            name += len;
            break;

        case '?':
            pattern ++;
            if( ! check_legal_filename_char(*name ++) )
                return 0;
            break;

        default:
            /*
             * check if a legal char
             */
            if( check_legal_filename_char(*pattern) )
            {
                if( *pattern ++ != *name ++ )
                    return 0;
            }
            else
            {
                return 0;
            }
            break;
        }
    }

    return 1;
}

/*-----------------------------------------------------------------------------
 *
 *  description:
 *
 *       This function is called to make a full path name:
 *
 *  argument:
 *
 *  return:     Ansc status.
 *
 *-----------------------------------------------------------------------------*/
int
user_make_path_name
    (
        char *                  dir_name,
        char *                  file_name,
        char **                 path_name,
        unsigned long *         path_len
    )
{
    ULONG                           len, len1, len2;
    char *                          path;

    /*
     * make the full path name of this file:
     */
    len1 = strlen(dir_name);
    len2 = strlen(file_name);
    len = len1 + len2 + 2; /* null terminator, separator */
    if( ! path_len || len > *path_len )
    {
        if( *path_name ) AnscFreeMemory(*path_name);

        *path_name = (char *) AnscAllocateMemory(len);
        if( ! *path_name )
        {
            return 0;
        }
        if( path_len )
            *path_len = len;
    }

    path = *path_name;

    memcpy(path, dir_name, len1);
    /*
     * see if there is a seperator at the end of the path name:
     */
    if( len1 > 0 && ! (path[len1-1] == '\\' || path[len1-1] == '/') )
    {
        path[len1++] = '/';
    }

    memcpy(path + len1, file_name, len2);
    path[len1+len2] = '\0';

    return 1;
}


int
user_find_first_file
    (
        char*                       dir_name,
        char*                       tar_file_name,
        void**                      ph_find_context,
        char*                       pb_directory,    /* 0 file, !0 dir */
        char*                       first_file_name
    )
{
    int                             bResult         = FALSE;
	PFIND_CONTEXT                   p_find_context  = NULL;
	struct dirent *                 d               = NULL;
    char *                          pFirstFileName  = NULL; /* for building a full directory name of the first file */
	struct stat                     fst;

    *pb_directory    = FALSE;
	p_find_context   = (PFIND_CONTEXT)AnscAllocateMemory(sizeof(FIND_CONTEXT));

    if ( !p_find_context)
    {
        goto  EXIT1;
    }

	p_find_context->DirName = strdup(dir_name);
	strncpy(p_find_context->TarFileName, tar_file_name, _LINUX_MAX_FNAME);
	p_find_context->TarFileName[_LINUX_MAX_FNAME-1] = 0;
	p_find_context->pDir = opendir (dir_name);

    if ( p_find_context->pDir == NULL )
    {
        goto  EXIT1;
    }
	/* change to readdir_r later */
	d = readdir (p_find_context->pDir);
	/*RDKB-6186, CID-24120; null check before use, if NULL perform clean exit*/
	if(d != NULL)
	{

		while (d != NULL){
			/* do not need to check match here. Di's caller function finishes this operation */
			/*if(match_wildcard_file_name(tar_file_name, d->d_name, strlen(d->d_name)))*/
			if(1)
			{
				bResult          = TRUE;
				strcpy(first_file_name, d->d_name);
				if( user_make_path_name(dir_name, first_file_name, &pFirstFileName, NULL) == 0)
				{
					goto EXIT1; /* failure */
				}
				bResult = stat (pFirstFileName, &fst);
				/* 0 is successful */
				if(bResult == 0){
					if ( fst.st_mode & S_IFDIR ) *pb_directory = 1;
					else *pb_directory = 0;
				}
				else{
					continue;
				}
				*ph_find_context = (void *) p_find_context;
				bResult = TRUE;
				break;
			}
			d = readdir (p_find_context->pDir);  /* this one is actually not necessary */
		}

		goto EXIT2;
	}

EXIT1:
	if (p_find_context)
	{
		if(p_find_context->DirName) AnscFreeMemory(p_find_context->DirName);
	    closedir(p_find_context->pDir);
		AnscFreeMemory(p_find_context);
		*ph_find_context = NULL;
	}
EXIT2:
	if(pFirstFileName)
	{
		AnscFreeMemory(pFirstFileName);
	}
    return  bResult;
}


int
user_find_next_file
    (
        void*                       h_find_context,
        char*                       pb_directory,
        char*                       next_file_name
    )
{
    int                             bResult         = FALSE;
	PFIND_CONTEXT                   p_find_context  = (PFIND_CONTEXT) h_find_context;
	struct dirent *                 d               = NULL;
    char *                          pFirstFileName  = NULL;
	struct stat                     fst;

    *pb_directory    = FALSE;

    if ( !p_find_context)
    {
        goto  EXIT1;
    }

    if ( p_find_context->pDir == NULL )
    {
        goto  EXIT1;
    }

	/* change to readdir_r later */
	d = readdir (p_find_context->pDir);
	while (d != NULL){
		/* do not need to check match here. Di's caller function finishes this operation */
		/*if(match_wildcard_file_name(tar_file_name, d->d_name, strlen(d->d_name)))*/
		if(1)
		{
			bResult          = TRUE;
			strcpy(next_file_name, d->d_name);
			if( user_make_path_name(p_find_context->DirName, next_file_name, &pFirstFileName, NULL) == 0)
			{
				goto EXIT1; /* failure */
			}
			bResult = stat (pFirstFileName, &fst);
			/* 0 is successful */
			if(bResult == 0){
				if ( fst.st_mode & S_IFDIR ) *pb_directory = 1;
				else *pb_directory = 0;
			}
			else{
				d = readdir (p_find_context->pDir);
				continue;
			}
			bResult = TRUE;
			break;
		}
		d = readdir (p_find_context->pDir); /* this one is actually not necessary */
	}

EXIT1:

	if ( pFirstFileName )
	{
        AnscFreeMemory(pFirstFileName);
	}

    return  bResult;
}


inline  void
user_find_close
    (
        void*                      h_find_context
    )
{
	PFIND_CONTEXT   p_find_context  = (PFIND_CONTEXT) h_find_context;
	if(p_find_context == NULL) return;
    closedir(p_find_context->pDir);
	if(p_find_context->DirName) AnscFreeMemory(p_find_context->DirName);
	AnscFreeMemory(p_find_context);
}
