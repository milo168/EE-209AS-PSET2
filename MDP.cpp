#include <iostream>
#include <string>
#include <cstdlib>
#include <time.h>
#include <utility>
#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>
#include <iomanip>
using namespace std;

#define GRIDWORLD_ROWS 6
#define GRIDWORLD_COLUMNS 6

//state struct for the problem set
struct state {
	int x;
	int y;
	int clock;
};

//possible actions our robot can move
enum ACTIONS {
	STILL = 1,
	FORWARD_STILL = 2,
	FORWARD_LEFT = 3,
	FORWARD_RIGHT = 4,
	BACKWARD_STILL = 5,
	BACKWARD_RIGHT = 6,
	BACKWARD_LEFT = 7
};

//helper function for calculating transition probability of staying still
double still_helper(state current, state next) {
	int changeX = next.x - current.x;
	int changeY = next.y - current.y;
	int changeClock = next.clock - current.clock;

	//s to s' probability is 1 only if next=current
	if (changeX == 0 && changeY == 0 && changeClock == 0) {
		return 1.0;
	}

	return 0.0;
	
}

//helper function for calculating transition probability of moving forward or backward
//forward and backward have the same logic but only differ by direction
double back_forward_helper(double errorProbability, state current, state next, int direction) {
	int changeX = next.x - current.x;
	int changeY = next.y - current.y;
	int changeClock = next.clock - current.clock;

	if (next.clock == 0 && current.clock == 11) {
		changeClock = 1;
	}
	else if ((next.clock == 1 && current.clock == 11) || (next.clock == 0 && current.clock == 10)) {
		changeClock = 2;
	}
	else if (next.clock == 11 && current.clock == 0) {
		changeClock = -1;
	}
	else if ((next.clock == 10 && current.clock == 0) || (next.clock == 11 && current.clock == 1)) {
		changeClock = -2;
	}

	//impossible to move more than one adjacent tile and change heading by 1
	if (abs(changeX) > 1 || abs(changeY) > 1 || abs(changeClock) > 2) {
		return 0.0;
	}

	//check if it prerotated left
	if (changeClock == -1) {
		//prerotated left at these heading causes change axis
		if (current.clock == 2 || current.clock == 5 || current.clock == 8 || current.clock == 11) {
			if (current.clock == 2 && changeX == 0 && changeY == 1*direction) {
				return errorProbability;
			}
			else if (current.clock == 5 && changeX == 1 * direction && changeY == 0) {
				return errorProbability;
			}
			else if (current.clock == 8 && changeX == 0 && changeY == -1 * direction) {
				return errorProbability;
			}
			else if (current.clock == 11 && changeX == -1 * direction && changeY == 0) {
				return errorProbability;
			}
		}
		else {
			//prerotated left at these headings do not cuase change in axis
			if ((current.clock == 3 || current.clock == 4) && changeX == 1 * direction && changeY == 0) {
				return errorProbability;
			}
			else if ((current.clock == 6 || current.clock == 7) && changeX == 0 && changeY == -1 * direction) {
				return errorProbability;
			}
			else if ((current.clock == 9 || current.clock == 10) && changeX == -1 * direction && changeY == 0) {
				return errorProbability;
			}
			else if ((current.clock == 0 || current.clock == 1) && changeX == 0 && changeY == 1 * direction) {
				return errorProbability;
			}
		}
	}
	else if (changeClock == 0) {
		//prerotate did not happen
		if ((current.clock == 2 || current.clock == 3 || current.clock == 4) && changeX == 1 * direction && changeY == 0) {
			return 1 - 2 * errorProbability;
		}
		else if ((current.clock == 5 || current.clock == 6 || current.clock == 7) && changeX == 0 && changeY == -1 * direction) {
			return 1 - 2 * errorProbability;
		}
		else if ((current.clock == 8 || current.clock == 9 || current.clock == 10) && changeX == -1 * direction && changeY == 0) {
			return 1 - 2 * errorProbability;
		}
		else if ((current.clock == 11 || current.clock == 0 || current.clock == 1) && changeX == 0 && changeY == 1 * direction) {
			return 1 - 2 * errorProbability;
		}
	}
	else if (changeClock == 1) {
		//prerotate right at these headings causes change in axis
		if (current.clock == 1 || current.clock == 4 || current.clock == 7 || current.clock == 10) {
			if (current.clock == 1 && changeX == 1 * direction && changeY == 0) {
				return errorProbability;
			}
			else if (current.clock == 4 && changeX == 0 && changeY == -1 * direction) {
				return errorProbability;
			}
			else if (current.clock == 7 && changeX == -1 * direction && changeY == 0) {
				return errorProbability;
			}
			else if (current.clock == 10 && changeX == 0 && changeY == 1 * direction) {
				return errorProbability;
			}
		}
		else {
			//prerotate right at these headings does not cause change in axis
			if ((current.clock == 11 || current.clock == 0) && changeX == 0 && changeY == 1 * direction) {
				return errorProbability;
			}
			else if ((current.clock == 2 || current.clock == 3) && changeX == 1 * direction && changeY == 0) {
				return errorProbability;
			}
			else if ((current.clock == 5 || current.clock == 6) && changeX == 0 && changeY == -1 * direction) {
				return errorProbability;
			}
			else if ((current.clock == 8 || current.clock == 9) && changeX == -1 * direction && changeY == 0) {
				return errorProbability;
			}
		}
	}
	return 0.0;
}

