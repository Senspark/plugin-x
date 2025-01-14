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
#ifndef __CCX_IPLUGIN_H__
#define __CCX_IPLUGIN_H__

#include "PluginParam.h"
#include <vector>

#if defined(__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
    #define CC_DEPRECATED_ATTRIBUTE __attribute__((deprecated))
#elif _MSC_VER >= 1400 //vs 2005 or higher
    #define CC_DEPRECATED_ATTRIBUTE __declspec(deprecated) 
#else
    #define CC_DEPRECATED_ATTRIBUTE
#endif 

#define NS_CC_PLUGIN_BEGIN  namespace cocos2d { namespace plugin {
#define NS_CC_PLUGIN_END    }}
#define USING_NS_CC_PLUGIN  using namespace cocos2d::plugin

NS_CC_PLUGIN_BEGIN
/**
	The super class for all plugins.
*/
class PluginFactory;
class ProtocolPlatform;
class PluginProtocol
{
public:
    virtual ~PluginProtocol();

    /**
    @brief plug-in info methods(name, version, SDK version)
    */
    inline const char* getPluginName() { return _pluginName.c_str(); }
    std::string getPluginVersion();
    std::string getSDKVersion();

    /**
    @brief switch debug plug-in on/off
    */
    void setDebugMode(bool bDebug);

    /**
     * @brief methods for reflections
     */
    void callFuncWithParam(const char* funcName, PluginParam* param, ...);
    void callFuncWithParam(const char* funcName, std::vector<PluginParam*> params);

    std::string callStringFuncWithParam(const char* funcName, PluginParam* param, ...);
    std::string callStringFuncWithParam(const char* funcName, std::vector<PluginParam*> params);

    int callIntFuncWithParam(const char* funcName, PluginParam* param, ...);
    int callIntFuncWithParam(const char* funcName, std::vector<PluginParam*> params);
    
    bool callBoolFuncWithParam(const char* funcName, PluginParam* param, ...);
    bool callBoolFuncWithParam(const char* funcName, std::vector<PluginParam*> params);
    
    long callLongFuncWithParam(const char* funcName, PluginParam* param, ...);
    long callLongFuncWithParam(const char* funcName, std::vector<PluginParam*> params);

    float callFloatFuncWithParam(const char* funcName, PluginParam* param, ...);
    float callFloatFuncWithParam(const char* funcName, std::vector<PluginParam*> params);
    
    double callDoubleFuncWithParam(const char* funcName, PluginParam* param, ...);
    double callDoubleFuncWithParam(const char* funcName, std::vector<PluginParam*> params);

protected:
    PluginProtocol() {}

    friend class PluginFactory;
    friend class ProtocolPlatform;

    inline void setPluginName(const char* name) {
    	_pluginName = name;
    }
    std::string _pluginName;
};

NS_CC_PLUGIN_END //namespace cocos2d { namespace plugin {

#endif /* __CCX_IPLUGIN_H__ */
