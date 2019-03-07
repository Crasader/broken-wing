//
//  AppDelegate.m
//  BBPlayer
//
//  Created by Nik Rudenko on 9/12/17.
//
//

#import "AppDelegate.h"
#import <GLKit/GLKit.h>
#include "PTPSettingsController.h"
#include "libs/cocos2dx/include/audio/include/SimpleAudioEngine.h"

@interface AppDelegate ()

@end

@implementation AppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
        
    return YES;
}


- (void)applicationWillResignActive:(UIApplication *)application {
    cocos2d::CCDirector::sharedDirector()->pause();
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
    cocos2d::CCApplication::sharedApplication()->applicationDidEnterBackground();
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
    cocos2d::CCApplication::sharedApplication()->applicationWillEnterForeground();
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
    cocos2d::CCDirector::sharedDirector()->resume();
}

- (void)applicationWillTerminate:(UIApplication *)application {
}

- (void)loadingDidComplete{
}

-(void)showCustomFullscreenAd{
}

- (void)screenOnEnter:(const char*) name{
}

- (void)screenOnExit:(const char*) name{
}

@end
