##########################################################################
# If not stated otherwise in this file or this component's Licenses.txt
# file the following copyright and licenses apply:
#
# Copyright 2015 RDK Management
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
##########################################################################
#                                              -*- Autoconf -*-
# Process this file with autoconf to produce configure script.
#

AC_PREREQ([2.65])
AC_INIT([ccsp_common], [1.0], [BUG-REPORT-ADDRESS])
AM_INIT_AUTOMAKE
LT_INIT
GTEST_ENABLE_FLAG = ""

AC_ARG_ENABLE([gtestapp],
             AS_HELP_STRING([--enable-gtestapp],[enable Gtest support (default is no)]),
             [
               case "${enableval}" in
                yes) GTEST_SUPPORT_ENABLED=true
                     GTEST_ENABLE_FLAG="-DGTEST_ENABLE"
                     m4_if(m4_sysval,[0],[AC_CONFIG_FILES([source/test/Makefile])]);;
                no) GTEST_SUPPORT_ENABLED=false AC_MSG_ERROR([Gtest support is disabled]);;
                 *) AC_MSG_ERROR([bad value ${enableval} for --enable-gtestapp ]);;
               esac
             ],
             [echo "Gtestapp is disabled"])
AM_CONDITIONAL([WITH_GTEST_SUPPORT], [test x$GTEST_SUPPORT_ENABLED = xtrue])

AC_PREFIX_DEFAULT(`pwd`)
AC_ENABLE_SHARED
AC_DISABLE_STATIC

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AM_PROG_CC_C_O
AM_PROG_LIBTOOL(libtool)

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE

# Checks for library functions.
AC_FUNC_MALLOC

