#pragma once

#include <Ogre.h>
#include <OgreLog.h>
#include <Compositor/OgreCompositorManager2.h>

#include <OgreMesh.h>
#include <OgreMeshManager.h>
#include <OgreMesh2.h>
#include <OgreMeshManager2.h>
#include <Hlms/Pbs/OgreHlmsPbs.h>
#include <OgreItem.h>
#include <Hlms/Pbs/OgreHlmsPbsDatablock.h>
#include "DirectionalLightSettings.h"
#include "DirectionalLight.h"
#include <memory>
using std::unique_ptr;
#include "AmbientLight.h"


class RenderSystem : public Ogre::FrameListener
{
public:
    RenderSystem();
    RenderSystem(Ogre::LogListener* logListener);
    ~RenderSystem();

    DirectionalLight* GetDirectionalLight();
    void Render();
    void Resize(unsigned int width, unsigned int height);
    void Initialize(unsigned int width, unsigned int height, unsigned long externalWindowHandle, unsigned long parentWindowHandle);

    bool IsDirectionalLightEnabled() const;
    bool IsInitialized() const;

    void SetAmbientSettings(AmbientLight settings);
    AmbientLight GetAmbientSettings();
    void DisableAmbientLight();
private:
    void RegisterResources();
    void LoadRenderSystem(unsigned int width, unsigned int height);
    void CreateRenderWindow(unsigned int width, unsigned int height, unsigned long externalWindowHandle, unsigned long parentWindowHandle);
    void CreateSceneManager();
    void CreateCamera();
    void CreateCompositor();
    void LoadHlms();

    unique_ptr<Ogre::Root> _root;

    // These ogre pointers are owned by Ogre, when root is freed
    // these pointers become invalid.
    Ogre::RenderWindow* _ogreWindow;
    Ogre::SceneManager* _ogreSceneManager;
    Ogre::Camera* _ogreCamera;
    Ogre::ColourValue _clearColor;
    Ogre::LogListener* _logListener;

    unique_ptr<DirectionalLight> _directionalLight;
    AmbientLight ambientSettings;

    bool _initialized;
};
