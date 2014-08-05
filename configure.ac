#                                              -*- Autoconf -*-
# Process this file with autoconf to produce configure script.
#

AC_PREREQ([2.65])
AC_INIT([CcspCommonLibrary], [1.0], [BUG-REPORT-ADDRESS])
AM_INIT_AUTOMAKE
LT_INIT

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AM_PROG_CC_C_O

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT64_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

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
	source/cosa/package/system/components/SysSetupManager/Makefile
	source/cosa/package/system/components/SysInfoRepository/Makefile
	source/cosa/package/system/components/SysIroV2/Makefile
	source/cosa/package/system/components/SysRecycleBin/Makefile
	source/cosa/package/system/components/SysRroAstr/Makefile
	source/cosa/package/system/components/SysRroBool/Makefile
	source/cosa/package/system/components/SysCtlvProcessor/Makefile
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
	source/Makefile
	./Makefile
)
#
# LIBS:
#	source/cosa/utilities/AnscObjectArray/AnscByteArray/libCcspCommonLibrary_AnscByteArray.la \
#	source/cosa/utilities/AnscObjectArray/AnscUIntArray/libCcspCommonLibrary_AnscUIntArray.la \
#	source/cosa/utilities/AnscObjectArray/AnscPtrArray/libCcspCommonLibrary_AnscPtrArray.la \
#	source/cosa/utilities/AnscObjectArray/AnscStringArray/libCcspCommonLibrary_AnscStringArray.la \
#	source/cosa/utilities/AnscObjectArray/ObjectArrayBase/libCcspCommonLibrary_ObjectArrayBase.la \
#	source/cosa/utilities/bsp_eng/libCcspCommonLibrary_bsp_eng.la \
#	source/cosa/package/bmw2/bwsp/components/BwspPortalHost/libCcspCommonLibrary_BwspPortalHost.la \
#	source/cosa/package/bmw2/bwsp/components/BwspWebServlet/libCcspCommonLibrary_BwspWebServlet.la \
#	source/cosa/package/bmw2/bwsp/components/BwspSiteCreator/libCcspCommonLibrary_BwspSiteCreator.la \
#	source/cosa/package/bmw2/bree/components/BreeSpo/libCcspCommonLibrary_BreeSpo.la \
#	source/cosa/package/bmw2/bree/components/BreeSrm/libCcspCommonLibrary_BreeSrm.la \
#	source/cosa/package/bmw2/bwrm/components/BwrmEnvController/libCcspCommonLibrary_BwrmEnvController.la \
#	source/cosa/package/bmw2/bwrm/components/BwrmCookedPage/libCcspCommonLibrary_BwrmCookedPage.la \
#	source/cosa/package/bmw2/bwrm/components/BwrmPageManager/libCcspCommonLibrary_BwrmPageManager.la \
#	source/cosa/package/bmw2/bwrm/components/BwrmFileManager/libCcspCommonLibrary_BwrmFileManager.la \
#	source/cosa/package/system/components/SysRroUint/libCcspCommonLibrary_SysRroUint.la \
#	source/cosa/package/system/components/SysRepositoryRecord/libCcspCommonLibrary_SysRepositoryRecord.la \
#	source/cosa/package/system/components/SysRroBstr/libCcspCommonLibrary_SysRroBstr.la \
#	source/cosa/package/system/components/SysRepositoryDriver/libCcspCommonLibrary_SysRepositoryDriver.la \
#	source/cosa/package/system/components/SysRroSint/libCcspCommonLibrary_SysRroSint.la \
#	source/cosa/package/system/components/SysRroHcxt/libCcspCommonLibrary_SysRroHcxt.la \
#	source/cosa/package/system/components/SysRroEnum/libCcspCommonLibrary_SysRroEnum.la \
#	source/cosa/package/system/components/SysSetupManager/libCcspCommonLibrary_SysSetupManager.la \
#	source/cosa/package/system/components/SysInfoRepository/libCcspCommonLibrary_SysInfoRepository.la \
#	source/cosa/package/system/components/SysIroV2/libCcspCommonLibrary_SysIroV2.la \
#	source/cosa/package/system/components/SysRecycleBin/libCcspCommonLibrary_SysRecycleBin.la \
#	source/cosa/package/system/components/SysRroAstr/libCcspCommonLibrary_SysRroAstr.la \
#	source/cosa/package/system/components/SysRroBool/libCcspCommonLibrary_SysRroBool.la \
#	source/cosa/package/system/components/SysCtlvProcessor/libCcspCommonLibrary_SysCtlvProcessor.la \
#	source/cosa/package/system/components/SysRepositoryFolder/libCcspCommonLibrary_SysRepositoryFolder.la \
#	source/cosa/package/slap/services/dslh/SlapDslhParamTree/libCcspCommonLibrary_SlapDslhParamTree.la \
#	source/cosa/package/slap/services/bmc2/SlapBmc2Server/libCcspCommonLibrary_SlapBmc2Server.la \
#	source/cosa/package/slap/services/bmc2/SlapBmc2Terminal/libCcspCommonLibrary_SlapBmc2Terminal.la \
#	source/cosa/package/slap/services/bmc2/SlapBmc2Output/libCcspCommonLibrary_SlapBmc2Output.la \
#	source/cosa/package/slap/services/bmc2/SlapBmc2Input/libCcspCommonLibrary_SlapBmc2Input.la \
#	source/cosa/package/slap/services/bmc2/SlapBmc2Command/libCcspCommonLibrary_SlapBmc2Command.la \
#	source/cosa/package/slap/services/bmc2/SlapBmc2Domain/libCcspCommonLibrary_SlapBmc2Domain.la \
#	source/cosa/package/slap/services/bmc2/SlapBmc2Application/libCcspCommonLibrary_SlapBmc2Application.la \
#	source/cosa/package/cli/components/ScliShell/libCcspCommonLibrary_ScliShell.la \
#	source/cosa/package/bmc2/components/Bmc2EnvController/libCcspCommonLibrary_Bmc2EnvController.la \
#	source/cosa/package/bmc2/components/Bmc2ReqController/libCcspCommonLibrary_Bmc2ReqController.la \
#	source/cosa/package/bmc2/components/Bmc2ComExecutor/libCcspCommonLibrary_Bmc2ComExecutor.la \
#	source/cosa/package/bmc2/components/Bmc2ComTerminal/libCcspCommonLibrary_Bmc2ComTerminal.la \
#	source/cosa/package/bmc2/components/Bmc2ComDomain/libCcspCommonLibrary_Bmc2ComDomain.la \
#	source/debug_api/libCcspCommonLibrary_debug_api.la \
#	source/ccsp/components/common/PoamIrepFolder/libCcspCommonLibrary_PoamIrepFolder.la \
#	source/ccsp/components/common/DataModel/dml/components/DslhVarEntity/libCcspCommonLibrary_DslhVarEntity.la \
#	source/ccsp/components/common/DataModel/dml/components/DslhObjController/libCcspCommonLibrary_DslhObjController.la \
#	source/ccsp/components/common/DataModel/dml/components/DslhObjRecord/libCcspCommonLibrary_DslhObjRecord.la \
#	source/ccsp/components/common/DataModel/dml/components/DslhVarRecord/libCcspCommonLibrary_DslhVarRecord.la \
#	source/ccsp/components/common/DataModel/dml/components/DslhWmpDatabase/libCcspCommonLibrary_DslhWmpDatabase.la \
#	source/ccsp/components/common/DataModel/dml/components/DslhDataModelAgent/libCcspCommonLibrary_DslhDataModelAgent.la \
#	source/ccsp/components/common/DataModel/dml/components/DslhCpeController/libCcspCommonLibrary_DslhCpeController.la \
#	source/ccsp/components/common/DataModel/dml/components/DslhObjEntity/libCcspCommonLibrary_DslhObjEntity.la \
#	source/ccsp/components/common/DataModel/dml/components/DslhObjcoTable/libCcspCommonLibrary_DslhObjcoTable.la \
#	source/ccsp/components/common/MessageBusHelper/helper/libCcspCommonLibrary_helper.la \
#	source/ccsp/components/CCSP_NamespaceMgr/libCcspCommonLibrary_CCSP_NamespaceMgr.la \
#	source/util_api/tls/containers/TlsModuleContainer/libCcspCommonLibrary_TlsModuleContainer.la \
#	source/util_api/tls/components/TlsRecordKeeper/libCcspCommonLibrary_TlsRecordKeeper.la \
#	source/util_api/tls/components/TlsCryptoProvider/libCcspCommonLibrary_TlsCryptoProvider.la \
#	source/util_api/tls/components/TlsEnvController/libCcspCommonLibrary_TlsEnvController.la \
#	source/util_api/tls/components/TlsSessionManager/libCcspCommonLibrary_TlsSessionManager.la \
#	source/util_api/tls/components/TlsRecordClient/libCcspCommonLibrary_TlsRecordClient.la \
#	source/util_api/tls/components/TlsHsoClient/libCcspCommonLibrary_TlsHsoClient.la \
#	source/util_api/tls/components/TlsHsoServer/libCcspCommonLibrary_TlsHsoServer.la \
#	source/util_api/tls/components/TlsSessionEntry/libCcspCommonLibrary_TlsSessionEntry.la \
#	source/util_api/tls/components/TlsAppConnector/libCcspCommonLibrary_TlsAppConnector.la \
#	source/util_api/tls/components/TlsHandShaker/libCcspCommonLibrary_TlsHandShaker.la \
#	source/util_api/tls/components/TlsCipherSwitch/libCcspCommonLibrary_TlsCipherSwitch.la \
#	source/util_api/tls/components/TlsAlertHandler/libCcspCommonLibrary_TlsAlertHandler.la \
#	source/util_api/tls/components/TlsConnController/libCcspCommonLibrary_TlsConnController.la \
#	source/util_api/ccsp_msg_bus/libCcspCommonLibrary_ccsp_msg_bus.la \
#	source/util_api/slap/components/SlapObjBroker/libCcspCommonLibrary_SlapObjBroker.la \
#	source/util_api/slap/components/SlapObjRecord/libCcspCommonLibrary_SlapObjRecord.la \
#	source/util_api/slap/components/SlapVarHelper/libCcspCommonLibrary_SlapVarHelper.la \
#	source/util_api/slap/components/SlapScoGallery/libCcspCommonLibrary_SlapScoGallery.la \
#	source/util_api/slap/components/SlapLoamServer/libCcspCommonLibrary_SlapLoamServer.la \
#	source/util_api/slap/components/SlapObjContainer/libCcspCommonLibrary_SlapObjContainer.la \
#	source/util_api/slap/components/SlapLoamClient/libCcspCommonLibrary_SlapLoamClient.la \
#	source/util_api/slap/components/SlapVarConverter/libCcspCommonLibrary_SlapVarConverter.la \
#	source/util_api/slap/components/SlapVarEntity/libCcspCommonLibrary_SlapVarEntity.la \
#	source/util_api/slap/components/SlapVarMapper/libCcspCommonLibrary_SlapVarMapper.la \
#	source/util_api/slap/components/SlapEnvController/libCcspCommonLibrary_SlapEnvController.la \
#	source/util_api/slap/components/SlapOwoUoac/libCcspCommonLibrary_SlapOwoUoac.la \
#	source/util_api/slap/components/SlapObjAgent/libCcspCommonLibrary_SlapObjAgent.la \
#	source/util_api/slap/components/SlapAccessManager/libCcspCommonLibrary_SlapAccessManager.la \
#	source/util_api/slap/components/SlapSrvComponent/libCcspCommonLibrary_SlapSrvComponent.la \
#	source/util_api/slap/components/SlapLoamBroker/libCcspCommonLibrary_SlapLoamBroker.la \
#	source/util_api/slap/components/SlapObjWrapper/libCcspCommonLibrary_SlapObjWrapper.la \
#	source/util_api/slap/components/SlapObjMapper/libCcspCommonLibrary_SlapObjMapper.la \
#	source/util_api/slap/components/SlapOwoLoao/libCcspCommonLibrary_SlapOwoLoao.la \
#	source/util_api/slap/components/SlapObjProxy/libCcspCommonLibrary_SlapObjProxy.la \
#	source/util_api/slap/components/SlapSrvProxy/libCcspCommonLibrary_SlapSrvProxy.la \
#	source/util_api/slap/components/SlapOwoLoac/libCcspCommonLibrary_SlapOwoLoac.la \
#	source/util_api/slap/components/SlapOwoUoao/libCcspCommonLibrary_SlapOwoUoao.la \
#	source/util_api/slap/components/SlapObjEntity/libCcspCommonLibrary_SlapObjEntity.la \
#	source/util_api/slap/components/SlapScoCollection/libCcspCommonLibrary_SlapScoCollection.la \
#	source/util_api/slap/components/SlapScoStandard/libCcspCommonLibrary_SlapScoStandard.la \
#	source/util_api/slap/components/SlapScoBuffer/libCcspCommonLibrary_SlapScoBuffer.la \
#	source/util_api/ccsp_dm_api/libCcspCommonLibrary_ccsp_dm_api.la \
#	source/util_api/web/containers/WebModuleContainer/libCcspCommonLibrary_WebModuleContainer.la \
#	source/util_api/web/components/WebResourceLocator/libCcspCommonLibrary_WebResourceLocator.la \
#	source/util_api/web/components/WebVirtualHost/libCcspCommonLibrary_WebVirtualHost.la \
#	source/util_api/web/components/WebStatusAtomTable/libCcspCommonLibrary_WebStatusAtomTable.la \
#	source/util_api/web/components/WebSimpleServer/libCcspCommonLibrary_WebSimpleServer.la \
#	source/util_api/web/components/WebAuthServer/libCcspCommonLibrary_WebAuthServer.la \
#	source/util_api/web/components/WebResourceOwner/libCcspCommonLibrary_WebResourceOwner.la \
#	source/util_api/web/components/WebUriPath/libCcspCommonLibrary_WebUriPath.la \
#	source/util_api/web/components/WebRooFsm/libCcspCommonLibrary_WebRooFsm.la \
#	source/util_api/web/components/WebGeneralSession/libCcspCommonLibrary_WebGeneralSession.la \
#	source/util_api/ansc/AnscLightComponent/libCcspCommonLibrary_AnscLightComponent.la \
#	source/util_api/ansc/AnscSimpleSystemTime/libCcspCommonLibrary_AnscSimpleSystemTime.la \
#	source/util_api/ansc/AnscTimerScheduler/libCcspCommonLibrary_AnscTimerScheduler.la \
#	source/util_api/ansc/AnscObjectMapper/libCcspCommonLibrary_AnscObjectMapper.la \
#	source/util_api/ansc/AnscBrokerEngineTcp/libCcspCommonLibrary_AnscBrokerEngineTcp.la \
#	source/util_api/ansc/AnscSimpleClientTcp/libCcspCommonLibrary_AnscSimpleClientTcp.la \
#	source/util_api/ansc/AnscCryptoPubSSLeay/libCcspCommonLibrary_AnscCryptoPubSSLeay.la \
#	source/util_api/ansc/AnscDaemonServerUdp/libCcspCommonLibrary_AnscDaemonServerUdp.la \
#	source/util_api/ansc/AnscCoVer3/libCcspCommonLibrary_AnscCoVer3.la \
#	source/util_api/ansc/AnscXmlDomParser/libCcspCommonLibrary_AnscXmlDomParser.la \
#	source/util_api/ansc/AnscXsocket/libCcspCommonLibrary_AnscXsocket.la \
#	source/util_api/ansc/AnscQueryInterface/libCcspCommonLibrary_AnscQueryInterface.la \
#	source/util_api/ansc/AnscObjectContainer/libCcspCommonLibrary_AnscObjectContainer.la \
#	source/util_api/ansc/AnscLpcConnector/libCcspCommonLibrary_AnscLpcConnector.la \
#	source/util_api/ansc/AnscUtilBox/libCcspCommonLibrary_AnscUtilBox.la \
#	source/util_api/ansc/AnscModuleContainer/libCcspCommonLibrary_AnscModuleContainer.la \
#	source/util_api/ansc/AnscDaemonServerTcp/libCcspCommonLibrary_AnscDaemonServerTcp.la \
#	source/util_api/ansc/AnscCryptoPub/libCcspCommonLibrary_AnscCryptoPub.la \
#	source/util_api/ansc/AnscBrokerSocketTcp/libCcspCommonLibrary_AnscBrokerSocketTcp.la \
#	source/util_api/ansc/AnscPlatform/libCcspCommonLibrary_AnscPlatform.la \
#	source/util_api/ansc/AnscString/libCcspCommonLibrary_AnscString.la \
#	source/util_api/ansc/AnscDaemonEngineUdp/libCcspCommonLibrary_AnscDaemonEngineUdp.la \
#	source/util_api/ansc/AnscComponentObject/libCcspCommonLibrary_AnscComponentObject.la \
#	source/util_api/ansc/AnscDaemonEngineTcp/libCcspCommonLibrary_AnscDaemonEngineTcp.la \
#	source/util_api/ansc/AnscBrokerServerTcp/libCcspCommonLibrary_AnscBrokerServerTcp.la \
#	source/util_api/ansc/AnscDaemonSocketUdp/libCcspCommonLibrary_AnscDaemonSocketUdp.la \
#	source/util_api/ansc/AnscAtomTable/libCcspCommonLibrary_AnscAtomTable.la \
#	source/util_api/ansc/AnscCrypto/libCcspCommonLibrary_AnscCrypto.la \
#	source/util_api/ansc/AnscSimpleClientUdp/libCcspCommonLibrary_AnscSimpleClientUdp.la \
#	source/util_api/ansc/AnscDaemonSocketTcp/libCcspCommonLibrary_AnscDaemonSocketTcp.la \
#	source/util_api/ansc/AnscStringUtil/libCcspCommonLibrary_AnscStringUtil.la \
#	source/util_api/ansc/AnscLpccoTcp/libCcspCommonLibrary_AnscLpccoTcp.la \
#	source/util_api/stun/components/StunSimpleClient/libCcspCommonLibrary_StunSimpleClient.la \
#	source/util_api/http/containers/HttpHelperContainer/libCcspCommonLibrary_HttpHelperContainer.la \
#	source/util_api/http/components/HttpSimpleServer/libCcspCommonLibrary_HttpSimpleServer.la \
#	source/util_api/http/components/HttpAuth/libCcspCommonLibrary_HttpAuth.la \
#	source/util_api/http/components/HttpAtoStatus/libCcspCommonLibrary_HttpAtoStatus.la \
#	source/util_api/http/components/HttpMboChunked/libCcspCommonLibrary_HttpMboChunked.la \
#	source/util_api/http/components/HttpMessageBody/libCcspCommonLibrary_HttpMessageBody.la \
#	source/util_api/http/components/HttpTransRecord/libCcspCommonLibrary_HttpTransRecord.la \
#	source/util_api/http/components/HttpBmoRep/libCcspCommonLibrary_HttpBmoRep.la \
#	source/util_api/http/components/HttpAuthClient/libCcspCommonLibrary_HttpAuthClient.la \
#	source/util_api/http/components/HttpAtoFxCat2/libCcspCommonLibrary_HttpAtoFxCat2.la \
#	source/util_api/http/components/HttpAtoHeader/libCcspCommonLibrary_HttpAtoHeader.la \
#	source/util_api/http/components/HttpSimpleClient/libCcspCommonLibrary_HttpSimpleClient.la \
#	source/util_api/http/components/HttpMboIdentity/libCcspCommonLibrary_HttpMboIdentity.la \
#	source/util_api/http/components/HttpWebsTrans/libCcspCommonLibrary_HttpWebsTrans.la \
#	source/util_api/http/components/HttpWebsSession/libCcspCommonLibrary_HttpWebsSession.la \
#	source/util_api/http/components/HttpAtoFxCat1/libCcspCommonLibrary_HttpAtoFxCat1.la \
#	source/util_api/http/components/HttpMboFile/libCcspCommonLibrary_HttpMboFile.la \
#	source/util_api/http/components/HttpWebcTrans/libCcspCommonLibrary_HttpWebcTrans.la \
#	source/util_api/http/components/HttpSimpleProxy/libCcspCommonLibrary_HttpSimpleProxy.la \
#	source/util_api/http/components/HttpAtoMethod/libCcspCommonLibrary_HttpAtoMethod.la \
#	source/util_api/http/components/HttpAtoCgiEnv/libCcspCommonLibrary_HttpAtoCgiEnv.la \
#	source/util_api/http/components/HttpPsoVer2/libCcspCommonLibrary_HttpPsoVer2.la \
#	source/util_api/http/components/HttpBmoReq/libCcspCommonLibrary_HttpBmoReq.la \
#	source/util_api/http/components/HttpAdvancedProxy/libCcspCommonLibrary_HttpAdvancedProxy.la \
#	source/util_api/http/components/HttpWebcSession/libCcspCommonLibrary_HttpWebcSession.la \
#	source/util_api/http/components/HttpFormContent/libCcspCommonLibrary_HttpFormContent.la \
#	source/util_api/http/components/HttpProxySession/libCcspCommonLibrary_HttpProxySession.la \
#	source/util_api/http/components/HttpBasicMessage/libCcspCommonLibrary_HttpBasicMessage.la \
#	source/util_api/http/utilities/HttpSimpleMsgParser/libCcspCommonLibrary_HttpSimpleMsgParser.la \
#	source/util_api/asn.1/components/al_pki/libCcspCommonLibrary_al_pki.la \
#	source/util_api/asn.1/components/asn1_x509/libCcspCommonLibrary_asn1_x509.la \
#	source/util_api/asn.1/components/asn1_basic/libCcspCommonLibrary_asn1_basic.la \
#	source/util_api/asn.1/components/al_pkcs12/libCcspCommonLibrary_al_pkcs12.la \

AC_OUTPUT

