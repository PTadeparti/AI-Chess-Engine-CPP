#include <iostream>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <stdlib.h>
#include "ra/enums.hpp"
#include "ra/Piece.hpp"
#include "ra/Player.hpp"
#include "ra/Pawn.hpp"
#include "ra/Rook.hpp"
#include "ra/Knight.hpp"
#include "ra/Bishop.hpp"
#include "ra/Queen.hpp"
#include "ra/King.hpp"
#include "ra/DrawUtils.hpp"

#define EMPTY 0
#define P 1
#define R 2
#define KN 3
#define B 4
#define Q 5
#define K 6
#define windowHeight 700
#define windowWidth 1500

namespace Chess
{
    class ChessBoard
    {
    public:
        const int scaledSquareSize = squareSize * 1500 / 900; // using GLUTOrtho2D scaled positions
        Piece *chessBoard[8][8];
        Piece *selectedPiece;
        Empty *emptySquares[8][8]; // Board of empty squares aids in drawing and identifying identifying open squares
        bool isPieceSelected = false;
        bool isWhitesTurn = true;
        bool isWhiteInCheck, isBlackInCheck;
        bool gameOver = false;
        Player WhitePlayer;
        Player BlackPlayer;
        King *whiteKing; // Have set objects for kings for easier access to draw methods when in check
        King *blackKing;

        ChessBoard(int difficulty)
        {
            this->difficulty = difficulty;
            InitializeBoard();
        }

        void InitializeBoard()
        {
            // Preset position for board
            int initialBoard[8][8] = {{R, KN, B, Q, K, B, KN, R},
                                      {P, P, P, P, P, P, P, P},
                                      {0, 0, 0, 0, 0, 0, 0, 0},
                                      {0, 0, 0, 0, 0, 0, 0, 0},
                                      {0, 0, 0, 0, 0, 0, 0, 0},
                                      {0, 0, 0, 0, 0, 0, 0, 0},
                                      {-P, -P, -P, -P, -P, -P, -P, -P},
                                      {-R, -KN, -B, -Q, -K, -B, -KN, -R}};

            whiteKing = new King(4, 0, Colour::white);
            blackKing = new King(4, 7, Colour::black);
            WhitePlayer = Player(Colour::white);
            BlackPlayer = Player(Colour::black);
            for (int y = 0; y < 8; y++)
            {
                for (int x = 0; x < 8; x++)
                {
                    int pieceKey = initialBoard[y][x];
                    emptySquares[x][y] = new Empty(x, y, Colour::na);
                    Colour c = pieceKey < 0 ? Colour::black : Colour::white;
                    switch (abs(pieceKey))
                    {
                    case R:
                        chessBoard[x][y] = new Rook(x, y, c);
                        break;
                    case KN:
                        chessBoard[x][y] = new Knight(x, y, c);
                        break;
                    case B:
                        chessBoard[x][y] = new Bishop(x, y, c);
                        break;
                    case Q:
                        chessBoard[x][y] = new Queen(x, y, c);
                        break;
                    case K:
                        chessBoard[x][y] = c == Colour::white ? whiteKing : blackKing;
                        break;
                    case P:
                        chessBoard[x][y] = new Pawn(x, y, c);
                        break;
                    case EMPTY:
                        chessBoard[x][y] = emptySquares[x][y];
                        break;
                    default:
                        chessBoard[x][y] = emptySquares[x][y];
                    }
                    chessBoard[x][y]->SetType();
                    if (chessBoard[x][y]->Type() != PieceType::empty)
                    {
                        chessBoard[x][y]->GetColour() == Colour::white ? WhitePlayer.AddPiece(chessBoard[x][y]) : BlackPlayer.AddPiece(chessBoard[x][y]);
                    }
                }
            }
            gameOver = isWhiteInCheck = isBlackInCheck = false;
            isWhitesTurn = true;
            updateBoard();
        }

        // Delete objects created by new to maintain empty board that can setup for new game
        void Cleanup()
        {
            for (int y = 0; y < 8; y++)
            {
                for (int x = 0; x < 8; x++)
                {
                    delete chessBoard[x][y];
                }
            }
        }

