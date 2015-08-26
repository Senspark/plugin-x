//
//  BaaSParse.h
//  PluginParse
//
//  Created by Duc Nguyen on 8/14/15.
//  Copyright (c) 2015 Senspark Co., Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "InterfaceBaaS.h"

@interface BaaSParse : NSObject<InterfaceBaaS>
{
}

- (void) configDeveloperInfo:(NSMutableDictionary *)devInfo;
- (void) signUpWithParams: (NSDictionary*) params ;
- (void) loginWithUsername: (NSString*) username andPassword: (NSString*) password;
- (void) logout;

- (void) saveObjectInBackground: (NSString*) className withParams: (NSDictionary*) obj;
- (NSString*) saveObject: (NSString*) className withParams: (NSDictionary*) obj;

- (void) getObjectInBackground: (NSString*) className withId: (NSString*) objId;
- (NSDictionary*) getObject: (NSString*) className withId: (NSString*) objId;

- (void) updateObjectInBackground: (NSString*) className withId: (NSString*) objId withParams: (NSDictionary*) params;
- (NSString*) updateObject: (NSString*) className withId: (NSString*) objId withParams: (NSDictionary*) params;

- (NSString*) getSDKVersion;
- (NSString*) getPluginVersion;

- (NSDictionary*) getUserInfo;

@end