//helper function for calculating transition probability of moving forward then left or backward then left
//forward then left and backward then left have the same logic but only differ by direction
double back_forward_left_helper(double errorProbability, state current, state next, int direction) {
	int changeX = next.x - current.x;
	int changeY = next.y - current.y;
	int changeClock = next.clock - current.clock;

	if (next.clock == 0 && current.clock == 11) {
		changeClock = 1;
	}
	else if ((next.clock == 1 && current.clock == 11) || (next.clock == 0 && current.clock == 10)) {
		changeClock = 2;
	}
	else if (next.clock == 11 && current.clock == 0) {
		changeClock = -1;
	}
	else if ((next.clock == 10 && current.clock == 0) || (next.clock == 11 && current.clock == 1)) {
		changeClock = -2;
	}

	//impossible to move more than one adjacent tile and change heading by 1
	if (abs(changeX) > 1 || abs(changeY) > 1 || abs(changeClock) > 2) {
		return 0.0;
	}
	
	//if prerotate causes a total change of heading of 2
	if (changeClock == -2) {
		//at these headings the axis changes
		if (current.clock == 4 || current.clock == 7 || current.clock == 10 || current.clock == 1 || 
			current.clock == 3 || current.clock == 6 || current.clock == 9 || current.clock == 0) {
			if ((current.clock == 3 || current.clock == 4) && changeX == 1 * direction && changeY == 0) {
				return errorProbability;
			}
			else if ((current.clock == 6 || current.clock == 7) && changeX == 0 && changeY == -1 * direction) {
				return errorProbability;
			}
			else if ((current.clock == 9 || current.clock == 10) && changeX == -1 * direction && changeY == 0) {
				return errorProbability;
			}
			else if ((current.clock == 0 || current.clock == 1) && changeX == 0 && changeY == 1 * direction) {
				return errorProbability;
			}
		}
		else {
			//otherwise the axis is still same
			if (current.clock == 2 && changeX == 0 && changeY == 1 * direction) {
				return errorProbability;
			}
			else if (current.clock == 5 && changeX == 1 * direction && changeY == 0) {
				return errorProbability;
			}
			else if (current.clock == 8 && changeX == 0 && changeY == -1 * direction) {
				return errorProbability;
			}
			else if (current.clock == 11 && changeX == -1 * direction && changeY == 0) {
				return errorProbability;
			}
		}
	}
	else if (changeClock == -1) {
		//no prerotation error
		if ((current.clock == 2 || current.clock == 3 || current.clock == 4) && changeX == 1 * direction && changeY == 0) {
			return 1 - 2 * errorProbability;
		}
		else if ((current.clock == 5 || current.clock == 6 || current.clock == 7) && changeX == 0 && changeY == -1 * direction) {
			return 1 - 2 * errorProbability;
		}
		else if ((current.clock == 8 || current.clock == 9 || current.clock == 10) && changeX == -1 * direction && changeY == 0) {
			return 1 - 2 * errorProbability;
		}
		else if ((current.clock == 11 || current.clock == 0 || current.clock == 1) && changeX == 0 && changeY == 1 * direction) {
			return 1 - 2 * errorProbability;
		}
	}
	else if (changeClock == 0) {
		//prerotated right but then turned left after action
		//at these headings the axis changes
		if (current.clock == 1 || current.clock == 4 || current.clock == 7 || current.clock == 10) {
			if (current.clock == 1 && changeX == 1 * direction && changeY == 0) {
				return errorProbability;
			}
			else if (current.clock == 4 && changeX == 0 && changeY == -1 * direction) {
				return errorProbability;
			}
			else if (current.clock == 7 && changeX == -1 * direction && changeY == 0) {
				return errorProbability;
			}
			else if (current.clock == 10 && changeX == 0 && changeY == 1 * direction) {
				return errorProbability;
			}
		}
		else {
			//at these headings axis does not change
			if ((current.clock == 11 || current.clock == 0) && changeX == 0 && changeY == 1 * direction) {
				return errorProbability;
			}
			else if ((current.clock == 2 || current.clock == 3) && changeX == 1 * direction && changeY == 0) {
				return errorProbability;
			}
			else if ((current.clock == 5 || current.clock == 6) && changeX == 0 && changeY == -1 * direction) {
				return errorProbability;
			}
			else if ((current.clock == 8 || current.clock == 9) && changeX == -1 * direction && changeY == 0) {
				return errorProbability;
			}
		}
	}
	return 0.0;
}
//helper function for calculating transition probability of moving forward then right or backward then right
//forward then right and backward then right have the same logic but only differ by direction
double back_forward_right_helper(double errorProbability, state current, state next, int direction) {
	int changeX = next.x - current.x;
	int changeY = next.y - current.y;
	int changeClock = next.clock - current.clock;

	if (next.clock == 0 && current.clock == 11) {
		changeClock = 1;
	}
	else if ((next.clock == 1 && current.clock == 11) || (next.clock == 0 && current.clock == 10)) {
		changeClock = 2;
	}
	else if (next.clock == 11 && current.clock == 0) {
		changeClock = -1;
	}
	else if ((next.clock == 10 && current.clock == 0) || (next.clock == 11 && current.clock == 1)) {
		changeClock = -2;
	}

	//impossible to move more than one adjacent tile and change heading by 1
	if (abs(changeX) > 1 || abs(changeY) > 1 || abs(changeClock) > 2) {
		return 0.0;
	}

	//if prerotate cuases a change of heading by 2
	if (changeClock == 2) {
		//at these headings the axis changes
		if (current.clock == 2 || current.clock == 5 || current.clock == 8 || current.clock == 11 ||
			current.clock == 3 || current.clock == 6 || current.clock == 9 || current.clock == 0) {
			if ((current.clock == 2 || current.clock == 3) && changeX == 1 * direction && changeY == 0) {
				return errorProbability;
			}
			else if ((current.clock == 5 || current.clock == 6) && changeX == 0 && changeY == -1 * direction) {
				return errorProbability;
			}
			else if ((current.clock == 8 || current.clock == 9) && changeX == -1 * direction && changeY == 0) {
				return errorProbability;
			}
			else if ((current.clock == 11 || current.clock == 0) && changeX == 0 && changeY == 1 * direction) {
				return errorProbability;
			}
		}
		else {
			//at these headings the axis does not change
			if (current.clock == 4 && changeX == 0 && changeY == -1 * direction) {
				return errorProbability;
			}
			else if (current.clock == 7 && changeX == -1 * direction && changeY == 0) {
				return errorProbability;
			}
			else if (current.clock == 10 && changeX == 0 && changeY == 1 * direction) {
				return errorProbability;
			}
			else if (current.clock == 1 && changeX == 1 * direction && changeY == 0) {
				return errorProbability;
			}
		}
	}
	else if (changeClock == 1) {
		//no prerotation error
		if ((current.clock == 5 || current.clock == 6 || current.clock == 7) && changeX == 0 && changeY == -1 * direction) {
			return 1 - 2 * errorProbability;
		}
		else if ((current.clock == 8 || current.clock == 9 || current.clock == 10) && changeX == -1 * direction && changeY == 0) {
			return 1 - 2 * errorProbability;
		}
		else if ((current.clock == 11 || current.clock == 0 || current.clock == 1) && changeX == 0 && changeY == 1 * direction) {
			return 1 - 2 * errorProbability;
		}
		else if ((current.clock == 2 || current.clock == 3 || current.clock == 4) && changeX == 1 * direction && changeY == 0) {
			return 1 - 2 * errorProbability;
		}
	}
	else if (changeClock == 0) {
		//prerotated left but then rotated right after movement
		//at these headings the axis changes
		if (current.clock == 11 || current.clock == 8 || current.clock == 5 || current.clock == 2) {
			if (current.clock == 11 && changeX == -1 * direction && changeY == 0) {
				return errorProbability;
			}
			else if (current.clock == 8 && changeX == 0 && changeY == -1 * direction) {
				return errorProbability;
			}
			else if (current.clock == 5 && changeX == 1 * direction && changeY == 0) {
				return errorProbability;
			}
			else if (current.clock == 2 && changeX == 0 && changeY == 1 * direction) {
				return errorProbability;
			}
		}
		else {
			//at these headings the axis does not change
			if ((current.clock == 0 || current.clock == 1) && changeX == 0 && changeY == 1 * direction) {
				return errorProbability;
			}
			else if ((current.clock == 10 || current.clock == 9) && changeX == -1 * direction && changeY == 0) {
				return errorProbability;
			}
			else if ((current.clock == 7 || current.clock == 6) && changeX == 0 && changeY == -1 * direction) {
				return errorProbability;
			}
			else if ((current.clock == 4 || current.clock == 3) && changeX == 1 * direction && changeY == 0) {
				return errorProbability;
			}
		}
	}
	return 0.0;
}


