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

#import <GoogleMobileAds/GoogleMobileAds.h>

#import "AdsAdmob.h"
#import "SSBannerAdListener.h"
#import "SSNativeExpressAdListener.h"
#import "SSNativeAdvancedAdListener.h"
#import "SSInterstitialAdListener.h"
#import "SSRewardedVideoAdListener.h"
#import "SSAdColonyMediation.h"
#import "SSAdMobUtility.h"
#import "AdsWrapper.h"

typedef NS_ENUM(NSInteger, SSAdMobAdType) {
    SSAdMobAdTypeBanner,
    SSAdMobAdTypeNativeExpress,
    SSAdmobAdTypeAdvanced,
};

#define OUTPUT_LOG(...)                                                        \
    if (self.debug)                                                            \
        NSLog(__VA_ARGS__);

static NSString* const NativeAdsAdvancedLayoutIdExtra   = @"layout_id";

@implementation AdsAdmob

- (id)init {
    self = [super init];
    if (self == nil) {
        return self;
    }

    bannerAdListener_ = [[SSBannerAdListener alloc] initWithAdsInterface:self];

    nativeExpressAdListener_ =
        [[SSNativeExpressAdListener alloc] initWithAdsInterface:self];

    nativeAdvancedAdListener_ = [[SSNativeAdvancedAdListener alloc] initWithAdsInterface:self];

    interstitialAdListener_ =
        [[SSInterstitialAdListener alloc] initWithAdsInterface:self];

    rewardedVideoAdListener_ =
        [[SSRewardedVideoAdListener alloc] initWithAdsInterface:self];

    adViews_ = [[NSMutableDictionary alloc] init];
    adSizes_ = [[NSMutableDictionary alloc] init];

    return self;
}

- (void)dealloc {
    [self _removeInterstitialAd];
    [self setTestDeviceIDs:nil];

    [bannerAdListener_ release];
    bannerAdListener_ = nil;

    [nativeExpressAdListener_ release];
    nativeExpressAdListener_ = nil;
    
    [nativeAdvancedAdListener_ release];
    nativeAdvancedAdListener_ = nil;

    [interstitialAdListener_ release];
    interstitialAdListener_ = nil;

    [rewardedVideoAdListener_ release];
    rewardedVideoAdListener_ = nil;
    
    for (NSString* key in [adViews_ allKeys]) {
        [self destroyAd:key];
    }
    
    [adViews_ release];
    adViews_ = nil;

    [adLoaders_ release];
    adLoaders_ = nil;
    
    [adSizes_ release];
    adSizes_ = nil;

    [super dealloc];
}

- (void)initialize:(NSString* _Nonnull)applicationId {
    [GADMobileAds configureWithApplicationID:applicationId];
}

- (void)addTestDevice:(NSString*)deviceId {
    if ([self testDeviceIDs] == nil) {
        [self setTestDeviceIDs:[NSMutableArray array]];
        [[self testDeviceIDs] addObject:kDFPSimulatorID];
    }

    [[self testDeviceIDs] addObject:deviceId];
}

- (void)configMediationAdColony:(NSDictionary*)params {
    // clang-format off
    NSString* appId                = [params objectForKey:@"AdColonyAppID"];
    NSString* interstitialAdZoneId = [params objectForKey:@"AdColonyInterstitialAdID"];
    NSString* rewardedAdZoneId     = [params objectForKey:@"AdColonyRewardedAdID"];
    // clang-format on

    // Reset old values.
    [self setAdColonyRewardedAdZoneId:nil];
    [self setAdColonyInterstitialAdZoneId:nil];

    if (appId != nil) {
        if ([SSAdColonyMediation isLinkedWithAdColony]) {
            NSArray* zones = [NSArray
                arrayWithObjects:interstitialAdZoneId, rewardedAdZoneId, nil];
            [SSAdColonyMediation startWithAppId:appId
                                       andZones:zones
                                    andCustomId:nil];

            // Assigns new values.
            [self setAdColonyInterstitialAdZoneId:interstitialAdZoneId];
            [self setAdColonyRewardedAdZoneId:rewardedAdZoneId];
        } else {
            NSLog(@"%s: AdColony is not linked!", __PRETTY_FUNCTION__);
        }
    }
}

#pragma mark InterfaceAds impl

- (void)configDeveloperInfo:(NSDictionary*)devInfo {
    NSLog(@"%s: deprecated.", __PRETTY_FUNCTION__);
}

- (void)showAds:(NSDictionary*)info position:(int)pos {
    NSLog(@"%s: deprecated.", __PRETTY_FUNCTION__);
}

