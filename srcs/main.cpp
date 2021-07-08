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

std::pair<__uint64_t,__uint64_t> uint128_encode(__uint128_t src)
{
    constexpr const __uint128_t bottom_mask = (__uint128_t{1} << 64) - 1;
    constexpr const __uint128_t top_mask = ~bottom_mask;
    return { src & bottom_mask, (src & top_mask) >> 64 };
}

__uint128_t uint128_decode(__uint64_t src1, __uint64_t src2)
{
    return (__uint128_t{src2} << 64) | src1;
}

__uint128_t	board_masks[9] = {	uint128_decode(0x1c0e07, 0x0),
								uint128_decode(0xe07038, 0x0),
								uint128_decode(0x70381c0, 0x0),
								uint128_decode(0xe07038000000, 0x0),
								uint128_decode(0x70381c0000000, 0x0),
								uint128_decode(0x381c0e00000000, 0x0),
								uint128_decode(0x81c0000000000000, 0x703),
								uint128_decode(0xe00000000000000, 0x381c),
								uint128_decode(0x7000000000000000, 0x1c0e0)
							};

struct State
{
	State():
	_boards{0, 0}, _wins{0, 0}
	{};

	State(const State& src):
	_boards{src._boards[CR], src._boards[CL]},
	_wins{src._wins[CR], src._wins[CL]}
	{}; 

	~State() {}

	State&	operator=(const State& src) {
		if (this != &src) {
			_boards[0] = src._boards[1];
			_boards[0] = src._boards[1];
			_wins[0] = src._wins[1];
			_wins[0] = src._wins[1];
		}
		return *this;
	}


	void	set_marking(bool player, int x, int y) { // sets current player's marking on (x, y) position
		_boards[player] = (_boards[player] | ((__uint128_t)1 << (x * BOARD_SZ + y)));
	}

	bool	is_marking(bool player, int x, int y) { // return true if player's (x, y) position is marked and false otherwise
		return (_boards[player] & ((__uint128_t)1 << (x * BOARD_SZ + y)));
	}

	// get square for x, y coordinates
	static __uint16_t	which_square(__uint16_t x, __uint16_t y) {
		return x / SQ_SZ * SQ_SZ + y / SQ_SZ;
	}
	bool	sq_is_win(bool player, int x, int y) { // returns true if player wins square
		// set the position at the top right hand corner of the square
		cout << "x " << x << " y " << y << endl;
		x = x / SQ_SZ * SQ_SZ;
		y = y / SQ_SZ * SQ_SZ;

		cout << "x " << x << " y " << y << endl;
		// horizontal check
		for (int i = 0 ; i < SQ_SZ ; i++) {
			for (int j = 0 ; j < SQ_SZ ; j++) {
				if (!is_marking(player, x + i, y + j))
					break;
				else if (j == 2)
					return true;
			}
		}

		// vertical check
		for (int i = 0 ; i < SQ_SZ ; i++) {
			for (int j = 0 ; j < SQ_SZ ; j++) {
				if (!is_marking(player, x + j, y + i))
					break;
				else if (j == 2)
					return true;
			}
		}

		// diagonal check
		if ((is_marking(player, x, y) && is_marking(player, x + 1, y + 1) && is_marking(player, x + 2, y + 2)) ||\
			(is_marking(player, x + 2, y) && is_marking(player, x + 1, y + 1) && is_marking(player, x, y + 2)))
				return true;
		return false;
	}


	// get the possible moves following current state
	// x and y are the coordiates of the last move of the opposing player
	// does not work if the opponent hasn't played before
	// each set bit corresponds to a possible position
	__uint128_t			get_possible_moves(int x, int y) {
		// every set bit in possible_moves is an authorized mode for this situation
		__uint128_t	possible_moves = 0;
		
		// get the current square
		__uint128_t	current_square = (x / SQ_SZ * SQ_SZ + y / SQ_SZ);

		// if the square is already finished, return all empty cells on unfinished squares
		if (_wins[CR] & ((__uint128_t)1 << current_square) ||\
			_wins[CL] & ((__uint128_t)1 << current_square)) {
				for (__uint16_t i = 0 ; i < 9 ; i++) {
					// if current square is not finished
					if (!(_wins[CR] & ((__uint16_t)1 << i)) &&\
						!(_wins[CL] & ((__uint16_t)1 << i))) {
							// set x1 and y1 to the beginning of the square
							__uint128_t x1 = i / SQ_SZ * SQ_SZ;
							__uint128_t y1 = i % SQ_SZ * SQ_SZ;

							// iterate through every cell in the square
							for (__uint128_t j = 0 ; j < SQ_SZ ; j++) {
								for (__uint128_t k = 0 ; k < SQ_SZ ; k++) {
									// if cell is empty, set it as a possible move
									if (!is_marking(CR, x1 + j, y1 + k) && !is_marking(CL, x1 + k, y1 + j))
										possible_moves = (possible_moves | ((__uint128_t)1 << (x1 * BOARD_SZ + y1)));
								}
							}
					}
				}
		}
		// if the square is unfinished, return all empty cells of that square
		else {
			// get top left hand corner position of current square
			__uint128_t	x1 = x / SQ_SZ * SQ_SZ;
			__uint128_t	y1 = y / SQ_SZ * SQ_SZ;

			// iterate through every cell in the square, each empty cell encountered is set as a possible move 
			for (__uint128_t j = 0 ; j < SQ_SZ ; j++) {
				for (__uint128_t k = 0 ; k < SQ_SZ ; k++) {
					// if cell is empty, set it as a possible move
					if (!is_marking(CR, x1 + j, y1 + k))
						possible_moves = (possible_moves | ((__uint128_t)1 << ((x1 + j) * BOARD_SZ + (y1 + k))));
				}
			}
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
};


// int main()
// {
// 	State	state;
// 	state.player = !state.player;
// 	state.set_marking(3, 2);
// 	state.set_marking(4, 1);
// 	state.set_marking(5, 0);
// 	cout << state << endl;
// 	cout << (state.sq_is_win(3, 2) ? "W " : "N ");
// 	cout << endl;
//     // game loop
//     // while (1) {
//     //     int opponentRow;
//     //     int opponentCol;
//     //     cin >> opponentRow >> opponentCol; cin.ignore();
//     //     int validActionCount;
//     //     cin >> validActionCount; cin.ignore();
//     //     for (int i = 0; i < validActionCount; i++) {
//     //         int row;
//     //         int col;
//     //         cin >> row >> col; cin.ignore();
//     //     }

//     //     // Write an action using cout. DON'T FORGET THE "<< endl"
//     //     // To debug: cerr << "Debug messages..." << endl;

//     //     cout << "0 0" << endl;
//     // }
// }