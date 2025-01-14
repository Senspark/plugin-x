//
//  AnalyticsGoogle.h
//  PluginGoogleAnalytics
//
//  Created by Duc Nguyen on 7/7/15.
//  Copyright (c) 2015 Senspark Co., Ltd. All rights reserved.
//

#ifndef __PluginGoogleAnalytics__AnalyticsGoogle__
#define __PluginGoogleAnalytics__AnalyticsGoogle__

#import "InterfaceAnalytics.h"
#import "GAILogger.h"
#import "GAITracker.h"

@interface AnalyticsGoogle : NSObject <InterfaceAnalytics> {
    BOOL _debug;
}

@property (nonatomic, retain)
    NSDictionary<NSString*, id<GAITracker>>* registeredTrackers;
@property (nonatomic, assign) id<GAITracker> currentTracker;

/**
 interfaces of protocol : InterfaceAnalytics
 */
- (void)startSession:(NSString*)appKey;
- (void)stopSession;
- (void)setSessionContinueMillis:(long)millis;
- (void)setCaptureUncaughtException:(BOOL)isEnabled;
- (void)setDebugMode:(BOOL)isDebugMode;
- (void)logError:(NSString*)errorId withMsg:(NSString*)message;
- (void)logEvent:(NSString*)eventId;
- (void)logEvent:(NSString*)eventId withParam:(NSMutableDictionary*)paramMap;
- (void)logTimedEventBegin:(NSString*)eventId;
- (void)logTimedEventEnd:(NSString*)eventId;
- (NSString*)getSDKVersion;
- (NSString*)getPluginVersion;

/**
 interfaces of Google Analytics SDK
 */

- (void)configureTracker:(NSString*)trackerId;
- (void)enableTracker:(NSString*)trackerId;
- (void)createTracker:(NSString*)trackerId;

- (void)setLogLevel:(NSNumber*)logLevel;
- (void)dispatchHits;
- (void)dispatchPeriodically:(NSNumber*)seconds;
- (void)stopPeriodicalDispatch;

- (void)setParameter:(NSDictionary*)dict;
- (void)sendHit:(NSDictionary*)dict;

- (void)setDryRun:(NSNumber*)isDryRun;
- (void)enableAdvertisingTracking:(NSNumber*)enabled;

@end

#endif /* defined(__PluginGoogleAnalytics__AnalyticsGoogle__) */
