#include "ra/Piece.hpp"

namespace Chess
{
    class King : public Piece
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
            if (isInCheck && !isSelected)
            {
                glColor3f(1, 0, 0);
            }
            int x_ = x * squareSize;
            int y_ = y * squareSize;
            drawRectangle(x_ + 10, y_ + 10, x_ + 40, y_ + 15);
            drawRectangle(x_ + 18, y_ + 15, x_ + 32, y_ + 30);
            drawRectangle(x_ + 23, y_ + 30, x_ + 27, y_ + 45);
            drawRectangle(x_ + 20, y_ + 38, x_ + 30, y_ + 42);
            glFlush();
        }

        void updateLegalMoves(Piece *board[8][8])
        {
            legalMoves.clear();
            capturablePieces.clear();
            posXBlock = posYBlock = negXBlock = negYBlock = posXYBlock = posXnegYBlock = negXYBlock = negXposYBlock = 0;
            if ((x + 1) < 8 && !posXBlock)
            {
                if (CanCapture(board[x + 1][y]))
                {
                    capturablePieces.emplace_back(coordinate(x + 1, y));
                    legalMoves.emplace_back(coordinate(x + 1, y));
                }

                posXBlock |= (board[x + 1][y])->Type() != PieceType::empty;
                if (!posXBlock)
                    legalMoves.emplace_back(coordinate(x + 1, y));
            }
            if ((x - 1) >= 0 && !negXBlock)
            {
                if (CanCapture(board[x - 1][y]))
                {
                    capturablePieces.emplace_back(coordinate(x - 1, y));
                    legalMoves.emplace_back(coordinate(x - 1, y));
                }

                negXBlock |= (board[x - 1][y])->Type() != PieceType::empty;
                if (!negXBlock)
                    legalMoves.emplace_back(coordinate(x - 1, y));
            }
            if ((y + 1) < 8 && !posYBlock)
            {
                if (CanCapture(board[x][y + 1]))
                {
                    capturablePieces.emplace_back(coordinate(x, y + 1));
                    legalMoves.emplace_back(coordinate(x, y + 1));
                }

                posYBlock |= (board[x][y + 1])->Type() != PieceType::empty;
                if (!posYBlock)
                    legalMoves.emplace_back(coordinate(x, y + 1));
            }
            if ((y - 1) >= 0 && !negYBlock)
            {
                if (CanCapture(board[x][y - 1]))
                {
                    capturablePieces.emplace_back(coordinate(x, y - 1));
                    legalMoves.emplace_back(coordinate(x, y - 1));
                }

                negYBlock |= (board[x][y - 1])->Type() != PieceType::empty;
                if (!negYBlock)
                    legalMoves.emplace_back(coordinate(x, y - 1));
            }
            if ((x + 1) < 8 && (y + 1) < 8 && !posXYBlock)
            {
                if (CanCapture(board[x + 1][y + 1]))
                {
                    capturablePieces.emplace_back(coordinate(x + 1, y + 1));
                    legalMoves.emplace_back(coordinate(x + 1, y + 1));
                }

                posXYBlock |= (board[x + 1][y + 1])->Type() != PieceType::empty;
                if (!posXYBlock)
                    legalMoves.emplace_back(coordinate(x + 1, y + 1));
            }
            if ((x - 1) >= 0 && (y + 1) < 8 && !negXposYBlock)
            {
                if (CanCapture(board[x - 1][y + 1]))
                {
                    capturablePieces.emplace_back(coordinate(x - 1, y + 1));
                    legalMoves.emplace_back(coordinate(x - 1, y + 1));
                }

                negXposYBlock |= (board[x - 1][y + 1])->Type() != PieceType::empty;
                if (!negXposYBlock)
                    legalMoves.emplace_back(coordinate(x - 1, y + 1));
            }
            if ((x + 1) < 8 && (y - 1) >= 0 && !posXnegYBlock)
            {
                if (CanCapture(board[x + 1][y - 1]))
                {
                    capturablePieces.emplace_back(coordinate(x + 1, y - 1));
                    legalMoves.emplace_back(coordinate(x + 1, y - 1));
                }

                posXnegYBlock |= (board[x + 1][y - 1])->Type() != PieceType::empty;
                if (!posXnegYBlock)
                    legalMoves.emplace_back(coordinate(x + 1, y - 1));
            }
            if ((x - 1) >= 0 && (y - 1) >= 0 && !negXYBlock)
            {
                if (CanCapture(board[x - 1][y - 1]))
                {
                    capturablePieces.emplace_back(coordinate(x - 1, y - 1));
                    legalMoves.emplace_back(coordinate(x - 1, y - 1));
                }

                negXYBlock |= (board[x - 1][y - 1])->Type() != PieceType::empty;
                if (!negXYBlock)
                    legalMoves.emplace_back(coordinate(x - 1, y - 1));
            }
        }
        void SetType()
        {
            pieceType = PieceType::king;
            value = 0;
        }

    private:
        bool posXBlock, posYBlock, negXBlock, negYBlock, posXYBlock, posXnegYBlock, negXYBlock, negXposYBlock;
    };
}