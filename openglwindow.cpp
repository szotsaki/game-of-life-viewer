#include "openglwindow.h"

#include <QtCore/QCoreApplication>

#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>

OpenGLWindow::OpenGLWindow(QWindow *parent)
    : QWindow(parent)
    , m_update_pending(false)
    , m_animating(false)
    , m_animation_delay(0)
    , m_context(0)
    , m_device(0)
{
    setSurfaceType(QWindow::OpenGLSurface);

    m_timer.setParent(this);
    m_timer.setSingleShot(true);
    m_timer.setTimerType(Qt::PreciseTimer);
}

OpenGLWindow::~OpenGLWindow()
{
    delete m_device;
}
void OpenGLWindow::render(QPainter *painter)
{
    Q_UNUSED(painter);
}

void OpenGLWindow::initialize()
{
}

void OpenGLWindow::render()
{
    if (!m_device)
        m_device = new QOpenGLPaintDevice;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    m_device->setSize(size());

    QPainter painter(m_device);
    render(&painter);
}

void OpenGLWindow::renderLater()
{
    if (!m_update_pending) {
        m_update_pending = true;
        m_timer.start();
    }
}

void OpenGLWindow::postUpdateRequest()
{
    QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
}

bool OpenGLWindow::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::UpdateRequest:
        renderNow();
        return true;
    default:
        return QWindow::event(event);
    }
}

void OpenGLWindow::exposeEvent(QExposeEvent *event)
{
    Q_UNUSED(event);

    if (isExposed())
        renderNow();
}

void OpenGLWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    if (isExposed())
        renderNow();
}

void OpenGLWindow::renderNow()
{
    if (!isExposed())
        return;

    m_update_pending = false;

    bool needsInitialize = false;

    if (!m_context) {
        m_context = new QOpenGLContext(this);
        m_context->setFormat(requestedFormat());
        m_context->create();

        needsInitialize = true;
    }

    m_context->makeCurrent(this);

    if (needsInitialize) {
        initializeOpenGLFunctions();
        initialize();
    }

    render();

    m_context->swapBuffers(this);

    if (m_animating)
        renderLater();
}

void OpenGLWindow::setAnimating(bool animating)
{
    m_animating = animating;

    if (animating)
        renderLater();
}

void OpenGLWindow::setAnimationDelay(int animation_delay)
{
    m_animation_delay = animation_delay;
    if (animation_delay > 0) {
        m_animating = true;
        m_timer.setInterval(animation_delay);
        connect(&m_timer, SIGNAL(timeout()), this, SLOT(postUpdateRequest()));
    } else {
        m_animating = true;
        m_timer.disconnect();
    }
}
