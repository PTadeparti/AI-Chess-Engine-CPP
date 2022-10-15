#include "ra/Piece.hpp"

namespace Chess
{
    class Queen : public Piece
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
            drawRectangle(x_ + 8, y_ + 10, x_ + 42, y_ + 30);
            drawTriangle(x_ + 10, y_ + 30, x_ + 20, y_ + 43);
            drawTriangle(x_ + 20, y_ + 30, x_ + 30, y_ + 43);
            drawTriangle(x_ + 30, y_ + 30, x_ + 40, y_ + 43);
            glFlush();
        }

        void updateLegalMoves(Piece *board[8][8])
        {

            legalMoves.clear();
            capturablePieces.clear();
            posXBlock = posYBlock = negXBlock = negYBlock = posXYBlock = posXnegYBlock = negXYBlock = negXposYBlock = 0;
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
            value = 9;
            pieceType = PieceType::queen;
        }

    private:
        bool posXBlock, posYBlock, negXBlock, negYBlock, posXYBlock, posXnegYBlock, negXYBlock, negXposYBlock;
    };

}