/**
 * Kelompok 1 UAS Komputer Grafik:
 * - 412024018 - Triemas Putra
 * - 412024019 - Richard Devin Sutisna
 * - 412024034 - Jonathan Siswoyo
 * - 412024037 - Alejandro Julian Mac Arthur Simanjuntak
 * - 412024038 - Erivol Patricio Hutadjulu
 * - 412024043 - Andrew Linardi
 * 
 * 
 */

// Include glew sebelum freeglut
#include "include/GL/glew.h"
#include "GL/freeglut.h"
// Include stb_image.h untuk keperluan loading image
#include "include/stb_image.h"

void drawSteve() {
    glColor3f(1.0f, 0.0f, 0.0f);
    glutSolidCube(5.0);
    //glTranslatef(0.0f, 0.0f, -5.0f);

}

void display() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    gluLookAt(0.0, 0.0, 5.0,  // Eye position
              0.0, 0.0, 0.0,  // Look at point
              0.0, 5.0, 0.0); // Up vector
    // Rendering code would go here
    drawSteve();

    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("UAS");

    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}