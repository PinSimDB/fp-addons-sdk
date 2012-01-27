/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtGui>
#include <QtOpenGL>

#include <math.h>

#include "glwidget.h"

#include "ops-base.h"
#include "ops-tools.h"
#include "ops-lzo.h"
#include "ops-ms3d.h"
#include "ops-msole.h"
#include "ops-fp.h"
#include "ops-fp-writer.h"




GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers | QGL::StencilBuffer), parent)
{
    primaryModel = NULL;
    reflectionModel = NULL;
    maskModel = NULL;

    xRot = 0;
    yRot = 0;
    zRot = 0;

    zPos = 200;

    offset = 0;
    modelAngle = 0;

    drawWFModel = false;
    drawWFShapes = false;
    drawWFMask = false;
    drawBoundingBox = false;
    drawPlayfield = true;
    drawGrid = false;
    drawBall = false;

    ballPos[0] = -50.0f;
    ballPos[1] = 27.0f / 2;
    ballPos[2] = 0.0f;

    currentCollisionElement = -1;
}


GLWidget::~GLWidget()
{
}


QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const

{
    return QSize(400, 400);
}


static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}


void GLWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::setZPosition(int z)
{
    if (z != zPos) {
        zPos = z;
        emit zPositionChanged(z);
        updateGL();
    }
}

void GLWidget::setOffset(float off) {
    if (off != offset) {
        offset = off;
        emit offsetChanged(off);
        double doubleOffset = offset;
        emit offsetChanged(doubleOffset);
        updateGL();
    }
}

void GLWidget::setOffset(double off) {
    float floatOff = off;
    setOffset(floatOff);
}

void GLWidget::setAngle(int angle) {
    if (angle != modelAngle) {
        modelAngle = angle;
        emit angleChanged(angle);
        updateGL();
    }
}


void GLWidget::setDrawWFModel(bool flag) {
    if (flag != drawWFModel) {
        drawWFModel = flag;
        updateGL();
    }
}

void GLWidget::setDrawWFShapes(bool flag) {
    if (flag != drawWFShapes) {
        drawWFShapes = flag;
        updateGL();
    }
}

void GLWidget::setDrawWFMask(bool flag) {
    if (flag != drawWFMask) {
        drawWFMask = flag;
        updateGL();
    }
}

void GLWidget::setDrawBoundingBox(bool flag) {
    if (flag != drawBoundingBox) {
        drawBoundingBox = flag;
        updateGL();
    }
}

void GLWidget::setDrawPlayfield(bool flag) {
    if (flag != drawPlayfield) {
        drawPlayfield = flag;
        updateGL();
    }
}

void GLWidget::setDrawGrid(bool flag) {
    if (flag != drawGrid) {
        drawGrid = flag;
        updateGL();
    }
}

void GLWidget::setDrawBall(bool flag) {
    if (flag != drawBall) {
        drawBall = flag;
        updateGL();
    }
}

void GLWidget::setPrimaryModel(const char* ms3d, uint32_t len, int type) {
    if (primaryModel != NULL) {
        delete primaryModel;
        primaryModel = NULL;
    }
    if (ms3d != NULL) {
        primaryModel = new msModel();
        primaryModel->LoadFromMemory((uint8_t*)ms3d, len);

        // Get bounding box

        bbMins[0] = 99999;
        bbMins[1] = 99999;
        bbMins[2] = 99999;
        bbMaxs[0] = -99999;
        bbMaxs[1] = -99999;
        bbMaxs[2] = -99999;


        ms3d_vertex_t *ppVertex;
        for (int i = 0; i<primaryModel->GetNumVertices(); i++) {
            ppVertex = primaryModel->GetVertex(i);
            if (bbMins[0] > ppVertex->vertex[0]) {
                bbMins[0] = ppVertex->vertex[0];
            }
            if (bbMins[1] > ppVertex->vertex[1]) {
                bbMins[1] = ppVertex->vertex[1];
            }

            if (bbMins[2] > ppVertex->vertex[2]) {
                bbMins[2] = ppVertex->vertex[2];
            }
            if (bbMaxs[0] < ppVertex->vertex[0]) {
                bbMaxs[0] = ppVertex->vertex[0];
            }
            if (bbMaxs[1] < ppVertex->vertex[1]) {
                bbMaxs[1] = ppVertex->vertex[1];
            }

            if (bbMaxs[2] < ppVertex->vertex[2]) {
                bbMaxs[2] = ppVertex->vertex[2];
            }
        }

        primaryModelType = type;


    }
    updateGL();
}

