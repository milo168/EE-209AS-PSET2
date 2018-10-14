#include <iostream>
#include <string>
#include <cstdlib>
#include <time.h>
#include <utility>
#include <vector>
using namespace std;

int GRIDWORLD_ROWS = 6;
int GRIDWORLD_COLUMNS = 6;

struct state {
	int x;
	int y;
	int clock;
};

enum ACTIONS {
	STILL = 1,
	FORWARD_STILL = 2,
	FORWARD_LEFT = 3,
	FORWARD_RIGHT = 4,
	BACKWARD_STILL = 5,
	BACKWARD_RIGHT = 6,
	BACKWARD_LEFT = 7
};

double still_helper(state current, state next) {
	int changeX = next.x - current.x;
	int changeY = next.y - current.y;
	int changeClock = next.clock - current.clock;

	if (changeX == 0 && changeY == 0 && changeClock == 0) {
		return 1.0;
	}

	return 0.0;
	
}

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

	if (abs(changeX) > 1 || abs(changeY) > 1 || abs(changeClock) > 2) {
		return 0.0;
	}

	if (changeClock == -1) {
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

	if (abs(changeX) > 1 || abs(changeY) > 1 || abs(changeClock) > 2) {
		return 0.0;
	}
	
	if (changeClock == -2) {
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

	if (abs(changeX) > 1 || abs(changeY) > 1 || abs(changeClock) > 2) {
		return 0.0;
	}

	if (changeClock == 2) {
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
	//	cout << possibleStates[i].first.x << " " << possibleStates[i].first.y << " " << possibleStates[i].first.clock << " " << possibleStates[i].second << "\n";
	//}

	//GENERATE THE NEXT STATE
	state next;
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

double get_reward(double** gridWorld, state current) {
	return gridWorld[GRIDWORLD_ROWS-current.y-1][current.x];
}

ACTIONS get_action(state current, int goalX, int goalY) {
	//For h in +/- x
	if (current.clock == 2 || current.clock == 3 || current.clock == 4 ||
		current.clock == 8 || current.clock == 9 || current.clock == 10) {
		if (abs(goalX - current.x) != 0) {
			if ((goalX < current.x && (current.clock == 2 || current.clock == 3 || current.clock == 4)) ||
				(goalX > current.x && (current.clock == 8 || current.clock == 9 || current.clock == 10))) {
				if (goalY - current.y == 0) {
					return BACKWARD_STILL;
				}
				else {
					if (current.clock == 2 || current.clock == 8) {
						return BACKWARD_LEFT;
					}
					else {
						return BACKWARD_RIGHT;
					}
				}
			}
			else {
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
			if (current.x == 0 || current.x == GRIDWORLD_COLUMNS - 1) {
				if ((current.x == 0 && (current.clock == 2 || current.clock == 3 || current.clock == 4)) ||
					(current.x == GRIDWORLD_COLUMNS - 1 && (current.clock == 8 || current.clock == 9 || current.clock == 10))) {
					if (goalY - current.y == 0) {
						return STILL;
					}
					else {
						if (current.clock == 2 || current.clock == 8) {
							return BACKWARD_LEFT;
						}
						else {
							return BACKWARD_RIGHT;
						}
					}
				}
				else {
					if (goalY - current.y == 0) {
						return STILL;
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
				if (goalY - current.y == 0) {
					return STILL;
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
	}

	//For h in +/- y
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

ACTIONS*** get_initial_actions(int goalX, int goalY) {
	ACTIONS*** init_actions_space = new ACTIONS**[GRIDWORLD_ROWS];
	for (int i = 0; i < GRIDWORLD_ROWS; i++) {
		init_actions_space[i] = new ACTIONS*[GRIDWORLD_COLUMNS];
		for (int j = 0; j < GRIDWORLD_COLUMNS; j++) {
			init_actions_space[i][j] = new ACTIONS[12];
		}
	}

	for (int i = 0; i < GRIDWORLD_ROWS; i++) {
		for (int j = 0; j < GRIDWORLD_COLUMNS; j++) {
			for (int k = 0; k < 12; k++) {
				state current;
				current.x = j;
				current.y = GRIDWORLD_ROWS - i - 1;
				current.clock = k;
				init_actions_space[GRIDWORLD_ROWS-i-1][j][k] = get_action(current, goalX, goalY);
 			}
		}
	}
	return init_actions_space;
}

void generate_and_plot(ACTIONS*** action_policies, state initial, double errorProbability, int goalX, int goalY) {
	int MAX_STEPS = GRIDWORLD_ROWS * GRIDWORLD_COLUMNS * 12 * 100;
	int count = 0;
	state generated = initial;
	vector<state> history;

	history.push_back(generated);
	while ((generated.x != goalX || generated.y != goalY) && count < MAX_STEPS) {
		generated = get_next_state(errorProbability, generated, action_policies[generated.y][generated.x][generated.clock]);
		history.push_back(generated);
		count++;
	}

	char** gridPrint = new char*[GRIDWORLD_ROWS];
	for (int i = 0; i < GRIDWORLD_ROWS; i++) {
		gridPrint[i] = new char[GRIDWORLD_COLUMNS];
		for (int j = 0; j < GRIDWORLD_COLUMNS; j++) {
			gridPrint[i][j] = '-';
		}
	}

	for (int i = 0; i < history.size(); i++) {
		gridPrint[history[i].y][history[i].x] = 'x';
		cout << history[i].x << " " << history[i].y << " " << history[i].clock << "\n";
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

int main(int argc, char** argv) {
	srand(time(NULL));

	state current;
	current.x = 2;
	current.y = 3;
	current.clock = 8;

	state goal;
	goal.x = 5;
	goal.y = 5;

	double errorProbability = 0.45;

	//get_next_state(errorProbability,current, BACKWARD_STILL);

	ACTIONS*** action_policies = get_initial_actions(goal.x, goal.y);
	/*for (int i = 0; i < GRIDWORLD_ROWS; i++) {
		for (int j = 0; j < GRIDWORLD_COLUMNS; j++) {
			cout << action_policies[GRIDWORLD_ROWS - i - 1][j][7] << " ";
		}
		cout << "\n";
	}*/
	generate_and_plot(action_policies, current, errorProbability, goal.x, goal.y);

	return 0;
}