//calculate transition probability to get to s' from s given some action
double transition_probability(double errorProbability, state current, state next, ACTIONS action) {
	switch (action) {
	case STILL:
		return still_helper(current, next);
	case FORWARD_STILL:
		return back_forward_helper(errorProbability, current, next, 1);
	case BACKWARD_STILL:
		return back_forward_helper(errorProbability, current, next, -1);
	case FORWARD_LEFT:
		return back_forward_left_helper(errorProbability, current, next, 1);
	case BACKWARD_LEFT:
		return back_forward_left_helper(errorProbability, current, next, -1);
	case FORWARD_RIGHT:
		return back_forward_right_helper(errorProbability, current, next, 1);
	case BACKWARD_RIGHT:
		return back_forward_right_helper(errorProbability, current, next, -1);
	default:
		return 0;
	}

}

//find the next state
state get_next_state(double errorProbability, state current, ACTIONS action) {
	int chance = (int)(100.0 * rand() / (RAND_MAX + 1.0)) + 1;
	vector<pair<state, double>> possibleStates;

	//ITERATE THROUGH ALL POSSIBLE STATES IMMEDIATELY NEXT TO THE CURRENT GRID COORDINATE
	for (int i = 0; i < 12; i++) {
		state next;
		next.x = current.x;
		next.y = current.y;
		next.clock = i;
		possibleStates.push_back(make_pair(next, transition_probability(errorProbability, current, next, action)));
	}
	for (int i = 0; i < 12; i++) {
		state next;
		next.x = current.x + 1;
		next.y = current.y;
		next.clock = i;
		possibleStates.push_back(make_pair(next, transition_probability(errorProbability, current, next, action)));
	}
	for (int i = 0; i < 12; i++) {
		state next;
		next.x = current.x - 1;
		next.y = current.y;
		next.clock = i;
		possibleStates.push_back(make_pair(next, transition_probability(errorProbability, current, next, action)));
	}
	for (int i = 0; i < 12; i++) {
		state next;
		next.x = current.x;
		next.y = current.y + 1;
		next.clock = i;
		possibleStates.push_back(make_pair(next, transition_probability(errorProbability, current, next, action)));
	}
	for (int i = 0; i < 12; i++) {
		state next;
		next.x = current.x;
		next.y = current.y - 1;
		next.clock = i;
		possibleStates.push_back(make_pair(next, transition_probability(errorProbability, current, next, action)));
	}
	//for (int i = 0; i < possibleStates.size(); i++) {
	//	cout << possibleStates[i].first.x << " " << possibleStates[i].first.y << " " << possibleStates[i].first.clock << " " << possibleStates[i].second << " " << action << "\n";
	//}
	//cout << "\n\n";

	//GENERATE THE NEXT STATE
	state next;
	if (action == STILL) {
		for (int i = 0; i < possibleStates.size(); i++) {
			if (possibleStates[i].second == 1) {
				next = possibleStates[i].first;
				break;
			}
		}
	}
	
	if (chance <= (1 - 2 * errorProbability)*100) {
		for (int i = 0; i < possibleStates.size(); i++) {
			if (possibleStates[i].second == 1 - 2 * errorProbability) {
				next = possibleStates[i].first;
				break;
			}
		}
	}
	else if (chance > (1 - 2 * errorProbability)*100 && chance <= (1 - errorProbability)*100) {
		for (int i = 0; i < possibleStates.size(); i++) {
			if (possibleStates[i].second == errorProbability) {
				next = possibleStates[i].first;
				break;
			}
		}
	}
	else if (chance > (1 - errorProbability)*100 && chance <= 1*100) {
		int once = 0;
		for (int i = 0; i < possibleStates.size(); i++) {
			if (possibleStates[i].second == errorProbability) {
				next = possibleStates[i].first;
				once++;
				if (once == 2) {
					break;
				}
			}
		}
	}
	//DO BOUND CHECK HERE
	if (next.x >= GRIDWORLD_COLUMNS || next.x < 0) {
		next.x = current.x;
	}
	else if (next.y >= GRIDWORLD_ROWS || next.y < 0) {
		next.y = current.y;
	}

	//cout << "The current state is: \t" << current.x << " " << current.y << " " << current.clock << " " << action << "\n";
	//cout << "The next state is: \t" << next.x << " " << next.y << " " << next.clock << "\n";
	
	return next;
}

//returns the reward
double get_reward(double gridWorld[GRIDWORLD_ROWS][GRIDWORLD_COLUMNS][12], state current) {
	return gridWorld[GRIDWORLD_ROWS-current.y-1][current.x][current.clock];
}

