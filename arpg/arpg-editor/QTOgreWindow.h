#pragma once

#include <QApplication>
#include <QKeyEvent>
#include <QWindow>

#include <Ogre.h>
#include <Hlms/Unlit/OgreHlmsUnlit.h>
#include <Hlms/Pbs/OgreHlmsPbs.h>
#include <OgreItem.h>
#include "OgreLogWatcher.h"

class QTOgreWindow : public QWindow, public Ogre::FrameListener
{
    Q_OBJECT
public:
    explicit QTOgreWindow(OgreLogWatcher* logWatcher = NULL, QWindow* parent = NULL);
    ~QTOgreWindow();

    virtual void render(QPainter* painter);
    virtual void render();
    virtual void initialize();
    virtual void createScene();

    virtual void createCompositor();

    void setAnimating(bool animating);

public slots:
    virtual void renderLater();
    virtual void renderNow();

    virtual bool eventFilter(QObject* target, QEvent* event);

signals:
    void entitySelected(Ogre::v1::Entity* entity);

protected:
    Ogre::Root* _root;
    Ogre::RenderWindow* _ogreWindow;
    Ogre::SceneManager* _ogreSceneManager;
    Ogre::Camera* _ogreCamera;
    Ogre::ColourValue _ogreBackground;
    OgreLogWatcher* _logWatcher;

    Ogre::Vector3 _prev_mouse_pos;

    unsigned long _currentTime;

    bool _update_pending;
    bool _animating;

    virtual void keyPressEvent(QKeyEvent * ev);
    virtual void keyReleaseEvent(QKeyEvent * ev);
    virtual void mouseMoveEvent(QMouseEvent* e);
    virtual void wheelEvent(QWheelEvent* e);
    virtual void mousePressEvent(QMouseEvent* e);
    virtual void mouseReleaseEvent(QMouseEvent* e);
    virtual void exposeEvent(QExposeEvent *event);
    virtual bool event(QEvent *event);

    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);

    void log(Ogre::String msg);
    void log(QString msg);
};
