#ifndef PTBASEMODELGENERALSETTINGS_H
#define PTBASEMODELGENERALSETTINGS_H

#include "PTModel.h"

#include "attributes/PTAttributeEnum.h"

#include <list>
#include <ctime>

#include <cocos2d.h>

class PTAttributeInt;
class PTAttributeUInt;
class PTAttributeFloat;
class PTAttributeBool;
class PTAttributeString;
class PTAttributeRect;
class PTAttributeSprite;
class PTAttributeSize;
class PTAttributeKey;

PT_MODEL_DECL(PTModelSpriteContainer);

enum PTObjectFrameEdgeState {
    PTObjectFrameEdgeStatePass,
    PTObjectFrameEdgeStateBlock,
    PTObjectFrameEdgeStateKill,
    PTObjectFrameEdgeStateCount
};
PT_ATTRIBUTE_ENUM2(FrameEdgeState, PTObjectFrameEdgeState);

//TODO: move these somewhere else
static float kSimulationTimeScaleMultiplier = 50; // +
static float kFrictionMultiplier = 100; // +
static float kBounceForceMutliplier = 2.0; //+
static float kRestitutionMultiplier = 100; //+
static float kVeclocityScaleMultiplier = 100; //+
static float kUpForceMultiplier = 4.0; // +
static float kRotationScaleMultiplier = 100; //+
static float kRotationLeftRightMultiplier = 100; // +
static float kPlatformFrictionMultiplier = 40; //+
static float kCharacterFriction = 100; // +
static float kGameSpeedMultiplier = 25; // +
static float kMoveSpeedMultiplier = 3; // +
static float kExtraDeletionThreshold = 300;

class PTBaseModelGeneralSettings : public PTModel {
public:
    using Base = PTModel;

    enum Orientation {
        PortraitOrientation,
        LandscapeOrientation
    };
    PT_ATTRIBUTE_ENUM(Orientation);

    enum ScreenAdjustment {
        DefaultAdjustment,
        VerticalAdjustment,
        HorisontalAdjustment,
        AutoAdjustment
    };
    PT_ATTRIBUTE_ENUM(ScreenAdjustment);

    void pack(PTMessagePack &out) const override;
    void unpack(const PTMessagePack &objects) override;

    void initWithDictionary(cocos2d::CCDictionary *dictionary);
    cocos2d::CCDictionary* getDictionary() const;

    const std::string& ibVersion() const noexcept;
    void setIbVersion(const std::string &version);

    void ibVersion(int &big, int &major, int &minor) const;
    inline int bigIbVersion() const { int big = 2, major = 0, minor = 0; ibVersion(big, major, minor); return big; }
    inline int majorIbVersion() const { int big = 2, major = 0, minor = 0; ibVersion(big, major, minor); return major; }
    inline int minorIbVersion() const { int big = 2, major = 0, minor = 0; ibVersion(big, major, minor); return minor; }

    uint32_t lastId() const noexcept;
    void setLastId(std::uint32_t id);

    void setUseEncryption(bool value);
    bool useEncryption() const;

    Orientation orientation() const noexcept;
    void setOrientation(Orientation orientation);

    PTObjectFrameEdgeState objectFrameEdgeState(std::size_t i) const noexcept;
    void setObjectFrameEdgeState(std::size_t i, PTObjectFrameEdgeState state);

    ScreenAdjustment screenAdjustment() const noexcept;
    void setScreenAdjustment(ScreenAdjustment adjustment);

    const std::string& scoreType() const noexcept;
    void setScoreType(const std::string &type);

    float gameplayAngleDirection() const noexcept;
    void setGameplayAngleDirection(float direction);

    cocos2d::CCPoint gameplayDirection() const;

    bool isTrial() const noexcept;
    void setTrial(bool state);

    const cocos2d::CCRect& objectFrame() const noexcept;
    void setObjectFrame(const cocos2d::CCRect &rect);

    const cocos2d::CCSize& designResolution() const noexcept;
    void setDesignResolution(const cocos2d::CCSize &size);
    void setDesignResolution(float width, float height);

    const std::string& shareScreen() const noexcept;
    void setShareScreen(const std::string &screen);

    const std::string& versionNumber() const noexcept;
    void setVersionNumber(const std::string &version);

    const std::string& applicationName() const noexcept;
    void setApplicationName(const std::string &name);

    const std::string& comments() const noexcept;
    void setComments(const std::string &comments);

    const std::string& resetText() const noexcept;
    void setResetText(const std::string &text);

    std::time_t buildDate() const;
    void setBuildDate(std::time_t date);

    const PTModelSpriteContainerPtr& icon() const noexcept;
    void setIcon(const PTModelSpriteContainerPtr &icon);

    int moveUpKey() const noexcept;
    void setMoveUpKey(int key);

    int moveDownKey() const noexcept;
    void setMoveDownKey(int key);

    int moveLeftKey() const noexcept;
    void setMoveLeftKey(int key);

    int moveRightKey() const noexcept;
    void setMoveRightKey(int key);

    int motorCWKey() const noexcept;
    void setMotorCWKey(int key);

    int motorCCWKey() const noexcept;
    void setMotorCCWKey(int key);

    int rotateLeftKey() const noexcept;
    void setRotateLeftKey(int key);

    int rotateRightKey() const noexcept;
    void setRotateRightKey(int key);

    int shootKey() const noexcept;
    void setShootKey(int key);

    int jumpKey() const noexcept;
    void setJumpKey(int key);

    bool isKeyMapped(int key) const noexcept;

