#include "uttt.hpp"

#include <stdio.h>

#define FLT_MIN 1.17549435E-38F


/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/


Node*	memory  = new Node[5000000];

Node*	current_address = memory;
bool	pl_mark;


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

uint_fast8_t    popcnt_u128 (__uint128_t n)
{
    const uint64_t      n_hi    = n >> 64;
    const uint64_t      n_lo    = n;
	#ifdef __MACH__
    const uint_fast8_t  cnt_hi  = __popcount<__uint64_t>(n_hi);
    const uint_fast8_t  cnt_lo  = __popcount<__uint64_t>(n_lo);
	#elif __linux__
    const uint_fast8_t  cnt_hi  = __builtin_popcountll(n_hi);
    const uint_fast8_t  cnt_lo  = __builtin_popcountll(n_lo);
	#endif
    const uint_fast8_t  cnt     = cnt_hi + cnt_lo;

    return  cnt;
}

State::State():
_boards{0, 0}
{};

State::State(const State& src):
_boards{src._boards[CR], src._boards[CL]}
{};

State::~State() {}

State&	State::operator=(const State& src) {
	if (this != &src) {
		_boards[0] = src._boards[1];
		_boards[0] = src._boards[1];
	}
	return *this;
}


void	State::set_marking(bool player, int x, int y) { // sets current player's marking on (x, y) position
	_boards[player] = (_boards[player] | ((__uint128_t)1 << (x * BOARD_SZ + y)));
}

bool	State::is_marking(bool player, int x, int y) { // return true if player's (x, y) position is marked and false otherwise
	return (_boards[player] & ((__uint128_t)1 << (x * BOARD_SZ + y)));
}

// get square for x, y coordinates
__uint8_t	State::which_square(__uint16_t x, __uint16_t y) {
	return x / SQ_SZ * SQ_SZ + y / SQ_SZ;
}

__uint8_t	State::which_square(__uint128_t move) {
	for (auto i = 0 ; i < 9 ; i++)
		if (move & board_masks[i])
			return i;
	return 10; // error value
}

void	State::set_sq_as_won(bool player, __uint8_t sq) {
	_boards[player] |= board_masks[sq];
	// _boards[!player] = _boards[!player] | (board_masks[sq] ^ fullmap_mask); // not necessary, remove later
}

// returns true if square is won by player
// call this function when you just inserted an element, to check if that move won the square
// otherwise, call sq_is_finished
bool	State::sq_is_win(bool player, __uint8_t x, __uint8_t y) {
	// set coordinates to the top left hand corner of the square
	x -= x % SQ_SZ;
	y -= y % SQ_SZ;

	// move the square to the beginning of board and remove all of the other set bits
	__uint128_t	curr_sq = (_boards[player] >> (x * BOARD_SZ + y)) & board_masks[0];

	// check if sq is in a finished configuration
	for (auto i = 0 ; i < 8 ; i++) {
		// cerr << "popcnt: " << popcnt_u128(curr_sq & square_masks[i]) << endl;
		if (popcnt_u128((curr_sq & square_masks[i])) == 3)
			return true;
	}

	// if that is not the case, the square is not finished
	return false;
}

bool	State::sq_is_finished(__uint8_t sq) {
	// cerr << __popcount<__uint8_t>(board_masks[sq] & _boards[CR]) << endl;
	return ((board_masks[sq] & _boards[CR]) == board_masks[sq] ||\
			(board_masks[sq] & _boards[CL]) == board_masks[sq]);
}

// get the possible moves following current state
// sq is the number of the square to check
// does not work if the opponent hasn't played before
// each set bit corresponds to a possible position
__uint128_t			State::get_possible_moves(__uint8_t sq) {
	__uint128_t	possible_moves = 0;

	if (sq == 10 || sq_is_finished(sq)){
		possible_moves = ~(_boards[CR] | _boards[CL]) & fullmap_mask;
	}
	else
		possible_moves = ((~(_boards[CR] | _boards[CL])) & board_masks[sq]);

	return possible_moves;
}


// much like sq_is_win, but for the whole board
bool	State::is_win(bool player) {
	if (popcnt_u128(_boards[CR] | _boards[CL]) == 0x51)
		return true;
	for (auto i = 0 ; i < 8 ; i++)
		if (popcnt_u128(_boards[player] & board_win_masks[i]) == 0x1B)
			return true;
	return false;
}
// when all squares have been finished, determins who won, i.e who has won the most squares
bool	State::who_won() {
	return popcnt_u128(_boards[CR]) < popcnt_u128(_boards[CL]);
}

__uint8_t	which_bit(__uint128_t n) {
	for (__uint128_t i = 0 ; i < 81 ; i++) {
		if (n & ((__uint128_t)1 << i))
			return i;
	}
	// cerr << "which bit ERROR" << endl;
	// print_board(n);
	// cerr << "which error" << endl;
	return 82;
}

