#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(Ogre::SceneManager *sceneManager, DirectionalLightSettings *settings)
{
    _node = sceneManager->createSceneNode();
    _light = sceneManager->createLight();
    _node->attachObject(_light);
    _light->setPowerScale(1.0f);
    _light->setType(Ogre::Light::LT_DIRECTIONAL);
    _light->setDirection(Ogre::Vector3(1, -1, -1));


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

void DirectionalLight::SetDirectionX(float new_x)
{
    // Get current direction, modify then reset
    auto direction = _light->getDirection();
    direction.x = new_x;
    _light->setDirection(direction);
}

void DirectionalLight::SetDirectionY(float new_y)
{
    auto direction = _light->getDirection();
    direction.y = new_y;
    _light->setDirection(direction);
}

void DirectionalLight::SetDirectionZ(float new_z)
{
    auto direction = _light->getDirection();
    direction.z = new_z;
    _light->setDirection(direction);
}

void DirectionalLight::SetDiffuseColor(Ogre::ColourValue color)
{
    _light->setDiffuseColour(color);
}

Ogre::Vector3 DirectionalLight::GetDirection()
{
    return _light->getDirection();
}

void DirectionalLight::SetDiffuseColorR(Ogre::Real new_r)
{
    auto color = _light->getDiffuseColour();
    color.r = new_r;
    _light->setDiffuseColour(color);
}

void DirectionalLight::SetDiffuseColorG(Ogre::Real new_g)
{
    auto color = _light->getDiffuseColour();
    color.g = new_g;
    _light->setDiffuseColour(color);
}

void DirectionalLight::SetDiffuseColorB(Ogre::Real new_b)
{
    auto color = _light->getDiffuseColour();
    color.b = new_b;
    _light->setDiffuseColour(color);
}
