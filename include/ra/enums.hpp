#ifndef ENUMS
#define ENUMS
namespace Chess
{
    enum Colour
    {
        white = 0,
        black = 1,
        na = 2
    };

    enum PieceType
    {
        empty = 0,
        pawn = 1,
        rook = 2,
        knight = 3,
        bishop = 4,
        queen = 5,
        king = 6
    };  
	
	enum MoveResult
    {
        success = 0,
        capture = 1,
        fail = 3
    };
}
#endif