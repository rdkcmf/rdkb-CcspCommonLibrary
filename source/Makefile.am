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
SUBDIRS = cosa debug_api ccsp util_api dm_pack

if WITH_GTEST_SUPPORT
SUBDIRS += test
endif

lib_LTLIBRARIES = libccsp_common.la

libccsp_common_la_CPPFLAGS = \
	-D_ANSC_AES_USED_ \
	-D_ANSC_IPV6_COMPATIBLE_ \
	-D_ANSC_USE_OPENSSL_ \
	-D_BBHM_SSE_FILE_IO \
	-DBUILD_WEB \
	-D_CCSP_CWMP_TCP_CONNREQ_HANDLER \
	-D_DSLH_STUN_ \
	-DENABLE_SA_KEY \
	-D_NO_PKI_KB5_SUPPORT \
	-I$(top_srcdir)/cosa/include/linux \
	-I$(top_srcdir)/debug_api/include \
	-I$(top_srcdir)/cosa/include \
	-I$(top_srcdir)/cosa/utilities/include \
	-I$(top_srcdir)/cosa/custom \
	-I$(top_srcdir)/cosa/package/bmw2/bwsp/include \
	-I$(top_srcdir)/cosa/package/bmw2/include \
	-I$(top_srcdir)/cosa/package/bmw2/bree/include \
	-I$(top_srcdir)/cosa/package/bmw2/beep/include \
	-I$(top_srcdir)/cosa/package/bmw2/bwrm/include \
	-I$(top_srcdir)/cosa/package/system/include \
	-I$(top_srcdir)/cosa/package/slap/include \
	-I$(top_srcdir)/cosa/package/slap/services/dslh/include \
	-I$(top_srcdir)/cosa/package/slap/services/bmc2/include \
	-I$(top_srcdir)/cosa/package/cli/include \
	-I$(top_srcdir)/cosa/package/bmc2/include \
	-I$(top_srcdir)/ccsp/include \
	-I$(top_srcdir)/ccsp/components/include \
	-I$(top_srcdir)/ccsp/components/common/PoamIrepFolder \
	-I$(top_srcdir)/ccsp/components/common/MessageBusHelper/include \
	-I$(top_srcdir)/ccsp/custom \
	-I$(top_srcdir)/util_api/tls/include \
	-I$(top_srcdir)/util_api/web/include \
	-I$(top_srcdir)/util_api/ansc/include \
	-I$(top_srcdir)/util_api/stun/include \
	-I$(top_srcdir)/util_api/http/include \
	-I$(top_srcdir)/util_api/http/utilities/include \
	-I$(top_srcdir)/util_api/asn.1/include