- (void)hideAds:(NSDictionary*)info {
    NSLog(@"%s: deprecated.", __PRETTY_FUNCTION__);
}

- (void)queryPoints {
    OUTPUT_LOG(@"Admob not support query points!");
}

- (void)spendPoints:(int)points {
    OUTPUT_LOG(@"Admob not support spend points!");
}

- (void)setDebugMode:(BOOL)isDebugMode {
    self.debug = isDebugMode;
}

- (NSString*)getSDKVersion {
    return [GADRequest sdkVersion];
}

- (NSString*)getPluginVersion {
    return @"0.3.0";
}

#pragma mark - Banner ad & native express ad

- (void)createBannerAd:(NSDictionary*)params {
    NSAssert([params count] == 3, @"Invalid number of params");

    NSString* adId = params[@"Param1"];
    NSNumber* width = params[@"Param2"];
    NSNumber* height = params[@"Param3"];

    NSAssert([adId isKindOfClass:[NSString class]], @"...");
    NSAssert([width isKindOfClass:[NSNumber class]], @"...");
    NSAssert([height isKindOfClass:[NSNumber class]], @"...");

    [self _createAd:SSAdMobAdTypeBanner adId:adId width:width height:height extras: nil];
}

- (void)createNativeExpressAd:(NSDictionary* _Nonnull)params {
    NSAssert([params count] == 3, @"Invalid number of params");

    NSString* adId = params[@"Param1"];
    NSNumber* width = params[@"Param2"];
    NSNumber* height = params[@"Param3"];

    NSAssert([adId isKindOfClass:[NSString class]], @"...");
    NSAssert([width isKindOfClass:[NSNumber class]], @"...");
    NSAssert([height isKindOfClass:[NSNumber class]], @"...");

    [self _createAd:SSAdMobAdTypeNativeExpress
               adId:adId
              width:width
             height:height
             extras: nil];
}

- (void)createNativeAdvancedAd:(NSDictionary* _Nonnull)params {
    NSAssert([params count] == 3, @"Invalid number of params");

    NSString* adId = params[@"Param1"];
    NSString* layoutId = params[@"Param2"];
    NSNumber* width = params[@"Param3"];
    NSNumber* height = params[@"Param4"];

    NSAssert([adId isKindOfClass:[NSString class]], @"...");
    NSAssert([layoutId isKindOfClass:[NSString class]], @"...");
    NSAssert([width isKindOfClass:[NSNumber class]], @"...");
    NSAssert([height isKindOfClass:[NSNumber class]], @"...");

    NSDictionary* extras = [NSDictionary dictionaryWithObject:layoutId forKey:NativeAdsAdvancedLayoutIdExtra];

    [self _createAd:SSAdmobAdTypeAdvanced
               adId:adId
              width:width
             height:height
             extras: extras];
}

- (void)_createAd:(SSAdMobAdType)adType
             adId:(NSString* _Nonnull)adId
            width:(NSNumber* _Nonnull)width
           height:(NSNumber* _Nonnull)height
           extras:(NSDictionary*) extras {
    
    GADAdSize size = [self _createAdSize:width height:height];
    if ([self _hasAd:adId size:size]) {
        NSLog(@"%s: attempted to create an ad with id = %@ width = %@ height = "
              @"%@ but it is already created.",
              __PRETTY_FUNCTION__, adId, width, height);
        return;
    }

    [self _createAd:adType adId:adId size:size extras:extras];
}

- (BOOL)_hasAd:(NSString* _Nonnull)adId size:(GADAdSize)size {
    NSValue* elt = [adSizes_ objectForKey:adId];
    if (elt == nil) {
        return NO;
    }

    GADAdSize cachedSize = GADAdSizeFromNSValue(elt);
    return GADAdSizeEqualToSize(size, cachedSize);
}

- (void)_createAd:(SSAdMobAdType)adType
             adId:(NSString* _Nonnull)adId
             size:(GADAdSize)size
             extras: (NSDictionary*) extras {
    [self destroyAd:adId];

    UIView* view = nil;
    UIViewController* controller =
        [SSAdMobUtility getCurrentRootViewController];

    if (adType == SSAdMobAdTypeBanner) {
        view = [self _createBannerAd:adId size:size controller:controller];
    } else if (adType == SSAdMobAdTypeNativeExpress) {
        view =
            [self _createNativeExpressAd:adId size:size controller:controller];
    } else if (adType == SSAdmobAdTypeAdvanced) {
        if (extras == nil) {
            NSAssert(NO, @"Admob Native Ads Advanced REQUIRED a layout.");
        }
        
        NSString* layoutId = [extras objectForKey:NativeAdsAdvancedLayoutIdExtra];
        if (layoutId == nil) {
            NSAssert(NO, @"Admob Native Ads Advanced layout ID is NULL.");
        }
        
        view = [self _createNativeAdvancedAd:adId layout:layoutId size:size controller:controller];
    } else {
        NSAssert(NO, @"...");
    }

    [view setHidden:YES];
    [[controller view] addSubview:view];

    [adViews_ setObject:view forKey:adId];
    [adSizes_ setObject:NSValueFromGADAdSize(size) forKey:adId];
}

