# Microsoft Developer Studio Project File - Name="Asn1X509kb5" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Asn1X509kb5 - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Asn1X509-kb5.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Asn1X509-kb5.mak" CFG="Asn1X509kb5 - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Asn1X509kb5 - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Asn1X509kb5 - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Product/BroadWay/Bisga/isc/asn.1/components/asn1_basic", DQGEAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Asn1X509kb5 - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /I "..\include" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "_ANSC_WINDOWSNT" /D "_ANSC_USER" /FR /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Asn1X509kb5-Release.lib"

!ELSEIF  "$(CFG)" == "Asn1X509kb5 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\include" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "_ANSC_WINDOWSNT" /D "_ANSC_USER" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Asn1X509kb5 - DEBUG.lib"

!ENDIF 

# Begin Target

# Name "Asn1X509kb5 - Win32 Release"
# Name "Asn1X509kb5 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ansc_asn1_algorithmidentifier.c
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_certificate.c
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_certificaterequest.c
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_contentinfo.c
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_crl.c
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_extensions.c
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_keyagreement.c
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_name.c
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_othername.c
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_privatekeyinfo.c
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_publickey.c
# End Source File
# Begin Source File

SOURCE=.\pki_cert_attr.c
# End Source File
# Begin Source File

SOURCE=.\pki_export_utils.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ansc_asn1_advanced_local.h
# End Source File
# Begin Source File

SOURCE="..\adv-include\ansc_asn1_algorithmidentifier_export_api.h"
# End Source File
# Begin Source File

SOURCE="..\adv-include\ansc_asn1_algorithmidentifier_interface.h"
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_algorithmidentifier_internal.h
# End Source File
# Begin Source File

SOURCE="..\adv-include\ansc_asn1_certificate_export_api.h"
# End Source File
# Begin Source File

SOURCE="..\adv-include\ansc_asn1_certificate_interface.h"
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_certificate_internal.h
# End Source File
# Begin Source File

SOURCE="..\adv-include\ansc_asn1_certificaterequest_export_api.h"
# End Source File
# Begin Source File

SOURCE="..\adv-include\ansc_asn1_certificaterequest_interface.h"
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_certificaterequest_internal.h
# End Source File
# Begin Source File

SOURCE="..\adv-include\ansc_asn1_contentinfo_export_api.h"
# End Source File
# Begin Source File

SOURCE="..\adv-include\ansc_asn1_contentinfo_interface.h"
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_contentinfo_internal.h
# End Source File
# Begin Source File

SOURCE="..\adv-include\ansc_asn1_crl_export_api.h"
# End Source File
# Begin Source File

SOURCE="..\adv-include\ansc_asn1_crl_interface.h"
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_crl_internal.h
# End Source File
# Begin Source File

SOURCE="..\adv-include\ansc_asn1_extensions_export_api.h"
# End Source File
# Begin Source File

SOURCE="..\adv-include\ansc_asn1_extensions_interface.h"
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_extensions_internal.h
# End Source File
# Begin Source File

SOURCE="..\adv-include\ansc_asn1_name_export_api.h"
# End Source File
# Begin Source File

SOURCE="..\adv-include\ansc_asn1_name_interface.h"
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_name_internal.h
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_othername_internal.h
# End Source File
# Begin Source File

SOURCE="..\adv-include\ansc_asn1_privatekeyinfo_export_api.h"
# End Source File
# Begin Source File

SOURCE="..\adv-include\ansc_asn1_privatekeyinfo_interface.h"
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_privatekeyinfo_internal.h
# End Source File
# Begin Source File

SOURCE="..\adv-include\ansc_asn1_publickey_export_api.h"
# End Source File
# Begin Source File

SOURCE="..\adv-include\ansc_asn1_publickey_interface.h"
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_publickey_internal.h
# End Source File
# Begin Source File

SOURCE="..\adv-include\ansc_asn1_rsadigestinfo_export_api.h"
# End Source File
# Begin Source File

SOURCE="..\adv-include\ansc_asn1_rsadigestinfo_interface.h"
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_rsadigestinfo_internal.h
# End Source File
# Begin Source File

SOURCE="..\adv-include\asn1_advanced_export_api.h"
# End Source File
# Begin Source File

SOURCE="..\adv-include\asn1_advanced_global.h"
# End Source File
# Begin Source File

SOURCE="..\adv-include\asn1_advanced_interface.h"
# End Source File
# Begin Source File

SOURCE="..\adv-include\crypt_proc.h"
# End Source File
# Begin Source File

SOURCE="..\adv-include\pki_export_api.h"
# End Source File
# Begin Source File

SOURCE="..\adv-include\pki_export_utils.h"
# End Source File
# Begin Source File

SOURCE="..\adv-include\pki_interface.h"
# End Source File
# Begin Source File

SOURCE=.\pki_internal.h
# End Source File
# End Group
# End Target
# End Project