libccsp_common_la_LDFLAGS = -shared -fPIC -lssl -lcrypto -lrbus
libccsp_common_la_SOURCES =
libccsp_common_la_DEPENDENCIES = \
	cosa/utilities/AnscObjectArray/AnscByteArray/libCcspCommonLibrary_AnscByteArray.la \
	cosa/utilities/AnscObjectArray/AnscUIntArray/libCcspCommonLibrary_AnscUIntArray.la \
	cosa/utilities/AnscObjectArray/AnscPtrArray/libCcspCommonLibrary_AnscPtrArray.la \
	cosa/utilities/AnscObjectArray/AnscStringArray/libCcspCommonLibrary_AnscStringArray.la \
	cosa/utilities/AnscObjectArray/ObjectArrayBase/libCcspCommonLibrary_ObjectArrayBase.la \
	cosa/utilities/bsp_eng/libCcspCommonLibrary_bsp_eng.la \
	cosa/package/bmw2/bwsp/components/BwspPortalHost/libCcspCommonLibrary_BwspPortalHost.la \
	cosa/package/bmw2/bwsp/components/BwspWebServlet/libCcspCommonLibrary_BwspWebServlet.la \
	cosa/package/bmw2/bwsp/components/BwspSiteCreator/libCcspCommonLibrary_BwspSiteCreator.la \
	cosa/package/bmw2/bree/components/BreeSpo/libCcspCommonLibrary_BreeSpo.la \
	cosa/package/bmw2/bree/components/BreeSrm/libCcspCommonLibrary_BreeSrm.la \
	cosa/package/bmw2/bwrm/components/BwrmEnvController/libCcspCommonLibrary_BwrmEnvController.la \
	cosa/package/bmw2/bwrm/components/BwrmCookedPage/libCcspCommonLibrary_BwrmCookedPage.la \
	cosa/package/bmw2/bwrm/components/BwrmPageManager/libCcspCommonLibrary_BwrmPageManager.la \
	cosa/package/bmw2/bwrm/components/BwrmFileManager/libCcspCommonLibrary_BwrmFileManager.la \
	cosa/package/system/components/SysRroUint/libCcspCommonLibrary_SysRroUint.la \
	cosa/package/system/components/SysRepositoryRecord/libCcspCommonLibrary_SysRepositoryRecord.la \
	cosa/package/system/components/SysRroBstr/libCcspCommonLibrary_SysRroBstr.la \
	cosa/package/system/components/SysRepositoryDriver/libCcspCommonLibrary_SysRepositoryDriver.la \
	cosa/package/system/components/SysRroSint/libCcspCommonLibrary_SysRroSint.la \
	cosa/package/system/components/SysRroHcxt/libCcspCommonLibrary_SysRroHcxt.la \
	cosa/package/system/components/SysRroEnum/libCcspCommonLibrary_SysRroEnum.la \
	cosa/package/system/components/SysInfoRepository/libCcspCommonLibrary_SysInfoRepository.la \
	cosa/package/system/components/SysRecycleBin/libCcspCommonLibrary_SysRecycleBin.la \
	cosa/package/system/components/SysRroAstr/libCcspCommonLibrary_SysRroAstr.la \
	cosa/package/system/components/SysRroBool/libCcspCommonLibrary_SysRroBool.la \
	cosa/package/system/components/SysRepositoryFolder/libCcspCommonLibrary_SysRepositoryFolder.la \
	cosa/package/slap/services/dslh/SlapDslhParamTree/libCcspCommonLibrary_SlapDslhParamTree.la \
	cosa/package/slap/services/bmc2/SlapBmc2Server/libCcspCommonLibrary_SlapBmc2Server.la \
	cosa/package/slap/services/bmc2/SlapBmc2Terminal/libCcspCommonLibrary_SlapBmc2Terminal.la \
	cosa/package/slap/services/bmc2/SlapBmc2Output/libCcspCommonLibrary_SlapBmc2Output.la \
	cosa/package/slap/services/bmc2/SlapBmc2Input/libCcspCommonLibrary_SlapBmc2Input.la \
	cosa/package/slap/services/bmc2/SlapBmc2Command/libCcspCommonLibrary_SlapBmc2Command.la \
	cosa/package/slap/services/bmc2/SlapBmc2Domain/libCcspCommonLibrary_SlapBmc2Domain.la \
	cosa/package/slap/services/bmc2/SlapBmc2Application/libCcspCommonLibrary_SlapBmc2Application.la \
	cosa/package/cli/components/ScliShell/libCcspCommonLibrary_ScliShell.la \
	cosa/package/bmc2/components/Bmc2EnvController/libCcspCommonLibrary_Bmc2EnvController.la \
	cosa/package/bmc2/components/Bmc2ReqController/libCcspCommonLibrary_Bmc2ReqController.la \
	cosa/package/bmc2/components/Bmc2ComExecutor/libCcspCommonLibrary_Bmc2ComExecutor.la \
	cosa/package/bmc2/components/Bmc2ComTerminal/libCcspCommonLibrary_Bmc2ComTerminal.la \
	cosa/package/bmc2/components/Bmc2ComDomain/libCcspCommonLibrary_Bmc2ComDomain.la \
	debug_api/libCcspCommonLibrary_debug_api.la \
	ccsp/components/common/PoamIrepFolder/libCcspCommonLibrary_PoamIrepFolder.la \
	ccsp/components/common/DataModel/dml/components/DslhVarEntity/libCcspCommonLibrary_DslhVarEntity.la \
	ccsp/components/common/DataModel/dml/components/DslhObjController/libCcspCommonLibrary_DslhObjController.la \
	ccsp/components/common/DataModel/dml/components/DslhObjRecord/libCcspCommonLibrary_DslhObjRecord.la \
	ccsp/components/common/DataModel/dml/components/DslhVarRecord/libCcspCommonLibrary_DslhVarRecord.la \
	ccsp/components/common/DataModel/dml/components/DslhWmpDatabase/libCcspCommonLibrary_DslhWmpDatabase.la \
	ccsp/components/common/DataModel/dml/components/DslhDataModelAgent/libCcspCommonLibrary_DslhDataModelAgent.la \
	ccsp/components/common/DataModel/dml/components/DslhCpeController/libCcspCommonLibrary_DslhCpeController.la \
	ccsp/components/common/DataModel/dml/components/DslhObjEntity/libCcspCommonLibrary_DslhObjEntity.la \
	ccsp/components/common/DataModel/dml/components/DslhObjcoTable/libCcspCommonLibrary_DslhObjcoTable.la \
	ccsp/components/common/MessageBusHelper/helper/libCcspCommonLibrary_helper.la \
	ccsp/components/CCSP_NamespaceMgr/libCcspCommonLibrary_CCSP_NamespaceMgr.la \
        ccsp/components/CCSP_AliasMgr/libCcspCommonLibrary_CCSP_AliasMgr.la \
	util_api/tls/containers/TlsModuleContainer/libCcspCommonLibrary_TlsModuleContainer.la \
	util_api/tls/components/TlsRecordKeeper/libCcspCommonLibrary_TlsRecordKeeper.la \
	util_api/tls/components/TlsCryptoProvider/libCcspCommonLibrary_TlsCryptoProvider.la \
	util_api/tls/components/TlsEnvController/libCcspCommonLibrary_TlsEnvController.la \
	util_api/tls/components/TlsSessionManager/libCcspCommonLibrary_TlsSessionManager.la \
	util_api/tls/components/TlsRecordClient/libCcspCommonLibrary_TlsRecordClient.la \
	util_api/tls/components/TlsHsoClient/libCcspCommonLibrary_TlsHsoClient.la \
	util_api/tls/components/TlsHsoServer/libCcspCommonLibrary_TlsHsoServer.la \
	util_api/tls/components/TlsSessionEntry/libCcspCommonLibrary_TlsSessionEntry.la \
	util_api/tls/components/TlsAppConnector/libCcspCommonLibrary_TlsAppConnector.la \
	util_api/tls/components/TlsHandShaker/libCcspCommonLibrary_TlsHandShaker.la \
	util_api/tls/components/TlsCipherSwitch/libCcspCommonLibrary_TlsCipherSwitch.la \
	util_api/tls/components/TlsAlertHandler/libCcspCommonLibrary_TlsAlertHandler.la \
	util_api/tls/components/TlsConnController/libCcspCommonLibrary_TlsConnController.la \
	util_api/ccsp_msg_bus/libCcspCommonLibrary_ccsp_msg_bus.la \
	util_api/slap/components/SlapObjBroker/libCcspCommonLibrary_SlapObjBroker.la \
	util_api/slap/components/SlapObjRecord/libCcspCommonLibrary_SlapObjRecord.la \
	util_api/slap/components/SlapVarHelper/libCcspCommonLibrary_SlapVarHelper.la \
	util_api/slap/components/SlapScoGallery/libCcspCommonLibrary_SlapScoGallery.la \
	util_api/slap/components/SlapLoamServer/libCcspCommonLibrary_SlapLoamServer.la \
	util_api/slap/components/SlapObjContainer/libCcspCommonLibrary_SlapObjContainer.la \
	util_api/slap/components/SlapLoamClient/libCcspCommonLibrary_SlapLoamClient.la \
	util_api/slap/components/SlapVarConverter/libCcspCommonLibrary_SlapVarConverter.la \
	util_api/slap/components/SlapVarEntity/libCcspCommonLibrary_SlapVarEntity.la \
	util_api/slap/components/SlapVarMapper/libCcspCommonLibrary_SlapVarMapper.la \
	util_api/slap/components/SlapEnvController/libCcspCommonLibrary_SlapEnvController.la \
	util_api/slap/components/SlapOwoUoac/libCcspCommonLibrary_SlapOwoUoac.la \
	util_api/slap/components/SlapObjAgent/libCcspCommonLibrary_SlapObjAgent.la \
	util_api/slap/components/SlapAccessManager/libCcspCommonLibrary_SlapAccessManager.la \
	util_api/slap/components/SlapSrvComponent/libCcspCommonLibrary_SlapSrvComponent.la \
	util_api/slap/components/SlapLoamBroker/libCcspCommonLibrary_SlapLoamBroker.la \
	util_api/slap/components/SlapObjWrapper/libCcspCommonLibrary_SlapObjWrapper.la \
	util_api/slap/components/SlapObjMapper/libCcspCommonLibrary_SlapObjMapper.la \
	util_api/slap/components/SlapOwoLoao/libCcspCommonLibrary_SlapOwoLoao.la \
	util_api/slap/components/SlapObjProxy/libCcspCommonLibrary_SlapObjProxy.la \
	util_api/slap/components/SlapSrvProxy/libCcspCommonLibrary_SlapSrvProxy.la \
	util_api/slap/components/SlapOwoLoac/libCcspCommonLibrary_SlapOwoLoac.la \
	util_api/slap/components/SlapOwoUoao/libCcspCommonLibrary_SlapOwoUoao.la \
	util_api/slap/components/SlapObjEntity/libCcspCommonLibrary_SlapObjEntity.la \
	util_api/slap/components/SlapScoCollection/libCcspCommonLibrary_SlapScoCollection.la \
	util_api/slap/components/SlapScoStandard/libCcspCommonLibrary_SlapScoStandard.la \
	util_api/slap/components/SlapScoBuffer/libCcspCommonLibrary_SlapScoBuffer.la \
	util_api/ccsp_dm_api/libCcspCommonLibrary_ccsp_dm_api.la \
	util_api/web/containers/WebModuleContainer/libCcspCommonLibrary_WebModuleContainer.la \
	util_api/web/components/WebResourceLocator/libCcspCommonLibrary_WebResourceLocator.la \
	util_api/web/components/WebVirtualHost/libCcspCommonLibrary_WebVirtualHost.la \
	util_api/web/components/WebStatusAtomTable/libCcspCommonLibrary_WebStatusAtomTable.la \
	util_api/web/components/WebSimpleServer/libCcspCommonLibrary_WebSimpleServer.la \
	util_api/web/components/WebAuthServer/libCcspCommonLibrary_WebAuthServer.la \
	util_api/web/components/WebResourceOwner/libCcspCommonLibrary_WebResourceOwner.la \
	util_api/web/components/WebUriPath/libCcspCommonLibrary_WebUriPath.la \
	util_api/web/components/WebRooFsm/libCcspCommonLibrary_WebRooFsm.la \
	util_api/web/components/WebGeneralSession/libCcspCommonLibrary_WebGeneralSession.la \
	util_api/ansc/AnscLightComponent/libCcspCommonLibrary_AnscLightComponent.la \
	util_api/ansc/AnscSimpleSystemTime/libCcspCommonLibrary_AnscSimpleSystemTime.la \
	util_api/ansc/AnscTimerScheduler/libCcspCommonLibrary_AnscTimerScheduler.la \
	util_api/ansc/AnscObjectMapper/libCcspCommonLibrary_AnscObjectMapper.la \
	util_api/ansc/AnscBrokerEngineTcp/libCcspCommonLibrary_AnscBrokerEngineTcp.la \
	util_api/ansc/AnscSimpleClientTcp/libCcspCommonLibrary_AnscSimpleClientTcp.la \
	util_api/ansc/AnscCryptoPubSSLeay/libCcspCommonLibrary_AnscCryptoPubSSLeay.la \
	util_api/ansc/AnscDaemonServerUdp/libCcspCommonLibrary_AnscDaemonServerUdp.la \
	util_api/ansc/AnscCoVer3/libCcspCommonLibrary_AnscCoVer3.la \
	util_api/ansc/AnscXmlDomParser/libCcspCommonLibrary_AnscXmlDomParser.la \
	util_api/ansc/AnscXsocket/libCcspCommonLibrary_AnscXsocket.la \
	util_api/ansc/AnscQueryInterface/libCcspCommonLibrary_AnscQueryInterface.la \
	util_api/ansc/AnscObjectContainer/libCcspCommonLibrary_AnscObjectContainer.la \
	util_api/ansc/AnscLpcConnector/libCcspCommonLibrary_AnscLpcConnector.la \
	util_api/ansc/AnscUtilBox/libCcspCommonLibrary_AnscUtilBox.la \
	util_api/ansc/AnscModuleContainer/libCcspCommonLibrary_AnscModuleContainer.la \
	util_api/ansc/AnscDaemonServerTcp/libCcspCommonLibrary_AnscDaemonServerTcp.la \
	util_api/ansc/AnscCryptoPub/libCcspCommonLibrary_AnscCryptoPub.la \
	util_api/ansc/AnscBrokerSocketTcp/libCcspCommonLibrary_AnscBrokerSocketTcp.la \
	util_api/ansc/AnscPlatform/libCcspCommonLibrary_AnscPlatform.la \
	util_api/ansc/AnscString/libCcspCommonLibrary_AnscString.la \
	util_api/ansc/AnscDaemonEngineUdp/libCcspCommonLibrary_AnscDaemonEngineUdp.la \
	util_api/ansc/AnscComponentObject/libCcspCommonLibrary_AnscComponentObject.la \
	util_api/ansc/AnscDaemonEngineTcp/libCcspCommonLibrary_AnscDaemonEngineTcp.la \
	util_api/ansc/AnscBrokerServerTcp/libCcspCommonLibrary_AnscBrokerServerTcp.la \
	util_api/ansc/AnscDaemonSocketUdp/libCcspCommonLibrary_AnscDaemonSocketUdp.la \
	util_api/ansc/AnscAtomTable/libCcspCommonLibrary_AnscAtomTable.la \
	util_api/ansc/AnscCrypto/libCcspCommonLibrary_AnscCrypto.la \
	util_api/ansc/AnscSimpleClientUdp/libCcspCommonLibrary_AnscSimpleClientUdp.la \
	util_api/ansc/AnscDaemonSocketTcp/libCcspCommonLibrary_AnscDaemonSocketTcp.la \
	util_api/ansc/AnscStringUtil/libCcspCommonLibrary_AnscStringUtil.la \
	util_api/ansc/AnscLpccoTcp/libCcspCommonLibrary_AnscLpccoTcp.la \
	util_api/stun/components/StunSimpleClient/libCcspCommonLibrary_StunSimpleClient.la \
	util_api/http/containers/HttpHelperContainer/libCcspCommonLibrary_HttpHelperContainer.la \
	util_api/http/components/HttpSimpleServer/libCcspCommonLibrary_HttpSimpleServer.la \
	util_api/http/components/HttpAuth/libCcspCommonLibrary_HttpAuth.la \
	util_api/http/components/HttpAtoStatus/libCcspCommonLibrary_HttpAtoStatus.la \
	util_api/http/components/HttpMboChunked/libCcspCommonLibrary_HttpMboChunked.la \
	util_api/http/components/HttpMessageBody/libCcspCommonLibrary_HttpMessageBody.la \
	util_api/http/components/HttpTransRecord/libCcspCommonLibrary_HttpTransRecord.la \
	util_api/http/components/HttpBmoRep/libCcspCommonLibrary_HttpBmoRep.la \
	util_api/http/components/HttpAuthClient/libCcspCommonLibrary_HttpAuthClient.la \
	util_api/http/components/HttpAtoFxCat2/libCcspCommonLibrary_HttpAtoFxCat2.la \
	util_api/http/components/HttpAtoHeader/libCcspCommonLibrary_HttpAtoHeader.la \
	util_api/http/components/HttpSimpleClient/libCcspCommonLibrary_HttpSimpleClient.la \
	util_api/http/components/HttpMboIdentity/libCcspCommonLibrary_HttpMboIdentity.la \
	util_api/http/components/HttpWebsTrans/libCcspCommonLibrary_HttpWebsTrans.la \
	util_api/http/components/HttpWebsSession/libCcspCommonLibrary_HttpWebsSession.la \
	util_api/http/components/HttpAtoFxCat1/libCcspCommonLibrary_HttpAtoFxCat1.la \
	util_api/http/components/HttpMboFile/libCcspCommonLibrary_HttpMboFile.la \
	util_api/http/components/HttpWebcTrans/libCcspCommonLibrary_HttpWebcTrans.la \
	util_api/http/components/HttpSimpleProxy/libCcspCommonLibrary_HttpSimpleProxy.la \
	util_api/http/components/HttpAtoMethod/libCcspCommonLibrary_HttpAtoMethod.la \
	util_api/http/components/HttpAtoCgiEnv/libCcspCommonLibrary_HttpAtoCgiEnv.la \
	util_api/http/components/HttpPsoVer2/libCcspCommonLibrary_HttpPsoVer2.la \
	util_api/http/components/HttpBmoReq/libCcspCommonLibrary_HttpBmoReq.la \
	util_api/http/components/HttpAdvancedProxy/libCcspCommonLibrary_HttpAdvancedProxy.la \
	util_api/http/components/HttpWebcSession/libCcspCommonLibrary_HttpWebcSession.la \
	util_api/http/components/HttpFormContent/libCcspCommonLibrary_HttpFormContent.la \
	util_api/http/components/HttpProxySession/libCcspCommonLibrary_HttpProxySession.la \
	util_api/http/components/HttpBasicMessage/libCcspCommonLibrary_HttpBasicMessage.la \
	util_api/http/utilities/HttpSimpleMsgParser/libCcspCommonLibrary_HttpSimpleMsgParser.la \
	util_api/asn.1/components/al_pki/libCcspCommonLibrary_al_pki.la \
	util_api/asn.1/components/asn1_x509/libCcspCommonLibrary_asn1_x509.la \
	util_api/asn.1/components/asn1_basic/libCcspCommonLibrary_asn1_basic.la \
	util_api/asn.1/components/al_pkcs12/libCcspCommonLibrary_al_pkcs12.la \
	dm_pack/libCcspCommonLibrary_dm_pack.la


libccsp_common_la_LIBADD = $(libccsp_common_la_DEPENDENCIES)
