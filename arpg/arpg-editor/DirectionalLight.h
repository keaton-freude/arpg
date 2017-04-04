#pragma once

#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include "DirectionalLightSettings.h"

// Marries a SceneNode and directional Light together
class DirectionalLight
{
public:
    DirectionalLight(Ogre::SceneManager* sceneManager, DirectionalLightSettings* settings = nullptr);

    void Enable();
    void Disable();
    void SetDirection(Ogre::Vector3 direction);
    void SetDiffuseColor(Ogre::ColourValue color);
private:
    Ogre::SceneNode* _node;
    Ogre::Light* _light;
};
