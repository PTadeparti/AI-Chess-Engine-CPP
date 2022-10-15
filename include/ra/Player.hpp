#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include "ra/Piece.hpp"
#include "ra/enums.hpp"
#include <stdlib.h>
#include <string>

using namespace Chess;
namespace Chess
{
    using Move = std::pair<Piece *, coordinate>;
    class Player
    {
    public:
        Player(Colour colour)
        {
            this->colour = colour;
            pieces_.reserve(16);
            activePieces_.reserve(16);
            isInCheck = 0;
            capturedPieces = 0;
            activePieceCount = 16;
        }

        Player()
        {
        }

        Colour GetColour() const
        {
            return colour;
        }

        int ActivePieceCount() const
        {
            return activePieceCount;
        }

        void UpdateActivePieces()
        {
            activePieces_.clear();
            activePieceCount = 0;
            for (auto piece : pieces_)
            {
                if (!(piece->IsCaptured()))
                {
                    activePieces_.push_back(piece);
                    activePieceCount++;
                }
            }
        }

        void UpdateLegalMoves(Piece *board[8][8])
        {
            for (auto piece : pieces_)
            {
                if (!(piece->IsCaptured()))
                {
                    piece->updateLegalMoves(board);
                }
            }
        }

        static std::string getPieceTypeFromEnum(PieceType type)
        {
            switch (type)
            {
            case PieceType::queen:
                return "queen";
                break;
            case PieceType::king:
                return "king";
                break;
            case PieceType::pawn:
                return "pawn";
                break;
            case PieceType::rook:
                return "rook";
                break;
            case PieceType::bishop:
                return "bishop";
                break;
            case PieceType::knight:
                return "knight";
                break;
            default:
                return "empty";
            }
        }

        void ClearLegalMoves()
        {
            legalMoves_.clear();
        }

        void AddLegalMove(Move move)
        {
            legalMoves_.push_back(move);
        }

        std::vector<Move> LegalMoves() const
        {
            return legalMoves_;
        }

        void SetLegalMoves(std::vector<Move> moveList)
        {
            legalMoves_ = moveList;
        }

        std::vector<Piece *> Pieces() const
        {
            return pieces_;
        }

        std::vector<Piece *> ActivePieces() const
        {
            return activePieces_;
        }

        bool InCheck() const
        {
            return isInCheck;
        }

        void AddPiece(Piece *piece)
        {
            pieces_.push_back(piece);
            if (!(piece->IsCaptured()))
                activePieces_.push_back(piece);
        }

        double PositionEvaluation()
        {
            double eval = 0;
            for (auto piece : ActivePieces())
            {
                eval += (double)piece->Value();
            }
            return eval;
        }

        double PositionEvaluation(bool isAdvanced)
        {
            double eval = 0;
            for (auto piece : ActivePieces())
            {

                double multiplier = 1;
                bool centerCondition = ((piece->Type() == PieceType::pawn && ((piece->X() == 3 || piece->X() == 4) && (piece->Y() == 3 || piece->Y() == 4))) || ((piece->Type() == PieceType::knight || piece->Type() == PieceType::bishop) && ((piece->X() >= 2 || piece->X() <= 5) && (piece->Y() >= 2 || piece->Y() <= 5))));
                if (centerCondition)
                {
                    srand(time(NULL));
                    double random = ((double)(rand() % 10) / 100);
                    multiplier = 1.1 + random;
                }

                eval += (((double)piece->Value()) * multiplier);
            }
            return eval;
        }

    private:
        Colour colour;
        bool isInCheck;
        std::vector<Piece *> pieces_, activePieces_;
        std::vector<Move> legalMoves_;
        int activePieceCount;
        int capturedPieces;
    };
}

#endif