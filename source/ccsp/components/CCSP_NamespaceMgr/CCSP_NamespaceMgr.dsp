# Microsoft Developer Studio Project File - Name="CCSP_NamespaceMgr" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=CCSP_NamespaceMgr - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CCSP_NamespaceMgr.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CCSP_NamespaceMgr.mak" CFG="CCSP_NamespaceMgr - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CCSP_NamespaceMgr - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "CCSP_NamespaceMgr - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CCSP_NamespaceMgr - Win32 Release"

# ADD CPP /I "..\include"
BSC32=bscmake.exe
LIB32=link.exe -lib

!ELSEIF  "$(CFG)" == "CCSP_NamespaceMgr - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_DEBUG" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_ANSC_WINDOWSNT" /D "_ANSC_USER" /D "_DEBUG" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_DEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "CCSP_NamespaceMgr - Win32 Release"
# Name "CCSP_NamespaceMgr - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ccsp_ns_comp_base.c

!IF  "$(CFG)" == "CCSP_NamespaceMgr - Win32 Release"

!ELSEIF  "$(CFG)" == "CCSP_NamespaceMgr - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ccsp_ns_comp_operation.c

!IF  "$(CFG)" == "CCSP_NamespaceMgr - Win32 Release"

!ELSEIF  "$(CFG)" == "CCSP_NamespaceMgr - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ccsp_ns_mgr_base.c

!IF  "$(CFG)" == "CCSP_NamespaceMgr - Win32 Release"

!ELSEIF  "$(CFG)" == "CCSP_NamespaceMgr - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ccsp_ns_mgr_operation.c

!IF  "$(CFG)" == "CCSP_NamespaceMgr - Win32 Release"

!ELSEIF  "$(CFG)" == "CCSP_NamespaceMgr - Win32 Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\include\CCSP_base_api.h
# End Source File
# Begin Source File

SOURCE=..\include\ccsp_cr_definitions.h
# End Source File
# Begin Source File

SOURCE=..\include\ccsp_cr_interface.h
# End Source File
# Begin Source File

SOURCE=..\include\ccsp_namespace_mgr.h
# End Source File
# Begin Source File

SOURCE=.\ccsp_ns_comp_internal_api.h
# End Source File
# Begin Source File

SOURCE=.\ccsp_ns_mgr_internal_api.h
# End Source File
# Begin Source File

SOURCE=..\include\ccsp_types.h
# End Source File
# End Group
# End Target
# End Project
