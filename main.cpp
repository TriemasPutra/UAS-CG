/**
 ============================================================================
  File        : main.cpp
  Judul       : Steve Kejatuhan Pokeball di Bawah Oak Tree
  Deskripsi   :
    Program ini merupakan aplikasi grafika 3D berbasis OpenGL
    (menggunakan FreeGLUT dan GLEW) yang menampilkan sebuah scene
    interaktif berisi karakter Steve (Minecraft), pohon, Pokeball
    dengan animasi, teapot sebagai Earth, serta background luar angkasa.

    Aplikasi ini menggunakan OpenGL fixed-function pipeline dan
    menekankan pada konsep dasar grafika komputer, seperti:
    - Hierarchical modeling menggunakan transformasi bertingkat
    - Texture mapping pada objek 3D
    - Pencahayaan menggunakan beberapa sumber cahaya
    - Animasi sederhana berbasis state machine
    - Kontrol kamera dan interaksi pengguna melalui keyboard
    - Pengecekan error OpenGL dan validasi file tekstur

  Fitur Utama :
    - Karakter Steve dibangun dari beberapa cuboid bertekstur
    - Pohon terdiri dari batang dan lapisan daun
    - Pokeball memiliki animasi jatuh, bergoyang, dan menangkap Steve
    - Beberapa light source yang dapat diaktifkan / dimatikan
    - Kamera orbit yang dapat digerakkan oleh pengguna
    - Background 2D bertekstur menggunakan proyeksi ortografik

  Kontrol :
    W / S        : Zoom kamera mendekat / menjauh
    A / D        : Rotasi kamera ke kiri / kanan
    Panah Atas   : Menambah kecepatan jatuh Pokeball
    Panah Bawah  : Mengurangi kecepatan jatuh Pokeball
    P            : Pause / lanjutkan animasi
    R            : Reset animasi
    1 – 4        : Toggle masing-masing lampu
    0            : Mematikan semua lampu
    9            : Menyalakan semua lampu
    ESC          : Keluar dari program

  Catatan :
    - Program ini tidak menggunakan shader (GLSL), sehingga
      pengecekan error shader tidak diperlukan.
    - Pengecekan error dilakukan melalui glGetError()
      dan validasi pemuatan file tekstur.
 ============================================================================
*/

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

// Include glew sebelum freeglut biar glew nya gak tantrum
#include "include/GL/glew.h"
#include "include/GL/freeglut.h"
#include "include/getBMP.h"
#include <cmath>

/**
 * GLOBAL VARIABLE DEFINITION
 */
float yaw = 0.0f, radius = 80.0f; // Variabel untuk orbital camera
unsigned int texture[37]; // Array untuk menyimpan texture

enum PokeballState {
    FALLING = 0,
    SHAKING = 1,
    CAUGHT = 2
}; // Enum untuk kondisi Pokeball yang ada

PokeballState ballState = FALLING; // Variabel untuk kondisi Pokeball saat ini

float ballY = 25.0f; // Variabel untuk ketinggian Pokeball saat ini
float ballFallSpeed = 0.3f; // Kecepatan jatuh Pokeball

float shakeAngle = 0.0f; // Variabel untuk rotasi Pokeball
int shakeCount = 0; // Variabel untuk jumlah shake

bool steveVisible = true; // Variabel untuk menggambar Steve atau tidak

bool lightEnabled[4] = { true, true, true, true }; // Array untuk kondisi cahaya

bool animationRunning = true; // Variabel untuk menjalankan animasi

/**
 * function checkGLError digunakan untuk debugging function
 * yang mengalami error.
 * @param function yaitu nama function yang akan di cek
 */
void checkGLError(const char* function) {
    GLenum err;
    // Jika terjadi error akan melakukan print nama function yang mengalami
    // error dan errornya ke console
    while ((err = glGetError()) != GL_NO_ERROR) {
        printf("OpenGL ERROR at %s: %d\n", function, err);
    }
}

/**
 * function loadTexture digunakan untuk load texture data dari bmp image
 * function ini bergantung pada function getBMP pada file getBMP.cpp
 * @param filepath yaitu string posisi file pada folder
 */
GLuint loadTexture(std::string filepath) {
    imageFile *image;
    image = getBMP(filepath);

    // Mengecek apakah file berhasil di load
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
    checkGLError("loadTexture");
    return textureID;
}

