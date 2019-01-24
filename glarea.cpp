#include "glarea.h"
#include "cylindre.h"
#include <GL/glu.h>
#include <QDebug>
#include <QSurfaceFormat>

GLArea::GLArea(QWidget *parent) :
    QOpenGLWidget(parent)
{
    qDebug() << "init GLArea" ;

    // Ce n'est pas indispensable
    QSurfaceFormat sf;
    sf.setDepthBufferSize(24);
    setFormat(sf);
    qDebug() << "Depth is"<< format().depthBufferSize();

    setEnabled(true);  // événements clavier et souris
    setFocusPolicy(Qt::StrongFocus); // accepte focus
    setFocus();                      // donne le focus

    m_timer = new QTimer(this);
    m_timer->setInterval(50);  // msec
    connect (m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    connect (this, SIGNAL(radiusChanged(double)), this, SLOT(setRadius(double)));
}

GLArea::~GLArea()
{
    qDebug() << "destroy GLArea";

    delete m_timer;

    // Contrairement aux méthodes virtuelles initializeGL, resizeGL et repaintGL,
    // dans le destructeur le contexte GL n'est pas automatiquement rendu courant.
    makeCurrent();

    // ici destructions de ressources GL

    doneCurrent();
}


void GLArea::initializeGL()
{
    qDebug() << __FUNCTION__ ;
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
}

void GLArea::doProjection()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat hr = m_radius, wr = hr * m_ratio;
    glFrustum(-wr, wr, -hr, hr, 1.0, 5.0);
    glMatrixMode(GL_MODELVIEW);
}

void GLArea::resizeGL(int w, int h)
{
    qDebug() << __FUNCTION__ << w << h;

    // C'est fait par défaut
    glViewport(0, 0, w, h);

    m_ratio = (double) w / h;
    doProjection();
}

void GLArea::DrawCylindre(Cylindre *cylindre){
    cylindre = new Cylindre(0.5, 0.7, 20);
    cylindre->setColor(255,0,0);
    cylindre->dessiner_cylindre(cylindre->color);
}

void GLArea::paintGL()
{
    qDebug() << __FUNCTION__ ;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt (0, 0, 3.0, 0, 0, 0, 0, 1, 0);
    glRotatef (m_angle, 1, 0, 0);
    DrawCylindre(cylindre);
    /*
    glRotatef (m_angle, 0, 1.0, 0.1);
    glBegin(GL_TRIANGLES);
    glColor3f (1.0, 0.6, 0.6);
    glVertex3f(-0.7, -0.5, -0.1);
    glVertex3f(0.8, -0.2, -0.1);
    glVertex3f(0.1, 0.9-m_anim, 0.3);
    glColor3f(0.6, 1.0, 0.6);
    glVertex3f(-0.6, 0.7, -0.2);
    glVertex3f(0.8, 0.8, -0.2);
    glVertex3f(0.1, -0.9, 0.7);
    glEnd();
    */
    // on ne met pas glutSwapBuffers(); !
}

void GLArea::keyPressEvent(QKeyEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->text();

    switch(ev->key()) {
        case Qt::Key_Space :
            m_angle += 1;
            if (m_angle >= 360) m_angle -= 360;
            update();
            break;
        case Qt::Key_A :
            if (m_timer->isActive())
                m_timer->stop();
            else m_timer->start();
            break;
        case Qt::Key_R :
            if (ev->text() == "r")
                 setRadius(m_radius-0.05);
            else setRadius(m_radius+0.05);
            break;
    }
}

void GLArea::keyReleaseEvent(QKeyEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->text();
}

void GLArea::mousePressEvent(QMouseEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->x() << ev->y() << ev->button();
}

void GLArea::mouseReleaseEvent(QMouseEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->x() << ev->y() << ev->button();
}

void GLArea::mouseMoveEvent(QMouseEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->x() << ev->y();
}

void GLArea::onTimeout()
{
    qDebug() << __FUNCTION__ ;
    m_anim += 0.01;
    if (m_anim > 1) m_anim = 0;
    update();
}

void GLArea::setRadius(double radius)
{
    qDebug() << __FUNCTION__ << radius << sender();
    if (radius != m_radius && radius > 0.01 && radius <= 10) {
        m_radius = radius;
        qDebug() << "  emit radiusChanged()";
        emit radiusChanged(radius);
        makeCurrent();
        doProjection();
        doneCurrent();
        update();
    }
}




