#ifndef PIECE_H
#define PIECE_H

#include <vector>
#include <thread>
#include "ra/enums.hpp"
#include "ra/DrawUtils.hpp"
#define squareSize 50
using namespace Chess;

namespace Chess
{
    using coordinate = std::pair<int, int>;

    class Piece
    {

    public:
        virtual void Draw() = 0;

        virtual void SetType() = 0;
        virtual void updateLegalMoves(Piece *board[8][8]) = 0;
        Piece(int x, int y, Colour colour);

        Piece();

        bool Move(int, int, Piece *[8][8], bool);

        int GetColour() const;

        void SetColour(Colour value);

        int X() const;

        void SetX(int value);

        int Y() const;

        void SetY(int value);

        bool IsSelected() const;

        std::vector<coordinate> LegalMoves() const;

        std::vector<coordinate> CapturablePieceLocations() const;

        void Capture(bool);

        void Uncapture(int x, int y, Piece *board[8][8], bool);

        bool IsCaptured() const;

        bool IsCaptureDrawn() const;

        void SetCaptureDrawn();

        void SetIsSelected(int value);

        PieceType Type() const;

        bool CanCapture(const Piece *otherPiece) const;

        void SetLocation(int x, int y);

        void SetInCheck(bool value);

        bool InCheck() const;

        int Value() const;

    protected:
        Colour colour;
        PieceType pieceType;
        int x, y, value;
        bool isSelected, isCaptured, isCaptureDrawn, isInCheck;
        std::vector<coordinate> legalMoves;
        std::vector<coordinate> capturablePieces;
    };

    class Empty : public Piece
    {
    public:
        using Piece::Piece;

        void Draw();

        bool Move(int xNext, int yNext, Piece *board[8][8]);

        void SetType();

        void updateLegalMoves(Piece *board[8][8]);

    private:
    };

}

#endif
