//
//  AdmobProtocolAds.m
//  PluginSenspark
//
//  Created by Duc Nguyen on 7/23/15.
//  Copyright (c) 2015 Senspark Co., Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

#include "AdmobProtocolAds.h"
#include "PluginUtilsIOS.h"

USING_NS_SENSPARK_PLUGIN_ADS;

using namespace cocos2d::plugin;

AdmobProtocolAds::AdmobProtocolAds() {
    
}

AdmobProtocolAds::~AdmobProtocolAds() {
    PluginUtilsIOS::erasePluginOCData(this);
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
    callFuncWithParam("slideUpBannerAds", nullptr);
}

void AdmobProtocolAds::slideBannerDown() {
    callFuncWithParam("slideDownBannerAds", nullptr);
}

