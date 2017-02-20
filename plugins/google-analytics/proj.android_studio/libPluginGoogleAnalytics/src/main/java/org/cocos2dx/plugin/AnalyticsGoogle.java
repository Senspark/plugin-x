package org.cocos2dx.plugin;

import android.content.Context;
import android.util.Log;

import com.google.android.gms.analytics.ExceptionReporter;
import com.google.android.gms.analytics.GoogleAnalytics;
import com.google.android.gms.analytics.HitBuilders;
import com.google.android.gms.analytics.Tracker;
import com.google.android.gms.analytics.ecommerce.Product;
import com.google.android.gms.analytics.ecommerce.ProductAction;

import org.json.JSONException;
import org.json.JSONObject;

import java.lang.Thread.UncaughtExceptionHandler;
import java.util.HashMap;
import java.util.Hashtable;
import java.util.Locale;
import java.util.Map;

public class AnalyticsGoogle implements InterfaceAnalytics {
    protected static final String          LOG_TAG              = AnalyticsGoogle.class.getName();
    protected              Context         mContext             = null;
    protected static       boolean         isDebug              = false;
    protected              GoogleAnalytics mGoogleAnalytics     = null;
    protected              Tracker         tracker              = null;
    protected              boolean         mCrashUncaughtEnable = false;

    protected Map<String, Tracker> mTrackers = null;

    public AnalyticsGoogle(Context context) {
        mTrackers = new HashMap<String, Tracker>();
        mContext = context;
        mGoogleAnalytics = GoogleAnalytics.getInstance(context);
    }

    private void logD(String msg) {
        if (isDebug) {
            Log.d(LOG_TAG, msg);
        }
    }

    public void configureTracker(String trackerId) {
        logD("configureTracker: id = " + trackerId);
        if (null == trackerId) {
            logD("Null tracker id at configure time.");
            return;
        }

        createTracker(trackerId);
    }

    public void createTracker(String trackerId) {
        logD("createTracker: id = " + trackerId);
        Tracker tr = mTrackers.get(trackerId);
        if (null == tr) {
            tr = this.mGoogleAnalytics.newTracker(trackerId);


            this.mTrackers.put(trackerId, tr);
        }

        enableTracker(trackerId);
    }

    public void enableTracker(String trackerId) {
        logD("enableTracker: id = " + trackerId);
        if (null == trackerId) {
            return;
        }

        Tracker tr = mTrackers.get(trackerId);
        if (null == tr) {
            logD("Trying to enable unknown tracker: " + trackerId);
        } else {
            logD("Selected tracker: " + trackerId);
            this.tracker = tr;
        }
    }

    public void setLogLevel(int logLevel) {
        mGoogleAnalytics.getLogger().setLogLevel(logLevel);
    }

    public void dispatchHits() {
        mGoogleAnalytics.dispatchLocalHits();
    }

    public void dispatchPeriodically(int numberOfSeconds) {
        mGoogleAnalytics.setLocalDispatchPeriod(numberOfSeconds);
    }

    public void stopPeriodicalDispatch() {
        mGoogleAnalytics.setLocalDispatchPeriod(-1);
    }

    @Override
    public void startSession(String appKey) {
        if (this.tracker != null) {
            this.tracker.setScreenName(appKey);
            this.tracker.send(new HitBuilders.ScreenViewBuilder().setNewSession().build());
        } else { Log.e(LOG_TAG, "Start session called w/o valid tracker."); }
    }

    @Override
    public void stopSession() {
        if (this.tracker != null) {
            this.tracker.send((new HitBuilders.ScreenViewBuilder().set("&sc", "end")).build());
        } else { Log.e(LOG_TAG, "Start session called w/o valid tracker."); }
    }

    public void trackScreen(String screenName) {
        logD("trackScreen: name = " + screenName);
        if (null != this.tracker) {
            this.tracker.setScreenName(screenName);
            this.tracker.send(new HitBuilders.ScreenViewBuilder().build());
        } else {
            Log.e(LOG_TAG, "Log Screen called w/o valid tracker.");
        }
    }

    public void trackEventWithCategory(String category, String action, String label, int value) {
        logD(String.format(Locale.getDefault(),
            "trackEvent: category = %s action = %s label = %s value = %d", category, action, label,
            value));
        if (null != this.tracker) {
            this.tracker.send(new HitBuilders.EventBuilder()
                .setCategory(category)
                .setAction(action)
                .setLabel(label)
                .setValue(value)
                .build());
        } else { Log.e(LOG_TAG, "Log Event called w/o valid tracker."); }
    }