- (GADBannerView*)_createBannerAd:(NSString* _Nonnull)adId
                             size:(GADAdSize)size
                       controller:(UIViewController* _Nonnull)controller {
    GADBannerView* view =
        [[[GADBannerView alloc] initWithAdSize:size] autorelease];
    if (view == nil) {
        NSLog(@"%s: invalid ad size.", __PRETTY_FUNCTION__);
        return nil;
    }

    [view setAdUnitID:adId];
    [view setDelegate:bannerAdListener_];
    [view setRootViewController:controller];
    return view;
}

- (GADNativeExpressAdView*)_createNativeExpressAd:(NSString* _Nonnull)adId
                                             size:(GADAdSize)size
                                       controller:(UIViewController* _Nonnull)
                                                      controller {
    GADNativeExpressAdView* view =
        [[[GADNativeExpressAdView alloc] initWithAdSize:size] autorelease];
    if (view == nil) {
        NSLog(@"%s: invalid ad size.", __PRETTY_FUNCTION__);
        return nil;
    }

    [view setAdUnitID:adId];
    [view setDelegate:nativeExpressAdListener_];
    [view setRootViewController:controller];
    return view;
}

- (GADNativeAppInstallAdView*) _createNativeAdvancedAd: (NSString* _Nonnull) adId
                                      layout: (NSString*) layout
                                        size: (GADAdSize) size

                                controller: (UIViewController* _Nonnull) controller {
    
    GADNativeAppInstallAdView* appInstallAdView = [[[NSBundle mainBundle] loadNibNamed:layout owner:nil options:nil] firstObject];
    
    GADAdLoader* adLoader = [[GADAdLoader alloc] initWithAdUnitID:adId rootViewController:controller adTypes:@[kGADAdLoaderAdTypeNativeAppInstall] options:nil];
    
    [adLoaders_ setObject:adLoader forKey:adId];
    
    adLoader.delegate = nativeAdvancedAdListener_;
    
    CGRect frame = appInstallAdView.frame;
    frame.size = size.size;
    appInstallAdView.frame = frame;
    
    return appInstallAdView;
}

- (void) displayNativeAdvancedAd: (GADNativeAppInstallAd* _Nonnull) ad adLoader: (GADAdLoader* _Nonnull) adLoader {
    
    NSString *adId = [[adLoaders_ allKeysForObject:adLoader] firstObject];
    
    NSAssert(adId != nil, @"Ad ID must not NIL");
    
    GADNativeAppInstallAdView* appInstallAdView = (GADNativeAppInstallAdView*) [adViews_ objectForKey:adId];
    
    NSAssert(appInstallAdView != nil, @"Not provide Ad view yet!!!");
    
    appInstallAdView.nativeAppInstallAd = ad;
    
    ((UILabel*) appInstallAdView.headlineView).text = ad.headline;
    ((UIImageView*) appInstallAdView.iconView).image = ad.icon.image;
    ((UILabel*) appInstallAdView.bodyView).text = ad.body;
    ((UIImageView *)appInstallAdView.imageView).image =
      ((GADNativeAdImage *)[ad.images firstObject]).image;
    [((UIButton *)appInstallAdView.callToActionView)setTitle:ad.callToAction
                                                  forState:UIControlStateNormal];
    
    // Other assets are not, however, and should be checked first.
    if (ad.starRating) {
//        ((UIImageView *) appInstallAdView.starRatingView).image =
//            [self imageForStars:ad.starRating];
            appInstallAdView.starRatingView.hidden = NO;
    } else {
        appInstallAdView.starRatingView.hidden = YES;
    }

    if (ad.store) {
        ((UILabel *)appInstallAdView.storeView).text = ad.store;
        appInstallAdView.storeView.hidden = NO;
    } else {
        appInstallAdView.storeView.hidden = YES;
    }

    if (ad.price) {
        ((UILabel *)appInstallAdView.priceView).text = ad.price;
        appInstallAdView.priceView.hidden = NO;
    } else {
        appInstallAdView.priceView.hidden = YES;
    }
    
    appInstallAdView.callToActionView.userInteractionEnabled = NO;
}


