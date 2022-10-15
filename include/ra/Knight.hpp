#include "ra/Piece.hpp"
namespace Chess
{
    class Knight : public Piece
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
            drawRectangle(x_ + 10, y_ + 10, x_ + 18, y_ + 35);
            drawRectangle(x_ + 10, y_ + 35, x_ + 35, y_ + 40);
            drawTriangle(x_ + 25, y_ + 40, x_ + 40, y_ + 20);
            glFlush();
        }

        void updateLegalMoves(Piece *board[8][8])
        {
            legalMoves.clear();
            capturablePieces.clear();
            if ((x + 1) < 8)
            {
                if ((y + 2) < 8 && ((board[x + 1][y + 2])->Type() == PieceType::empty || CanCapture(board[x + 1][y + 2])))
                {
                    if (CanCapture(board[x + 1][y + 2]))
                        capturablePieces.emplace_back(coordinate(x + 1, y + 2));
                    legalMoves.emplace_back(coordinate(x + 1, y + 2));
                }
                if ((y - 2) >= 0 && ((board[x + 1][y - 2])->Type() == PieceType::empty || CanCapture(board[x + 1][y - 2])))
                {
                    if (CanCapture(board[x + 1][y - 2]))
                        capturablePieces.emplace_back(coordinate(x + 1, y - 2));
                    legalMoves.emplace_back(coordinate(x + 1, y - 2));
                }
            }
            if ((x - 1) >= 0)
            {
                if ((y + 2) < 8 && ((board[x - 1][y + 2])->Type() == PieceType::empty || CanCapture(board[x - 1][y + 2])))
                {
                    if (CanCapture(board[x - 1][y + 2]))
                        capturablePieces.emplace_back(coordinate(x - 1, y + 2));
                    legalMoves.emplace_back(coordinate(x - 1, y + 2));
                }
                if ((y - 2) >= 0 && ((board[x - 1][y - 2])->Type() == PieceType::empty || CanCapture(board[x - 1][y - 2])))
                {
                    if (CanCapture(board[x - 1][y - 2]))
                        capturablePieces.emplace_back(coordinate(x - 1, y - 2));
                    legalMoves.emplace_back(coordinate(x - 1, y - 2));
                }
            }
            if ((y + 1) < 8)
            {
                if ((x + 2) < 8 && ((board[x + 2][y + 1])->Type() == PieceType::empty || CanCapture(board[x + 2][y + 1])))
                {
                    if (CanCapture(board[x + 2][y + 1]))
                        capturablePieces.emplace_back(coordinate(x + 2, y + 1));
                    legalMoves.emplace_back(coordinate(x + 2, y + 1));
                }
                if ((x - 2) >= 0 && ((board[x - 2][y + 1])->Type() == PieceType::empty || CanCapture(board[x - 2][y + 1])))
                {
                    if (CanCapture(board[x - 2][y + 1]))
                        capturablePieces.emplace_back(coordinate(x - 2, y + 1));
                    legalMoves.emplace_back(coordinate(x - 2, y + 1));
                }
            }
            if ((y - 1) >= 0)
            {
                if ((x + 2) < 8 && ((board[x + 2][y - 1])->Type() == PieceType::empty || CanCapture(board[x + 2][y - 1])))
                {
                    if (CanCapture(board[x + 2][y - 1]))
                        capturablePieces.emplace_back(coordinate(x + 2, y - 1));
                    legalMoves.emplace_back(coordinate(x + 2, y - 1));
                }
                if ((x - 2) >= 0 && ((board[x - 2][y - 1])->Type() == PieceType::empty || CanCapture(board[x - 2][y - 1])))
                {
                    if (CanCapture(board[x - 2][y - 1]))
                        capturablePieces.emplace_back(coordinate(x - 2, y - 1));
                    legalMoves.emplace_back(coordinate(x - 2, y - 1));
                }
            }
        }
        void SetType()
        {
            value = 3;
            pieceType = PieceType::knight;
        }

    private:
    };
}