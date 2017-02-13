//
//  debugdraw.cpp
//  qface
//
//  Created by tangshi on 16/11/4.
//
//

#include <QtGui/QOpenGLBuffer>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLVertexArrayObject>
#include "facewindow.h"
#include "debugdraw.h"

/// x, y, z
static GLfloat kVertices[] = {
    0.0f, 0.0f, 0.0f,
    0.618f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.618f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.618f
};

//static GLfloat kVertices[] = {
//-0.5f, -0.5f, 0.0f,
//0.5f, -0.5f, 0.0f,
//0.0f,  0.5f, 0.0f,
//-0.5f, -0.5f, 0.0f,
//0.5f, -0.5f, 0.0f,
//0.0f,  0.5f, 0.0f
//};

static GLfloat kColors[] = {
    0.5f, 0.5f, 0.5f,
    1.0f, 0.0f, 0.0f,
    0.5f, 0.5f, 0.5f,
    0.0f, 1.0f, 0.0f,
    0.5f, 0.5f, 0.5f,
    0.0f, 0.0f, 1.0f
};

const GLchar *kVertexShader =
"#version 330 core\n"
"layout(location = 0) in vec3 positionAttr;\n"
"layout(location = 1) in vec3 colorAttr;\n"
"out vec3 color;\n"
"void main() {\n"
"  color = colorAttr;\n"
"  gl_Position = vec4(positionAttr, 1.0f);\n"
"}\n\0";

const GLchar *kFragmentShader =
"#version 330 core\n"
"in vec3 color;\n"
"out vec3 outColor;\n"
"void main() {\n"
"  outColor = color;\n"
"}\n\0";

DebugDraw::DebugDraw(FaceWindow *parent, int axes) : mParent(parent) {

    if (axes >= (1 << 3) || axes < 0) {
        mAxes = AxisMask::None;
    }
    else {
        mAxes = axes;
    }

    mProgram = new QOpenGLShaderProgram(mParent);
    mProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, kVertexShader);
    mProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, kFragmentShader);
//    mProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fragment.glsl");
    mProgram->link();

    auto mPositionBuf = new QOpenGLBuffer();
    mPositionBuf->create();
    mPositionBuf->setUsagePattern(QOpenGLBuffer::StaticDraw);
    mPositionBuf->bind();
    mPositionBuf->allocate(kVertices, sizeof(kVertices));
    mPositionBuf->release();

    auto mColorBuf = new QOpenGLBuffer();
    mColorBuf->create();
    mColorBuf->setUsagePattern(QOpenGLBuffer::StaticDraw);
    mColorBuf->bind();
    mColorBuf->allocate(kColors, sizeof(kColors));
    mColorBuf->release();

    mVAO = new QOpenGLVertexArrayObject(mParent);
    mVAO->create();

    mVAO->bind();
    {
        mPositionBuf->bind();
        mProgram->enableAttributeArray("positionAttr");
        mProgram->setAttributeBuffer("positionAttr", GL_FLOAT, 0, 3);
        mPositionBuf->release();
    }
    mVAO->release();
}

void DebugDraw::render() {

    mProgram->bind();
    mVAO->bind();
//    for (int i=0; i<3; i++) {
//        if (mAxes & (1 << i)) {
//            mParent->glDrawArrays(GL_LINES, i*2, 2);
//        }
//    }
    mParent->glDrawArrays(GL_TRIANGLES, 0, 6);
    mVAO->release();
    mProgram->release();
}

DebugDraw::~DebugDraw() {
    delete mProgram;
    delete mVAO;
}

