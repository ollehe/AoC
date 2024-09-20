#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;
// a 3 dimensionsal structure
struct Cube {
	int x;
	int y;
	int z;
	bool active;
	bool swap;
};

bool operator==(const Cube &u, const Cube &v) {
	return u.x == v.x && u.y == v.y && u.z == v.z;
}

vector<string> readFile(string str) {
	vector<string> input;
	ifstream inputFile(str);
	if (!inputFile.is_open()) {
		// Handle the error if the file couldn't be opened
		cerr << "Error opening the file." << endl;
		return input;
	}
	string line;
	while (getline(inputFile, line)) {
		input.push_back(line);
	}
	inputFile.close();
	return input;
}

int distance(Cube u, Cube v) {
	return max(abs(u.x - v.x), abs(u.y - v.y), abs(u.z - v.z));
}

void deactivate(vector<Cube> &active_cubes) {
	for (int i = 0; i < active_cubes.size(); i++) {
		int number_of_active_neighbors = 0;
		for (int j = i + 1; j < active_cubes.size(); j++) {
			if (distance(active_cubes[i], active_cubes[j]) == 1) {
				number_of_active_neighbors++;
			}
		}
		active_cubes[i].swap = !(number_of_active_neighbors == 2 ||
		                         number_of_active_neighbors == 3);
	}
}

vector<Cube> get_neighbors(Cube c) {
	int shifts[] = {-1, 0, 1};

	for (int x_shift : shifts) {
		for (int y_shift : shifts) {
			for (int z_shift : shifts) {
				if (!(x_shift == 0 && y_shift == 0 && z_shift == 0)) {
					Cube neighbor = {.x = x_shift + c.x,
					                 .y = y_shift + c.y,
					                 .z = z_shift + c.z,
					                 .activate = False,
					                 .swap = False};
				}
			}
		}
	}
}

vector<Cube> activate(vector<Cube> &active_cubes) {
	// First find all cubes that have 2 other cubes within distance 2
	vector<Cube> possible_cubes;
	for (int i = 0; i < active_cubes.size(); i++) {
		for (int j = j + 1; j < active_cubes.size(); j++) {
			for (int k = j + 1; k < active_cubes.size(); k++) {
				int d_ij = distance(active_cubes[i], active_cubes[j]);
				int d_jk = distance(active_cubes[j], active_cubes[k]);
				int d_ki = distance(active_cubes[k], active_cubes[i]);
				if (d_ij <= 2 && d_jk <= 2 && d_ki <= 2) {
					possible_cubes.push_back(active_cubes[i]);
				}
			}
		}
	}
	// Next for each possible cube, loop over all neighbors
	// and determine which cube should be activated
	for (Cube c &possible_cubes) {
		vector<Cube> neighbors = get_neighbors(possible_cubes);
	}
}

void simulate(int number_of_turns, vector<string> initial_configuration) {
	int turn = 0;
	vector<Cube> active_cubes;
	// Initalize
	for (int x = 0; x < initial_configuration.size(); x++) {
		for (int y = 0; y < initial_configuration[0].size(); y++) {
			if (initial_configuration[x][y] == '#') {
				Cube c = {
				    .x = x, .y = y, .z = 0, .active = true, .swap = false};
				active_cubes.push_back(c);
			}
		}
	}

	while (turn < number_of_turns) {
		vector<Cube> new_cubes;
		// First loop through all active and see which should be deactivated
		deactivate(active_cubes);
		// Then find all triples of cubes that such that all are within distance
		// 3 of each other
		vector<Cube> new_cubes = activate(active_cubes);
		for (Cube c : active_cubes) {
			if (!c.swap) {
				new_cubes.push_back(c);
			}
		}

		active_cubes = new_cubes;

		turn++;
	}
}

int main() {
	vector<string> initial_configuration = readFile("test.txt");

	return 0;
}