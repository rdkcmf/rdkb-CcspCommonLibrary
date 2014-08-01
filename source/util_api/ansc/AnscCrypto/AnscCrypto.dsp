# Microsoft Developer Studio Project File - Name="AnscCrypto" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=AnscCrypto - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AnscCrypto.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AnscCrypto.mak" CFG="AnscCrypto - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AnscCrypto - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "AnscCrypto - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Product/BroadWay/Bisga/isc/asn.1/components/asn1_basic", DQGEAAAA"
# PROP Scc_LocalPath "..\..\isc\asn.1\components\asn1_basic"
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AnscCrypto - Win32 Release"

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
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_ANSC_WINDOWSNT" /D "_ANSC_USER" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "AnscCrypto - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_ANSC_WINDOWSNT" /D "_ANSC_USER" /FR /YX /FD /GZ /c
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

# Name "AnscCrypto - Win32 Release"
# Name "AnscCrypto - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ansc_crypto_base.c
# End Source File
# Begin Source File

SOURCE=.\ansc_crypto_compression.c
# End Source File
# Begin Source File

SOURCE=.\ansc_crypto_decompression.c
# End Source File
# Begin Source File

SOURCE=.\ansc_crypto_decryption.c
# End Source File
# Begin Source File

SOURCE=.\ansc_crypto_digestion.c
# End Source File
# Begin Source File

SOURCE=.\ansc_crypto_encryption.c
# End Source File
# Begin Source File

SOURCE=.\ansc_crypto_hash.c
# End Source File
# Begin Source File

SOURCE=.\ansc_crypto_interface.c
# End Source File
# Begin Source File

SOURCE=.\ansc_crypto_key.c
# End Source File
# Begin Source File

SOURCE=.\ansc_crypto_random.c
# End Source File
# Begin Source File

SOURCE=.\ansc_crypto_utility.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ansc_crypto_global.h
# End Source File
# Begin Source File

SOURCE=.\ansc_crypto_internal_api.h
# End Source File
# Begin Source File

SOURCE=.\crypto\BLOWFISH\BF_LOCL.H
# End Source File
# Begin Source File

SOURCE=.\crypto\BLOWFISH\BF_PI.H
# End Source File
# Begin Source File

SOURCE=.\crypto\BLOWFISH\BLOWFISH.H
# End Source File
# Begin Source File

SOURCE=.\crypto\zlib123\crc32.h
# End Source File
# Begin Source File

SOURCE=.\crypto\zlib123\deflate.h
# End Source File
# Begin Source File

SOURCE=.\crypto\DES\DES.H
# End Source File
# Begin Source File

SOURCE=.\crypto\DES\DES_LOCL.H
# End Source File
# Begin Source File

SOURCE=.\crypto\DES\DES_VER.H
# End Source File
# Begin Source File

SOURCE=.\crypto\IDEA\IDEA.H
# End Source File
# Begin Source File

SOURCE=.\crypto\IDEA\IDEA_LCL.H
# End Source File
# Begin Source File

SOURCE=.\crypto\zlib123\inffast.h
# End Source File
# Begin Source File

SOURCE=.\crypto\zlib123\inffixed.h
# End Source File
# Begin Source File

SOURCE=.\crypto\zlib123\inflate.h
# End Source File
# Begin Source File

SOURCE=.\crypto\zlib123\inftrees.h
# End Source File
# Begin Source File

SOURCE=.\crypto\LZS\LZSC.H
# End Source File
# Begin Source File

SOURCE=.\crypto\MD5\MD2.H
# End Source File
# Begin Source File

SOURCE=.\crypto\MD4\md4.h
# End Source File
# Begin Source File

SOURCE=.\crypto\MD5\MD5.H
# End Source File
# Begin Source File

SOURCE=.\crypto\MD5\MD5_LOCL.H
# End Source File
# Begin Source File

SOURCE=.\crypto\DES\PODD.H
# End Source File
# Begin Source File

SOURCE=.\crypto\RC4\RC4.H
# End Source File
# Begin Source File

SOURCE=.\crypto\rc4\rc4_2.h
# End Source File
# Begin Source File

SOURCE=.\crypto\DES\RPC_DES.H
# End Source File
# Begin Source File

SOURCE=.\crypto\SHA1\SHA.H
# End Source File
# Begin Source File

SOURCE=.\crypto\SHA1\SHA_LOCL.H
# End Source File
# Begin Source File

SOURCE=.\crypto\DES\SK.H
# End Source File
# Begin Source File

SOURCE=.\crypto\DES\SPR.H
# End Source File
# Begin Source File

SOURCE=.\crypto\zlib123\trees.h
# End Source File
# Begin Source File

SOURCE=.\crypto\zlib123\zconf.h
# End Source File
# Begin Source File

SOURCE=.\crypto\zlib123\zlib.h
# End Source File
# Begin Source File

SOURCE=.\crypto\zlib123\zutil.h
# End Source File
# End Group
# End Target
# End Project
