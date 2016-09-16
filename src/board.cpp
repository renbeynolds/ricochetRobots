#include "board.h"

using namespace std;

Board::Board(const char* filename) {
	int n, x, y, i;
	char which;

	// Open the input board file for reading
	ifstream board_file;
	board_file.open(filename);
	if(board_file.is_open()) {

		board_file >> _width;
		board_file >> _height;
        if(_width > 16 || _height > 16) { cout << "ERROR: board too large" << endl; }
		for(i=0; i<_width; i++) {
			_spaces.push_back(vector<unsigned short>(_height, 0));
		}

		// Add the perimeter walls
		for(i=0; i<_width; i++) { addWall(i, 0, 'N'); addWall(i, _height-1, 'S'); }
		for(i=0; i<_height; i++) { addWall(_width-1, i, 'E'); addWall(0, i, 'W'); }

		// Add the internal walls
		board_file >> n;
		for(i=0; i<n; i++) {
			board_file >> x >> y >> which;
			addWall(x, y, which);
		}

		// Add the robots
		board_file >> n;
		for(i=0; i<n; i++) {
			board_file >> x >> y >> which;
			_robots[which] = new Robot(Position(x, y));
			switch(which) {
				case 'R': { _spaces[x][y] |= 0x0800; break; }
				case 'G': { _spaces[x][y] |= 0x0400; break; }
				case 'B': { _spaces[x][y] |= 0x0200; break; }
				case 'Y': { _spaces[x][y] |= 0x0100; break; }
			}
		}

		// Add the goals
		board_file >> n;
		for(i=0; i<n; i++) {
			board_file >> x >> y >> which;
            _goals[which] = new Position(x, y);
            switch(which) {
                case 'R': { _spaces[x][y] |= 0x0080; break; }
                case 'G': { _spaces[x][y] |= 0x0040; break; }
                case 'B': { _spaces[x][y] |= 0x0020; break; }
                case 'Y': { _spaces[x][y] |= 0x0010; break; }
            }
		}

	} else {
		cout << "ERROR: could not open " << filename << endl;
	}
}

bool Board::hasWall(int x, int y, char dir) const {
	if(x < _width && y < _height && x >= 0 && y >= 0) {
		switch(dir) {
			case 'N': { return hasWallNorth(x, y); }
			case 'S': { return hasWallSouth(x, y); }
			case 'E': { return hasWallEast(x, y); }
			case 'W': { return hasWallWest(x, y); }
			default: { return false; }
		}
	} else {
		cout << "ERROR: position outside board boundaries" << endl;
		return false;
	}
}

bool Board::hasRobot(int x, int y, char dir) const {
        switch(dir) {
            case 'N': { return hasRobotNorth(x, y); }
            case 'S': { return hasRobotSouth(x, y); }
            case 'E': { return hasRobotEast(x, y); }
            case 'W': { return hasRobotWest(x, y); }
            default: { return false; }
        }
}

Position Board::getRobotPosition(char which) {
	return _robots[which]->getPosition();
}

void Board::print() const {
	for(int j=0; j<_height; j++) {
		for(int i=0; i<_width; i++) {
			cout << "-" << (hasWall(i, j, 'N')? "---":"   ");
		}
		cout << "-" << endl;
		for(int i=0; i<_width; i++) {
            char which = getRobot(i, j);
            switch(which) {
                case('R'): { cout << (hasWall(i, j, 'W')? "| R ":"  R "); break; }
                case('G'): { cout << (hasWall(i, j, 'W')? "| G ":"  G "); break; }
                case('B'): { cout << (hasWall(i, j, 'W')? "| B ":"  B "); break; }
                case('Y'): { cout << (hasWall(i, j, 'W')? "| Y ":"  Y "); break; }
                default: {
                    char which2 = getGoal(i, j);
                    switch(which2) {
                        case('R'): { cout << (hasWall(i, j, 'W')? "| r ":"  r "); break; }
                        case('G'): { cout << (hasWall(i, j, 'W')? "| g ":"  g "); break; }
                        case('B'): { cout << (hasWall(i, j, 'W')? "| b ":"  b "); break; }
                        case('Y'): { cout << (hasWall(i, j, 'W')? "| y ":"  y "); break; }
                        default: { cout << (hasWall(i, j, 'W')? "|   ":"    "); } 
                    }
                }
            }
		}
		cout << "|" << endl;
	}
	cout << string((4*_width)+1, '-') << endl;
}

