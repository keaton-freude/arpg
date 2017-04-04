#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(Ogre::SceneManager *sceneManager, DirectionalLightSettings *settings)
{
    _node = sceneManager->createSceneNode();
    _light = sceneManager->createLight();
    _node->attachObject(_light);

    if (settings != nullptr)
    {
        // Apply settings
        _light->setDirection(settings->direction);
        _light->setDiffuseColour(settings->diffuseColor);
    }
}

void DirectionalLight::Enable()
{
    _light->setVisible(true);
}

void DirectionalLight::Disable()
{
    _light->setVisible(false);
}

void DirectionalLight::SetDirection(Ogre::Vector3 direction)
{
    _light->setDirection(direction);
}

void DirectionalLight::SetDiffuseColor(Ogre::ColourValue color)
{
    _light->setDiffuseColour(color);
}
