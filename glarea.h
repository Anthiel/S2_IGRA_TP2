#ifndef GLAREA_H
#define GLAREA_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QKeyEvent>
#include <QTimer>
#include "cylindre.h"

class GLArea : public QOpenGLWidget,
               protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GLArea(QWidget *parent = 0);
    ~GLArea();

public slots:
    void setRadius(double radius);

signals:  // On ne les implémente pas, elles seront générées par MOC ;
          // les paramètres seront passés aux slots connectés.
    void radiusChanged(double newRadius);

protected slots:
    void onTimeout();

public:
    void DrawCylindre(Cylindre *cylindre, GLdouble ep_cyl, GLdouble r_cyl, GLint nb_fac, int r, int v, int b);

protected:
    void initializeGL() override;
    void doProjection();
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void keyPressEvent(QKeyEvent *ev) override;
    void keyReleaseEvent(QKeyEvent *ev) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void mouseReleaseEvent(QMouseEvent *ev) override;
    void mouseMoveEvent(QMouseEvent *ev) override;

private:
    double m_angle = 0;
    QTimer *m_timer = nullptr;
    double m_anim = 0;
    double m_alpha = 0;
    double m_radius = 0.5;
    double m_ratio = 1;
    Cylindre *GroscylindreG;
    Cylindre *PetitcylindreG;
};

#endif // GLAREA_H
