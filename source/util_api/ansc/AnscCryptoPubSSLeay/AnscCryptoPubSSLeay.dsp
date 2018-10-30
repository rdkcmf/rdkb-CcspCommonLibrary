# Microsoft Developer Studio Project File - Name="AnscCryptoPubSSLeay" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=AnscCryptoPubSSLeay - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AnscCryptoPubSSLeay.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AnscCryptoPubSSLeay.mak" CFG="AnscCryptoPubSSLeay - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AnscCryptoPubSSLeay - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "AnscCryptoPubSSLeay - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AnscCryptoPubSSLeay - Win32 Release"

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

!ELSEIF  "$(CFG)" == "AnscCryptoPubSSLeay - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "_ANSC_WINDOWSNT" /D "_ANSC_USER" /D "NO_MD2" /D "NO_RC4" /FR /YX /FD /GZ /c
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

# Name "AnscCryptoPubSSLeay - Win32 Release"
# Name "AnscCryptoPubSSLeay - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\a_bitstr.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\a_bmp.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\a_bool.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\a_bytes.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\a_d2i_fp.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\a_digest.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\a_dup.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\a_hdr.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\a_i2d_fp.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\a_int.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\a_meth.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\a_object.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\a_octet.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\a_print.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\a_set.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\a_sign.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\a_type.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\a_utctm.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\a_verify.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\aes_cbc.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\aes_cfb.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\aes_core.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\aes_ctr.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\aes_ecb.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\aes_ofb.c
# End Source File
# Begin Source File

SOURCE=.\ansc_crypto_pub_ssleay_base.c
# End Source File
# Begin Source File

SOURCE=.\ansc_crypto_pub_ssleay_dh.c
# End Source File
# Begin Source File

SOURCE=.\ansc_crypto_pub_ssleay_encrypt.c
# End Source File
# Begin Source File

SOURCE=.\ansc_crypto_pub_ssleay_interface.c
# End Source File
# Begin Source File

SOURCE=.\ansc_crypto_pub_ssleay_key.c
# End Source File
# Begin Source File

SOURCE=.\ansc_crypto_pub_ssleay_key_gen.c
# End Source File
# Begin Source File

SOURCE=.\ansc_crypto_pub_ssleay_process.c
# End Source File
# Begin Source File

SOURCE=.\ansc_crypto_pub_ssleay_sign.c
# End Source File
# Begin Source File

SOURCE=.\ansc_crypto_pub_ssleay_util.c
# End Source File
# Begin Source File

SOURCE=.\ansc_crypto_pub_ssleay_verify.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\asn1_err.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\asn1_lib.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\asn1_par.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\bio_b64.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\bio_cb.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\bio_enc.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\bio_err.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\bio_lib.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\bio_md.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\bn_add.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\bn_blind.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\bn_div.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\bn_err.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\bn_exp.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\bn_gcd.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\bn_lib.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\bn_m.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\bn_mod.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\bn_mont.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\bn_mpi.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\bn_mul.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\bn_mulw.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\bn_prime.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\bn_print.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\bn_rand.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\bn_recp.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\bn_shift.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\bn_sqr.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\bn_sub.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\bn_word.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\bss_file.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\bss_mem.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\bss_null.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\buf_err.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\buffer.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\cryptlib.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\dh_check.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\dh_err.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\dh_gen.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\dh_key.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\dh_lib.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\dsa_err.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\dsa_gen.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\dsa_key.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\dsa_lib.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\dsa_sign.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\dsa_vrf.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\dsa_vrf.cbk
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\err.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\ex_data.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\lh_stats.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\lhash.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\md_rand.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\mem.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\obj_dat.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\obj_err.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\obj_lib.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\rsa_eay.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\rsa_err.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\rsa_gen.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\rsa_lib.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\rsa_none.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\rsa_pk1.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\rsa_saos.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\rsa_sign.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\rsa_ssl.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\stack.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\x509_cmp.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\x509_d2.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\x509_def.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\x509_err.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\x509_ext.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\x509_lu.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\x509_obj.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\x509_r2x.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\x509_req.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\x509_set.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\x509_txt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\x509_v3.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\x509_vfy.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\x509name.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\x509pack.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\x509rset.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\x509type.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\x_algor.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\x_all.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\x_attrib.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\x_cinf.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\x_crl.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\x_exten.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\x_info.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\x_name.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\x_pkey.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\x_pubkey.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\x_req.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\x_sig.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\x_spki.c
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\x_val.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\aes.h
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\aes_locl.h
# End Source File
# Begin Source File

SOURCE=..\include\ansc_crypto_pub_ssleay_external_api.h
# End Source File
# Begin Source File

SOURCE=.\ansc_crypto_pub_ssleay_global.h
# End Source File
# Begin Source File

SOURCE=..\include\ansc_crypto_pub_ssleay_interface.h
# End Source File
# Begin Source File

SOURCE=.\ansc_crypto_pub_ssleay_internal_api.h
# End Source File
# Begin Source File

SOURCE=.\ansc_crypto_pub_ssleay_util.h
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\asn1.h
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\asn1_mac.h
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\bio.h
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\blowfish.h
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\bn.h
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\bn_lcl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\bn_prime.h
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\buffer.h
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\cast.h
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\cast_s.h
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\cryptlib.h
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\crypto.h
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\date.h
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\des.h
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\dh.h
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\dsa.h
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\e_os.h
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\err.h
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\evp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\idea.h
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\lhash.h
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\lzsc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\md2.h
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\md5.h
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\mdc2.h
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\obj_dat.h
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\objects.h
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\pem.h
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\pkcs7.h
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\rand.h
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\rc2.h
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\rc4.h
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\rc5.h
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\ripemd.h
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\rsa.h
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\sha.h
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\stack.h
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\x509.h
# End Source File
# Begin Source File

SOURCE=..\..\..\opensource\Crypto\x509_vfy.h
# End Source File
# End Group
# End Target
# End Project
