//
//  GoogleProtocolAnalyticsImpl.cpp
//  PluginSenspark
//
//  Created by Zinge on 8/11/16.
//  Copyright © 2016 Senspark Co., Ltd. All rights reserved.
//

#include <cassert>
#include <sstream>

#include "GoogleProtocolAnalytics.h"
#include "senspark/utility.hpp"

namespace senspark {
namespace plugin {
namespace analytics {
namespace {
/// Counts the number of character @c _ in the specified string.
constexpr std::size_t count_placeholders(const char* s, std::size_t index = 0) {
    return s[index] == '\0' ? 0 : (s[index] == '_') +
                                      count_placeholders(s, index + 1);
}

/// Converts float to string without trailing zero.
std::string float_to_string(float x) {
    std::stringstream ss;
    ss << x;
    return ss.str();
}

/// Merges two maps.
void merge(TrackingDict& lhs, const TrackingDict& rhs) {
    for (auto&& elt : rhs) {
        lhs[elt.first] = elt.second;
    }
}

template <std::size_t Placeholders,
          std::size_t Parameters = (Placeholders > 1 ? Placeholders : 1)>
class Parameter {
public:
    template <class... Args, class = typename std::enable_if<sizeof...(Args) ==
                                                             Parameters>::type>
    constexpr explicit Parameter(Args&&... args)
        : s_{args...} {}

