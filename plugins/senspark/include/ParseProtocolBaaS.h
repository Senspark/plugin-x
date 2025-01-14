//
//  ParseProtocolBaaS.h
//  PluginSenspark
//
//  Created by Duc Nguyen on 8/17/15.
//  Copyright (c) 2015 Senspark Co., Ltd. All rights reserved.
//

#ifndef PluginSenspark_ParseProtocolBaaS_h
#define PluginSenspark_ParseProtocolBaaS_h

#include "ProtocolBaaS.h"
#include "SensparkPluginMacros.h"
#include <string>
#include <map>

NS_SENSPARK_PLUGIN_BAAS_BEGIN

class ParseProtocolBaaS : public cocos2d::plugin::ProtocolBaaS
{
public:
    virtual ~ParseProtocolBaaS();
    
    std::string getUserInfo();
    std::string setUserInfo(const std::string& jsonChanges);
    void saveUserInfo(BaaSCallback& cb);
    void fetchUserInfo(BaaSCallback& cb);
    
    std::string getInstallationInfo();
    std::string setInstallationInfo(const std::string& changes);
    void saveInstallationInfo(BaaSCallback& cb);
    
    void loginWithFacebookAccessToken(BaaSCallback& cb);
    
    std::string getSubscribedChannels();
    void subscribeChannels(const std::string& channels);
    void unsubscribeChannels(const std::string& channels);
    
    
    void    fetchConfigInBackground(BaaSCallback& cb);
    bool    getBoolConfig(const std::string& param);
    int     getIntegerConfig(const std::string& param);
    double  getDoubleConfig(const std::string& param);
    long    getLongConfig(const std::string& param);
    std::string getStringConfig(const std::string& param);
    std::string getArrayConfig(const std::string& param);
};

NS_SENSPARK_PLUGIN_BAAS_END

#endif