//used for initial policy
ACTIONS get_action(state current, int goalX, int goalY) {
	//For h in +/- x
	//check which way the robot is facing
	if (current.clock == 2 || current.clock == 3 || current.clock == 4 ||
		current.clock == 8 || current.clock == 9 || current.clock == 10) {
		//if the robot is not immediate above/under the goal
		if (abs(goalX - current.x) != 0) {
			//if the robot are at these headings and orientation to the goal...
			//corresponds to right of the goal and left of the goal
			if ((goalX < current.x && (current.clock == 2 || current.clock == 3 || current.clock == 4)) ||
				(goalX > current.x && (current.clock == 8 || current.clock == 9 || current.clock == 10))) {
				//if it is immediate to left/right of goal
				if (goalY - current.y == 0) {
					return BACKWARD_STILL;
				}
				else {
					//try to get immediate above/under to the goal
					if (current.clock == 2 || current.clock == 8) {
						return BACKWARD_LEFT;
					}
					else {
						return BACKWARD_RIGHT;
					}
				}
			}
			else {
				//same as above
				if (goalY - current.y == 0) {
					return FORWARD_STILL;
				}
				else {
					if (current.clock == 2 || current.clock == 8) {
						return FORWARD_LEFT;
					}
					else {
						return FORWARD_RIGHT;
					}
				}
			}
		}
		else {
			//if robot is at boundary and directly under goal
			if (current.x == 0 || current.x == GRIDWORLD_COLUMNS - 1) {
				//check boundary and heading
				if ((current.x == 0 && (current.clock == 2 || current.clock == 3 || current.clock == 4)) ||
					(current.x == GRIDWORLD_COLUMNS - 1 && (current.clock == 8 || current.clock == 9 || current.clock == 10))) {

					//this means goal is at boundary
					if (goalY - current.y == 0) {
						return STILL;
					}
					else {
						//drive into wall and try to orientate to face goal
						if (current.clock == 2 || current.clock == 8) {
							return BACKWARD_LEFT;
						}
						else {
							return BACKWARD_RIGHT;
						}
					}
				}
				else {
					//goal at boundary
					if (goalY - current.y == 0) {
						return STILL;
					}
					else {
						//move along the wall
						if (current.clock == 2 || current.clock == 8) {
							return FORWARD_LEFT;
						}
						else {
							return FORWARD_RIGHT;
						}
					}
				}
			}
			else {
				//goal is not at boundary and robot is at goal
				if (goalY - current.y == 0) {
					return STILL;
				}
				else {
					//try to get to same y
					if (current.clock == 2 || current.clock == 8) {
						return FORWARD_LEFT;
					}
					else {
						return FORWARD_RIGHT;
					}
				}
			}
		}
	}

	//For h in +/- y
	//see above function for explanation
	//this is a mirror but with changes for y
	if (current.clock == 11 || current.clock == 0 || current.clock == 1 ||
		current.clock == 5 || current.clock == 6 || current.clock == 7) {
		if (abs(goalY - current.y) != 0) {
			if ((goalY < current.y && (current.clock == 11 || current.clock == 0 || current.clock == 1)) ||
				(goalY > current.y && (current.clock == 5 || current.clock == 6 || current.clock == 7))) {
				if (goalX - current.x == 0) {
					return BACKWARD_STILL;
				}
				else {
					if (current.clock == 5 || current.clock == 11) {
						return BACKWARD_LEFT;
					}
					else {
						return BACKWARD_RIGHT;
					}
				}
			}
			else {
				if (goalX - current.x == 0) {
					return FORWARD_STILL;
				}
				else {
					if (current.clock == 5 || current.clock == 11) {
						return FORWARD_LEFT;
					}
					else {
						return FORWARD_RIGHT;
					}
				}
			}
		}
		else {
			if (current.y == 0 || current.y == GRIDWORLD_ROWS - 1) {
				if ((current.y == 0 && (current.clock == 11 || current.clock == 0 || current.clock == 1)) ||
					(current.y == GRIDWORLD_ROWS - 1 && (current.clock == 5 || current.clock == 6 || current.clock == 7))) {
					if (goalX - current.x == 0) {
						return STILL;
					}
					else {
						if (current.clock == 5 || current.clock == 11) {
							return BACKWARD_LEFT;
						}
						else {
							return BACKWARD_RIGHT;
						}
					}
				}
				else {
					if (goalX - current.x == 0) {
						return STILL;
					}
					else {
						if (current.clock == 5 || current.clock == 11) {
							return FORWARD_LEFT;
						}
						else {
							return FORWARD_RIGHT;
						}
					}
				}
			}
			else {
				if (goalX - current.x == 0) {
					return STILL;
				}
				else {
					if (current.clock == 5 || current.clock == 11) {
						return FORWARD_LEFT;
					}
					else {
						return FORWARD_RIGHT;
					}
				}
			}
		}
	}
}

//generates initial actions for all states
ACTIONS*** get_initial_actions(state goal) {
	ACTIONS*** actionPolicies = new ACTIONS**[GRIDWORLD_ROWS];
	for (int i = 0; i < GRIDWORLD_ROWS; i++) {
		actionPolicies[i] = new ACTIONS*[GRIDWORLD_COLUMNS];
		for (int j = 0; j < GRIDWORLD_COLUMNS; j++) {
			actionPolicies[i][j] = new ACTIONS[12];
		}
	}

	for (int i = 0; i < GRIDWORLD_ROWS; i++) {
		for (int j = 0; j < GRIDWORLD_COLUMNS; j++) {
			for (int k = 0; k < 12; k++) {
				state current;
				current.x = j;
				current.y = GRIDWORLD_ROWS - i - 1;
				current.clock = k;
				actionPolicies[GRIDWORLD_ROWS-i-1][j][k] = get_action(current, goal.x, goal.y);
 			}
		}
	}
	return actionPolicies;
}

//printing for robot trajectory it takes for some policy, initial state, and goal
void generate_and_plot(ACTIONS*** action_policies, state initial, double errorProbability, state goal) {
	int MAX_STEPS = GRIDWORLD_ROWS * GRIDWORLD_COLUMNS * 12 * 1000000;
	int count = 0;
	state generated = initial;
	vector<state> history;

	history.push_back(generated);
	//while ((generated.x != goal.x || generated.y != goal.y) && count < MAX_STEPS) {
	while (action_policies[generated.y][generated.x][generated.clock] != STILL && count < MAX_STEPS){
		generated = get_next_state(errorProbability, generated, action_policies[generated.y][generated.x][generated.clock]);
		history.push_back(generated);
		count++;
	}
	//generated = get_next_state(errorProbability, generated, action_policies[generated.y][generated.x][generated.clock]);
	//cout << "AFTER FINDING STILL STATE: " << generated.x << " " << generated.y <<  " " << generated.clock << "\n";

	char** gridPrint = new char*[GRIDWORLD_ROWS];
	for (int i = 0; i < GRIDWORLD_ROWS; i++) {
		gridPrint[i] = new char[GRIDWORLD_COLUMNS];
		for (int j = 0; j < GRIDWORLD_COLUMNS; j++) {
			gridPrint[i][j] = '-';
		}
	}

	for (int i = 0; i < history.size(); i++) {
		gridPrint[history[i].y][history[i].x] = 'x';
		//cout << history[i].x << " " << history[i].y << " " << history[i].clock << "\n";
	}

	cout << "\n";
	for (int i = 0; i < GRIDWORLD_ROWS; i++) {
		for (int j = 0; j < GRIDWORLD_COLUMNS; j++) {
			cout << gridPrint[GRIDWORLD_ROWS-i-1][j] << ",";
		}
		cout << "\n";
	}

	for (int i = 0; i < GRIDWORLD_ROWS; i++) {
		delete[] gridPrint[i];
	}
	delete[] gridPrint;
}

