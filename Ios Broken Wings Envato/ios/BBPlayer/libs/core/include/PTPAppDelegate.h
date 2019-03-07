#ifndef  PTPLAYERAPPDELEGATE_H
#define  PTPLAYERAPPDELEGATE_H

#include "cocos2d.h"

#include <string>

class  PTPAppDelegate : private cocos2d::CCApplication
{
public:

    PTPAppDelegate();

    virtual ~PTPAppDelegate();

    virtual bool initInstance();
    virtual bool applicationDidFinishLaunching();
    virtual void applicationDidEnterBackground();
    virtual void applicationWillEnterForeground();

    void scheduledLoading();

    bool isTrial();
    void dumpSignatures();

    void setDataArchiveProcessor(const std::string &processor) { _dataArchiveProcessor = processor; }

private:
    std::string _dataArchiveProcessor;
};

#endif // PTPLAYERAPPDELEGATE_H

