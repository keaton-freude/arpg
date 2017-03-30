#pragma once

#include <QApplication>
#include <QKeyEvent>
#include <QWindow>

#include "OgreLogWatcher.h"
#include "RenderSystem.h"

class QTOgreWindow : public QWindow, public Ogre::FrameListener
{
    Q_OBJECT
public:
    explicit QTOgreWindow(OgreLogWatcher* logWatcher = NULL, QWindow* parent = NULL);
    ~QTOgreWindow();

    virtual void render(QPainter* painter);
    virtual void render();

    void setAnimating(bool animating);

public slots:
    virtual void renderLater();
    virtual void renderNow();

    virtual bool eventFilter(QObject* target, QEvent* event);

protected:
    bool _update_pending;
    bool _animating;
    RenderSystem _renderSystem;

    virtual void keyPressEvent(QKeyEvent * ev);
    virtual void keyReleaseEvent(QKeyEvent * ev);
    virtual void mouseMoveEvent(QMouseEvent* e);
    virtual void wheelEvent(QWheelEvent* e);
    virtual void mousePressEvent(QMouseEvent* e);
    virtual void mouseReleaseEvent(QMouseEvent* e);
    virtual void exposeEvent(QExposeEvent *event);
    virtual bool event(QEvent *event);

    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
};