//policy evaluation
double*** evaulate_policy(double gridWorld[][GRIDWORLD_COLUMNS][12], ACTIONS*** actionPolicies, double errorProbability, double discountFactor) {

	double*** values_for_states = new double**[GRIDWORLD_ROWS];
	for (int i = 0; i < GRIDWORLD_ROWS; i++) {
		values_for_states[i] = new double*[GRIDWORLD_COLUMNS];
		for (int j = 0; j < GRIDWORLD_COLUMNS; j++) {
			values_for_states[i][j] = new double[12];
			for (int k = 0; k < 12; k++) {
				values_for_states[i][j][k] = 0.0;
			}
		}
	}

	double*** values_for_states2 = new double**[GRIDWORLD_ROWS];
	for (int i = 0; i < GRIDWORLD_ROWS; i++) {
		values_for_states2[i] = new double*[GRIDWORLD_COLUMNS];
		for (int j = 0; j < GRIDWORLD_COLUMNS; j++) {
			values_for_states2[i][j] = new double[12];
			for (int k = 0; k < 12; k++) {
				values_for_states2[i][j][k] = 0;
			}
		}
	}

	//for all states get the total value from going at some current state to all s' next states
	//most will be 0 because of transition probability
	//stop only if none of the values go above some delta
	while (true) {
		double delta = 0;
		for (int i = 0; i < GRIDWORLD_ROWS; i++) {
			for (int j = 0; j < GRIDWORLD_COLUMNS; j++) {
				for (int k = 0; k < 12; k++) {
					
					double temp = values_for_states2[GRIDWORLD_ROWS - i - 1][j][k];
					state current;
					current.y = GRIDWORLD_ROWS - i - 1;
					current.x = j;
					current.clock = k;

					for (int a = -1; a < GRIDWORLD_ROWS+1; a++) {
						for (int b = -1; b < GRIDWORLD_COLUMNS+1; b++) {
							for (int c = 0; c < 12; c++) {
								state next;
								next.y = GRIDWORLD_ROWS - a - 1;
								next.x = b;
								next.clock = c;

								if (a == -1 && b == -1) {
									continue;
								}
								else if (a == -1 && b == GRIDWORLD_COLUMNS) {
									continue;
								}
								else if (a == GRIDWORLD_ROWS && b == -1) {
									continue;
								}
								else if (a == GRIDWORLD_ROWS && b == GRIDWORLD_COLUMNS) {
									continue;
								}

								double adjustedOffset = 0;
								//top row
								if (b > -1  && b < GRIDWORLD_COLUMNS && a == -1) {
									adjustedOffset = values_for_states2[GRIDWORLD_ROWS - 1][b][c];
								}//bottom row
								else if (b > -1 && b < GRIDWORLD_COLUMNS && a == GRIDWORLD_ROWS) {
									adjustedOffset = values_for_states2[0][b][c];
								}//side left
								else if (a > -1 && a < GRIDWORLD_ROWS && b == -1) {
									adjustedOffset = values_for_states2[GRIDWORLD_ROWS - a - 1][b + 1][c];
								}//side right
								else if (a > -1 && a < GRIDWORLD_ROWS && b == GRIDWORLD_COLUMNS) {
									adjustedOffset = values_for_states2[GRIDWORLD_ROWS - a - 1][b - 1][c];
								}//normal case
								else {
									adjustedOffset = values_for_states2[GRIDWORLD_ROWS - a - 1][b][c];
								}
								values_for_states[GRIDWORLD_ROWS - i - 1][j][k] += transition_probability(errorProbability, current, next, actionPolicies[current.y][current.x][current.clock]) *
									(get_reward(gridWorld, current) + discountFactor * adjustedOffset);// values_for_states2[GRIDWORLD_ROWS - a - 1][b][c]);
							}
						}
					}
					delta = max(delta, abs(temp - values_for_states[GRIDWORLD_ROWS-i-1][j][k]));
				}
			}
		}

		for (int a = 0; a < GRIDWORLD_ROWS; a++) {
			for (int b = 0; b < GRIDWORLD_COLUMNS; b++) {
				for (int c = 0; c < 12; c++) {

					values_for_states2[GRIDWORLD_ROWS - a - 1][b][c] = values_for_states[GRIDWORLD_ROWS - a - 1][b][c];
					values_for_states[GRIDWORLD_ROWS - a - 1][b][c] = 0;
				}
			}
		}

		//this_thread::sleep_for(chrono::seconds(5));
		if (delta < 0.001) {
			break;
		}
	}

	/*for (int c = 0; c < 12; c++) {
		for (int a = 0; a < GRIDWORLD_ROWS; a++) {
			for (int b = 0; b < GRIDWORLD_COLUMNS; b++) {
				cout << values_for_states2[GRIDWORLD_ROWS - a - 1][b][c] << ",";
			}
			cout << "\n";
		}
		cout << "\n";
	}*/
	cout << values_for_states2[GRIDWORLD_ROWS - 4 - 1][1][6] << "\n";

	for (int i = 0; i < GRIDWORLD_ROWS; i++) {
		for (int j = 0; j < GRIDWORLD_COLUMNS; j++) {
			delete[] values_for_states[i][j];
		}
		delete[] values_for_states[i];
	}
	delete[] values_for_states;

	return values_for_states2;
}

