/****************************************************************************
Copyright (c) 2012-2013 cocos2d-x.org

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
#ifndef __TEST_ADS_SCENE_H__
#define __TEST_ADS_SCENE_H__

#include "cocos2d.h"
#include "Configs.h"
#include "ProtocolAds.h"
#include "SensparkPlugin.h"

NS_SENSPARK_BEGIN

class MyAdsListener : public cocos2d::plugin::AdsListener
{
public:
	virtual void onAdsResult(cocos2d::plugin::AdsResultCode code, const char* msg);
	virtual void onPlayerGetPoints(cocos2d::plugin::ProtocolAds* pAdsPlugin, int points);
};

class TestSensparkAds : public cocos2d::Layer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* scene();
    
    // a selector callback
    void menuBackCallback(Ref* pSender);
    void testShow(Ref* pSender);
	void testHide(Ref* pSender);

	// option changed callback
	void caseChanged(Ref* pSender);
	void posChanged(Ref* pSender);
	void typeChanged(Ref* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(TestSensparkAds);

private:
    plugin::ads::AdmobProtocolAds* _admob;
    plugin::ads::VungleProtocolAds* _vungle;
    cocos2d::plugin::ProtocolAds* _flurryAds;
    cocos2d::plugin::ProtocolAds* _facebookAds;
    
    MyAdsListener* _listener;

    cocos2d::MenuItemToggle* _caseItem;
    cocos2d::MenuItemToggle* _posItem;
    cocos2d::MenuItemToggle* _typeItem;

    cocos2d::plugin::ProtocolAds* _ads;
    cocos2d::plugin::ProtocolAds::AdsPos _pos;
    
    cocos2d::plugin::TAdsInfo adInfo;
};

NS_SENSPARK_END

#endif // __TEST_ADS_SCENE_H__
