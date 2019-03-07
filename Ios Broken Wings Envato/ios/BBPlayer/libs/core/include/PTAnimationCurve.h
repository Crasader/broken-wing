#ifndef PTANIMATIONCURVE_H
#define PTANIMATIONCURVE_H

#include "cocos2d.h"

#include "PTMessagePackInterface.h"

USING_NS_CC;

typedef struct
{
    float	hdl1;
    CCPoint	knot;
    float	hdl2;

} bez3;

typedef enum
{
    PTAnimationCurveInterpolationConstant = 0,
    PTAnimationCurveInterpolationLinear,
    PTAnimationCurveInterpolationBizer

} PTAnimationCurveInterpolationType;

typedef enum
{
    PTAnimationCurveExtrapolationConstant = 0,
    PTAnimationCurveExtrapolationExtrapolation,
    PTAnimationCurveExtrapolationCyclic,
    PTAnimationCurveExtrapolationCyclicExtrapolation

} PTAnimationCurveExtrapolationType;


class PTAnimationCurve : public PTMessagePackInterface {
public:
    static PTAnimationCurve *nullCurve();
    static PTAnimationCurve *linearCurve();
    static PTAnimationCurve *easeInCurve();
    static PTAnimationCurve *easeOutCurve();
    static PTAnimationCurve *elasticInCurve();

    enum AnimationState {
        OpenState = 0,
        IdleState = 1,
        CloseState = 2
    };

    enum AnimationCurveType {
        Linear = 0,
        EasyIn,
        EasyOut,
        ElasticIn,
        Custom,
        Null
    };

    PTAnimationCurve();

    PTAnimationCurve *duplicated();

    virtual void initWithDictionary(CCDictionary *container);
    virtual CCDictionary *getDictionary();
    void pack(PTMessagePack &out) const override;
    void unpack(const PTMessagePack &objects) override;

    PTAnimationCurveExtrapolationType extrapolation;
    PTAnimationCurveInterpolationType interpolation;

    float time;
    AnimationCurveType preset();

    int pointIndex( float time ) const;
    bez3 point( int index );
    void setPoint(int, bez3);

    void setPointValue( int, CCPoint );
    CCPoint pointValue( int );

    void setHandler1Value( int, float );
    float handler1Value(int);

    void setHandler2Value( int, float );
    float handler2Value(int);

    int pointsNumber();

    void updated(float dt);
    float cubicBezier(float t, float a, float b, float c, float d) const;
    void calculateRatio();
    void reset();
    void setPoints( bez3 *points );
    void setValue(float _value, float _time);
    float value(float _time) const;
    float value() const;
    void removePoint(float _time);
    void removePointAtIndex( int );

    std::vector<int> keyFrameMarkers() const;

    // return true if A has time bigger than B
    bool needSwap( int a, int b );
    void swapPoints( int, int );

private:
    float tRatio;
    float vRatio;
    float tvRatio;
    int currentPoint;
    float deltaTime;
    bez3 *pPoints;
    unsigned int pNumberOfPoints;
    float pValue;

    AnimationCurveType _preset;
};

typedef std::map<PTAnimationCurve::AnimationState, PTAnimationCurve*> PTAnimationCurveMap;

std::vector<msgpack::object> packAnimationCurveMap(PTAnimationCurveMap curvesMap, msgpack::zone &zone);
PTAnimationCurveMap unpackAnimationCurveMap(std::vector<msgpack::v2::object> &animations);

cocos2d::CCObject* animationCurveMapToContainer(PTAnimationCurveMap map);
PTAnimationCurveMap animationCurveContainerToMap(cocos2d::CCObject *container);
PTAnimationCurveMap animationCurveMapCopy(PTAnimationCurveMap map);

#endif // PTANIMATIONCURVE_H
