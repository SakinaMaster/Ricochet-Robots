/** ---------------------------------------------
* RicoRobots.cpp
*      This version stores moves on a linked list, allowing
*      infinite undo back to the beginning of the game.

* Board display looks like:

        Move robot 2 to square M
        |- - - - - - - - - - - - - - - -|
        |. . . . .|. . . . . .|. . . . .|
        |                              |
        |. . .|. . . . . .|. . . . . . .|
        |    -            -        -  |
        |. . . . . . . . . . . . . . .|.|
        |                              |
        |.|. . . . . . . . . . . . . . .|
        |  -        -                  |
        |. . . . . .|. . . . .|. . . . .|
        |-                  -        -|
        |. . . . . . . . . . . . . . . 4|
        |          -            -      |
        |. . . . . .|. . . . . .|. . . .|
        |              - -              |
        |. . . .|. . .|. .|. . . . . . .|
        |      -                        |
        |. . . . . . .|. .|. . . . . . .|
        |  -      -  - -              |
        |. .|. . .|. . . . . . . . . . .|
        |                              -|
        |. . . . . . . . .|. . . . . . .|
        |-              -        -    |
        |. . . . . . . . . . . . .|3 . .|
        |                              |
        |. . . . . . M|. . . . . . . . .|
        |            -                  |
        |. . . . . . . . .|. . . . . . .|
        |                  -        -  |
        |. .|. . . . . . . . . . . . 1|.|
        |    -                          |
        |. . . . . .|. . . . . .|. . . 2|
        |- - - - - - - - - - - - - - - -|

        1. Please enter the robot to move and the direction (e.g. 2 r): 
* 
* -----------------------------------------------------------------------------------
*/

#include <iostream>    // for cin and cout
#include <cstring>     // for getline
#include <cstdlib>     // for the system command
#include <cctype>      // For the letter checking functions, e.g. toupper( )
#include <fstream>     // For file input and output
using namespace std;

// Global data structure for board
const int BOARD_EDGE = 16;
const int BOARD_SIZE = BOARD_EDGE * BOARD_EDGE;
// theBoard itself is represented by a one-dimensional array. 
// The other entries are used to keep track of walls.  Each row
// in theBoard array will contain the character to be displayed,
// followed by any "walls" to be displayed around the displayed
// character.  For instance the first couple of entries are be:
//
//      PIECE  LEFT ABOVE RIGHT BELOW
//      ---0---  --1-  --2-  --3-  --4-
//    0    .      |    _           
//    1    .            _
//  ...
//  255  .                  |    _
//
char theBoard[ BOARD_SIZE][ 5];        // global array of board display characters
// declare constants to later use instead of numbers to select values from theBoard
const int PIECE = 0;    // to reference the character, such as in: theBoard[ i][ PIECE]
const int LEFT = 1;     // to reference the left wall character, such as in: theBoard[ i][ left]
const int ABOVE = 2;    // to reference the above wall character, such as in: theBoard[ i][ above]
const int RIGHT = 3;    // to reference the right wall character, such as in: theBoard[ i][ right]
const int BELOW = 4;    // to reference the below wall character, such as in: theBoard[ i][ below]

// declare the 4 robots, which will store the board position of where they are found
// Robots are displayed using values 1-4.  We declare 5 robots, though we will never use
// the 0th robot in the array.
const int NUMBER_OF_ROBOTS = 4;
int theRobots[ NUMBER_OF_ROBOTS + 1] = {0,0,0,0,0};

// Declare node structure for linked list
struct Node {
      char theBoard[ BOARD_SIZE][ 5];        // copy of array of board display characters
      int theRobots[ NUMBER_OF_ROBOTS + 1];  // copy of robots locations
      int moveNumber;                        // copy of move number
      Node *pNext;                          // pointer to next node
};

