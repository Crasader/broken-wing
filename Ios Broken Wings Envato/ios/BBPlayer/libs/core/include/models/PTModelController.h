#ifndef PTMODELCONTROLLER_H
#define PTMODELCONTROLLER_H

#include "PTModel.h"

#include <cstdint>
#include <unordered_map>
#include <string>
#include <vector>
#include <functional>
#include <limits>
#include <algorithm>

#include <cocos2d.h>

class PTAttribute;
class PTArchiveReader;

//#define _NOT_USING_BBDOCX_ON_EXPORT 1

class PTModelController {
public:
    enum DataType {
        Attributes,
        Connections
    };

    enum LoadFileStatus {
        LoadFileSuccess,
        LoadFileNotFound,
        LoadFileError
    };

    static PTModelController* shared();
    static void resetShared();

    void clean();

    bool isUsingDataEncryption() const { return _usingDataEncryption; }
    void setUsingDataEncryption( bool value ) { _usingDataEncryption = value; }

    /**************************************Factory**************************************/
    template <class Model>
    void addFactory();

    template <class Model>
    void addFactory(const std::string &name);

    /**************************************Models**************************************/
    template <class Model, typename... Args>
    std::shared_ptr<Model> createModel(const Args&... args);

    PTModelPtr createModel(const std::string &className, bool add = true);

    void addModel(const PTModelPtr &model, std::size_t index = std::numeric_limits<std::size_t>::max());
    inline void addModel(PTModel *model, std::size_t index = std::numeric_limits<std::size_t>::max()) { addModel(model->ptr(), index); }

    void removeModel(const PTModelPtr &model);
    inline void removeModel(PTModel *model) { removeModel(model->ptr()); }

    PTModelPtr getModel(std::uint32_t id) const;

    template <class Model>
    std::shared_ptr<Model> getModel(std::uint32_t id) const;

    template <class Model>
    std::shared_ptr<Model> getModelOfType(std::uint32_t id) const;

    template <class Model>
    std::shared_ptr<Model> getFirstModel() const;

    template <class Model>
    std::vector<std::shared_ptr<Model>> getModels() const;

    template <class Model>
    std::vector<std::shared_ptr<Model>> getModelsOfType() const;

    std::vector<PTModelPtr> getModels(std::size_t type) const;

    inline bool hasModel(const PTModelPtr &model) const { return _idModelMap.find(model->id()) != _idModelMap.end(); }
    inline bool hasModel(const PTModel *model) const { return _idModelMap.find(model->id()) != _idModelMap.end(); }

    std::size_t modelIndex(const PTModelPtr &model) const;
    inline std::size_t modelIndex(PTModel *model) const { return modelIndex(model->ptr()); }

    template <typename Model, typename Compare>
    void sortModels(Compare compare);

    template <typename Compare>
    void sortModels(std::size_t type, Compare compare);

    void modelIdChanged(const PTModelPtr &model, std::uint32_t oldId);
    void modelIdChanged(PTModel *model, std::uint32_t oldId) { modelIdChanged(model->ptr(), oldId); }

    void modelChange(const PTModelPtr &model);
    inline void modelChange(PTModel *model) { modelChange(model->ptr()); }

    void attributeChange(PTAttribute *attribute, PTAttribute::ChangeType changeType);

    inline void setChangesState(bool state) { _changesState = state; }
    inline bool hasChanges() const noexcept { return _changesState; }

    inline std::uint32_t lastModelId() const noexcept { return _lastModelId; }
    inline std::uint32_t nextModelId() noexcept { return ++_lastModelId; }
    inline void setLastModelId(std::uint32_t id) noexcept { _lastModelId = id; }

    /**************************************Loading**************************************/
    void loadDataForSplashScreen(const std::string &dataPath, const char *archivePassword);
    void loadDataX(const char *data, std::size_t dataSize, DataType type);

    bool loadDirectory(const std::string &path);
    bool loadDirectoryX(const std::string &path);
    LoadFileStatus loadFile(const std::string &path, DataType type);
    LoadFileStatus loadFileX(const std::string &path, DataType type);

    void openDataArchive(const std::string &path);
    void closeDataArchive();
    bool loadDataArchive(const std::string &password = std::string());
    bool loadDataArchiveX(const std::string &password = std::string());
    LoadFileStatus loadArchiveFile(const std::string &name, DataType type, const std::string &processor = std::string());
    LoadFileStatus loadArchiveFileX(const std::string &name, DataType type, const std::string &processor = std::string());

    char *readFile(const std::string &path, uint64_t &outSize);
    char *readFileArchive(const std::string &path, uint64_t &outSize, const std::string &password);

    std::size_t loadProgress() const;

    inline const std::string& loadError() const { return _loadError; }
    inline void clearLoadError() { _loadError.clear(); }

    /**************************************Misc**************************************/
    static void ibVersion(int &big, int &major, int &minor);
    static int bigIbVersion();
    static int majorIbVersion();
    static int minorIbVersion();

protected:
    PTModelController();
    virtual ~PTModelController();

    /**************************************Factory**************************************/
    void initFactory();

    /**************************************Loading**************************************/
    void loadDictionary(cocos2d::CCDictionary *dict, DataType type);

protected:
    /**************************************Models**************************************/
    virtual void modelAdded(const PTModelPtr &model);
    virtual void modelRemoved(const PTModelPtr &model);
    virtual void modelChanged(const PTModelPtr &model);

    virtual void attributeChanged(PTAttribute *attribute, PTAttribute::ChangeType changeType);

protected:
    static PTModelController *_instance;

    std::unordered_map<std::string, std::function<PTModelPtr()>> _factoryMap;

    std::map<std::size_t, std::vector<PTModelPtr>> _typeModelMap;
    std::unordered_map<std::uint32_t, PTModelPtr> _idModelMap;

    std::string _loadError;

    bool _changesState;

private:
    class LoadProgress;

    std::uint32_t _lastModelId;

    bool _usingDataEncryption;
    PTArchiveReader *_dataArchive;
    LoadProgress *_loadProgress;

    std::string updateProcessor(std::string proc);
};

#include "PTModelController.ipp"

#endif // PTMODELCONTROLLER_H