        void updateBoard()
        {
            for (int file = 0; file < 8; file++)
            {
                for (int rank = 0; rank < 8; rank++)
                {
                    if (chessBoard[file][rank] != nullptr)
                    {
                        chessBoard[file][rank]->updateLegalMoves(chessBoard);
                        chessBoard[file][rank]->Draw();
                    }
                }
            }

            if (gameOver)
            {
                char playAgain;
                std::cout << "Play again? (y/n)" << std::endl;
                std::cin >> playAgain;
                if (playAgain == 'y')
                {
                    Cleanup();
                    InitializeBoard();
                    display();
                }
                else
                {
                    Cleanup();
                    glutDestroyWindow(glutGetWindow());
                    return;
                }
            }
            else
            {

                updateCapturedPieces();

                if (isWhitesTurn)
                {
                    UpdateAllLegalMoves(Colour::white);
                }
                else
                {
                    UpdateAllLegalMoves(Colour::black);
                }

                if (isStalemate())
                {
                    gameOver = true;
                }

                if (!isWhitesTurn)
                {
                    PlayEngineMove(Colour::black, difficulty);
                }
            }
        }

        void UpdateAllLegalMoves(Colour col)
        {

            Player *player = col == Colour::white ? &WhitePlayer : &BlackPlayer;
            player->UpdateActivePieces();
            player->ClearLegalMoves();
            Piece *previousPiece;

            for (auto piece : player->ActivePieces())
            {
                piece->updateLegalMoves(chessBoard);
                int prevX = piece->X();
                int prevY = piece->Y();
                for (auto destination : piece->LegalMoves())
                {
                    auto move = Move(piece, destination);
                    previousPiece = chessBoard[destination.first][destination.second];
                    MakeComputerMove(move, false);

                    if (!IsInCheck(col, false))
                    {
                        player->AddLegalMove(move);
                    }
                    UndoMove(move, Move(previousPiece, coordinate(prevX, prevY)));
                }
            }
        }

        void updateCapturedPieces()
        {

            for (auto piece : BlackPlayer.Pieces())
            {
                if (piece->IsCaptured() && !(piece->IsCaptureDrawn()))
                {
                    piece->SetLocation(9 + ((16 - BlackPlayer.ActivePieceCount()) % 5), (16 - BlackPlayer.ActivePieceCount()) / 5);
                    piece->Draw();
                    piece->SetCaptureDrawn();
                }
            }

            for (auto piece : WhitePlayer.Pieces())
            {
                if (piece->IsCaptured() && !(piece->IsCaptureDrawn()))
                {
                    piece->SetLocation(9 + ((16 - WhitePlayer.ActivePieceCount()) % 5), (7 - (16 - WhitePlayer.ActivePieceCount()) / 5));
                    piece->Draw();
                    piece->SetCaptureDrawn();
                }
            }
        }

        bool IsInCheck(Colour col, bool checkForMate)
        {
            col == Colour::white ? BlackPlayer.UpdateActivePieces() : WhitePlayer.UpdateActivePieces();
            auto PieceList = col == Colour::white ? BlackPlayer.ActivePieces() : WhitePlayer.ActivePieces();
            bool isInCheck = false;

            for (auto piece : PieceList)
            {
                piece->updateLegalMoves(chessBoard);
                for (auto move : piece->CapturablePieceLocations())
                {
                    if (chessBoard[move.first][move.second]->Type() == PieceType::king)
                    {
                        isInCheck = true;
                    }
                }
            }

            if (isInCheck && checkForMate)
            {
                if (isCheckmate(col))
                {
                    std::cout << "CHECKMATE!!!!" << std::endl;
                    col == Colour::white ? whiteKing->SetInCheck(true) : blackKing->SetInCheck(true);
                    col == Colour::white ? drawCheckMark(420, 380) : drawCheckMark(420, 30);
                    gameOver = true;
                }
                return isInCheck;
            }

            col == Colour::white ? isWhiteInCheck = isInCheck : isBlackInCheck = isInCheck;
            return isInCheck;
        }

