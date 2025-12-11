/**
 * Kelompok 1 UAS Komputer Grafik:
 * - 412024018 - Triemas Putra
 * - 412024019 - Richard Devin Sutisna
 * - 412024034 - Jonathan Siswoyo
 * - 412024037 - Alejandro Julian Mac Arthur Simanjuntak
 * - 412024038 - Erivol Patricio Hutadjulu
 * - 412024043 - Andrew Linardi
 */

 /**
  * X+ Menuju Kanan, X- Menuju Kiri
  * Y+ Menuju Atas, Y- Menuju Bawah
  * Z+ Menuju Kedepan (Terlihat lebih besar karena mendekat),
  * Z- Menuju Belakang (Terlihat lebih kecil karena menjauh)
  */

// Include glew sebelum freeglut
#include "include/GL/glew.h"
#include "include/GL/freeglut.h"
// Include stb_image.h untuk keperluan loading image
#include "include/stb_image.h"
#include "include/getBMP.h"
#include <cmath>

float yaw = 0.0f, radius = 80.0f;
unsigned int texture[1];

GLuint steveTexID;

void loadSteveTexture() {
    imageFile* img = getBMP("texture/steve.bmp");

    glGenTextures(1, &steveTexID);
    glBindTexture(GL_TEXTURE_2D, steveTexID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA8,
        img->width,
        img->height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        img->data
    );
    
    delete img;
}

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

struct UV {
    float u1, v1, u2, v2;
};

float U(float x) {
    return x / 64.0f;
}
float V(float y) {
    return 1.0f - (y / 64.0f);
}

UV HEAD_UV[6] = {
    {U(8),  V(16), U(16), V(8)},   // front
    {U(24), V(16), U(32), V(8)},   // back
    {U(0),  V(16), U(8),  V(8)},   // left
    {U(16), V(16), U(24), V(8)},   // right
    {U(8),  V(8),  U(16), V(0)},   // top
    {U(16), V(8),  U(24), V(0)}    // bottom
};

UV BODY_UV[6] = {
    {U(20), V(32), U(28), V(20)}, // front (8×12)
    {U(32), V(32), U(40), V(20)}, // back
    {U(16), V(32), U(20), V(20)}, // left
    {U(28), V(32), U(32), V(20)}, // right
    {U(20), V(20), U(28), V(16)}, // top (8×4)
    {U(28), V(20), U(36), V(16)}  // bottom (8×4)
};

UV ARM_UV[6] = {
    {U(44), V(32), U(48), V(20)}, // front
    {U(52), V(32), U(56), V(20)}, // back
    {U(40), V(32), U(44), V(20)}, // left
    {U(48), V(32), U(52), V(20)}, // right
    {U(44), V(20), U(48), V(16)}, // top
    {U(48), V(20), U(52), V(16)}  // bottom
};

UV LEG_UV[6] = {
    {U(4), V(48), U(8), V(36)},  // front
    {U(12),V(48), U(16),V(36)},  // back
    {U(0), V(48), U(4), V(36)},  // left
    {U(8), V(48), U(12),V(36)},  // right
    {U(4), V(36), U(8), V(32)},  // top
    {U(8), V(36), U(12),V(32)}   // bottom
};