void GLWidget::setMaskModel(const char* ms3d, uint32_t len) {
    if (maskModel != NULL) {
        delete maskModel;
        maskModel = NULL;
    }
    if (ms3d != NULL) {
        maskModel = new msModel();
        maskModel->LoadFromMemory((uint8_t*)ms3d, len);
    }
    updateGL();
}


void GLWidget::setReflectionModel(const char* ms3d, uint32_t len) {
    if (reflectionModel != NULL) {
        delete reflectionModel;
        reflectionModel = NULL;
    }
    if (ms3d != NULL) {
        reflectionModel = new msModel();
        reflectionModel->LoadFromMemory((uint8_t*)ms3d, len);
    }
    updateGL();
}


void GLWidget::setCurrentCollisionElement(int current) {
    if (current != currentCollisionElement) {
        currentCollisionElement = current;
        updateGL();
    }
}

void GLWidget::setCollisionElements(std::vector<ops::fp::FPModelCollisionData> elements) {
    collisionElements = elements;
    updateGL();
}


void GLWidget::initializeGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);

    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0f);

    glClearStencil(0);

    glShadeModel(GL_SMOOTH);

    // Light
    glEnable(GL_LIGHT0);
    GLfloat diffuse[4]= { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat ambient[4]= { 0.5f, 0.5f, 0.5f, 1.0f };    
    GLfloat model_ambient[4] = {0.2f, 0.2f, 0.2f, 1.0f};

    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);


    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 0);

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

    glEnable(GL_LIGHTING);

    // Texture
    glGenTextures(2, texture);

    glBindTexture(GL_TEXTURE_2D, texture[0]);
    QImage t;
    QImage b;
    b.load( ":/images/p1-beachwood.png" );
    t = QGLWidget::convertToGLFormat( b );
    glTexImage2D ( GL_TEXTURE_2D, 0, 3, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

    glBindTexture(GL_TEXTURE_2D, texture[1]);
    QImage t2;
    QImage b2;
    b2.load( ":/images/[Chrome-Silver].bmp" );
    t2 = QGLWidget::convertToGLFormat( b2 );
    glTexImage2D ( GL_TEXTURE_2D, 0, 3, t2.width(), t2.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t2.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

    glDisable(GL_TEXTURE_2D);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_CULL_FACE);
}


void GLWidget::resizeGL(int width, int height)
{
    float ratio = ((float)width) / height;
    glViewport(0  , 0 , width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, ratio, 10.0, 3000.0);
    glMatrixMode(GL_MODELVIEW);
}


