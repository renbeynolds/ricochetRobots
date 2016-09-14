#ifndef ROBOT_H
#define ROBOT_H

class Position {
	public:
		Position(int x, int y): x(x), y(y) {};

		bool operator==(const Position& other) const {
			return (x == other.x && y == other.y) ;
		}

		bool operator!=(const Position& other) const {
			return !(*this == other);
		}

		int x;
		int y;
};

class Robot {
        public:

			// CONSTRUCTOR
			Robot(Position p): _p(p) {};
			
			// ACCESSORS
			Position getPosition() const { return _p; }
            uint8_t getPositionAsNumber() const { return (_p.x << 4) | _p.y; }

			// MODIFIERS
			void setPosition(Position p) { _p = p; }	
			void setPosition(int x, int y) { setPosition(Position(x, y)); }

        private:
			Position _p;
};

#endif