ostream&    operator<<(ostream& os, State const &state) {
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

void			print_board(__uint128_t b) {
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

void	mcts(Node *root, State& state, int timelimit) {
	struct timeval stop, start;
	gettimeofday(&start, NULL);
	do {
		bool	player = !pl_mark;
		Node*	leaf = traverse(root, state, player);
		expand_node(leaf, state);
		float	simulation_result = rollout(state, player, leaf->move);
		backpropagate(root, leaf, simulation_result);
		gettimeofday(&stop, NULL);
	}
	while ((stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec < timelimit);

}

__uint128_t		get_best_move(Node* root) {
	__uint8_t	most_visits = 0;
	__uint128_t	best_move = 0;
	for (auto i = 0 ; i < root->nb ; i++)
		if (root->children[i].visits > most_visits)
			best_move = root->children[i].move;
	return best_move;
}

void	backpropagate(Node* root, Node* node, float simulation_result) {
	while (node != root) {
		node->wins += simulation_result;
		node->visits++;
		node = node->parent;
	}
}

Node*	traverse(Node* node, State& state, bool &player) {
	do {
		// cerr << *node << endl;
		node = rollout_policy(node);
		// cerr << "1" << endl;
		// cerr << "node_address " << node << endl;
		state._boards[player] |= node->move;
		// cerr << "2" << endl;
		player = !player;
	} while (node->visits);
	return result(node);
}

Node*	rollout_policy(Node* node) {
	// cerr << "1" << endl;
	float	best_score = -1000000;
	Node*	best_child = NULL;
	float	score = 0;
	// cerr << "2" << endl;
	// cerr << "PARENT" << endl << *node << endl;
	for (auto i = 0 ; i < node->nb ; i++) {
		// cerr << "3" << endl;
		// cerr << "CHILD" << endl << node->children[i] << endl;
		score = node->children[i].wins / (node->children[i].visits + 1) + sqrt(2) * sqrt(log(node->visits + 1) / (node->children[i].visits + 1));
		// cerr << "3.5" << endl;
		// cerr << "idx: " << i << endl << "score: " << score << " best: " << best_score << endl;
		if (best_score < score) {
			best_score = score;
			best_child = &node->children[i];
			// cerr << "5" << endl;
		}
	}
	return best_child;
}

void	expand_node(Node* leaf, State& state) {
	__uint128_t	possible_moves =\
		state.get_possible_moves(State::which_square(leaf->move));
	leaf->nb = 0;
	leaf->children = current_address;
	for (__uint128_t i = 0 ; i < 81 ; i++) {
		__uint128_t	current_move = (__uint128_t)1 << i;
		if (possible_moves & current_move) {
			current_address->parent = leaf;
			current_address->move = current_move;
			current_address->visits = 0;
			current_address->wins = 0;
			leaf->children[leaf->nb] = *current_address;
			++leaf->nb;
			++current_address;
		}
	}
}

// pick child with most visits
Node*	result(Node* node) {
	return node;
}

__uint128_t	pick_random_move(const __uint128_t possible_moves) {
	default_random_engine 				generator;
	uniform_int_distribution<__uint128_t>	distribution(0, popcnt_u128(possible_moves) - 1);

	__uint128_t		move_idx = distribution(generator);
	__uint128_t			curr_idx = 0;
	__uint128_t i = 0;
	for (; i < 81 ; i++) {
		__uint128_t curr_move = (__uint128_t)1 << i;
		if (possible_moves & curr_move) {
			if (curr_idx == move_idx)
				return curr_move;
			curr_idx++;
		}
	}
	// print_board(possible_moves);
	// for (__uint128_t j = 81 ; j < 128 ; j++) {
	// 	if (possible_moves & ((__uint128_t)1 << j))
	// 		exit(0);
	// }
	// cerr << "ERROR" << endl;
	// cerr << (uint64_t)curr_idx << endl;
	// cerr << (uint64_t)move_idx << "/" << __popcount<__uint128_t>(possible_moves) - 1 << endl;;
	return 0;
}

float	rollout(State state, bool player, const __uint128_t& last_move) {

	__uint128_t move = last_move;
	int count = 0;
	while (true) {
		// cerr << "count: " << count++ << endl;
		// cerr << state << endl;
		__uint8_t	sq = identify_square(move);
		__uint128_t	possible_moves = state.get_possible_moves(sq);
		move = pick_random_move(possible_moves);
		state._boards[player] |= move;
		__uint8_t	set_bit = which_bit(move);
		__uint8_t	x = set_bit / BOARD_SZ;
		__uint8_t	y = set_bit % BOARD_SZ;

		sq = identify_square(move);
		if (state.sq_is_win(player, x, y) ||\
		(board_masks[sq] & (state._boards[CR] | state._boards[CL])) == board_masks[sq]) {
			cerr << "won sq " << (uint64_t)sq << endl;
			// cerr << "BEFORE" << endl;
			// cerr << state << endl;
			state.set_sq_as_won(player, sq);
			if (state.is_win(player)) {
				// cerr << "OVER" << endl;
				cerr << state << endl;
				exit(0);
				return player == pl_mark ? 1 : 0;
			}
		}
		// else
		// 	cerr << "pocnt: " << (uint64_t)popcnt_u128(board_masks[sq] & (state._boards[CR] | state._boards[CL])) << endl;
		player = !player;
	}
	return 0;
}

__uint8_t	identify_square(__uint128_t move) {
	for (auto i = 0 ; i < 9 ; i++)
		if (move & direction_masks[i])
			return i;
	return 10;
}

ostream&    operator<<(ostream& os, Node const &node) {
	os << "parent: " << node.parent << endl;
	os << "move: " << (__uint64_t)node.move << endl;
	os << "nb: " << (__uint64_t)node.nb << endl;
	os << "visits: " << node.visits << endl;
	os << "wins: " << node.wins << endl;

	return os;
}
