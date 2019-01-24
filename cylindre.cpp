#include "cylindre.h"
#include <GL/glu.h>
#include <QDebug>
#include <math.h>


Cylindre::Cylindre(GLdouble ep_cyl, GLdouble r_cyl, GLint nb_fac)
{
    this->ep_cyl = ep_cyl; // epaisseur AB
    this->r_cyl = r_cyl; //rayon AC
    this->nb_fac = nb_fac; // nombre de facettes
}

void Cylindre::setColor(int r, int v, int b){
    color[0] = r; // rouge
    color[1] = v; // vert
    color[2] = b; // bleu
}

void Cylindre::dessiner_cylindre(GLint colorCylindre[3]){
    float alphaDecal = 360/nb_fac;
        printf("alphaDecal = %f\n", alphaDecal);
        for (int i = 0; i < nb_fac; ++i)
        {
            glPushMatrix();

           /* switch(sens){ // faire tourner les deux pièces avec 'espace'
                case 0 : glRotatef (angleAnima, 0, 0, 1); break;
                default : glRotatef (-angleAnima, 0, 0, 1); break;
            }
            */

            glRotatef (360*i/nb_fac, 0, 0, 1);

            dessiner_rayon();


            glPopMatrix();
        }
}

void Cylindre::dessiner_rayon(){
    GLdouble alpha = 2 * M_PI/nb_fac;
    switch(flag_fill){
        case false : glBegin(GL_LINE_LOOP);break;
        default : glBegin(GL_POLYGON);
    }
    glColor3f (color[0]/255.,	color[1]/255.,	color[2]/255.);
    glVertex3f (0,					0,					ep_cyl/2);
    glVertex3f (r_cyl,				0,					ep_cyl/2);
    glVertex3f (r_cyl*cos(alpha),	r_cyl*sin(alpha),	ep_cyl/2);
    glEnd();

    switch(flag_fill){
        case false : glBegin(GL_LINE_LOOP);break;
        default : glBegin(GL_POLYGON);
    }
    glColor3f (color[0]/255., color[1]/255.,color[2]/255.);
    glVertex3f (0,					0,					-ep_cyl/2);
    glVertex3f (r_cyl,				0,					-ep_cyl/2);
    glVertex3f (r_cyl*cos(alpha),	r_cyl*sin(alpha),	-ep_cyl/2);
    glEnd();
    double mod;
    switch(flag_fill){
        case false :
            glBegin(GL_LINE_LOOP);
            mod=0.6;
        break;
        default : glBegin(GL_QUADS);
            mod=0.8;
    }

    glColor3f ((color[0]/255.)*mod,	(color[1]/255.)*mod,	(color[2]/255.)*mod);
    glVertex3f (r_cyl,				0,					ep_cyl/2);
    glVertex3f (r_cyl*cos(alpha),	r_cyl*sin(alpha),	ep_cyl/2);
    glVertex3f (r_cyl*cos(alpha),	r_cyl*sin(alpha),	-ep_cyl/2);
    glVertex3f (r_cyl,				0,					-ep_cyl/2);
    glEnd();

    if(!flag_fill){
        glBegin(GL_LINE_LOOP);
        glVertex3f (0,					0,					ep_cyl/2);
        glVertex3f (0,					0,					-ep_cyl/2);
        glEnd();
    }

}
