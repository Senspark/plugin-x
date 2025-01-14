/****************************************************************************
Copyright (c) 2013 cocos2d-x.org

http://www.cocos2d+x.org

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

#import "AdsWrapper.h"

#include "PluginUtilsIOS.h"
#include "ProtocolAds.h"

using namespace cocos2d::plugin;

@implementation AdsWrapper

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

+ (void)onAdsResult:(id)obj withRet:(int)ret withMsg:(NSString*)msg {
    PluginProtocol* plugin = PluginUtilsIOS::getPluginPtr(obj);
    ProtocolAds* adsPlugin = dynamic_cast<ProtocolAds*>(plugin);
    if (adsPlugin) {
        const char* chMsg = [msg UTF8String];
        AdsResultCode cRet = static_cast<AdsResultCode>(ret);
        AdsListener* listener = adsPlugin->getAdsListener();
        ProtocolAds::AdsCallback callback = adsPlugin->getCallback();
        if (listener) {
            listener->onAdsResult(cRet, chMsg);
        } else if (callback) {
            std::string stdmsg(chMsg);
            callback(cRet, stdmsg);
        }
    } else {
        PluginUtilsIOS::outputLog(
            "Can't find the C++ object of the ads plugin");
    }
}

+ (void)onPlayerGetPoints:(id)obj withPoints:(int)points {
    PluginProtocol* plugin = PluginUtilsIOS::getPluginPtr(obj);
    ProtocolAds* adsPlugin = dynamic_cast<ProtocolAds*>(plugin);
    if (adsPlugin) {
        AdsListener* listener = adsPlugin->getAdsListener();
        if (listener) {
            listener->onPlayerGetPoints(adsPlugin, points);
        }
    } else {
        PluginUtilsIOS::outputLog(
            "Can't find the C++ object of the ads plugin");
    }
}

#pragma GCC diagnostic pop

+ (NSString*)buildVersion {
    NSString* SDKPlatformVersion =
        [[NSBundle mainBundle] infoDictionary][@"DTPlatformVersion"];

    if (SDKPlatformVersion) {
        return SDKPlatformVersion;
    }

    // adapted from
    // http://stackoverflow.com/questions/25540140/can-one-determine-the-ios-sdk-version-used-to-build-a-binary-programmatically
    // form character set of digits and punctuation
    NSMutableCharacterSet* characterSet =
        [[NSCharacterSet decimalDigitCharacterSet] mutableCopy];

    [characterSet
        formUnionWithCharacterSet:[NSCharacterSet punctuationCharacterSet]];

    // get only those things in characterSet from the SDK name
    NSString* SDKName = [[NSBundle mainBundle] infoDictionary][@"DTSDKName"];
    NSArray* components = [[SDKName
        componentsSeparatedByCharactersInSet:[characterSet invertedSet]]
        filteredArrayUsingPredicate:[NSPredicate
                                        predicateWithFormat:@"length != 0"]];

    if ([components count]) {
        return components[0];
    }

    return nil;
}

+ (BOOL)wasBuiltForiOS8orLater {
    return [[self buildVersion] compare:@"8.0" options:NSNumericSearch] !=
           NSOrderedAscending;
}

+ (BOOL)requireRotation {
    return ![self wasBuiltForiOS8orLater] ||
           ([[[UIDevice currentDevice] systemVersion] floatValue] < 8.0);
}

/// http://stackoverflow.com/questions/24150359/is-uiscreen-mainscreen-bounds-size-becoming-orientation-dependent-in-ios8?noredirect=1&lq=1
+ (CGSize)getOrientationDependentScreenSize {
    UIViewController* controller = [AdsWrapper getCurrentRootViewController];

    if (nil == controller) {
        PluginUtilsIOS::outputLog("Can't get the UIViewController object");
        return CGSizeZero;
    }

    CGSize rootSize = [[controller view] frame].size;

    if ([self requireRotation] &&
        UIInterfaceOrientationIsLandscape([controller interfaceOrientation])) {
        CGFloat temp = rootSize.width;
        rootSize.width = rootSize.height;
        rootSize.height = temp;
    }

    return rootSize;
}

+ (UIViewController* _Nullable)getCurrentRootViewController {
    UIViewController* result = nil;

    // Try to find the root view controller programmically
    // Find the top window (that is not an alert view or other window)
    UIWindow* topWindow = [[UIApplication sharedApplication] keyWindow];
    if ([topWindow windowLevel] != UIWindowLevelNormal) {
        NSArray* windows = [[UIApplication sharedApplication] windows];
        for (topWindow in windows) {
            if ([topWindow windowLevel] == UIWindowLevelNormal) {
                break;
            }
        }
    }

    UIView* rootView = [[topWindow subviews] objectAtIndex:0];
    id nextResponder = [rootView nextResponder];

    if ([nextResponder isKindOfClass:[UIViewController class]]) {
        result = nextResponder;
    } else if ([topWindow respondsToSelector:@selector(rootViewController)] &&
               [topWindow rootViewController] != nil) {
        result = [topWindow rootViewController];
    } else {
        NSAssert(NO, @"Could not find a root view controller.");
    }

    return result;
}

@end
