#include "ra/Piece.hpp"
namespace Chess
{
    class Pawn : public Piece
    {
    public:
        using Piece::Piece;

        void Draw()
        {
            colour == Colour::white ? glColor3f(1,1,1) : glColor3f(0, 0, 0);
            if (isSelected)
            {
                glColor3f(0, 1, 0);
            }
            int x_ = x * squareSize;
            int y_ = y * squareSize;
            drawTriangle(x_ + 10, y_ + 10, x_ + 40, y_ + 30);
            drawCircle(x_ + 25, y_ + 35, 10);
            glFlush();
        }

        void updateLegalMoves(Piece *board[8][8])
        {
            legalMoves.clear();
            capturablePieces.clear();
            block = false;
            int yInc;
            colour == Colour::white ? yInc = 1 : yInc = -1;
            if (y < 7 && y > 0)
            {
                if (board[x][y + yInc]->Type() == PieceType::empty)
                {
                    legalMoves.emplace_back(coordinate(x, y + yInc));
                }
            }
            if (x < 7)
            {
                if (CanCapture(board[x + 1][y + yInc]))
                {
                    capturablePieces.emplace_back(coordinate(x + 1, y + yInc));
                    legalMoves.emplace_back(coordinate(x + 1, y + yInc));
                }
            }
            if (x > 0)
            {
                if (CanCapture(board[x - 1][y + yInc]))
                {
                    capturablePieces.emplace_back(coordinate(x - 1, y + yInc));
                    legalMoves.emplace_back(coordinate(x - 1, y + yInc));
                }
            }
            if (((y == 1 && GetColour() == Colour::white) || (y == 6 && GetColour() == Colour::black)) && board[x][y + 2 * yInc]->Type() == PieceType::empty)
            {
                legalMoves.emplace_back(coordinate(x, y + 2 * yInc));
            }
        }

        void SetType()
        {
            pieceType = PieceType::pawn;
            value = 1;
        }

    private:
        bool isStartingSquare = 1;
        bool block;
    };
}