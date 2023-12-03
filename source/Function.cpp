#define STB_IMAGE_IMPLEMENTATION
#include "Header.h"



// Poziom gry
int ROZDZIAL = 1;

// Pojedy�czy krok
const float STEP = 0.5f;

// Do tekstury
int width, height, channels;

// Wymiary pod�ogi
float FLOOR_SIZE = 10.0f;
float FLOOR_Y = 0.0f;
float FLOOR_X = 10.0f;
float FLOOR_Z = 10.0f;
float FLOOR_HEIGHT = 10.0f;

// Wysoko�� skoku kostki
const float JUMP_HEIGHT = 5.0f;

// Pozycja kostki na pod�odze
float cubeX = 0.0f;
float cubeY = 0.5f;
float cubeZ = 0.0f;
float startCubeY = cubeY;
float tmpCubeY = startCubeY;

//Pozycja r�owej kostki na pod�odze
float cubePinkX = 0.0f;
float cubePinkZ = -5.0f;
float cubePinkY = 0.5f;

//Pozycja drabiny
float ledderX = 0.0f;
float ledderZ = 7.0f;
float ledderY = 0.5f;

//Ruchome przeszkody
float obj0X;
float obj1X = -FLOOR_X;
float obj2X = FLOOR_X;
float obj3X = -FLOOR_X;
float obj4X = FLOOR_X;

// Flagi okre�laj�ce kierunek ruchu kostki
bool cubeIsJumping = false;
bool cubeIsFalling = false;

//Flagi okre�laj�ce przej�cie poziom�w
bool lvl1done = false;
bool lvl2done = false;
bool lvl3done = false;
bool lvl4done = false;
bool lvl5done = false;

// pocz�tkowe pozycje myszy
int mouseX = 0;
int mouseY = 0;

// obroty kamery
float cameraRotationX = 0.0f;
float cameraRotationY = 0.0f;

// Pozycje kamery
GLfloat cameraPosition[] = { 0.0f, 24.0f, 9.0f };
GLfloat cameraPositionBackup[] = { 0.0f, 24.0f, 9.0f };
GLfloat cameraPositionStairsLvl[] = { 15.0f, 35.0f, 6.0f };
GLfloat cameraPositionLvl5[] = { 0.0f, 30.0f, 4.5f };

// Obracanie postaci
float rotationAngle = 0.0f;

//Czy 'e' jest klikni�te
bool clickedE = false;

//Kolizja ze �cianami wersja 1
bool checkCollision(float cubeX, float cubeY, float cubeZ, int sciana) {

    if (sciana == 1 && cubeZ <= -FLOOR_Z) //zielona
        return true;
    else if (sciana == 2 && cubeZ >= FLOOR_Z)  //r�owa
        return true;
    else if (sciana == 3 && cubeX <= -FLOOR_X)  //��ta
        return true;
    else if (sciana == 4 && cubeX >= FLOOR_X)  //czerwona
        return true;
    else
        return false;
}

// Kolizja postaci
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

// Kolizja z obiektami
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

// Tworzenie drzwi
void doorCreate(float x, float z, int side) {

    // �adowanie nowej tekstury dla �ciany
    unsigned char* doorData = stbi_load("Tekstury\\door-small.jpg", &width, &height, &channels, 0);

    // Generowanie nowej tekstury dla �ciany
    unsigned int doorTextureId;
    glGenTextures(1, &doorTextureId);

    // Ustawienie nowych parametr�w tekstury dla �ciany
    glBindTexture(GL_TEXTURE_2D, doorTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, doorData);

    // Zwalnianie pami�ci
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

// Tworzenie przeszk�d 
void lavaCreate(float x, float z, float xStep, float zStep) {

    // �adowanie nowej tekstury dla �ciany
    unsigned char* lavaData = stbi_load("Tekstury\\lava-small.jpg", &width, &height, &channels, 0);

    // Generowanie nowej tekstury dla �ciany
    unsigned int lavaTextureId;
    glGenTextures(1, &lavaTextureId);

    // Ustawienie nowych parametr�w tekstury dla �ciany
    glBindTexture(GL_TEXTURE_2D, lavaTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, lavaData);

    // Zwalnianie pami�ci
    stbi_image_free(lavaData);

    glBindTexture(GL_TEXTURE_2D, lavaTextureId);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x, 0.01f, z);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x + xStep, 0.01f, z); // Zmiana z 10.0f na 1.0f
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x + xStep, 0.01f, z + zStep); // Zmiana z 10.0f na 1.0f
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x, 0.01f, z + zStep); // Zmiana z 10.0f na 1.0f

    glEnd();
}

// Restartowanie gry 
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
    cameraPosition[0] = cameraPositionBackup[0];
    cameraPosition[1] = cameraPositionBackup[1];
    cameraPosition[2] = cameraPositionBackup[2];
}

// �adowanie modeli
void loadModel(const char* filename) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "Failed to load model: " << importer.GetErrorString() << std::endl;
        return;
    }

    aiMesh* mesh = scene->mMeshes[0]; // Zak�adamy, �e istnieje tylko jeden mesh w modelu

    // Przygotowanie tablic wierzcho�k�w, normalnych i tekstur
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> texCoords;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        // Wierzcho�ki
        vertices.push_back(mesh->mVertices[i].x * 0.5);
        vertices.push_back(mesh->mVertices[i].y * 0.5);
        vertices.push_back(mesh->mVertices[i].z * 0.5);

        // Normalne
        normals.push_back(mesh->mNormals[i].x);
        normals.push_back(mesh->mNormals[i].y);
        normals.push_back(mesh->mNormals[i].z);

        // Tekstury
        if (mesh->mTextureCoords[0]) { // Sprawd�, czy istniej� wsp�rz�dne tekstur
            texCoords.push_back(mesh->mTextureCoords[0][i].x);
            texCoords.push_back(mesh->mTextureCoords[0][i].y);
        }
        else {
            texCoords.push_back(0.0f);
            texCoords.push_back(0.0f);
        }
    }

    // Przygotowanie tablic indeks�w dla element�w
    std::vector<GLuint> indices;
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // Renderowanie modelu
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, 0, vertices.data());
    glNormalPointer(GL_FLOAT, 0, normals.data());
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords.data());

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

//Miejsce drabiny
bool miejsceDrabiny(float cubeX, float cubeY, float cubeZ) {
    if (cubeX == ledderX && cubeZ == 0.0f)
        return true;
    else
        return false;
}

