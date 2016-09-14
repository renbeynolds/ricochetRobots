#include "board.h"
#include <queue>

using namespace std;


int main() {
	Board myboard("input.txt");

    queue<uint32_t> configs;
    configs.push(myboard.asNumber());

    while(!myboard.done() && !configs.empty()) {

        myboard.setToConfig(configs.front());
        configs.pop();

        map<char, Robot*>::iterator it;
        map<char, Robot*> robots = myboard.getRobots();
        for(it = robots.begin(); it != robots.end(); it++) {

            Position p = myboard.moveRobot(it->first, 'N');
            if(myboard.done()) { break; }
            configs.push(myboard.asNumber());
            myboard.moveRobotToPosition(it->first, p);

            p = myboard.moveRobot(it->first, 'S');
            if(myboard.done()) { break; }
            configs.push(myboard.asNumber());
            myboard.moveRobotToPosition(it->first, p);

            p = myboard.moveRobot(it->first, 'E');
            if(myboard.done()) { break; }
            configs.push(myboard.asNumber());
            myboard.moveRobotToPosition(it->first, p);

            p = myboard.moveRobot(it->first, 'W');
            if(myboard.done()) { break; }
            configs.push(myboard.asNumber());
            myboard.moveRobotToPosition(it->first, p);

        } 

    }

    myboard.print();
    
    return 0;
}