/**
 * function loadAllTextures untuk mendefinisikan semua texture yang akan digunakan.
 * function ini dipanggil sekali pada setup
 */
void loadAllTextures() {
    // Texture buat Steve
    texture[0] = loadTexture("texture/steve/head-r.bmp");
    texture[1] = loadTexture("texture/steve/head-f.bmp");
    texture[2] = loadTexture("texture/steve/head-l.bmp");
    texture[3] = loadTexture("texture/steve/head-b.bmp");
    texture[4] = loadTexture("texture/steve/head-top.bmp");
    texture[5] = loadTexture("texture/steve/head-bot.bmp");
    texture[6] = loadTexture("texture/steve/torso-r.bmp");
    texture[7] = loadTexture("texture/steve/torso-f.bmp");
    texture[8] = loadTexture("texture/steve/torso-l.bmp");
    texture[9] = loadTexture("texture/steve/torso-b.bmp");
    texture[10] = loadTexture("texture/steve/torso-top.bmp");
    texture[11] = loadTexture("texture/steve/torso-bot.bmp");
    texture[12] = loadTexture("texture/steve/arm-r.bmp");
    texture[13] = loadTexture("texture/steve/arm-f.bmp");
    texture[14] = loadTexture("texture/steve/arm-l.bmp");
    texture[15] = loadTexture("texture/steve/arm-b.bmp");
    texture[16] = loadTexture("texture/steve/arm-top.bmp");
    texture[17] = loadTexture("texture/steve/arm-bot.bmp");
    texture[18] = loadTexture("texture/steve/leg-r.bmp");
    texture[19] = loadTexture("texture/steve/leg-f.bmp");
    texture[20] = loadTexture("texture/steve/leg-l.bmp");
    texture[21] = loadTexture("texture/steve/leg-b.bmp");
    texture[22] = loadTexture("texture/steve/leg-top.bmp");
    texture[23] = loadTexture("texture/steve/leg-bot.bmp");
    // Texture buat Log
    texture[24] = loadTexture("texture/log/log-r.bmp");    
    texture[25] = loadTexture("texture/log/log-f.bmp");
    texture[26] = loadTexture("texture/log/log-l.bmp");
    texture[27] = loadTexture("texture/log/log-b.bmp");
    texture[28] = loadTexture("texture/log/log-top.bmp");
    texture[29] = loadTexture("texture/log/log-bot.bmp");
    // Texture buat Leaf
    texture[30] = loadTexture("texture/leaf/azalea.bmp");
    texture[31] = loadTexture("texture/leaf/berry.bmp");
    texture[32] = loadTexture("texture/leaf/leaf1.bmp");
    texture[33] = loadTexture("texture/leaf/leaf2.bmp");

    // Texture buat Pokeball
    texture[34] = loadTexture("texture/pokeball/pokeball-f.bmp");

    // Texture buat Earth Teapot
    texture[35] = loadTexture("texture/earth.bmp");

    // Texture buat Background Space
    texture[36] = loadTexture("texture/space.bmp");

    checkGLError("loadAllTextures");
}

/**
 * function myReshape untuk mengatur ukuran viewport dan gluPerspective
 * dipanggil oleh glutReshapeFunc setiap kali ada perubahan
 */
void myReshape(int w, int h) {
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)w / (double)h, 1.0, 500.0);
}

/**
 * Menggambar sebuah persegi panjang bertekstur
 * dengan posisi pusat di titik origin.
 * Digunakan sebagai dasar untuk sisi kubus dan background.
 *
 * @param length Panjang persegi
 * @param height Tinggi persegi
 */
