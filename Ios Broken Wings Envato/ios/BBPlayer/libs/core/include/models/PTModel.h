#ifndef PTMODEL_H
#define PTMODEL_H

#include "PTModelDeclaration.h"

#include "attributes/PTAttribute.h"

#include <cstdint>
#include <vector>
#include <memory>
#include <typeinfo>
#include <typeindex>
#include <limits>

#include "PTMessagePackInterface.h"

class PTAttributeString;
class PTModelConsumer;

namespace cocos2d { class CCDictionary; }

PT_MODEL_DECL(PTModel);

#define PT_MODEL_BASE(CLASS_NAME) \
    public: \
        CLASS_NAME& operator =(const CLASS_NAME &other) = delete; \
        \
        virtual const char* className() const noexcept { return staticClassName(); } \
        static constexpr const char* staticClassName() noexcept { return #CLASS_NAME; } \
        \
        virtual std::size_t type() const noexcept { return staticType(); } \
        static std::size_t staticType() noexcept { static const std::size_t type = typeid(CLASS_NAME).hash_code(); return type; } \
        \
        std::shared_ptr<CLASS_NAME> ptr() const noexcept { return std::static_pointer_cast<CLASS_NAME>(_thisPtr.lock()); } \
        \
        template <typename... Args> \
        std::shared_ptr<CLASS_NAME> duplicated(const Args&... args) const \
        { return std::static_pointer_cast<CLASS_NAME>(duplicate(args...)); }

#define PT_MODEL_BASIC(CLASS_NAME, BASE_CLASS) \
    PT_MODEL_BASE(CLASS_NAME) \
    protected: \
        virtual bool isDerived(std::size_t type) const noexcept { return staticType() == type || BASE_CLASS::isDerived(type); }

#define PT_MODEL(CLASS_NAME, BASE_CLASS) \
    PT_MODEL_BASIC(CLASS_NAME, BASE_CLASS) \
    public: \
        template <typename... Args> \
        static std::shared_ptr<CLASS_NAME> create(const Args&... args) { \
            std::shared_ptr<CLASS_NAME> ptr(new CLASS_NAME(args...)); \
            ptr->setThisPtr(ptr); \
            return ptr; \
        } \
        \
    protected: \
        CLASS_NAME(const std::shared_ptr<CLASS_NAME> &other): CLASS_NAME(*other) {} \
        \
        virtual PTModelPtr duplicate(bool duplicateChildren = true, bool checkManagement = true) const { \
            std::shared_ptr<CLASS_NAME> model = CLASS_NAME::create(*this); \
            if (checkManagement) model->checkManagement(this); \
            model->copyChildren(ptr(), duplicateChildren, true); \
            return model; \
        }

#define PT_MODEL_CLASS(CLASS_NAME, BASE_CLASS, ...) \
    PT_MODEL_DECL(CLASS_NAME); \
    class CLASS_NAME : public BASE_CLASS { \
        PT_MODEL(CLASS_NAME, BASE_CLASS) \
        protected: \
            CLASS_NAME(const std::string &name = std::string(__VA_ARGS__)): BASE_CLASS(name) {} \
            CLASS_NAME(const CLASS_NAME &other): BASE_CLASS(other) {} \
    }

class PTModel : public PTMessagePackInterface {
    PT_MODEL_BASE(PTModel)

public:
    virtual ~PTModel();

    inline std::uint32_t id() const noexcept { return _id; }
    void setId(std::uint32_t id);

    template <class Model>
    std::shared_ptr<Model> ptr() const noexcept;

    void pack(PTMessagePack &out) const override;
    void unpack(const PTMessagePack &objects) override;
    void packConnections(PTMessagePack &out) const override;
    void unpackConnections(const PTMessagePack &objects) override;

    //Init
    virtual void initWithDictionary(cocos2d::CCDictionary *dictionary);
    virtual void initConnectionsWithDictionary(cocos2d::CCDictionary *dictionary);
    virtual cocos2d::CCDictionary* getDictionary() const;
    virtual cocos2d::CCDictionary* getConnectionDictionary() const;

    //Attributes
    void addAttribute(PTAttribute *attribute, std::size_t index = std::numeric_limits<std::size_t>::max());
    void removeAttribute(PTAttribute *attribute);
    inline void removeAttribute(const std::string &name) { removeAttribute(attribute(name)); }

    void replaceAttribute(const PTAttribute *attribute, std::size_t toIndex);

    bool hasAttribute(const PTAttribute *attribute) const;
    bool hasAttribute(const std::string &name) const;

    std::size_t attributeIndex(const PTAttribute *attribute) const noexcept;

    PTAttribute* attribute(const std::string &name) const;
    PTAttribute* attributeAt(std::size_t index) const;

    template <class Attribute>
    Attribute* attribute(const std::string &name) const;

    template <class Attribute>
    Attribute* attributeAt(std::size_t index) const;

    inline const std::vector<PTAttribute*>& attributes() const noexcept { return _attributes; }
    inline std::size_t attributesCount() const noexcept { return _attributes.size(); }

    virtual void attributeChanged(PTAttribute *attribute, PTAttribute::ChangeType changeType);

    const std::string& name() const noexcept;
    void setName(const std::string &name);

    //Parents
    void addParent(PTModelPtr model, bool silent = false);
    void removeParent(PTModelPtr model, bool silent = false);
    void removeParents(bool silent = false);

    PTModelPtr firstParent() const noexcept;

    template <class Model>
    std::shared_ptr<Model> firstParent() const noexcept;

    template <class Model>
    std::shared_ptr<Model> firstParentOfType() const noexcept;

    template <class Model>
    std::shared_ptr<Model> findFirstParent() const noexcept;

    template <class Model>
    std::shared_ptr<Model> findFirstParentOfType() const noexcept;

    std::vector<PTModelPtr> parents() const;

    template <class Model>
    std::vector<std::shared_ptr<Model>> parents() const;

    template <class Model>
    std::vector<std::shared_ptr<Model>> parentsOfType() const;

    bool hasParent(const PTModelPtr &model) const;

    //Parent attributes. Mainly used in PTAttributeModel. It is not recommended to touch these functions
    void addParentAttribute(PTAttribute *attribute);
    void removeParentAttribute(PTAttribute *attribute);

    inline const std::vector<PTAttribute*>& parentAttributes() const noexcept { return _parentAttributes; }

    template <class Attribute>
    std::vector<Attribute*> parentAttributes() const;

    //Children
    void addChild(PTModelPtr model, bool silent = false);
    void addChild(PTModelPtr model, std::size_t index, bool silent = false);
    virtual void copyChildren(const PTModelPtr &from, bool duplicate = true, bool checkManagement = true, bool silent = false);
    void removeChild(PTModelPtr model, bool silent = false);
    void removeChildren(bool silent = false);

    void replaceChild(const PTModelPtr &model, std::size_t toIndex, bool silent = false);
    void replaceChild(const PTModelPtr &model, const PTModelPtr &toModel, bool below = false, bool silent = false);

    inline std::size_t childrenCount() const noexcept { return _children.size(); }

    inline const std::vector<PTModelPtr>& children() const noexcept { return _children; }

    template <class Model>
    std::vector<std::shared_ptr<Model>> children() const;

    template <class Model>
    std::vector<std::shared_ptr<Model>> childrenOfType() const;

    template <class Model>
    std::shared_ptr<Model> firstChild() const noexcept;

    template <class Model>
    std::shared_ptr<Model> firstChildOfType() const noexcept;

    PTModelPtr childAt(std::size_t index) const;

    template <class Model>
    std::shared_ptr<Model> childAt(std::size_t index) const;

    template <class Model>
    std::shared_ptr<Model> childOfTypeAt(std::size_t index) const;

    bool hasChild(const PTModelPtr &model) const;
    inline bool hasChild(const PTModel *model) const { return hasChild(model->ptr()); }

    template <class Model>
    bool hasChild() const noexcept;

    std::size_t childIndex(const PTModelPtr &model) const;

    template <typename Compare>
    void sortChildren(Compare compare);

    //Consumers
    void addConsumer(PTModelConsumer *consumer);
    void removeConsumer(PTModelConsumer *consumer);

    inline const std::vector<PTModelConsumer*>& consumers() const noexcept { return _consumers; }

    //Type casting
    template <class Model>
    static Model* cast(PTModel *model) noexcept;

    template <class Model>
    static const Model* cast(const PTModel *model) noexcept;

    template <class Model>
    static std::shared_ptr<Model> cast(const PTModelPtr &model) noexcept;

    template <class Model>
    static Model* dynamicCast(PTModel *model) noexcept;

    template <class Model>
    static const Model* dynamicCast(const PTModel *model) noexcept;

    template <class Model>
    static std::shared_ptr<Model> dynamicCast(const PTModelPtr &model) noexcept;

protected:
    PTModel(const std::string &name = std::string());
    PTModel(const PTModel &other);

    virtual PTModelPtr duplicate(bool duplicateChildren = true, bool checkManagement = true) const = 0;

    virtual bool isDerived(std::size_t type) const noexcept { return staticType() == type; }

    virtual void childAdded(const PTModelPtr &model);
    virtual void childRemoved(const PTModelPtr &model);
    virtual void childrenRemoved();

    void changed();

    //Do not touch these functions unless you are absolutely sure what you are doing
    void setThisPtr(const std::weak_ptr<PTModel> &self);
    void checkManagement(const PTModel *other);

private:
    void removeNullParents();

protected:
    std::weak_ptr<PTModel> _thisPtr;

    PTAttributeString *_nameAttribute;

private:
    std::uint32_t _id;

    std::vector<PTAttribute*> _attributes;

    std::vector<std::weak_ptr<PTModel>> _parents;
    std::vector<PTAttribute*> _parentAttributes;
    std::vector<PTModelPtr> _children;

    std::vector<PTModelConsumer*> _consumers;
};

static unsigned int qHash(const PTModelPtr &item, unsigned int seed = 0) {
    return item ? item->id() : 0;
}

#include "PTModel.ipp"

#endif // PTMODEL_H