//Funkcja rysuj�ca okr�g
void drawCircle(float x, float y, float high, float radius) {
    glColor3f(1.0f, 0.0f, 1.0f);

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

//Funkcja rysuj�ca drabin�
void drawLadder(float x, float z)
{
    // �adowanie nowej tekstury dla drabiny
    unsigned char* lavaData = stbi_load("Tekstury\\ladder.jpg", &width, &height, &channels, 0);

    // Generowanie nowej tekstury dla drabiny
    unsigned int lavaTextureId;
    glGenTextures(1, &lavaTextureId);

    // Ustawienie nowych parametr�w tekstury dla drabiny
    glBindTexture(GL_TEXTURE_2D, lavaTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, lavaData);

    // Zwalnianie pami�ci
    stbi_image_free(lavaData);

    glBindTexture(GL_TEXTURE_2D, lavaTextureId);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.f); glVertex3f(FLOOR_X - 0.001f, 0.0f, -1.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(FLOOR_X - 0.001f, 0.0f, 1.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(FLOOR_X - 0.001f, 5.0f, 1.5f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(FLOOR_X - 0.001f, 5.0f, -1.5f);

    glEnd();

}


// Funkcja rysuj�ca scen�
void renderScene() {
    if (!miejsceDrabiny(cubeX, cubeY, cubeZ) && cubeY > 0.5f && ROZDZIAL != 3) {
        cubeIsFalling = true;
    }

    // Aktualizacja pozycji kostki
    if (cubeIsJumping)
    {
        cubeY += 0.1f;
        if (cubeY >= tmpCubeY + JUMP_HEIGHT + FLOOR_Y)
        {
            cubeIsJumping = false;
            cubeIsFalling = true;
        }
        glutPostRedisplay(); // Od�wie�enie ekranu po skoku
    }
    else if (cubeIsFalling)
    {
        cubeY -= 0.1f;
        if (cubeY <= startCubeY)
        {
            cubeY = FLOOR_Y + 0.5f;
            cubeIsFalling = false;
        }
        glutPostRedisplay(); // Od�wie�enie ekranu po opadaniu
        tmpCubeY = cubeY;
    }

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

    // Pierwszy rozdzia� gry
    if (ROZDZIAL == 1) {

        FLOOR_X = 10.0f;
        FLOOR_Z = 10.0f;



        // �adowanie nowej tekstury dla pod�ogi
        unsigned char* floorData = stbi_load("Tekstury\\floor-small.jpg", &width, &height, &channels, 0);

        // Generowanie nowej tekstury dla pod�ogi
        unsigned int floorTextureID;
        glGenTextures(1, &floorTextureID);

        // �adowanie nowej tekstury dla �ciany
        unsigned char* wallData = stbi_load("Tekstury\\wall-small.jpg", &width, &height, &channels, 0);

        // Generowanie nowej tekstury dla �ciany
        unsigned int wallTextureID;
        glGenTextures(1, &wallTextureID);

        // �adowanie nowej tekstury dla kostki
        unsigned char* containerData = stbi_load("Tekstury\\container-small.jpg", &width, &height, &channels, 0);

        // Generowanie nowej tekstury dla kostki
        unsigned int containerTextureID;
        glGenTextures(1, &containerTextureID);

        // Ustawienie nowych parametr�w tekstury dla pod�ogi
        glBindTexture(GL_TEXTURE_2D, floorTextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, floorData);

        // Ustawienie nowych parametr�w tekstury dla �ciany
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, wallData);

        // Ustawienie nowych parametr�w tekstury dla koskti
        glBindTexture(GL_TEXTURE_2D, containerTextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, containerData);

        // Zwalnianie pami�ci
        stbi_image_free(wallData);
        stbi_image_free(floorData);
        stbi_image_free(containerData);

        glEnable(GL_TEXTURE_2D);

        // Wyczyszczenie bufora koloru i g��boko�ci
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Rysowanie pod�ogi
        glBindTexture(GL_TEXTURE_2D, floorTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-FLOOR_SIZE, 0.0f, FLOOR_SIZE);
        glTexCoord2f(0.0f, 10.0f); glVertex3f(-FLOOR_SIZE, 0.0f, -FLOOR_SIZE);
        glTexCoord2f(10.0f, 10.0f); glVertex3f(FLOOR_SIZE, 0.0f, -FLOOR_SIZE);
        glTexCoord2f(10.0f, 0.0f); glVertex3f(FLOOR_SIZE, 0.0f, FLOOR_SIZE);

        glEnd();

        glEnable(GL_TEXTURE_2D);

        // Rysowanie lewej �ciany
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-FLOOR_SIZE, 0.0f, -FLOOR_SIZE);
        glTexCoord2f(0.0f, 5.0f); glVertex3f(-FLOOR_SIZE, 0.0f, FLOOR_SIZE);
        glTexCoord2f(5.0f, 5.0f); glVertex3f(-FLOOR_SIZE, FLOOR_HEIGHT, FLOOR_SIZE);
        glTexCoord2f(5.0f, 0.0f); glVertex3f(-FLOOR_SIZE, FLOOR_HEIGHT, -FLOOR_SIZE);

        // Rysowanie prawej �ciany
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(FLOOR_SIZE, 0.0f, -FLOOR_SIZE);
        glTexCoord2f(0.0f, 5.0f); glVertex3f(FLOOR_SIZE, 0.0f, FLOOR_SIZE);
        glTexCoord2f(5.0f, 5.0f); glVertex3f(FLOOR_SIZE, FLOOR_HEIGHT, FLOOR_SIZE);
        glTexCoord2f(5.0f, 0.0f); glVertex3f(FLOOR_SIZE, FLOOR_HEIGHT, -FLOOR_SIZE);

        // Rysowanie �ciany na wprost
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-FLOOR_SIZE, 0.0f, -FLOOR_SIZE);
        glTexCoord2f(0.0f, 5.0f);  glVertex3f(FLOOR_SIZE, 0.0f, -FLOOR_SIZE);
        glTexCoord2f(5.0f, 5.0f); glVertex3f(FLOOR_SIZE, FLOOR_HEIGHT, -FLOOR_SIZE);
        glTexCoord2f(5.0f, 0.0f); glVertex3f(-FLOOR_SIZE, FLOOR_HEIGHT, -FLOOR_SIZE);

        // Rysowanie �ciany z ty�u
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-FLOOR_SIZE, 0.0f, FLOOR_SIZE);
        glTexCoord2f(0.0f, 5.0f);  glVertex3f(-FLOOR_SIZE, FLOOR_HEIGHT, FLOOR_SIZE);
        glTexCoord2f(5.0f, 5.0f); glVertex3f(FLOOR_SIZE, FLOOR_HEIGHT, FLOOR_SIZE);
        glTexCoord2f(5.0f, 0.0f); glVertex3f(FLOOR_SIZE, 0.0f, FLOOR_SIZE);
        glEnd();

        // Rysowanie modelu kostki
        glBindTexture(GL_TEXTURE_2D, containerTextureID);
        glPushMatrix();
        glTranslatef(cubePinkX, cubePinkY, cubePinkZ);
        loadModel("Modele//cube.obj");
        glPopMatrix();
        glEnd;

        // Pozycja o�wietlenia
        GLfloat lightPosition1[] = { FLOOR_SIZE, FLOOR_HEIGHT / 2, 0.0f, 1.0f }; // Pozycja pierwszego �wiat�a punktowego
        GLfloat lightPosition2[] = { -FLOOR_SIZE, FLOOR_HEIGHT / 2, 0.0f, 1.0f }; // Pozycja drugiego �wiat�a punktowego
        GLfloat lightPosition3[] = { 0.0f, FLOOR_HEIGHT / 2, FLOOR_SIZE, 1.0f }; // Pozycja trzeciego �wiat�a punktowego
        GLfloat lightPosition4[] = { 0.0f, FLOOR_HEIGHT / 2, -FLOOR_SIZE, 1.0f }; // Pozycja czwartego �wiat�a punktowego

        // Atrybuty o�wietlenia
        GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // Sk�adowa dyfuzyjna �wiat�a
        GLfloat lightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f }; // Sk�adowa ambientalna �wiat�a
        GLfloat lightDirection[] = { 0.0f, -1.0f, -1.0f }; // Kierunek �wiat�a (w tym przypadku skierowane w d�)
        GLfloat lightDirection2[] = { 0.0f, 0.0f, 1.0f }; // Kierunek �wiat�a (w tym przypadku skierowane do przodu)


        glEnable(GL_LIGHTING); // W��cz o�wietlenie

        glLightfv(GL_LIGHT1, GL_POSITION, lightPosition1); // Ustaw pozycj� pierwszego �wiat�a punktowego
        glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse); // Ustaw sk�adow� dyfuzyjn� �wiat�a
        glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient); // Ustaw sk�adow� ambientaln� �wiat�a
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, lightDirection); // Ustaw kierunek �wiat�a typu spotlight
        glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0f); // Ustaw k�t rozwarcia sto�ka �wietlnego (w tym przypadku 45 stopni)
        glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0f); // Ustaw wyk�adnik sto�ka �wietlnego

        glEnable(GL_LIGHT1); // W��cz pierwsze �wiat�o

        glLightfv(GL_LIGHT2, GL_POSITION, lightPosition2); // Ustaw pozycj� drugiego �wiat�a punktowego
        glLightfv(GL_LIGHT2, GL_DIFFUSE, lightDiffuse); // Ustaw sk�adow� dyfuzyjn� �wiat�a
        glLightfv(GL_LIGHT2, GL_AMBIENT, lightAmbient); // Ustaw sk�adow� ambientaln� �wiat�a
        glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, lightDirection2); // Ustaw kierunek �wiat�a typu spotlight
        glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 45.0f); // Ustaw k�t rozwarcia sto�ka �wietlnego (w tym przypadku 45 stopni)
        glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 2.0f); // Ustaw wyk�adnik sto�ka �wietlnego

        glEnable(GL_LIGHT2); // W��cz drugie �wiat�o

        glLightfv(GL_LIGHT3, GL_POSITION, lightPosition3); // Ustaw pozycj� trzeciego �wiat�a punktowego
        glLightfv(GL_LIGHT3, GL_DIFFUSE, lightDiffuse); // Ustaw sk�adow� dyfuzyjn� �wiat�a
        glLightfv(GL_LIGHT3, GL_AMBIENT, lightAmbient); // Ustaw sk�adow� ambientaln� �wiat�a
        glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, lightDirection); // Ustaw kierunek �wiat�a typu spotlight
        glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 45.0f); // Ustaw k�t rozwarcia sto�ka �wietlnego (w tym przypadku 45 stopni)
        glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 2.0f); // Ustaw wyk�adnik sto�ka �wietlnego

        glEnable(GL_LIGHT3); // W��cz trzecie �wiat�o

        glLightfv(GL_LIGHT4, GL_POSITION, lightPosition4); // Ustaw pozycj� czwartego �wiat�a punktowego
        glLightfv(GL_LIGHT4, GL_DIFFUSE, lightDiffuse); // Ustaw sk�adow� dyfuzyjn� �wiat�a
        glLightfv(GL_LIGHT4, GL_AMBIENT, lightAmbient); // Ustaw sk�adow� ambientaln� �wiat�a
        glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, lightDirection2); // Ustaw kierunek �wiat�a typu spotlight
        glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 45.0f); // Ustaw k�t rozwarcia sto�ka �wietlnego (w tym przypadku 45 stopni)
        glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 2.0f); // Ustaw wyk�adnik sto�ka �wietlnego

        glEnable(GL_LIGHT4); // W��cz czwarte �wiat�o

        // Uko�czenie pierwszego poziomu
        if (lvl1done) {

            // �adowanie nowej tekstury
            unsigned char* doorData = stbi_load("Tekstury\\doors.jpg", &width, &height, &channels, 0);

            // Generowanie nowej tekstury
            unsigned int doorTextureID;
            glGenTextures(1, &doorTextureID);

            glBindTexture(GL_TEXTURE_2D, doorTextureID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, doorData);

            // Zwolnienie pami�ci
            stbi_image_free(doorData);
            glEnable(GL_TEXTURE_2D);

            // Rysowanie drzwi
            glBindTexture(GL_TEXTURE_2D, doorTextureID);
            glPushMatrix();
            glTranslatef(2.5f, 2.5f, -FLOOR_Z + 0.01f);
            glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
            loadModel("Modele//door.obj");
            glPopMatrix();
            glEnd;

            // Przechodzenie przez drzwi
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
            drawCircle(-5.0f, -5.0f, FLOOR_Y, 0.5f);
            glPopMatrix;
            drawCircle(5.0f, 5.0f, FLOOR_Y, 0.5f);
            glPopMatrix;
        }

        // Zaliczenie poziomu
        if (cubePinkX == -5.0f && cubePinkZ == -5.0f && cubeX == 5.0f && cubeZ == 5.0f) {
            lvl1done = true;
        }
        glColor3f(1.0f, 1.0f, 1.0f);
    }

    // Drugi rozdzia� gry
    else if (ROZDZIAL == 2) {

        cubePinkX = 10000.0f;
        cubePinkY = 10000.0f;
        cubePinkZ = 10000.0f;
        FLOOR_X = 5.0f;
        FLOOR_Z = 10.0f;

        // Pozycja o�wietlenia
        GLfloat lightPosition1[] = { FLOOR_SIZE, FLOOR_HEIGHT / 2, 0.0f, 1.0f }; // Pozycja pierwszego �wiat�a punktowego
        GLfloat lightPosition2[] = { FLOOR_SIZE + 4.0f, FLOOR_HEIGHT / 2, 0.0f, 1.0f }; // Pozycja drugiego �wiat�a punktowego
        GLfloat lightPosition3[] = { 0.0f, FLOOR_HEIGHT / 2, FLOOR_SIZE, 1.0f }; // Pozycja trzeciego �wiat�a punktowego
        GLfloat lightPosition4[] = { 0.0f, FLOOR_HEIGHT / 2, -FLOOR_SIZE, 1.0f }; // Pozycja czwartego �wiat�a punktowego

        // Atrybuty o�wietlenia
        GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // Sk�adowa dyfuzyjna �wiat�a
        GLfloat lightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f }; // Sk�adowa ambientalna �wiat�a
        GLfloat lightDirection[] = { 0.0f, -1.0f, -1.0f }; // Kierunek �wiat�a (w tym przypadku skierowane w d�)
        GLfloat lightDirection2[] = { 0.0f, 0.0f, 1.0f }; // Kierunek �wiat�a (w tym przypadku skierowane do przodu)


        glEnable(GL_LIGHTING); // W��cz o�wietlenie

        glLightfv(GL_LIGHT1, GL_POSITION, lightPosition1); // Ustaw pozycj� pierwszego �wiat�a punktowego
        glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse); // Ustaw sk�adow� dyfuzyjn� �wiat�a
        glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient); // Ustaw sk�adow� ambientaln� �wiat�a
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, lightDirection); // Ustaw kierunek �wiat�a typu spotlight
        glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0f); // Ustaw k�t rozwarcia sto�ka �wietlnego (w tym przypadku 45 stopni)
        glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0f); // Ustaw wyk�adnik sto�ka �wietlnego

        // glEnable(GL_LIGHT1); // W��cz pierwsze �wiat�o

        glLightfv(GL_LIGHT2, GL_POSITION, lightPosition2); // Ustaw pozycj� drugiego �wiat�a punktowego
        glLightfv(GL_LIGHT2, GL_DIFFUSE, lightDiffuse); // Ustaw sk�adow� dyfuzyjn� �wiat�a
        glLightfv(GL_LIGHT2, GL_AMBIENT, lightAmbient); // Ustaw sk�adow� ambientaln� �wiat�a
        glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, lightDirection2); // Ustaw kierunek �wiat�a typu spotlight
        glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 45.0f); // Ustaw k�t rozwarcia sto�ka �wietlnego (w tym przypadku 45 stopni)
        glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 2.0f); // Ustaw wyk�adnik sto�ka �wietlnego

        // glEnable(GL_LIGHT2); // W��cz drugie �wiat�o

        glLightfv(GL_LIGHT3, GL_POSITION, lightPosition3); // Ustaw pozycj� trzeciego �wiat�a punktowego
        glLightfv(GL_LIGHT3, GL_DIFFUSE, lightDiffuse); // Ustaw sk�adow� dyfuzyjn� �wiat�a
        glLightfv(GL_LIGHT3, GL_AMBIENT, lightAmbient); // Ustaw sk�adow� ambientaln� �wiat�a
        glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, lightDirection); // Ustaw kierunek �wiat�a typu spotlight
        glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 45.0f); // Ustaw k�t rozwarcia sto�ka �wietlnego (w tym przypadku 45 stopni)
        glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 2.0f); // Ustaw wyk�adnik sto�ka �wietlnego

        glEnable(GL_LIGHT3); // W��cz trzecie �wiat�o

        glLightfv(GL_LIGHT4, GL_POSITION, lightPosition4); // Ustaw pozycj� czwartego �wiat�a punktowego
        glLightfv(GL_LIGHT4, GL_DIFFUSE, lightDiffuse); // Ustaw sk�adow� dyfuzyjn� �wiat�a
        glLightfv(GL_LIGHT4, GL_AMBIENT, lightAmbient); // Ustaw sk�adow� ambientaln� �wiat�a
        glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, lightDirection2); // Ustaw kierunek �wiat�a typu spotlight
        glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 45.0f); // Ustaw k�t rozwarcia sto�ka �wietlnego (w tym przypadku 45 stopni)
        glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 2.0f); // Ustaw wyk�adnik sto�ka �wietlnego

        // glEnable(GL_LIGHT4); // W��cz czwarte �wiat�o



         // Wyczyszczenie bufora koloru i g��boko�ci
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // �adowanie nowej tekstury dla �ciany
        unsigned char* wallData = stbi_load("Tekstury\\bricks-small.jpg", &width, &height, &channels, 0);

        // Generowanie nowej tekstury dla �ciany
        unsigned int wallTextureID;
        glGenTextures(1, &wallTextureID);

        // Ustawienie nowych parametr�w tekstury dla �ciany
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, wallData);

        // �adowanie nowej tekstury dla drzwi
        unsigned char* doorData = stbi_load("Tekstury\\doors.jpg", &width, &height, &channels, 0);

        // Generowanie nowej tekstury dla drzwi
        unsigned int doorTextureID;
        glGenTextures(1, &doorTextureID);

        // Ustawienie nowych parametr�w tekstury dla drzwi
        glBindTexture(GL_TEXTURE_2D, doorTextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, doorData);

        // Zwalnianie pami�ci
        stbi_image_free(wallData);
        stbi_image_free(doorData);


        glEnable(GL_TEXTURE_2D);

        // Rysowanie lewej �ciany
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-FLOOR_X, 0.0f, -FLOOR_Z);
        glTexCoord2f(0.0f, 5.0f); glVertex3f(-FLOOR_X, 0.0f, FLOOR_Z);
        glTexCoord2f(5.0f, 5.0f); glVertex3f(-FLOOR_X, FLOOR_HEIGHT, FLOOR_Z);
        glTexCoord2f(5.0f, 0.0f); glVertex3f(-FLOOR_X, FLOOR_HEIGHT, -FLOOR_Z);

        // Rysowanie prawej �ciany
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(FLOOR_X, 0.0f, -FLOOR_Z);
        glTexCoord2f(0.0f, 5.0f); glVertex3f(FLOOR_X, 0.0f, FLOOR_Z);
        glTexCoord2f(5.0f, 5.0f); glVertex3f(FLOOR_X, FLOOR_HEIGHT, FLOOR_Z);
        glTexCoord2f(5.0f, 0.0f); glVertex3f(FLOOR_X, FLOOR_HEIGHT, -FLOOR_Z);
        // Rysowanie �ciany na wprost
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-FLOOR_X, 0.0f, -FLOOR_Z);
        glTexCoord2f(0.0f, 5.0f);  glVertex3f(FLOOR_X, 0.0f, -FLOOR_Z);
        glTexCoord2f(5.0f, 5.0f); glVertex3f(FLOOR_X, FLOOR_HEIGHT, -FLOOR_Z);
        glTexCoord2f(5.0f, 0.0f); glVertex3f(-FLOOR_X, FLOOR_HEIGHT, -FLOOR_Z);

        // Rysowanie �ciany z ty�u
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-FLOOR_X, 0.0f, FLOOR_Z);
        glTexCoord2f(0.0f, 5.0f);  glVertex3f(-FLOOR_X, FLOOR_HEIGHT, FLOOR_Z);
        glTexCoord2f(5.0f, 5.0f); glVertex3f(FLOOR_X, FLOOR_HEIGHT, FLOOR_Z);
        glTexCoord2f(5.0f, 0.0f); glVertex3f(FLOOR_X, 0.0f, FLOOR_Z);
        glEnd();

        // Rysowanie pod�ogi
        glBegin(GL_QUADS);
        // Czarne i bia�e pola
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

        // Rysowanie drzwi do poprzedniego poziomu
        glBindTexture(GL_TEXTURE_2D, doorTextureID);
        glPushMatrix();
        glTranslatef(2.5f, 2.5f, FLOOR_Z + 0.01f);
        glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
        loadModel("Modele//door.obj");
        glPopMatrix();
        glEnd;

        // Rysowanie drzwi do nast�pnego poziomu
        glBindTexture(GL_TEXTURE_2D, doorTextureID);
        glPushMatrix();
        glTranslatef(2.5f, 2.5f, -FLOOR_Z + 0.01f);
        glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
        loadModel("Modele//door.obj");
        glPopMatrix();
        glEnd;

        //Przeszkody
        glBegin(GL_QUADS);
        lavaCreate(0.0f, 0.0f, 0.0f, 0.0f);
        lavaCreate(-5.0f, 2.0f, 8.0f, 4.0f);
        lavaCreate(-5.0f, -1.0f, 4.0f, 2.0f);
        lavaCreate(1.0f, -1.0f, 4.0f, 2.0f);
        lavaCreate(-5.0f, 6.0f, 2.0f, 4.0f);
        lavaCreate(3.0f, -5.0f, 2.0f, 3.0f);
        lavaCreate(-5.0f, -5.0f, 7.0f, 3.0f);
        lavaCreate(-3.0f, -8.0f, 8.0f, 2.0f);

        glEnd();

        //Powr�t do poprzedniego poziomu
        if (cubeX > -1.5f && cubeX < 1.5f && cubeZ == FLOOR_Z - STEP) {
            ROZDZIAL--;
            cubeX = 0.0f;
            cubeZ = -9.0f;
        }
        //Przej�cie do nast�pnego poziomu
        else if (cubeX > -1.5f && cubeX < 1.5f && cubeZ == -FLOOR_Z + STEP) {
            ROZDZIAL++;
            cubeX = 0.0f;
            cubeZ = 9.0f;
            cameraPosition[0] = cameraPositionStairsLvl[0];
            cameraPosition[1] = cameraPositionStairsLvl[1];
            cameraPosition[2] = cameraPositionStairsLvl[2];
        }

        //Sprawdzenie czy kolizji z law�
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

        // �adowanie nowej tekstury dla pod�ogi
        unsigned char* floorData = stbi_load("Tekstury\\stairs-small.jpg", &width, &height, &channels, 0);

        // Generowanie nowej tekstury dla pod�ogi
        unsigned int floorTextureID;
        glGenTextures(1, &floorTextureID);

        // Wyczyszczenie bufora koloru i g��boko�ci
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // �adowanie nowej tekstury dla �ciany
        unsigned char* wallData = stbi_load("Tekstury\\bricks2-small.jpg", &width, &height, &channels, 0);

        // Generowanie nowej tekstury dla �ciany
        unsigned int wallTextureID;
        glGenTextures(1, &wallTextureID);

        // Ustawienie nowych parametr�w tekstury dla pod�ogi
        glBindTexture(GL_TEXTURE_2D, floorTextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, floorData);

        // Ustawienie nowych parametr�w tekstury dla �ciany
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, wallData);


        // �adowanie nowej tekstury dla drzwi
        unsigned char* doorData = stbi_load("Tekstury\\doors.jpg", &width, &height, &channels, 0);

        // Generowanie nowej tekstury dla drzwi
        unsigned int doorTextureID;
        glGenTextures(1, &doorTextureID);

        // Ustawienie nowych parametr�w tekstury dla drzwi
        glBindTexture(GL_TEXTURE_2D, doorTextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, doorData);

        // Zwalnianie pami�ci
        stbi_image_free(doorData);
        stbi_image_free(wallData);
        stbi_image_free(floorData);

        glEnable(GL_TEXTURE_2D);

        // Wyczyszczenie bufora koloru i g��boko�ci
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Rysowanie pod�ogi
        glBindTexture(GL_TEXTURE_2D, floorTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-FLOOR_X, 0.0f, FLOOR_Z);
        glTexCoord2f(0.0f, 10.0f); glVertex3f(-FLOOR_X, FLOOR_HEIGHT - 0.5f, -FLOOR_Z);
        glTexCoord2f(10.0f, 10.0f); glVertex3f(FLOOR_X, FLOOR_HEIGHT - 0.5f, -FLOOR_Z);
        glTexCoord2f(10.0f, 0.0f); glVertex3f(FLOOR_X, 0.0f, FLOOR_Z);

        glEnd();

        glEnable(GL_TEXTURE_2D);

        // Rysowanie lewej �ciany
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-FLOOR_X, FLOOR_HEIGHT - 0.5f, -FLOOR_Z);
        glTexCoord2f(0.0f, 5.0f); glVertex3f(-FLOOR_X, 0.0f, FLOOR_Z);
        glTexCoord2f(5.0f, 5.0f); glVertex3f(-FLOOR_X, FLOOR_HEIGHT, FLOOR_Z);
        glTexCoord2f(5.0f, 0.0f); glVertex3f(-FLOOR_X, FLOOR_HEIGHT * 2 - 0.5f, -FLOOR_Z);

        // Rysowanie prawej �ciany
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(FLOOR_X, FLOOR_HEIGHT - 0.5f, -FLOOR_Z);
        glTexCoord2f(0.0f, 5.0f); glVertex3f(FLOOR_X, 0.0f, FLOOR_Z);
        glTexCoord2f(5.0f, 5.0f); glVertex3f(FLOOR_X, FLOOR_HEIGHT, FLOOR_Z);
        glTexCoord2f(5.0f, 0.0f); glVertex3f(FLOOR_X, FLOOR_HEIGHT * 2 - 0.5f, -FLOOR_Z);
        // Rysowanie �ciany na wprost
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-FLOOR_X, FLOOR_HEIGHT - 0.5f, -FLOOR_Z);
        glTexCoord2f(0.0f, 5.0f);  glVertex3f(FLOOR_X, FLOOR_HEIGHT - 0.5f, -FLOOR_Z);
        glTexCoord2f(5.0f, 5.0f); glVertex3f(FLOOR_X, FLOOR_HEIGHT * 2 - 0.5f, -FLOOR_Z);
        glTexCoord2f(5.0f, 0.0f); glVertex3f(-FLOOR_X, FLOOR_HEIGHT * 2 - 0.5f, -FLOOR_Z);

        // Rysowanie �ciany z ty�u
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-FLOOR_X, 0.0f, FLOOR_Z);
        glTexCoord2f(0.0f, 5.0f);  glVertex3f(-FLOOR_X, FLOOR_HEIGHT, FLOOR_Z);
        glTexCoord2f(5.0f, 5.0f); glVertex3f(FLOOR_X, FLOOR_HEIGHT, FLOOR_Z);
        glTexCoord2f(5.0f, 0.0f); glVertex3f(FLOOR_X, 0.0f, FLOOR_Z);
        glEnd();

        glEnd();

        // Rysowanie drzwi do poprzedniego poziomu
        glBindTexture(GL_TEXTURE_2D, doorTextureID);
        glPushMatrix();
        glTranslatef(2.5f, 2.5f, FLOOR_Z + 0.01f);
        glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
        loadModel("Modele//door.obj");
        glPopMatrix();
        glEnd();

        // Rysowanie drzwi do nast�pnego poziomu
        glBindTexture(GL_TEXTURE_2D, doorTextureID);
        glPushMatrix();
        glTranslatef(2.5f, FLOOR_HEIGHT + 1.5f, -FLOOR_Z + 0.01f);
        glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
        loadModel("Modele//door.obj");
        glPopMatrix();
        glEnd;

        // Powr�t do poprzedniego poziomu
        if (cubeX > -1.5f && cubeX < 1.5f && cubeZ == FLOOR_Z - STEP) {
            ROZDZIAL--;
            cubeX = 0.0f;
            cubeZ = -9.0f;
            cubeY = 0.5f;
            cameraPosition[0] = cameraPositionBackup[0];
            cameraPosition[1] = cameraPositionBackup[1];
            cameraPosition[2] = cameraPositionBackup[2];
        }

        // Przej�cie do nast�pnego poziomu
        else if (cubeX > -1.5f && cubeX < 1.5f && cubeZ == -FLOOR_Z + STEP) {
            ROZDZIAL++;
            cubeX = 0.0f;
            cubeZ = 9.0f;
            cubeY = 0.5f;
            cameraPosition[0] = cameraPositionBackup[0];
            cameraPosition[1] = cameraPositionBackup[1];
            cameraPosition[2] = cameraPositionBackup[2];
        }

    }

    // Rozdzia� czwarty gry
    else if (ROZDZIAL == 4) {

        FLOOR_X = 5.0f;
        FLOOR_Z = 10.0f;
        cubeY = 0.5f;
        cubePinkX = 10000.0f;
        cubePinkY = 10000.0f;
        cubePinkZ = 10000.0f;

        // Wyczyszczenie bufora koloru i g��boko�ci
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // �adowanie nowej tekstury dla �ciany
        unsigned char* wallData = stbi_load("Tekstury\\container-small.jpg", &width, &height, &channels, 0);

        // Generowanie nowej tekstury dla �ciany
        unsigned int wallTextureID;
        glGenTextures(1, &wallTextureID);

        // Ustawienie nowych parametr�w tekstury dla �ciany
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, wallData);

        // �adowanie nowej tekstury dla drzwi
        unsigned char* doorData = stbi_load("Tekstury\\doors.jpg", &width, &height, &channels, 0);

        // Generowanie nowej tekstury dla drzwi
        unsigned int doorTextureID;
        glGenTextures(1, &doorTextureID);

        // Ustawienie nowych parametr�w tekstury dla drzwi
        glBindTexture(GL_TEXTURE_2D, doorTextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, doorData);

        // Zwalnianie pami�ci
        stbi_image_free(doorData);
        stbi_image_free(wallData);


        glEnable(GL_TEXTURE_2D);

        // Rysowanie lewej �ciany
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-FLOOR_X, 0.0f, -FLOOR_Z);
        glTexCoord2f(0.0f, 5.0f); glVertex3f(-FLOOR_X, 0.0f, FLOOR_Z);
        glTexCoord2f(5.0f, 5.0f); glVertex3f(-FLOOR_X, FLOOR_HEIGHT, FLOOR_Z);
        glTexCoord2f(5.0f, 0.0f); glVertex3f(-FLOOR_X, FLOOR_HEIGHT, -FLOOR_Z);

        // Rysowanie prawej �ciany
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(FLOOR_X, 0.0f, -FLOOR_Z);
        glTexCoord2f(0.0f, 5.0f); glVertex3f(FLOOR_X, 0.0f, FLOOR_Z);
        glTexCoord2f(5.0f, 5.0f); glVertex3f(FLOOR_X, FLOOR_HEIGHT, FLOOR_Z);
        glTexCoord2f(5.0f, 0.0f); glVertex3f(FLOOR_X, FLOOR_HEIGHT, -FLOOR_Z);

        // Rysowanie �cianyy na wprost
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-FLOOR_X, 0.0f, -FLOOR_Z);
        glTexCoord2f(0.0f, 5.0f);  glVertex3f(FLOOR_X, 0.0f, -FLOOR_Z);
        glTexCoord2f(5.0f, 5.0f); glVertex3f(FLOOR_X, FLOOR_HEIGHT, -FLOOR_Z);
        glTexCoord2f(5.0f, 0.0f); glVertex3f(-FLOOR_X, FLOOR_HEIGHT, -FLOOR_Z);

        // Rysowanie �ciany z ty�u
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-FLOOR_X, 0.0f, FLOOR_Z);
        glTexCoord2f(0.0f, 5.0f);  glVertex3f(-FLOOR_X, FLOOR_HEIGHT, FLOOR_Z);
        glTexCoord2f(5.0f, 5.0f); glVertex3f(FLOOR_X, FLOOR_HEIGHT, FLOOR_Z);
        glTexCoord2f(5.0f, 0.0f); glVertex3f(FLOOR_X, 0.0f, FLOOR_Z);
        glEnd();

        // Rysowanie pod�ogi
        glBegin(GL_QUADS);
        // Czarne i bia�e pola
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

        // Rysowanie drzwi do poprzedniego poziomu
        glBindTexture(GL_TEXTURE_2D, doorTextureID);
        glPushMatrix();
        glTranslatef(2.5f, 2.5f, FLOOR_Z + 0.2f);
        glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
        loadModel("Modele//door.obj");
        glPopMatrix();
        glEnd();

        // Rysowanie drzwi do nast�pnego poziomu
        glBindTexture(GL_TEXTURE_2D, doorTextureID);
        glPushMatrix();
        glTranslatef(2.5f, 2.5f, -FLOOR_Z + 0.2f);
        glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
        loadModel("Modele//door.obj");
        glPopMatrix();
        glEnd;

        // Ustalamy pr�dko�� poruszania
        float movementSpeed1 = 0.01f;
        float movementSpeed2 = 0.05f;
        float movementSpeed3 = 0.075f;
        float movementSpeed4 = 0.1f;

        //Rysowanie przeszk�d
        glBegin(GL_QUADS);

        glColor3f(1.0f, 0.0f, 0.0f);

        lavaCreate(obj0X, 0.0f, 0.0f, 0.0f);
        lavaCreate(obj1X, 6.0f, 5.0f, 0.5f);
        lavaCreate(obj2X, 3.0f, 4.0f, 1.0f);
        lavaCreate(obj3X, -3.0f, 3.0f, 1.5f);
        lavaCreate(obj4X, -7.5f, 2.0f, 2.5f);

        glEnd();

        // Poruszanie si� obiektu                
        // Aktualizujemy pozycj� kostki
        obj1X += movementSpeed1;
        obj2X += -movementSpeed2;
        obj3X += movementSpeed3;
        obj4X += -movementSpeed4;


        // Sprawdzamy czy kostka dotar�a do lewej �ciany
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

        // Sprawdzanie kolizji
        if (checkCollisionWithObj(cubeX, cubeZ, obj1X, 6.0f, 5.0f, 0.5f) ||
            checkCollisionWithObj(cubeX, cubeZ, obj2X, 3.0f, 4.0f, 1.0f) ||
            checkCollisionWithObj(cubeX, cubeZ, obj3X, -3.0f, 3.0f, 1.5f) ||
            checkCollisionWithObj(cubeX, cubeZ, obj4X, -7.5f, 2.0f, 2.5f)
            ) {
            restartGame();
        }

        //Powr�t do poprzedniego poziomu
        if (cubeX > -1.5f && cubeX < 1.5f && cubeZ == FLOOR_Z - STEP) {
            ROZDZIAL--;
            cubeX = 0.0f;
            cubeZ = -9.0f;
            cubeY = 10.0f;
            cameraPosition[0] = cameraPositionStairsLvl[0];
            cameraPosition[1] = cameraPositionStairsLvl[1];
            cameraPosition[2] = cameraPositionStairsLvl[2];
        }

        //Przej�cie do nast�pnego poziomu
        else if (cubeX > -1.5f && cubeX < 1.5f && cubeZ == -FLOOR_Z + STEP) {
            ROZDZIAL++;
            cubeX = 0.0f;
            cubeZ = 4.0f;
            cameraPosition[0] = cameraPositionLvl5[0];
            cameraPosition[1] = cameraPositionLvl5[1];
            cameraPosition[2] = cameraPositionLvl5[2];
        }
    }

    // Rozdzia� pi�ty gry
    else if (ROZDZIAL == 5) {

        FLOOR_X = 15.0f;
        FLOOR_Z = 5.0f;
        cubePinkX = 10000.0f;
        cubePinkY = 10000.0f;
        cubePinkZ = 10000.0f;

        // Pozycja �wiat�a
        GLfloat lightPosition1[] = { -FLOOR_X, FLOOR_HEIGHT / 2, FLOOR_Z, 1.0f }; // Pozycja pierwszego �wiat�a punktowego
        GLfloat lightPosition2[] = { FLOOR_X, FLOOR_HEIGHT / 2, FLOOR_Z, 1.0f }; // Pozycja drugiego �wiat�a punktowego
        GLfloat lightPosition3[] = { 0.0f, FLOOR_HEIGHT / 2, FLOOR_Z, 1.0f }; // Pozycja trzeciego �wiat�a punktowego
        GLfloat lightPosition4[] = { 10.0f, FLOOR_HEIGHT / 2, FLOOR_Z, 1.0f }; // Pozycja czwartego �wiat�a punktowego
        GLfloat lightPosition5[] = { -10.0f, FLOOR_HEIGHT / 2, FLOOR_Z, 1.0f }; // Pozycja pi�tego �wiat�a punktowego
        GLfloat lightPosition6[] = { 0.0f, FLOOR_HEIGHT / 2, FLOOR_Z, 1.0f }; // Pozycja sz�stego �wiat�a punktowego
        GLfloat lightPosition7[] = { 5.0f, FLOOR_HEIGHT / 2, FLOOR_Z, 1.0f }; // Pozycja si�dmego �wiat�a punktowego
        GLfloat lightPosition8[] = { -5.0f, FLOOR_HEIGHT / 2, FLOOR_Z, 1.0f }; // Pozycja �smego �wiat�a punktowego

        // Atrybuty �wiat��
        GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // Sk�adowa dyfuzyjna �wiat�a
        GLfloat lightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f }; // Sk�adowa ambientalna �wiat�a
        GLfloat lightDirection[] = { 0.0f, -1.0f, -1.0f }; // Kierunek �wiat�a (w tym przypadku skierowane w d�)
        GLfloat lightDirection2[] = { 0.0f, 0.0f, 1.0f }; // Kierunek �wiat�a (w tym przypadku skierowane do przodu)


        glEnable(GL_LIGHTING); // W��cz o�wietlenie

        glLightfv(GL_LIGHT1, GL_POSITION, lightPosition1); // Ustaw pozycj� pierwszego �wiat�a punktowego
        glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse); // Ustaw sk�adow� dyfuzyjn� �wiat�a
        glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient); // Ustaw sk�adow� ambientaln� �wiat�a
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, lightDirection); // Ustaw kierunek �wiat�a typu spotlight
        glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0f); // Ustaw k�t rozwarcia sto�ka �wietlnego (w tym przypadku 45 stopni)
        glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0f); // Ustaw wyk�adnik sto�ka �wietlnego

        glEnable(GL_LIGHT1); // W��cz pierwsze �wiat�o

        glLightfv(GL_LIGHT2, GL_POSITION, lightPosition2); // Ustaw pozycj� drugiego �wiat�a punktowego
        glLightfv(GL_LIGHT2, GL_DIFFUSE, lightDiffuse); // Ustaw sk�adow� dyfuzyjn� �wiat�a
        glLightfv(GL_LIGHT2, GL_AMBIENT, lightAmbient); // Ustaw sk�adow� ambientaln� �wiat�a
        glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, lightDirection); // Ustaw kierunek �wiat�a typu spotlight
        glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 45.0f); // Ustaw k�t rozwarcia sto�ka �wietlnego (w tym przypadku 45 stopni)
        glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 2.0f); // Ustaw wyk�adnik sto�ka �wietlnego

        glEnable(GL_LIGHT2); // W��cz drugie �wiat�o

        glLightfv(GL_LIGHT3, GL_POSITION, lightPosition3); // Ustaw pozycj� trzeciego �wiat�a punktowego
        glLightfv(GL_LIGHT3, GL_DIFFUSE, lightDiffuse); // Ustaw sk�adow� dyfuzyjn� �wiat�a
        glLightfv(GL_LIGHT3, GL_AMBIENT, lightAmbient); // Ustaw sk�adow� ambientaln� �wiat�a
        glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, lightDirection); // Ustaw kierunek �wiat�a typu spotlight
        glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, 45.0f); // Ustaw k�t rozwarcia sto�ka �wietlnego (w tym przypadku 45 stopni)
        glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, 2.0f); // Ustaw wyk�adnik sto�ka �wietlnego

        glEnable(GL_LIGHT3); // W��cz trzecie �wiat�o

        glLightfv(GL_LIGHT4, GL_POSITION, lightPosition4); // Ustaw pozycj� czwartego �wiat�a punktowego
        glLightfv(GL_LIGHT4, GL_DIFFUSE, lightDiffuse); // Ustaw sk�adow� dyfuzyjn� �wiat�a
        glLightfv(GL_LIGHT4, GL_AMBIENT, lightAmbient); // Ustaw sk�adow� ambientaln� �wiat�a
        glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, lightDirection); // Ustaw kierunek �wiat�a typu spotlight
        glLightf(GL_LIGHT4, GL_SPOT_CUTOFF, 45.0f); // Ustaw k�t rozwarcia sto�ka �wietlnego (w tym przypadku 45 stopni)
        glLightf(GL_LIGHT4, GL_SPOT_EXPONENT, 2.0f); // Ustaw wyk�adnik sto�ka �wietlnego

        glEnable(GL_LIGHT4); // W��cz czwarte �wiat�o

        glLightfv(GL_LIGHT5, GL_POSITION, lightPosition5); // Ustaw pozycj� czwartego �wiat�a punktowego
        glLightfv(GL_LIGHT5, GL_DIFFUSE, lightDiffuse); // Ustaw sk�adow� dyfuzyjn� �wiat�a
        glLightfv(GL_LIGHT5, GL_AMBIENT, lightAmbient); // Ustaw sk�adow� ambientaln� �wiat�a
        glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, lightDirection); // Ustaw kierunek �wiat�a typu spotlight
        glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, 45.0f); // Ustaw k�t rozwarcia sto�ka �wietlnego (w tym przypadku 45 stopni)
        glLightf(GL_LIGHT5, GL_SPOT_EXPONENT, 2.0f); // Ustaw wyk�adnik sto�ka �wietlnego

        glEnable(GL_LIGHT5); // W��cz pi�te �wiat�o

        glLightfv(GL_LIGHT6, GL_POSITION, lightPosition6); // Ustaw pozycj� czwartego �wiat�a punktowego
        glLightfv(GL_LIGHT6, GL_DIFFUSE, lightDiffuse); // Ustaw sk�adow� dyfuzyjn� �wiat�a
        glLightfv(GL_LIGHT6, GL_AMBIENT, lightAmbient); // Ustaw sk�adow� ambientaln� �wiat�a
        glLightfv(GL_LIGHT6, GL_SPOT_DIRECTION, lightDirection); // Ustaw kierunek �wiat�a typu spotlight
        glLightf(GL_LIGHT6, GL_SPOT_CUTOFF, 45.0f); // Ustaw k�t rozwarcia sto�ka �wietlnego (w tym przypadku 45 stopni)
        glLightf(GL_LIGHT6, GL_SPOT_EXPONENT, 2.0f); // Ustaw wyk�adnik sto�ka �wietlnego

        glEnable(GL_LIGHT6); // W��cz sz�ste �wiat�o

        glLightfv(GL_LIGHT7, GL_POSITION, lightPosition7); // Ustaw pozycj� czwartego �wiat�a punktowego
        glLightfv(GL_LIGHT7, GL_DIFFUSE, lightDiffuse); // Ustaw sk�adow� dyfuzyjn� �wiat�a
        glLightfv(GL_LIGHT7, GL_AMBIENT, lightAmbient); // Ustaw sk�adow� ambientaln� �wiat�a
        glLightfv(GL_LIGHT7, GL_SPOT_DIRECTION, lightDirection); // Ustaw kierunek �wiat�a typu spotlight
        glLightf(GL_LIGHT7, GL_SPOT_CUTOFF, 45.0f); // Ustaw k�t rozwarcia sto�ka �wietlnego (w tym przypadku 45 stopni)
        glLightf(GL_LIGHT7, GL_SPOT_EXPONENT, 2.0f); // Ustaw wyk�adnik sto�ka �wietlnego

        glEnable(GL_LIGHT7); // W��cz si�dme �wiat�o

        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition8); // Ustaw pozycj� czwartego �wiat�a punktowego
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse); // Ustaw sk�adow� dyfuzyjn� �wiat�a
        glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient); // Ustaw sk�adow� ambientaln� �wiat�a
        glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightDirection); // Ustaw kierunek �wiat�a typu spotlight
        glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45.0f); // Ustaw k�t rozwarcia sto�ka �wietlnego (w tym przypadku 45 stopni)
        glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2.0f); // Ustaw wyk�adnik sto�ka �wietlnego

        glEnable(GL_LIGHT0); // W��cz �sme �wiat�o

        // Wyczyszczenie bufora koloru i g��boko�ci
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // �adowanie nowej tekstury dla �ciany
        unsigned char* wallData = stbi_load("Tekstury\\wall-small.jpg", &width, &height, &channels, 0);

        // �adowanie nowej tekstury dla drzwi
        unsigned char* doorData = stbi_load("Tekstury\\doors-small.jpg", &width, &height, &channels, 0);

        // Generowanie nowej tekstury dla �ciany
        unsigned int wallTextureID;
        glGenTextures(1, &wallTextureID);

        // Generowanie nowej tekstury dla drzwi
        unsigned int doorTextureID;
        glGenTextures(1, &doorTextureID);

        // Ustawienie nowych parametr�w tekstury dla �ciany
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, wallData);

        // Ustawienie nowych parametr�w tekstury dla drzwi
        glBindTexture(GL_TEXTURE_2D, doorTextureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width / 2, height / 2, 0, GL_RGB, GL_UNSIGNED_BYTE, doorData);

        // Zwalnianie pami�ci
        stbi_image_free(wallData);
        stbi_image_free(doorData);

        glEnable(GL_TEXTURE_2D);

        // Rysowanie lewej �ciany
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-FLOOR_X, 0.0f, -FLOOR_Z);
        glTexCoord2f(0.0f, 5.0f); glVertex3f(-FLOOR_X, 0.0f, FLOOR_Z);
        glTexCoord2f(5.0f, 5.0f); glVertex3f(-FLOOR_X, FLOOR_HEIGHT, FLOOR_Z);
        glTexCoord2f(5.0f, 0.0f); glVertex3f(-FLOOR_X, FLOOR_HEIGHT, -FLOOR_Z);

        // Rysowanie prawej �ciany
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(FLOOR_X, 0.0f, -FLOOR_Z);
        glTexCoord2f(0.0f, 5.0f); glVertex3f(FLOOR_X, 0.0f, FLOOR_Z);
        glTexCoord2f(5.0f, 5.0f); glVertex3f(FLOOR_X, FLOOR_HEIGHT, FLOOR_Z);
        glTexCoord2f(5.0f, 0.0f); glVertex3f(FLOOR_X, FLOOR_HEIGHT, -FLOOR_Z);

        // Rysowanie �ciany na wprost
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-FLOOR_X, 0.0f, -FLOOR_Z);
        glTexCoord2f(0.0f, 5.0f);  glVertex3f(FLOOR_X, 0.0f, -FLOOR_Z);
        glTexCoord2f(5.0f, 5.0f); glVertex3f(FLOOR_X, FLOOR_HEIGHT, -FLOOR_Z);
        glTexCoord2f(5.0f, 0.0f); glVertex3f(-FLOOR_X, FLOOR_HEIGHT, -FLOOR_Z);

        // Rysowanie �ciany z ty�u
        glBindTexture(GL_TEXTURE_2D, wallTextureID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-FLOOR_X, 0.0f, FLOOR_Z);
        glTexCoord2f(0.0f, 5.0f);  glVertex3f(-FLOOR_X, FLOOR_HEIGHT, FLOOR_Z);
        glTexCoord2f(5.0f, 5.0f); glVertex3f(FLOOR_X, FLOOR_HEIGHT, FLOOR_Z);
        glTexCoord2f(5.0f, 0.0f); glVertex3f(FLOOR_X, 0.0f, FLOOR_Z);
        glEnd();

        // Rysowanie pod�ogi
        glBegin(GL_QUADS);
        // Czarne i bia�e pola
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
        glEnd();

        glEnable(GL_TEXTURE_2D);

        // Rysowanie drzwi do poprzedniego poziomu
        glBindTexture(GL_TEXTURE_2D, doorTextureID);
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, FLOOR_Z - 0.2f);
        loadModel("Modele//door.obj");
        glPopMatrix();
        glEnd;

        // Rozdzia� pi�ty gry
        if (lvl5done) {

            // �adowanie nowej tekstury dla drzwi
            unsigned char* doorData = stbi_load("Tekstury\\doors.jpg", &width, &height, &channels, 0);

            // Generowanie nowej tekstury dla drzwi
            unsigned int doorTextureID;
            glGenTextures(1, &doorTextureID);

            // Ustawienie nowych parametr�w tekstury dla drzwi
            glBindTexture(GL_TEXTURE_2D, doorTextureID);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, doorData);

            // Zwalnianie pami�ci
            stbi_image_free(doorData);

            // Rysowanie drzwi do nast�pnego poziomu
            glBindTexture(GL_TEXTURE_2D, doorTextureID);
            glPushMatrix();
            glTranslatef(-FLOOR_X + 0.2f, 2.5f, -2.5f);
            glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
            glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
            loadModel("Modele//door.obj");
            glPopMatrix();
            glEnd;

            if (cubeZ > -1.5f && cubeZ < 1.5f && cubeX == -FLOOR_X + STEP) {
                ROZDZIAL++;
                cubeX = 0.0f;
                cubeZ = 9.0f;
                restartGame(); //- usun��
            }
        }
        else {
            drawCircle(13.5f, 0.0f, 8.0f, 0.5f);
            glPopMatrix;
            drawCircle(13.5f, 0.0f, 8.0f, 0.5f);
            glPopMatrix;
        }

        glColor3f(1.0f, 1.0f, 1.0f);

        ledderX = FLOOR_X - STEP;
        ledderZ = FLOOR_Z;
        drawLadder(ledderX, ledderZ);
        glEnd();

        //Powr�t do poprzedniego poziomu
        if (cubeX > -1.5f && cubeX < 1.5f && cubeZ == FLOOR_Z - STEP) {
            ROZDZIAL--;
            cubeX = 0.0f;
            cubeZ = -9.0f;
            cameraPosition[0] = cameraPositionBackup[0];
            cameraPosition[1] = cameraPositionBackup[1];
            cameraPosition[2] = cameraPositionBackup[2];
        }

        //Uko�czenie poziomu
        else if (cubeX == 13.5f && cubeZ == 0.0f && cubeY >= 8.0f) {
            lvl5done = true;
        }
    }

    // �adowanie nowej tekstury dla �ciany
    unsigned char* characterData = stbi_load("Tekstury\\lava-small.jpg", &width, &height, &channels, 0);

    // Generowanie nowej tekstury dla �ciany
    unsigned int characterTextureId;
    glGenTextures(1, &characterTextureId);

    // Ustawienie nowych parametr�w tekstury dla �ciany
    glBindTexture(GL_TEXTURE_2D, characterTextureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, characterData);

    // Zwalnianie pami�ci
    stbi_image_free(characterData);


    // Rysowanie postaci
    glBindTexture(GL_TEXTURE_2D, characterTextureId);
    glPushMatrix();
    glTranslatef(cubeX, cubeY, cubeZ);
    glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f);
    loadModel("Modele//character.obj");
    glPopMatrix();
    glEnd();
    glColor3f(1.0f, 1.0f, 1.0f);

    glutPostRedisplay(); // Od�wie�enie ekranu po dotarciu na odpowiednie koordy

    // Wymiana bufor�w
    glutSwapBuffers();
}

