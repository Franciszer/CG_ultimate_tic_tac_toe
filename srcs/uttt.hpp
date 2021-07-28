#ifndef UTT_HPP

#define UTT_HPP

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#ifdef	__MACH__
#include <bit>
#elif __linux__
#include <nmmintrin.h>
#endif
#include <stdint.h>
#include <cmath>
#include <unordered_map>
#include <random>
#include <chrono>
#include <sys/time.h>
#include <ctime>
#include <sys/time.h>

using std::cout; using std::endl;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;

#define FLT_MIN 1.17549435E-38F

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

struct Node;
struct State;

std::pair<__uint64_t,__uint64_t> uint128_encode(__uint128_t src);

__uint128_t uint128_decode(__uint64_t src1, __uint64_t src2);

void			print_board(__uint128_t b);

uint_fast8_t    popcnt_u128 (__uint128_t n);

struct State
{
	State();

	State(const State& src); 

	~State();

	State&	operator=(const State& src);


	void	set_marking(bool player, int x, int y);

	bool	is_marking(bool player, int x, int y);

	// get square for x, y coordinates
	static __uint8_t	which_square(__uint16_t x, __uint16_t y);

	static __uint8_t	which_square(__uint128_t move);

	void	set_sq_as_won(bool player, __uint8_t sq);

	// returns true if square is won by player
	// call this function when you just inserted an element, to check if that move won the square
	// otherwise, call sq_is_finished
	bool	sq_is_win(bool player, __uint8_t x, __uint8_t y);

	bool	sq_is_finished(__uint8_t sq);

	// get the possible moves following current state
	// sq is the number of the square to check
	// does not work if the opponent hasn't played before
	// each set bit corresponds to a possible position
	__uint128_t			get_possible_moves(__uint8_t sq);

	friend ostream&    operator<<(ostream& os, State const &state);


	// much like sq_is_win, but for the whole board
	bool	is_win(bool player);

	// when all squares have been finished, determins who won, i.e who has won the most squares
	bool	who_won();
    
    __uint128_t		_boards[2];	// 1 map for player's and opponent's markings
};

__uint8_t	which_bit(__uint128_t n);

struct Node {
	Node*		parent; 	// parent node
	__uint128_t	move;		// move that has to be applied to the parent node to get this child
	Node*		children; 	// first child address in memory
	__uint8_t	nb;			// number of children
	float		visits;		// number of times the node was visited
	float		wins;		// number of times this node or its descendants won a game
};

void	mcts(Node *root, State& state, int timelimit);

void	backpropagate(Node* root, Node* node, float simulation_result);

Node*	traverse(Node* node, State& state, bool &player);

Node*	rollout_policy(Node* node);

void	expand_node(Node* leaf, State& state);
// pick child with most visits
Node*	result(Node* node);

__uint128_t	pick_random_move(const __uint128_t possible_moves);

float	rollout(State state, bool player, const __uint128_t& last_move);

__uint128_t		get_best_move(Node* root);

ostream&    operator<<(ostream& os, Node const &node);

__uint8_t	identify_square(__uint128_t move);

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

const __uint128_t	fullmap_mask = uint128_decode(0xFFFFFFFFFFFFFFFF, 0x1FFFF);

// directions from top left to bottom right
const __uint128_t	direction_masks[9] = {
	uint128_decode(0x1240000248000049, 0x0),
	uint128_decode(0x2480000490000092, 0x0),
	uint128_decode(0x4900000920000124, 0x0),
	uint128_decode(0x8000049000009200, 0x24),
	uint128_decode(0x92000012400, 0x49),
	uint128_decode(0x124000024800, 0x92),
	uint128_decode(0x9200001240000, 0x4900),
	uint128_decode(0x12400002480000, 0x9200),
	uint128_decode(0x24800004900000, 0x12400)
};

extern Node*	memory;

extern Node*	current_address;
extern bool		pl_mark;

#endif