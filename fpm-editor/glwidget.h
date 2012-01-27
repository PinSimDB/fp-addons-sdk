#ifndef GLWIDGET_H
#define GLWIDGET_H


#include <QGLWidget>

#include "ops-base.h"
#include "ops-tools.h"
#include "ops-lzo.h"
#include "ops-ms3d.h"
#include "ops-msole.h"
#include "ops-fp.h"
#include "ops-fp-writer.h"


class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const;
    QSize sizeHint() const;

    float getModelWidth();
    float getModelHeight();
    float getModelLength();

    msModel * getModel();


public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
    void setZPosition(int z);
    void setOffset(float off);
    void setOffset(double off);
    void setAngle(int angle);
    void setDrawWFModel(bool flag);
    void setDrawWFShapes(bool flag);
    void setDrawWFMask(bool flag);
    void setDrawBoundingBox(bool flag);
    void setDrawPlayfield(bool flag);
    void setDrawGrid(bool flag);
    void setDrawBall(bool flag);
    void setPrimaryModel(const char* ms3d, uint32_t len, int type);
    void setMaskModel(const char* ms3d, uint32_t len);
    void setReflectionModel(const char* ms3d, uint32_t len);
    void setCurrentCollisionElement(int current);
	void setCollisionElements(std::vector<ops::fp::FPModelCollisionData>);

signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);
    void zPositionChanged(int z);
    void offsetChanged(float off);
    void offsetChanged(double off);
    void angleChanged(int angle);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void renderModel(msModel * model);

    GLvoid glDrawFlipper(float radius1, float radius2, float lenght, float halfHeight);
    GLvoid glDrawCube();
    GLvoid glDrawBB();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent * e);

private:
    msModel * primaryModel;
    int primaryModelType;
    msModel * maskModel;
    msModel * reflectionModel;

    int32_t currentCollisionElement;
	std::vector<ops::fp::FPModelCollisionData> collisionElements;

    int xRot;
    int yRot;
    int zRot;
    int zPos;

    float offset;
    int modelAngle;

    float bbMins[3];
    float bbMaxs[3];

    float ballPos[3];

    bool drawWFModel;
    bool drawWFShapes;
    bool drawWFMask;
    bool drawBoundingBox;
    bool drawPlayfield;
    bool drawGrid;
    bool drawBall;




    GLuint texture[2];
    //GLuint texture2;

    QPoint lastPos;

};


#endif // GLWIDGET_H
