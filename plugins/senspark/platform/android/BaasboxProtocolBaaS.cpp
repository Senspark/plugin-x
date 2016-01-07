#include "BaasboxProtocolBaaS.h"
#include "PluginUtils.h"
#include "PluginJniHelper.h"
#include "PluginJavaData.h"
#include <string>

USING_NS_SENSPARK_PLUGIN_BAAS;
using namespace cocos2d;
using namespace cocos2d::plugin;
using namespace std;

BaasboxProtocolBaaS::~BaasboxProtocolBaaS(){
	PluginUtils::erasePluginJavaData(this);
}

void BaasboxProtocolBaaS::loginWithFacebookToken(const std::string &facebookToken, BaaSCallback &cb){

	PluginUtils::outputLog("BaasboxProtocolBaaS", "loginWithFacebookToken with facebookToken %s", facebookToken.c_str());

	PluginJavaData* pData = PluginUtils::getPluginJavaData(this);
	PluginJniMethodInfo t;

	if (PluginJniHelper::getMethodInfo(t,
			pData->jclassName.c_str(),
			"loginWithFacebookToken",
			"(Ljava/lang/String;I)V")){
		jstring strFacebookToken = t.env->NewStringUTF(facebookToken.c_str());

		CallbackWrapper* wrapper = new CallbackWrapper(cb);
		PluginUtils::outputLog("BaasboxProtocolBaaS", "callback id %d", (long)wrapper);
		// invoke java method
		t.env->CallVoidMethod(pData->jobj, t.methodID, strFacebookToken, (long) wrapper);
		t.env->DeleteLocalRef(strFacebookToken);
		t.env->DeleteLocalRef(t.classID);
	}

}

void BaasboxProtocolBaaS::updateUserProfile(const std::string &profile, BaaSCallback &cb){

	PluginUtils::outputLog("BaasboxProtocol", "updateUserProfile with profile %s ", profile.c_str());
	PluginJavaData* pData = PluginUtils::getPluginJavaData(this);
	PluginJniMethodInfo t;
	if (PluginJniHelper::getMethodInfo(
			t
			, pData->jclassName.c_str()
			, "updateUserProfile"
			, "(Ljava/lang/String;I)V"))
	{
		jstring strProfile = t.env->NewStringUTF(profile.c_str());
		CallbackWrapper* wrapper = new CallbackWrapper(cb);

		// invoke java method
		t.env->CallVoidMethod(pData->jobj, t.methodID, strProfile, (long) wrapper);
		t.env->DeleteLocalRef(strProfile);
		t.env->DeleteLocalRef(t.classID);
	}
}

void BaasboxProtocolBaaS::fetchUserProfile(BaaSCallback &cb){
	CallbackWrapper* wrapper = new CallbackWrapper(cb);
	PluginUtils::callJavaFunctionWithName_oneParam(this, "fetchUserProfile", "(I)V", (long) wrapper);
}

void BaasboxProtocolBaaS::fetchScoresFriendsFacebookWithPlayers(const std::string& players, BaaSCallback &cb){

	PluginUtils::outputLog("BaasboxProtocol", "fetchScoresFriendsFacebookWithPlayers with plaers %s .", players.c_str());
	PluginJavaData* pData = PluginUtils::getPluginJavaData(this);
	PluginJniMethodInfo t;
	if (PluginJniHelper::getMethodInfo(
			t
			, pData->jclassName.c_str()
			, "fetchScoresFriendsFacebookWithPlayers"
			, "(Ljava/lang/String;I)V"))
	{
		jstring strPlayers = t.env->NewStringUTF(players.c_str());
		CallbackWrapper* wrapper = new CallbackWrapper(cb);

		// invoke java method
		t.env->CallVoidMethod(pData->jobj, t.methodID, strPlayers, (long)wrapper);
		t.env->DeleteLocalRef(strPlayers);
		t.env->DeleteLocalRef(t.classID);
	}
}
