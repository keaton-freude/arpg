#pragma once
#include <OgreColourValue.h>


// Directional lights have no location and infinite distance.
// They have no attenuation, and cast light equally anywhere in the scene.
// A directional light typically applies light similar to the Sun.
struct DirectionalLightSettings
{
public:
    DirectionalLightSettings() = default;

    DirectionalLightSettings(Ogre::ColourValue diffuse, Ogre::Vector3 direction)
    {
        diffuseColor = diffuse;
        direction = direction;
    }

    // The color of the light
    Ogre::ColourValue diffuseColor;

    // The direction of the light
    Ogre::Vector3 direction;
};