        bool isCheckmate(Colour col)
        {
            UpdateAllLegalMoves(col);
            Player player = col == Colour::white ? WhitePlayer : BlackPlayer;
            bool isCheckmate = (player.LegalMoves().size() < 1) ? true : false;
            return isCheckmate;
        }

        bool isStalemate()
        {
            BlackPlayer.UpdateActivePieces();
            WhitePlayer.UpdateActivePieces();
            return ((WhitePlayer.ActivePieceCount() <= 1) && (BlackPlayer.ActivePieceCount() <= 1));
        }

        // Engine makes several moves and needs to undo them
        // moveToUndo represents specific piece and destination square of move to undo
        // previousInfo represents piece initially on destination square, and initial square of moveToUbndo piece.
        void UndoMove(Move moveToUndo, Move previousInfo)
        {
            if (moveToUndo.first == nullptr || previousInfo.first == nullptr || gameOver)
                return;

            moveToUndo.first->SetX(previousInfo.second.first);
            moveToUndo.first->SetY(previousInfo.second.second);
            chessBoard[previousInfo.second.first][previousInfo.second.second] = moveToUndo.first;
            chessBoard[moveToUndo.second.first][moveToUndo.second.second] = previousInfo.first;

            if (previousInfo.first->IsCaptured())
            {
                previousInfo.first->Uncapture(moveToUndo.second.first, moveToUndo.second.second, chessBoard, false);
            }
            moveToUndo.first->updateLegalMoves(chessBoard);
        }

        // Makes move without checking for legality, since this method is called only with moves from the legal moves vector
        void MakeComputerMove(Move move, bool updateBoardDrawing)
        {
            if (move.first == nullptr || gameOver)
                return;

            auto pieceToMove = move.first;
            int prevX = pieceToMove->X();
            int prevY = pieceToMove->Y();
            int nextX = move.second.first;
            int nextY = move.second.second;
            Piece *previousPiece = chessBoard[nextX][nextY];
            bool success = pieceToMove->Move(nextX, nextY, chessBoard, false);
            if (success)
            {
                chessBoard[nextX][nextY] = pieceToMove;
                chessBoard[prevX][prevY] = emptySquares[prevX][prevY];

                if (updateBoardDrawing)
                {
                    updateBoard();
                }
            }
        }

        // Check legal move list for desired move
        bool ValidateUserMove(Move move)
        {

            Player *player = move.first->GetColour() == Colour::white ? &WhitePlayer : &BlackPlayer;

            for (auto legalMove : player->LegalMoves())
            {
                if (move.first == legalMove.first && move.second == legalMove.second)
                {
                    return true;
                }
            }

            return false;
        }

        // Make move with legality checking, since user can click any square
        void MakeUserMove(Move move, bool updateBoardDrawing)
        {
            if (gameOver || move.first == nullptr)
                return;

            bool isMoveValid = ValidateUserMove(move);
            bool isWhiteInCheck, isBlackInCheck;
            if (isMoveValid)
            {
                Piece *piece = move.first;
                coordinate c = move.second;
                int prevX = piece->X();
                int prevY = piece->Y();

                piece->Move(c.first, c.second, chessBoard, updateBoardDrawing);
                chessBoard[c.first][c.second] = piece;
                chessBoard[prevX][prevY] = emptySquares[prevX][prevY];
                isWhitesTurn = !isWhitesTurn;
            }

            isWhiteInCheck = IsInCheck(Colour::white, true);
            isBlackInCheck = IsInCheck(Colour::black, true);

            if (updateBoardDrawing)
            {
                if (move.first == nullptr)
                    return;

                move.first->SetIsSelected(false);
                isPieceSelected = false;
                isWhiteInCheck ? whiteKing->SetInCheck(true) : whiteKing->SetInCheck(false);
                isBlackInCheck ? blackKing->SetInCheck(true) : blackKing->SetInCheck(false);
                updateBoard();
            }
        }