//for part of finding optimal policy
ACTIONS*** change_policy(double*** values_for_states, double gridWorld[][GRIDWORLD_COLUMNS][12], double errorProbability, double discountFactor) {
	ACTIONS*** actionsToTake = new ACTIONS**[GRIDWORLD_ROWS];
	for (int i = 0; i < GRIDWORLD_ROWS; i++) {
		actionsToTake[i] = new ACTIONS*[GRIDWORLD_COLUMNS];
		for (int j = 0; j < GRIDWORLD_COLUMNS; j++) {
			actionsToTake[i][j] = new ACTIONS[12];
		}
	}
	//for each current state, get the action that maximizes your reward to get to all possible next states
	//most will be 0 because of transition probability
	for (int i = 0; i < GRIDWORLD_ROWS; i++) {
		for (int j = 0; j < GRIDWORLD_COLUMNS; j++) {
			for (int k = 0; k < 12; k++) {
				state current;
				current.y = GRIDWORLD_ROWS - i - 1;
				current.x = j;
				current.clock = k;

				double checkActions[7] = { 0.0 };
				for (int a = -1; a < GRIDWORLD_ROWS+1; a++) {
					for (int b = -1; b < GRIDWORLD_COLUMNS+1; b++) {
						for (int c = 0; c < 12; c++) {
							state next;
							next.y = GRIDWORLD_ROWS - a - 1;
							next.x = b;
							next.clock = c;

							if (a == -1 && b == -1) {
								continue;
							}
							else if (a == -1 && b == GRIDWORLD_COLUMNS) {
								continue;
							}
							else if (a == GRIDWORLD_ROWS && b == -1) {
								continue;
							}
							else if (a == GRIDWORLD_ROWS && b == GRIDWORLD_COLUMNS) {
								continue;
							}

							double adjustedOffset = 0;
							//top row
							if (b > -1 && b < GRIDWORLD_COLUMNS && a == -1) {
								adjustedOffset = values_for_states[GRIDWORLD_ROWS - 1][b][c];
							}//bottom row
							else if (b > -1 && b < GRIDWORLD_COLUMNS && a == GRIDWORLD_ROWS) {
								adjustedOffset = values_for_states[0][b][c];
							}//side left
							else if (a > -1 && a < GRIDWORLD_ROWS && b == -1) {
								adjustedOffset = values_for_states[GRIDWORLD_ROWS - a - 1][b + 1][c];
							}//side right
							else if (a > -1 && a < GRIDWORLD_ROWS && b == GRIDWORLD_COLUMNS) {
								adjustedOffset = values_for_states[GRIDWORLD_ROWS - a - 1][b - 1][c];
							}//normal case
							else {
								adjustedOffset = values_for_states[GRIDWORLD_ROWS - a - 1][b][c];
							}

							checkActions[0] += transition_probability(errorProbability, current, next, STILL) *
								(get_reward(gridWorld, current) + discountFactor * adjustedOffset);// values_for_states[GRIDWORLD_ROWS - a - 1][b][c]);
							checkActions[1] += transition_probability(errorProbability, current, next, BACKWARD_LEFT) *
								(get_reward(gridWorld, current) + discountFactor * adjustedOffset);//values_for_states[GRIDWORLD_ROWS - a - 1][b][c]);
							checkActions[2] += transition_probability(errorProbability, current, next, BACKWARD_RIGHT) *
								(get_reward(gridWorld, current) + discountFactor * adjustedOffset);//values_for_states[GRIDWORLD_ROWS - a - 1][b][c]);
							checkActions[3] += transition_probability(errorProbability, current, next, FORWARD_LEFT) *
								(get_reward(gridWorld, current) + discountFactor * adjustedOffset);//values_for_states[GRIDWORLD_ROWS - a - 1][b][c]);
							checkActions[4] += transition_probability(errorProbability, current, next, FORWARD_RIGHT) *
								(get_reward(gridWorld, current) + discountFactor * adjustedOffset);//values_for_states[GRIDWORLD_ROWS - a - 1][b][c]);
							checkActions[5] += transition_probability(errorProbability, current, next, BACKWARD_STILL) *
								(get_reward(gridWorld, current) + discountFactor * adjustedOffset);//values_for_states[GRIDWORLD_ROWS - a - 1][b][c]);
							checkActions[6] += transition_probability(errorProbability, current, next, FORWARD_STILL) *
								(get_reward(gridWorld, current) + discountFactor * adjustedOffset);//values_for_states[GRIDWORLD_ROWS - a - 1][b][c]);
						}
					}
				}
				double val = checkActions[0];
				int index = 0;
				for (int i = 0; i < 7; i++) {
					if(val < checkActions[i]){
						index = i;
						val = checkActions[i];
					}
				}
				switch (index) {
				case 0:
					actionsToTake[GRIDWORLD_ROWS - i - 1][j][k] = STILL;
					break;
				case 1:
					actionsToTake[GRIDWORLD_ROWS - i - 1][j][k] = BACKWARD_LEFT;
					break;
				case 2:
					actionsToTake[GRIDWORLD_ROWS - i - 1][j][k] = BACKWARD_RIGHT;
					break;
				case 3:
					actionsToTake[GRIDWORLD_ROWS - i - 1][j][k] = FORWARD_LEFT;
					break;
				case 4:
					actionsToTake[GRIDWORLD_ROWS - i - 1][j][k] = FORWARD_RIGHT;
					break;
				case 5:
					actionsToTake[GRIDWORLD_ROWS - i - 1][j][k] = BACKWARD_STILL;
					break;
				case 6:
					actionsToTake[GRIDWORLD_ROWS - i - 1][j][k] = FORWARD_STILL;
					break;
				}
			}
		}
	}

	/*for (int i = 0; i < GRIDWORLD_ROWS; i++) {
		for (int j = 0; j < GRIDWORLD_COLUMNS; j++) {
			cout << values_for_states[GRIDWORLD_ROWS - i - 1][j][5] << " ";
		}
		cout << "\n";
	}
	cout << "\n";

	for (int i = 0; i < GRIDWORLD_ROWS; i++) {
		for (int j = 0; j < GRIDWORLD_COLUMNS; j++) {
			cout << actionsToTake[GRIDWORLD_ROWS - i - 1][j][5] << " ";
		}
		cout << "\n";
	}
	cout << "\n";*/

	return actionsToTake;
}

//policy iteration
void optimal_policy_iteration(double gridWorld[][GRIDWORLD_COLUMNS][12], double errorProbability, double discountFactor, state current, state goal) {

	/*ACTIONS*** actionPolicies = get_initial_actions(goal);
	double*** valuesForStates = evaulate_policy(gridWorld, actionPolicies, errorProbability, discountFactor);
	cout << "The value of the trajectory at (" << current.x << "," << current.y << "," << current.clock << ") is: " << valuesForStates[current.y][current.x][current.clock] << "\n";
	generate_and_plot(actionPolicies, current, errorProbability, goal);*/

	ACTIONS*** holdPolicies = new ACTIONS**[GRIDWORLD_ROWS];
	for (int i = 0; i < GRIDWORLD_ROWS; i++) {
		holdPolicies[i] = new ACTIONS*[GRIDWORLD_COLUMNS];
		for (int j = 0; j < GRIDWORLD_COLUMNS; j++) {
			holdPolicies[i][j] = new ACTIONS[12];
		}
	}

	ACTIONS*** actionPolicies = get_initial_actions(goal);
	ACTIONS*** tmpPointerActions;
	double*** valueForStates;
	double*** tmpPointerValues;
	bool notSame = true;

	//evaluate the policy by calculating the values at each state
	//so call evaluate_policy
	//then generate a new set of policies by passing the values from the old policies
	//stop until policies do not change at any of the states
	while (notSame == true) {
		
		valueForStates = evaulate_policy(gridWorld, actionPolicies, errorProbability, discountFactor);
		
		for (int i = 0; i < GRIDWORLD_ROWS; i++) {
			for (int j = 0; j < GRIDWORLD_COLUMNS; j++) {
				for (int k = 0; k < 12; k++) {
					holdPolicies[GRIDWORLD_ROWS-i-1][j][k] = actionPolicies[GRIDWORLD_ROWS - i - 1][j][k];
				}
			}
		}

		tmpPointerActions = actionPolicies;
		for (int i = 0; i < GRIDWORLD_ROWS; i++) {
			for (int j = 0; j < GRIDWORLD_COLUMNS; j++) {
				delete[] tmpPointerActions[i][j];
			}
			delete[] tmpPointerActions[i];
		}
		delete[] tmpPointerActions;

		actionPolicies = change_policy(valueForStates, gridWorld, errorProbability, discountFactor);

		bool check = false;
		for (int i = 0; i < GRIDWORLD_ROWS; i++) {
			for (int j = 0; j < GRIDWORLD_COLUMNS; j++) {
				for (int k = 0; k < 12; k++) {
					if (holdPolicies[GRIDWORLD_ROWS - i - 1][j][k] != actionPolicies[GRIDWORLD_ROWS - i - 1][j][k]) {
						check = true;
					}
				}
			}
		}
		notSame = check;

		if (check != false) {
			tmpPointerValues = valueForStates;
			for (int i = 0; i < GRIDWORLD_ROWS; i++) {
				for (int j = 0; j < GRIDWORLD_COLUMNS; j++) {
					delete[] tmpPointerValues[i][j];
				}
				delete[] tmpPointerValues[i];
			}
			delete[] tmpPointerValues;
		}
	}

	generate_and_plot(holdPolicies, current, errorProbability, goal);
	cout << valueForStates[GRIDWORLD_ROWS - 4 - 1][1][6] << "\n";

	tmpPointerValues = valueForStates;
	for (int i = 0; i < GRIDWORLD_ROWS; i++) {
		for (int j = 0; j < GRIDWORLD_COLUMNS; j++) {
			delete[] tmpPointerValues[i][j];
		}
		delete[] tmpPointerValues[i];
	}
	delete[] tmpPointerValues;

	for (int i = 0; i < GRIDWORLD_ROWS; i++) {
		for (int j = 0; j < GRIDWORLD_COLUMNS; j++) {
			delete[] holdPolicies[i][j];
		}
		delete[] holdPolicies[i];
	}
	delete[] holdPolicies;
}