void GLWidget::paintGL()
{

    if (primaryModelType == 9
        || maskModel != NULL)
    {
        setOffset(-bbMaxs[1]);
    } else {
        setOffset(-bbMins[1]);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glLoadIdentity();

    gluLookAt(0.0, 30.0, zPos, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);

    GLfloat position[4] = { -500, 1000, 1000, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    if (drawPlayfield) {
        // stencil mask
        glColorMask(false, false, false, false);
        glDepthMask(false);
        glEnable(GL_STENCIL_TEST);
        // add playfield in stencil
        glStencilFunc(GL_ALWAYS, 1, 1);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glBegin(GL_QUADS);
        glNormal3f( 0.0, 1.0, 0.0);
        glVertex3f( -258.0, 0.0, 534);
        glVertex3f( 258.0, 0.0, 534);
        glVertex3f( 258.0, 0.0, -534);
        glVertex3f( -258.0, 0.0, -534);
        glEnd();
        // remove mask model from stencil
        if (maskModel != NULL) {
            glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);
            // TODO
            glPushMatrix();
            glRotatef(modelAngle / 16.0, 0.0, 1.0, 0.0);
            renderModel(maskModel);
            glPopMatrix();

        }
        glDepthMask(true);
        glColorMask(true, true, true, true);

        // reflection model
        glStencilFunc(GL_EQUAL, 1, 1);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        glPushMatrix();

        // Mmmm
        glPushMatrix();
        glScalef(-1.0f, 1.0f, -1.0f);
        glLightfv(GL_LIGHT0, GL_POSITION, position);
        glPopMatrix();

        double eq[4] = {0.0, -1.0, 0.0, 0.0};
        glClipPlane(GL_CLIP_PLANE0, eq);
        glEnable(GL_CLIP_PLANE0);
        glCullFace(GL_FRONT);
        glScalef(1.0f, -1.0f, 1.0f);

        if (drawBall) {
            glPushMatrix();

            glDisable(GL_LIGHTING);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texture[1]);

            glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
            glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
            glEnable(GL_TEXTURE_GEN_S);
            glEnable(GL_TEXTURE_GEN_T);

            glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

            glTranslatef(ballPos[0], ballPos[1], ballPos[2]);
            GLUquadric* quad = gluNewQuadric();
            gluQuadricNormals(quad, GLU_SMOOTH);
            gluSphere(quad, 27/2, 50, 50);

            glDisable(GL_TEXTURE_GEN_S);
            glDisable(GL_TEXTURE_GEN_T);

            glDisable(GL_TEXTURE_2D);
            glEnable(GL_LIGHTING);

            glPopMatrix();
        }



        glTranslatef(0.0, offset, 0.0);
        glRotatef(modelAngle / 16.0, 0.0, 1.0, 0.0);
        GLfloat no_mat[4] = {0.0f, 0.0f, 0.0f, 1.0f};
        GLfloat mat_ambient[4] = {0.2f, 0.2f, 0.2f, 1.0f};
        GLfloat mat_diffuse[4] = {0.7f, 0.7f, 0.7f, 1.0f};
        GLfloat mat_specular[4] = {1.0f, 1.0f, 1.0f, 1.0f};
        float no_shininess = 0.0f;
        float low_shininess = 5.0f;
        float high_shininess = 100.0f;
        GLfloat mat_emission[4] = {0.3f, 0.2f, 0.2f, 0.0f};

        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, no_mat);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, no_shininess);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, no_mat);

        if (reflectionModel != NULL) {
            renderModel(reflectionModel);
        } else {
            renderModel(primaryModel);
        }



        glCullFace(GL_BACK);
        glDisable(GL_CLIP_PLANE0);
        glPopMatrix();
    }

    // light
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    // draw playfield
    if (drawPlayfield) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        GLfloat no_mat[4] = {0.0f, 0.0f, 0.0f, 0.7f};
        GLfloat mat_ambient[4] = {0.2f, 0.2f, 0.2f, 0.7f};
        GLfloat mat_diffuse[4] = {0.7f, 0.7f, 0.7f, 0.7f};
        float no_shininess = 0.0f;
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, no_mat);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, no_shininess);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, no_mat);

        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glBegin(GL_QUADS);
        glNormal3f( 0.0, 1.0, 0.0);
        glVertex3f( -258.0, 0.0, 534);glTexCoord2i(1,0);
        glVertex3f( 258.0, 0.0, 534);glTexCoord2i(1,1);
        glVertex3f( 258.0, 0.0, -534);glTexCoord2i(0,1);
        glVertex3f( -258.0, 0.0, -534);glTexCoord2i(0,0);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);
        glDisable(GL_STENCIL_TEST);
    }

    // Grid
    if (drawGrid) {
        glDisable(GL_CULL_FACE);
        //glDisable(GL_DEPTH_TEST);

        //glEnable (GL_LINE_SMOOTH);
        //glDepthMask(false);
        //glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);

        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND);
        glBegin(GL_LINES);
        for(int i=-10;i<=10;i++) {
            if (i%10==0) {
                glColor4f(0.7, 0.2, 0.2, 1.0f);
            } else {
                if (i%5==0) {
                    glColor4f(0.2, 0.2, 0.7, 1.0f);
                } else {
                    glColor4f(.4, .4, .4, 1.0f);
                }
            };
            glVertex3f(i*10,0.01,-10*10);
            glVertex3f(i*10,0.01, 10*10);
            glVertex3f(-10*10,0.01,i*10);
            glVertex3f( 10*10,0.01,i*10);
        };
        glEnd();
        glEnable(GL_LIGHTING);
        //glDepthMask(true);
        //glDisable (GL_LINE_SMOOTH);

        //glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        glEnable(GL_CULL_FACE);
    }





    // draw primary model
    if (drawWFModel) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);
    }

    glPushMatrix();
    glTranslatef(0.0, offset, 0.0);
    glRotatef(modelAngle / 16.0, 0.0, 1.0, 0.0);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    GLfloat no_mat[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat mat_ambient[4] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat mat_diffuse[4] = {0.7f, 0.7f, 0.7f, 1.0f};
    GLfloat mat_specular[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    float no_shininess = 0.0f;
    float low_shininess = 5.0f;
    float high_shininess = 100.0f;
    GLfloat mat_emission[4] = {0.3f, 0.2f, 0.2f, 0.0f};

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, no_mat);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, no_shininess);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, no_mat);

    renderModel(primaryModel);    
    glPopMatrix();

    if (drawWFModel) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_CULL_FACE);
    }


    if (drawBall) {
        glPushMatrix();

        glDisable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture[1]);

        glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
        glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
        glEnable(GL_TEXTURE_GEN_S);
        glEnable(GL_TEXTURE_GEN_T);

        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

        glTranslatef(ballPos[0], ballPos[1], ballPos[2]);
        GLUquadric* quad = gluNewQuadric();
        gluQuadricNormals(quad, GLU_SMOOTH);
        gluSphere(quad, 27/2, 50, 50);

        glDisable(GL_TEXTURE_GEN_S);
        glDisable(GL_TEXTURE_GEN_T);

        glDisable(GL_TEXTURE_2D);
        glEnable(GL_LIGHTING);

        glPopMatrix();
    }


    if (drawBoundingBox) {
        glColor4f(1.0f, 1.0f, 1.0f, 0.3f);
        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND);
        glDisable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glPushMatrix();
        glTranslatef(0.0, offset, 0.0);
        glRotatef(modelAngle / 16.0, 0.0, 1.0, 0.0);
        glDrawBB();
        glPopMatrix();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_CULL_FACE);
        glDisable(GL_BLEND);
        glEnable(GL_LIGHTING);
    }


    // draw mask
    if (drawWFMask) {
        glDisable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    glPushMatrix();
    glRotatef(modelAngle / 16.0, 0.0, 1.0, 0.0);
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.1f, 0.45f, 0.1f, 0.3f);
    renderModel(maskModel);
    glEnable(GL_LIGHTING);
    glDisable(GL_BLEND);
    glPopMatrix();
    if (drawWFMask) {
        glEnable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }


    if (drawWFShapes) {
        glDisable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    // draw physics shapes
    if (currentCollisionElement != -1) {

        for (uint32_t i=0; i<collisionElements.size(); i++) {
			ops::fp::FPModelCollisionData * element = &collisionElements[i];

            glDisable(GL_LIGHTING);

            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

            if (element->effectBall == 1) {
                if (i == currentCollisionElement) {
                    glColor4f(0.45f, 0.1f, 0.45f, 0.3f);
                } else {
                    glColor4f(0.3f, 0.1f, 0.3f, 0.3f);
                }
            } else {
                if (i == currentCollisionElement) {
                    glColor4f(0.1f, 0.45f, 0.45f, 0.3f);
                } else {
                    glColor4f(0.1f, 0.3f, 0.3f, 0.3f);
                }
            }


            glPushMatrix();
            glTranslatef(0.0, offset, 0.0);
            glRotatef(modelAngle / 16.0, 0.0, 1.0, 0.0);

            glTranslatef(element->x, element->y, element->z);

            //   2 : Sphere
            //   3 : Flipper
            //   4 : Bumper : not used in FP
            //   5 : Box
            //   6 : Convex hull : not used in FP
            //   7 : Horizontal cylinder
            switch (element->type) {
            case 1 :  {
                // 1 : Vertical Cylinder
                GLUquadric* quad = gluNewQuadric();
                gluQuadricNormals(quad, GLU_SMOOTH);
                glRotatef(90.0, 1.0, 0.0, 0.0);
                glTranslatef(0.0f,0.0f,-element->value2);
                gluCylinder(quad, element->value1, element->value1, element->value2*2, 20, 1);
                glRotatef(180.0, 1.0, 0.0, 0.0);
                gluDisk(quad, 0, element->value1, 20, 1);
                glRotatef(180.0, 1.0, 0.0, 0.0);
                glTranslatef(0.0f,0.0f,2*element->value2);                
                gluDisk(quad, 0, element->value1, 20, 1);

                break;
            }
            case 2 :  {
                // 2 : Sphere
                GLUquadric* quad = gluNewQuadric();
                gluQuadricNormals(quad, GLU_SMOOTH);
                gluSphere(quad, element->value1, 20, 20);

                break;
            }
            case 3 :  {
                // 3 : Flipper
                glDrawFlipper(element->value1, element->value2, element->value3, element->value4);
                break;
            }
            case 5 :  {
                // 5 : Box
                glScalef(element->value1, element->value2, element->value3);
                glDrawCube();
                break;
            }
            case 7 :  {
                // 7 : Horizontal cylinder
                GLUquadric* quad = gluNewQuadric();
                gluQuadricNormals(quad, GLU_SMOOTH);
                glTranslatef(0.0f,0.0f,-element->value2);                
                gluCylinder(quad, element->value1, element->value1, element->value2*2, 20, 1);
                glRotatef(180.0, 1.0, 0.0, 0.0);
                gluDisk(quad, 0, element->value1, 20, 1);
                glRotatef(180.0, 1.0, 0.0, 0.0);
                glTranslatef(0.0f,0.0f,2*element->value2);                
                gluDisk(quad, 0, element->value1, 20, 1);
                break;
            }
            default : {

            }
            }

            glEnable(GL_LIGHTING);            
            glDisable(GL_BLEND);
            glPopMatrix();
        }

    }
    if (drawWFShapes) {
        glEnable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}


void GLWidget::renderModel(msModel * model) {
    if (model != NULL) {

        ms3d_triangle_t *ppTriangle;
        ms3d_vertex_t *ppVertex;

        glBegin(GL_TRIANGLES);
        for (int i = 0; i<model->GetNumTriangles(); i++) {

            ppTriangle = model->GetTriangle(i);

            ppVertex = model->GetVertex(ppTriangle->vertexIndices[0]);
            glNormal3f( ppTriangle->vertexNormals[0][0], ppTriangle->vertexNormals[0][1], ppTriangle->vertexNormals[0][2]);
            glVertex3f( ppVertex->vertex[0], ppVertex->vertex[1], ppVertex->vertex[2]);

            ppVertex = model->GetVertex(ppTriangle->vertexIndices[1]);
            glNormal3f( ppTriangle->vertexNormals[1][0], ppTriangle->vertexNormals[1][1], ppTriangle->vertexNormals[1][2]);
            glVertex3f( ppVertex->vertex[0], ppVertex->vertex[1], ppVertex->vertex[2]);

            ppVertex = model->GetVertex(ppTriangle->vertexIndices[2]);
            glNormal3f( ppTriangle->vertexNormals[2][0], ppTriangle->vertexNormals[2][1], ppTriangle->vertexNormals[2][2]);
            glVertex3f( ppVertex->vertex[0], ppVertex->vertex[1], ppVertex->vertex[2]);
        }
        glEnd();
    }
}


GLvoid GLWidget::glDrawFlipper(float radius1, float radius2, float lenght, float halfHeight)
{    
    glPushMatrix();
    GLUquadric* quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);

    double eq[4] = {0.0, 0.0, 1.0, 0.0};
    glClipPlane(GL_CLIP_PLANE0, eq);
    glEnable(GL_CLIP_PLANE0);

    glRotatef(90.0, 1.0, 0.0, 0.0);
    glTranslatef(0.0f,0.0f,-halfHeight);
    gluCylinder(quad, radius1, radius1, halfHeight*2, 20, 1);
    glRotatef(180.0, 1.0, 0.0, 0.0);
    gluDisk(quad, 0, radius1, 20, 1);
    glRotatef(180.0, 1.0, 0.0, 0.0);
    glTranslatef(0.0f,0.0f,2*halfHeight);
    gluDisk(quad, 0, radius1, 20, 1);

    glDisable(GL_CLIP_PLANE0);

    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f,0.0f,-lenght);
    gluQuadricNormals(quad, GLU_SMOOTH);

    double eq2[4] = {0.0, 0.0, -1.0, 0.0};
    glClipPlane(GL_CLIP_PLANE0, eq2);
    glEnable(GL_CLIP_PLANE0);

    glRotatef(90.0, 1.0, 0.0, 0.0);
    glTranslatef(0.0f,0.0f,-halfHeight);
    gluCylinder(quad, radius2, radius2, halfHeight*2, 20, 1);
    glRotatef(180.0, 1.0, 0.0, 0.0);
    gluDisk(quad, 0, radius2, 20, 1);
    glRotatef(180.0, 1.0, 0.0, 0.0);
    glTranslatef(0.0f,0.0f,2*halfHeight);
    gluDisk(quad, 0, radius2, 20, 1);

    glDisable(GL_CLIP_PLANE0);

    glPopMatrix();


    glBegin(GL_QUADS);
    // up face
    glNormal3f( 0.0f, 1.0f, 0.0f);
    glVertex3f(-radius1, halfHeight,  0.0f);
    glVertex3f(radius1, halfHeight,  0.0f);
    glVertex3f(radius2, halfHeight,  -lenght);
    glVertex3f(-radius2, halfHeight,  -lenght);
    // right face
    glNormal3f( 1.0f, 0.0f, 0.0f);
    glVertex3f(radius1, halfHeight,  0.0f);
    glVertex3f(radius1, -halfHeight,  0.0f);
    glVertex3f(radius2, -halfHeight,  -lenght);
    glVertex3f(radius2, halfHeight,  -lenght);
    // down face
    glNormal3f( 0.0f, -1.0f, 0.0f);
    glVertex3f(radius1, -halfHeight,  0.0f);
    glVertex3f(-radius1, -halfHeight,  0.0f);
    glVertex3f(-radius2, -halfHeight,  -lenght);
    glVertex3f(radius2, -halfHeight,  -lenght);
    // left face
    glNormal3f( -1.0f, 0.0f, 0.0f);
    glVertex3f(-radius1, halfHeight,  0.0f);
    glVertex3f(-radius2, halfHeight,  -lenght);
    glVertex3f(-radius2, -halfHeight,  -lenght);
    glVertex3f(-radius1, -halfHeight,  0.0f);
    glEnd();


}

