#pragma once

#include <QObject>
#include <string>
#include <OgreLog.h>

using std::string;

class OgreLogWatcher: public QObject, public Ogre::LogListener
{
    Q_OBJECT

public:
    OgreLogWatcher();
    void messageLogged(const Ogre::String &message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String &logName, bool &skipThisMessage) override;

signals:
    void logAdded(string content);
};
