# Microsoft Developer Studio Project File - Name="Asn1Alcert" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Asn1Alcert - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "asn1alcert.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "asn1alcert.mak" CFG="Asn1Alcert - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Asn1Alcert - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Asn1Alcert - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Product/BroadWay/Bisga/library/bisga_asn1_tank", GDYGAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Asn1Alcert - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /I "..\..\include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_ANSC_WINDOWSNT" /D "_ANSC_USER" /FR /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Asn1Alcert-Release.lib"

!ELSEIF  "$(CFG)" == "Asn1Alcert - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\..\include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_ANSC_WINDOWSNT" /D "_ANSC_USER" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Asn1Alcert - DEBUG.lib"

!ENDIF 

# Begin Target

# Name "Asn1Alcert - Win32 Release"
# Name "Asn1Alcert - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\al_cert_api.c
# End Source File
# Begin Source File

SOURCE=.\alcert_base.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ansc_asn1_advanced_local.h
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_algorithmidentifier_internal.h
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_certificate_internal.h
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_certificaterequest_internal.h
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_contentinfo_internal.h
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_crl_internal.h
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_extensions_internal.h
# End Source File
# Begin Source File

SOURCE="..\adv-include\ansc_asn1_name_export_api.h"
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_name_internal.h
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_privatekeyinfo_internal.h
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_publickey_internal.h
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_rsadigestinfo_internal.h
# End Source File
# Begin Source File

SOURCE=.\pki_internal.h
# End Source File
# End Group
# End Target
# End Project
