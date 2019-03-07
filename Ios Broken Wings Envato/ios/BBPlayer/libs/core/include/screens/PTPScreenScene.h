#ifndef PTPSCREENSCENE_H
#define PTPSCREENSCENE_H

#include "PTPScreen.h"
#include "PTPScreensController.h"

#include <unordered_set>
#include <box2D/Box2D.h>

class PTPScreenUi;
class PTPInputController;
class PTPObjectAssetPowerup;
class PTPObjectAssetTeleport;
class PTPObjectAssetMirror;
class PTPGameFieldCamera;
class PTPObjectAsset;
class PTPObject;
class PTPObjectGeneralSettings;

PT_MODEL_DECL(PTModelScreenScene);
PT_MODEL_DECL(PTModelGeneralSettings);
PT_MODEL_DECL(PTModelLevelSection);
PT_MODEL_DECL(PTModelObject);

struct PTPScreenSceneAssetCreation {
    PTPObjectAsset* asset;
    float32 assetLinearDamping;
    b2Vec2 assetLinearVelocity;
    PTPObjectAsset *parent;
};

struct PTPScreenSceneCreationQueue {
    CCPoint position;
    CCArray *objects;
    int index;
    PTModelLevelSectionPtr model;
    CCArray* createdObjects;
};

class PTPScreenScene : public PTPScreen
{
public:
    enum RestartOperation {
        NoRestart = 0,
        Restart = 0x00001,
        ResetProgress = 0x00002,
        ResetOriginalUI = 0x00004,
        FullRestart = ResetProgress | ResetOriginalUI
    };

    PTPScreenScene();
    ~PTPScreenScene();

    PTModelScreenScenePtr model() const noexcept;

    static cocos2d::CCScene* scene();

    CREATE_FUNC( PTPScreenScene );

    virtual bool init();
    virtual void update(float dt);
    virtual void updateCameraObjects( float dt );

    virtual void visit();

    void cameraShake( float cameraShake );
    void cameraFlash( float cameraFlash );

    void loadBackgroundObjects();

    virtual bool isBannerWillShow();

    virtual void pressHilightedButton();
    virtual void hilightButton(int direction);

    void addAsset( PTPObjectAsset *asset, CCNode *sender );
    CCArray *getAssetsList();

    bool updateObjectCreationQueue();

    PTPGameFieldCamera* camera();

    void deleteAsset(PTModelObjectPtr assetModel);

    virtual void onBackFromChildScreen();

    void resetGameplay();
    void setDebugMode(bool enable);

    PTPInputController* inputController();

    void submitScoreAndUpdateBestScores();

    virtual void activatePowerup( PTModelAssetPowerupPtr powerup, bool aboveCharacter = true);
    PTPObjectAssetPowerup* activePowerup( CCString type,  PTPObject *sender);
    std::list<PTPObjectAssetPowerup*> powerupList(CCString type);

    void setPowerupMagnetMode( bool enabled );
    void setPowerupKillAllEnemy();
    void setPowerupCheckPoint( CCPoint point, int levelSectionId );
    void setPowerupNextCheckPoint();

    void resetPowerups();

    void scheduleGameRestart(RestartOperation operation = FullRestart);
    void schedulePause(bool state);

    b2World* world();

    virtual void onEnter();
    virtual void onKeyDown( int keyCode );
    virtual void onKeyUp( int keyCode );
    virtual void keyBackClicked();
    virtual void setTouchEnabled( bool enabled );

    void setGameSpeedSlowDown( float factor );

    void setGameOverMode( bool gameOverMode );
    bool isGameOverMode();
    void setPauseMode( bool pauseMode );
    bool isPauseMode();

    void pauseButtonAction( CCObject *sender );
    void backButttonAction( CCObject *pSender );
    void restartButtonAction( CCObject *sender );
    void endGameAction();

    PTPScreenUi *currentUI() const;

    void scheduleCloneObject(PTPObjectAsset *asset, PTPObjectAsset *parent);

    std::list<PTPObjectAssetTeleport *> teleportList( float groupId );

    void switchToNewUI();
    void loadOriginalUi();

    void restart(RestartOperation operation = FullRestart);

    void switchScene(const std::string &link, bool pause = false, bool restart = false, PTPScreensController::ScoresOperation scoresOperation = PTPScreensController::NoSaveScoresOperation);

    void removeShaderEffect(PTPObject *effect);

    void runScoresResetComponent();

protected:
    float addLevelSection(  PTModelLevelSectionPtr model, float position ); // return width of added level section
    PTPObjectAsset* createAsset(PTModelObjectPtr assetModel, int levelSectionModelId, CCPoint levelSectionPosition, bool characterVerification = true );
    PTPObjectAsset* createAsset(PTModelObjectPtr assetModel, PTPObjectAsset *parent);

    virtual void onButtonPressed(const PTModelObjectButtonPtr &button);

    virtual PTModelScreenPtr getModelForKey(const char *key);

    PTPGameFieldCamera *_camera;
    PTModelGeneralSettingsPtr _generalSettings;
    PTPObjectGeneralSettings* _generalSettingsObject;
    b2World *_world;

    CCArray *_backgroundObjects;

private:
    void checkForDeletion(PTPObject *obj);
    void checkIsCharacterCreated( unsigned int modelId ); // we checking is character created durign scene inititalization if not we will grab his position from Start Scene
    PTModelLevelSectionPtr getLevelSectionByDistance( float d );
    std::vector<std::size_t> levelSectionIndicesByDistance( float d, bool cooledDown); //return indices of level section that cooled down and not
    void levelSectionCooldownUpdate();
    void addStartLevelSection();

    void killAllEnemyOnObject( CCNode * );
    void resetPowerupsOnObject( CCNode * );

    void pauseAnimations();
    void resumeAnimations();

    void setNodeAnimationsState(CCNode *node, bool pause);

    PTPObjectAssetPowerup* findClosesCheckpoint(CCNode *node = nullptr);

    PTPInputController* _inputController;

    std::vector<PTModelLevelSectionPtr> _levelSectionModels;
    float _levelSectionCurentLength;

    float _levelSectionLength;

    CCScene *_debugScene;
    PTPScreenUi *_uiScreen;

    bool _pauseMode;
    bool _gameOverMode;

    //powerups
    bool _powerupMagnetMode;
    RestartOperation _scheduledGameRestartOperation;
    bool _scheduledPauseState;

    float _gameSpeedslowDownFactor;
    float _gameOverDelay;

    float _cameraShake;
    float _cameraFlash;
    float _cameraFlashDuration;
    CCSprite *_cameraFlashSprite;
    float _simulationTimeAccumulator;

    std::vector<PTPScreenSceneAssetCreation> _assetCreationList;
    std::vector<PTPScreenSceneCreationQueue> objectCreationList;

    std::vector<PTPObjectAssetMirror*> _shaderEffectList;

    std::unordered_set<std::string> _unpausedUiList;

    float _lastCharactersDistance;

    friend class PTPScreenUi;
};

#endif // PTPSCREENSCENE_H
