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
		if (current.clock == 4 || current.clock == 7 || current.clock == 10 || current.clock == 1) {
			if (current.clock == 4 && changeX == 1 * direction && changeY == 0) {
				return errorProbability;
			}
			else if (current.clock == 7 && changeX == 0 && changeY == -1 * direction) {
				return errorProbability;
			}
			else if (current.clock == 10 && changeX == -1 * direction && changeY == 0) {
				return errorProbability;
			}
			else if (current.clock == 1 && changeX == 0 && changeY == 1 * direction) {
				return errorProbability;
			}
		}
		else {
			if ((current.clock == 2 || current.clock == 3) && changeX == 0 && changeY == 1 * direction) {
				return errorProbability;
			}
			else if ((current.clock == 5 || current.clock == 6) && changeX == 1 * direction && changeY == 0) {
				return errorProbability;
			}
			else if ((current.clock == 8 || current.clock == 9) && changeX == 0 && changeY == -1 * direction) {
				return errorProbability;
			}
			else if ((current.clock == 11 || current.clock == 0) && changeX == -1 * direction && changeY == 0) {
				return errorProbability;
			}
		}
	}
	else if (changeClock == -1) {
		if (current.clock == 2 || current.clock == 5 || current.clock == 8 || current.clock == 11) {
			if (current.clock == 2 && changeX == 0 && changeY == 1 * direction) {
				return 1 - 2 * errorProbability;
			}
			else if (current.clock == 5 && changeX == 1 * direction && changeY == 0) {
				return 1 - 2 * errorProbability;
			}
			else if (current.clock == 8 && changeX == 0 && changeY == -1 * direction) {
				return 1 - 2 * errorProbability;
			}
			else if (current.clock == 11 && changeX == -1 * direction && changeY == 0) {
				return 1 - 2 * errorProbability;
			}
		}
		else {
			if ((current.clock == 3 || current.clock == 4) && changeX == 1 * direction && changeY == 0) {
				return 1 - 2 * errorProbability;
			}
			else if ((current.clock == 6 || current.clock == 7) && changeX == 0 && changeY == -1 * direction) {
				return 1 - 2 * errorProbability;
			}
			else if ((current.clock == 9 || current.clock == 10) && changeX == -1 * direction && changeY == 0) {
				return 1 - 2 * errorProbability;
			}
			else if ((current.clock == 0 || current.clock == 1) && changeX == 0 && changeY == 1 * direction) {
				return 1 - 2 * errorProbability;
			}
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
		if (current.clock == 2 || current.clock == 5 || current.clock == 8 || current.clock == 11) {
			if (current.clock == 2 && changeX == 1 * direction && changeY == 0) {
				return errorProbability;
			}
			else if (current.clock == 5 && changeX == 0 && changeY == -1 * direction) {
				return errorProbability;
			}
			else if (current.clock == 8 && changeX == -1 * direction && changeY == 0) {
				return errorProbability;
			}
			else if (current.clock == 11 && changeX == 0 && changeY == 1 * direction) {
				return errorProbability;
			}
		}
		else {
			if ((current.clock == 3 || current.clock == 4) && changeX == 0 && changeY == -1 * direction) {
				return errorProbability;
			}
			else if ((current.clock == 6 || current.clock == 7) && changeX == -1 * direction && changeY == 0) {
				return errorProbability;
			}
			else if ((current.clock == 9 || current.clock == 10) && changeX == 0 && changeY == 1 * direction) {
				return errorProbability;
			}
			else if ((current.clock == 0 || current.clock == 1) && changeX == 1 * direction && changeY == 0) {
				return errorProbability;
			}
		}
	}
	else if (changeClock == 1) {
		if (current.clock == 4 || current.clock == 7 || current.clock == 10 || current.clock == 1) {
			if (current.clock == 4 && changeX == 0 && changeY == -1 * direction) {
				return 1 - 2 * errorProbability;
			}
			else if (current.clock == 7 && changeX == -1 * direction && changeY == 0) {
				return 1 - 2 * errorProbability;
			}
			else if (current.clock == 10 && changeX == 0 && changeY == 1 * direction) {
				return 1 - 2 * errorProbability;
			}
			else if (current.clock == 1 && changeX == 1 * direction && changeY == 0) {
				return 1 - 2 * errorProbability;
			}
		}
		else {
			if ((current.clock == 5 || current.clock == 6) && changeX == 0 && changeY == -1 * direction) {
				return 1 - 2 * errorProbability;
			}
			else if ((current.clock == 8 || current.clock == 9) && changeX == -1 * direction && changeY == 0) {
				return 1 - 2 * errorProbability;
			}
			else if ((current.clock == 11 || current.clock == 0) && changeX == 0 && changeY == 1 * direction) {
				return 1 - 2 * errorProbability;
			}
			else if ((current.clock == 2 || current.clock == 3) && changeX == 1 * direction && changeY == 0) {
				return 1 - 2 * errorProbability;
			}
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
	double chance = (double)(100.0 * rand() / (RAND_MAX + 1.0)) + 1;
	vector<pair<state, double>> possibleStates;

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
	for (int i = 0; i < possibleStates.size(); i++) {
		cout << possibleStates[i].first.x << " " << possibleStates[i].first.y << " " << possibleStates[i].first.clock << " " << possibleStates[i].second << "\n";
	}
	state l;
	return l;
}

double get_reward(double** gridWorld, state current) {
	return gridWorld[GRIDWORLD_ROWS-current.x][GRIDWORLD_COLUMNS-current.y];
}

int main(int argc, char** argv) {
	srand(time(NULL));
	state current;
	current.x = 2;
	current.y = 2;
	current.clock = 1;
	get_next_state(0.25,current, BACKWARD_RIGHT);
	return 0;
}
