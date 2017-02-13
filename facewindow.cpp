#include <QCoreApplication>
#include <QOpenGLContext>
#include <QOpenGLPaintDevice>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QScreen>
#include <QKeyEvent>
#include <QApplication>
#include "facewindow.h"
#include "facedata.h"


FaceWindow::FaceWindow(const FaceData *facedata, QWindow *parent)
    : QWindow(parent)
    , mFaceData(facedata)
    , mProgram(nullptr)
    , mVAO(nullptr)
    , mPolygonMode(GL_FILL)
    , mUpdatePending(false)
    , mAnimating(false)
    , mContext(0)
    , mDevice(0)
    , mFrame(0)
{
    setSurfaceType(QWindow::OpenGLSurface);

    QSurfaceFormat format;
    format.setProfile(QSurfaceFormat::OpenGLContextProfile::CoreProfile);
    format.setVersion(3, 3);
    format.setRenderableType(QSurfaceFormat::RenderableType::OpenGL);
    format.setSwapBehavior(QSurfaceFormat::SwapBehavior::DoubleBuffer);
    format.setDepthBufferSize(32);
    setFormat(format);
}

FaceWindow::~FaceWindow() {
    delete mProgram;
    delete mVAO;
    delete mIndexBuf;
    delete mColorBuf;
    delete mPositionBuf;
    delete mDevice;
}

void FaceWindow::keyReleaseEvent(QKeyEvent *event) {

    if (event->key() == Qt::Key_M) {
        mPolygonMode = (mPolygonMode == GL_FILL) ? GL_LINE : GL_FILL;
        renderLater();
    }
    else if (event->key() == Qt::Key_Q && event->modifiers().testFlag(Qt::ControlModifier)) {
        QApplication::quit();
    }
    else if (event->key() == Qt::Key_F) {
        if (windowState() != Qt::WindowFullScreen) {
            showFullScreen();
        }
        else {
            showNormal();
        }
    }
    else if (event->key() == Qt::Key_Space) {
        setAnimating(! isAnimating());
    }
    else if (event->key() == Qt::Key_R) {
        mFrame = 0;
        setAnimating(false);
        renderLater();
    }
    else if (event->key() == Qt::Key_B) {
        static bool b = true;
        if (b) {
            glDisable(GL_CULL_FACE);
        }
        else {
            glEnable(GL_CULL_FACE);
        }
        b = !b;
        renderLater();
    }
    else {
        return;
    }
}

void FaceWindow::initialize() {

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    setupShaderProgram();

    setupVertexBuffers();

    setupVertexArrayObject();
}

void FaceWindow::setupShaderProgram() {

    mProgram = new QOpenGLShaderProgram(this);
    mProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vertex.glsl");
    mProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fragment.glsl");
    mProgram->link();

    mMVP = mProgram->uniformLocation("MVP");
}

void FaceWindow::setupVertexBuffers() {

    const cv::Mat& shape = mFaceData->mu_shape();
    const cv::Mat& tex = mFaceData->mu_tex();
    const cv::Mat& tl = mFaceData->tl();

    mPositionBuf = new QOpenGLBuffer();
    mPositionBuf->create();
    mPositionBuf->setUsagePattern(QOpenGLBuffer::StaticDraw);
    mPositionBuf->bind();
    mPositionBuf->allocate(shape.data, shape.elemSize() * shape.rows * shape.cols);
    mPositionBuf->release();

    mColorBuf = new QOpenGLBuffer();
    mColorBuf->create();
    mColorBuf->setUsagePattern(QOpenGLBuffer::StaticDraw);
    mColorBuf->bind();
    mColorBuf->allocate(tex.data, tex.elemSize() * tex.rows * tex.cols);
    mColorBuf->release();

    mIndexBuf = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    mIndexBuf->create();
    mIndexBuf->setUsagePattern(QOpenGLBuffer::StaticDraw);
    mIndexBuf->bind();
    mIndexBuf->allocate(tl.data, tl.elemSize() * tl.rows * tl.cols);
    mIndexBuf->release();
}

void FaceWindow::setupVertexArrayObject() {

    mVAO = new QOpenGLVertexArrayObject(this);
    mVAO->create();

    mVAO->bind();
    {
        mPositionBuf->bind();
        mProgram->enableAttributeArray("positionAttr");
        mProgram->setAttributeBuffer("positionAttr", GL_FLOAT, 0, 3);
        mPositionBuf->release();

        mColorBuf->bind();
        mProgram->enableAttributeArray("colorAttr");
        mProgram->setAttributeBuffer("colorAttr", GL_FLOAT, 0, 3);
        mColorBuf->release();

        mIndexBuf->bind();
    }
    mVAO->release();
}


void FaceWindow::render()
{
    if (!mDevice)
    {
        mDevice = new QOpenGLPaintDevice;
    }

    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClearColor(0.25f, 0.3f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 matrix;
    matrix.perspective(60.0f, width()/height(), 1.0f, 1e+6);
    matrix.lookAt(QVector3D(0, 0, 0.3e+6), QVector3D(0, 0, 0), QVector3D(0, 1, 0));
    matrix.rotate(mFrame / screen()->refreshRate(), 0, 1, 0);

    mProgram->bind();
    mProgram->setUniformValue(mMVP, matrix);
    mVAO->bind();
    const cv::Mat& tl = mFaceData->tl();
    glPolygonMode(GL_FRONT_AND_BACK, mPolygonMode);
    glDrawElements(GL_TRIANGLES, tl.rows * tl.cols, GL_UNSIGNED_SHORT, 0);
    mProgram->release();

    ++mFrame;
}

void FaceWindow::renderLater()
{
    if (!mUpdatePending)
    {
        mUpdatePending = true;
        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    }
}

bool FaceWindow::event(QEvent *event)
{
    switch (event->type())
    {
    case QEvent::UpdateRequest:
        mUpdatePending = false;
        renderNow();
        return true;
    default:
        return QWindow::event(event);
    }
}

void FaceWindow::exposeEvent(QExposeEvent *event)
{
    Q_UNUSED(event);

    if (isExposed())
    {
        renderNow();
    }
}

void FaceWindow::renderNow()
{
    if (!isExposed())
    {
        return;
    }

    bool needsInitialize = false;

    if (!mContext)
    {
        mContext = new QOpenGLContext(this);
        mContext->setFormat(requestedFormat());
        mContext->create();

        needsInitialize = true;
    }

    mContext->makeCurrent(this);

    if (needsInitialize)
    {
        initializeOpenGLFunctions();
        initialize();
    }

    render();

    mContext->swapBuffers(this);

    if (mAnimating)
    {
        renderLater();
    }
}

bool FaceWindow::isAnimating()
{
    return mAnimating;
}

void FaceWindow::setAnimating(bool animating)
{
    mAnimating = animating;

    if (animating)
    {
        renderLater();
    }
}