void drawRectangle(float length, float height) {
    float x = length / 2;
    float y = height / 2;
    
    glBegin(GL_QUADS);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-x, -y, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f( x, -y, 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f( x,  y, 0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-x,  y, 0.0f);
    glEnd();
}

/**
 * Menggambar sebuah kubus (cuboid) bertekstur dengan
 * enam tekstur berbeda untuk setiap sisi.
 * Fungsi ini menjadi dasar hierarchical modeling.
 *
 * @param texIndex Array indeks tekstur (6 sisi)
 * @param translateX Posisi X
 * @param translateY Posisi Y
 * @param translateZ Posisi Z
 * @param length Panjang kubus
 * @param height Tinggi kubus
 * @param depth Kedalaman kubus
 */
void drawCube(int texIndex[],
                float translateX, float translateY, float translateZ,
                float length, float height, float depth) {
    glPushMatrix();
        glTranslatef(translateX, translateY, translateZ);
        
        // Right face
        glBindTexture(GL_TEXTURE_2D, texture[texIndex[0]]);
        glNormal3f(1.0f, 0.0f, 0.0f);
        glPushMatrix();
            glTranslatef(length / 2, 0.0f, 0.0f);
            glRotatef(90, 0, 1, 0);
            drawRectangle(depth, height);
        glPopMatrix();
        
        // Front face
        glBindTexture(GL_TEXTURE_2D, texture[texIndex[1]]);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glPushMatrix();
            glTranslatef(0.0f, 0.0f, depth / 2);
            drawRectangle(length, height);
        glPopMatrix();
        
        // Left face
        glBindTexture(GL_TEXTURE_2D, texture[texIndex[2]]);
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glPushMatrix();
            glTranslatef(-length / 2, 0.0f, 0.0f);
            glRotatef(-90, 0, 1, 0);
            drawRectangle(depth, height);
        glPopMatrix();
        
        // Back face
        glBindTexture(GL_TEXTURE_2D, texture[texIndex[3]]);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glPushMatrix();
            glTranslatef(0.0f, 0.0f, -depth / 2);
            glRotatef(180, 0, 1, 0);
            drawRectangle(length, height);
        glPopMatrix();
        
        // Top face
        glBindTexture(GL_TEXTURE_2D, texture[texIndex[4]]);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glPushMatrix();
            glTranslatef(0.0f, height / 2, 0.0f);
            glRotatef(-90, 1, 0, 0);
            drawRectangle(length, depth);
        glPopMatrix();
        
        // Bottom face
        glBindTexture(GL_TEXTURE_2D, texture[texIndex[5]]);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glPushMatrix();
            glTranslatef(0.0f, -height / 2, 0.0f);
            glRotatef(90, 1, 0, 0);
            drawRectangle(length, depth);
        glPopMatrix();
    glPopMatrix();
}

/**
 * Menggambar karakter Steve menggunakan beberapa cuboid
 * (kepala, badan, tangan, dan kaki) dengan teknik
 * hierarchical modeling.
 */
void drawSteve() {
    // Kepala Steve
    int headTextureIndices[6] = {0, 1, 2, 3, 4, 5};
    drawCube(
        headTextureIndices,
        0.0f, 8.5f, 0.0f,
        2.5f, 3.0f, 2.5f // Y dari 20.0 sampai 25.0
    );

    // Badan Steve
    int bodyTextureIndices[6] = {6, 7, 8, 9, 10, 11};
    drawCube(
        bodyTextureIndices,
        0.0f, 5.25f, 0.0f, // X dari -2.5 sampai 2.5
        2.5f, 3.5f, 2.5f // Y dari 10.0 sampai 20.0
    );

    // Lengan Kanan Steve
    int armTextureIndices[6] = {12, 13, 14, 15, 16, 17};
    drawCube(
        armTextureIndices,
        1.675f, 5.25f, 0.0f, // X dari 2.5 sampai 7.5
        1.25f, 3.5f, 1.5f // Y dari 10.0 sampai 20.0
    );

    // Lengan Kiri Steve
    drawCube(
        armTextureIndices,
        -1.675f, 5.25f, 0.0f, // X dari -7.5 sampai -2.5
        1.25f, 3.5f, 1.5f // Y dari 10.0 sampai 20.0
    );

    // Kaki Kanan Steve
    int legTextureIndices[6] = {18, 19, 20, 21, 22, 23};
    drawCube(
        legTextureIndices,
        0.675f, 1.75f, 0.0f,
        1.25f, 3.5f, 2.5f // Y dari 10.0 sampai 20.0
    );

    // Kaki Kiri Steve
    drawCube(
        legTextureIndices,
        -0.675f, 1.75f, 0.0f,
        1.25f, 3.5f, 2.5f // Y dari 0.0 sampai 10.0
    );

    checkGLError("drawSteve");
}

/**
 * Menggambar pohon yang terdiri dari batang pohon
 * dan lapisan daun dengan tekstur berbeda.
 */
void drawTree() {
    // Batang pohon dari Y 0.0 sampai 40.0
    int trunkTextureIndices[6] = {24, 25, 26, 27, 28, 29};
    for (int i = 0; i < 5; i++) {    
        drawCube(
            trunkTextureIndices,
            0.0f, 2.5f + i * 5.0f, -10.0f,
            5.0f, 5.0f, 5.0f
        );
    }

    // Daun pohon
    int leafTextureIndices[6] = {32, 33, 32, 33, 32, 33};
    for (int i = 0; i < 6; i++) {
        drawCube(
            leafTextureIndices,
            0.0f, 27.5f + i * 5.0f, -10.0f,
            45.0f - i * 5.0f, 5.0f, 45.0f - i * 5.0f
        );
    }

    // Tambahan daun berry
    glBindTexture(GL_TEXTURE_2D, texture[31]);
    glPushMatrix();
        glTranslatef(5.0f, 30.0f, -20.0f);
        drawRectangle(5.0f, 5.0f);
    glPopMatrix();

    // Tambahan daun azalea
    glBindTexture(GL_TEXTURE_2D, texture[30]);
    glPushMatrix();
        glTranslatef(-2.5f, 27.5f, -17.5f);
        drawRectangle(5.0f, 5.0f);
    glPopMatrix();

    checkGLError("drawTree");
}

/**
 * Membentuk mesh bola bertekstur menggunakan quad strip.
 * Normal dihitung per-vertex untuk mendukung pencahayaan.
 *
 * @param radius Jari-jari bola
 * @param slices Jumlah potongan horizontal
 * @param stacks Jumlah potongan vertikal
 */
void drawBall(double radius, int slices, int stacks) {
    for (int i = 0; i < stacks; ++i) {
        double lat0 = M_PI * (-0.5 + (double)i / stacks);
        double lat1 = M_PI * (-0.5 + (double)(i + 1) / stacks);

        double z0 = sin(lat0);
        double z1 = sin(lat1);
        double zr0 = cos(lat0);
        double zr1 = cos(lat1);

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= slices; ++j) {
            double lng = 2 * M_PI * (double)j / slices;
            double x = cos(lng);
            double y = sin(lng);

            glNormal3f(x * zr0, y * zr0, z0);
            glTexCoord2f((float)j / slices, (float)i / stacks);
            glVertex3f(radius * x * zr0,
                       radius * y * zr0,
                       radius * z0);

            glNormal3f(x * zr1, y * zr1, z1);
            glTexCoord2f((float)j / slices, (float)(i + 1) / stacks);
            glVertex3f(radius * x * zr1,
                       radius * y * zr1,
                       radius * z1);
        }
        glEnd();
    }
}

