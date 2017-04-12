#pragma once
#include <OgreColourValue.h>
#include <OgreVector3.h>

struct AmbientLight
{
    Ogre::ColourValue _upperHemisphere;
    Ogre::ColourValue _lowerHemisphere;
    Ogre::Vector3 _hemisphereDirection;
    Ogre::Real _envmapScale;
    AmbientLight()
        : _upperHemisphere(),
          _lowerHemisphere(),
          _hemisphereDirection(Ogre::Vector3::ZERO),
          _envmapScale(1.0f)
    {

    }

    AmbientLight(Ogre::ColourValue upperHemisphere, Ogre::ColourValue lowerHemisphere,
                 Ogre::Vector3 hemisphereDirection, Ogre::Real envmapScale)
        : _upperHemisphere(upperHemisphere),
          _lowerHemisphere(lowerHemisphere),
          _hemisphereDirection(hemisphereDirection),
          _envmapScale(envmapScale)
    {

    }
};
