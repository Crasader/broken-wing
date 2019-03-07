#ifndef PTPSCREEN_H
#define PTPSCREEN_H

#include "PTAnimationCurve.h"
#include "models/PTModelDeclaration.h"

#include <cocos2d.h>

#include <functional>

#define kRootLayerItemTag 0
#define kRootCCMenuItemTag 100

class PTPScreenScene;
class PTSoundController;
class PTPObjectButton;

PT_MODEL_DECL(PTModelScreen);
PT_MODEL_DECL(PTModelObjectButton);
PT_MODEL_DECL(PTModelAssetPowerup);
PT_MODEL_DECL(PTModelObjectLabel);
PT_MODEL_DECL(PTModelObject);

class PTPScreen : public CCLayer
{
public:
	PTPScreen();
    ~PTPScreen();
    virtual bool init();
    static cocos2d::CCScene* scene();
    static PTPScreen *getRunningScreen();

    CREATE_FUNC( PTPScreen );

    bool load(const char* key, bool createObjects = true);
    PTPObjectButton* getButton( const char* key );
    std::vector<PTPObjectButton*> getButtons(const std::string &key) const { return getButtons(key.c_str()); }
    std::vector<PTPObjectButton*> getButtons(const CCString &key) const { return getButtons(key.getCString()); }
    std::vector<PTPObjectButton*> getButtons(const char *key) const;
    PTModelObjectLabelPtr getLabel( const char* key);

    void update(float dt);

    virtual void onKeyUp( int keyCode );

    virtual void stopBackgroundMusic();
    virtual void playBackgroundMusic();

    virtual void visit();

    virtual void reset();
    virtual void removeAds();

    virtual void pressHilightedButton();
    virtual void hilightButton(int direction);
    virtual void hilightDefaultButton();


    virtual void deleteObject(const PTModelObjectPtr &model);
    void hideAds();
    virtual void showAds();

    virtual bool isBannerWillShow();
    bool isBannerWillShow(const char* screenModelName);
    bool isInterstitialWillShow();
    bool isInterstitialWillShow(const char* screenModelName);

    virtual PTPScreenScene *parentScreenScene();
    virtual void onBackToParentScreen();
    virtual void defaultBackActionHandler();

    virtual void onEnter();

    virtual void removeFromParent();

    void setUiLayerVisible( bool );
    void setChildrenButtonsEnabled( bool );

    void ignoreNextInterstitial();

    virtual void activatePowerup( PTModelAssetPowerupPtr powerup, bool aboveCharacter = true);

    void addChildScreen(PTPScreen *);
    PTPScreen *getChildScreen();
    static PTPScreen *getTopScreen();

    inline PTModelScreenPtr model() const noexcept { return _model; }

    void powerupButtonAction(CCObject* sender);
    void purchaseButtonAction(CCObject* sender);

    void playCloseAnimation(std::function<void()> finishCallback = std::function<void()>());

    inline PTAnimationCurve::AnimationState animationState() const noexcept { return _animationState; }

protected:
    PTModelScreenPtr _model;

    void assignActionToButtons(const char* buttonId, CCObject* target, SEL_MenuHandler selector);
    void assignActionsToButtons(const char* buttonId, CCObject* target, SEL_MenuHandler selectedSelector, SEL_MenuHandler unselectedSelector);

    virtual void buttonAction(CCObject* pSender);
    virtual void onButtonPressed(const PTModelObjectButtonPtr &button);

    void restorePurchasesButtonAction(CCObject* pSender);
    virtual void gameServicesButtonAction(CCObject* pSender);

    virtual PTModelScreenPtr getModelForKey(const char *key);

    void hideUiChildren();

    std::map<PTModelObjectPtr, CCNode*> _objectMap;

    CCNode *_uiNode;
    float _uiNodeUnhideTimeOut;
    float _uiNodeScheduleForUnhide;

    void switchToScene( CCScene* );

    bool _ignoreNextInterstitialShow;

    void setAnimationState( PTAnimationCurve::AnimationState state );

    void closeAnimationCallback(float);

    PTSoundController *_soundController;

private:
    CCLabelBMFont *_distanceSessionLabel;
    CCLabelBMFont *_distanceBestLabel;

    CCLabelBMFont *_coinsSessionLabel;
    CCLabelBMFont *_coinsTotalLabel;

    PTAnimationCurve::AnimationState _animationState;
    float _currentTime;

    std::function<void()> _closeAnimationCallback;
};

#endif // PTPSCREEN_H
