#ifndef PTMODEL_IPP
#define PTMODEL_IPP

#include "PTModel.h"

#include <algorithm>

template <class Model>
std::shared_ptr<Model> PTModel::ptr() const noexcept {
    return PTModel::cast<Model>(_thisPtr.lock());
}

template <class Attribute>
Attribute* PTModel::attribute(const std::string &name) const {
    return PTAttribute::cast<Attribute>(attribute(name));
}

template <class Attribute>
Attribute* PTModel::attributeAt(std::size_t index) const {
    return PTAttribute::cast<Attribute>(attributeAt(index));
}

template <class Model>
std::shared_ptr<Model> PTModel::firstParent() const noexcept {
    return PTModel::cast<Model>(firstParent());
}

template <class Model>
std::shared_ptr<Model> PTModel::firstParentOfType() const noexcept {
    return PTModel::dynamicCast<Model>(firstParent());
}

template <class Model>
std::shared_ptr<Model> PTModel::findFirstParent() const noexcept {
    for (const std::weak_ptr<PTModel> &weakParent: parents()) {
        if (std::shared_ptr<Model> parent = PTModel::cast<Model>(weakParent.lock())) {
            return parent;
        }
    }

    return std::shared_ptr<Model>();
}

template <class Model>
std::shared_ptr<Model> PTModel::findFirstParentOfType() const noexcept {
    for (const std::weak_ptr<PTModel> &weakParent: parents()) {
        if (std::shared_ptr<Model> parent = PTModel::dynamicCast<Model>(weakParent.lock())) {
            return parent;
        }
    }

    return std::shared_ptr<Model>();
}

template <class Model>
std::vector<std::shared_ptr<Model>> PTModel::parents() const {
    std::vector<std::shared_ptr<Model>> parentsList;

    for (const PTModelPtr &parent: parents()) {
        if (std::shared_ptr<Model> castParent = PTModel::cast<Model>(parent)) {
            parentsList.emplace_back(castParent);
        }
    }

    return parentsList;
}

template <class Model>
std::vector<std::shared_ptr<Model>> PTModel::parentsOfType() const {
    std::vector<std::shared_ptr<Model>> parentsList;

    for (const PTModelPtr &parent: parents()) {
        if (std::shared_ptr<Model> castParent = PTModel::dynamicCast<Model>(parent)) {
            parentsList.emplace_back(castParent);
        }
    }

    return parentsList;
}

template <class Attribute>
std::vector<Attribute*> PTModel::parentAttributes() const {
    std::vector<Attribute*> attributesList;

    for (PTAttribute *attribute: attributes()) {
        if (Attribute *castAttribute = PTAttribute::cast<Attribute>(attribute)) {
            attributesList.emplace_back(castAttribute);
        }
    }

    return attributesList;
}

template <class Model>
std::vector<std::shared_ptr<Model>> PTModel::children() const {
    std::vector<std::shared_ptr<Model>> models;

    for (const PTModelPtr &model: _children) {
        std::shared_ptr<Model> typeModel = PTModel::cast<Model>(model);

        if (typeModel) {
            models.emplace_back(typeModel);
        }
    }

    return models;
}

template <class Model>
std::vector<std::shared_ptr<Model>> PTModel::childrenOfType() const {
    std::vector<std::shared_ptr<Model>> models;

    for (const PTModelPtr &model: _children) {
        std::shared_ptr<Model> typeModel = PTModel::dynamicCast<Model>(model);

        if (typeModel) {
            models.emplace_back(typeModel);
        }
    }

    return models;
}

template <class Model>
std::shared_ptr<Model> PTModel::firstChild() const noexcept {
    for (const PTModelPtr &model: _children) {
        std::shared_ptr<Model> typeModel = PTModel::cast<Model>(model);

        if (typeModel) {
            return typeModel;
        }
    }

    return std::shared_ptr<Model>();
}

template <class Model>
std::shared_ptr<Model> PTModel::firstChildOfType() const noexcept {
    for (const PTModelPtr &model: _children) {
        std::shared_ptr<Model> typeModel = PTModel::dynamicCast<Model>(model);

        if (typeModel) {
            return typeModel;
        }
    }

    return std::shared_ptr<Model>();
}

template <class Model>
std::shared_ptr<Model> PTModel::childAt(std::size_t index) const {
    return PTModel::cast<Model>(childAt(index));
}

template <class Model>
std::shared_ptr<Model> PTModel::childOfTypeAt(std::size_t index) const {
    return PTModel::dynamicCast<Model>(childAt(index));
}

template <typename Model>
bool PTModel::hasChild() const noexcept {
    for (const PTModelPtr &model: _children) {
        if (model->type() == Model::staticType()) {
            return true;
        }
    }

    return false;
}

template <typename Compare>
void PTModel::sortChildren(Compare compare) {
    std::sort(_children.begin(), _children.end(), compare);
}

template <class Model>
Model* PTModel::cast(PTModel *model) noexcept {
    return model && model->type() == Model::staticType() ? static_cast<Model*>(model) : nullptr;
}

template <class Model>
const Model* PTModel::cast(const PTModel *model) noexcept {
    return model && model->type() == Model::staticType() ? static_cast<const Model*>(model) : nullptr;
}

template <class Model>
std::shared_ptr<Model> PTModel::cast(const PTModelPtr &model) noexcept {
    return model && model->type() == Model::staticType() ? std::static_pointer_cast<Model>(model) : std::shared_ptr<Model>();
}

template <class Model>
Model* PTModel::dynamicCast(PTModel *model) noexcept {
    return model && model->isDerived(Model::staticType()) ? static_cast<Model*>(model) : nullptr;
}

template <class Model>
const Model* PTModel::dynamicCast(const PTModel *model) noexcept {
    return model && model->isDerived(Model::staticType()) ? static_cast<const Model*>(model) : nullptr;
}

template <class Model>
std::shared_ptr<Model> PTModel::dynamicCast(const PTModelPtr &model) noexcept {
    return model && model->isDerived(Model::staticType()) ? std::static_pointer_cast<Model>(model) : std::shared_ptr<Model>();
}

#endif // PTMODEL_IPP
