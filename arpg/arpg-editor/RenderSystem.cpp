#include "RenderSystem.h"

RenderSystem::RenderSystem()
    : _root(nullptr),
      _ogreWindow(nullptr),
      _ogreSceneManager(nullptr),
      _ogreCamera(nullptr),
      _clearColor(Ogre::ColourValue(100.f / 255.f, 149.f / 255.f, 237.f / 255.f, 1.0f)), // Cornflower Blue
      _logListener(nullptr),
      _directionalLight(nullptr),
      _initialized(false)
{
}

RenderSystem::RenderSystem(Ogre::LogListener *logListener)
    : _root(nullptr),
      _ogreWindow(nullptr),
      _ogreSceneManager(nullptr),
      _ogreCamera(nullptr),
      _clearColor(Ogre::ColourValue(100.f / 225.f, 149.f / 255.f, 237.f / 255.f, 1.0f)),
      _logListener(logListener),
      _directionalLight(nullptr),
      _initialized(false)
{

}

RenderSystem::~RenderSystem()
{
    if (_logListener != nullptr)
    {
        Ogre::LogManager::getSingleton().getDefaultLog()->removeListener(_logListener);
        _logListener = nullptr;
    }

    delete _root;
    _root = nullptr;
}

DirectionalLight *RenderSystem::GetDirectionalLight()
{
    return _directionalLight;
}

void RenderSystem::Render()
{
    _root->renderOneFrame();
}

void RenderSystem::Resize(unsigned int width, unsigned int height)
{
    _ogreWindow->resize(width, height);
}

void RenderSystem::Initialize(unsigned int width, unsigned int height, unsigned long externalWindowHandle, unsigned long parentWindowHandle)
{
    // Initialize Ogre
    _root = new Ogre::Root();

    if (_logListener != nullptr)
        Ogre::LogManager::getSingletonPtr()->getDefaultLog()->addListener(_logListener);

    RegisterResources();
    LoadRenderSystem(width, height);

    _root->initialise(false);

    CreateRenderWindow(width, height, externalWindowHandle, parentWindowHandle);
    CreateSceneManager();
    CreateCamera();
    CreateCompositor();
    LoadHlms();

    Ogre::TextureManager::getSingletonPtr()->setDefaultNumMipmaps(5);
    Ogre::ResourceGroupManager::getSingletonPtr()->initialiseAllResourceGroups();

    _directionalLight = new DirectionalLight(_ogreSceneManager);
    _root->addFrameListener(this);
    _initialized = true;

    // Load a sphere for testing lighting
    Ogre::Item* sphereItem = _ogreSceneManager->createItem("Sphere1000.mesh");
    Ogre::SceneNode* sphereSceneNode = _ogreSceneManager->getRootSceneNode()->createChildSceneNode();
    sphereSceneNode->setPosition(0, 0, 2);
    sphereItem->setDatablock("DebugSphere1000");
    sphereSceneNode->attachObject(sphereItem);
}

bool RenderSystem::IsDirectionalLightEnabled() const
{
    return _directionalLight != nullptr;
}

bool RenderSystem::IsInitialized() const
{
    return _initialized;
}

void RenderSystem::RegisterResources()
{
    // Our resources are located in a config file called resources.cfg
    // We need to parse this file, adding each of the resources to Ogre

    Ogre::ConfigFile ogreConfig;

    ogreConfig.load("resources.cfg"); // For now we assume resources.cfg is next to the built application

    auto sectionIterator = ogreConfig.getSectionIterator();
    Ogre::String sectionName, typeName, archiveName;

    while (sectionIterator.hasMoreElements())
    {
        sectionName = sectionIterator.peekNextKey();
        auto settings = sectionIterator.getNext();

        // Ignore the section called Hlms, it's not a _real_ resource,
        // but rather points to the HLMS implementation folder root
        if (sectionName != "Hlms")
        {
            Ogre::ConfigFile::SettingsMultiMap::iterator i;
            for(i = settings->begin(); i != settings->end(); ++i)
            {
                typeName = i->first;
                archiveName = i->second;
                Ogre::ResourceGroupManager::getSingletonPtr()->addResourceLocation(
                            archiveName, typeName, sectionName);
            }
        }
    }
}

