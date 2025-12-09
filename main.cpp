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

 /**
  * X+ Menuju Kanan
  * X- Menuju Kiri
  * Y+ Menuju Atas
  * Y- Menuju Bawah
  * Z+ Menuju Kedepan (Terlihat lebih besar karena mendekat)
  * Z- Menuju Belakang (Terlihat lebih kecil karena menjauh)
  */

// Include glew sebelum freeglut
#include "include/GL/glew.h"
#include "include/GL/freeglut.h"
// Include stb_image.h untuk keperluan loading image
#include "include/stb_image.h"

void myReshape(int w, int h) {
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)w / (double)h, 1.0, 500.0);
}

void drawCube(float x, float y, float z, double size) {
    glPushMatrix();
        glTranslatef(x, y, z);
        glutSolidCube(size);
    glPopMatrix();
}

void drawCuboid(float x, float y, float z, float width,
                float height, float depth, float angle=0.0f) {
    glPushMatrix();
        glTranslatef(x, y, z);
        glRotatef(angle, 0.0f, 0.0f, 1.0f);
        glScalef(width, height, depth);
        glutSolidCube(1.0);
    glPopMatrix();
}

void drawSteve() {
    // Kepala
    glColor3f(1.0f, 0.0f, 0.0f);
    drawCube(0.0f, 8.0f, 0.0f, 5.0f);
    // Badan
    glColor3f(0.0f, 0.0f, 1.0f);
    drawCuboid(0.0f, 4.0f, 0.0f,
                4.0f, 6.0f, 2.0f);
    // Lengan Kiri
    glColor3f(0.0f, 1.0f, 0.0f);
    drawCuboid(-3.0f, 4.0f, 0.0f,
                2.0f, 5.0f, 2.0f, -10.0f);
    // Lengan Kanan
    glColor3f(0.0f, 1.0f, 0.0f);
    drawCuboid(3.0f, 4.0f, 0.0f,
                2.0f, 5.0f, 2.0f, 10.0f);
    // Kaki Kiri
    glColor3f(0.0f, 0.0f, 0.0f);
    drawCuboid(-1.0f, 0.0f, 0.0f,
                2.0f, 5.0f, 2.0f);
    // Kaki Kanan
    glColor3f(0.0f, 0.0f, 0.0f);
    drawCuboid(1.0f, 0.0f, 0.0f,
                2.0f, 5.0f, 2.0f);
}

void drawTree() {
    // Batang pohon
    glColor3f(0.55f, 0.27f, 0.07f); // Coklat
    drawCuboid(15.0f, 7.5f, 5.0f,
                2.0f, 15.0f, 2.0f);
    // Daun pohon (menggunakan beberapa bola untuk efek)
    glColor3f(0.0f, 0.5f, 0.0f); // Hijau
    glPushMatrix();
        glTranslatef(15.0f, 17.5f, 5.0f);
        glutSolidSphere(5.0f, 20, 20);
        glTranslatef(-3.0f, 2.0f, 5.0f);
        glutSolidSphere(4.0f, 20, 20);
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0.0, 0.0, 50.0,  // Eye position
              0.0, 0.0, 0.0,  // Look at point
              0.0, 5.0, 0.0); // Up vector
    
    // Rendering code would go here
    drawSteve();
    drawTree();

    glutSwapBuffers();
}

void keyInput(unsigned char key, int x, int y) {
    switch (key) {
        case 27: // Tombol 'Esc' untuk keluar
            exit(0);
            break;
        default:
            break;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    // Membuat window dengan ukuran 800x600 bernama "UAS"
    glutInitWindowSize(800, 600);
    glutCreateWindow("Steve Kejatuhan Pokeball dibawah Oak Tree");

    //glewInit();
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5f, 0.8f, 0.92f, 1.0f); // Warna langit cerah

    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyInput);

    glutMainLoop();
    return 0;
}