/**
 * Menggambar bola pada posisi tertentu di dunia 3D.
 * Merupakan pembungkus dari fungsi drawBall.
 *
 * @param radius Jari-jari bola
 * @param x Posisi X
 * @param y Posisi Y
 * @param z Posisi Z
 * @param slices Jumlah slices
 * @param stacks Jumlah stacks
 */
void drawSphere(double radius,
                  float x, float y, float z,
                  int slices, int stacks) {
    glPushMatrix();
        glTranslatef(x, y, z);
        drawBall(radius, slices, stacks);
    glPopMatrix();
}

/**
 * Menggambar Pokeball dan menerapkan transformasi
 * berdasarkan state animasi (FALLING, SHAKING, CAUGHT).
 */
void drawPokeball() {
    glBindTexture(GL_TEXTURE_2D, texture[34]);

    glPushMatrix();
        glTranslatef(0.0f, ballY, 0.0f);

        if (ballState == SHAKING) {
            glRotatef(shakeAngle, 0, 1, 0);
        }
        if (ballState == CAUGHT) {
            glScalef(1.1f, 0.8f, 1.1f);
        }

        drawSphere(3.0f, 0, 0, 0, 36, 36);
    glPopMatrix();
    checkGLError("drawPokeball");
}

/**
 * Menggambar objek Earth menggunakan GLUT solid teapot
 * dengan tekstur bumi.
 */
