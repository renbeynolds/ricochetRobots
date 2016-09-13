#include "board.h"
#include <queue>

using namespace std;


int main() {
	Board myboard("input.txt");

    uint32_t config = myboard.asNumber();
    myboard.print();
    myboard.moveRobot('R', 'S');
    myboard.moveRobot('R', 'E');
    myboard.moveRobot('R', 'N');
    myboard.print();
    myboard.setToConfig(config);
    myboard.print();
 //    queue<uint32_t> configs;
 //    configs.push(myboard.asNumber());

    // while(!myboard.done() && !configs.empty()) {

    //     myboard.setToConfig(configs.front());
    //     configs.pop();

    //     map<char, Robot*>::iterator it;
    //     map<char, Robot*> robots = myboard.getRobots();
    //     for(it = robots.begin(); it != robots.end(); it++) {

    //     } 

    // }
    
    return 0;
}
