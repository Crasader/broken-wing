#ifndef PTMODELGENERALSETTINGS_H
#define PTMODELGENERALSETTINGS_H

#ifndef __QT__
#include "PTBaseModelGeneralSettings.h"
typedef PTBaseModelGeneralSettings BaseGeneralSettings;
#else
#include "qt/QModelGeneralSettings.h"
typedef QModelGeneralSettings BaseGeneralSettings;
#endif

PT_MODEL_DECL(PTModelGeneralSettings);

class PTModelGeneralSettings : public BaseGeneralSettings {
    PT_MODEL_BASIC(PTModelGeneralSettings, BaseGeneralSettings)

public:
    static PTModelGeneralSettingsPtr shared();
    static void resetShared();

    template <typename... Args>
    static PTModelGeneralSettingsPtr create(const Args&... /*args*/) { return shared(); }

protected:
    PTModelGeneralSettings();
    PTModelGeneralSettings(const PTModelGeneralSettings &other) = delete; //No copy constructor for general settings
    PTModelGeneralSettings(const PTModelGeneralSettingsPtr &other) = delete; //No copy constructor for general settings

    PTModelPtr duplicate(bool /*duplicateChildren*/ = true, bool /*checkManagement*/ = true) const { return shared(); } //PTModelGeneralSettings cannot be duplicated

private:
    static PTModelGeneralSettingsPtr _instance;
};

#endif // PTMODELGENERALSETTINGS_H
