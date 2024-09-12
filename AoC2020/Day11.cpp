#include <iostream>
#include <string> 
#include <vector>
#include <fstream>
using namespace std;


vector<string> readFile(string str) {
	vector< string > input;
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

struct Seat {
    int row;
    int col; 
    Seat(const int r,const int c) : row(r) , col(c) {}
};

Seat operator*(int scalar, const Seat& seat){
    return Seat(seat.row *scalar, seat.col *scalar);
}
Seat operator+(const Seat& s, const Seat& u) {
    return Seat(s.row +u.row, s.col +u.col);
}


void print_grid( vector<string>& grid){
    for(string row : grid){
        cout<<row<<endl;
    }
}

bool in_bounds(int row, int col , int rows, int cols){
    return row>=0 && col >= 0 && row < rows && col < cols;
}

bool in_bounds(Seat s, int rows, int cols) {
    return s.row>=0 && s.col >= 0 && s.row < rows && s.col < cols;
}

bool are_equal(vector<string>& current, vector<string>& previous ){
    //Useful for the first step ( and some edge cases I suppose)
    if(current.empty() || previous.empty()){
        return false;
    }else if( !(current.size() == previous.size() && current[0].size() == previous[0].size() ) ){
        return false;
    } else {
    //Check each element of the grid, if one differ return false, otherwise return true        
        for( int i = 0 ; i<current.size() ; i++) {
            for( int j = 0 ; j < current[0].size() ; j++){
                if(current[i][j] != previous[i][j] ){
                    return false;
                }
            }
        }
        return true;
    }
}

bool change_seat_state(vector<string>& grid, int row, int col){
    if( grid[row][col] != '.'){
        vector<int> shifts = {-1,0,1};
        int number_of_occupied = 0;
        for( int row_shift: shifts) {
            for( int col_shift : shifts ){
                if( !( row_shift == 0 && col_shift ==0) && 
                    in_bounds(row + row_shift, col + col_shift, grid.size(), grid[0].size()) ){
                        if(grid[row + row_shift][col+col_shift] == '#' ) {number_of_occupied ++;}
                }
            }
        }
        return (grid[row][col] =='L' && number_of_occupied == 0) || (grid[row][col] =='#'&& number_of_occupied >=4);

    } else{
        return false;
    }
}

bool change_seat_state_part_2(vector<string>& grid, int row, int col) {
    int rows = grid.size();
    int cols = grid[0].size();
    if (grid[row][col] != '.') {
        vector<Seat> neighbouring_seats{
            Seat(0, 1), Seat(1, 0), Seat(0, -1), Seat(-1, 0),
            Seat(1, 1), Seat(-1, -1), Seat(-1, 1), Seat(1, -1)
        };
        int number_of_occupied = 0;
        for (Seat nbr : neighbouring_seats) {
            int step = 1;
            // Go until we find the first seat (no seat is found)
            while (in_bounds(row + step * nbr.row, col + step * nbr.col, rows, cols) &&
                   grid[row + step * nbr.row][col + step * nbr.col] == '.') {
                step++;
            }
            // Check what actually is the case. If we've seen an occupied seat then we increase the counter.
            if (in_bounds(row + step * nbr.row, col + step * nbr.col, rows, cols) &&
                grid[row + step * nbr.row][col + step * nbr.col] == '#') {
                number_of_occupied++;
            }
        }
        return (grid[row][col] == 'L' && number_of_occupied == 0) || (grid[row][col] == '#' && number_of_occupied >= 5);
    } else {
        return false;
    }
}


int number_of_occupied_seats(vector<string>& grid){
    int number_of_occupied=0;
    for( string row : grid) {
        for( char seat : row) {
            if( seat == '#') { number_of_occupied++; }
        }
    }
    return number_of_occupied;
}
void simulate( vector<string> grid ){ 
    vector<string> previous;
    int step=0;
    while (!are_equal(grid,previous) ){
        cout<< "Step: "<< step << endl;
        previous = grid;
        vector<Seat> changed_seats; 
        for( int i = 0 ; i < grid.size() ; i++) {
            for( int j = 0 ; j < grid[0].size() ; j++) {
                //Check if the state of the seat should change
                if( change_seat_state_part_2(grid,i,j)){//Change this line depending on what part it is
                    Seat new_seat = Seat(i,j);
                    changed_seats.push_back(new_seat);
                }
            }
        }
        //Change the seats simultaneously
        for(Seat seat : changed_seats) {
            grid[seat.row][seat.col] == '#' ? grid[seat.row][seat.col] = 'L' :grid[seat.row][seat.col] = '#';
        }
        step++;
    }
    cout<< number_of_occupied_seats(grid) <<endl;
}

int main() {
	string input = "input.txt";
	vector<string> grid = readFile(input);
    simulate(grid);
}