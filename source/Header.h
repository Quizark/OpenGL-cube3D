#ifndef HEADER_H
#define HEADER_H

// Biblioteka OpenGL
#include <GL/glut.h> 

// Biblioteka do obs�ugi strumieni wej�cia/wyj�cia
#include <iostream> 

// Bibioteka do obs�ugi tekstur
#include "stb-master/stb_image.h" 

// Biblioteka do obs�ugi modeli
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>

// Rozmiary okna
const int WIDTH = 1366;
const int HEIGHT = 768;


// Sprawdzanie kolizji ze �cianami pomieszczenia
bool checkCollision(float cubeX, float cubeY, float cubeZ, int sciana);

// Sprawdzanie kolizji postaci
bool collisionCube(float cubeX, float cubeZ, float cubePinkX, float cubePinkZ, int kierunek);

// Sprawdzanie kolizji z obiektami
bool checkCollisionWithObj(float cubeX, float cubeZ, float startObjX, float startObjZ, float widthX, float widthZ);

// tworzenie drzwi kt�re pozwalaj� na przechodzenie pomi�dzy poziomami
void doorCreate(float x, float z, int side);

// tworzenie przeszk�d
void lavaCreate(float x, float z, float xStep, float zStep);

// Restartowanie gry w przypadku wej�cia w przeszkode
void restartGame();

// Za�adowanie modelu 
void loadModel(const char* filename);

// Sprawdzanie czy jeste�my w pobli�u drabiny 
bool miejsceDrabiny(float cubeX, float cubeY, float cubeZ);

// Rysowanie okr�g�w 
void drawCircle(float x, float y, float high, float radius);

// Rysowanie drabiny
void drawLadder(float x, float z);

// Renderowanie sceny
void renderScene();

// Zaprogramowanie klawiszy potrzebnych do rozgrywki
void keyboard(unsigned char key, int x, int y);

// Poruszanie mysz�
void motion(int x, int y);

// Zapis pozycji myszy
void mouse(int button, int state, int x, int y);

// Zmiana odleg�o�ci kamery
void handleScroll(int button, int state, int x, int y);

#endif