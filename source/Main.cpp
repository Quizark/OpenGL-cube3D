#include "Header.h"

#define STB_IMAGE_IMPLEMENTATION


// Funkcja inicjuj�ca
void init() {

    // Ustawienie koloru t�a
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // W��czenie testu g��boko�ci
    glEnable(GL_DEPTH_TEST);

    // Ustawienie perspektywy
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
}


int main(int argc, char** argv) {

    // Inicjalizacja biblioteki GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("OpenGL - Game Cube3D");

    // Rejestracja funkcji odpowiedzialnej za rysowanie
    glutDisplayFunc(renderScene);

    // Rejestracja funkcji obs�uguj�cej klawiatur�
    glutKeyboardFunc(keyboard);

    // Ustawienie perspektywy
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

    // W��czenie testu g��boko�ci
    glEnable(GL_DEPTH_TEST);

    // ustawiamy funkcje wywo�ywane podczas poruszania mysz� i naci�ni�cia przycisku myszy
    glutMotionFunc(motion);
    glutMouseFunc(mouse);
    glutMouseFunc(handleScroll);

    // Rozpocz�cie p�tli g��wnej programu
    glutMainLoop();

    return 0;
}