//for value iteration
double*** change_values(ACTIONS*** actionsToTake, double*** oldValuesForStates, double gridWorld[][GRIDWORLD_COLUMNS][12], double errorProbability, double discountFactor) {
	double*** valuesForStates = new double**[GRIDWORLD_ROWS];
	for (int i = 0; i < GRIDWORLD_ROWS; i++) {
		valuesForStates[i] = new double*[GRIDWORLD_COLUMNS];
		for (int j = 0; j < GRIDWORLD_COLUMNS; j++) {
			valuesForStates[i][j] = new double[12];
			for (int k = 0; k < 12; k++) {
				valuesForStates[i][j][k] = 0;
			}
		}
	}

	//find the action that maximizes the value at current state for all possible s' next states
	//store those action too for optimal policy
	for (int i = 0; i < GRIDWORLD_ROWS; i++) {
		for (int j = 0; j < GRIDWORLD_COLUMNS; j++) {
			for (int k = 0; k < 12; k++) {
				state current;
				current.y = GRIDWORLD_ROWS - i - 1;
				current.x = j;
				current.clock = k;

				double checkActions[7] = { 0.0 };
				for (int a = -1; a < GRIDWORLD_ROWS+1; a++) {
					for (int b = -1; b < GRIDWORLD_COLUMNS+1; b++) {
						for (int c = 0; c < 12; c++) {
							state next;
							next.y = GRIDWORLD_ROWS - a - 1;
							next.x = b;
							next.clock = c;

							if (a == -1 && b == -1) {
								continue;
							}
							else if (a == -1 && b == GRIDWORLD_COLUMNS) {
								continue;
							}
							else if (a == GRIDWORLD_ROWS && b == -1) {
								continue;
							}
							else if (a == GRIDWORLD_ROWS && b == GRIDWORLD_COLUMNS) {
								continue;
							}

							double adjustedOffset = 0;
							//top row
							if (b > -1 && b < GRIDWORLD_COLUMNS && a == -1) {
								adjustedOffset = oldValuesForStates[GRIDWORLD_ROWS - 1][b][c];
							}//bottom row
							else if (b > -1 && b < GRIDWORLD_COLUMNS && a == GRIDWORLD_ROWS) {
								adjustedOffset = oldValuesForStates[0][b][c];
							}//side left
							else if (a > -1 && a < GRIDWORLD_ROWS && b == -1) {
								adjustedOffset = oldValuesForStates[GRIDWORLD_ROWS - a - 1][b + 1][c];
							}//side right
							else if (a > -1 && a < GRIDWORLD_ROWS && b == GRIDWORLD_COLUMNS) {
								adjustedOffset = oldValuesForStates[GRIDWORLD_ROWS - a - 1][b - 1][c];
							}//normal case
							else {
								adjustedOffset = oldValuesForStates[GRIDWORLD_ROWS - a - 1][b][c];
							}

							checkActions[0] += transition_probability(errorProbability, current, next, STILL) *
								(get_reward(gridWorld, current) + discountFactor * adjustedOffset);//oldValuesForStates[GRIDWORLD_ROWS - a - 1][b][c]);
							checkActions[1] += transition_probability(errorProbability, current, next, BACKWARD_LEFT) *
								(get_reward(gridWorld, current) + discountFactor * adjustedOffset);//oldValuesForStates[GRIDWORLD_ROWS - a - 1][b][c]);
							checkActions[2] += transition_probability(errorProbability, current, next, BACKWARD_RIGHT) *
								(get_reward(gridWorld, current) + discountFactor * adjustedOffset);//oldValuesForStates[GRIDWORLD_ROWS - a - 1][b][c]);
							checkActions[3] += transition_probability(errorProbability, current, next, FORWARD_LEFT) *
								(get_reward(gridWorld, current) + discountFactor * adjustedOffset);//oldValuesForStates[GRIDWORLD_ROWS - a - 1][b][c]);
							checkActions[4] += transition_probability(errorProbability, current, next, FORWARD_RIGHT) *
								(get_reward(gridWorld, current) + discountFactor * adjustedOffset);//oldValuesForStates[GRIDWORLD_ROWS - a - 1][b][c]);
							checkActions[5] += transition_probability(errorProbability, current, next, BACKWARD_STILL) *
								(get_reward(gridWorld, current) + discountFactor * adjustedOffset);//oldValuesForStates[GRIDWORLD_ROWS - a - 1][b][c]);
							checkActions[6] += transition_probability(errorProbability, current, next, FORWARD_STILL) *
								(get_reward(gridWorld, current) + discountFactor * adjustedOffset);//oldValuesForStates[GRIDWORLD_ROWS - a - 1][b][c]);
						}
					}
				}
				double val = checkActions[0];
				int index = 0;
				for (int i = 0; i < 7; i++) {
					if (val < checkActions[i]) {
						index = i;
						val = checkActions[i];
					}
				}

				valuesForStates[GRIDWORLD_ROWS - i - 1][j][k] += val;

				switch (index) {
				case 0:
					actionsToTake[GRIDWORLD_ROWS - i - 1][j][k] = STILL;
					break;
				case 1:
					actionsToTake[GRIDWORLD_ROWS - i - 1][j][k] = BACKWARD_LEFT;
					break;
				case 2:
					actionsToTake[GRIDWORLD_ROWS - i - 1][j][k] = BACKWARD_RIGHT;
					break;
				case 3:
					actionsToTake[GRIDWORLD_ROWS - i - 1][j][k] = FORWARD_LEFT;
					break;
				case 4:
					actionsToTake[GRIDWORLD_ROWS - i - 1][j][k] = FORWARD_RIGHT;
					break;
				case 5:
					actionsToTake[GRIDWORLD_ROWS - i - 1][j][k] = BACKWARD_STILL;
					break;
				case 6:
					actionsToTake[GRIDWORLD_ROWS - i - 1][j][k] = FORWARD_STILL;
					break;
				}
			}
		}
	}

	/*for (int i = 0; i < GRIDWORLD_ROWS; i++) {
	for (int j = 0; j < GRIDWORLD_COLUMNS; j++) {
	cout << valuesForStates[GRIDWORLD_ROWS - i - 1][j][5] << " ";
	}
	cout << "\n";
	}
	cout << "\n";

	for (int i = 0; i < GRIDWORLD_ROWS; i++) {
	for (int j = 0; j < GRIDWORLD_COLUMNS; j++) {
	cout << actionsToTake[GRIDWORLD_ROWS - i - 1][j][5] << " ";
	}
	cout << "\n";
	}
	cout << "\n";*/

	return valuesForStates;
}

