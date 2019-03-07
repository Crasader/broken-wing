#ifndef PTBASEATTRIBUTE_H
#define PTBASEATTRIBUTE_H

#include "PTAnimationCurve.h"
#include "models/PTModelDeclaration.h"

#include <string>
#include <vector>
#include <algorithm>

class PTAttribute;

namespace cocos2d {
class CCDictionary;
}

PT_MODEL_DECL(PTModel);

#define PT_ATTRIBUTE(CLASS_NAME) \
    public: \
        CLASS_NAME(const CLASS_NAME &other) = delete; \
        \
        virtual const char* className() const noexcept { return staticClassName(); } \
        static constexpr const char* staticClassName() noexcept { return #CLASS_NAME; } \
        \
        virtual std::size_t type() const noexcept { return staticType(); } \
        static std::size_t staticType() noexcept { static const std::size_t type = typeid(CLASS_NAME).hash_code(); return type; } \
        \
        virtual CLASS_NAME* duplicated(const std::string &name) const { return new CLASS_NAME(name, *this); } \
        virtual CLASS_NAME* duplicated(PTModel *model) const { return new CLASS_NAME(model, *this); } \
        virtual CLASS_NAME* duplicated(const std::string &name, PTModel *model) const { return new CLASS_NAME(name, model, *this); }

#define PT_ATTRIBUTE_CLASS(CLASS_NAME, BASE_CLASS) \
    class CLASS_NAME : public BASE_CLASS { \
        PT_ATTRIBUTE(CLASS_NAME) \
        public: \
        CLASS_NAME(const std::string &name, PTModel *model = nullptr, ValueType valueType = ExactValue): BASE_CLASS(name, model, valueType) {} \
        CLASS_NAME(const std::string &name, const CLASS_NAME &other): CLASS_NAME(name, other.model(), other) {} \
        CLASS_NAME(PTModel *model, const CLASS_NAME &other): CLASS_NAME(other.name(), model, other) {} \
        CLASS_NAME(const std::string &name, PTModel *model, const CLASS_NAME &other): BASE_CLASS(name, model, other) {} \
        CLASS_NAME& operator =(const CLASS_NAME &other) { BASE_CLASS::operator =(other); return *this; } \
    }

#define PT_ATTRIBUTE_CLASS2(CLASS_NAME, BASE_CLASS) \
    class CLASS_NAME : public BASE_CLASS { \
        PT_ATTRIBUTE(CLASS_NAME) \
        public: \
        CLASS_NAME(const std::string &name, PTModel *model = nullptr): BASE_CLASS(name, model) {} \
        CLASS_NAME(const std::string &name, const CLASS_NAME &other): CLASS_NAME(name, other.model(), other) {} \
        CLASS_NAME(PTModel *model, const CLASS_NAME &other): CLASS_NAME(other.name(), model, other) {} \
        CLASS_NAME(const std::string &name, PTModel *model, const CLASS_NAME &other): BASE_CLASS(name, model, other) {} \
        CLASS_NAME& operator =(const CLASS_NAME &other) { BASE_CLASS::operator =(other); return *this; } \
    }

class PTBaseAttribute : public PTMessagePackInterface {
    PT_ATTRIBUTE(PTBaseAttribute)

public:
    enum ValueType {
        ExactValue,
        VariableValue
    };

    enum ConnectionType {
        NoConnectionType,
        InputConnectionType,
        OutputConnectionType
    };

    enum ChangeType {
        ValueChangeType,
        ConnectionChangeType,
        VisibilityChangeType
    };

    void packConnections(PTMessagePack &out) const override;
    void unpackConnections(const PTMessagePack &objects) override;

    //Init
    virtual void initWithDictionary(cocos2d::CCDictionary */*dictionary*/) {}
    virtual void initConnectionsWithDictionary(cocos2d::CCDictionary *dictionary);
    virtual cocos2d::CCDictionary* getDictionary() const;
    virtual cocos2d::CCDictionary* getConnectionDictionary() const;

    //Info
    inline const std::string& name() const noexcept { return _name; }
    void setName(const std::string &name, bool silent = false);

    inline PTModel* model() const noexcept { return _model; }
    void setModel(PTModel *model);
    void setModel(const PTModelPtr &model);

    inline ValueType valueType() const noexcept { return _valueType; }
    inline void setValueType(ValueType type) { _valueType = type; }

    virtual std::vector<PTAnimationCurve*> animationCurves(PTAnimationCurve::AnimationState state) const;
    virtual void setAnimationCurves(const std::vector<PTAnimationCurve*> &curves, PTAnimationCurve::AnimationState state);

    //Connection
    inline ConnectionType connectionType() const noexcept { return _connectionType; }
    void setConnectionType(ConnectionType type, bool silent = false);

    void connect(PTBaseAttribute *attribute, bool silent = false);
    void disconnect(bool silent = false);
    void disconnect(PTBaseAttribute *attribute, bool silent = false);

    inline bool isConnected() const noexcept { return !_connectedAttributes.empty(); }
    bool isConnected(PTBaseAttribute *attribute) const;

    inline PTAttribute* connectionTarget() const noexcept { return connectionType() == OutputConnectionType && !_connectedAttributes.empty() ? _connectedAttributes.front() : nullptr; }
    inline const std::vector<PTAttribute*> connectedAttributes() const noexcept { return _connectedAttributes; }

    //Type casting
    template <class Attribute>
    static Attribute* cast(PTBaseAttribute *attribute) noexcept
    { return attribute && attribute->type() == Attribute::staticType() ? static_cast<Attribute*>(attribute) : nullptr; }

protected:
    PTBaseAttribute(const std::string &name, PTModel *model = nullptr, ValueType valueType = ExactValue);
    PTBaseAttribute(const std::string &name, const PTBaseAttribute &other);
    PTBaseAttribute(PTModel *model, const PTBaseAttribute &other);
    PTBaseAttribute(const std::string &name, PTModel *model, const PTBaseAttribute &other);
    virtual ~PTBaseAttribute();

    PTBaseAttribute& operator =(const PTBaseAttribute &other); //The current name will be preserved

    virtual void changed(ChangeType type = ValueChangeType);

private:
    std::string _name;
    PTModel *_model;
    ValueType _valueType;

    ConnectionType _connectionType;
    std::vector<PTAttribute*> _connectedAttributes;
};

#endif // PTBASEATTRIBUTE_H
