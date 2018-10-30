# Microsoft Developer Studio Project File - Name="BreeServiceProvider" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=BreeServiceProvider - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BreeServiceProvider.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BreeServiceProvider.mak" CFG="BreeServiceProvider - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BreeServiceProvider - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "BreeServiceProvider - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BreeServiceProvider - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_ANSC_WINDOWSNT" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "BreeServiceProvider - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "BreeServiceProvider___Win32_Debug"
# PROP BASE Intermediate_Dir "BreeServiceProvider___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_ANSC_WINDOWSNT" /D "_ANSC_USER" /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "BreeServiceProvider - Win32 Release"
# Name "BreeServiceProvider - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\bree_spo_base.c
# End Source File
# Begin Source File

SOURCE=.\bree_spo_cooked_page_api.c
# End Source File
# Begin Source File

SOURCE=.\bree_spo_helper.c
# End Source File
# Begin Source File

SOURCE=.\bree_spo_helper_base.c
# End Source File
# Begin Source File

SOURCE=.\bree_spo_helper_ext.c
# End Source File
# Begin Source File

SOURCE=.\bree_spo_interface.c
# End Source File
# Begin Source File

SOURCE=.\bree_spo_operation.c
# End Source File
# Begin Source File

SOURCE=.\bree_spo_process.c
# End Source File
# Begin Source File

SOURCE=.\bree_spo_soaif.c
# End Source File
# Begin Source File

SOURCE=.\bree_spo_srm.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\include\bree_definitions.h
# End Source File
# Begin Source File

SOURCE=..\..\include\bree_spo_cooked_page.h
# End Source File
# Begin Source File

SOURCE=.\bree_spo_cooked_page_api.h
# End Source File
# Begin Source File

SOURCE=..\..\include\bree_spo_exported_api.h
# End Source File
# Begin Source File

SOURCE=.\bree_spo_global.h
# End Source File
# Begin Source File

SOURCE=.\bree_spo_helper.h
# End Source File
# Begin Source File

SOURCE=.\bree_spo_helper_api.h
# End Source File
# Begin Source File

SOURCE=.\bree_spo_helper_ext.h
# End Source File
# Begin Source File

SOURCE=..\..\include\bree_spo_interface.h
# End Source File
# Begin Source File

SOURCE=.\bree_spo_internal_api.h
# End Source File
# Begin Source File

SOURCE=..\..\include\bree_spo_srm.h
# End Source File
# End Group
# End Target
# End Project
