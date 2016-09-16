#include "../src/board.cpp"
#include <iomanip>
#include <gtest/gtest.h>

//-------------------------------------------------------
// Test the board constructor which reads the input file
//-------------------------------------------------------
TEST(BoardTest, BoardSetup) {

	// Construct the board and check overall dimensions
	Board board("test/input/input1.txt");
	ASSERT_EQ(board.getWidth(), 2);
	ASSERT_EQ(board.getHeight(), 2);

	// Check for correct placement of perimeter walls
	ASSERT_EQ(board.hasWall(0, 0, 'W'), true);
	ASSERT_EQ(board.hasWall(0, 0, 'N'), true);
	ASSERT_EQ(board.hasWall(1, 0, 'N'), true);
	ASSERT_EQ(board.hasWall(1, 0, 'E'), true);
	ASSERT_EQ(board.hasWall(1, 1, 'E'), true);
	ASSERT_EQ(board.hasWall(1, 1, 'S'), true);
	ASSERT_EQ(board.hasWall(0, 1, 'S'), true);
	ASSERT_EQ(board.hasWall(0, 1, 'W'), true);

	// Check for correct placement of interior wall
	ASSERT_EQ(board.hasWall(0, 0, 'E'), true);
	ASSERT_EQ(board.hasWall(1, 0, 'W'), true);

	// Check for correct placement of robot
	ASSERT_EQ(board.getRobots().size(), 1);
	ASSERT_EQ(board.getRobot(0, 0), 'R');
	ASSERT_EQ(board.getRobotPosition('R'), Position(0, 0));
	ASSERT_EQ(board.hasRobot(0, 1, 'N'), true);

	// Check for correct placement of goal
	ASSERT_EQ(board.getGoals().size(), 1);
	ASSERT_EQ(board.getGoal(1,0), 'R');

	// Check for correct board state
	ASSERT_EQ(board.robotSolved('R'), false);
	ASSERT_EQ(board.goalSolved('R'), false);
	ASSERT_EQ(board.done(), false);
	
}

//-------------------------------------------------------
// Test conversion from board to config and vice versa
//-------------------------------------------------------
TEST(BoardTest, BoardAsNumber) {

	// Construct the board
	Board board("test/input/input1.txt");
	ASSERT_EQ(board.asNumber(), 0);

	// Set the board to the completed state and check success
	board.setToConfig(0x10000000);
	ASSERT_EQ(board.getRobot(0, 0), 0);
	ASSERT_EQ(board.getRobot(1, 0), 'R');
	ASSERT_EQ(board.getRobotPosition('R'), Position(1, 0));
	ASSERT_EQ(board.hasRobot(0, 1, 'N'), false);
	ASSERT_EQ(board.hasRobot(1, 1, 'N'), true);
	ASSERT_EQ(board.asNumber(), 0x10000000);

}

//-------------------------------------------------------
// Test on input1 (2x2, one robot, one goal, one wall)
//-------------------------------------------------------
TEST(BoardTest, Input1) {

	// Construct and solve the board
	Board board("test/input/input1.txt");
	board.solve();

	// Check for the correct completed state
	ASSERT_EQ(board.getRobot(0, 0), 0);
	ASSERT_EQ(board.getRobot(1, 0), 'R');
	ASSERT_EQ(board.getRobotPosition('R'), Position(1, 0));
	ASSERT_EQ(board.hasRobot(0, 1, 'N'), false);
	ASSERT_EQ(board.hasRobot(1, 1, 'N'), true);
	ASSERT_EQ(board.asNumber(), 0x10000000);

}

//-------------------------------------------------------
// Test on input2 (4x4, two robots, one goal, no walls)
//-------------------------------------------------------
TEST(BoardTest, Input2) {

	// Construct and solve the board
	Board board("test/input/input2.txt");
	board.solve();

	// Check for the correct completed state
	ASSERT_EQ(board.getRobot(0, 0), 0);
	ASSERT_EQ(board.getRobot(0, 3), 0);
	ASSERT_EQ(board.getRobot(2, 0), 'R');
	ASSERT_EQ(board.getRobot(3, 0), 'G');
	ASSERT_EQ(board.getRobotPosition('R'), Position(2, 0));
	ASSERT_EQ(board.getRobotPosition('G'), Position(3, 0));
	ASSERT_EQ(board.asNumber(), 0x30200000);

}

//-------------------------------------------------------
// Test on input3 (4x4, two robots, two goals, no walls)
//-------------------------------------------------------
TEST(BoardTest, Input3) {

	// Construct and solve the board
	Board board("test/input/input3.txt");
	board.solve();

	// Check for the correct completed state
	ASSERT_EQ(board.getRobot(0, 0), 0);
	ASSERT_EQ(board.getRobot(0, 3), 0);
	ASSERT_EQ(board.getRobot(2, 0), 'R');
	ASSERT_EQ(board.getRobot(3, 3), 'G');
	ASSERT_EQ(board.getRobotPosition('R'), Position(2, 0));
	ASSERT_EQ(board.getRobotPosition('G'), Position(3, 3));
	ASSERT_EQ(board.asNumber(), 0x33200000);

}

int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
