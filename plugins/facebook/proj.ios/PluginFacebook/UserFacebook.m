/****************************************************************************
 Copyright (c) 2014 Chukong Technologies Inc.
 
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

#import "UserFacebook.h"
#import <FBSDKCoreKit/FBSDKCoreKit.h>
#import <FBSDKLoginKit/FBSDKLoginKit.h>
#import "UserWrapper.h"
#import "ParseUtils.h"
#define OUTPUT_LOG(...)     if (self.debug) NSLog(__VA_ARGS__);

@implementation UserFacebook


//@synthesize mUserInfo;
@synthesize debug = __debug;
@synthesize permissions = _permissions;

//bool _isLogin = false;
//NSString *_userId = @"";
//NSString *_accessToken = @"";

- (void) configDeveloperInfo : (NSMutableDictionary*) cpInfo{
}

- (void) login {
    self.permissions = @[@"public_profile", @"email", @"user_friends"];
    [self loginWithPermissionsInArray:_permissions];
}

- (void) loginWithPermissions: (NSString *) permissions {
    self.permissions = [permissions componentsSeparatedByString:@","];
    [self loginWithPermissionsInArray:_permissions];
}

- (void) onLoginResult: (FBSDKLoginManagerLoginResult*) result error: (NSError*) error {
    if (error) {
        [UserWrapper onActionResult:self withRet:kLoginFailed withMsg:@"Login facebook fail!"];
    } else if (result.isCancelled) {
        [UserWrapper onActionResult:self withRet:kLoginFailed withMsg:@"Login facebook: user cancel"];
    } else if (result.declinedPermissions.count > 0) {
        [UserWrapper onActionResult:self withRet:kLoginFailed withMsg:@"Permission declined"];
    } else {
        [UserWrapper onActionResult:self withRet:kLoginSucceed withMsg:@"Login facebook success"];
    }
}

- (void) loginWithPermissionsInArray:(NSArray *) permission {
    FBSDKLoginManager *loginManager = [[FBSDKLoginManager alloc] init];
    [loginManager logInWithReadPermissions:permission handler:^(FBSDKLoginManagerLoginResult *result, NSError *error) {
        [self onLoginResult:result error: error];
    }];
}

- (void) logout{
    if ([FBSDKAccessToken currentAccessToken]) {
        FBSDKLoginManager *login = [[FBSDKLoginManager alloc] init];
        [login logOut];
        [FBSDKAccessToken setCurrentAccessToken:nil];
        
        [UserWrapper onActionResult:self withRet:kLogoutSucceed withMsg:@"Facebook logout"];
    } else {
        [UserWrapper onActionResult:self withRet:kLogoutFailed withMsg:@"Not login yet."];
    }
}

- (NSString *) getUserID{
    return [FBSDKAccessToken currentAccessToken].userID;
}
- (BOOL) isLoggedIn{
    return [FBSDKAccessToken currentAccessToken] ? YES : NO;
}

-(NSString *) getPermissionList {
    NSSet* permissions = [FBSDKAccessToken currentAccessToken].permissions;
    return [[permissions allObjects] componentsJoinedByString:@","];
}

-(NSString *)getAccessToken{
    return [FBSDKAccessToken currentAccessToken].tokenString;
}
- (NSString*) getSessionID{
    return @"";
}
- (void) setDebugMode: (BOOL) debug{
    __debug = debug;
}

- (NSString*) getSDKVersion{
    return [FBSDKSettings sdkVersion];
}

- (NSString*) getPluginVersion{
    return @"0.1.0";
}

//-(void)api:(NSMutableDictionary *)params{
//    NSString *graphPath = [params objectForKey:@"Param1"];
//    int methodID = [[params objectForKey:@"Param2"] intValue];
//    NSString * method = methodID == 0? @"GET":methodID == 1?@"POST":@"DELETE";
//    NSDictionary *param = [params objectForKey:@"Param3"];
//    int cbId = [[params objectForKey:@"Param4"] intValue];
//    [FBRequestConnection startWithGraphPath:graphPath
//                                 parameters:param HTTPMethod:method
//                          completionHandler:^(FBRequestConnection *connection, id result, NSError *error) {
//                              if(!error){
//                                  NSString *msg = [ParseUtils NSDictionaryToNSString:(NSDictionary *)result];
//                                  if(nil == msg){
//                                       NSString *msg = [ParseUtils MakeJsonStringWithObject:@"parse result failed" andKey:@"error_message"];
//                                      [UserWrapper onGraphResult:self withRet:kGraphResultFail withMsg:msg withCallback:cbId];
//                                  }else{
//                                      OUTPUT_LOG(@"success");
//                                      [UserWrapper onGraphResult:self withRet:kGraphResultSuccess withMsg:msg withCallback:cbId];
//                                  }
//                              }else{
//                                   NSString *msg = [ParseUtils MakeJsonStringWithObject:error.description andKey:@"error_message"];
//                                  [UserWrapper onGraphResult:self withRet:(int)error.code withMsg:msg withCallback:cbId];
//                                  OUTPUT_LOG(@"error %@", error.description);
//                              }
//                              
//                          }];
//}

@end
