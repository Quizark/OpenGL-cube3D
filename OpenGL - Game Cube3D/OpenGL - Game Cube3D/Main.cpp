#include <GL/glut.h> // Biblioteka OpenGL
#include <iostream> // Biblioteka do obs³ugi strumieni wejœcia/wyjœcia
#define STB_IMAGE_IMPLEMENTATION
#include "stb-master/stb_image.h"

// Rozmiary okna
const int WIDTH = 1366;
const int HEIGHT = 768;
int ROZDZIAL = 1;
const float STEP = 0.5f;

// Do tekstury
int width, height, channels;

// Wymiary pod³ogi
float FLOOR_SIZE = 10.0f;
float FLOOR_Y = 0.0f;
float FLOOR_X = 10.0f;
float FLOOR_Z = 10.0f;
float FLOOR_HEIGHT = 10.0f;

// Wysokoœæ skoku kostki
const float JUMP_HEIGHT = 5.0f;
// Pozycja kostki na pod³odze
float cubeX = 0.0f;
float cubeY = 0.5f;
float cubeZ = 0.0f;
float startCubeY = cubeY;
float tmpCubeY = startCubeY;
//Pozycja ró¿owej kostki na pod³odze
float cubePinkX = 0.0f;
float cubePinkZ = -5.0f;
float cubePinkY = 0.5f;
//Pozycja drabiny
float ledderX = 0.0f;
float ledderZ = 7.0f;
float ledderY = 0.5f;

//Ruchome przeszkody
float obj1X = -FLOOR_X;
float obj2X = FLOOR_X;
float obj3X = -FLOOR_X;
float obj4X = FLOOR_X;

// Flagi okreœlaj¹ce kierunek ruchu kostki
bool cubeIsJumping = false;
bool cubeIsFalling = false;

//Flagi okreœlaj¹ce przejœcie poziomów
bool lvl1done = false;
bool lvl2done = false;
bool lvl3done = false;
bool lvl4done = false;
bool lvl5done = false;

void restartGame() {
    ROZDZIAL = 1;
    cubeX = 0.0f;
    cubeY = 0.5f;
    cubeZ = 0.0f;
    cubePinkX = 0.0f;
    cubePinkZ = -5.0f;
    cubePinkY = 0.5f;
    lvl1done = false;
    lvl2done = false;
    lvl3done = false;
    lvl4done = false;
    lvl5done = false;
}

void lavaCreate(float x, float z, float xStep, float zStep) {

    // £adowanie nowej tekstury dla œciany
    unsigned char* lavaData = stbi_load("Tekstury\\lava-small.jpg", &width, &height, &channels, 0);

    // Generowanie nowej tekstury dla œciany
    unsigned int lavaTextureId;
    glGenTextures(1, &lavaTextureId);

    // Ustawienie nowych parametrów tekstury dla œciany
    glBindTexture(GL_TEXTURE_2D, lavaTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, lavaData);

    // Zwalnianie pamiêci
    stbi_image_free(lavaData);

    glBindTexture(GL_TEXTURE_2D, lavaTextureId);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x, 0.01f, z);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x + xStep, 0.01f, z); // Zmiana z 10.0f na 1.0f
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x + xStep, 0.01f, z + zStep); // Zmiana z 10.0f na 1.0f
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x, 0.01f, z + zStep); // Zmiana z 10.0f na 1.0f

    glEnd();
}

