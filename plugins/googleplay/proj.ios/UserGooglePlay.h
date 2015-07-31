//
//  UserGooglePlay.h
//  PluginGooglePlay
//
//  Created by Duc Nguyen on 7/27/15.
//  Copyright (c) 2015 Senspark Co., Ltd. All rights reserved.
//

#ifndef PluginGooglePlay_UserGooglePlay_h
#define PluginGooglePlay_UserGooglePlay_h

#import <Foundation/Foundation.h>
#import <InterfaceUser.h>
#import <gpg/gpg.h>

@interface UserGooglePlay : NSObject <InterfaceUser> {
    std::unique_ptr<gpg::GameServices> _gameServices;
    BOOL _isSignedIn;
}

@property BOOL debug;
@property (copy, nonatomic) NSMutableDictionary* _userInfo;

// ----- Interface User ------
- (void) configDeveloperInfo : (NSMutableDictionary*) cpInfo;
- (void) login;
- (void) logout;
- (BOOL) isLogined;
- (NSString*) getSessionID;
- (void) setDebugMode: (BOOL) debug;
- (NSString*) getSDKVersion;
- (NSString*) getPluginVersion;

// ----- Interface of GooglePlayUser

- (void) beginUserInitiatedSignIn;

@end

#endif