#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <set>
using namespace std;
// a 3 dimensionsal structure
struct Cube {
	int x;
	int y;
	int z;
	int w;
	char state;
	string id() const {
		return "(" + to_string(x) + "," + to_string(y) + "," + to_string(z) + "," + to_string(w) + ")";
	}


};

bool operator==(const Cube& u, const Cube& v) {
	return u.x == v.x && u.y == v.y && u.z == v.z && u.w == v.w;
}

struct CubeHash {
	size_t operator()(const Cube& c) const {
		return hash<string>()(c.id());
	}
};

// Debug print
ostream& operator << (ostream& os, const Cube& c) {
	os << "(" << c.x << "," << c.y << "," << c.z << ") state " << c.state;
	return os;
}
ostream& operator << (ostream& os, const unordered_set<Cube, CubeHash>& cubes) {
	for (const Cube& c : cubes) {
		os << c << endl;
	}
	return os;
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

int distance(const Cube& u, const Cube& v) {
	return max({ abs(u.x - v.x), abs(u.y - v.y), abs(u.z - v.z), abs(u.w - v.w) });
}

vector<Cube> get_neighbors(Cube c) {
	int shifts[] = { -1, 0, 1 };
	vector<Cube> neighbors; // Add this to store the neighbors

	for (int x_shift : shifts) {
		for (int y_shift : shifts) {
			for (int z_shift : shifts) {
				if (!(x_shift == 0 && y_shift == 0 && z_shift == 0)) {
					Cube neighbor = { .x = x_shift + c.x,
									  .y = y_shift + c.y,
									  .z = z_shift + c.z,
										.w = 0,
									  .state = '.' };
					neighbors.push_back(neighbor); // Push the neighbor to the vector
				}
			}
		}
	}
	return neighbors;
}

vector<Cube> get_temporal_neighbors(Cube c) {
	int shifts[] = { -1, 0, 1 };
	vector<Cube> neighbors; // Add this to store the neighbors

	for (int x_shift : shifts) {
		for (int y_shift : shifts) {
			for (int z_shift : shifts) {
				for (int t_shift : shifts) {
					if (!(x_shift == 0 && y_shift == 0 && z_shift == 0 && t_shift == 0)) {
						Cube neighbor = { .x = x_shift + c.x,
										  .y = y_shift + c.y,
										  .z = z_shift + c.z,
										  .w = t_shift + c.w,
										  .state = '.' };
						neighbors.push_back(neighbor); // Push the neighbor to the vector
					}
				}
			}
		}
	}
	return neighbors;
}


unordered_set<Cube, CubeHash> update(unordered_set<Cube, CubeHash>& active_cubes, bool is_temporal) {
	unordered_set<Cube, CubeHash> to_check = active_cubes;
	for (const Cube& c : active_cubes) {
		vector<Cube> neighbors;
		if (is_temporal) {
			neighbors = get_temporal_neighbors(c);
		}
		else {
			neighbors = get_neighbors(c);
		}
		for (Cube& nbr : neighbors) {
			if (to_check.find(nbr) == to_check.end()) {
				to_check.insert(nbr);
			}
		}
	}
	unordered_set<Cube, CubeHash> new_cubes;
	for (const Cube& c : to_check) {
		int active_neighbors = 0;
		for (const Cube& possible_neighbor : to_check) {
			if (distance(c, possible_neighbor) == 1 && possible_neighbor.state == '#') {
				active_neighbors++;
			}
		}
		if (c.state == '#' && (active_neighbors == 2 || active_neighbors == 3)) {
			new_cubes.insert(c);
		}
		else if (c.state == '.' && active_neighbors == 3) {
			Cube activated = c;
			activated.state = '#';
			new_cubes.insert(activated);
		}
	}
	return new_cubes;
}

void simulate(int number_of_turns, vector<string> initial_configuration, bool is_temporal) {
	int turn = 0;
	unordered_set<Cube, CubeHash> active_cubes;
	// Initalize
	for (int x = 0; x < initial_configuration.size(); x++) {
		for (int y = 0; y < initial_configuration[0].size(); y++) {
			if (initial_configuration[x][y] == '#') {
				Cube c = {
					.x = x, .y = y, .z = 0, .w = 0, .state = '#' };
				active_cubes.insert(c);
			}
		}
		//cout << endl;
	}
	//cout << "Inital states" << endl << active_cubes;
	while (turn < number_of_turns) {
		unordered_set<Cube, CubeHash> new_set = update(active_cubes, is_temporal);
		active_cubes = new_set;
		turn++;
		//cout << "turn " << turn << " number of active cubes " << active_cubes.size() << endl<< active_cubes;
	}
	cout << active_cubes.size();
}


int main() {
	vector<string> initial_configuration = readFile("test.txt");
	simulate(6, initial_configuration, true);
	return 0;
}