//-------------------------------------------------------------------------------------
// createBoard() - Create the board, initializing all the display characters
//     
void createBoard()
{
    for( int i=0; i<BOARD_SIZE; i++) {
        theBoard[ i][ PIECE]= '.';    // Set each playing piece default
        // set the default wall values to be blank
        theBoard[ i][ LEFT]= ' ';
        theBoard[ i][ ABOVE]= ' '; 
        theBoard[ i][ RIGHT]= ' '; 
        theBoard[ i][ BELOW]=  ' ';

        // reset the edge pieces
        // reset the left wall if piece is on left edge
        if( (i % BOARD_EDGE) == 0) {
            theBoard[ i][ LEFT]= '|';       
        }
        // reset the above wall if piece is on top edge
        if( i < BOARD_EDGE) {
            theBoard[ i][ ABOVE]= '-';       
        }
        // reset the right wall if piece is on right edge
        if( ((i+1) % BOARD_EDGE) == 0) {
            theBoard[ i][ RIGHT]= '|';       
        }
        // reset the below wall if piece is on bottom edge
        if( i >= (BOARD_SIZE - BOARD_EDGE) ) {
            theBoard[ i][ BELOW]= '-';       
        }
    }//end for( int i...

}//end createBoard()


//-------------------------------------------------------------------------------------
// readFromFileAndModifyBoard() - Read from data file, updating board accordingly
//         
void readFromFileAndModifyBoard(int &numberOfDestinationPieces)
{
  int numberOfSpecialSquares;    // read from file, denotes number of pieces with some kind of wall

  ifstream inStream;             // declare an input file stream
  // open input file
  inStream.open("board.txt");   // associate the actual file name with "inStream"
  if ( inStream.fail() ) {
      cout << "Input file opening failed.  Exiting...\n\n";
      exit(-1);
  }

  // read from file
  inStream >> numberOfDestinationPieces;      // read how many destination pieces there are
  inStream >> numberOfSpecialSquares;        // number of square with walls around them somewhere
  int pieceNumber;                           // pieceNumber for each square defined in data file
  char left, above, right, below;            // variables to store possible walls indicators
  char letterToDisplay[ 2];                  // array to store possible letter to be displayed

  // process the special squares, updating the board
  for( int i=0; i<numberOfSpecialSquares; i++) {
        // read the piece number
        inStream >> pieceNumber;
        // read the information on whether or not there is each of the potential 4 walls
        inStream >> left >> above >> right >> below;
        // store the corresponding walls
        if( left=='1') {
            theBoard[ pieceNumber][ LEFT] = '|';
        }   
        if( above=='1') {
            theBoard[ pieceNumber][ ABOVE] = '-';
        }
        if( right=='1') {
            theBoard[ pieceNumber][ RIGHT] = '|';
        }
        if( below=='1') {
            theBoard[ pieceNumber][ BELOW] = '-';
        }
        // read either nothing else on the line, or the space and the letter to be displayed
        inStream.getline( letterToDisplay, '\n');
        if( strlen( letterToDisplay) > 0) {
            // place the input character on the board if there was one
            theBoard[ pieceNumber][ PIECE] = letterToDisplay[ 1];   
        }
  }//end for( int i... 

  // process the 4 robots, updating the board
  char junk[80];          // used to read and discard input file information
  // Loop starts counting from 1 (rather than 0), since we ignore the 0th position
  // of the robots array, so that robot numbers correspond to what is shown
  // on the board.
  for( int i=1; i<=NUMBER_OF_ROBOTS; i++) {
        inStream >> theRobots[ i];
        inStream.getline( junk, '\n');    // discard rest of line
        // set robot on board, converting the integer value to a character
        theBoard[ theRobots[ i]][ PIECE] = (char)('0'+i);    // record the robot number in place on board
  }

  inStream.close();        // close the input file stream 

}//end readFromFileAndModifyBoard()


//-------------------------------------------------------------------------------------
// chooseRandomRobotAndDestination() - choose a robot at random, and a destination at random
//
void chooseRandomRobotAndDestination(
                          int &goalRobot,        // returns the robot to use
                          char &goalDestination,  // returns the destination square
                          int numberOfDestinationPieces) // # of possible destinations
{
    int randomNumber = rand()%4;    // get random number from 0 to 3
    goalRobot = randomNumber + 1;  // set random robot to be some value between 1 - 4 

    // get random number from 0 to numberOfDestinationPieces
    randomNumber = rand() % numberOfDestinationPieces; 
    // add this number to 'A' to set the destination character
    goalDestination = 'A' + randomNumber;                                                                       
}// end chooseRandomRobotAndDestination()