void doorCreate(float x, float z, int side) {

    // £adowanie nowej tekstury dla œciany
    unsigned char* doorData = stbi_load("Tekstury\\door-small.jpg", &width, &height, &channels, 0);

    // Generowanie nowej tekstury dla œciany
    unsigned int doorTextureId;
    glGenTextures(1, &doorTextureId);

    // Ustawienie nowych parametrów tekstury dla œciany
    glBindTexture(GL_TEXTURE_2D, doorTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, doorData);

    // Zwalnianie pamiêci
    stbi_image_free(doorData);
    glBindTexture(GL_TEXTURE_2D, doorTextureId);
    glBegin(GL_QUADS);
    if (side == 1) {
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.5f, 0.0f, FLOOR_Z - 0.01f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(1.5f, 0.0f, FLOOR_Z - 0.01f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(1.5f, 10.0f, FLOOR_Z - 0.01f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.5f, 10.0f, FLOOR_Z - 0.01f);
    }
    else {
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.5f, 0.0f, -FLOOR_Z + 0.01f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(1.5f, 0.0f, -FLOOR_Z + 0.01f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(1.5f, 10.0f, -FLOOR_Z + 0.01f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.5f, 10.0f, -FLOOR_Z + 0.01f);
    }
    glEnd();
}
//Kolizja ze œcianami wersja 1
bool checkCollision(float cubeX, float cubeY, float cubeZ, int sciana) {

    if (sciana == 1 && cubeZ <= -FLOOR_Z) //zielona
        return true;
    else if (sciana == 2 && cubeZ >= FLOOR_Z)  //ró¿owa
        return true;
    else if (sciana == 3 && cubeX <= -FLOOR_X)  //¿ó³ta
        return true;
    else if (sciana == 4 && cubeX >= FLOOR_X)  //czerwona
        return true;
    else
        return false;
}
bool collisionCube(float cubeX, float cubeZ, float cubePinkX, float cubePinkZ, int kierunek) { //kierunek: w=1 s=2 a=3 d=4
    if (kierunek == 1 && cubePinkZ == cubeZ - STEP && cubeX - STEP == cubePinkX //cubePinkZ <= -FLOOR_SIZE &&
        || cubePinkZ == cubeZ - STEP && cubeX + STEP == cubePinkX
        || cubePinkZ == cubeZ - STEP && cubeX == cubePinkX)
        return true;
    else if (kierunek == 2 && cubePinkZ == cubeZ + STEP && cubeX - STEP == cubePinkX //cubePinkZ >= FLOOR_SIZE &&
        || cubePinkZ == cubeZ + STEP && cubeX + STEP == cubePinkX
        || cubePinkZ == cubeZ + STEP && cubeX == cubePinkX)
        return true;
    else if (kierunek == 3 && cubePinkX == cubeX - STEP && cubeZ - STEP == cubePinkZ //cubePinkX <= -FLOOR_SIZE &&
        || cubePinkX == cubeX - STEP && cubeZ + STEP == cubePinkZ
        || cubePinkX == cubeX - STEP && cubeZ == cubePinkZ)
        return true;
    else if (kierunek == 4 && cubePinkX == cubeX + STEP && cubeZ - STEP == cubePinkZ //cubePinkX >= FLOOR_SIZE &&
        || cubePinkX == cubeX + STEP && cubeZ + STEP == cubePinkZ
        || cubePinkX == cubeX + STEP && cubeZ == cubePinkZ)
        return true;
    else
        return false;
}

bool checkCollisionWithObj(float cubeX, float cubeZ, float startObjX, float startObjZ, float widthX, float widthZ) {
    if (cubeX + STEP > startObjX &&
        cubeX - STEP < startObjX + widthX &&
        cubeZ + STEP > startObjZ &&
        cubeZ - STEP < startObjZ + widthZ) {
        return true;
    }
    else
        return false;
}

//Czy 'e' jest klikniête
bool clickedE = false;
//Miejsce drabiny
bool miejsceDrabiny(float cubeX, float cubeY, float cubeZ) {
    if (cubeX == ledderX && cubeZ == 0.0f )
        return true;
    else
        return false;
}
// pocz¹tkowe pozycje myszy
int mouseX = 0;
int mouseY = 0;
// obroty kamery
float cameraRotationX = 0.0f;
float cameraRotationY = 0.0f;

GLfloat cameraPosition[] = { 0.0f, 2.0f, 6.0f };

//Funkcje rysuj¹ce obiekty

void drawCircle(float x, float y,float high, float radius)
{
    glColor3f(1.0f, 0.0f, 1.0f);
    //glTranslatef(5.0f, 0.0f, 5.0f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < 360; i++)
    {
        float angle = i * 3.14159265f / 180.0f;
        float cx = x + radius * cosf(angle);
        float cy = y + radius * sinf(angle);
        glVertex3f(cx, high + 0.1f, cy);
    }
    glEnd();
}


void drawLadder(float x, float z)
{
    // £adowanie nowej tekstury dla œciany
    unsigned char* lavaData = stbi_load("Tekstury\\ladder.jpg", &width, &height, &channels, 0);

    // Generowanie nowej tekstury dla œciany
    unsigned int lavaTextureId;
    glGenTextures(1, &lavaTextureId);

    // Ustawienie nowych parametrów tekstury dla œciany
    glBindTexture(GL_TEXTURE_2D, lavaTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, lavaData);

    // Zwalnianie pamiêci
    stbi_image_free(lavaData);

    glBindTexture(GL_TEXTURE_2D, lavaTextureId);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.f); glVertex3f(FLOOR_X - 0.001f, 0.0f, -1.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(FLOOR_X - 0.001f, 0.0f, 1.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(FLOOR_X - 0.001f, 10.0f, 1.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(FLOOR_X - 0.001f, 10.0f, -1.5f);

    glEnd();

}

// Funkcja rysuj¹ca scenê
void renderScene() {
    // Aktualizacja pozycji kostki
    if (cubeIsJumping)
    {
        cubeY += 0.1f;
        if (cubeY >=tmpCubeY + JUMP_HEIGHT + FLOOR_Y)
        {
            cubeIsJumping = false;
            cubeIsFalling = true;
        }
        glutPostRedisplay(); // Odœwie¿enie ekranu po skoku
    }
    else if (cubeIsFalling)
    {
        cubeY -= 0.1f;
        if (cubeY <= startCubeY)
        {
            cubeY = FLOOR_Y + 0.5f;
            cubeIsFalling = false;
        }
        glutPostRedisplay(); // Odœwie¿enie ekranu po opadaniu
        tmpCubeY = cubeY;
    }

    //Spadanie kostki
    /*if (miejsceDrabiny(cubeX, cubeY, cubeZ) && (!cubeIsFalling || !cubeIsJumping)) {
        cubeIsFalling = true;
    }*/

    // Ustawienie rzutowania perspektywicznego
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

    // Ustawienie kamery
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraPosition[0], cameraPosition[1], cameraPosition[2],
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f);
    glRotatef(cameraRotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(cameraRotationY, 0.0f, 1.0f, 0.0f);

    if (ROZDZIAL == 1) {

        FLOOR_X = 10.0f;
        FLOOR_Z = 10.0f;

        // £adowanie nowej tekstury
        unsigned char* floorData = stbi_load("Tekstury\\floor-small.jpg", &width, &height, &channels, 0);

        // Generowanie nowej tekstury
        unsigned int floorTextureID;
        glGenTextures(1, &floorTextureID);

        // £adowanie nowej tekstury dla œciany
        unsigned char* wallData = stbi_load("Tekstury\\wall-small.jpg", &width, &height, &channels, 0);

        // Generowanie nowej tekstury dla œciany
        unsigned int wallTextureID;
        glGenTextures(1, &wallTextureID);

        // Ustawienie nowych parametrów tekstury
        glBindTexture(GL_TEXTURE_2D, floorTextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, floorData);

        // Ustawienie nowych parametrów tekstury dla œciany
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, wallData);

        // Zwalnianie pamiêci
        stbi_image_free(wallData);

        // Zwalnianie pamiêci
        stbi_image_free(floorData);
        glEnable(GL_TEXTURE_2D);

        // Wyczyszczenie bufora koloru i g³êbokoœci
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Rysowanie pod³ogi
        glBindTexture(GL_TEXTURE_2D, floorTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-FLOOR_SIZE, 0.0f, FLOOR_SIZE);
        glTexCoord2f(0.0f, 10.0f); glVertex3f(-FLOOR_SIZE, 0.0f, -FLOOR_SIZE);
        glTexCoord2f(10.0f, 10.0f); glVertex3f(FLOOR_SIZE, 0.0f, -FLOOR_SIZE);
        glTexCoord2f(10.0f, 0.0f); glVertex3f(FLOOR_SIZE, 0.0f, FLOOR_SIZE);

        glEnd();

        // ¯ó³ta lewa œciana
        glEnable(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-FLOOR_SIZE, 0.0f, -FLOOR_SIZE);
        glTexCoord2f(0.0f, 5.0f); glVertex3f(-FLOOR_SIZE, 0.0f, FLOOR_SIZE);
        glTexCoord2f(5.0f, 5.0f); glVertex3f(-FLOOR_SIZE, FLOOR_HEIGHT, FLOOR_SIZE);
        glTexCoord2f(5.0f, 0.0f); glVertex3f(-FLOOR_SIZE, FLOOR_HEIGHT, -FLOOR_SIZE);

        // Czerwona prawa œciana
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(FLOOR_SIZE, 0.0f, -FLOOR_SIZE);
        glTexCoord2f(0.0f, 5.0f); glVertex3f(FLOOR_SIZE, 0.0f, FLOOR_SIZE);
        glTexCoord2f(5.0f, 5.0f); glVertex3f(FLOOR_SIZE, FLOOR_HEIGHT, FLOOR_SIZE);
        glTexCoord2f(5.0f, 0.0f); glVertex3f(FLOOR_SIZE, FLOOR_HEIGHT, -FLOOR_SIZE);
        // Zielona œciana na wprost
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-FLOOR_SIZE, 0.0f, -FLOOR_SIZE);
        glTexCoord2f(0.0f, 5.0f);  glVertex3f(FLOOR_SIZE, 0.0f, -FLOOR_SIZE);
        glTexCoord2f(5.0f, 5.0f); glVertex3f(FLOOR_SIZE, FLOOR_HEIGHT, -FLOOR_SIZE);
        glTexCoord2f(5.0f, 0.0f); glVertex3f(-FLOOR_SIZE, FLOOR_HEIGHT, -FLOOR_SIZE);

        // Ró¿owa œciana z ty³u
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-FLOOR_SIZE, 0.0f, FLOOR_SIZE);
        glTexCoord2f(0.0f, 5.0f);  glVertex3f(-FLOOR_SIZE, FLOOR_HEIGHT, FLOOR_SIZE);
        glTexCoord2f(5.0f, 5.0f); glVertex3f(FLOOR_SIZE, FLOOR_HEIGHT, FLOOR_SIZE);
        glTexCoord2f(5.0f, 0.0f); glVertex3f(FLOOR_SIZE, 0.0f, FLOOR_SIZE);
        glEnd();

        // Rysowanie ró¿owej kostki
        glPushMatrix();
        glColor3f(1.0f, 0.5f, 0.5f);
        glTranslatef(cubePinkX, cubePinkY, cubePinkZ);
        glutSolidCube(1.0f);
        glPopMatrix();

        if (lvl1done) {

            glBegin(GL_QUADS);
            //Zielone drzwi po przejœciu poziomu
            glColor3f(0.0f, 1.0f, 0.0f);
            glVertex3f(-1.5f, 0.0f, -FLOOR_Z + 0.01f);
            glVertex3f(1.5f, 0.0f, -FLOOR_Z + 0.01f);
            glVertex3f(1.5f, 10.0f, -FLOOR_Z + 0.01f);
            glVertex3f(-1.5f, 10.0f, -FLOOR_Z + 0.01f);
            glEnd();


            if (cubeX < 1.5f && cubeX > -1.5f && cubeZ == -9.5f) {
                ROZDZIAL++;
                cubeX = 0.0f;
                cubeZ = 9.0f;
                cubeY = 0.5f;
                cubePinkX = 10000.0f;
                cubePinkY = 10000.0f;
                cubePinkZ = 10000.0f;
            }
        }
        else {
            drawCircle(-5.0f, -5.0f,FLOOR_Y, 0.5f);
            glPopMatrix;
            drawCircle(5.0f, 5.0f,FLOOR_Y, 0.5f);
            glPopMatrix;
        }

        if (cubePinkX == -5.0f && cubePinkZ == -5.0f && cubeX == 5.0f && cubeZ == 5.0f) {
            lvl1done = true;
        }
        glColor3f(1.0f, 1.0f, 1.0f);
    }

    else if (ROZDZIAL == 2) {

        cubePinkX = 10000.0f;
        cubePinkY = 10000.0f;
        cubePinkZ = 10000.0f;
        FLOOR_X = 5.0f;
        FLOOR_Z = 10.0f;

        // Wyczyszczenie bufora koloru i g³êbokoœci
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // £adowanie nowej tekstury dla œciany
        unsigned char* wallData = stbi_load("Tekstury\\bricks-small.jpg", &width, &height, &channels, 0);

        // Generowanie nowej tekstury dla œciany
        unsigned int wallTextureID;
        glGenTextures(1, &wallTextureID);

        // Ustawienie nowych parametrów tekstury dla œciany
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, wallData);

        // Zwalnianie pamiêci
        stbi_image_free(wallData);

        // ¯ó³ta lewa œciana
        glEnable(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-FLOOR_X, 0.0f, -FLOOR_Z);
        glTexCoord2f(0.0f, 5.0f); glVertex3f(-FLOOR_X, 0.0f, FLOOR_Z);
        glTexCoord2f(5.0f, 5.0f); glVertex3f(-FLOOR_X, FLOOR_HEIGHT, FLOOR_Z);
        glTexCoord2f(5.0f, 0.0f); glVertex3f(-FLOOR_X, FLOOR_HEIGHT, -FLOOR_Z);

        // Czerwona prawa œciana
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(FLOOR_X, 0.0f, -FLOOR_Z);
        glTexCoord2f(0.0f, 5.0f); glVertex3f(FLOOR_X, 0.0f, FLOOR_Z);
        glTexCoord2f(5.0f, 5.0f); glVertex3f(FLOOR_X, FLOOR_HEIGHT, FLOOR_Z);
        glTexCoord2f(5.0f, 0.0f); glVertex3f(FLOOR_X, FLOOR_HEIGHT, -FLOOR_Z);
        // Zielona œciana na wprost
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-FLOOR_X, 0.0f, -FLOOR_Z);
        glTexCoord2f(0.0f, 5.0f);  glVertex3f(FLOOR_X, 0.0f, -FLOOR_Z);
        glTexCoord2f(5.0f, 5.0f); glVertex3f(FLOOR_X, FLOOR_HEIGHT, -FLOOR_Z);
        glTexCoord2f(5.0f, 0.0f); glVertex3f(-FLOOR_X, FLOOR_HEIGHT, -FLOOR_Z);

        // Ró¿owa œciana z ty³u
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-FLOOR_X, 0.0f, FLOOR_Z);
        glTexCoord2f(0.0f, 5.0f);  glVertex3f(-FLOOR_X, FLOOR_HEIGHT, FLOOR_Z);
        glTexCoord2f(5.0f, 5.0f); glVertex3f(FLOOR_X, FLOOR_HEIGHT, FLOOR_Z);
        glTexCoord2f(5.0f, 0.0f); glVertex3f(FLOOR_X, 0.0f, FLOOR_Z);
        glEnd();

        // Rysowanie pod³ogi
        glBegin(GL_QUADS);
        // Czarne i bia³e pola
        for (float x = -FLOOR_X; x < FLOOR_X; x += 1.0f) {
            for (float z = -FLOOR_Z; z < FLOOR_Z; z += 1.0f) {
                if ((int)x % 2 == 0) {
                    if ((int)z % 2 == 0) {
                        glColor3f(1.0f, 1.0f, 1.0f);
                    }
                    else {
                        glColor3f(0.0f, 0.0f, 0.0f);
                    }
                }
                else {
                    if ((int)z % 2 == 0) {
                        glColor3f(0.0f, 0.0f, 0.0f);
                    }
                    else {
                        glColor3f(1.0f, 1.0f, 1.0f);
                    }
                }
                glVertex3f(x, 0.0f, z);
                glVertex3f(x, 0.0f, z + 1.0f);
                glVertex3f(x + 1.0f, 0.0f, z + 1.0f);
                glVertex3f(x + 1.0f, 0.0f, z);
            }
        }

        glEnd();


        glBegin(GL_QUADS);

        //Zielone drzwi do poprzedniego poziomu
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-1.5f, 0.0f, FLOOR_Z - 0.01f);
        glVertex3f(1.5f, 0.0f, FLOOR_Z - 0.01f);
        glVertex3f(1.5f, 10.0f, FLOOR_Z - 0.01f);
        glVertex3f(-1.5f, 10.0f, FLOOR_Z - 0.01f);

        //Pomarañczowe drzwi do nastepnego poziomu
        glColor3f(1.0f, 0.5f, 0.0f);
        glVertex3f(-1.5f, 0.0f, -FLOOR_Z + 0.01f);
        glVertex3f(1.5f, 0.0f, -FLOOR_Z + 0.01f);
        glVertex3f(1.5f, 10.0f, -FLOOR_Z + 0.01f);
        glVertex3f(-1.5f, 10.0f, -FLOOR_Z + 0.01f);
        glEnd();

        //Czerwone pod³ogi - kucie docelowo lawa
        glBegin(GL_QUADS);

        glColor3f(1.0f, 1.0f, 1.0f);

        lavaCreate(-5.0f, 2.0f, 8.0f, 4.0f);
        lavaCreate(-5.0f, -1.0f, 4.0f, 2.0f);
        lavaCreate(1.0f, -1.0f, 4.0f, 2.0f);
        lavaCreate(-5.0f, 6.0f, 2.0f, 4.0f);
        lavaCreate(3.0f, -5.0f, 2.0f, 3.0f);
        lavaCreate(-5.0f, -5.0f, 7.0f, 3.0f);
        lavaCreate(-3.0f, -8.0f, 8.0f, 2.0f);

        glEnd();

        //Powrót do poprzedniego poziomu
        if (cubeX > -1.5f && cubeX < 1.5f && cubeZ == FLOOR_Z - STEP) {
            ROZDZIAL--;
            cubeX = 0.0f;
            cubeZ = -9.0f;
        }
        //Przejœcie do nastêpnego poziomu
        else if (cubeX > -1.5f && cubeX < 1.5f && cubeZ == -FLOOR_Z + STEP) {
            ROZDZIAL++;
            cubeX = 0.0f;
            cubeZ = 9.0f;
        }
        //Sprawdzenie czy kolizji z law¹
        else if (checkCollisionWithObj(cubeX, cubeZ, -5.0f, 2.0f, 8.0f, 4.0f)
            || checkCollisionWithObj(cubeX, cubeZ, -5.0f, -1.0f, 4.0f, 2.0f)
            || checkCollisionWithObj(cubeX, cubeZ, 1.0f, -1.0f, 4.0f, 2.0f)
            || checkCollisionWithObj(cubeX, cubeZ, -5.0f, 6.0f, 2.0f, 4.0f)
            || checkCollisionWithObj(cubeX, cubeZ, 3.0f, -5.0f, 2.0f, 3.0f)
            || checkCollisionWithObj(cubeX, cubeZ, -5.0f, -5.0f, 7.0f, 3.0f)
            || checkCollisionWithObj(cubeX, cubeZ, -3.0f, -8.0f, 8.0f, 2.0f)
            ) {
            restartGame();
        }
    }

    else if (ROZDZIAL == 3) {

        FLOOR_X = 5.0f;
        FLOOR_Z = 10.0f;
        cubePinkX = 10000.0f;
        cubePinkY = 10000.0f;
        cubePinkZ = 10000.0f;

        // £adowanie nowej tekstury
        unsigned char* floorData = stbi_load("Tekstury\\stairs-small.jpg", &width, &height, &channels, 0);

        // Generowanie nowej tekstury
        unsigned int floorTextureID;
        glGenTextures(1, &floorTextureID);


        // Wyczyszczenie bufora koloru i g³êbokoœci
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // £adowanie nowej tekstury dla œciany
        unsigned char* wallData = stbi_load("Tekstury\\bricks2-small.jpg", &width, &height, &channels, 0);

        // Generowanie nowej tekstury dla œciany
        unsigned int wallTextureID;
        glGenTextures(1, &wallTextureID);

        // Ustawienie nowych parametrów tekstury
        glBindTexture(GL_TEXTURE_2D, floorTextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, floorData);

        // Ustawienie nowych parametrów tekstury dla œciany
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, wallData);

        // Zwalnianie pamiêci
        stbi_image_free(wallData);

        // Zwalnianie pamiêci
        stbi_image_free(floorData);
        glEnable(GL_TEXTURE_2D);

        // Wyczyszczenie bufora koloru i g³êbokoœci
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Rysowanie pod³ogi
        glBindTexture(GL_TEXTURE_2D, floorTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-FLOOR_X, 0.0f, FLOOR_Z);
        glTexCoord2f(0.0f, 10.0f); glVertex3f(-FLOOR_X, FLOOR_HEIGHT - 0.5f, -FLOOR_Z);
        glTexCoord2f(10.0f, 10.0f); glVertex3f(FLOOR_X, FLOOR_HEIGHT - 0.5f, -FLOOR_Z);
        glTexCoord2f(10.0f, 0.0f); glVertex3f(FLOOR_X, 0.0f, FLOOR_Z);

        glEnd();
        // ¯ó³ta lewa œciana
        glEnable(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-FLOOR_X, FLOOR_HEIGHT - 0.5f, -FLOOR_Z);
        glTexCoord2f(0.0f, 5.0f); glVertex3f(-FLOOR_X, 0.0f, FLOOR_Z);
        glTexCoord2f(5.0f, 5.0f); glVertex3f(-FLOOR_X, FLOOR_HEIGHT, FLOOR_Z);
        glTexCoord2f(5.0f, 0.0f); glVertex3f(-FLOOR_X, FLOOR_HEIGHT * 2 - 0.5f, -FLOOR_Z);

        // Czerwona prawa œciana
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(FLOOR_X, FLOOR_HEIGHT - 0.5f, -FLOOR_Z);
        glTexCoord2f(0.0f, 5.0f); glVertex3f(FLOOR_X, 0.0f, FLOOR_Z);
        glTexCoord2f(5.0f, 5.0f); glVertex3f(FLOOR_X, FLOOR_HEIGHT, FLOOR_Z);
        glTexCoord2f(5.0f, 0.0f); glVertex3f(FLOOR_X, FLOOR_HEIGHT * 2 - 0.5f, -FLOOR_Z);
        // Zielona œciana na wprost
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-FLOOR_X, FLOOR_HEIGHT - 0.5f, -FLOOR_Z);
        glTexCoord2f(0.0f, 5.0f);  glVertex3f(FLOOR_X, FLOOR_HEIGHT - 0.5f, -FLOOR_Z);
        glTexCoord2f(5.0f, 5.0f); glVertex3f(FLOOR_X, FLOOR_HEIGHT * 2 - 0.5f, -FLOOR_Z);
        glTexCoord2f(5.0f, 0.0f); glVertex3f(-FLOOR_X, FLOOR_HEIGHT * 2 - 0.5f, -FLOOR_Z);

        // Ró¿owa œciana z ty³u
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-FLOOR_X, 0.0f, FLOOR_Z);
        glTexCoord2f(0.0f, 5.0f);  glVertex3f(-FLOOR_X, FLOOR_HEIGHT, FLOOR_Z);
        glTexCoord2f(5.0f, 5.0f); glVertex3f(FLOOR_X, FLOOR_HEIGHT, FLOOR_Z);
        glTexCoord2f(5.0f, 0.0f); glVertex3f(FLOOR_X, 0.0f, FLOOR_Z);
        glEnd();

        glEnd();

        glBegin(GL_QUADS);
        //Pomarañczowe drzwi do poprzedniego poziomu
        glColor3f(1.0f, 0.5f, 0.0f);
        glVertex3f(-1.5f, 0.0f, FLOOR_Z - 0.01f);
        glVertex3f(1.5f, 0.0f, FLOOR_Z - 0.01f);
        glVertex3f(1.5f, 10.0f, FLOOR_Z - 0.01f);
        glVertex3f(-1.5f, 10.0f, FLOOR_Z - 0.01f);

        //¯ó³te drzwi do nastepnego poziomu
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex3f(-1.5f, FLOOR_HEIGHT - 0.5f, -FLOOR_Z + 0.01f);
        glVertex3f(1.5f, FLOOR_HEIGHT - 0.5f, -FLOOR_Z + 0.01f);
        glVertex3f(1.5f, FLOOR_HEIGHT * 2 - 0.5f, -FLOOR_Z + 0.01f);
        glVertex3f(-1.5f, FLOOR_HEIGHT * 2 - 0.5f, -FLOOR_Z + 0.01f);
        glEnd();

        //Powrót do poprzedniego poziomu
        if (cubeX > -1.5f && cubeX < 1.5f && cubeZ == FLOOR_Z - STEP) {
            ROZDZIAL--;
            cubeX = 0.0f;
            cubeZ = -9.0f;
            cubeY = 0.5f;
        }
        //Przejœcie do nastêpnego poziomu
        else if (cubeX > -1.5f && cubeX < 1.5f && cubeZ == -FLOOR_Z + STEP) {
            ROZDZIAL++;
            cubeX = 0.0f;
            cubeZ = 9.0f;
            cubeY = 0.5f;
        }

    }
    else if (ROZDZIAL == 4) {

        FLOOR_X = 5.0f;
        FLOOR_Z = 10.0f;
        cubeY = 0.5f;
        cubePinkX = 10000.0f;
        cubePinkY = 10000.0f;
        cubePinkZ = 10000.0f;

        // Wyczyszczenie bufora koloru i g³êbokoœci
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // £adowanie nowej tekstury dla œciany
        unsigned char* wallData = stbi_load("Tekstury\\container-small.jpg", &width, &height, &channels, 0);

        // Generowanie nowej tekstury dla œciany
        unsigned int wallTextureID;
        glGenTextures(1, &wallTextureID);

        // Ustawienie nowych parametrów tekstury dla œciany
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, wallData);

        // Zwalnianie pamiêci
        stbi_image_free(wallData);

        // ¯ó³ta lewa œciana
        glEnable(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-FLOOR_X, 0.0f, -FLOOR_Z);
        glTexCoord2f(0.0f, 5.0f); glVertex3f(-FLOOR_X, 0.0f, FLOOR_Z);
        glTexCoord2f(5.0f, 5.0f); glVertex3f(-FLOOR_X, FLOOR_HEIGHT, FLOOR_Z);
        glTexCoord2f(5.0f, 0.0f); glVertex3f(-FLOOR_X, FLOOR_HEIGHT, -FLOOR_Z);

        // Czerwona prawa œciana
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(FLOOR_X, 0.0f, -FLOOR_Z);
        glTexCoord2f(0.0f, 5.0f); glVertex3f(FLOOR_X, 0.0f, FLOOR_Z);
        glTexCoord2f(5.0f, 5.0f); glVertex3f(FLOOR_X, FLOOR_HEIGHT, FLOOR_Z);
        glTexCoord2f(5.0f, 0.0f); glVertex3f(FLOOR_X, FLOOR_HEIGHT, -FLOOR_Z);
        // Zielona œciana na wprost
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-FLOOR_X, 0.0f, -FLOOR_Z);
        glTexCoord2f(0.0f, 5.0f);  glVertex3f(FLOOR_X, 0.0f, -FLOOR_Z);
        glTexCoord2f(5.0f, 5.0f); glVertex3f(FLOOR_X, FLOOR_HEIGHT, -FLOOR_Z);
        glTexCoord2f(5.0f, 0.0f); glVertex3f(-FLOOR_X, FLOOR_HEIGHT, -FLOOR_Z);

        // Ró¿owa œciana z ty³u
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-FLOOR_X, 0.0f, FLOOR_Z);
        glTexCoord2f(0.0f, 5.0f);  glVertex3f(-FLOOR_X, FLOOR_HEIGHT, FLOOR_Z);
        glTexCoord2f(5.0f, 5.0f); glVertex3f(FLOOR_X, FLOOR_HEIGHT, FLOOR_Z);
        glTexCoord2f(5.0f, 0.0f); glVertex3f(FLOOR_X, 0.0f, FLOOR_Z);
        glEnd();

        // Rysowanie pod³ogi
        glBegin(GL_QUADS);
        // Czarne i bia³e pola
        for (float x = -FLOOR_X; x < FLOOR_X; x += 1.0f) {
            for (float z = -FLOOR_Z; z < FLOOR_Z; z += 1.0f) {
                if ((int)x % 2 == 0) {
                    if ((int)z % 2 == 0) {
                        glColor3f(1.0f, 1.0f, 1.0f);
                    }
                    else {
                        glColor3f(0.0f, 0.0f, 0.0f);
                    }
                }
                else {
                    if ((int)z % 2 == 0) {
                        glColor3f(0.0f, 0.0f, 0.0f);
                    }
                    else {
                        glColor3f(1.0f, 1.0f, 1.0f);
                    }
                }
                glVertex3f(x, 0.0f, z);
                glVertex3f(x, 0.0f, z + 1.0f);
                glVertex3f(x + 1.0f, 0.0f, z + 1.0f);
                glVertex3f(x + 1.0f, 0.0f, z);
            }
        }

        glEnd();

        glBegin(GL_QUADS);
        //¯ó³te drzwi do poprzedniego poziomu
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex3f(-1.5f, 0.0f, FLOOR_Z - 0.01f);
        glVertex3f(1.5f, 0.0f, FLOOR_Z - 0.01f);
        glVertex3f(1.5f, 10.0f, FLOOR_Z - 0.01f);
        glVertex3f(-1.5f, 10.0f, FLOOR_Z - 0.01f);

        //Czerwone drzwi do nastepnego poziomu
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-1.5f, 0.0f, -FLOOR_Z + 0.01f);
        glVertex3f(1.5f, 0.0f, -FLOOR_Z + 0.01f);
        glVertex3f(1.5f, 10.0f, -FLOOR_Z + 0.01f);
        glVertex3f(-1.5f, 10.0f, -FLOOR_Z + 0.01f);
        glEnd();

        // Ustalamy prêdkoœæ poruszania
        float movementSpeed1 = 0.01f;
        float movementSpeed2 = 0.05f;
        float movementSpeed3 = 0.075f;
        float movementSpeed4 = 0.1f;

        //Czerwone pod³ogi - kucie docelowo lawa
        glBegin(GL_QUADS);

        glColor3f(1.0f, 0.0f, 0.0f);

        lavaCreate(obj1X, 6.0f, 5.0f, 0.5f);
        lavaCreate(obj2X, 3.0f, 4.0f, 1.0f);
        lavaCreate(obj3X, -3.0f, 3.0f, 1.5f);
        lavaCreate(obj4X, -7.5f, 2.0f, 2.5f);

        glEnd();

        // Poruszanie siê obiektu                
        // Aktualizujemy pozycjê kostki
        obj1X += movementSpeed1;
        obj2X += -movementSpeed2;
        obj3X += movementSpeed3;
        obj4X += -movementSpeed4;


        // Sprawdzamy czy kostka dotar³a do lewej œciany
        if (obj1X >= FLOOR_X + 5.0f) {
            obj1X = -FLOOR_X - 5.0f;
        }
        if (obj2X <= -FLOOR_X - 5.0f) {
            obj2X = FLOOR_X + 5.0f;
        }
        if (obj3X >= FLOOR_X + 5.0f) {
            obj3X = -FLOOR_X - 5.0f;
        }
        if (obj4X <= -FLOOR_X - 5.0f) {
            obj4X = FLOOR_X + 5.0f;
        }


        if (checkCollisionWithObj(cubeX, cubeZ, obj1X, 6.0f, 5.0f, 0.5f) ||
            checkCollisionWithObj(cubeX, cubeZ, obj2X, 3.0f, 4.0f, 1.0f) ||
            checkCollisionWithObj(cubeX, cubeZ, obj3X, -3.0f, 3.0f, 1.5f) ||
            checkCollisionWithObj(cubeX, cubeZ, obj4X, -7.5f, 2.0f, 2.5f)
            ) {
            restartGame();
        }

        //Powrót do poprzedniego poziomu
        if (cubeX > -1.5f && cubeX < 1.5f && cubeZ == FLOOR_Z - STEP) {
            ROZDZIAL--;
            cubeX = 0.0f;
            cubeZ = -9.0f;
            cubeY = 10.0f;
        }
        //Przejœcie do nastêpnego poziomu
        else if (cubeX > -1.5f && cubeX < 1.5f && cubeZ == -FLOOR_Z + STEP) {
            ROZDZIAL++;
            cubeX = 0.0f;
            cubeZ = 4.0f;
        }
    }
    else if (ROZDZIAL == 5) {

        FLOOR_X = 15.0f;
        FLOOR_Z = 5.0f;
        cubePinkX = 10000.0f;
        cubePinkY = 10000.0f;
        cubePinkZ = 10000.0f;

        // Wyczyszczenie bufora koloru i g³êbokoœci
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // £adowanie nowej tekstury dla œciany
        unsigned char* wallData = stbi_load("Tekstury\\wall-small.jpg", &width, &height, &channels, 0);

        // Generowanie nowej tekstury dla œciany
        unsigned int wallTextureID;
        glGenTextures(1, &wallTextureID);

        // Ustawienie nowych parametrów tekstury dla œciany
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, wallData);

        // Zwalnianie pamiêci
        stbi_image_free(wallData);

        // ¯ó³ta lewa œciana
        glEnable(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-FLOOR_X, 0.0f, -FLOOR_Z);
        glTexCoord2f(0.0f, 5.0f); glVertex3f(-FLOOR_X, 0.0f, FLOOR_Z);
        glTexCoord2f(5.0f, 5.0f); glVertex3f(-FLOOR_X, FLOOR_HEIGHT, FLOOR_Z);
        glTexCoord2f(5.0f, 0.0f); glVertex3f(-FLOOR_X, FLOOR_HEIGHT, -FLOOR_Z);

        // Czerwona prawa œciana
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(FLOOR_X, 0.0f, -FLOOR_Z);
        glTexCoord2f(0.0f, 5.0f); glVertex3f(FLOOR_X, 0.0f, FLOOR_Z);
        glTexCoord2f(5.0f, 5.0f); glVertex3f(FLOOR_X, FLOOR_HEIGHT, FLOOR_Z);
        glTexCoord2f(5.0f, 0.0f); glVertex3f(FLOOR_X, FLOOR_HEIGHT, -FLOOR_Z);
        // Zielona œciana na wprost
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-FLOOR_X, 0.0f, -FLOOR_Z);
        glTexCoord2f(0.0f, 5.0f);  glVertex3f(FLOOR_X, 0.0f, -FLOOR_Z);
        glTexCoord2f(5.0f, 5.0f); glVertex3f(FLOOR_X, FLOOR_HEIGHT, -FLOOR_Z);
        glTexCoord2f(5.0f, 0.0f); glVertex3f(-FLOOR_X, FLOOR_HEIGHT, -FLOOR_Z);

        // Ró¿owa œciana z ty³u
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-FLOOR_X, 0.0f, FLOOR_Z);
        glTexCoord2f(0.0f, 5.0f);  glVertex3f(-FLOOR_X, FLOOR_HEIGHT, FLOOR_Z);
        glTexCoord2f(5.0f, 5.0f); glVertex3f(FLOOR_X, FLOOR_HEIGHT, FLOOR_Z);
        glTexCoord2f(5.0f, 0.0f); glVertex3f(FLOOR_X, 0.0f, FLOOR_Z);
        glEnd();

        // Rysowanie pod³ogi
        glBegin(GL_QUADS);
        // Czarne i bia³e pola
        for (float x = -FLOOR_X; x < FLOOR_X; x += 1.0f) {
            for (float z = -FLOOR_Z; z < FLOOR_Z; z += 1.0f) {
                if ((int)x % 2 == 0) {
                    if ((int)z % 2 == 0) {
                        glColor3f(1.0f, 1.0f, 1.0f);
                    }
                    else {
                        glColor3f(0.0f, 0.0f, 0.0f);
                    }
                }
                else {
                    if ((int)z % 2 == 0) {
                        glColor3f(0.0f, 0.0f, 0.0f);
                    }
                    else {
                        glColor3f(1.0f, 1.0f, 1.0f);
                    }
                }
                glVertex3f(x, 0.0f, z);
                glVertex3f(x, 0.0f, z + 1.0f);
                glVertex3f(x + 1.0f, 0.0f, z + 1.0f);
                glVertex3f(x + 1.0f, 0.0f, z);
            }
        }

        glEnd();

        glBegin(GL_QUADS);
        //Czerwone drzwi do poprzedniego poziomu
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-1.5f, 0.0f, FLOOR_Z - 0.01f);
        glVertex3f(1.5f, 0.0f, FLOOR_Z - 0.01f);
        glVertex3f(1.5f, 10.0f, FLOOR_Z - 0.01f);
        glVertex3f(-1.5f, 10.0f, FLOOR_Z - 0.01f);

        if (lvl5done) {
            //Szare drzwi do nastepnego poziomu
            glColor3f(0.5f, 0.5f, 0.5f);
            glVertex3f(-FLOOR_X + 0.01f, 0.0f, -1.5f);
            glVertex3f(-FLOOR_X + 0.01f, 0.0f, 1.5f);
            glVertex3f(-FLOOR_X + 0.01f, 10.0f, 1.5f);
            glVertex3f(-FLOOR_X + 0.01f, 10.0f, -1.5f);
            glColor3f(1.0f, 1.0f, 1.0f);

            if (cubeZ > -1.5f && cubeZ < 1.5f && cubeX == -FLOOR_X + STEP) {
                ROZDZIAL++;
                cubeX = 0.0f;
                cubeZ = 9.0f;
                restartGame(); //- usun¹æ
            }
        }
        else {

         
            drawCircle(13.5f, 0.0f, 8.0f, 0.5f);
            glPopMatrix;
            drawCircle(13.5f, 0.0f, 8.0f, 0.5f);
            glPopMatrix;
            // NAPRAWIÆ!!!!!!!!!






























        }
        glColor3f(1.0f, 1.0f, 1.0f);
        
        ledderX = FLOOR_X - STEP;
        ledderZ = FLOOR_Z;
        drawLadder(ledderX, ledderZ);
        glEnd();
        //Powrót do poprzedniego poziomu
        if (cubeX > -1.5f && cubeX < 1.5f && cubeZ == FLOOR_Z - STEP) {
            ROZDZIAL--;
            cubeX = 0.0f;
            cubeZ = -9.0f;
        }
        //Ukoñczenie poziomu
        else if (cubeX == 13.5f && cubeZ == 0.0f && cubeY >= 8.0f) {
            lvl5done = true;
        }
    }
    // Rysowanie kostki
    glPushMatrix();
    glColor3f(0.0f, 0.0f, 1.0f);
    glTranslatef(cubeX, cubeY, cubeZ);
    glutSolidCube(0.99f);
    glPopMatrix();

    glColor3f(1.0f, 1.0f, 1.0f);

    glutPostRedisplay(); // Odœwie¿enie ekranu po dotarciu na odpowiednie koordy

    // Wymiana buforów
    glutSwapBuffers();
}
// Obs³uga klawiszy
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'w':
        cubeZ -= STEP;
        if (ROZDZIAL == 3) {
            cubeY += STEP / 2;
        }
        if (checkCollision(cubeX, cubeY, cubeZ, 1)) {
            cubeZ = -FLOOR_Z + STEP;
            if (ROZDZIAL == 3) {
                cubeY -= STEP / 2;
            }
        }
        else if (ROZDZIAL == 1 && checkCollision(cubePinkX, cubePinkY, cubePinkZ - STEP, 1)) {
            cubePinkZ = -FLOOR_Z + STEP;
            if (collisionCube(cubeX, cubeZ, cubePinkX, cubePinkZ, 1))
                cubeZ = cubePinkZ + 2 * STEP;
        }
        //Chwycenie
        else if (clickedE) {
            cubePinkZ -= STEP;
        }
        //Przemieszczenie ró¿owej kostki
        else if (collisionCube(cubeX, cubeZ, cubePinkX, cubePinkZ, 1)) {
            cubePinkZ -= STEP;
        }
        break;
    case 's':

        cubeZ += STEP;
        if (ROZDZIAL == 3) {
            cubeY -= STEP / 2;
        }
        if (checkCollision(cubeX, cubeY, cubeZ, 2)) {
            cubeZ = FLOOR_Z - STEP;
            if (ROZDZIAL == 3) {
                cubeY += STEP / 2;
            }
        }
        else if (ROZDZIAL == 1 && checkCollision(cubePinkX, cubePinkY, cubePinkZ + STEP, 2)) {
            cubePinkZ = FLOOR_Z - STEP;
            if (collisionCube(cubeX, cubeZ, cubePinkX, cubePinkZ, 2))
                cubeZ = cubePinkZ - 2 * STEP;
        }
        else if (clickedE) {
            cubePinkZ += STEP;
        }
        else if (cubePinkZ == cubeZ + STEP && cubeX - STEP == cubePinkX //cubePinkZ >= FLOOR_X &&
            || cubePinkZ == cubeZ + STEP && cubeX + STEP == cubePinkX
            || cubePinkZ == cubeZ + STEP && cubeX == cubePinkX) {
            cubePinkZ += STEP;
        }

        break;
    case 'a':

        cubeX -= STEP;
        if (checkCollision(cubeX, cubeY, cubeZ, 3)) {
            cubeX = -FLOOR_X + STEP;
        }
        else if (ROZDZIAL == 1 && checkCollision(cubePinkX - STEP, cubePinkY, cubePinkZ, 3)) {
            cubePinkX = -FLOOR_X + STEP;
            if (collisionCube(cubeX, cubeZ, cubePinkX, cubePinkZ, 3))
                cubeX = cubePinkX + 2 * STEP;
        }
        else if (clickedE) {
            cubePinkX -= STEP;
        }
        else if (cubePinkX == cubeX - STEP && cubeZ - STEP == cubePinkZ //cubePinkX <= -FLOOR_X &&
            || cubePinkX == cubeX - STEP && cubeZ + STEP == cubePinkZ
            || cubePinkX == cubeX - STEP && cubeZ == cubePinkZ

            ) {
            cubePinkX -= STEP;
        }
        break;
    case 'd':

        cubeX += STEP;
        if (checkCollision(cubeX, cubeY, cubeZ, 4)) {
            cubeX = FLOOR_X - STEP;
        }
        else if (ROZDZIAL == 1 && checkCollision(cubePinkX + STEP, cubePinkY, cubePinkZ, 4)) {
            cubePinkX = FLOOR_X - STEP;
            if (collisionCube(cubeX, cubeZ, cubePinkX, cubePinkZ, 3))
                cubeX = cubePinkX - 2 * STEP;

        }
        else if (clickedE) {
            cubePinkX += STEP;
        }
        else if (cubePinkX == cubeX + STEP && cubeZ - STEP == cubePinkZ //cubePinkX >= FLOOR_SIZE &&
            || cubePinkX == cubeX + STEP && cubeZ + STEP == cubePinkZ
            || cubePinkX == cubeX + STEP && cubeZ == cubePinkZ
            ) {
            cubePinkX += STEP;
        }
        break;
    case 't'://Dzia³anie drabiny
        if (ROZDZIAL == 5 && miejsceDrabiny(cubeX, cubeY, cubeZ) && cubeY < 5.0f) {
            cubeY += 0.1f;
            tmpCubeY = cubeY;
        }
        break;
    case 'r'://Dzia³anie drabiny
        if (ROZDZIAL == 5 && miejsceDrabiny(cubeX, cubeY, cubeZ) && cubeY > 0.5f)
            cubeY -= 0.1f;
        break;
    case 'e'://Ci¹gniêcie kostki - TEST
        if (
            clickedE == true
            )
            clickedE = false;

        else if (
            clickedE == false &&
            (cubeZ == cubePinkZ && cubeX + 2 * STEP == cubePinkX) ||
            (cubeZ == cubePinkZ && cubeX - 2 * STEP == cubePinkX) ||
            (cubeX == cubePinkX && cubeZ + 2 * STEP == cubePinkZ) ||
            (cubeX == cubePinkX && cubeZ - 2 * STEP == cubePinkZ)
            //Sprawdzanie czy niebieska kostka dotyka ró¿owej
            //cubeX == cubePinkX + STEP  ||
            //cubeX == cubePinkX - STEP  ||
            //cubeZ == cubePinkZ + STEP  ||
            //cubeZ == cubePinkZ - STEP  ||
            //cubeZ == cubePinkZ  ||
            //cubeZ == cubePinkZ  ||
            //cubeX == cubePinkX  ||
            //cubeX == cubePinkX  
            )
            clickedE = true;

        break;

    case 32: // Spacja
        if (!cubeIsJumping && !cubeIsFalling)
        {
            cubeIsJumping = true;
        }
        break;
    default:
        break;
    }
    glutPostRedisplay(); // Odœwie¿enie ekranu
}
// Funkcja inicjuj¹ca
void init() {
    // Ustawienie koloru t³a
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // W³¹czenie testu g³êbokoœci
    glEnable(GL_DEPTH_TEST);
    // Ustawienie perspektywy
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
}
// funkcja wywo³ywana podczas poruszania mysz¹
void motion(int x, int y) {
    // obliczamy ró¿nicê pozycji myszy
    int deltaX = x - mouseX;
    int deltaY = y - mouseY;

    // aktualizujemy obroty kamery
    cameraRotationY += deltaX * 0.5f;
    cameraRotationX += deltaY * 0.5f;

    // zapisujemy aktualn¹ pozycjê myszy
    mouseX = x;
    mouseY = y;

    // odœwie¿amy scenê
    glutPostRedisplay();
}
// funkcja wywo³ywana podczas naciœniêcia przycisku myszy
void mouse(int button, int state, int x, int y) {
    // zapisujemy pozycjê myszy
    mouseX = x;
    mouseY = y;
}
void handleScroll(int button, int state, int x, int y) {
    if (button == 3) {
        // przybli¿enie kamery
        cameraPosition[2] -= 1.0;
    }
    else if (button == 4) {
        // oddalenie kamery
        cameraPosition[2] += 1.0;
    }
    // odœwie¿amy scenê
    glutPostRedisplay();
}
int main(int argc, char** argv) {
    // Inicjalizacja biblioteki GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("OpenGL - Game Cube3D");
    // Rejestracja funkcji odpowiedzialnej za rysowanie
    glutDisplayFunc(renderScene);

    // Rejestracja funkcji obs³uguj¹cej klawiaturê
    glutKeyboardFunc(keyboard);

    // Ustawienie perspektywy
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

    // W³¹czenie testu g³êbokoœci
    glEnable(GL_DEPTH_TEST);

    // Ustaw funkcjê wywo³ywan¹ w ka¿dej klatce animacji
    // glutIdleFunc(animate); 

    // ustawiamy funkcje wywo³ywane podczas poruszania mysz¹ i naciœniêcia przycisku myszy
    glutMotionFunc(motion);
    glutMouseFunc(mouse);
    glutMouseFunc(handleScroll);

    // Rozpoczêcie pêtli g³ównej programu
    glutMainLoop();

    return 0;
}