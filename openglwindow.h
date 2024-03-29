#include <QtGui/QWindow>
#include <QtGui/QOpenGLFunctions>
#include <QTimer>

class QPainter;
class QOpenGLContext;
class QOpenGLPaintDevice;

class OpenGLWindow : public QWindow, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit OpenGLWindow(QWindow *parent = 0);
    ~OpenGLWindow();

    virtual void render(QPainter *painter);
    virtual void render();

    virtual void initialize();

    void setAnimating(bool animating);
    void setAnimationDelay(int animation_delay);

public slots:
    void renderLater();
    void renderNow();

protected:
    bool event(QEvent *event);

    void exposeEvent(QExposeEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    bool m_update_pending;
    bool m_animating;
    int m_animation_delay;

    QOpenGLContext *m_context;
    QOpenGLPaintDevice *m_device;
    QTimer m_timer;

private slots:
    void postUpdateRequest();
};
