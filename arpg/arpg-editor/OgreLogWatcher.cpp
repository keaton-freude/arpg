#include "OgreLogWatcher.h"

OgreLogWatcher::OgreLogWatcher()
{
}

void OgreLogWatcher::messageLogged(const Ogre::String &message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String &logName, bool &skipThisMessage)
{
    string prefix = "";

    if (lml == Ogre::LogMessageLevel::LML_TRIVIAL)
        prefix = "[Trivial] - ";
    else if (lml == Ogre::LogMessageLevel::LML_NORMAL)
        prefix = "[Normal] - ";
    else
        prefix = "[Critical] - ";

    emit(logAdded(prefix + message));
}
