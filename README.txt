README

Variable $INSTALL_DIR should be created to denote the directory where the software will be installed
Variable $TOP_DIR should be created to denote the directory in which this README file is contained

To build and install the software run the commands:

cd $TOP_DIR
cmake -H. -Btemp -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR
cmake --build temp --clean-first --target install


A demo script has been provided to run the chess program at a level 3 difficulty.
To run the demo script, run the command:

$INSTALL_DIR/bin/demo

Note that even with the demo script, the user must interact with the software to observe the program, as chess is an interactive 2 player game.

To run the software manually, run the command:
$INSTALL_DIR/bin/chess n     where n is an integer between 0 and 4. n denotes the difficulty of the chess opponent. 4 is maximum difficulty.


To Play the game:
Left click an active piece on the board, then right click on a legal destination square. The game will continue until a checkmate or stalemate.
Once a game is over, there is an option in the command line to play again (y/n). If y, a new game is started. If any other value typed into the
command line, the program will close.

