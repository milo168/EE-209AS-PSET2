#include <iostream>
#include <string>

using namespace std;

enum DIRECTIONS{
	NORTH = 1,
	EAST = 2,
	SOUTH = 3,
	WEST = 4
};

struct state{
	int x;
	int y;
	int clock;
	DIRECTIONS direction;
};


enum ACTIONS{
	STILL = 1,
	FORWARD_STILL = 2,
	FORWARD_LEFT = 3,
	FORWARD_RIGHT = 4,
	BACKWARDS_STILL = 5,
	BACKWARDS_RIGHT = 6,
	BACKWARDS_LEFT = 7
};

double transition_probability(double errorProbability, state current, ACTIONS action, state next){
	int changeX = next.x - current.x;
	int changeY = next.y - current.y;
	int changeClock = next.clock - current.clock;

	if(next.clock == 0 && current.clock == 11){
		changeClock = 1;
	}else if(next.clock == 11 && current.clock == 0){
		changeClock = -1;
	}

	if(abs(changeX) > 1 || abs(changeY) > 1 || abs(changeClock) > 1){
		return 0.0;
	}

	switch(action){
		case STILL:
			if(changeX == 0 && changeY == 0 && changeClock == 0){
				return 1.0;
			}else{
				return 0.0;
			}
		case FORWARD_STILL:
			if(current.direction == next.direction && changeClock == 0){
				if(current.direction == NORTH && changeX == 0 && changeY == 1){
					return 1.0;
				}else if(current.direction == SOUTH && changeX == 0 && changeY == -1){
					return 1.0;
				}else if(current.direction == EAST && changeX == 1 && changeY == 0){
					return 1.0;
				}else if(current.direction == WEST && changeX == -1 && changeY == 0){
					return 1.0;
				}
			}
			return 0.0;
		default:
			return 0;
	}

}

int main(int argc, char** argv){

	return 0;
}