AC_CONFIG_FILES(
	source/cosa/utilities/AnscObjectArray/AnscByteArray/Makefile
	source/cosa/utilities/AnscObjectArray/AnscUIntArray/Makefile
	source/cosa/utilities/AnscObjectArray/AnscPtrArray/Makefile
	source/cosa/utilities/AnscObjectArray/AnscStringArray/Makefile
	source/cosa/utilities/AnscObjectArray/ObjectArrayBase/Makefile
	source/cosa/utilities/AnscObjectArray/Makefile
	source/cosa/utilities/bsp_eng/Makefile
	source/cosa/utilities/Makefile
	source/cosa/package/bmw2/bwsp/components/BwspPortalHost/Makefile
	source/cosa/package/bmw2/bwsp/components/BwspWebServlet/Makefile
	source/cosa/package/bmw2/bwsp/components/BwspSiteCreator/Makefile
	source/cosa/package/bmw2/bwsp/components/Makefile
	source/cosa/package/bmw2/bwsp/Makefile
	source/cosa/package/bmw2/bree/components/BreeSpo/Makefile
	source/cosa/package/bmw2/bree/components/BreeSrm/Makefile
	source/cosa/package/bmw2/bree/components/Makefile
	source/cosa/package/bmw2/bree/Makefile
	source/cosa/package/bmw2/bwrm/components/BwrmEnvController/Makefile
	source/cosa/package/bmw2/bwrm/components/BwrmCookedPage/Makefile
	source/cosa/package/bmw2/bwrm/components/BwrmPageManager/Makefile
	source/cosa/package/bmw2/bwrm/components/BwrmFileManager/Makefile
	source/cosa/package/bmw2/bwrm/components/Makefile
	source/cosa/package/bmw2/bwrm/Makefile
	source/cosa/package/bmw2/Makefile
	source/cosa/package/system/components/SysRroUint/Makefile
	source/cosa/package/system/components/SysRepositoryRecord/Makefile
	source/cosa/package/system/components/SysRroBstr/Makefile
	source/cosa/package/system/components/SysRepositoryDriver/Makefile
	source/cosa/package/system/components/SysRroSint/Makefile
	source/cosa/package/system/components/SysRroHcxt/Makefile
	source/cosa/package/system/components/SysRroEnum/Makefile
	source/cosa/package/system/components/SysInfoRepository/Makefile
	source/cosa/package/system/components/SysRecycleBin/Makefile
	source/cosa/package/system/components/SysRroAstr/Makefile
	source/cosa/package/system/components/SysRroBool/Makefile
	source/cosa/package/system/components/SysRepositoryFolder/Makefile
	source/cosa/package/system/components/Makefile
	source/cosa/package/system/Makefile
	source/cosa/package/slap/services/dslh/SlapDslhParamTree/Makefile
	source/cosa/package/slap/services/dslh/Makefile
	source/cosa/package/slap/services/bmc2/SlapBmc2Server/Makefile
	source/cosa/package/slap/services/bmc2/SlapBmc2Terminal/Makefile
	source/cosa/package/slap/services/bmc2/SlapBmc2Output/Makefile
	source/cosa/package/slap/services/bmc2/SlapBmc2Input/Makefile
	source/cosa/package/slap/services/bmc2/SlapBmc2Command/Makefile
	source/cosa/package/slap/services/bmc2/SlapBmc2Domain/Makefile
	source/cosa/package/slap/services/bmc2/SlapBmc2Application/Makefile
	source/cosa/package/slap/services/bmc2/Makefile
	source/cosa/package/slap/services/Makefile
	source/cosa/package/slap/Makefile
	source/cosa/package/cli/components/ScliShell/Makefile
	source/cosa/package/cli/components/Makefile
	source/cosa/package/cli/Makefile
	source/cosa/package/bmc2/components/Bmc2EnvController/Makefile
	source/cosa/package/bmc2/components/Bmc2ReqController/Makefile
	source/cosa/package/bmc2/components/Bmc2ComExecutor/Makefile
	source/cosa/package/bmc2/components/Bmc2ComTerminal/Makefile
	source/cosa/package/bmc2/components/Bmc2ComDomain/Makefile
	source/cosa/package/bmc2/components/Makefile
	source/cosa/package/bmc2/Makefile
	source/cosa/package/Makefile
	source/cosa/Makefile
	source/debug_api/Makefile
	source/ccsp/components/common/PoamIrepFolder/Makefile
	source/ccsp/components/common/DataModel/dml/components/DslhVarEntity/Makefile
	source/ccsp/components/common/DataModel/dml/components/DslhObjController/Makefile
	source/ccsp/components/common/DataModel/dml/components/DslhObjRecord/Makefile
	source/ccsp/components/common/DataModel/dml/components/DslhVarRecord/Makefile
	source/ccsp/components/common/DataModel/dml/components/DslhWmpDatabase/Makefile
	source/ccsp/components/common/DataModel/dml/components/DslhDataModelAgent/Makefile
	source/ccsp/components/common/DataModel/dml/components/DslhCpeController/Makefile
	source/ccsp/components/common/DataModel/dml/components/DslhObjEntity/Makefile
	source/ccsp/components/common/DataModel/dml/components/DslhObjcoTable/Makefile
	source/ccsp/components/common/DataModel/dml/components/Makefile
	source/ccsp/components/common/DataModel/dml/Makefile
	source/ccsp/components/common/DataModel/Makefile
	source/ccsp/components/common/MessageBusHelper/helper/Makefile
	source/ccsp/components/common/MessageBusHelper/Makefile
	source/ccsp/components/common/Makefile
	source/ccsp/components/CCSP_NamespaceMgr/Makefile
        source/ccsp/components/CCSP_AliasMgr/Makefile
	source/ccsp/components/Makefile
	source/ccsp/Makefile
	source/util_api/tls/containers/TlsModuleContainer/Makefile
	source/util_api/tls/containers/Makefile
	source/util_api/tls/components/TlsRecordKeeper/Makefile
	source/util_api/tls/components/TlsCryptoProvider/Makefile
	source/util_api/tls/components/TlsEnvController/Makefile
	source/util_api/tls/components/TlsSessionManager/Makefile
	source/util_api/tls/components/TlsRecordClient/Makefile
	source/util_api/tls/components/TlsHsoClient/Makefile
	source/util_api/tls/components/TlsHsoServer/Makefile
	source/util_api/tls/components/TlsSessionEntry/Makefile
	source/util_api/tls/components/TlsAppConnector/Makefile
	source/util_api/tls/components/TlsHandShaker/Makefile
	source/util_api/tls/components/TlsCipherSwitch/Makefile
	source/util_api/tls/components/TlsAlertHandler/Makefile
	source/util_api/tls/components/TlsConnController/Makefile
	source/util_api/tls/components/Makefile
	source/util_api/tls/Makefile
	source/util_api/ccsp_msg_bus/Makefile
	source/util_api/slap/components/SlapObjBroker/Makefile
	source/util_api/slap/components/SlapObjRecord/Makefile
	source/util_api/slap/components/SlapVarHelper/Makefile
	source/util_api/slap/components/SlapScoGallery/Makefile
	source/util_api/slap/components/SlapLoamServer/Makefile
	source/util_api/slap/components/SlapObjContainer/Makefile
	source/util_api/slap/components/SlapLoamClient/Makefile
	source/util_api/slap/components/SlapVarConverter/Makefile
	source/util_api/slap/components/SlapVarEntity/Makefile
	source/util_api/slap/components/SlapVarMapper/Makefile
	source/util_api/slap/components/SlapEnvController/Makefile
	source/util_api/slap/components/SlapOwoUoac/Makefile
	source/util_api/slap/components/SlapObjAgent/Makefile
	source/util_api/slap/components/SlapAccessManager/Makefile
	source/util_api/slap/components/SlapSrvComponent/Makefile
	source/util_api/slap/components/SlapLoamBroker/Makefile
	source/util_api/slap/components/SlapObjWrapper/Makefile
	source/util_api/slap/components/SlapObjMapper/Makefile
	source/util_api/slap/components/SlapOwoLoao/Makefile
	source/util_api/slap/components/SlapObjProxy/Makefile
	source/util_api/slap/components/SlapSrvProxy/Makefile
	source/util_api/slap/components/SlapOwoLoac/Makefile
	source/util_api/slap/components/SlapOwoUoao/Makefile
	source/util_api/slap/components/SlapObjEntity/Makefile
	source/util_api/slap/components/SlapScoCollection/Makefile
	source/util_api/slap/components/SlapScoStandard/Makefile
	source/util_api/slap/components/SlapScoBuffer/Makefile
	source/util_api/slap/components/Makefile
	source/util_api/slap/Makefile
	source/util_api/ccsp_dm_api/Makefile
	source/util_api/web/containers/WebModuleContainer/Makefile
	source/util_api/web/containers/Makefile
	source/util_api/web/components/WebResourceLocator/Makefile
	source/util_api/web/components/WebVirtualHost/Makefile
	source/util_api/web/components/WebStatusAtomTable/Makefile
	source/util_api/web/components/WebSimpleServer/Makefile
	source/util_api/web/components/WebAuthServer/Makefile
	source/util_api/web/components/WebResourceOwner/Makefile
	source/util_api/web/components/WebUriPath/Makefile
	source/util_api/web/components/WebRooFsm/Makefile
	source/util_api/web/components/WebGeneralSession/Makefile
	source/util_api/web/components/Makefile
	source/util_api/web/Makefile
	source/util_api/ansc/AnscLightComponent/Makefile
	source/util_api/ansc/AnscSimpleSystemTime/Makefile
	source/util_api/ansc/AnscTimerScheduler/Makefile
	source/util_api/ansc/AnscObjectMapper/Makefile
	source/util_api/ansc/AnscBrokerEngineTcp/Makefile
	source/util_api/ansc/AnscSimpleClientTcp/Makefile
	source/util_api/ansc/AnscCryptoPubSSLeay/Makefile
	source/util_api/ansc/AnscDaemonServerUdp/Makefile
	source/util_api/ansc/AnscCoVer3/Makefile
	source/util_api/ansc/AnscXmlDomParser/Makefile
	source/util_api/ansc/AnscXsocket/Makefile
	source/util_api/ansc/AnscQueryInterface/Makefile
	source/util_api/ansc/AnscObjectContainer/Makefile
	source/util_api/ansc/AnscLpcConnector/Makefile
	source/util_api/ansc/AnscUtilBox/Makefile
	source/util_api/ansc/AnscModuleContainer/Makefile
	source/util_api/ansc/AnscDaemonServerTcp/Makefile
	source/util_api/ansc/AnscCryptoPub/Makefile
	source/util_api/ansc/AnscBrokerSocketTcp/Makefile
	source/util_api/ansc/AnscPlatform/Makefile
	source/util_api/ansc/AnscString/Makefile
	source/util_api/ansc/AnscDaemonEngineUdp/Makefile
	source/util_api/ansc/AnscComponentObject/Makefile
	source/util_api/ansc/AnscDaemonEngineTcp/Makefile
	source/util_api/ansc/AnscBrokerServerTcp/Makefile
	source/util_api/ansc/AnscDaemonSocketUdp/Makefile
	source/util_api/ansc/AnscAtomTable/Makefile
	source/util_api/ansc/AnscCrypto/Makefile
	source/util_api/ansc/AnscSimpleClientUdp/Makefile
	source/util_api/ansc/AnscDaemonSocketTcp/Makefile
	source/util_api/ansc/AnscStringUtil/Makefile
	source/util_api/ansc/AnscLpccoTcp/Makefile
	source/util_api/ansc/Makefile
	source/util_api/stun/components/StunSimpleClient/Makefile
	source/util_api/stun/components/Makefile
	source/util_api/stun/Makefile
	source/util_api/http/containers/HttpHelperContainer/Makefile
	source/util_api/http/containers/Makefile
	source/util_api/http/components/HttpSimpleServer/Makefile
	source/util_api/http/components/HttpAuth/Makefile
	source/util_api/http/components/HttpAtoStatus/Makefile
	source/util_api/http/components/HttpMboChunked/Makefile
	source/util_api/http/components/HttpMessageBody/Makefile
	source/util_api/http/components/HttpTransRecord/Makefile
	source/util_api/http/components/HttpBmoRep/Makefile
	source/util_api/http/components/HttpAuthClient/Makefile
	source/util_api/http/components/HttpAtoFxCat2/Makefile
	source/util_api/http/components/HttpAtoHeader/Makefile
	source/util_api/http/components/HttpSimpleClient/Makefile
	source/util_api/http/components/HttpMboIdentity/Makefile
	source/util_api/http/components/HttpWebsTrans/Makefile
	source/util_api/http/components/HttpWebsSession/Makefile
	source/util_api/http/components/HttpAtoFxCat1/Makefile
	source/util_api/http/components/HttpMboFile/Makefile
	source/util_api/http/components/HttpWebcTrans/Makefile
	source/util_api/http/components/HttpSimpleProxy/Makefile
	source/util_api/http/components/HttpAtoMethod/Makefile
	source/util_api/http/components/HttpAtoCgiEnv/Makefile
	source/util_api/http/components/HttpPsoVer2/Makefile
	source/util_api/http/components/HttpBmoReq/Makefile
	source/util_api/http/components/HttpAdvancedProxy/Makefile
	source/util_api/http/components/HttpWebcSession/Makefile
	source/util_api/http/components/HttpFormContent/Makefile
	source/util_api/http/components/HttpProxySession/Makefile
	source/util_api/http/components/HttpBasicMessage/Makefile
	source/util_api/http/components/Makefile
	source/util_api/http/utilities/HttpSimpleMsgParser/Makefile
	source/util_api/http/utilities/Makefile
	source/util_api/http/Makefile
	source/util_api/asn.1/components/al_pki/Makefile
	source/util_api/asn.1/components/asn1_x509/Makefile
	source/util_api/asn.1/components/asn1_basic/Makefile
	source/util_api/asn.1/components/al_pkcs12/Makefile
	source/util_api/asn.1/components/Makefile
	source/util_api/asn.1/Makefile
	source/util_api/Makefile
	source/dm_pack/Makefile
	source/Makefile
	Makefile


)

AC_SUBST(GTEST_ENABLE_FLAG)

AC_OUTPUT

