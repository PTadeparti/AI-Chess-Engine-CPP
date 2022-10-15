#ifndef DRAWUTILS_H
#define DRAWUTILS_H
#include <GL/glut.h>
namespace Chess
{
    void drawRectangle(int, int, int, int);
    void drawTriangle(int, int, int, int);
    void drawCircle(int, int, int);
    void drawEmptySquare(int, int);
    void drawCheckMark(int, int);
}

#endif