char Board::getRobot(int x, int y) const {
	if(_spaces[x][y] & 0x0800) {
		return 'R';
	} else if(_spaces[x][y] & 0x0400) {
		return 'G';
	} else if(_spaces[x][y] & 0x0200) {
		return 'B';
	} else if(_spaces[x][y] & 0x0100) {
		return 'Y';
	} else { return 0; }
}

char Board::getGoal(int x, int y) const {
    if(_spaces[x][y] & 0x0080) {
        return 'R';
    } else if(_spaces[x][y] & 0x0040) {
        return 'G';
    } else if(_spaces[x][y] & 0x0020) {
        return 'B';
    } else if(_spaces[x][y] & 0x0010) {
        return 'Y';
    } else { return 0; }
}

bool Board::robotSolved(char which) const {
    Position p = _robots.find(which)->second->getPosition();
    switch(which) {
        case 'R': { return _spaces[p.x][p.y] & 0x0080; }
        case 'G': { return _spaces[p.x][p.y] & 0x0040; }
        case 'B': { return _spaces[p.x][p.y] & 0x0020; }
        case 'Y': { return _spaces[p.x][p.y] & 0x0010; }
        default: { return false; }
    }
}

bool Board::goalSolved(char which) const {
    Position* p = _goals.find(which)->second;
    switch(which) {
        case 'R': { return _spaces[p->x][p->y] & 0x0800; }
        case 'G': { return _spaces[p->x][p->y] & 0x0400; }
        case 'B': { return _spaces[p->x][p->y] & 0x0200; }
        case 'Y': { return _spaces[p->x][p->y] & 0x0100; }
        default: { return false; }
    }
}

bool Board::done() const {
    std::map<char, Position*>::const_iterator it;
    for(it = _goals.begin(); it != _goals.end(); it++) {
        if(!goalSolved(it->first)) { return false; }
    }
    return true;
}

Position Board::moveRobot(char which, char dir) {
	Robot* r = _robots[which];
	Position p = r->getPosition();
    Position old_p = p;
	_spaces[p.x][p.y] &= 0xF0FF;
	while(!hasWall(p.x, p.y, dir) && !hasRobot(p.x, p.y, dir)) {
		switch(dir) {
			case 'N': { r->setPosition(p.x, p.y-1); break; }
			case 'S': { r->setPosition(p.x, p.y+1); break; }
			case 'E': { r->setPosition(p.x+1, p.y); break; }
			case 'W': { r->setPosition(p.x-1, p.y); break; }
		}
		p = r->getPosition();
	}
	switch(which) {
		case 'R': { _spaces[p.x][p.y] |= 0x0800; break; }
		case 'G': { _spaces[p.x][p.y] |= 0x0400; break; }
		case 'B': { _spaces[p.x][p.y] |= 0x0200; break; }
		case 'Y': { _spaces[p.x][p.y] |= 0x0100; break; }
	}
	return old_p;
}

void Board::setToConfig(uint32_t config) {
    map<char, Robot*>::const_iterator it;
    int shift = 24;
    for(it = _robots.begin(); it != _robots.end(); it++) {
        Position p = it->second->getPosition();
        _spaces[p.x][p.y] &= 0xF0FF; 
        uint8_t new_bot_position = config >> shift;
        int x = (new_bot_position & 0xF0) >> 4;
        int y = new_bot_position & 0x0F;
        it->second->setPosition(x, y);
        switch(it->first) {
            case 'R': { _spaces[x][y] |= 0x0800; break; }
            case 'G': { _spaces[x][y] |= 0x0400; break; }
            case 'B': { _spaces[x][y] |= 0x0200; break; }
            case 'Y': { _spaces[x][y] |= 0x0100; break; }
        }
        shift -= 8;
    }
}

