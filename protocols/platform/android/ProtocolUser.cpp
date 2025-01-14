/****************************************************************************
Copyright (c) 2013 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
#include "ProtocolUser.h"
#include "PluginJniHelper.h"
#include <android/log.h>
#include "PluginUtils.h"
#include "PluginJavaData.h"

namespace cocos2d { namespace plugin {

extern "C" {
JNIEXPORT void JNICALL Java_org_cocos2dx_plugin_UserWrapper_nativeOnActionResult(JNIEnv* env, jobject thiz, jstring className, jint ret, jstring msg)
{
	std::string strClassName = PluginJniHelper::jstring2string(env, className);
	std::string strMsg = PluginJniHelper::jstring2string(env, msg);
	PluginProtocol* pPlugin = PluginUtils::getPluginPtr(strClassName);
	PluginUtils::outputLog("ProtocolUser", "nativeOnActionResult(), Get plugin ptr : %p", pPlugin);
	if (pPlugin != NULL)
	{
		PluginUtils::outputLog("ProtocolUser", "nativeOnActionResult(), Get plugin name : %s", pPlugin->getPluginName());
		ProtocolUser* pUser = dynamic_cast<ProtocolUser*>(pPlugin);
		if (pUser != NULL)
		{
			ProtocolUser::UserCallback callback = pUser->getCallback();
			if (callback) {
				callback(ret, strMsg);
			} else {
				PluginUtils::outputLog("Listener of plugin %s not set correctly", pPlugin->getPluginName());
			}
		}
	}
}

JNIEXPORT void JNICALL Java_org_cocos2dx_plugin_UserWrapper_nativeOnGraphRequestResultFrom(JNIEnv* env, jobject thiz, jstring className, jint ret, jstring msg, jlong cbID)
{
	std::string strClassName = PluginJniHelper::jstring2string(className);
	std::string strMsg		 = PluginJniHelper::jstring2string(msg);
	PluginProtocol* pPlugin = PluginUtils::getPluginPtr(strClassName);
	if (pPlugin != NULL) {
		ProtocolUser* pUser = dynamic_cast<ProtocolUser*>(pPlugin);
		if (pUser != NULL && cbID) {
			ProtocolUser::CallbackWrapper* wrapper = (ProtocolUser::CallbackWrapper*) cbID;
			wrapper->fnPtr((int)ret, strMsg);
			delete wrapper;
		} else {
			PluginUtils::outputLog("ProtocolUser", "pUser null");
		}
	}
}

}

ProtocolUser::ProtocolUser()
{
}

ProtocolUser::~ProtocolUser()
{
}

void ProtocolUser::configDeveloperInfo(TUserInfo devInfo)
{
    if (devInfo.empty())
    {
        PluginUtils::outputLog("ProtocolUser", "The developer info is empty!");
        return;
    }
    else
    {
        PluginJavaData* pData = PluginUtils::getPluginJavaData(this);
    	PluginJniMethodInfo t;
        if (PluginJniHelper::getMethodInfo(t
    		, pData->jclassName.c_str()
    		, "configDeveloperInfo"
    		, "(Ljava/util/Hashtable;)V"))
    	{
        	// generate the hashtable from map
        	jobject obj_Map = PluginUtils::createJavaMapObject(&devInfo);

            // invoke java method
            t.env->CallVoidMethod(pData->jobj, t.methodID, obj_Map);
            t.env->DeleteLocalRef(obj_Map);
            t.env->DeleteLocalRef(t.classID);
        }
    }
}

void ProtocolUser::login()
{
    PluginUtils::callJavaFunctionWithName(this, "login");
}

void ProtocolUser::login(UserCallback& cb) {
	_callback = cb;
	ProtocolUser::login();
}

void ProtocolUser::logout()
{
	PluginUtils::callJavaFunctionWithName(this, "logout");
}

void ProtocolUser::logout(UserCallback& cb) {
	_callback = cb;
	ProtocolUser::logout();
}

bool ProtocolUser::isLoggedIn()
{
    return PluginUtils::callJavaBoolFuncWithName(this, "isLoggedIn");
}

std::string ProtocolUser::getSessionID()
{
    return PluginUtils::callJavaStringFuncWithName(this, "getSessionID");
}

std::string ProtocolUser::getAccessToken()
{
	return PluginUtils::callJavaStringFuncWithName(this, "getAccessToken");
}

std::string ProtocolUser::getUserID() {
	return PluginUtils::callJavaStringFuncWithName(this, "getUserID");
}

std::string ProtocolUser::getAvatarUrl() {
	return PluginUtils::callJavaStringFuncWithName(this, "getAvatarUrl");
}

std::string ProtocolUser::getDisplayName() {
	return PluginUtils::callJavaStringFuncWithName(this, "getDisplayName");
}



}} // namespace cocos2d { namespace plugin {

