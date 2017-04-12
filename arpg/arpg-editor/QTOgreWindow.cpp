#include "QTOgreWindow.h"

QTOgreWindow::QTOgreWindow(OgreLogWatcher* logWatcher, QWindow *parent)
    : QWindow(parent),
      _update_pending(false),
      _animating(false),
      _renderSystem(logWatcher)
{
    setAnimating(true);
    installEventFilter(this);
}

QTOgreWindow::~QTOgreWindow()
{
}

void QTOgreWindow::render(QPainter *painter)
{
    Q_UNUSED(painter);
}

void QTOgreWindow::render()
{
    Ogre::WindowEventUtilities::messagePump();

    // only render if Initialized
    if (_renderSystem.IsInitialized())
        _renderSystem.Render();
}

void QTOgreWindow::setAnimating(bool animating)
{
    _animating = animating;

    if (animating)
        renderLater();
}

RenderSystem *QTOgreWindow::GetRenderSystem()
{
    return &_renderSystem;
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
            // Our window width/height isn't valid until the first
            // resize, so we'll check if our RenderSystem is initialized
            // if its not, its our first time through and we'll Initialize
            // Otherwise we'll handle the ResizeEvent by forwarding it to the
            // RenderSystem.

            if (!_renderSystem.IsInitialized())
            {
                _renderSystem.Initialize(this->width(), this->height(), this->winId(), this->winId());


            }
            else
            {
                // Handle Resize
                _renderSystem.Resize(this->width(), this->height());
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

