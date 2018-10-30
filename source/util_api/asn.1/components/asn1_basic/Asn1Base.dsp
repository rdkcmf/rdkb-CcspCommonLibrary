# Microsoft Developer Studio Project File - Name="Asn1Base" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Asn1Base - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Asn1Base.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Asn1Base.mak" CFG="Asn1Base - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Asn1Base - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Asn1Base - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Product/BroadWay/Bisga/library/bisga_asn1_tank", GDYGAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Asn1Base - Win32 Release"

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
# ADD LIB32 /nologo /out:"Asn1Base - RELEASE.lib"

!ELSEIF  "$(CFG)" == "Asn1Base - Win32 Debug"

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
# ADD LIB32 /nologo /out:"Asn1Base - DEBUG.lib"

!ENDIF 

# Begin Target

# Name "Asn1Base - Win32 Release"
# Name "Asn1Base - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ansc_asn1_any.c
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_attr_interface.c
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_base_interface.c
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_bitstring.c
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_bool.c
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_choice.c
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_enumerate.c
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_export_utils.c
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_external_api.c
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_integer.c
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_null.c
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_oid.c
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_real.c
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_sequence.c
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_sequenceof.c
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_set.c
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_setof.c
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_string.c
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_taginfo_interface.c
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_time.c
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_utils.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\include\al_cmp_export_api.h
# End Source File
# Begin Source File

SOURCE=..\..\include\al_cmp_interface.h
# End Source File
# Begin Source File

SOURCE=..\..\include\al_ocsp_export_api.h
# End Source File
# Begin Source File

SOURCE=..\..\include\al_ocsp_interface.h
# End Source File
# Begin Source File

SOURCE=..\..\include\al_pkcs12_export_api.h
# End Source File
# Begin Source File

SOURCE=..\..\include\al_pkcs12_interface.h
# End Source File
# Begin Source File

SOURCE=..\..\include\alcert_export_api.h
# End Source File
# Begin Source File

SOURCE=..\..\include\alcert_global.h
# End Source File
# Begin Source File

SOURCE=..\..\include\alcert_interface.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ANSC_ASN1.H
# End Source File
# Begin Source File

SOURCE=..\..\include\ansc_asn1_algorithmidentifier_interface.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ansc_asn1_attr_extern_api.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ansc_asn1_attr_interface.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ansc_asn1_base_extern_api.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ansc_asn1_base_interface.h
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_base_internal.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ansc_asn1_certificate_export_api.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ansc_asn1_certificate_interface.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ansc_asn1_certificaterequest_export_api.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ansc_asn1_certificaterequest_interface.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ansc_asn1_contentinfo_export_api.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ansc_asn1_contentinfo_interface.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ansc_asn1_crl_export_api.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ansc_asn1_crl_interface.h
# End Source File
# Begin Source File

SOURCE=..\include\ansc_asn1_export_utils.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ansc_asn1_export_utils.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ansc_asn1_extensions_export_api.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ansc_asn1_extensions_interface.h
# End Source File
# Begin Source File

SOURCE=..\include\ansc_asn1_external_api.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ansc_asn1_external_api.h
# End Source File
# Begin Source File

SOURCE=..\include\ansc_asn1_global.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ansc_asn1_global.h
# End Source File
# Begin Source File

SOURCE=..\include\ansc_asn1_interface.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ansc_asn1_interface.h
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_internal.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ansc_asn1_name_export_api.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ansc_asn1_name_interface.h
# End Source File
# Begin Source File

SOURCE=..\include\ansc_asn1_obj_extern_api.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ansc_asn1_obj_extern_api.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ansc_asn1_obj_interface.h
# End Source File
# Begin Source File

SOURCE=.\ansc_asn1_obj_internal.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ansc_asn1_othername_export_api.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ansc_asn1_othername_interface.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ansc_asn1_privatekeyinfo_export_api.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ansc_asn1_privatekeyinfo_interface.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ansc_asn1_publickey_export_api.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ansc_asn1_publickey_interface.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ansc_asn1_rsadigestinfo_export_api.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ansc_asn1_rsadigestinfo_interface.h
# End Source File
# Begin Source File

SOURCE=..\include\ansc_asn1_status.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ansc_asn1_status.h
# End Source File
# Begin Source File

SOURCE=..\include\ansc_asn1_taginfo_extern_api.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ansc_asn1_taginfo_extern_api.h
# End Source File
# Begin Source File

SOURCE=..\include\ansc_asn1_taginfo_interface.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ansc_asn1_taginfo_interface.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ansc_asn1_utils.h
# End Source File
# Begin Source File

SOURCE=..\include\ansc_base_string_util.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ansc_base_string_util.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ansc_pki_export_api.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ansc_pki_global.h
# End Source File
# Begin Source File

SOURCE=..\..\include\asn1_advanced_export_api.h
# End Source File
# Begin Source File

SOURCE=..\..\include\asn1_advanced_global.h
# End Source File
# Begin Source File

SOURCE=..\..\include\asn1_advanced_interface.h
# End Source File
# Begin Source File

SOURCE=..\..\include\asn1_certreqmsg_export_api.h
# End Source File
# Begin Source File

SOURCE=..\..\include\asn1_certreqmsg_interface.h
# End Source File
# Begin Source File

SOURCE=..\..\include\asn1_ocsprequest_export_api.h
# End Source File
# Begin Source File

SOURCE=..\..\include\asn1_ocsprequest_interface.h
# End Source File
# Begin Source File

SOURCE=..\..\include\asn1_pfx_export_api.h
# End Source File
# Begin Source File

SOURCE=..\..\include\asn1_pfx_interface.h
# End Source File
# Begin Source File

SOURCE=..\..\include\asn1_pkimessage_export_api.h
# End Source File
# Begin Source File

SOURCE=..\..\include\asn1_pkimessage_interface.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ca_entity_interface.h
# End Source File
# Begin Source File

SOURCE=..\..\include\ca_export_api.h
# End Source File
# Begin Source File

SOURCE=..\..\include\cert_attr_structure.h
# End Source File
# Begin Source File

SOURCE=..\..\include\cert_exported_api.h
# End Source File
# Begin Source File

SOURCE=..\..\include\cert_status.h
# End Source File
# Begin Source File

SOURCE=..\..\include\crypt_proc.h
# End Source File
# Begin Source File

SOURCE=..\..\include\crypt_spi.h
# End Source File
# Begin Source File

SOURCE=..\..\include\cw_vpn_sakm.h
# End Source File
# Begin Source File

SOURCE=..\..\include\pkcs12_handler_export_api.h
# End Source File
# Begin Source File

SOURCE=..\..\include\pkcs12_handler_interface.h
# End Source File
# Begin Source File

SOURCE=..\..\include\pkcs12_utility_interface.h
# End Source File
# Begin Source File

SOURCE=..\..\include\pki_entity_interface.h
# End Source File
# Begin Source File

SOURCE=..\..\include\pki_export_api.h
# End Source File
# Begin Source File

SOURCE=..\..\include\pki_export_utils.h
# End Source File
# Begin Source File

SOURCE=..\..\include\pki_interface.h
# End Source File
# Begin Source File

SOURCE=..\..\include\pki_utility_api.h
# End Source File
# Begin Source File

SOURCE=..\..\include\smartcard_sample_interface.h
# End Source File
# End Group
# End Target
# End Project
