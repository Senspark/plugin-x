//
//  GoogleProtocolAnalytics.cpp
//  PluginSenspark
//
//  Created by Duc Nguyen on 7/20/15.
//  Copyright (c) 2015 Senspark Co., Ltd. All rights reserved.
//

#include "GoogleProtocolAnalytics.h"
#include "PluginUtils.h"

using namespace std;

using namespace cocos2d::plugin;
USING_NS_SENSPARK_PLUGIN_ANALYTICS;


GoogleProtocolAnalytics::GoogleProtocolAnalytics() {

}

GoogleProtocolAnalytics::~GoogleProtocolAnalytics() {
    PluginUtils::erasePluginJavaData(this);
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

void GoogleProtocolAnalytics::setLogLevel(GALogLevel logLevel) {
    PluginParam logLevelParam((int) logLevel);
    callFuncWithParam("setLogLevel", &logLevelParam, nullptr);
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
    PluginUtils::outputLog("GoogleProtocolAnalytics", "Track event: category: %s - action: %s - label: %s - value: %f", category.c_str(), action.c_str(), label.c_str(), value);
    PluginParam categoryParam(category.c_str());
    PluginParam actionParam(action.c_str());
    PluginParam labelParam(label.c_str());
    PluginParam valueParam(value);

    callFuncWithParam("trackEventWithCategory", &categoryParam, &actionParam, &labelParam, &valueParam, nullptr);
}

void GoogleProtocolAnalytics::trackException(const string& description, bool isFatal) {
    PluginParam descParam(description.c_str());
    PluginParam fatalParam(isFatal);

    callFuncWithParam("trackException", &descParam, &fatalParam, nullptr);
}

void GoogleProtocolAnalytics::trackTiming(const std::string& category, int interval, const std::string& name, const std::string& label)	{
    PluginUtils::outputLog("GoogleProtocolAnalytics", "Track timing: category: %s - interval: %d - name: %s - label: %s", category.c_str(), interval, name.c_str(), label.c_str());
	PluginParam categoryParam(category.c_str());
    PluginParam intervalParam(interval);
    PluginParam nameParam(name.c_str());
    PluginParam labelParam(label.c_str());

    callFuncWithParam("trackTiming", &categoryParam, &intervalParam, &nameParam, &labelParam, nullptr);
}

void GoogleProtocolAnalytics::trackEcommerceTransactions(const string& identity, const string& productName, const string& productCategory, float priceValue) {
    PluginUtils::outputLog("GoogleProtocolAnalytics", "Track ecommerce: identity: %s - name: %s - category: %s - price: %f", identity.c_str(), productName.c_str(), productCategory.c_str(), priceValue);
    PluginParam identityParam(identity.c_str());
    PluginParam nameParam(productName.c_str());
    PluginParam categoryParam(productCategory.c_str());
    PluginParam priceParam(priceValue);

    callFuncWithParam("trackEcommerceTransactions", &identityParam, &nameParam, &categoryParam, &priceValue, nullptr);
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
