#include "ra/Piece.hpp"

namespace Chess
{
    class Bishop : public Piece
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
            drawRectangle(x_ + 20, y_ + 13, x_ + 30, y_ + 32);
            drawTriangle(x_ + 15, y_ + 32, x_ + 35, y_ + 40);
            glFlush();
        }

        void updateLegalMoves(Piece *board[8][8])
        {
            legalMoves.clear();
            capturablePieces.clear();
            posXYBlock = posXnegYBlock = negXposYBlock = negXYBlock = 0;
            for (int i = 1; i < 8; i++)
            {
                if ((x + i) < 8 && (y + i) < 8 && !posXYBlock)
                {
                    if (CanCapture(board[x + i][y + i]))
                    {
                        capturablePieces.emplace_back(coordinate(x + i, y + i));
                        legalMoves.emplace_back(coordinate(x + i, y + i));
                    }

                    posXYBlock |= (board[x + i][y + i])->Type() != PieceType::empty;
                    if (!posXYBlock)
                        legalMoves.emplace_back(coordinate(x + i, y + i));
                }
                if ((x - i) >= 0 && (y + i) < 8 && !negXposYBlock)
                {
                    if (CanCapture(board[x - i][y + i]))
                    {
                        capturablePieces.emplace_back(coordinate(x - i, y + i));
                        legalMoves.emplace_back(coordinate(x - i, y + i));
                    }

                    negXposYBlock |= (board[x - i][y + i])->Type() != PieceType::empty;
                    if (!negXposYBlock)
                        legalMoves.emplace_back(coordinate(x - i, y + i));
                }
                if ((x + i) < 8 && (y - i) >= 0 && !posXnegYBlock)
                {
                    if (CanCapture(board[x + i][y - i]))
                    {
                        capturablePieces.emplace_back(coordinate(x + i, y - i));
                        legalMoves.emplace_back(coordinate(x + i, y - i));
                    }

                    posXnegYBlock |= (board[x + i][y - i])->Type() != PieceType::empty;
                    if (!posXnegYBlock)
                        legalMoves.emplace_back(coordinate(x + i, y - i));
                }
                if ((x - i) >= 0 && (y - i) >= 0 && !negXYBlock)
                {
                    if (CanCapture(board[x - i][y - i]))
                    {
                        capturablePieces.emplace_back(coordinate(x - i, y - i));
                        legalMoves.emplace_back(coordinate(x - i, y - i));
                    }

                    negXYBlock |= (board[x - i][y - i])->Type() != PieceType::empty;
                    if (!negXYBlock)
                        legalMoves.emplace_back(coordinate(x - i, y - i));
                }
            }
        }
        void SetType()
        {
            value = 3;
            pieceType = PieceType::bishop;
        }

    private:
        bool posXYBlock, posXnegYBlock, negXYBlock, negXposYBlock;
    };

}