//-------------------------------------------------------------------------------------
// displayBoard()  - Display the Board
//
int displayBoard( int goalRobot,            // robot that needs to reach destination
                  char destinationLetter,    // the destination letter
                  char theBoard[ BOARD_SIZE][ 5])  // the board to be displayed
{
    // display robot to be moved and destination square
    cout << "Move robot " << goalRobot << " to square " << destinationLetter << endl;

    // display the top edge
    cout << '|';  // display the leading edge
    for( int i=0; i<BOARD_EDGE; i++) {
        cout << theBoard[ i][ ABOVE];
        // only display the space if it is not at the end of the row
        if( i < (BOARD_EDGE - 1) ) {
            cout << " ";   
        }
    }
    cout << "|" << endl;      // display trailing edge

    // display the "body" of the board
    for( int i=0; i<BOARD_SIZE; i++) {
        // Figure out what character should be displayed to the left.  It will
        // be a wall if the current spot has a left wall, or if the spot to the
        // left has a right wall.
        char leftCharacter = theBoard[ i][ LEFT];  // set left display char
        // See if the piece to the left has a right wall.  Don't do this for the
        // first piece on the board, since it has no left neighbor.
        if ( (i>0) && (theBoard[ i-1][ RIGHT] != ' ')) {
              leftCharacter = theBoard[ i-1][ RIGHT];
        }
        cout << leftCharacter << theBoard[ i][ PIECE];
        // see if we're at the end of a row
        if( ((i+1) % BOARD_EDGE) == 0) {
            // we are at the end of a row, so display right wall and go to next line
            cout << theBoard[ i][ RIGHT] << endl;
            // Now display any walls immediately below the line of pieces just displayed
            // Backup our index counter j to the beginning of this line again, to find any
            // walls displayed below this line
            cout << "|";      // display the left boundary
            for( int j=i-BOARD_EDGE+1; j<(i+1); j++) {
                  // Set the character to be displayed.  This is a wall if the
                  // current spot has a wall below, or if the spot below has a wall
                  // above. 
                  char belowCharacter = theBoard[ j][ BELOW];
                  // Only check the square below if we're NOT on the bottom row
                  if ( (j<(BOARD_SIZE - BOARD_EDGE)) &&      // verify not on bottom row
                      (theBoard[ j+16][ ABOVE] != ' ')) {    // piece below has wall above 
                      belowCharacter = theBoard[ j+16][ ABOVE];
                  }
                  cout << belowCharacter;        // display the character
                  // display an extra space, if we're not at the end of a row
                  if( ((j+1) % BOARD_EDGE) != 0) {
                      cout << " ";       
                  }
            }
            cout << "|" << endl;      // display the right boundary
        }//end if( ((i+1...

    }//end for( int i=0;...

    cout << endl;

}//end displayTheBoard()


//-------------------------------------------------------------------------------------
// canMove( )
//      See if the robot can move in the desired direction
//
bool canMove( int theRobot, int directionOffset, char moveDirection)
{
    // find neighbor location
    int theNeighbor = theRobot + directionOffset; 

    // verify that we're not out-of-bounds before we use robot position to check board array
    if( (theNeighbor < 0) || (theNeighbor >= BOARD_SIZE)) {
        return false;    // attempting to move out of bounds, which is invalid   
    }   

    // See if we can move in the desired direction of travel.  For each direction we must check for
    //      1. a wall in the piece we are moving from
    //      2. a wall in the piece we are moving to
    // If any wall is found, return false.
    switch( moveDirection) {
        case 'L':  if( (theBoard[ theRobot][ LEFT] != ' ') ||
                      (theBoard[ theNeighbor][ RIGHT] != ' ')
                    )
                    return false;  // we can't make the move
                    break;
        case 'U':  if( (theBoard[ theRobot][ ABOVE] != ' ') ||
                      (theBoard[ theNeighbor][ BELOW] != ' ')
                    )
                    return false;  // we can't make the move
                    break; 
        case 'R':  if( (theBoard[ theRobot][ RIGHT] != ' ') ||
                      (theBoard[ theNeighbor][ LEFT] != ' ')
                    )
                    return false;  // we can't make the move
                    break; 
        case 'D':  if( (theBoard[ theRobot][ BELOW] != ' ') ||
                      (theBoard[ theNeighbor][ ABOVE] != ' ')
                    )
                    return false;  // we can't make the move
                    break;   
        default: cout << "*** Error, invalid check for neighbor in canMove(). Exiting...\n";
                exit( -1);      // exit the program
    }     

    // Verify that the neighbor in the direction of travel is not also the location of one of the other
    // robots.
    char neighborPiece = theBoard[ theNeighbor][ PIECE];      // Make copy of neighbor board piece
    // ensure destination doesn't contain a robot character, which is '1'..'4'
    // Convert NUMBER_OF_ROBOTS to a char by adding it to char '0'
    if( (neighborPiece >= '1') && (neighborPiece <= NUMBER_OF_ROBOTS+'0')) {
        return false;  // destination has another robot, so can't move there.
    }

    return true;      // passed all the tests, so move is valid     
}//end method canMove()


