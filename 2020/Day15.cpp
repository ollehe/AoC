#include <iostream>
#include <map>
#include <tuple>
#include <vector>
using namespace std;

struct Number {
	int last_turn;
	int number_of_times;
	int age;
};

void play(vector<int> starting_numbers) {
	map<int, Number> memory_list;
	int most_recent_number = starting_numbers.back();
	int current_turn = starting_numbers.size();
	for (int i = 0; i < starting_numbers.size(); i++) {
		Number num = {.last_turn = i + 1, .number_of_times = 1, .age = 0};
		memory_list.insert({starting_numbers[i], num});
	}
	while (current_turn < 30000000) {
		current_turn++;
		// If that was the first time the number has been spoken, the current
		// player says 0.
		if (memory_list[most_recent_number].number_of_times == 1) {
			most_recent_number = 0;
			memory_list[0].number_of_times++;
			memory_list[0].age = current_turn - memory_list[0].last_turn;
			memory_list[0].last_turn = current_turn;
		} else {
			// Otherwise, the number had been spoken before; the current player
			// announces how many turns apart the number is from when it was
			// previously spoken.
			int prev = most_recent_number;
			most_recent_number = memory_list[most_recent_number].age;
			if (memory_list.count(most_recent_number) == 0) {
				memory_list[most_recent_number].number_of_times = 1;
				memory_list[most_recent_number].age = 0;
				memory_list[most_recent_number].last_turn = current_turn;
			} else {
				memory_list[most_recent_number].number_of_times++;
				memory_list[most_recent_number].age =
				    current_turn - memory_list[most_recent_number].last_turn;
				memory_list[most_recent_number].last_turn = current_turn;
			}
		}
		if (current_turn == 2020) {
			cout << most_recent_number << endl;
		}
	}
	cout << most_recent_number << endl;
}

int main() {
	vector<int> test = {0, 3, 6};
	vector<int> input = {14, 3, 1, 0, 9, 5};
	play(input);
	return 0;
}