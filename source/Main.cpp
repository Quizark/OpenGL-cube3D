#include "Header.h"

#define STB_IMAGE_IMPLEMENTATION


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

    // ustawiamy funkcje wywo³ywane podczas poruszania mysz¹ i naciœniêcia przycisku myszy
    glutMotionFunc(motion);
    glutMouseFunc(mouse);
    glutMouseFunc(handleScroll);

    // Rozpoczêcie pêtli g³ównej programu
    glutMainLoop();

    return 0;
}