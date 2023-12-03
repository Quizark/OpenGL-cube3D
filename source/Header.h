#ifndef HEADER_H
#define HEADER_H

// Biblioteka OpenGL
#include <GL/glut.h> 

// Biblioteka do obs³ugi strumieni wejœcia/wyjœcia
#include <iostream> 

// Bibioteka do obs³ugi tekstur
#include "stb-master/stb_image.h" 

// Biblioteka do obs³ugi modeli
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>

// Rozmiary okna
const int WIDTH = 1366;
const int HEIGHT = 768;


// Sprawdzanie kolizji ze œcianami pomieszczenia
bool checkCollision(float cubeX, float cubeY, float cubeZ, int sciana);

// Sprawdzanie kolizji postaci
bool collisionCube(float cubeX, float cubeZ, float cubePinkX, float cubePinkZ, int kierunek);

// Sprawdzanie kolizji z obiektami
bool checkCollisionWithObj(float cubeX, float cubeZ, float startObjX, float startObjZ, float widthX, float widthZ);

// tworzenie drzwi które pozwalaj¹ na przechodzenie pomiêdzy poziomami
void doorCreate(float x, float z, int side);

// tworzenie przeszkód
void lavaCreate(float x, float z, float xStep, float zStep);

// Restartowanie gry w przypadku wejœcia w przeszkode
void restartGame();

// Za³adowanie modelu 
void loadModel(const char* filename);

// Sprawdzanie czy jesteœmy w pobli¿u drabiny 
bool miejsceDrabiny(float cubeX, float cubeY, float cubeZ);

// Rysowanie okrêgów 
void drawCircle(float x, float y, float high, float radius);

// Rysowanie drabiny
void drawLadder(float x, float z);

// Renderowanie sceny
void renderScene();

// Zaprogramowanie klawiszy potrzebnych do rozgrywki
void keyboard(unsigned char key, int x, int y);

// Poruszanie mysz¹
void motion(int x, int y);

// Zapis pozycji myszy
void mouse(int button, int state, int x, int y);

// Zmiana odleg³oœci kamery
void handleScroll(int button, int state, int x, int y);

#endif