void drawEarth() {
    glBindTexture(GL_TEXTURE_2D, texture[35]);
    glPushMatrix();
        glTranslatef(0.0f, -8.0f, -10.0f);
        glutSolidTeapot(20.0);
    glPopMatrix();
    checkGLError("drawEarth");
}

/**
 * Menggambar background 2D bertekstur dengan proyeksi ortografik.
 */
void drawSpace() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0, 1, 0, 1);
        glMatrixMode(GL_MODELVIEW);
    
        glPushMatrix();
            glLoadIdentity();
            // Disable depth & lighting
            glDisable(GL_DEPTH_TEST);
            glDisable(GL_LIGHTING);

            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texture[36]);

            glBegin(GL_QUADS);
                glTexCoord2f(0,0); glVertex2f(0,0);
                glTexCoord2f(1,0); glVertex2f(1,0);
                glTexCoord2f(1,1); glVertex2f(1,1);
                glTexCoord2f(0,1); glVertex2f(0,1);
            glEnd();

            // Restore states
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_LIGHTING);
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    checkGLError("drawSpace");
}

/**
 * Mengaktifkan atau menonaktifkan sumber cahaya
 * berdasarkan input pengguna.
 */
void applyLights() {
    if (lightEnabled[0]) glEnable(GL_LIGHT0); else glDisable(GL_LIGHT0);
    if (lightEnabled[1]) glEnable(GL_LIGHT1); else glDisable(GL_LIGHT1);
    if (lightEnabled[2]) glEnable(GL_LIGHT2); else glDisable(GL_LIGHT2);
    if (lightEnabled[3]) glEnable(GL_LIGHT3); else glDisable(GL_LIGHT3);
}

/**
 * Fungsi display utama yang dipanggil oleh glutDisplayFunc
 * pada main. Mengatur kamera, pencahayaan, dan menggambar
 * seluruh objek dalam scene.
 */
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    drawSpace();
    applyLights();

    float camX = radius * sinf(yaw);
    float camZ = radius * cosf(yaw);

    gluLookAt(camX, 0.0, camZ,  // Eye position
              0.0, 0.0, 0.0, // Look at point
              0.0, 1.0, 0.0); // Up vector
    
    GLfloat sunDir[] = { -0.3f, -1.0f, 0.4f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, sunDir);

    GLfloat fillPos[] = { 15.0f, 20.0f, -10.0f, 1.0f };
    glLightfv(GL_LIGHT1, GL_POSITION, fillPos);

    GLfloat rimDir[] = { 0.5f, 0.8f, 0.5f, 0.0f };
    glLightfv(GL_LIGHT2, GL_POSITION, rimDir);

    GLfloat lightPos3[] =  { sinf(yaw), 0.0f, cosf(yaw), 1.0f };
    glLightfv(GL_LIGHT3, GL_POSITION, lightPos3);

    // Mulai gambar objek
    if (steveVisible) {
        drawSteve();
    }
    drawTree();
    drawPokeball();
    drawEarth();

    glutSwapBuffers();
    checkGLError("display");
}

/**
 * Mengatur logika animasi Pokeball berdasarkan kondisi
 * Pokeball (FALLING, SHAKING, CAUGHT).
 */
void updateAnimation() {
    if (ballState == FALLING) {
        ballY -= ballFallSpeed;

        if (ballY <= 13.0f) {   // near Steve’s head
            ballY = 13.0f;
            ballState = SHAKING;
        }
    }
    else if (ballState == SHAKING) {
        shakeAngle += 5.0f;

        if (shakeAngle > 20.0f || shakeAngle < -20.0f) {
            shakeAngle = -shakeAngle;
            shakeCount++;
        }

        if (shakeCount >= 3) {
            ballState = CAUGHT;
            steveVisible = false;
        }
    }
    else if (ballState == CAUGHT) {
        ballY -= ballFallSpeed;

        if (ballY <= 3.0f) {
            ballY = 3.0f;
        }
    }
    checkGLError("updateAnimation");
}

/**
 * Menangani input keyboard standar untuk
 * kamera, pencahayaan, dan animasi.
 */
