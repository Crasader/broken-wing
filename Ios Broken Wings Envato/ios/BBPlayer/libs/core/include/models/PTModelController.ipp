#ifndef PTMODELCONTROLLER_IPP
#define PTMODELCONTROLLER_IPP

#include "PTModelController.h"

/**************************************Factory**************************************/
template <class Model>
void PTModelController::addFactory() {
    _factoryMap.emplace(Model::staticClassName(), &Model::template create<>);
}

template <class Model>
void PTModelController::addFactory(const std::string &name) {
    _factoryMap.emplace(name, &Model::template create<>);
}

/**************************************Models**************************************/
template <class Model, typename... Args>
std::shared_ptr<Model> PTModelController::createModel(const Args&... args) {
    std::shared_ptr<Model> model = Model::create(args...);

    addModel(model);

    return model;
}

template <class Model>
std::shared_ptr<Model> PTModelController::getModel(std::uint32_t id) const {
    return PTModel::cast<Model>(getModel(id));
}

template <class Model>
std::shared_ptr<Model> PTModelController::getModelOfType(std::uint32_t id) const {
    return PTModel::dynamicCast<Model>(getModel(id));
}

template <class Model>
std::shared_ptr<Model> PTModelController::getFirstModel() const {
    auto it = _typeModelMap.find(Model::staticType());

    return it != _typeModelMap.end() && !it->second.empty() ? PTModel::cast<Model>(it->second.front()) : std::shared_ptr<Model>();
}

template <class Model>
std::vector<std::shared_ptr<Model>> PTModelController::getModels() const {
    std::vector<std::shared_ptr<Model>> models;
    auto it = _typeModelMap.find(Model::staticType());

    if (it != _typeModelMap.end()) {
        for (const PTModelPtr &model: it->second) {
            models.emplace_back(std::static_pointer_cast<Model>(model));
        }
    }

    return models;
}

template <class Model>
std::vector<std::shared_ptr<Model>> PTModelController::getModelsOfType() const {
    std::vector<std::shared_ptr<Model>> models;

    for (const std::pair<const std::size_t, std::vector<PTModelPtr>> &items: _typeModelMap) {
        if (!items.second.empty() && PTModel::dynamicCast<Model>(items.second.front())) {
            for (const PTModelPtr &model: items.second) {
                models.emplace_back(std::static_pointer_cast<Model>(model));
            }
        }
    }

    return models;
}

template <typename Model, typename Compare>
void PTModelController::sortModels(Compare compare) {
    sortModels(Model::staticType(), compare);
}

template <typename Compare>
void PTModelController::sortModels(std::size_t type, Compare compare) {
    auto it = _typeModelMap.find(type);

    if (it != _typeModelMap.end()) {
        std::sort(it->second.begin(), it->second.end(), compare);
    }
}

#endif // PTMODELCONTROLLER_IPP
