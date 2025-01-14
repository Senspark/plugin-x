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
#ifndef __CCX_PROTOCOL_USER_H__
#define __CCX_PROTOCOL_USER_H__

#include "PluginProtocol.h"
#include <string>
#include <functional>

namespace cocos2d { namespace plugin {

typedef std::map<std::string, std::string> TUserInfo;

enum class UserActionResultCode {
    kLoginSucceed   = 0,
    kLoginFailed    = 1,
    kLogoutSucceed  = 2,
    kLogoutFailed   = 3,
};

enum class GraphResult {
    kGraphResultSuccess = 4,
    kGraphResultFail    = 5,
    kGraphResultCancel  = 6,
    kGraphResultTimeout = 7,
};

enum class PermissionListResult{
    kPermissionListSuccess  = 8,
    kPermissionListFailed   = 9,
};
    
enum class UserPermissionResult{
    kPermissionSucceed  = 10,
    kPermissionFailed   = 11,
};

class ProtocolUser : public PluginProtocol
{
public:
    ProtocolUser();
    virtual ~ProtocolUser();

    typedef std::function<void(int, std::string&)> UserCallback;
    
    typedef struct __CallbackWrapper {
        __CallbackWrapper(UserCallback& callback) {
            fnPtr = callback;
        }
        
        UserCallback fnPtr;
    } CallbackWrapper;
    
    /**
    @brief config the application info
    @param devInfo This parameter is the info of aplication,
           different plugin have different format
    @warning Must invoke this interface before other interfaces.
             And invoked only once.
    */
    void configDeveloperInfo(TUserInfo devInfo);

    /**
     @brief User login
     */
    void login();
    void login(UserCallback &cb);

    /**
     @brief User logout
     */
    void logout();
    void logout(UserCallback &cb);

    bool isLoggedIn();
    /**
     @brief Get session ID
     @return If user logined, return value is session ID;
             else return value is empty string.
     */
    std::string getSessionID();
    
    /**
     @brief get Access Token
     */
    std::string getAccessToken();

    /**
     * @brief get User ID
     */
    std::string getUserID();

    std::string getAvatarUrl();

    std::string getDisplayName();


    /**
     @brief set login callback function
     */
    inline void setCallback(const UserCallback &cb)
    {
        _callback = cb;
    }

    /**
     @brief get login callback function
     */
    inline UserCallback& getCallback()
    {
        return _callback;
    }

protected:
    UserCallback _callback;
};

}} // namespace cocos2d { namespace plugin {

#endif /* __CCX_PROTOCOL_USER_H__ */