void RenderSystem::LoadRenderSystem(unsigned int width, unsigned int height)
{
    // Only support OpenGL3+, its the most stable Desktop RenderSystem
    // and works on both Linux & Windows.

    // When building Ogre, the available RenderSystems are baked in, I think.
    // So we need to make sure it was built such that OpenGL3+ exists, otherwise we'll throw an exception
    const Ogre::RenderSystemList& rsList = _root->getAvailableRenderers();
    Ogre::RenderSystem* rs = nullptr;

    // If its empty, throw early
    if (rsList.empty())
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE,
                    "No RenderSystems found on Root. Did you forget to build one in?",
                    "RenderSystem::LoadRenderSystem");
    }

    const Ogre::String renderSystemName = "OpenGL 3+";

    // Walk the list of RenderSystems, looking for the selected system.

    for(auto it = rsList.begin(); it != rsList.end(); it++)
    {
        if ((*it)->getName().find(renderSystemName) != Ogre::String::npos)
        {
            rs = *it;
            break;
        }
    }

    // Did we find the RenderSystem? If not, throw an exception here
    if (rs == nullptr)
    {
        OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                    "Could not find requested RenderSystem",
                    "RenderSystem::LoadRenderSystem");
    }

    // Set configuration options for our RenderSystem

    // Ogre expects Dimension of the RenderSystem to look like: 1280 x 720, for example.
    Ogre::String dimensions = std::to_string(width) + " x " + std::to_string(height);
    rs->setConfigOption("Video Mode", dimensions);
    rs->setConfigOption("Full Screen", "No");
    rs->setConfigOption("VSync", "Yes");
    rs->setConfigOption("sRGB Gamma Conversion", "Yes");
    _root->setRenderSystem(rs);
}

void RenderSystem::CreateRenderWindow(unsigned int width, unsigned int height, unsigned long externalWindowHandle, unsigned long parentWindowHandle)
{
    Ogre::NameValuePairList parameters;

    // if the RenderSystem is based on OpenGL, turn off currentGLContext
    if (_root->getRenderSystem()->getName().find("GL") <= _root->getRenderSystem()->getName().size())
        parameters["currentGLContext"] = Ogre::String("false");

    // Set the Window Handle
    parameters["externalWindowHandle"] = Ogre::StringConverter::toString(externalWindowHandle);
    parameters["parentWindowHandle"] = Ogre::StringConverter::toString(parentWindowHandle);

    _ogreWindow = _root->createRenderWindow("Some Name", width, height, false, &parameters);
    _ogreWindow->setVisible(true);
}

void RenderSystem::CreateSceneManager()
{
    // Figure out how many threads we can run
    const size_t numThreads = std::max<int>(1, Ogre::PlatformInformation::getNumLogicalCores());
    Ogre::InstancingThreadedCullingMethod threadedCullingMethod = Ogre::INSTANCING_CULLING_SINGLETHREAD;
    if (numThreads > 1) threadedCullingMethod = Ogre::INSTANCING_CULLING_THREADED;
    _ogreSceneManager = _root->createSceneManager(Ogre::ST_GENERIC, numThreads, threadedCullingMethod);

    // Some sane defaults for Shadow Mapping
    _ogreSceneManager->setShadowDirectionalLightExtrusionDistance(500.0f);
    _ogreSceneManager->setShadowFarDistance(500.0f);
}

void RenderSystem::CreateCamera()
{
    _ogreCamera = _ogreSceneManager->createCamera("MainCamera");

    // Set to some default location
    _ogreCamera->setPosition(Ogre::Vector3(0.0f, 0.0f, 5.0f));
    _ogreCamera->lookAt(Ogre::Vector3::ZERO);
    _ogreCamera->setNearClipDistance(0.1f);
    _ogreCamera->setFarClipDistance(200.0f);

    _ogreCamera->setAspectRatio(Ogre::Real(_ogreWindow->getWidth()) / Ogre::Real(_ogreWindow->getHeight()));
    _ogreCamera->setAutoAspectRatio(true);
}

void RenderSystem::CreateCompositor()
{
    // A compositor describes the order in which elements are combined
    // to create a scene. We will create a basic one for now
    Ogre::CompositorManager2* compositorManager = _root->getCompositorManager2();
    const Ogre::String workspaceName = "default scene workspace";
    const Ogre::IdString workspaceNameHash = workspaceName;
    compositorManager->createBasicWorkspaceDef(workspaceName, _clearColor);
    compositorManager->addWorkspace(_ogreSceneManager, _ogreWindow, _ogreCamera, workspaceNameHash, true);
}

void RenderSystem::LoadHlms()
{
    // Figure out where the HLMS implementations live
    // resources.cfg has a special entry for it
    Ogre::ConfigFile cf;
    cf.load("resources.cfg");

    Ogre::String dataFolder = cf.getSetting("DoNotUseAsResource", "Hlms", "");

    Ogre::RenderSystem* renderSystem = _root->getRenderSystem();

    Ogre::Archive* archiveLibrary = Ogre::ArchiveManager::getSingletonPtr()->load(
                dataFolder + "Hlms/Common/GLSL", "FileSystem", true);

    Ogre::Archive* archiveLibraryAny = Ogre::ArchiveManager::getSingletonPtr()->load(
                dataFolder + "Hlms/Common/Any", "FileSystem", true);

    Ogre::ArchiveVec library;
    library.push_back(archiveLibrary);
    library.push_back(archiveLibraryAny);

    Ogre::Archive* archivePbs = Ogre::ArchiveManager::getSingletonPtr()->load(
                dataFolder + "Hlms/Pbs/GLSL", "FileSystem", true);
    Ogre::HlmsPbs* hlmsPbs = OGRE_NEW Ogre::HlmsPbs(archivePbs, &library);
    Ogre::Root::getSingletonPtr()->getHlmsManager()->registerHlms(hlmsPbs);
}




