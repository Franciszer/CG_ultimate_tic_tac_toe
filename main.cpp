#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#define ME 0
#define OPP 1

#define BOARD_SZ 9 // board BOARD_SZ

using namespace std;
using std::ostream;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/


struct State
{
	State(bool player = ME):
	_boards{0, 0}, _wins{0, 0},
	_player(player)
	{};

	State(const State& src):
	_boards{src._boards[ME], src._boards[OPP]},
	_wins{src._wins[ME], src._wins[OPP]},	
	_player(src._player)
	{}; 

	~State() {}

	State&	operator=(const State& src) {
		if (this != &src) {
			_boards[0] = src._boards[1];
			_boards[0] = src._boards[1];
			_wins[0] = src._wins[1];
			_wins[0] = src._wins[1];
			_player = src._player;
		}
		return *this;
	}


	void	set_marking(int x, int y) { // sets current player's marking on (x, y) position
		_boards[_player] = (_boards[_player] | ((__uint128_t)1 << (x * BOARD_SZ + y)));
	}
	bool	is_marking(bool player, int x, int y) { // return true if player's (x, y) position is marked and false otherwise
		return (_boards[player] & ((__uint128_t)1 << (x * BOARD_SZ + y)));
	}
	friend ostream&    operator<<(ostream& os, State const &state) {
		for (int i = 0 ; i < BOARD_SZ ; i++) {
			for (int j = 0 ; j < BOARD_SZ ; j++) {
				if ((state._boards[state._player] & ((__uint128_t)1 << (i * BOARD_SZ + j))))
					os << '1';
				else
					os << '0';
				os << ' ';
			}
			if (i != BOARD_SZ - 1)
				os << endl;
		}
		return os;
	}

    __uint128_t		_boards[2] = {0};	// 1 map for player's and opponent's markings
    uint16_t		_wins[2] = {0};		// map of player's winning maps
	bool			_player;			// current player
};


int main()
{
	State	state;
	state.set_marking(0, 0);
	cout << state << endl;
    // game loop
    // while (1) {
    //     int opponentRow;
    //     int opponentCol;
    //     cin >> opponentRow >> opponentCol; cin.ignore();
    //     int validActionCount;
    //     cin >> validActionCount; cin.ignore();
    //     for (int i = 0; i < validActionCount; i++) {
    //         int row;
    //         int col;
    //         cin >> row >> col; cin.ignore();
    //     }

    //     // Write an action using cout. DON'T FORGET THE "<< endl"
    //     // To debug: cerr << "Debug messages..." << endl;

    //     cout << "0 0" << endl;
    // }
}