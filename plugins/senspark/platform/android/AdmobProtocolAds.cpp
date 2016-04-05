#include "AdmobProtocolAds.h"
#include "PluginUtils.h"
#include "PluginJniHelper.h"
#include <sstream>

USING_NS_SENSPARK_PLUGIN_ADS;
using namespace cocos2d::plugin;
using namespace cocos2d;

std::string to_string(int input) {
    std::stringstream ss;
    ss << input;
    std::string ret(ss.str());
    return ret;
}

AdmobProtocolAds::AdmobProtocolAds() {

}

AdmobProtocolAds::~AdmobProtocolAds() {
    PluginUtils::erasePluginJavaData(this);
}

void AdmobProtocolAds::configureAds(const std::string &adsId, const std::string& appPublicKey) {
    TAdsInfo devInfo;
    devInfo["AdmobID"] = adsId;
    devInfo["AppPublicKey"] = appPublicKey;
    configDeveloperInfo(devInfo);
}

void AdmobProtocolAds::addTestDevice(const std::string &deviceId) {
    PluginParam deviceIdParam(deviceId.c_str());
    callFuncWithParam("addTestDevice", &deviceIdParam, nullptr);
}

void AdmobProtocolAds::loadInterstitial() {
    callFuncWithParam("loadInterstitial", nullptr);
}

bool AdmobProtocolAds::hasInterstitial() {
    return callBoolFuncWithParam("hasInterstitial", nullptr);
}

void AdmobProtocolAds::slideBannerUp() {
    callFuncWithParam("slideBannerUp", nullptr);
}

void AdmobProtocolAds::slideBannerDown() {
    callFuncWithParam("slideBannerDown", nullptr);
}
