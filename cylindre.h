#ifndef CYLINDRE_H
#define CYLINDRE_H

#include <GL/glu.h>

class Cylindre
{
public:
    GLdouble ep_cyl; // epaisseur AB
    GLdouble r_cyl; //rayon AC
    GLint nb_fac; // nombre de facettes
    GLint color[3]; // RVB
    boolean flag_fill;

public:
    Cylindre(GLdouble ep_cyl, GLdouble r_cyl, GLint nb_fac);
    void setColor(int r, int v, int b);
    void dessiner_cylindre(GLint colorCylindre[3]);
    void dessiner_rayon();
};

#endif // CYLINDRE_H
