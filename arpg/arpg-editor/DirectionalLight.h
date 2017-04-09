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
    void SetDirectionX(float new_x);
    void SetDirectionY(float new_y);
    void SetDirectionZ(float new_z);
    void SetDiffuseColor(Ogre::ColourValue color);
    void SetDiffuseColorR(Ogre::Real new_r);
    void SetDiffuseColorG(Ogre::Real new_g);
    void SetDiffuseColorB(Ogre::Real new_b);
    Ogre::Vector3 GetDirection();
private:
    Ogre::SceneNode* _node;
    Ogre::Light* _light;
};