void drawCuboid(float w, float h, float d, UV face[6]) {
    float x = w/2, y = h/2, z = d/2;

    glBegin(GL_QUADS);

    // FRONT
    glTexCoord2f(face[0].u1, face[0].v2); glVertex3f(-x, -y,  z);
    glTexCoord2f(face[0].u2, face[0].v2); glVertex3f( x, -y,  z);
    glTexCoord2f(face[0].u2, face[0].v1); glVertex3f( x,  y,  z);
    glTexCoord2f(face[0].u1, face[0].v1); glVertex3f(-x,  y,  z);

    // BACK
    glTexCoord2f(face[1].u1, face[1].v2); glVertex3f( x, -y, -z);
    glTexCoord2f(face[1].u2, face[1].v2); glVertex3f(-x, -y, -z);
    glTexCoord2f(face[1].u2, face[1].v1); glVertex3f(-x,  y, -z);
    glTexCoord2f(face[1].u1, face[1].v1); glVertex3f( x,  y, -z);

    // LEFT
    glTexCoord2f(face[2].u1, face[2].v2); glVertex3f(-x, -y, -z);
    glTexCoord2f(face[2].u2, face[2].v2); glVertex3f(-x, -y,  z);
    glTexCoord2f(face[2].u2, face[2].v1); glVertex3f(-x,  y,  z);
    glTexCoord2f(face[2].u1, face[2].v1); glVertex3f(-x,  y, -z);

    // RIGHT
    glTexCoord2f(face[3].u1, face[3].v2); glVertex3f( x, -y,  z);
    glTexCoord2f(face[3].u2, face[3].v2); glVertex3f( x, -y, -z);
    glTexCoord2f(face[3].u2, face[3].v1); glVertex3f( x,  y, -z);
    glTexCoord2f(face[3].u1, face[3].v1); glVertex3f( x,  y,  z);

    // TOP
    glTexCoord2f(face[4].u1, face[4].v2); glVertex3f(-x,  y,  z);
    glTexCoord2f(face[4].u2, face[4].v2); glVertex3f( x,  y,  z);
    glTexCoord2f(face[4].u2, face[4].v1); glVertex3f( x,  y, -z);
    glTexCoord2f(face[4].u1, face[4].v1); glVertex3f(-x,  y, -z);

    // BOTTOM
    glTexCoord2f(face[5].u1, face[5].v2); glVertex3f(-x, -y, -z);
    glTexCoord2f(face[5].u2, face[5].v2); glVertex3f( x, -y, -z);
    glTexCoord2f(face[5].u2, face[5].v1); glVertex3f( x, -y,  z);
    glTexCoord2f(face[5].u1, face[5].v1); glVertex3f(-x, -y,  z);

    glEnd();
}

void drawSteve() {
    glBindTexture(GL_TEXTURE_2D, steveTexID);
    glPushMatrix();
    drawCuboid(8, 8, 8, HEAD_UV);          // head
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, -10, 0);
    drawCuboid(8, 12, 4, BODY_UV);         // body
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-6, -10, 0);
    drawCuboid(4, 12, 4, ARM_UV);          // left arm
    glPopMatrix();

    glPushMatrix();
    glTranslatef(6, -10, 0);
    drawCuboid(4, 12, 4, ARM_UV);          // right arm
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-2, -22, 0);
    drawCuboid(4, 12, 4, LEG_UV);          // left leg
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2, -22, 0);
    drawCuboid(4, 12, 4, LEG_UV);          // right leg
    glPopMatrix();
}

void drawTree() {
    // Batang pohon
    glColor3f(0.5f, 0.2f, 0.1f); // Coklat
    // Daun pohon
    glColor3f(0.0f, 0.5f, 0.0f); // Hijau
    drawCube(15.0f, 17.5f, 5.0f, 5.0);
}

void drawPokeball() {
    glPushMatrix();
        glTranslatef(0.0f, 13.0f, 0.0f);
        glutSolidSphere(1.0f, 20, 20);
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float camX = radius * sinf(yaw);
    float camZ = radius * cosf(yaw);

    gluLookAt(camX, 0.0, camZ,  // Eye position
              0.0, 0.0, 0.0, // Look at point
              0.0, 5.0, 0.0); // Up vector

    // Rendering code would go here
    glColor3f(1.0f, 1.0f, 1.0f); // Putih untuk tekstur Steve
    drawSteve();
    drawTree();
    drawPokeball();

    glutSwapBuffers();
}

void keyInput(unsigned char key, int x, int y) {
    switch (key) {
        case 27: // Tombol 'Esc' untuk keluar
            exit(0);
            break;
        case 'w': // Maju mendekati objek
            radius -= 2;
            if (radius < 30.0f) radius = 30.0f;
            glutPostRedisplay();
            break;
        case 's': // Mundur menjauhi objek
            radius += 2;
            if (radius > 100.0f) radius = 100.0f;
            glutPostRedisplay();
            break;
        case 'a': // Geser ke kiri
            yaw -= 0.5f;
            glutPostRedisplay();
            break;
        case 'd': // Geser ke kanan
            yaw += 0.5f;
            glutPostRedisplay();
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

    glewInit();
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.5f, 0.8f, 0.92f, 1.0f); // Warna langit cerah
    loadSteveTexture();
    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyInput);

    glutMainLoop();
    return 0;
}
