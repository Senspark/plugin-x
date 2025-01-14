/****************************************************************************
 Copyright (c) 2013 cocos2d-x.org

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

#import <Foundation/Foundation.h>

#import "InterfaceAds.h"
#import <GoogleMobileAds/GADInterstitial.h>

@class SSBannerAdListener;
@class SSNativeExpressAdListener;
@class SSNativeAdvancedAdListener;
@class SSInterstitialAdListener;
@class SSRewardedVideoAdListener;

typedef enum {
    kTypeBanner = 1,
    kTypeFullScreen,
} AdmobType;

typedef enum {
    kNativeAdAdvancedTypeAppInstall = 0x01,
    kNativeAdAdvancedTypeContent    = 0x02,
} NativeAdAdvancedType;

@interface AdsAdmob : NSObject <InterfaceAds> {
    SSBannerAdListener* bannerAdListener_;
    SSNativeExpressAdListener* nativeExpressAdListener_;
    SSNativeAdvancedAdListener* nativeAdvancedAdListener_;
    SSInterstitialAdListener* interstitialAdListener_;
    SSRewardedVideoAdListener* rewardedVideoAdListener_;
    
    NSMutableDictionary<NSString*, GADAdLoader*> *adLoaders_;
    NSMutableDictionary<NSString*, NSDictionary*> *adOptions_;
    NSMutableDictionary<NSString*, UIView*>* adViews_;
    NSMutableDictionary<NSString*, NSValue*>* adSizes_;
}

@property (nonatomic) BOOL debug;
@property (nonatomic, retain, nullable) NSMutableArray* testDeviceIDs;

@property (nonatomic, copy, nullable) NSString* adColonyInterstitialAdZoneId;
@property (nonatomic, copy, nullable) NSString* adColonyRewardedAdZoneId;

@property (nonatomic, assign, nullable) GADInterstitial* interstitialAdView;

- (void)initialize:(NSString* _Nonnull)applicationId;
- (void)addTestDevice:(NSString* _Nonnull)deviceId;
- (void)configMediationAdColony:(NSDictionary* _Nonnull)params;

- (void)createBannerAd:(NSDictionary* _Nonnull)params;
- (void)createNativeExpressAd:(NSDictionary* _Nonnull)params;
- (void)createNativeAdvancedAd: (NSDictionary* _Nonnull)params;
- (void)displayNativeAdvancedAd: (GADNativeAppInstallAd* _Nonnull) ad adLoader: (GADAdLoader* _Nonnull) adLoader;
- (void)destroyAd:(NSString* _Nonnull)adId;
- (void)showAd:(NSString* _Nonnull)adId;
- (void)hideAd:(NSString* _Nonnull)adId;
- (void)moveAd:(NSDictionary* _Nonnull)params;

- (void)showInterstitialAd;
- (void)loadInterstitialAd:(NSString* _Nonnull)adId;
- (BOOL)hasInterstitialAd;

- (void)showRewardedAd;
- (void)loadRewardedAd:(NSString* _Nonnull)adID;
- (BOOL)hasRewardedAd;

- (NSNumber* _Nonnull)getSizeInPixels:(NSNumber* _Nonnull)size;
- (NSNumber* _Nonnull)getRealScreenWidthInPixels;
- (NSNumber* _Nonnull)getRealScreenHeightInPixels;

@end
