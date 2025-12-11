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
unsigned int texture[24];

GLuint loadTexture(std::string filepath) {
    imageFile *image;
    image = getBMP(filepath);

    // Check if image loaded successfully
    if (image == nullptr || image->data == nullptr) {
        printf("ERROR: Failed to load texture: %s\n", filepath.c_str());
        return 0;
    }
    
    printf("Loading: %s (Size: %dx%d)\n", filepath.c_str(), image->width, image->height);

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        image->width,
        image->height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        image->data
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Bersihin memori biar gak berat
    delete[] image->data;
    delete image;

    printf("Texture ID %d loaded successfully\n", textureID);
    return textureID;
}

void loadAllTextures() {
    // Texture buat Steve
    texture[0] = loadTexture("texture/steve-bmp/head-r.bmp");
    texture[1] = loadTexture("texture/steve-bmp/head-f.bmp");
    texture[2] = loadTexture("texture/steve-bmp/head-l.bmp");
    texture[3] = loadTexture("texture/steve-bmp/head-b.bmp");
    texture[4] = loadTexture("texture/steve-bmp/head-top.bmp");
    texture[5] = loadTexture("texture/steve-bmp/head-bot.bmp");
    texture[6] = loadTexture("texture/steve-bmp/torso-r.bmp");
    texture[7] = loadTexture("texture/steve-bmp/torso-f.bmp");
    texture[8] = loadTexture("texture/steve-bmp/torso-l.bmp");
    texture[9] = loadTexture("texture/steve-bmp/torso-b.bmp");
    texture[10] = loadTexture("texture/steve-bmp/torso-top.bmp");
    texture[11] = loadTexture("texture/steve-bmp/torso-bot.bmp");
    texture[12] = loadTexture("texture/steve-bmp/arm-r.bmp");
    texture[13] = loadTexture("texture/steve-bmp/arm-f.bmp");
    texture[14] = loadTexture("texture/steve-bmp/arm-l.bmp");
    texture[15] = loadTexture("texture/steve-bmp/arm-b.bmp");
    texture[16] = loadTexture("texture/steve-bmp/arm-top.bmp");
    texture[17] = loadTexture("texture/steve-bmp/arm-bot.bmp");
    texture[18] = loadTexture("texture/steve-bmp/leg-r.bmp");
    texture[19] = loadTexture("texture/steve-bmp/leg-f.bmp");
    texture[20] = loadTexture("texture/steve-bmp/leg-l.bmp");
    texture[21] = loadTexture("texture/steve-bmp/leg-b.bmp");
    texture[22] = loadTexture("texture/steve-bmp/leg-top.bmp");
    texture[23] = loadTexture("texture/steve-bmp/leg-bot.bmp");


}

void myReshape(int w, int h) {
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)w / (double)h, 1.0, 500.0);
}

void drawRectangle(float length, float height, float depth) {
    float x = length / 2;
    float y = height / 2;
    float z = depth / 2;
    
    glBegin(GL_QUADS);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-x, -y, z);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( x, -y, z);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( x,  y, z);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-x,  y, z);
    glEnd();
}

void drawCube(int texIndex[],
                float translateX, float translateY, float translateZ,
                float length, float height, float depth) {
    // Right face
    glBindTexture(GL_TEXTURE_2D, texture[texIndex[0]]);
    glPushMatrix();
        glTranslatef(translateX + length / 2, translateY, translateZ);
        glRotatef(90, 0, 1, 0);
        drawRectangle(depth, height, 0);
    glPopMatrix();
    
    // Front face
    glBindTexture(GL_TEXTURE_2D, texture[texIndex[1]]);
    glPushMatrix();
        glTranslatef(translateX, translateY, translateZ + depth / 2);
        drawRectangle(length, height, 0);
    glPopMatrix();

    // Left face
    glBindTexture(GL_TEXTURE_2D, texture[texIndex[2]]);
    glPushMatrix();
        glTranslatef(translateX - length / 2, translateY, translateZ);
        glRotatef(-90, 0, 1, 0);
        drawRectangle(depth, height, 0);
    glPopMatrix();

    // Back face
    glBindTexture(GL_TEXTURE_2D, texture[texIndex[3]]);
    glPushMatrix();
        glTranslatef(translateX, translateY, translateZ - depth / 2);
        glRotatef(180, 0, 1, 0);
        drawRectangle(length, height, 0);
    glPopMatrix();

    // Top face
    glBindTexture(GL_TEXTURE_2D, texture[texIndex[4]]);
    glPushMatrix();
        glTranslatef(translateX, translateY + height / 2, translateZ);
        glRotatef(-90, 1, 0, 0);
        drawRectangle(length, depth, 0);
    glPopMatrix();

    // Bottom face
    glBindTexture(GL_TEXTURE_2D, texture[texIndex[5]]);
    glPushMatrix();
        glTranslatef(translateX, translateY - height / 2, translateZ);
        glRotatef(90, 1, 0, 0);  // Changed from -90 to 90
        drawRectangle(length, depth, 0);
    glPopMatrix();
}

void drawSteve() {
    // Kepala Steve
    int headTextureIndices[6] = {0, 1, 2, 3, 4, 5};
    drawCube(
        headTextureIndices,
        0.0f, 10.0f, 0.0f,  // Changed Y position
        5.0f, 5.0f, 5.0f
    );

    // Badan Steve
    int bodyTextureIndices[6] = {6, 7, 8, 9, 10, 11};
    drawCube(
        bodyTextureIndices,
        0.0f, 0.0f, 0.0f,  // Changed Y position to center
        5.0f, 10.0f, 3.0f  // Adjusted proportions
    );

    // Lengan Kanan Steve
    int armTextureIndices[6] = {12, 13, 14, 15, 16, 17};
    drawCube(
        armTextureIndices,
        4.0f, 2.0f, 0.0f,  // Adjusted position
        2.0f, 10.0f, 2.0f  // Adjusted size
    );

    // Lengan Kiri Steve
    drawCube(
        armTextureIndices,
        -4.0f, 2.0f, 0.0f,  // Adjusted position
        2.0f, 10.0f, 2.0f
    );

    // Kaki Kanan Steve
    int legTextureIndices[6] = {18, 19, 20, 21, 22, 23};
    drawCube(
        legTextureIndices,
        1.25f, -10.0f, 0.0f,  // Adjusted position
        2.0f, 10.0f, 2.0f  // Adjusted size
    );

    // Kaki Kiri Steve
    drawCube(
        legTextureIndices,
        -1.25f, -10.0f, 0.0f,  // Adjusted position
        2.0f, 10.0f, 2.0f
    );
}

void drawTree() {
    // Batang pohon

    // Daun pohon
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

    loadAllTextures();

    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyInput);

    glutMainLoop();
    return 0;
}