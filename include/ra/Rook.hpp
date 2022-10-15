#include "ra/Piece.hpp"
namespace Chess
{
    class Rook : public Piece
    {
    public:
        using Piece::Piece;

        void Draw()
        {
            colour == Colour::white ? glColor3f(1, 1, 1) : glColor3f(0, 0, 0);
            if (isSelected)
            {
                glColor3f(0, 1, 0);
            }
            int x_ = x * squareSize;
            int y_ = y * squareSize;
            drawRectangle(x_ + 10, y_ + 10, x_ + 40, y_ + 13);
            drawRectangle(x_ + 15, y_ + 10, x_ + 35, y_ + 32);
            drawRectangle(x_ + 10, y_ + 32, x_ + 40, y_ + 36);
            drawRectangle(x_ + 10, y_ + 36, x_ + 15, y_ + 40);
            drawRectangle(x_ + 22, y_ + 36, x_ + 28, y_ + 40);
            drawRectangle(x_ + 35, y_ + 36, x_ + 40, y_ + 40);
            glFlush();
        }

        void updateLegalMoves(Piece *board[8][8])
        {

            legalMoves.clear();
            capturablePieces.clear();
            posXBlock = posYBlock = negXBlock = negYBlock = 0;

            for (int i = 1; i < 8; i++)
            {
                if ((x + i) < 8 && !posXBlock)
                {
                    if (CanCapture(board[x + i][y]))
                    {
                        capturablePieces.emplace_back(coordinate(x + i, y));
                        legalMoves.emplace_back(coordinate(x + i, y));
                    }

                    posXBlock |= (board[x + i][y])->Type() != PieceType::empty;
                    if (!posXBlock)
                        legalMoves.emplace_back(coordinate(x + i, y));
                }
                if ((x - i) >= 0 && !negXBlock)
                {
                    if (CanCapture(board[x - i][y]))
                    {
                        capturablePieces.emplace_back(coordinate(x - i, y));
                        legalMoves.emplace_back(coordinate(x - i, y));
                    }

                    negXBlock |= (board[x - i][y])->Type() != PieceType::empty;
                    if (!negXBlock)
                        legalMoves.emplace_back(coordinate(x - i, y));
                }
                if ((y + i) < 8 && !posYBlock)
                {
                    if (CanCapture(board[x][y + i]))
                    {
                        capturablePieces.emplace_back(coordinate(x, y + i));
                        legalMoves.emplace_back(coordinate(x, y + i));
                    }

                    posYBlock |= (board[x][y + i])->Type() != PieceType::empty;
                    if (!posYBlock)
                        legalMoves.emplace_back(coordinate(x, y + i));
                }
                if ((y - i) >= 0 && !negYBlock)
                {
                    if (CanCapture(board[x][y - i]))
                    {
                        capturablePieces.emplace_back(coordinate(x, y - i));
                        legalMoves.emplace_back(coordinate(x, y - i));
                    }

                    negYBlock |= (board[x][y - i])->Type() != PieceType::empty;
                    if (!negYBlock)
                        legalMoves.emplace_back(coordinate(x, y - i));
                }
            }
        }
        void SetType()
        {
            value = 5;
            pieceType = PieceType::rook;
        }

    private:
        bool posXBlock, posYBlock, negXBlock, negYBlock;
    };
}