// Obs�uga klawiszy
void keyboard(unsigned char key, int x, int y) {
    switch (key) {

        // Poruszanie w prz�d
    case 'w':
        cubeZ -= STEP;
        rotationAngle = 180.0f;
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
        else if (clickedE) {
            cubePinkZ -= STEP;
        }

        //Przemieszczenie r�owej kostki
        else if (collisionCube(cubeX, cubeZ, cubePinkX, cubePinkZ, 1)) {
            cubePinkZ -= STEP;
        }
        break;

        // Poruszanie do ty�u
    case 's':
        rotationAngle = 0.0f;
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

        // Poruszanie w lewo
    case 'a':

        cubeX -= STEP;
        rotationAngle = -90.0f;
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

        // Poruszanie w prawo
    case 'd':

        cubeX += STEP;
        rotationAngle = 90.0f;
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

        //Dzia�anie drabiny - poruszanie w g�r�
    case 't':
        if (ROZDZIAL == 5 && miejsceDrabiny(cubeX, cubeY, cubeZ) && cubeY < 5.0f) {
            cubeY += 0.1f;
            tmpCubeY = cubeY;
        }
        break;

        //Dzia�anie drabiny - poruszanie w d�
    case 'r':
        if (ROZDZIAL == 5 && miejsceDrabiny(cubeX, cubeY, cubeZ) && cubeY > 0.5f)
            cubeY -= 0.1f;
        break;

        //Ci�gni�cie kostki
    case 'e':
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
            )
            clickedE = true;

        break;

        // Spacja - skok
    case 32:
        if (!cubeIsJumping && !cubeIsFalling && ROZDZIAL != 3)
        {
            cubeIsJumping = true;
        }
        break;
    default:
        break;
    }
    glutPostRedisplay(); // Od�wie�enie ekranu
}

// funkcja wywo�ywana podczas poruszania mysz�
void motion(int x, int y) {

    // obliczamy r�nic� pozycji myszy
    int deltaX = x - mouseX;
    int deltaY = y - mouseY;

    // aktualizujemy obroty kamery
    cameraRotationY += deltaX * 0.5f;
    cameraRotationX += deltaY * 0.5f;

    // zapisujemy aktualn� pozycj� myszy
    mouseX = x;
    mouseY = y;

    // od�wie�amy scen�
    glutPostRedisplay();
}

// funkcja wywo�ywana podczas naci�ni�cia przycisku myszy
void mouse(int button, int state, int x, int y) {

    // zapisujemy pozycj� myszy
    mouseX = x;
    mouseY = y;
}

void handleScroll(int button, int state, int x, int y) {
    if (button == 3) {
        // przybli�enie kamery
        cameraPosition[2] -= 1.0;
    }
    else if (button == 4) {
        // oddalenie kamery
        cameraPosition[2] += 1.0;
    }
    // od�wie�amy scen�
    glutPostRedisplay();
}