    public void trackEventWithCategory(JSONObject jsonObject) {
        try {
            String category = jsonObject.getString("Param1");
            String action = jsonObject.getString("Param2");
            String label = jsonObject.getString("Param3");
            int value = jsonObject.getInt("Param4");

            trackEventWithCategory(category, action, label, value);
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

    public void trackException(String description, boolean fatal) {
        if (null != this.tracker) {
            this.tracker.send(new HitBuilders.ExceptionBuilder()
                .setDescription(description)
                .setFatal(fatal)
                .build());
        } else { Log.e(LOG_TAG, "Log Exception called w/o valid tracker."); }
    }

    public void trackExceptionWithDescription(Hashtable<String, String> params) {
        String description = params.get("Param1");
        boolean isFatal = Boolean.parseBoolean(params.get("Param2"));

        trackException(description, isFatal);
    }

    public void trackTiming(String category, int interval, String name, String label) {
        logD(String.format(Locale.getDefault(),
            "trackTiming: category = %s interval = %d name = %s label = %s", category, interval,
            name, label));
        if (null != this.tracker) {
            this.tracker.send(new HitBuilders.TimingBuilder()
                .setCategory(category)
                .setValue(interval)
                .setVariable(name)
                .setLabel(label)
                .build());
        } else { Log.e(LOG_TAG, "Log Timing called w/o valid tracker."); }
    }

    public void trackTimingWithCategory(JSONObject params) {
        try {
            String category = params.getString("Param1");
            int interval = params.getInt("Param2");
            String name = params.getString("Param3");
            String label = params.getString("Param4");

            trackTiming(category, interval, name, label);
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

    public void trackEcommerceTransactions(JSONObject params) {
        try {
            String identity = params.getString("Param1");
            String name = params.getString("Param2");
            String category = params.getString("Param3");
            double price = params.getDouble("Param4");

            trackEcommerceTransactions(identity, name, category, price);
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

    public void trackEcommerceTransactions(String id, String name, String category, double price) {
        logD(String.format(Locale.getDefault(),
            "trackEcommerceTransactions: id = %s name = %s category = %s price = %f", id, name,
            category, price));
        String productID = String.format("Product-%s", id);
        String transactionID = String.format("Transaction-%s", id);

        Product product =
            new Product().setId(productID).setName(name).setCategory(category).setPrice(price);
        ProductAction productAction = new ProductAction(ProductAction.ACTION_PURCHASE)
            .setTransactionId(transactionID)
            .setTransactionRevenue(price);
        HitBuilders.ScreenViewBuilder builder =
            new HitBuilders.ScreenViewBuilder().addProduct(product).setProductAction(productAction);
        if (this.tracker != null) {
            this.tracker.setScreenName("transaction");
            this.tracker.send(builder.build());
        } else { Log.e(LOG_TAG, "Log Ecommerce Transactions called w/o valid tracker."); }
    }

    public void trackSocial(String network, String action, String target) {
        if (this.tracker != null) {
            this.tracker.send(new HitBuilders.SocialBuilder()
                .setNetwork(network)
                .setAction(action)
                .setTarget(target)
                .build());
        } else { Log.e(LOG_TAG, "Log Social called w/o valid tracker."); }
    }

    public void trackSocialWithNetwork(Hashtable<String, String> params) {
        String network = params.get("Param1");
        String action = params.get("Param2");
        String target = params.get("Param3");

        trackSocial(network, action, target);
    }

    void setDryRun(boolean isDryRun) {
        mGoogleAnalytics.setDryRun(isDryRun);
    }

    void enableAdvertisingTracking(boolean enabled) {
        if (null != this.tracker) { this.tracker.enableAdvertisingIdCollection(enabled); } else {
            Log.e(LOG_TAG, "Advertising called w/o valid tracker.");
        }
    }

    @Override
    public void setSessionContinueMillis(int millis) {
        Log.i(LOG_TAG, "Not supported on Android");
    }

    @Override
    public void setCaptureUncaughtException(boolean isEnabled) {
        mCrashUncaughtEnable = isEnabled;

        if (isEnabled) {
            if (null != tracker) {
                UncaughtExceptionHandler myHandler =
                    new ExceptionReporter(tracker, Thread.getDefaultUncaughtExceptionHandler(),
                        mContext);
                Thread.setDefaultUncaughtExceptionHandler(myHandler);
            } else {
                Log.e(LOG_TAG, "setCaptureUncaughtException called w/o valid tracker.");
            }
        } else {
            Thread.setDefaultUncaughtExceptionHandler(null);
        }
    }

    @Override
    public void setDebugMode(boolean isDebugMode) {
        isDebug = isDebugMode;
        setDryRun(isDebug);
    }

    @Override
    public void logError(String errorId, String message) {
        Log.i(LOG_TAG, "Not supported on Android");
    }

    @Override
    public void logEvent(String eventId) {
        Log.i(LOG_TAG, "Not supported on Android");
    }

    @Override
    public void logEvent(String eventId, Hashtable<String, String> paramMap) {
        Log.i(LOG_TAG, "Not supported on Android");
    }

    @Override
    public void logTimedEventBegin(String eventId) {
        Log.i(LOG_TAG, "Not supported on Android");
    }

    @Override
    public void logTimedEventEnd(String eventId) {
        Log.i(LOG_TAG, "Not supported on Android");
    }

    @Override
    public String getSDKVersion() {
        return "7.5.71";
    }

    @Override
    public String getPluginVersion() {
        return "0.9.0";
    }

}
