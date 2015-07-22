//
//  GoogleProtocolAnalytics.cpp
//  PluginSenspark
//
//  Created by Duc Nguyen on 7/20/15.
//  Copyright (c) 2015 Senspark Co., Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>
#include "InterfaceAnalytics.h"
#include "GoogleProtocolAnalytics.h"
#include "PluginUtilsIOS.h"

USING_NS_SENSPARK_PLUGIN;
using namespace cocos2d::plugin;

GoogleProtocolAnalytics::GoogleProtocolAnalytics() {
    
}

GoogleProtocolAnalytics::~GoogleProtocolAnalytics() {
    PluginUtilsIOS::erasePluginOCData(this);
}

void GoogleProtocolAnalytics::configureTracker(const string& trackerId) {
    PluginParam trackerIdParam(trackerId.c_str());
    callFuncWithParam("configureTracker", &trackerIdParam, nullptr);
}

void GoogleProtocolAnalytics::createTracker(const string& trackerId) {
    PluginParam trackerIdParam(trackerId.c_str());
    callFuncWithParam("createTracker", &trackerIdParam, nullptr);
}

void GoogleProtocolAnalytics::enableTracker(const string& trackerId) {
    PluginParam trackerIdParam(trackerId.c_str());
    callFuncWithParam("enableTracker", &trackerIdParam, nullptr);
}

void GoogleProtocolAnalytics::startSession(const char* appName) {
    PluginOCData* pData = PluginUtilsIOS::getPluginOCData(this);
    assert(pData != nullptr);
    
    id ocObj = pData->obj;
    if ([ocObj conformsToProtocol:@protocol(InterfaceAnalytics)]) {
        NSObject<InterfaceAnalytics>* curObj = ocObj;
        NSString* pStrKey = [NSString stringWithUTF8String:appName];
        [curObj startSession:pStrKey];
    }
}

void GoogleProtocolAnalytics::stopSession() {
    PluginOCData* pData = PluginUtilsIOS::getPluginOCData(this);
    assert(pData != NULL);
    
    id ocObj = pData->obj;
    if ([ocObj conformsToProtocol:@protocol(InterfaceAnalytics)]) {
        NSObject<InterfaceAnalytics>* curObj = ocObj;
        [curObj stopSession];
    }
}

void GoogleProtocolAnalytics::setLogLevel(GALogLevel logLevel) {
    PluginParam logLevelParam((int) logLevel);
    callFuncWithParam("setLogLevel", &logLevelParam, nullptr);
}

void GoogleProtocolAnalytics::setCaptureUncaughtException(bool isEnabled) {
    PluginOCData* pData = PluginUtilsIOS::getPluginOCData(this);
    assert(pData != NULL);
    
    id ocObj = pData->obj;
    if ([ocObj conformsToProtocol:@protocol(InterfaceAnalytics)]) {
        NSObject<InterfaceAnalytics>* curObj = ocObj;
        [curObj setCaptureUncaughtException:isEnabled];
    }
}

void GoogleProtocolAnalytics::dispatchHits() {
    callFuncWithParam("dispatchHits", nullptr);
}

void GoogleProtocolAnalytics::dispatchPeriodically(int seconds) {
    PluginParam secondsParam(seconds);
    callFuncWithParam("dispatchPeriodically", &secondsParam, nullptr);
}

void GoogleProtocolAnalytics::stopPeriodicalDispatch() {
    callFuncWithParam("stopPeriodcalDispatch", nullptr);
}

void GoogleProtocolAnalytics::trackScreen(const string& screenName) {
    PluginParam screenParam(screenName.c_str());
    callFuncWithParam("trackScreen", &screenParam, nullptr);
}

void GoogleProtocolAnalytics::trackEvent(const string& category, const string& action, const string& label, float value) {
    PluginParam categoryParam(category.c_str());
    PluginParam actionParam(action.c_str());
    PluginParam labelParam(label.c_str());
    PluginParam valueParam(value);
    
    callFuncWithParam("trackEventWithCategory", &categoryParam, &actionParam, &labelParam, &valueParam, nullptr);
}

void GoogleProtocolAnalytics::trackException(const string& description, bool isFatal) {
    PluginParam descParam(description.c_str());
    PluginParam fatalParam(isFatal);
    
    callFuncWithParam("trackTimmingWithCategory", &descParam, &fatalParam, nullptr);
}

void GoogleProtocolAnalytics::trackTimming(const string& category, int interval, const string& name, const string& label) {
    PluginParam categoryParam(category.c_str());
    PluginParam intervalParam(interval);
    PluginParam nameParam(name.c_str());
    PluginParam labelParam(label.c_str());
    
    callFuncWithParam("trackTimmingWithCategory", &categoryParam, &intervalParam, &nameParam, &labelParam, nullptr);
}

void GoogleProtocolAnalytics::trackSocial(const string& network, const string& action, const string& target) {
    PluginParam networkParam(network.c_str());
    PluginParam actionParam(action.c_str());
    PluginParam targetParam(target.c_str());
    
    callFuncWithParam("trackSocialWithNetwork", &networkParam, &actionParam, &targetParam, nullptr);
}

void GoogleProtocolAnalytics::setDryRun(bool isDryRun) {
    PluginParam dryRun(isDryRun);
    callFuncWithParam("setDryRun", &dryRun, nullptr);
}

void GoogleProtocolAnalytics::enableAdvertisingTracking(bool enable) {
    PluginParam enableParam(enable);
    callFuncWithParam("enableAdvertisingTracking", &enableParam, nullptr);
}