    const char* platformValue(const std::string &platform, const std::string &key) const;
    void setPlatformValue(const std::string &platform, const std::string &key, const std::string &value);
    void removePlatformValue(const std::string &platform, const std::string &key);

    std::list<std::string> adBannersList(const std::string &platform) const;
    std::list<std::string> adInterstitialsList(const std::string &platform) const;

    std::vector<std::string> allPlatforms() const;

    //------------------Old style values------------------
    inline const cocos2d::CCPoint& gravity() const noexcept { return _gravity; }
    inline const cocos2d::CCPoint& moveSpeed() const noexcept { return _moveSpeed; }
    inline const cocos2d::CCPoint& bounceForce() const noexcept { return _bounceForce; }
    inline const cocos2d::CCPoint& upForce() const noexcept { return _upForce; }
    inline const cocos2d::CCPoint& characterFriction() const noexcept { return _characterFriction; }

    inline float upForceDuration() const noexcept { return _upForceDuration; }
    inline float upForceCounter() const noexcept { return _upForceCounter; }
    inline float jumpGroundThreshold() const noexcept { return _jumpGroundThreshold; }
    inline float leftLeanForce() const noexcept { return _leftLeanForce; }
    inline float rightLeanForce() const noexcept { return _rightLeanForce; }
    inline float simulationTimeScale() const noexcept { return _simulationTimeScale; }
    inline float friction() const noexcept { return _friction; }
    inline float velocityScale() const noexcept { return _velocityScale; }
    inline float rotationScale() const noexcept { return _rotationScale; }
    inline float restitution() const noexcept { return _restitution; }
    inline float platformFriction() const noexcept { return _platformFriction; }
    inline float scoreMultiplier() const noexcept { return _scoreMultiplier; }
    inline float backDeletionThreshold() const noexcept { return _backDeletionThreshold; }
    inline float sidesDeletionThreshold() const noexcept { return _sidesDeletionThreshold; }
    inline float gameSpeedMin() const noexcept { return _gameSpeedMin; }
    inline float gameSpeedMax() const noexcept { return _gameSpeedMax; }
    inline float gameSpeedIncrease() const noexcept { return _gameSpeedIncrease; }
    inline float gameSpeedCurrent() const noexcept { return _gameSpeedCurrent; }

    inline bool isUpForceFromGround() const noexcept { return _upForceFromGround; }
    inline bool isFixedRotation() const noexcept { return _fixedRotation; }
    inline bool isFixedPosition() const noexcept { return _fixedPosition; }
    inline bool isForcedMovement() const noexcept { return _forcedPosition; }
    inline bool autoImageDirection() const noexcept { return _autoImageDirection; }
    //------------------

    static float gameOverDelay();

protected:
    PTBaseModelGeneralSettings();
    PTBaseModelGeneralSettings(const PTBaseModelGeneralSettings &other) = delete; //No copy constructor for general settings
    ~PTBaseModelGeneralSettings();

protected:
    PTAttributeString *_ibVersioAttribute;
    PTAttributeUInt *_lastIdAttribute;

    PTAttributeOrientation *_orientationAttribute;
    PTAttributeFrameEdgeState *_edgeStateAttribute[4];
    PTAttributeScreenAdjustment *_screenAdjustmentAttribute;
    PTAttributeString *_scoreTypeAttribute;
    PTAttributeFloat *_gameplayAngleDirection;

    PTAttributeBool *_trialAttribute;
    PTAttributeBool *_useEncryptionAttribute;

    PTAttributeRect *_objectFrameAttribute;
    PTAttributeSize *_designResolutionAttribute;

    PTAttributeString *_shareScreenAttribute;

    PTAttributeString *_versionNumberAttribute;
    PTAttributeString *_applicationNameAttribute;
    PTAttributeString *_commentsAttribute;
    PTAttributeString *_resetTextAttribute;
    PTAttributeString *_buildDateAttribute;
    PTAttributeSprite *_iconAttribute;

    PTAttributeKey *_moveUpKeyAttribute;
    PTAttributeKey *_moveDownKeyAttribute;
    PTAttributeKey *_moveLeftKeyAttribute;
    PTAttributeKey *_moveRightKeyAttribute;
    PTAttributeKey *_motorCWKeyAttribute;
    PTAttributeKey *_motorCCWKeyAttribute;
    PTAttributeKey *_rotateLeftKeyAttribute;
    PTAttributeKey *_rotateRightKeyAttribute;
    PTAttributeKey *_shootKeyAttribute;
    PTAttributeKey *_jumpKeyAttribute;

    cocos2d::CCDictionary *_platformSpecificValues;

    //Old style values
    cocos2d::CCPoint _gravity;
    cocos2d::CCPoint _moveSpeed;
    cocos2d::CCPoint _bounceForce;
    cocos2d::CCPoint _upForce;
    cocos2d::CCPoint _characterFriction;

    float _upForceDuration;
    float _upForceCounter;
    float _jumpGroundThreshold;
    float _leftLeanForce;
    float _rightLeanForce;
    float _simulationTimeScale;
    float _friction;
    float _velocityScale;
    float _rotationScale;
    float _restitution;
    float _platformFriction;
    float _scoreMultiplier;
    float _backDeletionThreshold;
    float _sidesDeletionThreshold;
    float _gameSpeedMin;
    float _gameSpeedMax;
    float _gameSpeedIncrease;
    float _gameSpeedCurrent;

    bool _upForceFromGround;
    bool _fixedRotation;
    bool _fixedPosition;
    bool _forcedPosition;
    bool _autoImageDirection;
};

#endif // PTBASEMODELGENERALSETTINGS_H