- (void)destroyAd:(NSString* _Nonnull)adId {
    UIView* view = [adViews_ objectForKey:adId];
    if (view == nil) {
        NSLog(@"%s: attempted to destroy a non-created ad with id = %@",
              __PRETTY_FUNCTION__, adId);
        return;
    }

    [view removeFromSuperview];
    [adViews_ removeObjectForKey:adId];
    [adSizes_ removeObjectForKey:adId];
    [adLoaders_ removeObjectForKey:adId];
}

- (void)showAd:(NSString* _Nonnull)adId {
    UIView* view = [adViews_ objectForKey:adId];
    if (view == nil) {
        NSLog(@"%s: attempted to show a non-created ad with id = %@",
              __PRETTY_FUNCTION__, adId);
        return;
    }

    [view setHidden:NO];

    GADRequest* request = [GADRequest request];
    [request setTestDevices:[self testDeviceIDs]];

    if ([view isKindOfClass:[GADBannerView class]]) {
        GADBannerView* _view = (GADBannerView*)(view);
        [_view loadRequest:request];
    } else if ([view isKindOfClass:[GADNativeExpressAdView class]]) {
        GADNativeExpressAdView* _view = (GADNativeExpressAdView*)(view);
        [_view loadRequest:request];
    } else if ([view isKindOfClass:[GADNativeAppInstallAdView class]]) {
        GADAdLoader* adLoader = (GADAdLoader*) [adLoaders_ objectForKey:adId];
        [adLoader loadRequest:request];
    }
}

- (void)hideAd:(NSString* _Nonnull)adId {
    UIView* view = [adViews_ objectForKey:adId];
    if (view == nil) {
        NSLog(@"%s: attempted to hide a non-created ad with id = %@",
              __PRETTY_FUNCTION__, adId);
        return;
    }

    [view setHidden:YES];
}

- (void)moveAd:(NSDictionary* _Nonnull)params {
    NSAssert([params count] == 3, @"...");

    NSString* adId = params[@"Param1"];
    NSNumber* x = params[@"Param2"];
    NSNumber* y = params[@"Param3"];

    NSAssert([adId isKindOfClass:[NSString class]], @"...");
    NSAssert([x isKindOfClass:[NSNumber class]], @"...");
    NSAssert([y isKindOfClass:[NSNumber class]], @"...");

    [self _moveAd:adId x:x y:y];
}

- (void)_moveAd:(NSString* _Nonnull)adId
              x:(NSNumber* _Nonnull)x
              y:(NSNumber* _Nonnull)y {
    UIView* view = [adViews_ objectForKey:adId];
    if (view == nil) {
        NSLog(@"%s: attempted to move a non-created ad with id = %@",
              __PRETTY_FUNCTION__, adId);
        return;
    }

    CGFloat scale = [self _getRetinaScale];
    CGRect frame = [view frame];
    frame.origin.x = [x floatValue] / scale;
    frame.origin.y = [y floatValue] / scale;
    [view setFrame:frame];
}

#pragma mark - Interstitial ad

- (void)showInterstitialAd {
    NSLog(@"Interstitial view: %@", [self interstitialAdView]);

    if ([self hasInterstitialAd]) {
        UIViewController* controller =
            [SSAdMobUtility getCurrentRootViewController];
        [[self interstitialAdView] presentFromRootViewController:controller];
    } else {
        // Ad is not ready to present.
        NSLog(@"%s: interstitial cannot show, it is not ready.",
              __PRETTY_FUNCTION__);
    }
}

- (void)loadInterstitialAd:(NSString* _Nonnull)adId {
    [self _removeInterstitialAd];

    GADInterstitial* view = [[GADInterstitial alloc] initWithAdUnitID:adId];
    [view setDelegate:interstitialAdListener_];

    GADRequest* request = [GADRequest request];
    [request setTestDevices:[self testDeviceIDs]];
    if ([SSAdColonyMediation isLinkedWithAdColony] &&
        [[self adColonyInterstitialAdZoneId] length] > 0) {
        id<GADAdNetworkExtras> extras = [SSAdColonyMediation
            extrasWithZoneId:[self adColonyInterstitialAdZoneId]];
        [request registerAdNetworkExtras:extras];
    }
    [view loadRequest:request];

    [self setInterstitialAdView:view];
}

- (BOOL)hasInterstitialAd {
    return [[self interstitialAdView] isReady];
}

- (void)_removeInterstitialAd {
    if ([self interstitialAdView] != nil) {
        [[self interstitialAdView] release];
        [self setInterstitialAdView:nil];
    }
}

