#include "board.h"

using namespace std;


int main() {
	Board myboard("input.txt");
    vector<uint32_t> configs;
    configs.push_back(myboard.asNumber());

    while(!myboard.done()) {
        
    }
    
    return 0;
}