void Board::moveRobotToPosition(char which, Position new_p) {
    Robot* r = _robots[which];
    Position old_p = r->getPosition(); 
    _spaces[old_p.x][old_p.y] &= 0xF0FF;
    r->setPosition(new_p);
    switch(which) {
        case 'R': { _spaces[new_p.x][new_p.y] |= 0x0800; break; }
        case 'G': { _spaces[new_p.x][new_p.y] |= 0x0400; break; }
        case 'B': { _spaces[new_p.x][new_p.y] |= 0x0200; break; }
        case 'Y': { _spaces[new_p.x][new_p.y] |= 0x0100; break; }
    }
}

void Board::addWall(int x, int y, char dir) {
	if(x < _width && y < _height && x >= 0 && y >= 0) {
		switch(dir) {
			case 'N': {
				_spaces[x][y] |= 0x8000;
				if(y-1 >= 0) { _spaces[x][y-1] |= 0x4000; }
				break; }
			case 'S': {
				_spaces[x][y] |= 0x4000;
				if(y+1 < _height) { _spaces[x][y+1] |= 0x8000; }
				break; }
			case 'E': {
				_spaces[x][y] |= 0x2000;
				if(x+1 < _width) { _spaces[x+1][y] |= 0x1000; }
				break; }
			case 'W': {
				_spaces[x][y] |= 0x1000;
				if(x-1 >= 0) { _spaces[x-1][y] |= 0x2000; }
				break; }
		}
	} else {
		cout << "ERROR: position outside board boundaries" << endl;
	}
}

bool Board::hasRobotNorth(int x, int y) const {
    if(x < _width && y < _height && x >= 0 && y > 0) {
        return _spaces[x][y-1] & 0x0F00; 
    } else {
        cout << "ERROR: position outside board boundaries" << endl; 
        return false;
    }
}

bool Board::hasRobotSouth(int x, int y) const {
    if(x < _width && y < _height-1 && x >= 0 && y >= 0) {
        return _spaces[x][y+1] & 0x0F00; 
    } else {
        cout << "ERROR: position outside board boundaries" << endl; 
        return false;
    }
}

bool Board::hasRobotEast(int x, int y) const {
    if(x < _width-1 && y < _height && x >= 0 && y >= 0) {
        return _spaces[x+1][y] & 0x0F00; 
    } else {
        cout << "ERROR: position outside board boundaries" << endl; 
        return false;
    }
}

bool Board::hasRobotWest(int x, int y) const {
    if(x < _width && y < _height && x > 0 && y >= 0) {
        return _spaces[x-1][y] & 0x0F00; 
    } else {
        cout << "ERROR: position outside board boundaries" << endl; 
        return false;
    }
}

// 8 bits each for the four robots RGBY
// 4 upper bits for x, for lower bits for y
uint32_t Board::asNumber() const {
    map<char, Robot*>::const_iterator it;
    uint32_t answer = 0;
    int shift = 24;
    for(it = _robots.begin(); it != _robots.end(); it++) {
        answer |= (it->second->getPositionAsNumber() << shift);
        shift -= 8;
    }
    return answer;
}

void Board::solve() {

	this->print();

	queue<uint32_t> configs;
	configs.push(this->asNumber());

	while(!(this->done()) && !configs.empty()) {
		
		this->setToConfig(configs.front());
		configs.pop();

		map<char, Robot*>::iterator it;
		for(it = _robots.begin(); it != _robots.end(); it++) {

			Position p = this->moveRobot(it->first, 'N');
			if(this->done()) { break; }
			configs.push(this->asNumber());
			this->moveRobotToPosition(it->first, p);

			p = this->moveRobot(it->first, 'S');
			if(this->done()) { break; }
			configs.push(this->asNumber());
			this->moveRobotToPosition(it->first, p);

			p = this->moveRobot(it->first, 'E');
			if(this->done()) { break; }
			configs.push(this->asNumber());
			this->moveRobotToPosition(it->first, p);

			p = this->moveRobot(it->first, 'W');
			if(this->done()) { break; }
			configs.push(this->asNumber());
			this->moveRobotToPosition(it->first, p);

		}
		
	}

	this->print();

}









