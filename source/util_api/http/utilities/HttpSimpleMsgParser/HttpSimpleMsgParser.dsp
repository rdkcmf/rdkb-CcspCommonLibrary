# Microsoft Developer Studio Project File - Name="HttpSimpleMsgParser" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=HttpSimpleMsgParser - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "HttpSimpleMsgParser.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "HttpSimpleMsgParser.mak" CFG="HttpSimpleMsgParser - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "HttpSimpleMsgParser - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "HttpSimpleMsgParser - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "HttpSimpleMsgParser - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "_ANSC_WINDOWSNT" /D "_ANSC_USER" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "HttpSimpleMsgParser - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "_ANSC_WINDOWSNT" /D "_ANSC_USER" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "HttpSimpleMsgParser - Win32 Release"
# Name "HttpSimpleMsgParser - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\http_smpo_base.c
# End Source File
# Begin Source File

SOURCE=.\http_smpo_interface.c
# End Source File
# Begin Source File

SOURCE=.\http_smpo_parse.c
# End Source File
# Begin Source File

SOURCE=.\http_smpo_parse_helper.c
# End Source File
# Begin Source File

SOURCE=.\http_smpo_parse_uri.c
# End Source File
# Begin Source File

SOURCE=.\http_smpo_process.c
# End Source File
# Begin Source File

SOURCE=.\http_smpo_util.c
# End Source File
# Begin Source File

SOURCE=.\http_smpo_util_construct.c
# End Source File
# Begin Source File

SOURCE=.\http_smpo_util_getsize.c
# End Source File
# Begin Source File

SOURCE=.\http_smpo_util_parsing.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\http_smpo_construct.h
# End Source File
# Begin Source File

SOURCE=..\include\http_smpo_exported_api.h
# End Source File
# Begin Source File

SOURCE=.\http_smpo_global.h
# End Source File
# Begin Source File

SOURCE=..\include\http_smpo_interface.h
# End Source File
# Begin Source File

SOURCE=.\http_smpo_internal_api.h
# End Source File
# Begin Source File

SOURCE=.\http_smpo_parse.h
# End Source File
# Begin Source File

SOURCE=.\http_smpo_parse_helper.h
# End Source File
# Begin Source File

SOURCE=.\http_smpo_util.h
# End Source File
# Begin Source File

SOURCE=.\http_smpo_util_def.h
# End Source File
# End Group
# End Target
# End Project
