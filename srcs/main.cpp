#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <bit>

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
    return ((__uint128_t)src2 << 64) | src1;
}

const __uint128_t	board_masks[9] = {	uint128_decode(0x1c0e07, 0x0),
										uint128_decode(0xe07038, 0x0),
										uint128_decode(0x70381c0, 0x0),
										uint128_decode(0xe07038000000, 0x0),
										uint128_decode(0x70381c0000000, 0x0),
										uint128_decode(0x381c0e00000000, 0x0),
										uint128_decode(0x81c0000000000000, 0x703),
										uint128_decode(0xe00000000000000, 0x381c),
										uint128_decode(0x7000000000000000, 0x1c0e0)
									};

const __uint128_t	board_win_masks[8] = {	board_masks[0] | board_masks[1] | board_masks[2],
											board_masks[3] | board_masks[4] | board_masks[5],
											board_masks[6] | board_masks[7] | board_masks[8],

											board_masks[0] | board_masks[3] | board_masks[6],
											board_masks[1] | board_masks[4] | board_masks[7],
											board_masks[2] | board_masks[5] | board_masks[8],
											
											board_masks[0] | board_masks[4] | board_masks[8],
											board_masks[2] | board_masks[4] | board_masks[6]
										};

const __uint128_t	square_masks[8] {	uint128_decode(0x7, 0),
										uint128_decode(0xe00, 0),
										uint128_decode(0x1c0000, 0),
										uint128_decode(0x40201, 0),
										uint128_decode(0x80402, 0),
										uint128_decode(0x100804, 0),
										uint128_decode(0x100401, 0),
										uint128_decode(0x40404, 0)
									};

const __uint128_t	fullmap_mask = uint128_decode(0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF);

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
	static __uint8_t	which_square(__uint16_t x, __uint16_t y) {
		return x / SQ_SZ * SQ_SZ + y / SQ_SZ;
	}

	void	set_sq_as_won(bool player, __uint8_t diag) {
		_boards[player] = _boards[player] & board_masks[diag];
	}

	// returns true if square is won by player
	// call this function when you just inserted an element, to check if that move won the square
	// otherwise, call sq_is_finished
	bool	sq_is_win(bool player, __uint8_t x, __uint8_t y) {
		// set coordinates to the top left hand corner of the square
		x -= x % SQ_SZ;
		y -= y % SQ_SZ;

		// move the square to the beginning of board and remove all of the other set bits
		__uint128_t	curr_sq = (_boards[player] >> (x * BOARD_SZ + y)) & board_masks[0];

		// check if sq is in a finished configuration	
		for (auto i = 0 ; i < 8 ; i++) {
			// cerr << "popcnt: " << __popcount<__uint128_t>(curr_sq & square_masks[i]) << endl;
			if (__popcount<__uint128_t>((curr_sq & square_masks[i])) == 3)
				return true;
		}

		// if that is not the case, the square is not finished
		return false;
	}

	bool	sq_is_finished(__uint8_t sq) {
		// cerr << __popcount<__uint8_t>(board_masks[sq] & _boards[CR]) << endl;
		return (__popcount<__uint128_t>(board_masks[sq] & _boards[CR]) == 9 ||\
				__popcount<__uint128_t>(board_masks[sq] & _boards[CL]) == 9);
	}

	// get the possible moves following current state
	// sq is the number of the square to check
	// does not work if the opponent hasn't played before
	// each set bit corresponds to a possible position
	__uint128_t			get_possible_moves(__uint8_t sq) {
		__uint128_t	possible_moves = 0;

		if (sq_is_finished(sq))
			possible_moves = ~(_boards[CR] | _boards[CL]);
		else 
			possible_moves = ((~(_boards[CR] | _boards[CL])) & board_masks[sq]);

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
	static void			print_board(__uint128_t b) {
		for (int i = 0 ; i < BOARD_SZ ; i++) {
			for (int j = 0 ; j < BOARD_SZ ; j++) {
				if ((b & ((__uint128_t)1 << (i * BOARD_SZ + j))))
					cerr << '1';
				else
					cerr << '0';
				cerr << ' ';
			}
			if (i != BOARD_SZ - 1)
				cerr << endl;
		}
		cerr << endl;
	}

	// much like sq_is_win, but for the whole board
	bool	is_win(bool player) {
		for (auto i = 0 ; i < 8 ; i++)
			if (__popcount<__uint128_t>(_boards[player]) == 0x1B)
				return true;
		return false;
	}
	// when all squares have been finished, determins who won, i.e who has won the most squares
	bool	who_won() {
		return __popcount<__uint128_t>(_boards[CR]) < __popcount<__uint128_t>(_boards[CL]);
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