//-------------------------------------------------------------------------------------
// blankOutSourcePosition( )
//    Put the default character in the square we're moving from.
//    One other check must be made.  If a robot (which shouldn't be the goal robot)
//    is moving OFF of the goal square, we must re-display the goal square letter.
void blankOutSourcePosition( int robotIndex, int goalPosition, char destinationLetter)
{
    if( robotIndex != goalPosition) {
        theBoard[ robotIndex][ PIECE] = '.';
    }
    else {
        theBoard[ robotIndex][ PIECE] = destinationLetter;   
    }
}


//-------------------------------------------------------------------------------------
// getGoalPosition()
//      Find the index of the goal letter.  This is used to mark the position where the
//      goal letter must be redisplayed in the case (probably rare) where a non-goal
//      robot passes over the goal letter, blanking it out.
//
int getGoalPosition( char destinationLetter)
{
    for( int i=0; i<BOARD_SIZE; i++) {
        if( theBoard[ i][ PIECE] == destinationLetter) {
            // found the position where the destination letter is, so return it
            return i;
        }   
    }//end for( int i...

    // Should never have gotten here, so display error message
    cout << "*** Error, destination letter position not found.  Exiting program...";
    exit( -1);  // exit program
}


//-------------------------------------------------------------------------------------
// displayLinkedList()
//    display some of the entries from linked list, for debugging.
//
void displayLinkedList( Node *pHead)
{
    cout << "------------" << endl;
    // traverse list, displaying some contents from each node as we go
    while( pHead != NULL) {
            cout << "Robots 2 is at: " << pHead->theRobots[ 2] << " at move " << pHead->moveNumber << endl;
            cout << "*** Debugging, board is: " << endl;
            displayBoard( 2, 'M', pHead->theBoard);
            pHead = pHead->pNext;     
    }
    cout << "------------" << endl << endl;   
}


//-------------------------------------------------------------------------------------
// storeMoveOnList
//      Store the current board and robot positions at the front of the linked list
//
void storeMoveOnList( Node * &pHead,      // head of list, Reference parm so changes are reflected back
                      int moveNumber)    // used to store current move number on the list
{
    Node *pOldHead = pHead;  // store old head of the list, since we are changing it
                              // but need new node to point to it.

    pHead = new Node;        // allocate a new blank node
    pHead->pNext = pOldHead;  // points to previous node on list (or NULL if list is empty the first time)

    // duplicate the board into the node's copy of the board
    for( int i=0; i<BOARD_SIZE; i++) {
          for( int j=0; j<5; j++) {
              pHead->theBoard[i][j] = theBoard[i][j];     
          }   
    }

    // duplicate the robots array into the node's copy of the robots array
    for( int i=0; i<(NUMBER_OF_ROBOTS + 1); i++) {
          pHead->theRobots[ i] = theRobots[ i]; 
    }

    // add the move number to the node
    pHead->moveNumber = moveNumber;

}//end storeMoveOnList