    template <std::size_t Index,
              class = typename std::enable_if<(Index < Parameters)>::type>
    constexpr const char* get() const {
        return s_[Index];
    }

private:
    const char* s_[Parameters];
};

namespace parameters {
/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#t
/// Required for all hit types.
/// The type of hit. Must be one of 'pageview', 'screenview', 'event',
/// 'transaction', 'item', 'social', 'exception', 'timing'.
constexpr auto hit_type = Parameter<0>("t");

namespace types {
constexpr auto event = "event";
constexpr auto screen_view = "screenview";
constexpr auto timing = "timing";
constexpr auto social = "social";
constexpr auto exception = "exception";
} // namespace types

/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#cd
/// Required for screenview hit type.
constexpr auto screen_name = Parameter<0>("cd");

/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#ec
/// Required for event hit type.
/// Specifies the event category. Must not be empty.
constexpr auto event_category = Parameter<0>("ec");

/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#ea
/// Required for event hit type.
/// Specifies the event action. Must not be empty.
constexpr auto event_action = Parameter<0>("ea");

/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#el
/// Optional for event hit type.
/// Specifies the event label.
constexpr auto event_label = Parameter<0>("el");

/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#ev
/// Optional for event hit type.
/// Specifies the event value. Values must be non-negative.
constexpr auto event_value = Parameter<0>("ev");

/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#utc
/// Required for timing hit type.
/// Specifies the user timing category.
constexpr auto timing_category = Parameter<0>("utc");

/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#utv
/// Required for timing hit type.
/// Specifies the user timing variable.
constexpr auto timing_variable_name = Parameter<0>("utv");

/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#utt
/// Required for timing hit type.
/// Specifies the user timing value. The value is milliseconds.
constexpr auto timing_time = Parameter<0>("utt");

/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#utl
/// Optional for timing hit type.
/// Specifies the user timing label.
constexpr auto timing_label = Parameter<0>("utl");

/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#sn
/// Required for social hit type.
/// Specifies the social network, for example Facebook or Google Plus.
constexpr auto social_network = Parameter<0>("sn");

/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#sa
/// Required for social hit type.
/// Specifies the social interaction action. For example on Google Plus when a
/// user clicks the +1 button, the social action is 'plus'.
constexpr auto social_action = Parameter<0>("sa");

/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#st
/// Required for social hit type.
/// Specifies the target of a social interaction. This value is typically a URL
/// but can be any text.
constexpr auto social_action_target = Parameter<0>("st");

/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#exd
/// Optional for exception hit type.
/// Specifies the description of an exception.
constexpr auto exception_description = Parameter<0>("exd");

/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#exf
/// Optional for exception hit type.
/// Specifies whether the exception was fatal.
constexpr auto is_exception_fatal = Parameter<0>("exf");

/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#cd_
/// Optional for all hit types.
/// Each custom dimension has an associated index. There is a maximum of 20
/// custom dimensions (200 for Analytics 360 accounts). The dimension index must
/// be a positive integer between 1 and 200, inclusive.
constexpr auto custom_dimension = Parameter<1>("cd");

/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#cm_
/// Optional for all hit types.
/// Each custom metric has an associated index. There is a maximum of 20 custom
/// metrics (200 for Analytics 360 accounts). The metric index must be a
/// positive integer between 1 and 200, inclusive.
constexpr auto custom_metric = Parameter<1>("cm");

/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#pa
/// Optional for all hit types.
/// The role of the products included in a hit. If a product action is not
/// specified, all product definitions included with the hit will be ignored.
/// Must be one of: detail, click, add, remove, checkout, checkout_option,
/// purchase, refund. For analytics.js the Enhanced Ecommerce plugin must be
/// installed before using this field.
constexpr auto product_action = Parameter<0>("pa");

/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#ti
/// A unique identifier for the transaction. This value should be the same for
/// both the Transaction hit and Items hits associated to the particular
/// transaction.
constexpr auto transaction_id = Parameter<0>("ti");

/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#tr
/// Specifies the total revenue associated with the transaction. This value
/// should include any shipping or tax costs.
constexpr auto transaction_revenue = Parameter<0>("tr");

/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#pal
/// Optional for all hit types.
/// The list or collection from which a product action occurred. This is an
/// additional parameter that can be sent when Product Action is set to 'detail'
/// or 'click'. For analytics.js the Enhanced Ecommerce plugin must be installed
/// before using this field.
constexpr auto product_action_list = Parameter<0>("pal");

constexpr auto product_list_source = Parameter<0>("pls");

/// https://developers.google.com/analytics/devguides/collection/protocol/v1/parameters#pr_id
/// Product SKU.
/// Product Name.
/// Product Brand.
/// Product Category.
/// Product Variant.
/// Product Price.
/// Product Quantity.
/// Product Coupon Code.
/// Product Position.
/// Product Custom Dimension.
/// Product Custom Metric.
constexpr auto product_prefix = Parameter<1>("pr");

constexpr auto product_impression_prefix = Parameter<2>("il", "pi");

constexpr auto product_impression_list_name = Parameter<1, 2>("il", "nm");
} // namespace parameters

std::string make_parameter(const Parameter<0>& parameter) {
    return std::string("&") + parameter.get<0>();
}

std::string make_parameter(const Parameter<1>& parameter, std::size_t index) {
    return std::string("&") + parameter.get<0>() + std::to_string(index);
}

std::string make_parameter(const Parameter<1, 2>& parameter,
                           std::size_t index) {
    return std::string("&") + parameter.get<0>() + std::to_string(index) +
           parameter.get<1>();
}

std::string make_parameter(const Parameter<2>& parameter, std::size_t index0,
                           std::size_t index1) {
    return std::string("&") + parameter.get<0>() + std::to_string(index0) +
           parameter.get<1>() + std::to_string(index1);
}
} // namespace
Product& Product::setCategory(const std::string& value) {
    dict_["ca"] = value;
    return *this;
}

Product& Product::setId(const std::string& value) {
    dict_["id"] = value;
    return *this;
}

Product& Product::setName(const std::string& value) {
    dict_["nm"] = value;
    return *this;
}

Product& Product::setPrice(float price) {
    dict_["pr"] = float_to_string(price);
    return *this;
}

TrackingDict Product::build(std::size_t productIndex) const {
    return build(make_parameter(parameters::product_prefix, productIndex));
}

TrackingDict Product::build(std::size_t listIndex,
                            std::size_t productIndex) const {
    return build(make_parameter(parameters::product_impression_prefix,
                                listIndex, productIndex));
}

TrackingDict Product::build(const std::string& prefix) const {
    TrackingDict result;
    for (auto&& elt : dict_) {
        result[prefix + elt.first] = elt.second;
    }
    return result;
}

const std::string ProductAction::ActionAdd = "add";
const std::string ProductAction::ActionCheckout = "checkout";
const std::string ProductAction::ActionClick = "click";
const std::string ProductAction::ActionDetail = "detail";
const std::string ProductAction::ActionPurchase = "purchase";

ProductAction::ProductAction(const std::string& action) {
    dict_[make_parameter(parameters::product_action)] = action;
}

ProductAction& ProductAction::setProductActionList(const std::string& value) {
    dict_[make_parameter(parameters::product_action_list)] = value;
    return *this;
}

ProductAction& ProductAction::setProductListSource(const std::string& value) {
    dict_[make_parameter(parameters::product_list_source)] = value;
    return *this;
}

ProductAction& ProductAction::setTransactionId(const std::string& value) {
    dict_[make_parameter(parameters::transaction_id)] = value;
    return *this;
}

ProductAction& ProductAction::setTransactionRevenue(float value) {
    dict_[make_parameter(parameters::transaction_revenue)] =
        float_to_string(value);
    return *this;
}

TrackingDict ProductAction::build() const { return dict_; }

template <class T>
T& HitBuilders::Internal<T>::addImpression(const Product& product,
                                           const std::string& impressionList) {
    impressions_[impressionList].push_back(product);
    return static_cast<T&>(*this);
}

template <class T>
T& HitBuilders::Internal<T>::addProduct(const Product& product) {
    products_.push_back(product);
    return static_cast<T&>(*this);
}

template <class T>
T& HitBuilders::Internal<T>::setProductAction(const ProductAction& action) {
    productAction_.clear();
    productAction_.push_back(action);
    return static_cast<T&>(*this);
}

template <class T>
T& HitBuilders::Internal<T>::set(const std::string& paramName,
                                 const std::string& paramValue) {
    dict_[paramName] = paramValue;
    return static_cast<T&>(*this);
}

template <class T>
T& HitBuilders::Internal<T>::setCustomDimension(std::size_t index,
                                                const std::string& dimension) {
    return set(make_parameter(parameters::custom_dimension, index), dimension);
}

template <class T>
T& HitBuilders::Internal<T>::setCustomMetric(std::size_t index, float metric) {
    return set(make_parameter(parameters::custom_metric, index),
               float_to_string(metric));
}

template <class T> TrackingDict HitBuilders::Internal<T>::build() const {
    auto result = dict_;
    if (not products_.empty()) {
        std::size_t productIndex = 1;
        for (auto&& product : products_) {
            merge(result, product.build(productIndex));
            ++productIndex;
        }
    }
    if (not productAction_.empty()) {
        merge(result, productAction_[0].build());
    }
    if (not impressions_.empty()) {
        std::size_t listIndex = 1;
        for (auto&& elt : impressions_) {
            result[make_parameter(parameters::product_impression_list_name,
                                  listIndex)] = elt.first;
            std::size_t productIndex = 1;
            for (auto&& product : elt.second) {
                merge(result, product.build(listIndex, productIndex));
                ++productIndex;
            }
            ++listIndex;
        }
    }
    for (auto&& elt : result) {
        assert(elt.first[0] == '&');
    }
    return result;
}

template <class T>
T& HitBuilders::Internal<T>::setHitType(const std::string& hitType) {
    return set(make_parameter(parameters::hit_type), hitType);
}

template class HitBuilders::Internal<HitBuilders::ScreenViewBuilder>;
template class HitBuilders::Internal<HitBuilders::ExceptionBuilder>;
template class HitBuilders::Internal<HitBuilders::TimingBuilder>;
template class HitBuilders::Internal<HitBuilders::SocialBuilder>;
template class HitBuilders::Internal<HitBuilders::EventBuilder>;

HitBuilders::ScreenViewBuilder::ScreenViewBuilder() {
    setHitType(parameters::types::screen_view);
}

HitBuilders::ExceptionBuilder::ExceptionBuilder() {
    setHitType(parameters::types::exception);
}

HitBuilders::ExceptionBuilder&
HitBuilders::ExceptionBuilder::setDescription(const std::string& description) {
    return set(make_parameter(parameters::exception_description), description);
}

HitBuilders::ExceptionBuilder&
HitBuilders::ExceptionBuilder::setFatal(bool fatal) {
    return set(make_parameter(parameters::is_exception_fatal),
               std::to_string(fatal));
}

HitBuilders::TimingBuilder::TimingBuilder() {
    setHitType(parameters::types::timing);
}

HitBuilders::TimingBuilder::TimingBuilder(const std::string& category,
                                          const std::string& variable,
                                          int value)
    : TimingBuilder() {
    setCategory(category);
    setVariable(variable);
    setValue(value);
}

HitBuilders::TimingBuilder&
HitBuilders::TimingBuilder::setVariable(const std::string& variable) {
    return set(make_parameter(parameters::timing_variable_name), variable);
}

HitBuilders::TimingBuilder& HitBuilders::TimingBuilder::setValue(int value) {
    return set(make_parameter(parameters::timing_time), std::to_string(value));
}

HitBuilders::TimingBuilder&
HitBuilders::TimingBuilder::setCategory(const std::string& category) {
    return set(make_parameter(parameters::timing_category), category);
}

HitBuilders::TimingBuilder&
HitBuilders::TimingBuilder::setLabel(const std::string& label) {
    return set(make_parameter(parameters::timing_label), label);
}

HitBuilders::SocialBuilder::SocialBuilder() {
    setHitType(parameters::types::social);
}

HitBuilders::SocialBuilder&
HitBuilders::SocialBuilder::setNetwork(const std::string& network) {
    return set(make_parameter(parameters::social_network), network);
}

HitBuilders::SocialBuilder&
HitBuilders::SocialBuilder::setAction(const std::string& action) {
    return set(make_parameter(parameters::social_action), action);
}

HitBuilders::SocialBuilder&
HitBuilders::SocialBuilder::setTarget(const std::string& target) {
    return set(make_parameter(parameters::social_action_target), target);
}

HitBuilders::EventBuilder::EventBuilder() {
    setHitType(parameters::types::event);
}

HitBuilders::EventBuilder::EventBuilder(const std::string& category,
                                        const std::string& action)
    : EventBuilder() {
    setCategory(category);
    setAction(action);
}

HitBuilders::EventBuilder&
HitBuilders::EventBuilder::setCategory(const std::string& category) {
    return set(make_parameter(parameters::event_category), category);
}

HitBuilders::EventBuilder&
HitBuilders::EventBuilder::setAction(const std::string& action) {
    return set(make_parameter(parameters::event_action), action);
}

HitBuilders::EventBuilder&
HitBuilders::EventBuilder::setLabel(const std::string& label) {
    return set(make_parameter(parameters::event_label), label);
}

HitBuilders::EventBuilder& HitBuilders::EventBuilder::setValue(int value) {
    return set(make_parameter(parameters::event_value), std::to_string(value));
}

GoogleProtocolAnalytics::GoogleProtocolAnalytics() = default;

void GoogleProtocolAnalytics::configureTracker(const std::string& trackerId) {
    callFunction(this, "configureTracker", trackerId.c_str());
}

void GoogleProtocolAnalytics::createTracker(const std::string& trackerId) {
    callFunction(this, "createTracker", trackerId.c_str());
}

void GoogleProtocolAnalytics::enableTracker(const std::string& trackerId) {
    callFunction(this, "enableTracker", trackerId.c_str());
}

void GoogleProtocolAnalytics::setLogLevel(GALogLevel logLevel) {
    callFunction(this, "setLogLevel", static_cast<int>(logLevel));
}

void GoogleProtocolAnalytics::dispatchHits() {
    callFunction(this, "dispatchHits");
}

void GoogleProtocolAnalytics::dispatchPeriodically(int seconds) {
    callFunction(this, "dispatchPeriodically", seconds);
}

void GoogleProtocolAnalytics::stopPeriodicalDispatch() {
    callFunction(this, "stopPeriodicalDispatch");
}

void GoogleProtocolAnalytics::trackScreen(const std::string& screenName) {
    setScreenName(screenName);
    auto builder = HitBuilders::ScreenViewBuilder();
    sendHit(builder.build());
}

void GoogleProtocolAnalytics::trackEvent(const std::string& category,
                                         const std::string& action,
                                         const std::string& label, int value) {
    auto builder = HitBuilders::EventBuilder()
                       .setCategory(category)
                       .setAction(action)
                       .setLabel(label)
                       .setValue(value);
    sendHit(builder.build());
}

void GoogleProtocolAnalytics::trackException(const std::string& description,
                                             bool isFatal) {
    auto builder = HitBuilders::ExceptionBuilder()
                       .setDescription(description)
                       .setFatal(isFatal);
    sendHit(builder.build());
}

void GoogleProtocolAnalytics::trackTiming(const std::string& category,
                                          int interval, const std::string& name,
                                          const std::string& label) {
    auto builder = HitBuilders::TimingBuilder()
                       .setCategory(category)
                       .setValue(interval)
                       .setVariable(name)
                       .setLabel(label);
    sendHit(builder.build());
}

void GoogleProtocolAnalytics::trackEcommerceTransactions(
    const std::string& identity, const std::string& productName,
    const std::string& productCategory, float priceValue) {
    auto builder =
        HitBuilders::ScreenViewBuilder()
            .addProduct(Product()
                            .setId(identity)
                            .setCategory(productCategory)
                            .setName(productName)
                            .setPrice(priceValue))
            .setProductAction(ProductAction(ProductAction::ActionPurchase));
    sendHit(builder.build());
}

void GoogleProtocolAnalytics::trackSocial(const std::string& network,
                                          const std::string& action,
                                          const std::string& target) {
    auto builder = HitBuilders::SocialBuilder()
                       .setNetwork(network)
                       .setAction(action)
                       .setTarget(target);
    sendHit(builder.build());
}

void GoogleProtocolAnalytics::setDryRun(bool isDryRun) {
    callFunction(this, "setDryRun", isDryRun);
}

void GoogleProtocolAnalytics::enableAdvertisingTracking(bool enable) {
    callFunction(this, "enableAdvertisingTracking", enable);
}

void GoogleProtocolAnalytics::setParameter(const std::string& key,
                                           const std::string& value) {
    callFunction(this, "setParameter", key.c_str(), value.c_str());
}

void GoogleProtocolAnalytics::setScreenName(const std::string& screenName) {
    setParameter(make_parameter(parameters::screen_name), screenName);
}

void GoogleProtocolAnalytics::sendHit(const TrackingDict& parameters) {
    callFunction(this, "sendHit", parameters);
}

void GoogleProtocolAnalytics::doTests() {
    testTrackScreenView();
    testTrackEvent();
    testTrackTiming();
    testTrackException();
    testTrackSocial();
    testCustomDimensionAndMetric();
    testEcommerceImpression();
    testEcommerceAction();
}

void GoogleProtocolAnalytics::testTrackScreenView() {
    callFunction(this, "_testTrackScreenView",
                 HitBuilders::ScreenViewBuilder().build());
}

void GoogleProtocolAnalytics::testTrackEvent() {
    callFunction(this, "_testTrackEvent", HitBuilders::EventBuilder()
                                              .setCategory("category")
                                              .setAction("action")
                                              .setLabel("label")
                                              .setValue(1)
                                              .build());
}

void GoogleProtocolAnalytics::testTrackTiming() {
    callFunction(this, "_testTrackTiming", HitBuilders::TimingBuilder()
                                               .setCategory("category")
                                               .setVariable("variable")
                                               .setLabel("label")
                                               .setValue(1)
                                               .build());
}

void GoogleProtocolAnalytics::testTrackException() {
    callFunction(this, "_testTrackException", HitBuilders::ExceptionBuilder()
                                                  .setDescription("description")
                                                  .setFatal(true)
                                                  .build());
}

void GoogleProtocolAnalytics::testTrackSocial() {
    callFunction(this, "_testTrackSocial", HitBuilders::SocialBuilder()
                                               .setNetwork("network")
                                               .setAction("action")
                                               .setTarget("target")
                                               .build());
}

void GoogleProtocolAnalytics::testCustomDimensionAndMetric() {
    callFunction(this, "_testCustomDimensionAndMetric",
                 HitBuilders::ScreenViewBuilder()
                     .setCustomMetric(1, 1)
                     .setCustomMetric(2, 2)
                     .setCustomMetric(5, 5.5f)
                     .setCustomDimension(1, "dimension_1")
                     .setCustomDimension(2, "dimension_2")
                     .build());
}

void GoogleProtocolAnalytics::testEcommerceImpression() {
    auto product0 = Product()
                        .setCategory("category0")
                        .setId("id0")
                        .setName("name0")
                        .setPrice(1.5f);
    auto product1 = Product()
                        .setCategory("category1")
                        .setId("id1")
                        .setName("name1")
                        .setPrice(2.5f);
    auto product2 = Product()
                        .setCategory("category2")
                        .setId("id2")
                        .setName("name2")
                        .setPrice(3.0f);
    auto product3 = Product()
                        .setCategory("category3")
                        .setId("id3")
                        .setName("name3")
                        .setPrice(4);
    callFunction(this, "_testEcommerceImpression",
                 HitBuilders::ScreenViewBuilder()
                     .addImpression(product0, "impressionList0")
                     .addImpression(product1, "impressionList0")
                     .addImpression(product2, "impressionList1")
                     .addImpression(product3, "impressionList1")
                     .build());
}

void GoogleProtocolAnalytics::testEcommerceAction() {
    auto product0 = Product()
                        .setCategory("category0")
                        .setId("id0")
                        .setName("name0")
                        .setPrice(1.5f);
    auto product1 = Product()
                        .setCategory("category1")
                        .setId("id1")
                        .setName("name1")
                        .setPrice(2);
    auto action = ProductAction(ProductAction::ActionPurchase)
                      .setProductActionList("actionList")
                      .setProductListSource("listSource")
                      .setTransactionId("transactionId")
                      .setTransactionRevenue(2.0f);
    callFunction(this, "_testEcommerceAction", HitBuilders::ScreenViewBuilder()
                                                   .addProduct(product0)
                                                   .addProduct(product1)
                                                   .setProductAction(action)
                                                   .build());
}
} // namespace analytics
} // namespace plugin
} // namespace senspark
