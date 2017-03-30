#include "QTOgreWindow.h"
#include <QDir>
#include <Compositor/OgreCompositorManager2.h>

#include <OgreMesh.h>
#include <OgreMeshManager.h>
#include <OgreMeshManager2.h>
#include <OgreMesh2.h>
#include <OGRE/Hlms/Pbs/OgreHlmsPbsDatablock.h>

QTOgreWindow::QTOgreWindow(OgreLogWatcher* logWatcher, QWindow *parent)
    : QWindow(parent), _update_pending(false), _animating(false),
      _root(nullptr), _ogreWindow(nullptr), _ogreCamera(nullptr), _logWatcher(logWatcher), _prev_mouse_pos(Ogre::Vector3::ZERO), _currentTime(0)
{
    setAnimating(true);
    installEventFilter(this);
    // Set background fill color to grey
    // Cornflower Blue
    _ogreBackground = Ogre::ColourValue(100.f / 255.f, 149.f / 255.f, 237.f / 255.f, 1.0f);
}

QTOgreWindow::~QTOgreWindow()
{
    // unregister our logWatcher
    Ogre::LogManager::getSingletonPtr()->getDefaultLog()->removeListener(_logWatcher);
    delete _root;
}

void QTOgreWindow::render(QPainter *painter)
{
    Q_UNUSED(painter);
}
//
void QTOgreWindow::render()
{
    static Ogre::Timer timer;
    static int frameTimer = 0;

    auto timeSince = timer.getMilliseconds() - _currentTime;

    _currentTime = timer.getMilliseconds();

    frameTimer += timeSince;

    if (frameTimer < 17)
        return;

    frameTimer = 0;

    Ogre::WindowEventUtilities::messagePump();
    _root->renderOneFrame();
}

void QTOgreWindow::initialize()
{
    _root = new Ogre::Root();

    // If the user supplied a logwatcher, register it now
    if (_logWatcher != NULL)
        Ogre::LogManager::getSingletonPtr()->getDefaultLog()->addListener(_logWatcher);

    // Set the log level for our log...
    Ogre::LogManager::getSingletonPtr()->getDefaultLog()->setLogDetail(Ogre::LoggingLevel::LL_BOREME);

    Ogre::ConfigFile ogreConfig;

    ogreConfig.load("resources.cfg");

    auto pwd = QDir::currentPath();

    auto section_iter = ogreConfig.getSectionIterator();
    Ogre::String secName, typeName, archName;

    while (section_iter.hasMoreElements())
    {
        secName = section_iter.peekNextKey();
        auto settings = section_iter.getNext();

        if (secName != "Hlms")
        {
            Ogre::ConfigFile::SettingsMultiMap::iterator i;
            for(i = settings->begin(); i != settings->end(); ++i)
            {
                typeName = i->first;
                archName = i->second;
                Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                            archName, typeName, secName);
            }
        }
    }

    const Ogre::RenderSystemList& rsList = _root->getAvailableRenderers();
    Ogre::RenderSystem* rs = rsList[0];

    Ogre::StringVector renderOrder;
    renderOrder.push_back("OpenGL");
    renderOrder.push_back("OpenGL 3+");

    for(auto iter = renderOrder.begin(); iter != renderOrder.end(); iter++)
    {
        for(auto it = rsList.begin(); it != rsList.end(); it++)
        {
            if ((*it)->getName().find(*iter) != Ogre::String::npos)
            {
                rs = *it;
                break;
            }
        }
        if (rs != NULL) break;
    }
    if (rs == NULL)
    {
        if (!_root->restoreConfig())
        {
            if (!_root->showConfigDialog())
            {
                OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                            "Abort render system config.",
                            "QTOgreWindow::initialize");
            }
        }
    }

    QString dimensions = QString("%1 x %2").arg(this->width()).arg(this->height());
    rs->setConfigOption("Video Mode", dimensions.toStdString());
    rs->setConfigOption("Full Screen", "No");
    rs->setConfigOption("VSync", "Yes");
    _root->setRenderSystem(rs);
    _root->getRenderSystem()->setConfigOption( "sRGB Gamma Conversion", "Yes" );

    //_ogreSceneManager->setForward3D(true, 4, 4, 5, 96, 3.04, 200.0f);
    _root->initialise(false);



    Ogre::NameValuePairList parameters;

    if (rs->getName().find("GL") <= rs->getName().size())
        parameters["currentGLContext"] = Ogre::String("false");

    parameters["externalWindowHandle"] = Ogre::StringConverter::toString((unsigned long)(this->winId()));
    parameters["parentWindowHandle"] = Ogre::StringConverter::toString((unsigned long)(this->winId()));

    _ogreWindow = _root->createRenderWindow("QT Window",
                                            this->width(), this->height(),
                                            false, &parameters);
    _ogreWindow->setVisible(true);

    const size_t numThreads = std::max<int>(1, Ogre::PlatformInformation::getNumLogicalCores());
    Ogre::InstancingThreadedCullingMethod threadedCullingMethod = Ogre::INSTANCING_CULLING_SINGLETHREAD;
    if (numThreads > 1)threadedCullingMethod = Ogre::INSTANCING_CULLING_THREADED;
    _ogreSceneManager = _root->createSceneManager(Ogre::ST_GENERIC, numThreads, threadedCullingMethod);

    _ogreCamera = _ogreSceneManager->createCamera("MainCamera");
    _ogreCamera->setPosition(Ogre::Vector3(0.0f, 20.0f, 0.0f));
    _ogreCamera->lookAt(Ogre::Vector3(0.0f, 0.0f, 0.0f));
    _ogreCamera->setNearClipDistance(0.1f);
    _ogreCamera->setFarClipDistance(200.0f);
    _ogreSceneManager->setShadowDirectionalLightExtrusionDistance( 500.0f );
    _ogreSceneManager->setShadowFarDistance( 500.0f );

    createCompositor();

    _ogreCamera->setAspectRatio(Ogre::Real(_ogreWindow->getWidth()) /
                                Ogre::Real(_ogreWindow->getHeight()));

    _ogreCamera->setAutoAspectRatio(true);

    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);




    createScene();

    _root->addFrameListener(this);
}