//-------------------------------------------------------------------------------------
//  undoMove( )
//    If there is more than 1 node on the list, remove the front-most node and
//    make the board and the robot locations look like the following node on the list,
//    which represents the previous move.
//
void undoMove( Node * &pHead,            // head of list, gets changed and returned
              int &moveNumber)          // move number, which gets decremented
{         
    // head node should always exist when we get here.  Perform sanity check
    if( pHead == NULL) {
        cout << "Error in program, pHead is NULL.  Exiting..." << endl;   
    }
    // verify that there is in fact a next move to use in the undo
    if( pHead->pNext != NULL) {
        // undo the move by removing the front node from the list, and restoring theBoard
        // and theRobots from the list.
        Node *pOldHead = pHead;      // store old head of list so we can free up the memory later
        pHead = pHead->pNext;        // advance the head pointer on the list, to retrieve its values

        // extract the board and robot values from the node, reflecting the previous move
        for( int i=0; i<BOARD_SIZE; i++) {
              for( int j=0; j<5; j++) {
                  theBoard[i][j] = pHead->theBoard[i][j];   
              }   
        }
        // extract the robots array from the node's copy of the previous move robots array
        for( int i=0; i<(NUMBER_OF_ROBOTS + 1); i++) {
              theRobots[ i] = pHead->theRobots[ i]; 
        }       

        // extract the move number from the one stored for this node
        moveNumber = pHead->moveNumber;

        free( pOldHead);            // free up the memory from the old head of list node
    }       
}//end undoMove()


//-------------------------------------------------------------------------------------
// playGame() - allow user to make moves, until they quit or reach the goal
//
void playGame( int goalRobot,            // robot that needs to reach destination
              char destinationLetter)    // the destination letter
{
    int moveNumber = 1;          // track how many moves have been made
    char robot = ' ';            // user entry for robot to move
    char moveDirection = ' ';    // direction in which to move (L, U, R, or D)
    int robotIndex = 0;          // location of key robot
    int directionOffset = 0;    // value to add to current robot position to reflect move direction
    bool undoErrorFlag = false;  // Used to mark situation when undo is chosen when there is no
                                  //      stored move to which we can undo.
    Node *pHead = NULL;          // initialize head of list pointer.  Very important to initialize this!

    // Find which square has the goal letter in it. Store this to use in redisplaying the goal letter
    // if a non-goal robot passes over it.
    int goalPosition = getGoalPosition( destinationLetter); 

    // blank out all the letters except the destination letter, to make playing
    // the game more clear
    for( int i=0; i<BOARD_SIZE; i++) {
          // Clear off the alphabetic squares, except for the destination square
          //      isalpha is one of the char functions.  There are more such functions at
          //      http://logos.cs.uic.edu/Examples And Notes/misc/CStdLib.html
          if( isalpha( theBoard[ i][ PIECE]) && (theBoard[ i][ PIECE] != destinationLetter) ) {
              theBoard[ i][ PIECE] = '.';      // set it to the default character
          }   
    }
    // display board
    displayBoard( goalRobot, destinationLetter, theBoard);

    // create the initial node on the list, containing the blank board
    storeMoveOnList( pHead, moveNumber);

    // infinite loop to play the game
    while( true) {
        cout << moveNumber << ". Please enter the robot to move and the direction (e.g. 2 r): ";
        // handle input of robot, (or 'X' to exit, or 'U' to undo)
        cin >> robot;
        if( toupper( robot) == 'X') {
            break;      // exit program   
        }
        // Handle undoing a move.  Extra code is included so that the error message
        // displayed when attempting undo at the beginning of the game gets displayed
        // after the board, not before the board.
        if( toupper( robot) == 'U') {
            // Record if there is no history, to use in displaying message after attempting undo move
            if( pHead->pNext == NULL) {
                undoErrorFlag = true;  // there is no history, remember this
            }

            // Undo the move.             
            undoMove( pHead, moveNumber);
            system("cls");  // "cls" for unix
            displayBoard( goalRobot, destinationLetter, theBoard);

            // Display error message if there is no history to use in undo
            if ( undoErrorFlag) {
                  cout << "*** Can't undo past the beginning of the game.  Please retry move. " << endl << endl;
                  undoErrorFlag = false;      // reset for use again next time
            }
            continue;    // go back up and prompt for next move
        }
        cin >> moveDirection;
        moveDirection = toupper( moveDirection);  // fold into upper-case
        // ensure robot values were in range
        if( (robot < '1') || (robot > NUMBER_OF_ROBOTS+'0')) {
            cout << "Invalid robot value, please retry." << endl;
            continue;  // continue up to the user input loop again   
        }

        // Find the directionOffset for robot change in position
        switch( moveDirection) {
            case 'L': directionOffset = -1; break;
            case 'U': directionOffset = -BOARD_EDGE; break;
            case 'R': directionOffset = 1; break;
            case 'D': directionOffset = BOARD_EDGE; break;
            default: cout << "*** Error, invalid direction.  Retry Input. \n" << endl;
                continue;  // continue up to the user input loop again
        } 

        // Now move the piece in that direction, keeping on going until an obstacle is hit
        // Send the current robot location as well as direction offset.
        while( canMove( theRobots[ robot-'0'], directionOffset, moveDirection)) {

            // Find position of robot
            robotIndex = theRobots[ robot-'0'];
            // blank out where robot is moving from
            blankOutSourcePosition( robotIndex, goalPosition, destinationLetter);
            // clear screen
            system("cls");  // "cls" for unix

            // store robot in new location
            theBoard[ robotIndex + directionOffset][ PIECE] = robot;
            // update robot index position.  Must subtract '0' from robot since it's a char
            theRobots[ robot-'0'] = robotIndex + directionOffset;

            // display board
            displayBoard( goalRobot, destinationLetter, theBoard);
        }//end while( canMove...

        // Update number of moves
        moveNumber++;

        // Store new move on linked list
        storeMoveOnList( pHead, moveNumber);

        // See if we've arrived at destination.  We've arrived if the robot just moved
        // was the goal robot, and it's location is the goal position
        if( ((robot-'0') == goalRobot) && (theRobots[ robot-'0'] == goalPosition) ) {
            cout << "Great Job!  You did it in only " << moveNumber << " moves." << endl;
            break;        // break from endless loop making moves 
        }                                                             
    }//end while( true...

}// end playGame()