        void HandlePiecePickup(int mouseX, int mouseY)
        {
            if (gameOver)
                return;

            for (int x = 0; x < 8; x++)
            {
                for (int y = 0; y < 8; y++)
                {
                    chessBoard[x][y]->SetIsSelected(false);
                    if ((mouseX > (x * scaledSquareSize)) && (mouseX < (x * scaledSquareSize + scaledSquareSize)) && (mouseY > (y * scaledSquareSize)) && (mouseY < (y * scaledSquareSize + scaledSquareSize)))
                    {
                        if ((isWhitesTurn && chessBoard[x][y]->GetColour() == Colour::white) || (!isWhitesTurn && chessBoard[x][y]->GetColour() == Colour::black))
                        {
                            isPieceSelected = true;
                            chessBoard[x][y]->SetIsSelected(true);
                            selectedPiece = chessBoard[x][y];
                            chessBoard[x][y]->Draw();
                        }
                    }
                }
            }
        }

        void HandlePieceDrop(int mouseX, int mouseY)
        {

            if (!isPieceSelected || gameOver)
                return;

            for (int x = 0; x < 8; x++)
            {
                for (int y = 0; y < 8; y++)
                {
                    if ((mouseX > (x * scaledSquareSize)) && (mouseX < (x * scaledSquareSize + scaledSquareSize)) && (mouseY > (y * scaledSquareSize)) && (mouseY < (y * scaledSquareSize + scaledSquareSize)))
                    {
                        MakeUserMove(Move(selectedPiece, coordinate(x, y)), true);
                        return;
                    }
                }
            }
        }

        void PlayEngineMove(Colour col, int level)
        {
            UpdateAllLegalMoves(col);
            Move move;
            switch (level)
            {
            case 0:
                move = GenerateRandomMove(col);
                break;
            case 1:
                move = GenerateMoveFromMinimax(col, 2, false);
                break;
            case 2:
                move = GenerateMoveFromMinimax(col, 2, true);
                break;
            case 3:
                move = GenerateMoveFromMinimax(col, 3, true);
                break;
            case 4:
                move = GenerateMoveFromMinimax(col, 4, true);
                break;
            default:
                move = GenerateRandomMove(col);
            }

            WhitePlayer.UpdateActivePieces();
            BlackPlayer.UpdateActivePieces();
            UpdateAllLegalMoves(col);
            MakeUserMove(move, true);
        }

        Move GenerateRandomMove(Colour col)
        {
            Player *player;
            col == Colour::white ? player = &WhitePlayer : player = &BlackPlayer;
            srand(time(NULL));
            int randMoveIndex = std::rand() % player->LegalMoves().size();
            Move randMove = player->LegalMoves()[randMoveIndex];
            Piece *p = randMove.first;
            coordinate c = randMove.second;
            return Move(p, c);
        }

        Move GenerateMoveFromMinimax(Colour col, int depth, bool isAdvancedEval)
        {

            Player *player;
            Player *opponent;
            col == Colour::white ? player = &WhitePlayer : player = &BlackPlayer;
            int isMin = col == Colour::black ? -1 : 1; // Colour denotes whether to minimize or maximize - black = min, white = max
            Colour nextColour = col == Colour::white ? Colour::black : Colour ::white;
            double highestEval = -999 * isMin; // start with worst case eval
            Move bestMove;
            auto legalMoves = player->LegalMoves();

            // Look at every possible move and call minimax to check final evaluation
            for (auto move : legalMoves)
            {
                int prevX = move.first->X();
                int prevY = move.first->Y();
                Piece *previousPiece = chessBoard[move.second.first][move.second.second];
                MakeComputerMove(move, false);
                double eval = runMinimax(nextColour, depth - 1, 999 * isMin, -999 * isMin, isAdvancedEval); // Initially alpha is best value, beta is worst

                if (((isMin * eval) > (isMin * highestEval)))
                {
                    highestEval = eval;
                    bestMove = move;
                }
                UndoMove(move, Move(previousPiece, coordinate(prevX, prevY)));
            }

            return bestMove;
        }

