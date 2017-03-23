#include "QTOgreWindow.h"
#include <Compositor/OgreCompositorManager2.h>

QTOgreWindow::QTOgreWindow(QWindow *parent)
    : QWindow(parent), _update_pending(false), _animating(false),
      _root(nullptr), _ogreWindow(nullptr), _ogreCamera(nullptr)
{
    setAnimating(true);
    installEventFilter(this);
    // Set background fill color to grey
    // Cornflower Blue
    _ogreBackground = Ogre::ColourValue(100.f / 255.f, 149.f / 255.f, 237.f / 255.f, 1.0f);
}

QTOgreWindow::~QTOgreWindow()
{
    delete _root;
}

void QTOgreWindow::render(QPainter *painter)
{
    Q_UNUSED(painter);
}
//
void QTOgreWindow::render()
{
    Ogre::WindowEventUtilities::messagePump();
    _root->renderOneFrame();
}

void QTOgreWindow::initialize()
{
    _root = new Ogre::Root();
    Ogre::ConfigFile ogreConfig;

    ogreConfig.load("resources.cfg");

    auto section_iter = ogreConfig.getSectionIterator();
    Ogre::String secName, typeName, archName;

    while (section_iter.hasMoreElements())
    {
        secName = section_iter.peekNextKey();
        auto settings = section_iter.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for(i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                        archName, typeName, secName);
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
    _ogreCamera->setPosition(Ogre::Vector3(0.0f, 0.0f, 10.0f));
    _ogreCamera->lookAt(Ogre::Vector3(0.0f, 0.0f, -300.0f));
    _ogreCamera->setNearClipDistance(0.1f);
    _ogreCamera->setFarClipDistance(200.0f);

    createCompositor();

    _ogreCamera->setAspectRatio(Ogre::Real(_ogreWindow->getWidth()) /
                                Ogre::Real(_ogreWindow->getHeight()));

    _ogreCamera->setAutoAspectRatio(true);

    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();



    createScene();

    _root->addFrameListener(this);
}

void QTOgreWindow::createScene()
{

    Ogre::Archive *archiveLibrary = Ogre::ArchiveManager::getSingletonPtr()->load(
                    "/usr/local/share/OGRE/Media/Hlms/Common/GLSL",
                    "FileSystem", true );

    Ogre::Archive *archiveLibraryAny = Ogre::ArchiveManager::getSingletonPtr()->load(
                    "/usr/local/share/OGRE/Media/Hlms/Common/Any",
                    "FileSystem", true );

    Ogre::ArchiveVec library;
    library.push_back( archiveLibrary );
    library.push_back( archiveLibraryAny );


    Ogre::Archive* archiveUnlit = Ogre::ArchiveManager::getSingletonPtr()->load(
                "/usr/local/share/OGRE/Media/Hlms/Unlit/GLSL", "FileSystem", true);

    Ogre::HlmsUnlit* hlmsUnlit = new Ogre::HlmsUnlit(archiveUnlit, &library);
    _root->getHlmsManager()->registerHlms(hlmsUnlit);

    // Just a sample, will change a lot

    // Not sure how hemisphere lighting works, should play with this...
    _ogreSceneManager->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f),
                                       Ogre::ColourValue(0.5f, 0.5f, 0.5f),
                                       Ogre::Vector3(0.0f, 0.0f, 0.0f));

    //Ogre::v1::Entity* sphereMesh = _ogreSceneManager->createEntity(Ogre::SceneManager::PT_SPHERE);

    //Ogre::Item* item = _ogreSceneManager->createItem("cube.mesh");



    //Ogre::SceneNode* childSceneNode = _ogreSceneManager->getRootSceneNode()->createChildSceneNode();

   // childSceneNode->attachObject(item);

//    Ogre::MaterialPtr sphereMaterial = Ogre::MaterialManager::getSingleton()
//            .create("SphereMaterial", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);

//    sphereMaterial->getTechnique(0)->getPass(0)->setAmbient(0.1f, 0.1f, 0.1f);
//    sphereMaterial->getTechnique(0)->getPass(0)->setDiffuse(0.2f, 0.2f, 0.2f, 1.0f);
//    sphereMaterial->getTechnique(0)->getPass(0)->setSpecular(0.9f, 0.9f, 0.9f, 1.0f);

//    sphereMesh->setMaterialName("SphereMaterial");
//    childSceneNode->setPosition(Ogre::Vector3(0.0f, 0.0f, 0.0f));
//    childSceneNode->setScale(Ogre::Vector3(0.1f, 0.1f, 0.1f));

//    Ogre::SceneNode* pLightNode = _ogreSceneManager->getRootSceneNode()->createChildSceneNode();
//    Ogre::Light* light = _ogreSceneManager->createLight();
//    pLightNode->attachObject(light);
//    pLightNode->setPosition(20.0f, 80.0f, 50.0f);
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

