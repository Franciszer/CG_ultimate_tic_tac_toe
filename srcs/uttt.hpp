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
#include <cmath>
#include <unordered_map>
#include <stdint.h>
#include <random>
#include <chrono>
#include <sys/time.h>
#include <ctime>

using std::cout; using std::endl;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;

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

	friend ostream&    operator<<(ostream& os, State const &state);
    
    __uint128_t		_boards[2];	// 1 map for player's and opponent's markings
};

bool	is_draw(const State& state);
bool	is_win(const State& state, bool player);
bool	sq_is_finished(const State& state, __uint8_t sq);
bool	sq_is_win(__uint128_t board, __uint8_t sq);
bool	sq_is_draw(State& state, bool player, __uint8_t sq);

void	set_sq_as_won(State& state, bool player, __uint8_t sq);
void	set_sq_as_lost(State& state, bool player, __uint8_t sq);
__uint8_t	which_bit(__uint128_t n);
__uint8_t	which_sq(__uint128_t move);
__uint128_t			get_possible_moves(const State& state, __uint8_t sq);

struct Node {
	Node*		parent; 	// parent node
	__uint128_t	move;		// move that has to be applied to the parent node to get this child
	Node**		children; 	// first child address in memory
	float		visits;		// number of times the node was visited
	float		wins;		// number of times this node or its descendants won a game
};

void	mcts(Node *root, State& state, long timelimit);

void	backpropagate(Node* root, Node* node, float simulation_result);

Node*	traverse(Node* node, State& state, bool &player);

Node*	rollout_policy(Node* node);

void	expand_node(Node* leaf, State& state);

__uint128_t	pick_move(const __uint128_t possible_moves);

float	rollout(State state, bool player, __uint128_t last_move);

__uint128_t		get_best_move(Node* root);

ostream&    operator<<(ostream& os, Node const &node);

__uint8_t	id_sq(__uint128_t move);

void		pboard(State& state, __uint128_t move, bool player, ostream& os);

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

struct StateHasher {
    std::size_t operator()(const State& s) const {
        std::size_t h = 0;

        h ^= std::hash<int>{}(s._boards[CR])  + 0x9e3779b9 + (h << 6) + (h >> 2); 
        h ^= std::hash<int>{}(s._boards[CL])  + 0x9e3779b9 + (h << 6) + (h >> 2); 

        return h;
    }   
};

unordered_map<State, Node*, StateHasher>	g_map;

#endif