GLvoid GLWidget::glDrawCube()
{
    glBegin(GL_QUADS);			// Start Drawing Quads
    // Front Face
    glNormal3f( 0.0f, 0.0f, 1.0f);		// Normal Facing Forward
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Top Left Of The Texture and Quad
    // Back Face
    glNormal3f( 0.0f, 0.0f,-1.0f);		// Normal Facing Away
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Bottom Left Of The Texture and Quad
    // Top Face
    glNormal3f( 0.0f, 1.0f, 0.0f);		// Normal Facing Up
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Top Right Of The Texture and Quad
    // Bottom Face
    glNormal3f( 0.0f,-1.0f, 0.0f);		// Normal Facing Down
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Bottom Right Of The Texture and Quad
    // Right face
    glNormal3f( 1.0f, 0.0f, 0.0f);		// Normal Facing Right
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Top Left Of The Texture and Quad
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Bottom Left Of The Texture and Quad
    // Left Face
    glNormal3f(-1.0f, 0.0f, 0.0f);		// Normal Facing Left
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Bottom Left Of The Texture and Quad
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Bottom Right Of The Texture and Quad
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Top Right Of The Texture and Quad
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Top Left Of The Texture and Quad
    glEnd();					// Done Drawing Quads
}


GLvoid GLWidget::glDrawBB()
{
    glBegin(GL_QUADS);
    // Front Face
    glNormal3f( 0.0f, 0.0f, 1.0f);
    glVertex3f(bbMins[0], bbMins[1], bbMaxs[2]);
    glVertex3f(bbMaxs[0], bbMins[1], bbMaxs[2]);
    glVertex3f(bbMaxs[0], bbMaxs[1], bbMaxs[2]);
    glVertex3f(bbMins[0], bbMaxs[1], bbMaxs[2]);
    // Back Face
    glNormal3f( 0.0f, 0.0f,-1.0f);
    glVertex3f(bbMins[0], bbMins[1], bbMins[2]);
    glVertex3f(bbMins[0], bbMaxs[1], bbMins[2]);
    glVertex3f(bbMaxs[0], bbMaxs[1], bbMins[2]);
    glVertex3f(bbMaxs[0], bbMins[1], bbMins[2]);
    // Top Face
    glNormal3f( 0.0f, 1.0f, 0.0f);
    glVertex3f(bbMins[0], bbMaxs[1], bbMins[2]);
    glVertex3f(bbMins[0], bbMaxs[1], bbMaxs[2]);
    glVertex3f(bbMaxs[0], bbMaxs[1], bbMaxs[2]);
    glVertex3f(bbMaxs[0], bbMaxs[1], bbMins[2]);
    // Bottom Face
    glNormal3f( 0.0f,-1.0f, 0.0f);
    glVertex3f(bbMins[0], bbMins[1], bbMins[2]);
    glVertex3f(bbMaxs[0], bbMins[1], bbMins[2]);
    glVertex3f(bbMaxs[0], bbMins[1], bbMaxs[2]);
    glVertex3f(bbMins[0], bbMins[1], bbMaxs[2]);
    // Right face
    glNormal3f( 1.0f, 0.0f, 0.0f);
    glVertex3f(bbMaxs[0], bbMins[1], bbMins[2]);
    glVertex3f(bbMaxs[0], bbMaxs[1], bbMins[2]);
    glVertex3f(bbMaxs[0], bbMaxs[1], bbMaxs[2]);
    glVertex3f(bbMaxs[0], bbMins[1], bbMaxs[2]);
    // Left Face
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(bbMins[0], bbMins[1], bbMins[2]);
    glVertex3f(bbMins[0], bbMins[1], bbMaxs[2]);
    glVertex3f(bbMins[0], bbMaxs[1], bbMaxs[2]);
    glVertex3f(bbMins[0], bbMaxs[1], bbMins[2]);
    glEnd();
}



void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}


void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        // rotate X/Y
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        // model offset / rotation
        //setOffset(offset - ((dy*1.0)/10));
        setAngle(modelAngle + 16 * dx);
    } else if (event->buttons() & Qt::MidButton) {
        // ball move
        ballPos[0] += (dx*1.0)/3;
        ballPos[2] += (dy*1.0)/3;
        updateGL();
    }
    lastPos = event->pos();
}


void GLWidget::wheelEvent(QWheelEvent * e) {    
    setZPosition(zPos + e->delta()/5);
}



float GLWidget::getModelWidth() {
    if (primaryModel != NULL) {
        return (bbMaxs[0] - bbMins[0]);
    } else {
        return 0.0f;
    }
}

float GLWidget::getModelHeight() {
    if (primaryModel != NULL) {
        return (bbMaxs[1] - bbMins[1]);
    } else {
        return 0.0f;
    }

}

float GLWidget::getModelLength() {
    if (primaryModel != NULL) {
        return (bbMaxs[2] - bbMins[2]);
    } else {
        return 0.0f;
    }
}

msModel * GLWidget::getModel() {
    return primaryModel;
}
