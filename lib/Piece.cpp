#include <GL/glut.h>
#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <thread>
#include "ra/Piece.hpp"
using namespace Chess;
namespace Chess
{
    using coordinate = std::pair<int, int>;

    Piece::Piece(int x, int y, Colour colour)
    {
        this->x = x;
        this->y = y;
        this->colour = colour;
        isCaptured = 0;
        isCaptureDrawn = 0;
        isSelected = false;
        pieceType = PieceType::empty;
        legalMoves.reserve(21);
        capturablePieces.reserve(8);
    }

    Piece::Piece()
    {
        colour = Colour::white;
    }

    int Piece::GetColour() const
    {
        return colour;
    }

    void Piece::SetColour(Colour value)
    {
        colour = value;
    }

    int Piece::X() const
    {
        return x;
    }

    void Piece::SetX(int value)
    {
        x = value;
    }

    int Piece::Y() const
    {
        return y;
    }

    void Piece::SetY(int value)
    {
        y = value;
    }

    bool Piece::IsSelected() const
    {
        return isSelected;
    }

    void Piece::Capture(bool draw)
    {
        isCaptured = 1;
        capturablePieces.clear();

        if (draw)
            drawEmptySquare(x, y);
    }

    void Piece::Uncapture(int x, int y, Piece *board[8][8], bool draw)
    {
        isCaptured = 0;
        updateLegalMoves(board);
        drawEmptySquare(x, y);
        SetX(x);
        SetY(y);

        if (draw)
            Draw();
    }

    bool Piece::IsCaptured() const
    {
        return isCaptured;
    }

    std::vector<coordinate> Piece::LegalMoves() const
    {
        return legalMoves;
    }

    bool Piece::IsCaptureDrawn() const
    {
        return isCaptureDrawn;
    }

    bool Piece::Move(int xNext, int yNext, Piece *board[8][8], bool draw)
    {
        updateLegalMoves(board);
        if (std::find(legalMoves.begin(), legalMoves.end(), coordinate(xNext, yNext)) == legalMoves.end())
        {
            return false;
        }
        if (std::find(capturablePieces.begin(), capturablePieces.end(), coordinate(xNext, yNext)) != capturablePieces.end())
        {
            board[xNext][yNext]->Capture(draw);
        }

        SetX(xNext);
        SetY(yNext);
        return true;
    }

    void Piece::SetCaptureDrawn()
    {
        isCaptureDrawn = true;
    }

    void Piece::SetIsSelected(int value)
    {
        isSelected = value;
        Draw();
    }

    PieceType Piece::Type() const
    {
        return pieceType;
    }

    int Piece::Value() const
    {
        return value;
    }

    std::vector<coordinate> Piece::CapturablePieceLocations() const
    {
        return capturablePieces;
    }

    bool Piece::CanCapture(const Piece *otherPiece) const
    {
        return (otherPiece->Type() != PieceType::empty) && (colour != otherPiece->GetColour());
    }

    void Piece::SetLocation(int x, int y)
    {
        this->x = x;
        this->y = y;
    }

    void Piece::SetInCheck(bool value)
    {
        isInCheck = value;
        Draw();
    }

    bool Piece::InCheck() const
    {
        return isInCheck;
    }

    void Empty::Draw()
    {
        drawEmptySquare(x, y);
    }

    bool Empty::Move(int xNext, int yNext, Piece *board[8][8])
    {
        return MoveResult::fail;
    }
    void Empty::SetType()
    {
        pieceType = PieceType::empty;
    }

    void Empty::updateLegalMoves(Piece *board[8][8])
    {
        return;
    }   
}
