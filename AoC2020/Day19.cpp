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

struct Rule {
	char id;
	vector<vector<Rule>> sub_rules;
}
bool operator ==(Rule r, Rule v) {
	return r.id == v.id;
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


vector<string> get_valid_messages(){
	return vector<string>();
}

void parse_rules(vector<string> input){
	for( string& line : input) {
		if(  )
	}


}




int main() {
	vector<string> initial_configuration = readFile("test.txt");

	return 0;
}