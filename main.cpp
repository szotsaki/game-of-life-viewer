#include "openglwindow.h"

#include <QtGui/QGuiApplication>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>
#include <QDebug>
#include <QFile>
#include <QtCore/qmath.h>
#include <QKeyEvent>

#include <algorithm>

class GameOfLifeWindow : public OpenGLWindow
{
public:
    int m_width, m_height, m_totalFrames, m_currentFrame, m_speed;
    QStringList m_list;

    GameOfLifeWindow();
    ~GameOfLifeWindow();

    void initialize();
    void render();

protected:
    void keyPressEvent(QKeyEvent *);

private:
    GLfloat *m_coordinates;
    QOpenGLShaderProgram *m_program;
};

GameOfLifeWindow::GameOfLifeWindow()
    : m_width(0)
    , m_height(0)
    , m_totalFrames(0)
    , m_currentFrame(0)
    , m_speed(1000)
    , m_coordinates(nullptr)
    , m_program(nullptr)
{
}

GameOfLifeWindow::~GameOfLifeWindow()
{
    delete [] m_coordinates;
}

bool loadLifFile(GameOfLifeWindow &gameOfLifeWindow);

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    QSurfaceFormat format;
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSwapBehavior(QSurfaceFormat::TripleBuffer);

    GameOfLifeWindow window;
    if (! loadLifFile(window)) {
        return 1;
    }

    window.setFormat(format);
    window.resize(640, 640);
    window.show();
    window.setAnimating(true);

    return app.exec();
}

static const char *vertexShaderSource =
    "#version 330\n"
    "\n"
    "layout(location = 0) in vec4 position;\n"
    "void main() {\n"
    "   gl_Position = position;\n"
    "}\n";

static const char *fragmentShaderSource =
    "#version 330\n"
    "\n"
    "out vec4 outputColor;\n"
    "void main() {\n"
    "   outputColor = vec4(0.0);\n"
    "}\n";


void GameOfLifeWindow::initialize()
{
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->link();

    this->setAnimationDelay(m_speed);
    m_coordinates = new GLfloat[m_width * m_height * 2];

    glDisable(GL_BLEND);
    glDisable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_FASTEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void GameOfLifeWindow::render()
{
    const int size = std::min(width(), height());
    glPointSize(width() / m_width / 1.5);
    glViewport(0, 0, size, size);
    glClear(GL_COLOR_BUFFER_BIT);

    m_program->bind();

    //qDebug() << "Painting frame" << m_currentFrame + 1;

    int coordinates = 0;
    for (int currentLine = m_currentFrame * m_height, y = 0;
         (currentLine < m_currentFrame * m_height + m_height) && (currentLine < m_list.length());
         ++currentLine, ++y) {

        QString line = m_list.at(currentLine);
        for(int x = 0; (x < line.size()) && (x < m_width); ++x) {
            if (line[x] == QChar('.')) {
                continue;
            }

            m_coordinates[coordinates++] = -1 + (2.0 / m_width ) * x;
            m_coordinates[coordinates++] =  1 - (2.0 / m_height) * y;
        }
    }


    glVertexAttribPointer(0, 2, GL_FLOAT, GL_TRUE, 0, m_coordinates);
    glEnableVertexAttribArray(0);
    glDrawArrays(GL_POINTS, 0, coordinates / 2);
    glDisableVertexAttribArray(0);

    m_program->release();

    m_currentFrame = m_currentFrame == m_totalFrames - 1 ? 0 : m_currentFrame + 1;
    std::fill(m_coordinates, m_coordinates + (m_width * m_height), 0);
}


void GameOfLifeWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Escape) {
        qApp->quit();
    }
}

bool loadLifFile(GameOfLifeWindow &gameOfLifeWindow)
{
    QFile input;
    QTextStream inputText;

    input.setFileName("/home/aki/temp/gol.lif");
    if (!input.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open file" << input.fileName() << ":" << input.errorString();
        return false;
    }
    inputText.setDevice(&input);
    QStringList lines = inputText.readAll().split("\n");
    input.close();

    for (int i = 0, size = lines.size(); i < size; ++i) {
        if (lines[i].startsWith("#") || lines[i].isEmpty()) {
            continue;
        }
        if (lines[i].startsWith("width", Qt::CaseInsensitive)) {
            gameOfLifeWindow.m_width = lines[i].section("=", 1).trimmed().toInt();
            continue;
        }
        if (lines[i].startsWith("height", Qt::CaseInsensitive)) {
            gameOfLifeWindow.m_height = lines[i].section("=", 1).trimmed().toInt();
            continue;
        }
        if (lines[i].startsWith("frames", Qt::CaseInsensitive)) {
            gameOfLifeWindow.m_totalFrames = lines[i].section("=", 1).trimmed().toInt();
            continue;
        }
        if (lines[i].startsWith("speed", Qt::CaseInsensitive)) {
            gameOfLifeWindow.m_speed = lines[i].section("=", 1).trimmed().toInt();
            continue;
        }

        gameOfLifeWindow.m_list.append(lines[i]);
    }

    return true;
}