        double runMinimax(Colour col, int depth, double alpha, double beta, bool isAdvancedEval)
        {
            if (depth < 1)
            {
                WhitePlayer.UpdateActivePieces();
                BlackPlayer.UpdateActivePieces();
                if (isAdvancedEval)
                {
                    return Evaluation(true);
                }
                else
                {
                    return Evaluation(false);
                }
            }

            Player *player;
            col == Colour::white ? player = &WhitePlayer : player = &BlackPlayer;
            Colour nextColour = col == Colour::white ? Colour::black : Colour ::white;
            int isMin = col == Colour::black ? -1 : 1;
            UpdateAllLegalMoves(col);
            auto legalMoves = player->LegalMoves();

            // Checkmate is worst possible outcome
            if (isCheckmate(col))
            {
                return -999 * isMin;
            }

            // Stalemate is purely equal
            if (isStalemate())
            {
                return 0;
            }

            for (auto move : legalMoves)
            {
                int prevX = move.first->X();
                int prevY = move.first->Y();
                Piece *previousPiece = chessBoard[move.second.first][move.second.second];
                MakeComputerMove(move, false);

                double eval = runMinimax(nextColour, depth - 1, beta, alpha, isAdvancedEval); // Check for opponents best eval, swap beta and alpha
                if ((isMin * eval) >= (isMin * beta))
                {
                    UndoMove(move, Move(previousPiece, coordinate(prevX, prevY)));
                    return beta;
                }
                if (((isMin * eval) > (isMin * alpha)))
                {
                    alpha = eval;
                }

                UndoMove(move, Move(previousPiece, coordinate(prevX, prevY)));
            }

            return alpha;
        }

        double Evaluation()
        {
            double evaluation;
            evaluation = WhitePlayer.PositionEvaluation() - BlackPlayer.PositionEvaluation();
            return evaluation;
        }

        double Evaluation(bool isAdvanced)
        {
            double evaluation;
            evaluation = WhitePlayer.PositionEvaluation(true) - BlackPlayer.PositionEvaluation(true);
            return evaluation;
        }

        void mouse(int button, int state, int mouseX, int mouseY)
        {
            if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
            {
                mouseY = windowHeight - mouseY;
                HandlePiecePickup(mouseX, mouseY);
            }

            if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN && isPieceSelected)
            {
                mouseY = windowHeight - mouseY;
                HandlePieceDrop(mouseX, mouseY);
            }
        }

        // Use callbacks since GLUT Mouse and Draw functions need to be static.
        // These callbacks allow parameters from the board object to be used
        static void mouseCallback(int button, int state, int mouseX, int mouseY)
        {
            currentBoard->mouse(button, state, mouseX, mouseY);
        }

        static void drawCallback()
        {
            currentBoard->display();
        }

        void DrawChessBoard(int *argcp, char **argv)
        {

            glutInit(argcp, argv);
            glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

            glutInitWindowPosition(300, 300);
            glutInitWindowSize(windowWidth, windowHeight);
            glutCreateWindow("Chess");

            glClearColor(1.0, 1.0, 1.0, 0.0);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            gluOrtho2D(0.0, 900.0, 0.0, 420.0);

            currentBoard = this;
            glutDisplayFunc(drawCallback);
            glutMouseFunc(mouseCallback);
            glutMainLoop();
        }

        void display(void)
        {
            int xCoordinate = 0, yCoordinate = 0;
            glClear(GL_COLOR_BUFFER_BIT);
            glPointSize(1.0);
            glColor3f(0.23, 0.22, 0.44);

            glBegin(GL_POLYGON);
            glColor3f(0.65, 0.65, 0.65);
            glVertex2i(0, 0);
            glVertex2i(0, 750);
            glVertex2i(1500, 750);
            glVertex2i(1500, 0);
            glEnd();

            for (int rank = 0; rank <= 7; ++rank)
            {

                for (int file = 0; file <= 7; ++file)
                {
                    drawEmptySquare(file, rank);
                    chessBoard[file][rank]->Draw();
                    xCoordinate += 1;
                }
                yCoordinate += 1;
                xCoordinate = 0;
            }

            glFlush();
        }

    private:
        inline static ChessBoard *currentBoard;
        int difficulty;
    };
}
