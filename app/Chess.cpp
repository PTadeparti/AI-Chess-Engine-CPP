#include "ra/Board.hpp"
using namespace Chess;

int main(int argc, char **argv)
{
    auto Board = ChessBoard((int)(*(argv[1]) - 48));
    Board.DrawChessBoard(&argc, argv);    
}