//-------------------------------------------------------------------------------------
// main() - main part of program, that drives everything else
//
int main()
{
    // variable declarations
    int goalRobot;          // the robot that needs to reach the goal                                                     
    char destinationLetter;  // the goal letter
    int numberOfDestinationPieces;  // Number of destination pieces.  Value
                                    //    comes from the data file

    srand( 1); // Initialize random number generator.
              // Only do this ONCE in your program!
              //use srand( time(0)); to give different values each time.

    system("cls");        // clear the screen
    
    // Display Instructions
    cout << "Welcome to Ricochet Robots \n" <<
            "(\'Ricochet Robot\' is a registered trademark of Hans im Gluck\n" <<
            "Munich, Germany, 1999. The game was created by Alex Randolph.) \n" <<
            "\n" <<
            "The object of the game is to get the designated numbered robot \n" <<
            "to the indicated letter position. \n" <<
            "The numbered squares are all robots.  They unfortunately have \n" <<
            "no brakes, and so will continue in whatever direction they are \n" <<
            "moving until they encountar an obstacle. \n" <<
            "For each move enter the robot number and the desired direction. \n" <<
            "For instance entering \n" <<
            "  1 U \n" <<
            "would move the #1 robot as far up as it can go. \n" <<
            "The first letter of input is the robot number (1 - 4), \n" <<
            "and the second letter is the direction (L=left, U=up, R=right, D=down) \n" <<
            "Enter x to exit.  Enter u to undo a move.  Have fun!\n" <<   
            "\n";

    // create the board, initializing all pieces to their default settings
    createBoard();                // initialize the global theBoard array

    // read from the datafile and modify the board accordingly
    readFromFileAndModifyBoard( numberOfDestinationPieces);

    // choose a random robot and random destination
    chooseRandomRobotAndDestination( goalRobot, destinationLetter, numberOfDestinationPieces);
    // Hard-code the goalRobot and destination, over-riding the above randomly-chosen ones.
    // This is just to make the program consistent for the sake of debugging.
    goalRobot = 2;
    destinationLetter = 'M';

    // play the game
    playGame( goalRobot, destinationLetter);    // you'll need to pass some parameters...

    // pause output
    cout << "Press enter to exit program ..." << endl << endl;
    getchar();  // read in the newline character still on the input buffer from previous input
    getchar();  // read the 'x' character entered by the user.

}//end main()