//value iteration
void optimal_value_iteration(double gridWorld[][GRIDWORLD_COLUMNS][12], double errorProbability, double discountFactor, state current, state goal) {
	ACTIONS*** holdPolicies = new ACTIONS**[GRIDWORLD_ROWS];
	for (int i = 0; i < GRIDWORLD_ROWS; i++) {
		holdPolicies[i] = new ACTIONS*[GRIDWORLD_COLUMNS];
		for (int j = 0; j < GRIDWORLD_COLUMNS; j++) {
			holdPolicies[i][j] = new ACTIONS[12];
		}
	}

	double*** holdValues = new double**[GRIDWORLD_ROWS];
	for (int i = 0; i < GRIDWORLD_ROWS; i++) {
		holdValues[i] = new double*[GRIDWORLD_COLUMNS];
		for (int j = 0; j < GRIDWORLD_COLUMNS; j++) {
			holdValues[i][j] = new double[12];
			for (int k = 0; k < 12; k++) {
				holdValues[i][j][k] = 0;
			}
		}
	}

	double*** valueForStates;
	double*** tmpPointerValues;
	bool notSame = true;

	//maximize values for each state
	//stop until values for any state do not go pass some delta
	while (notSame == true) {

		valueForStates = change_values(holdPolicies, holdValues, gridWorld, errorProbability, discountFactor);

		bool check = false;
		for (int i = 0; i < GRIDWORLD_ROWS; i++) {
			for (int j = 0; j < GRIDWORLD_COLUMNS; j++) {
				for (int k = 0; k < 12; k++) {
					if (abs(valueForStates[GRIDWORLD_ROWS - i - 1][j][k] - holdValues[GRIDWORLD_ROWS - i - 1][j][k]) > 0.01) {
						check = true;
					}
				}
			}
		}
		notSame = check;

		tmpPointerValues = holdValues;
		for (int i = 0; i < GRIDWORLD_ROWS; i++) {
			for (int j = 0; j < GRIDWORLD_COLUMNS; j++) {
				delete[] tmpPointerValues[i][j];
			}
			delete[] tmpPointerValues[i];
		}
		delete[] tmpPointerValues;
		holdValues = valueForStates;
	}

	
	generate_and_plot(holdPolicies, current, errorProbability, goal);
	cout << valueForStates[GRIDWORLD_ROWS - 4 - 1][1][6] << "\n";

	for (int i = 0; i < GRIDWORLD_ROWS; i++) {
		for (int j = 0; j < GRIDWORLD_COLUMNS; j++) {
			delete[] holdPolicies[i][j];
		}
		delete[] holdPolicies[i];
	}
	delete[] holdPolicies;
}

//testing part goes here
int main(int argc, char** argv) {
	srand(time(NULL));
	
	double gridWorld[GRIDWORLD_ROWS][GRIDWORLD_COLUMNS][12];
	/*{ 
	{-100,	-100,	-100,		-100,	-100,		-100},
	{-100,	0,		-10,		1,		-10,		-100},
	{-100,	0,		-10,		0,		-10,		-100},
	{-100,	0,		-10,		0,		-10,		-100},
	{-100,	0,		0,			0,		0,			-100},
	{-100,	-100,	-100,		-100,	-100,		-100},
	};*/

	for (int k = 0; k < 12; k++) {
		gridWorld[0][0][k] = -100;
		gridWorld[0][1][k] = -100;
		gridWorld[0][2][k] = -100;
		gridWorld[0][3][k] = -100;
		gridWorld[0][4][k] = -100;
		gridWorld[0][5][k] = -100;

		gridWorld[1][0][k] = -100;
		gridWorld[1][1][k] = 0;
		gridWorld[1][2][k] = -10;
		gridWorld[1][3][k] = 0;//1;
		gridWorld[1][4][k] = -10;
		gridWorld[1][5][k] = -100;

		gridWorld[2][0][k] = -100;
		gridWorld[2][1][k] = 0;
		gridWorld[2][2][k] = -10;
		gridWorld[2][3][k] = 0;
		gridWorld[2][4][k] = -10;
		gridWorld[2][5][k] = -100;

		gridWorld[3][0][k] = -100;
		gridWorld[3][1][k] = 0;
		gridWorld[3][2][k] = -10;
		gridWorld[3][3][k] = 0;
		gridWorld[3][4][k] = -10;
		gridWorld[3][5][k] = -100;

		gridWorld[4][0][k] = -100;
		gridWorld[4][1][k] = 0;
		gridWorld[4][2][k] = 0;
		gridWorld[4][3][k] = 0;
		gridWorld[4][4][k] = 0;
		gridWorld[4][5][k] = -100;

		gridWorld[5][0][k] = -100;
		gridWorld[5][1][k] = -100;
		gridWorld[5][2][k] = -100;
		gridWorld[5][3][k] = -100;
		gridWorld[5][4][k] = -100;
		gridWorld[5][5][k] = -100;
	}

	gridWorld[1][3][5] = 1;
	gridWorld[1][3][6] = 1;
	gridWorld[1][3][7] = 1;

	state current;
	current.x = 1;
	current.y = 4;
	current.clock = 6;

	state goal;
	goal.x = 3;
	goal.y = 4;

	double errorProbability = 0.25;
	double discountFactor = 0.9;

	ACTIONS*** actionPolicies = get_initial_actions(goal);
	generate_and_plot(actionPolicies, current, errorProbability, goal);

	chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now();
	optimal_policy_iteration(gridWorld, errorProbability, discountFactor, current, goal);
	chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now();

	chrono::duration<double> timeSpan = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
	cout << "Time it took to execute policy iteration in seconds: " << timeSpan.count() << "\n";

	t1 = chrono::high_resolution_clock::now();
	optimal_value_iteration(gridWorld, errorProbability, discountFactor, current, goal);
	t2 = chrono::high_resolution_clock::now();

	timeSpan = chrono::duration_cast<chrono::duration<double>>(t2 - t1);
	cout << "Time it took to execute value iteration in seconds: " << timeSpan.count() << "\n";
	return 0;
}
