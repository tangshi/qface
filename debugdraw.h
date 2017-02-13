//
//  debugdraw.hpp
//  qface
//
//  Created by tangshi on 16/11/4.
//
//

#ifndef debugdraw_h
#define debugdraw_h

#include <QtGui/QOpenGLFunctions>


QT_BEGIN_NAMESPACE
class QOpenGLShaderProgram;
class QOpenGLVertexArrayObject;
QT_END_NAMESPACE

class FaceWindow;

class DebugDraw : QObject {

public:

    enum AxisMask {
        None = 0,
        X = 1,
        Y = 1 << 1,
        Z = 1 << 2
    };

public:
    DebugDraw(FaceWindow *parent, int axes);

    ~DebugDraw();

    int axes() { return mAxes; }
    void setAxes(int axes) { mAxes = axes; }
    void enableAxis(AxisMask axis) { mAxes = (mAxes | axis);}
    void disableAxis(AxisMask axis) { mAxes = (mAxes & (!axis)); }

    void render();

private:
    int mAxes;

    FaceWindow *mParent;

    QOpenGLShaderProgram *mProgram;
    QOpenGLVertexArrayObject *mVAO;

};

#endif /* debugdraw_hpp */