#pragma mark - Rewarded video ad

- (void)showRewardedAd {
    if ([self hasRewardedAd]) {
        [[GADRewardBasedVideoAd sharedInstance]
            presentFromRootViewController:
                [SSAdMobUtility getCurrentRootViewController]];
    }
}

- (void)loadRewardedAd:(NSString* _Nonnull)adId {
    [[GADRewardBasedVideoAd sharedInstance]
        setDelegate:rewardedVideoAdListener_];

    GADRequest* request = [GADRequest request];
    if ([SSAdColonyMediation isLinkedWithAdColony] &&
        [[self adColonyRewardedAdZoneId] length] > 0) {
        id<GADAdNetworkExtras> extras = [SSAdColonyMediation
            extrasWithZoneId:[self adColonyRewardedAdZoneId]];
        [request registerAdNetworkExtras:extras];
    }

    [[GADRewardBasedVideoAd sharedInstance] loadRequest:request
                                           withAdUnitID:adId];
}

- (BOOL)hasRewardedAd {
    return [[GADRewardBasedVideoAd sharedInstance] isReady];
}

#pragma mark - Utility

- (NSNumber* _Nonnull)getSizeInPixels:(NSNumber* _Nonnull)size_ {
    int size = [size_ intValue];
    if (size == -2) {
        return [self _getAutoHeightInPixels];
    }
    if (size == -1) {
        return [self _getFullWidthInPixels];
    }
    GADAdSize adSize = GADAdSizeFromCGSize(CGSizeMake(size, size));
    GADBannerView* banner =
        [[[GADBannerView alloc] initWithAdSize:adSize] autorelease];
    if (banner == nil) {
        NSLog(@"%s: invalid ad size", __PRETTY_FUNCTION__);
        return @(0);
    }
    CGFloat scale = [self _getRetinaScale];
    return @([banner frame].size.height * scale);
}

- (GADAdSize)_createAdSize:(NSNumber* _Nonnull)width
                    height:(NSNumber* _Nonnull)height {
    UIInterfaceOrientation orientation =
        [[UIApplication sharedApplication] statusBarOrientation];
    if ([width intValue] == -1) {
        // Full width.
        if ([height intValue] == -2) {
            // Auto height.
            // Smart banner.
            return [self _getSmartBannerAdSize];
        }
        if (UIInterfaceOrientationIsLandscape(orientation)) {
            return GADAdSizeFullWidthLandscapeWithHeight([height intValue]);
        }
        return GADAdSizeFullWidthPortraitWithHeight([height intValue]);
    }
    return GADAdSizeFromCGSize(CGSizeMake([width intValue], [height intValue]));
}

- (GADBannerView* _Nullable)_createDummySmartBanner {
    GADBannerView* banner =
        [[GADBannerView alloc] initWithAdSize:[self _getSmartBannerAdSize]];
    return [banner autorelease];
}

- (NSNumber* _Nonnull)_getAutoHeightInPixels {
    GADBannerView* banner = [self _createDummySmartBanner];
    if (banner == nil) {
        NSAssert(NO, @"...");
        return @(0);
    }
    CGFloat scale = [self _getRetinaScale];
    return @([banner frame].size.height * scale);
}

- (NSNumber* _Nonnull)_getFullWidthInPixels {
    GADBannerView* banner = [self _createDummySmartBanner];
    if (banner == nil) {
        NSAssert(NO, @"...");
        return @(0);
    }
    CGFloat scale = [self _getRetinaScale];
    return @([banner frame].size.width * scale);
}

/// Retrieves the size of the smart banner depends on the orientation of the
/// design resolution.
- (GADAdSize)_getSmartBannerAdSize {
    UIInterfaceOrientation orientation =
        [[UIApplication sharedApplication] statusBarOrientation];

    return UIInterfaceOrientationIsLandscape(orientation)
               ? kGADAdSizeSmartBannerLandscape
               : kGADAdSizeSmartBannerPortrait;
}

- (NSNumber* _Nonnull)getRealScreenWidthInPixels {
    return @([self _getRealScreenSizeInPixels].width);
}

- (NSNumber* _Nonnull)getRealScreenHeightInPixels {
    return @([self _getRealScreenSizeInPixels].height);
}

- (CGSize)_getRealScreenSizeInPixels {
    CGSize rootSize = [AdsWrapper getOrientationDependentScreenSize];
    CGFloat scale = [self _getRetinaScale];
    return CGSizeMake(rootSize.width * scale, rootSize.height * scale);
}

- (CGFloat)_getRetinaScale {
    CGFloat scale = [[UIScreen mainScreen] scale];
    return scale;
}

@end
