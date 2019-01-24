#include "glarea.h"
#include "cylindre.h"
#include <GL/glu.h>
#include <QDebug>
#include <QSurfaceFormat>
#include <math.h>

GLArea::GLArea(QWidget *parent) :
    QOpenGLWidget(parent)
{
    qDebug() << "init GLArea" ;

    // Ce n'est pas indispensable
    QSurfaceFormat sf;
    sf.setDepthBufferSize(24);
    sf.setSamples(16);
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

void GLArea::DrawCylindre(Cylindre *cylindre, GLdouble ep_cyl, GLdouble r_cyl, GLint nb_fac, int r, int v, int b){
    cylindre = new Cylindre(ep_cyl, r_cyl, nb_fac);
    cylindre->setColor(r,v,b);
    cylindre->flag_fill=flag_fill;
    cylindre->dessiner_cylindre(cylindre->color);
}

void GLArea::paintGL()
{
    qDebug() << __FUNCTION__ ;


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt (0, 0, 3.0, 0, 0, 0, 0, 1, 0);
    glRotatef (m_angle, 0, 1, 0);

    // cylindre principal
    glPushMatrix();
    glRotatef (m_alpha, 0, 0, 1);
    DrawCylindre(GroscylindreG, 0.25, 1, 20, 255,0,0);

    glPushMatrix();
    glTranslatef (0, 0, -0.5);
    DrawCylindre(PetitcylindreG, 1.4, 0.08, 10,255,255,255);
    glPopMatrix();
    glPopMatrix();

    glPushMatrix();
    glTranslatef (0.8*cos(m_alpha*M_PI/(180)), 0.8*sin(m_alpha*M_PI/(180)), 0.3);
    glPushMatrix();
    glRotatef (m_alpha, 0, 0, 1);
    DrawCylindre(PetitcylindreH, 1, 0.08, 5,0,255,255);
    glPopMatrix();
    glPushMatrix();
    glTranslatef (0, 0, 0.2);
    DrawCylindre(GroscylindreH, 0.4, 0.3, 20,120,0,255);
    glPopMatrix();
    glPopMatrix();

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
        case Qt::Key_T :
            flag_fill= !flag_fill;
            update();
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
    m_alpha += 2;
    if (m_alpha >= 360) m_alpha = 0;
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