void keyInput(unsigned char key, int x, int y) {
    switch (key) {
        case 27: // Tombol 'Esc' untuk keluar
            exit(0);
            break;
        case 'W':
        case 'w': // Maju mendekati objek
            radius -= 2;
            if (radius < 70.0f) radius = 70.0f;
            glutPostRedisplay();
            break;
        case 'S':
        case 's': // Mundur menjauhi objek
            radius += 2;
            if (radius > 150.0f) radius = 150.0f;
            glutPostRedisplay();
            break;
        case 'A':
        case 'a': // Geser ke kiri
            yaw -= 0.5f;
            glutPostRedisplay();
            break;
        case 'D':
        case 'd': // Geser ke kanan
            yaw += 0.5f;
            glutPostRedisplay();
            break;
        case '0': // all lights OFF
            for (int i = 0; i < 4; i++) lightEnabled[i] = false;
            break;
        case '9': // all lights ON
            for (int i = 0; i < 4; i++) lightEnabled[i] = true;
            break;
        case '1':
            lightEnabled[0] = !lightEnabled[0];
            break;
        case '2':
            lightEnabled[1] = !lightEnabled[1];
            break;
        case '3':
            lightEnabled[2] = !lightEnabled[2];
            break;
        case '4':
            lightEnabled[3] = !lightEnabled[3];
            break;
        case 'P':
        case 'p': // pause / resume animation
            animationRunning = !animationRunning;
            break;
        case 'R':
        case 'r': // reset animation
            ballState = FALLING;
            ballY = 25.0f;
            shakeAngle = 0.0f;
            shakeCount = 0;
            steveVisible = true;
            animationRunning = true;
            break;
        default:
            break;
    }
}

/**
 * Fungsi idle yang dipanggil terus-menerus oleh glutIdleFunc
 * pada main untuk menjalankan animasi.
 */
void idle() {
    if (animationRunning) {
        updateAnimation();
    }
    glutPostRedisplay();
}

/**
 * Menangani input tombol khusus yaitu
 * arrow key untuk mengatur kecepatan animasi.
 */
void specialInput(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:
            ballFallSpeed += 0.05f;
            if (ballFallSpeed > 2.0f) ballFallSpeed = 2.0f;
            break;

        case GLUT_KEY_DOWN:
            ballFallSpeed -= 0.05f;
            if (ballFallSpeed < 0.05f) ballFallSpeed = 0.05f;
            break;
    }
}

/**
 * Melakukan inisialisasi pencahayaan,
 * termasuk diffuse, ambient, dan specular, serta texture.
 */
void setup() {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    
    glEnable(GL_LIGHT0);
    GLfloat sunDiffuse[]  = { 1.0f, 0.98f, 0.95f, 1.0f };
    GLfloat sunSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat sunAmbient[]  = { 0.2f, 0.2f, 0.2f, 1.0f };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, sunDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, sunSpecular);
    glLightfv(GL_LIGHT0, GL_AMBIENT, sunAmbient);

    glEnable(GL_LIGHT1);
    GLfloat fillDiffuse[]  = { 0.9f, 0.6f, 0.3f, 1.0f };
    GLfloat fillAmbient[] = { 0.1f, 0.05f, 0.02f, 1.0f };
    glLightfv(GL_LIGHT1, GL_DIFFUSE, fillDiffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, fillDiffuse);
    glLightfv(GL_LIGHT1, GL_AMBIENT, fillAmbient);

    glEnable(GL_LIGHT2);
    GLfloat rimDiffuse[] = { 0.3f, 0.5f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT2, GL_DIFFUSE, rimDiffuse);
    glLightfv(GL_LIGHT2, GL_SPECULAR, rimDiffuse);

    glEnable(GL_LIGHT3);
    GLfloat white3[] = { 0.95f, 0.95f, 0.95f, 1.0f };
    glLightfv(GL_LIGHT3, GL_DIFFUSE, white3);
    glLightfv(GL_LIGHT3, GL_SPECULAR, white3);
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    loadAllTextures();

    checkGLError("setup");
}

/**
 * Fungsi utama program.
 * Menginisialisasi GLUT, OpenGL, memuat tekstur,
 * mendaftarkan callback, dan menjalankan main loop.
 */
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    // Membuat window dengan ukuran 800x600 bernama "UAS"
    glutInitWindowSize(800, 600);
    glutCreateWindow("Steve Kejatuhan Pokeball dibawah Oak Tree");

    glewInit();

    setup();

    glutReshapeFunc(myReshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyInput);
    glutSpecialFunc(specialInput);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}
