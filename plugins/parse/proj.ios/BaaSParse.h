//
//  BaaSParse.h
//  PluginParse
//
//  Created by Duc Nguyen on 8/14/15.
//  Copyright (c) 2015 Senspark Co., Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "InterfaceBaaS.h"
#import <Parse/Parse.h>
#import <ParseFacebookUtilsV4/PFFacebookUtils.h>

@interface BaaSParse : NSObject<InterfaceBaaS>
{
    PFConfig* _currentConfig;
}

- (void)        loginWithFacebookAccessToken: (NSNumber*) cbID;

- (void)        fetchConfigInBackground:(long) cbID;
- (BOOL)        getBoolConfig: (NSString*) param;
- (int)         getIntegerConfig: (NSString*) param;
- (double)      getDoubleConfig: (NSString*) param;
- (long)        getLongConfig: (NSString*) param;
- (NSString*)   getStringConfig: (NSString*) param;
- (NSArray*)    getArrayConfig: (NSString*) param;

- (NSString*)   getSDKVersion;
- (NSString*)   getPluginVersion;

- (NSString*)   getUserInfo;
- (NSString*)   setUserInfo: (NSDictionary*) params;
- (void)        saveUserInfo: (NSNumber*) cbID;
- (void)        fetchUserInfo: (NSNumber*) cbID;

- (NSString*)   getInstallationInfo;
- (NSString*)   setInstallationInfo: (NSDictionary*) params;
- (void)        saveInstallationInfo: (NSNumber*) cbID;

@end
