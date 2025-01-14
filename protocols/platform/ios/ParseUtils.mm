/****************************************************************************
 Copyright (c) 2014 Chukong Technologies Inc.
 
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


#import "ParseUtils.h"
#include <string>
#include <map>

using namespace std;

@implementation ParseUtils
+ (id)NSStringToArrayOrNSDictionary:(NSString *)jsonData{
    NSError *error = nil;
    NSData *stringData = [jsonData dataUsingEncoding:NSUTF8StringEncoding];
    id jsonObject = [NSJSONSerialization JSONObjectWithData:stringData options:NSJSONReadingAllowFragments | NSJSONReadingMutableContainers error:&error];
    
    if (jsonObject != nil && error == nil){
        return jsonObject;
    }else{
        return nil;
    }
    
}

+ (NSString*) NSArrayToNSString:(id) arr {
    NSString *result = @"";
    if (arr) {
        NSError *error = nil;
        NSData *jsonData = [NSJSONSerialization dataWithJSONObject:arr options:NSJSONWritingPrettyPrinted error:&error];
        if (! jsonData) {
            NSLog(@"Got an error: %@", error);
        } else {
            result = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
        }
    }
    return result;
}

+ (NSString *) NSDictionaryToNSString:(id)dic{
    NSString *result = @"";
    if (dic) {
        NSError *error;
        NSData *jsonData = [NSJSONSerialization dataWithJSONObject:dic
                                                           options:NSJSONWritingPrettyPrinted
                                                             error:&error];
        if (! jsonData) {
            NSLog(@"Got an error: %@", error);
        } else {
            result = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
        }
    }
    return result;
}

+(NSString *)MakeJsonStringWithObject:(id) obj andKey:(NSString *)key {
    return [ParseUtils NSDictionaryToNSString:[NSDictionary dictionaryWithObject:obj forKey:key]];
}

+ (map<string, string>) createMapFromDict: (NSDictionary*) paramDict {
    map<string, string> ret;
    if (paramDict != nil) {
        for (NSString* key in [paramDict allKeys])
            ret[[key UTF8String]] = [(NSString*) [paramDict objectForKey:key] UTF8String];
    }
    return ret;
}

@end