void QTOgreWindow::createScene()
{
    Ogre::ConfigFile cf;
    cf.load( "/home/keaton/Desktop/arpg/arpg/arpg/build/resources.cfg" );

    Ogre::String dataFolder = cf.getSetting( "DoNotUseAsResource", "Hlms", "" );

    if( dataFolder.empty() )
        dataFolder = "./";
    else if( *(dataFolder.end() - 1) != '/' )
        dataFolder += "/";

    Ogre::RenderSystem *renderSystem = _root->getRenderSystem();

    Ogre::String shaderSyntax = "GLSL";
    if( renderSystem->getName() == "Direct3D11 Rendering Subsystem" )
        shaderSyntax = "HLSL";
    else if( renderSystem->getName() == "Metal Rendering Subsystem" )
        shaderSyntax = "Metal";

    Ogre::Archive *archiveLibrary = Ogre::ArchiveManager::getSingletonPtr()->load(
                    dataFolder + "Hlms/Common/" + shaderSyntax,
                    "FileSystem", true );
    Ogre::Archive *archiveLibraryAny = Ogre::ArchiveManager::getSingletonPtr()->load(
                    dataFolder + "Hlms/Common/Any",
                    "FileSystem", true );

    Ogre::ArchiveVec library;
    library.push_back( archiveLibrary );
    library.push_back( archiveLibraryAny );

    Ogre::Archive *archiveUnlit = Ogre::ArchiveManager::getSingletonPtr()->load(
                    dataFolder + "Hlms/Unlit/" + shaderSyntax,
                    "FileSystem", true );

    Ogre::HlmsUnlit *hlmsUnlit = OGRE_NEW Ogre::HlmsUnlit( archiveUnlit, &library );
    //Ogre::Root::getSingleton().getHlmsManager()->registerHlms( hlmsUnlit );

    Ogre::Archive *archivePbs = Ogre::ArchiveManager::getSingletonPtr()->load(
                    dataFolder + "Hlms/Pbs/" + shaderSyntax,
                    "FileSystem", true );
    Ogre::HlmsPbs *hlmsPbs = OGRE_NEW Ogre::HlmsPbs( archivePbs, &library );
    Ogre::Root::getSingleton().getHlmsManager()->registerHlms( hlmsPbs );

    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

        //_ogreSceneManager->setForwardClustered( true, 16, 8, 24, 96, 5, 500 );
        //_ogreSceneManager->setForward3D(true, 4, 4, 5, 96, 3.04, 200.0f);

    // Just a sample, will change a lot

    // Not sure how hemisphere lighting works, should play with this...
    //_ogreSceneManager->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f),
    //                                   Ogre::ColourValue(0.5f, 0.5f, 0.5f),
    //                                   Ogre::Vector3(0.0f, 0.0f, 0.0f));

    //Ogre::v1::Entity* sphereMesh = _ogreSceneManager->createEntity(Ogre::SceneManager::PT_SPHERE);

    //Ogre::Item* item = _ogreSceneManager->createItem("Sphere1000.mesh");

    //Ogre::SceneNode* childSceneNode = _ogreSceneManager->getRootSceneNode()->createChildSceneNode();


    /*         Ogre::v1::MeshPtr planeMeshV1 = Ogre::v1::MeshManager::getSingleton().createPlane( "Plane v1",
                                            Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                            Ogre::Plane( Ogre::Vector3::UNIT_Y, 1.0f ), 50.0f, 50.0f,
                                            1, 1, true, 1, 4.0f, 4.0f, Ogre::Vector3::UNIT_Z,
                                            Ogre::v1::HardwareBuffer::HBU_STATIC,
                                            Ogre::v1::HardwareBuffer::HBU_STATIC );
                                            */

    Ogre::Item* sphereItem = _ogreSceneManager->createItem("Sphere1000.mesh");
    Ogre::SceneNode* sphereSceneNode = _ogreSceneManager->getRootSceneNode()->createChildSceneNode();
    sphereSceneNode->setPosition(0, 8, 0);

    sphereItem->setDatablock("DebugSphere1000");
    sphereItem->setCastShadows(true);
    sphereSceneNode->attachObject(sphereItem);

    Ogre::v1::MeshPtr planeMeshv1 =
            Ogre::v1::MeshManager::getSingleton().createPlane("Plane v1", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::Plane(Ogre::Vector3::UNIT_Y, 1.0f), 50.0f, 50.0f, 1, 1, true, 1, 1.0f, 1.0f, Ogre::Vector3::UNIT_Z,
                                                              Ogre::v1::HardwareBuffer::HBU_STATIC, Ogre::v1::HardwareBuffer::HBU_STATIC);

    Ogre::MeshPtr planeMesh = Ogre::MeshManager::getSingleton().createManual("Plane",
                                                                             Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

    planeMesh->importV1(planeMeshv1.get(), true, true, true);

    Ogre::Item* item = _ogreSceneManager->createItem(planeMesh, Ogre::SCENE_DYNAMIC);

    Ogre::SceneNode* sceneNode = _ogreSceneManager->getRootSceneNode(Ogre::SCENE_DYNAMIC)->createChildSceneNode(Ogre::SCENE_DYNAMIC);

    sceneNode->setPosition(0, 0, 0);

    item->setDatablock("Ground");
    item->setCastShadows(true);
    sceneNode->attachObject(item);

    /*auto dataBlock = static_cast<Ogre::HlmsPbsDatablock*>(item->getSubItem(0)->getDatablock());
    auto samplerBlock(*dataBlock->getSamplerblock(Ogre::PBSM_DIFFUSE));

    samplerBlock.mU = Ogre::TAM_WRAP;
    samplerBlock.mV = Ogre::TAM_WRAP;
    samplerBlock.mW = Ogre::TAM_WRAP;

    dataBlock->setSamplerblock(Ogre::PBSM_DIFFUSE, samplerBlock);*/




    //item->setDatablock("pbs");

    //childSceneNode->attachObject(item);


//    Ogre::MaterialPtr sphereMaterial = Ogre::MaterialManager::getSingleton()
//            .create("SphereMaterial", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);

//    sphereMaterial->getTechnique(0)->getPass(0)->setAmbient(0.1f, 0.1f, 0.1f);
//    sphereMaterial->getTechnique(0)->getPass(0)->setDiffuse(0.2f, 0.2f, 0.2f, 1.0f);
//    sphereMaterial->getTechnique(0)->getPass(0)->setSpecular(0.9f, 0.9f, 0.9f, 1.0f);

//    sphereMesh->setMaterialName("SphereMaterial");
//    childSceneNode->setPosition(Ogre::Vector3(0.0f, 0.0f, 0.0f));
//    childSceneNode->setScale(Ogre::Vector3(0.1f, 0.1f, 0.1f));

      Ogre::Light* directionalLight = _ogreSceneManager->createLight();
      Ogre::SceneNode* lightNode = _ogreSceneManager->getRootSceneNode()->createChildSceneNode();
      lightNode->attachObject(directionalLight);
      directionalLight->setPowerScale(1.0f);
      directionalLight->setType(Ogre::Light::LT_DIRECTIONAL);
      directionalLight->setDirection(Ogre::Vector3(1, -1, -1).normalisedCopy());
      directionalLight->setCastShadows(true);
      directionalLight->setPowerScale(0);

      _ogreSceneManager->setAmbientLight( Ogre::ColourValue( 0.3f, 0.5f, 0.7f ) * 0.1f * 0.75f,
                                     Ogre::ColourValue( 0.6f, 0.45f, 0.3f ) * 0.065f * 0.75f,
                                     -directionalLight->getDirection() + Ogre::Vector3::UNIT_Y * 0.2f );

      Ogre::SceneNode* pLightNode = _ogreSceneManager->getRootSceneNode()->createChildSceneNode();
      Ogre::Light* light = _ogreSceneManager->createLight();
      pLightNode->attachObject(light);

      light->setDiffuseColour(1.0f, 0.0f, 0.0f);
      light->setSpecularColour(1.0f, 0.0f, 0.0f);
      light->setPowerScale(Ogre::Math::PI);
      light->setType(Ogre::Light::LT_SPOTLIGHT);

      pLightNode->setPosition(0.0f, 10.0f, 0.0f);

      light->setCastShadows(false);
      light->setDirection(Ogre::Vector3(0, -1, 0));
      light->setAttenuationBasedOnRadius(100.0f, .10f);

      _ogreSceneManager->setForward3D( true, 4,4,5,96,3,200 );
}

void QTOgreWindow::createCompositor()
{
    Ogre::CompositorManager2* compMan = _root->getCompositorManager2();
    const Ogre::String workspaceName = "default scene workspace";
    const Ogre::IdString workspaceNameHash = workspaceName;
    compMan->createBasicWorkspaceDef(workspaceName, _ogreBackground);
    compMan->addWorkspace(_ogreSceneManager, _ogreWindow, _ogreCamera, workspaceNameHash, true);
}

void QTOgreWindow::setAnimating(bool animating)
{
    _animating = animating;

    if (animating)
        renderLater();
}

void QTOgreWindow::renderLater()
{
    if (!_update_pending)
    {
        _update_pending = true;
        QApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    }
}

void QTOgreWindow::renderNow()
{
    if (!isExposed())
        return;

    if (_root == NULL)
        initialize();

    render();

    if (_animating)
        renderLater();
}

bool QTOgreWindow::eventFilter(QObject *target, QEvent *event)
{
    if (target == this)
    {
        if (event->type() == QEvent::Resize)
        {
            if (isExposed() && _ogreWindow != NULL)
            {
                _ogreWindow->resize(this->width(), this->height());
            }
        }
    }

    return false;
}

void QTOgreWindow::keyPressEvent(QKeyEvent *ev)
{
    Q_UNUSED(ev);
}

void QTOgreWindow::keyReleaseEvent(QKeyEvent *ev)
{
    Q_UNUSED(ev);
}

void QTOgreWindow::mouseMoveEvent(QMouseEvent *e)
{
    Q_UNUSED(e);

    if (e->buttons() == Qt::RightButton)
    {
        Ogre::Vector3 curr_pos = Ogre::Vector3(-e->x(), 0.0f, -e->y());
//        auto str = QString::asprintf("%f, %f", curr_pos.x, curr_pos.z);

        if (_prev_mouse_pos == Ogre::Vector3::ZERO)
            _prev_mouse_pos = curr_pos;

        Ogre::Vector3 translation = curr_pos - _prev_mouse_pos;

        translation.normalise();

        auto str = QString::asprintf("%f, %f", translation.x, translation.z);
        log(str);

        _prev_mouse_pos = curr_pos;
        translation.y = 0.0;

        _ogreCamera->move(translation * 0.1);
    }

}

void QTOgreWindow::wheelEvent(QWheelEvent *e)
{
    Q_UNUSED(e);
}

void QTOgreWindow::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
}

void QTOgreWindow::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
}

void QTOgreWindow::exposeEvent(QExposeEvent *event)
{
    Q_UNUSED(event);

    if (isExposed())
        renderNow();
}

bool QTOgreWindow::event(QEvent *event)
{
    switch (event->type())
    {
    case QEvent::UpdateRequest:
        _update_pending = false;
        renderNow();
        return true;

    default:
        return QWindow::event(event);
    }
}

bool QTOgreWindow::frameRenderingQueued(const Ogre::FrameEvent &evt)
{
    Q_UNUSED(evt);
    return true;
}

void QTOgreWindow::log(Ogre::String msg)
{
    if (Ogre::LogManager::getSingletonPtr() != NULL)
        Ogre::LogManager::getSingletonPtr()->logMessage(msg);
}

void QTOgreWindow::log(QString msg)
{
    log(Ogre::String(msg.toStdString().c_str()));
}

