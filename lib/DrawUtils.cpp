#include "ra/DrawUtils.hpp"
#define squareSize 50
namespace Chess
{
    void drawRectangle(int x1, int y1, int x2, int y2)
    {
        glBegin(GL_POLYGON);
        glVertex2i(x1, y1);
        glVertex2i(x1, y2);
        glVertex2i(x2, y2);
        glVertex2i(x2, y1);
        glEnd();
    }

    void drawCircle(int x, int y, int r)
    {
        glPointSize(r);
        glBegin(GL_POINTS);
        glVertex2f(x, y);
        glEnd();
    }

    void drawTriangle(int x1, int y1, int x2, int y2)
    {
        glBegin(GL_POLYGON);
        glVertex2i(x1, y1);
        glVertex2i((x1 + x2) / 2, y2);
        glVertex2i(x2, y1);
        glEnd();
    }

    void drawEmptySquare(int x, int y)
    {
        bool isLightSquare;
        if (y % 2)
        {
            isLightSquare = (x % 2) ? 0 : 1;
        }
        else
        {
            isLightSquare = (x % 2) ? 1 : 0;
        }
        int x_ = x * squareSize;
        int y_ = y * squareSize;
        isLightSquare ? glColor3f(0.8, 0.8, 1) : glColor3f(0.4, 0.4, 0.5);
        glBegin(GL_POLYGON);
        glVertex2i(x_, y_);
        glVertex2i(x_, y_ + 50);
        glVertex2i(x_ + 50, y_ + 50);
        glVertex2i(x_ + 50, y_);
        glEnd();
    }

    void drawCheckMark(int x, int y)
    {
        glColor3f(0, 1, 0);
        glBegin(GL_POLYGON);
        glVertex2i(x, y);
        glVertex2i(x + 20, y - 5);
        glVertex2i(x + 15, y - 10);
        glEnd();
        glBegin(GL_POLYGON);
        glVertex2i(x + 15, y - 3);
        glVertex2i(x + 40, y + 17);
        glVertex2i(x + 15, y - 10);
        glEnd();
    }
}