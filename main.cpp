#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#define CR 0
#define CL 1

#define BOARD_SZ 9 // size of board
#define SQ_SZ 3 // size of a square

using namespace std;
using std::ostream;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/


struct State
{
	State(bool player = CR):
	_boards{0, 0}, _wins{0, 0},
	_player(player)
	{};

	State(const State& src):
	_boards{src._boards[CR], src._boards[CL]},
	_wins{src._wins[CR], src._wins[CL]},	
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

	bool	sq_is_win(int x, int y) { // returns true if _player wins square
		// set the position at the top right hand corner of the square
		x /= SQ_SZ;
		y /= SQ_SZ;

		// horizontal check
		for (int i = 0 ; i < SQ_SZ ; i++) {
			for (int j = 0 ; j < SQ_SZ ; j++) {
				cout << "pos " << x + i << ' ' << y + j << endl;
				if (!is_marking(_player, x + i, y + j))
					break;
				else if (j == 2)
					return true;
			}
		}

		// vertical check
		for (int i = 0 ; i < SQ_SZ ; i++) {
			for (int j = 0 ; j < SQ_SZ ; j++) {
				if (!is_marking(_player, x + j, y + i))
					break;
				else if (j == 2)
					return true;
			}
		}
		return false;
	}

	// get the possible moves following current state
	// x and y are the coordiates of the last move of the opposing player
	// does not work if the opponent hasn't played before
	// each set bit corresponds to a possible position
	__uint128_t			get_possible_moves(int x, int y) {
		__uint128_t	possible_moves = 0;
		
		// get the current square
		x /= SQ_SZ;
		y /= SQ_SZ;

		// if the square is already won by either player, return all empty cells on unfinished squares
		if (_wins[CR] & ((__uint128_t)1 << (x * SQ_SZ + y)) ||\
			_wins[CL] & ((__uint128_t)1 << (x * SQ_SZ + y))) {
				for (__uint16_t i = 0 ; i < 9 ; i++) {
					// if current square is not finished
					if (!(_wins[CR] & ((__uint16_t)1 << i)) &&\
						!(_wins[CL] & ((__uint16_t)1 << i))) {
							// set x and y to the beginning of the square
							x = i / SQ_SZ * BOARD_SZ;
							y = i % SQ_SZ * BOARD_SZ;
					}
				}
			return possible_moves;
		}
		return possible_moves;
	}

	friend ostream&    operator<<(ostream& os, State const &state) {
		for (int i = 0 ; i < BOARD_SZ ; i++) {
			for (int j = 0 ; j < BOARD_SZ ; j++) {
				if ((state._boards[CR] & ((__uint128_t)1 << (i * BOARD_SZ + j))))
					os << 'X';
				else if ((state._boards[CL] & ((__uint128_t)1 << (i * BOARD_SZ + j))))
					os << 'O';
				else
					os << '-';
				os << ' ';
			}
			if (i != BOARD_SZ - 1)
				os << endl;
		}
		return os;
	}

    __uint128_t		_boards[2];	// 1 map for player's and opponent's markings
    __uint16_t		_wins[2];		// map of player's winning maps
	bool			_player;			// current player
};


int main()
{
	State	state;
	state._player = !state._player;
	state.set_marking(0, 1);
	state.set_marking(1, 1);
	state.set_marking(2, 1);
	cout << state << endl;
	cout << (state.sq_is_win(0, 2) ? "W " : "N ");
	cout << endl;
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