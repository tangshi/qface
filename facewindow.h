#ifndef FACE_WINDOW_H
#define FACE_WINDOW_H

#include <QWindow>
#include <QOpenGLFunctions>

QT_BEGIN_NAMESPACE
class QOpenGLContext;
class QOpenGLPaintDevice;
class QOpenGLBuffer;
class QOpenGLShaderProgram;
class QOpenGLVertexArrayObject;
class QKeyEvent;
QT_END_NAMESPACE

class FaceData;

class FaceWindow : public QWindow, public QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit FaceWindow(const FaceData *facedata, QWindow *parent = nullptr);
    ~FaceWindow();

    void initialize();

    void render();

    bool isAnimating();
    void setAnimating(bool animating);

    const FaceData *mFaceData;

public slots:
    void renderLater();
    void renderNow();

protected:
    bool event(QEvent *event) override;
    void exposeEvent(QExposeEvent *event) override;
    void keyReleaseEvent(QKeyEvent *) override;

private:
    void setupShaderProgram();
    void setupVertexBuffers();
    void setupVertexArrayObject();

    GLuint mMVP;

    QOpenGLShaderProgram *mProgram;
    QOpenGLVertexArrayObject *mVAO;

    QOpenGLBuffer *mPositionBuf;
    QOpenGLBuffer *mColorBuf;
    QOpenGLBuffer *mIndexBuf;

    GLenum mPolygonMode;

    bool mUpdatePending;
    bool mAnimating;

    QOpenGLContext *mContext;
    QOpenGLPaintDevice *mDevice;

    int mFrame;
};

#endif // FACE_WINDOW_H
