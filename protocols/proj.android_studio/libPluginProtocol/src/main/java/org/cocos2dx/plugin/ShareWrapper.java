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
package org.cocos2dx.plugin;

import android.util.Log;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.Hashtable;
import java.util.Set;
import java.util.Iterator;

public class ShareWrapper {
	public static final int SHARERESULT_SUCCESS = 0;
	public static final int SHARERESULT_FAIL    = 1;
	public static final int SHARERESULT_CANCEL  = 2;
	public static final int SHARERESULT_TIMEOUT = 3;

	public static void onShareResult(final InterfaceShare obj, final int ret, final Hashtable<String, String> content, final String msg, final int callbackID) {

		PluginWrapper.runOnGLThread(new Runnable() {
			@Override
			public void run() {
				String name = obj.getClass().getName();
				name = name.replace('.', '/');

				JSONObject json = new JSONObject();

				Set<String> keySet = content.keySet();

				try {
					for (String key : keySet) {
						json.put(key, content.get(key));
					}
				} catch (JSONException ex) {
					ex.printStackTrace();
				}

				nativeOnShareResult(name, ret, json, msg, callbackID);
			}
		});

	}
	private static native void nativeOnShareResult(String className, int ret, JSONObject json, String msg, int callbackID);
}