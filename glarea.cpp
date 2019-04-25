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
    glFrustum(-wr, wr, -hr, hr, 1.0, 10.0);
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

    double GH=0.8;
    double HJ=2;
    double HI=GH*sin(-m_alpha*M_PI/(180));;
    double xJ=-((-1*GH*cos(-m_alpha*M_PI/(180))) + sqrt(pow(HJ,2)-pow(HI,2)));;
    double JI=(xJ-GH*cos(-m_alpha*M_PI/(180)));
    double beta=atan(HI/JI)*180/M_PI;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt (0, 0, 3.0, 0, 0, 0, 0, 1, 0);
    glRotatef (m_angle, 0, 1, 0);
    glTranslatef (-m_x, -m_y, m_z);

    // cylindre principal et axe
    glPushMatrix();
        glRotatef (-m_alpha, 0, 0, 1);
        DrawCylindre(cylindreG, 0.25, 1, 20, 255,0,0);

        glPushMatrix();
            glTranslatef (0, 0, -0.5);
            DrawCylindre(cylindreAxeG, 1.4, 0.08, 8,255,255,255);
        glPopMatrix();
    glPopMatrix();

    // cylindre rotatif et axe
    glPushMatrix();
        glTranslatef (GH*cos(-m_alpha*M_PI/(180)), GH*sin(-m_alpha*M_PI/(180)), 0.3);
        glPushMatrix();
            glTranslatef (0, 0, -0.05);
            glRotatef (-m_alpha, 0, 0, 1);
            DrawCylindre(cylindreAxeH, 0.8, 0.06, 6,0,255,255);
        glPopMatrix();

        glPushMatrix();
            glRotatef (-beta, 0, 0, 1);
            glTranslatef (0, 0, 0.2);
            DrawCylindre(cylindreH, 0.2, 0.3, 10,120,0,255);
        glPopMatrix();
    glPopMatrix();

    //cylindres exterieur + piece HJ
    glPushMatrix();
        glTranslatef (xJ,0,0.5);
        glPushMatrix();
            glRotatef (-beta, 0, 0, 1);
            DrawCylindre(cylindreJ1, 0.2, 0.3, 10,120,0,255);
        glPopMatrix();
        glPushMatrix();
            glTranslatef (0,0,0.4);
            DrawCylindre(cylindreJ2, 0.2, 0.2, 20,0,0,255);
        glPopMatrix();

        glPushMatrix();
            glTranslatef (0,0,0.16);
            DrawCylindre(cylindreAxeJ, 0.7, 0.04, 6, 255,255,0);
        glPopMatrix();
    glPopMatrix();
    //piece HJ
    glPushMatrix();
        glTranslatef ((xJ+GH*cos(-m_alpha*M_PI/(180)))/2,(GH*sin(-m_alpha*M_PI/(180)))/2,0.5); // xJ+GH*cos(-m_alpha*M_PI/(180)))/2 signe inverser pour GH*cos(-m_alpha*M_PI/(180)))/2 mais ça marche de cette manière seulement
        glRotatef (-beta , 0, 0, 1);
        glRotatef (90, 0, 1, 0);
        glRotatef (45, 0, 0, 1);
        DrawCylindre(cylindreHJ, 2, 0.14, 4, 120,0,255);
    glPopMatrix();

    //le piston et sa base
    glPushMatrix();
        glTranslatef (0,0,0.9);
        glPushMatrix();
            glTranslatef (xJ-1,0,0);
            glRotatef (90, 0, 1, 0);
            glRotatef (45, 0, 0, 1);
            DrawCylindre(cylindrePiston, 2, 0.14, 4, 0,0,255);
        glPopMatrix();
        glPushMatrix();
            glTranslatef (-4.1,0,0);
            glRotatef (90, 0, 1, 0);
            glRotatef (45, 0, 0, 1);
            DrawCylindre(cylindreBasePiston, 2, 0.3, 4, 0,120,255);

        glPopMatrix();
    glPopMatrix();
}

void GLArea::keyPressEvent(QKeyEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->text();

    switch(ev->key()) {
        case Qt::Key_4 :
            m_angle += 1;
            if (m_angle >= 360) m_angle -= 360;
            update();
            break;
        case Qt::Key_6 :
            m_angle -= 1;
            if (m_angle <= -1) m_angle += 360;
            update();
            break;
        case Qt::Key_8 :
            m_y += 0.1;
            update();
            break;
        case Qt::Key_2 :
            m_y -= 0.1;
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
        case Qt::Key_Z :
            m_z+=0.1;
            update();
            break;
        case Qt::Key_Q :
            m_x-=0.1;
            update();
            break;
        case Qt::Key_S :
            m_z-=0.1;
            update();
            break;
        case Qt::Key_D :
            m_x+=0.1;
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




