#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <stdint.h>
#include <queue>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include "robot.h"

#ifndef BOARD_H
#define BOARD_H

using namespace std;

class Board {

	public:
		// CONSTRUCTOR
		Board(const char* filename);

		// ACCESSORS
		bool hasWall(int x, int y, char dir) const;
        bool hasRobot(int x, int y, char dir) const;
		Position getRobotPosition(char which);
		void print() const;
		char getRobot(int x, int y) const;
        char getGoal(int x, int y) const;
        bool robotSolved(char which) const;
        bool goalSolved(char which) const;
        bool done() const;
        uint32_t asNumber() const;
        map<char, Robot*> getRobots() const { return _robots; }
		map<char, Position*> getGoals() const { return _goals; }
		int getWidth() const { return _width; }
		int getHeight() const { return _height; }

		// MODIFIERS
		Position moveRobot(char which, char dir);
        void setToConfig(uint32_t config);
        void moveRobotToPosition(char which, Position new_p);
		void solve();

	private:
		// MEMBER VARIABLES
		int _width;
		int _height;
		vector<vector<unsigned short> > _spaces;
		map<char, Robot*> _robots;
        map<char, Position*> _goals;
		unordered_set<uint32_t> _prev;

        // ACCESSORS
        bool hasWallNorth(int x, int y) const { return _spaces[x][y] & 0x8000; }
        bool hasWallSouth(int x, int y) const { return _spaces[x][y] & 0x4000; }
        bool hasWallEast(int x, int y) const { return _spaces[x][y] & 0x2000; }
        bool hasWallWest(int x, int y) const { return _spaces[x][y] & 0x1000; }

        bool hasRobotNorth(int x, int y) const;
        bool hasRobotSouth(int x, int y) const;
        bool hasRobotEast(int x, int y) const;
        bool hasRobotWest(int x, int y) const;

		// MODIFIERS
		void addWall(int x, int y, char dir);

};

#endif
