//
//  GameViewController.m
//  BBPlayer
//
//  Created by Nik Rudenko on 9/12/17.
//
//

#import "GameViewController.h"
#import <OpenGLES/ES2/glext.h>
#import "PTModelController.h"
#import "PTModelGeneralSettings.h"
#import "PTPAppDelegate.h"
#import "cocos2d.h"
#import "PTPConfig.h"
#include "PTPSettingsController.h"


#define IOS_MAX_TOUCHES_COUNT     10

static PTPAppDelegate s_sharedApplication;

@interface GameViewController () {
    NSString* shareMessage;
    bool sheduledForShareWidget;
}
@property (strong, nonatomic) EAGLContext *context;

@end

@implementation GameViewController

- (void)viewDidLoad{
    [super viewDidLoad];
    
    sheduledForShareWidget = false;
    self.context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];

    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    [view setMultipleTouchEnabled: YES];
    
    [self setPreferredFramesPerSecond:60];
    [EAGLContext setCurrentContext:self.context];
    
    PTModelController *mc = PTModelController::shared();
    mc->clean();
    
    unsigned long size = 0;
    char* pBuffer = (char*)CCFileUtils::sharedFileUtils()->getFileData("data/data.pkg", "rb", &size);
    if (pBuffer != NULL && size > 0){
        mc->setUsingDataEncryption( true );
    }
    
    mc->loadDataForSplashScreen("data/data.pkg", processor().c_str());
    
    s_sharedApplication.setDataArchiveProcessor(processor());
    
    
    cocos2d::CCApplication::sharedApplication()->run();
}

- (void)dealloc{
    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
}

- (void)didReceiveMemoryWarning{
    [super didReceiveMemoryWarning];

    if ([self isViewLoaded] && ([[self view] window] == nil)) {
        self.view = nil;
        
        if ([EAGLContext currentContext] == self.context) {
            [EAGLContext setCurrentContext:nil];
        }
        self.context = nil;
    }

    // Dispose of any resources that can be recreated.
}

- (BOOL)prefersStatusBarHidden {
    return YES;
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    cocos2d::CCDirector::sharedDirector()->setViewport();
    cocos2d::CCDirector::sharedDirector()->mainLoop();
}


- (void)update{
    if(sheduledForShareWidget == true){
        sheduledForShareWidget = false;
        
        GLKView *view  = (GLKView *)self.view;
        UIImage* screenshot = view.snapshot;
        
        PTLog("Opens Share Widget: screenshot was taken");
        
        UIActivityViewController *activityVC = [[UIActivityViewController alloc] initWithActivityItems:@[shareMessage, screenshot] applicationActivities:nil];
        
        NSArray *excludeActivities = @[UIActivityTypeSaveToCameraRoll,
                                       UIActivityTypeAssignToContact];
        activityVC.excludedActivityTypes = excludeActivities;
        
        
        float iOSVersion = [[UIDevice currentDevice].systemVersion floatValue];
        if(iOSVersion > 8.0){
            activityVC.popoverPresentationController.sourceView = self.view;
        }
        
        [self presentViewController:activityVC animated:YES completion:nil];
        PTLog("opens Share Widget: view controller presented");
    }
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(nullable UIEvent *)event{
    int ids[IOS_MAX_TOUCHES_COUNT] = {0};
    float xs[IOS_MAX_TOUCHES_COUNT] = {0.0f};
    float ys[IOS_MAX_TOUCHES_COUNT] = {0.0f};
    
    int i = 0;
    for (UITouch *touch in touches) {
        ids[i] = (intptr_t)touch;
        xs[i] = [touch locationInView: [touch view]].x * self.view.contentScaleFactor;
        ys[i] = [touch locationInView: [touch view]].y * self.view.contentScaleFactor;
        ++i;
    }
    cocos2d::CCEGLView::sharedOpenGLView()->handleTouchesBegin(i, ids, xs, ys);
}

- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(nullable UIEvent *)event{
    int ids[IOS_MAX_TOUCHES_COUNT] = {0};
    float xs[IOS_MAX_TOUCHES_COUNT] = {0.0f};
    float ys[IOS_MAX_TOUCHES_COUNT] = {0.0f};
    
    int i = 0;
    for (UITouch *touch in touches) {
        ids[i] = (intptr_t)touch;
        xs[i] = [touch locationInView: [touch view]].x * self.view.contentScaleFactor;;
        ys[i] = [touch locationInView: [touch view]].y * self.view.contentScaleFactor;;
        ++i;
    }
    cocos2d::CCEGLView::sharedOpenGLView()->handleTouchesMove(i, ids, xs, ys);
}

- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(nullable UIEvent *)event{
    int ids[IOS_MAX_TOUCHES_COUNT] = {0};
    float xs[IOS_MAX_TOUCHES_COUNT] = {0.0f};
    float ys[IOS_MAX_TOUCHES_COUNT] = {0.0f};
    
    int i = 0;
    for (UITouch *touch in touches) {
        ids[i] = (intptr_t)touch;
        xs[i] = [touch locationInView: [touch view]].x * self.view.contentScaleFactor;;
        ys[i] = [touch locationInView: [touch view]].y * self.view.contentScaleFactor;;
        ++i;
    }
    cocos2d::CCEGLView::sharedOpenGLView()->handleTouchesEnd(i, ids, xs, ys);
}

- (void)touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(nullable UIEvent *)event{
    int ids[IOS_MAX_TOUCHES_COUNT] = {0};
    float xs[IOS_MAX_TOUCHES_COUNT] = {0.0f};
    float ys[IOS_MAX_TOUCHES_COUNT] = {0.0f};
    
    int i = 0;
    for (UITouch *touch in touches) {
        ids[i] = (intptr_t)touch;
        xs[i] = [touch locationInView: [touch view]].x * self.view.contentScaleFactor;;
        ys[i] = [touch locationInView: [touch view]].y * self.view.contentScaleFactor;;
        ++i;
    }
    cocos2d::CCEGLView::sharedOpenGLView()->handleTouchesCancel(i, ids, xs, ys);
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    PTModelGeneralSettingsPtr generalSettings = PTModelGeneralSettings::shared();
    if(generalSettings->orientation() == PTModelGeneralSettings::LandscapeOrientation){
        return UIInterfaceOrientationIsLandscape( interfaceOrientation );
    }
    else if(generalSettings->orientation() == PTModelGeneralSettings::PortraitOrientation){
        return UIInterfaceOrientationIsPortrait( interfaceOrientation );
    }
    
    return NO;
}

- (NSUInteger) supportedInterfaceOrientations{
    PTModelGeneralSettingsPtr generalSettings = PTModelGeneralSettings::shared();
    if(generalSettings->orientation() == PTModelGeneralSettings::LandscapeOrientation){
        return UIInterfaceOrientationMaskLandscape;
        
    }
    else if(generalSettings->orientation() == PTModelGeneralSettings::PortraitOrientation){
        return UIInterfaceOrientationMaskPortrait;
    }
    
    return NO;
}

- (BOOL) shouldAutorotate {
    return NO;
}

-(void) scheduleOpenShareWidget:(const char*) message{
    shareMessage = [NSString stringWithUTF8String:message];
    sheduledForShareWidget = true;
}

@end
