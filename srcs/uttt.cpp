#include "uttt.hpp"

#include <stdio.h>

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/


Node*	memory  = new Node[5000000];

Node*	current_address = memory;
bool	pl_mark;

extern unordered_map<State, Node*, StateHasher> g_map;

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

inline __uint8_t	which_sq(__uint128_t move) {
	for (auto i = 0 ; i < 9 ; i++)
		if (move & board_masks[i])
			return i;
	return 10; // error value
}

inline void	set_sq_as_won(State& state, bool player, __uint8_t sq) {
	state._boards[player] |= board_masks[sq];
}

inline void	set_sq_as_lost(State& state, bool player, __uint8_t sq) {
	state._boards[player] &= ~board_masks[sq];
}

inline bool	sq_is_win(__uint128_t board, uint8_t sq) {
	
	// index of the top left hand corner of the square
	__uint8_t	idx = 3 * ( 3 * sq - sq % 3 * 2 );

	// move the square to the beginning of board and remove all of the other set bits
	__uint128_t	curr_sq = (board >> idx) & board_masks[0];

	// cerr << "board" << endl;
	// print_board(curr_sq);
	// check if sq is in a finished configuration
	for (auto i = 0 ; i < 8 ; i++) {
		// cerr << "popcnt: " << popcnt_u128(curr_sq & square_masks[i]) << endl;
		if (popcnt_u128((curr_sq & square_masks[i])) == 3)
			return true;
	}

	// if that is not the case, the square is not finished
	return false;
}


inline bool	sq_is_draw(State& state, bool player, __uint8_t sq) {
	return ((state._boards[player] | state._boards[!player]) & board_masks[sq]) == board_masks[sq];
}

inline bool	sq_is_finished(const State& state, __uint8_t sq) {
	// cerr << __popcount<__uint8_t>(board_masks[sq] & _boards[CR]) << endl;
	return ((state._boards[CR] | state._boards[CL]) & board_masks[sq]) == board_masks[sq] ;
}

// get the possible moves following current state
// sq is the number of the square to check
// does not work if the opponent hasn't played before
// each set bit corresponds to a possible position
inline __uint128_t			get_possible_moves(const State& state, __uint8_t sq) {
	__uint128_t	possible_moves = 0;

	if (sq_is_finished(state, sq)) {
		possible_moves = ~(state._boards[CR] | state._boards[CL]) & fullmap_mask;
	}
	else
		possible_moves = ((~(state._boards[CR] | state._boards[CL])) & board_masks[sq]);

	return possible_moves;
}


// much like sq_is_win, but for the whole board
inline bool	is_win(const State& state, bool player) {
	for (auto i = 0 ; i < 8 ; i++)
		if ( popcnt_u128(state._boards[player] & board_win_masks[i]) == 27 &&
		popcnt_u128(state._boards[!player] & board_win_masks[i]) == 0) {
			return true;
		}
	return false;
}

inline bool	is_draw(const State& state) {
	return ( popcnt_u128(state._boards[CR] | state._boards[CL]) == (__uint128_t) 81 );
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

void		pboard(State& state, __uint128_t move, bool player, ostream& os) {
	for (uint32_t i = 0 ; i < 81 ; i++) {
		__uint128_t	current = (__uint128_t)1 << i;

		if (move == current)
			os << "\033[0;31m" << (player == CR ? "X " : "O " ) << "\033[0m";
		else if (state._boards[CR] & current) {
			if (state._boards[CL] & current)
				os << "D ";
			else
				os << "X ";
		}
		else if (state._boards[CL] & current)
			os << "O ";
		else
			os << "- ";
		
		if ( ( i + 1 ) % 9 == 0)
			os << endl;
		else if ( (i + 1) % 3 == 0 )
			os << " ";
		if ( i == 26 || i == 53)
			os << endl;
	}
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

void	mcts(Node *root, State& state, long timelimit) {
	struct timeval time_now{};
    gettimeofday(&time_now, nullptr);
    time_t msecs_time = (time_now.tv_sec * 1000) + (time_now.tv_usec / 1000);
	time_t t2;

	State	s2;
	int count = 0;
	do {
		cerr << count << "  " << (current_address - memory) << endl;
		++count;
		s2 = state;
		bool	player = pl_mark;
		Node*	leaf = traverse(root, s2, player);
		// cerr << "count: " << count << endl << "bit: " << (uint)which_bit(leaf->move) << endl << s2 << endl;
		// if (count == 10) break;
		cerr << "traverse passed" << endl;
		expand_node(leaf, s2);
		cerr << "expand_node passed" << endl;
		float	simulation_result = rollout(s2, player, leaf->move);
		backpropagate(root, leaf, simulation_result);
		cerr << "backpropagate passed" << endl;
		gettimeofday(&time_now, NULL);	
		t2 = (time_now.tv_sec * 1000) + (time_now.tv_usec / 1000);
	}
	while (t2 - msecs_time < timelimit);
	cerr << "count " << count << endl;
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
	while (node->visits) {
		node = rollout_policy(node);

		state._boards[player] |= node->move;
		player = !player;
	}
	return node;
}

Node*	rollout_policy(Node* node) {
	// cerr << "1" << endl;
	float	best_score = -1000000;
	Node*	best_child = NULL;
	float	score = 0;

	for (auto i = 0 ; i < node->nb ; i++) {
		// cerr << "3" << endl;
		// cerr << "CHILD" << endl << node->children[i] << endl;
		if (!node->children[i].visits)
			return &node->children[i];
		score = node->children[i].wins / (node->children[i].visits) + sqrt(2) * sqrt(log(node->visits + 1) / (node->children[i].visits ));
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
		get_possible_moves(state, which_sq(leaf->move));
	leaf->nb = 0;
	leaf->children = current_address;
	// cerr << "possible_moves" << endl;
	// print_board(possible_moves);
	for (__uint128_t i = 0 ; i < 81 ; i++) {
		__uint128_t	current_move = (__uint128_t)1 << i;
		if (possible_moves & current_move) {

			// cerr << "i: " << (uint64_t)i << endl;
			// cerr << "current_address: " << current_address << endl;
			// print_board(current_move);
			current_address->parent = leaf;
			current_address->move = current_move;
			current_address->visits = 0;
			current_address->wins = 0;
			++leaf->nb;
			++current_address;
		}
	}
	// cerr << "CHILDREN" << endl;
	// __uint128_t	child_moves = 0;
	// for (uint i = 0 ; i < leaf->nb ; i++)
	// 	child_moves |= leaf->children[i].move;
	// print_board(child_moves);
	// cerr << "end in expand_node" << endl;
	// exit(0);
}

std::mt19937 rng(std::random_device{}());
inline __uint128_t	pick_move(const __uint128_t possible_moves) {
	
	uint32_t	limit = popcnt_u128(possible_moves) - 1;
 
	uniform_int_distribution<uint32_t>
		distribution(0, limit);

	uint32_t		move_idx = distribution(rng);
	__uint128_t			curr_idx = 0;
	__uint128_t i = 0;
	// cerr << "rand  " << (uint64_t)move_idx << "/" << (uint64_t)limit << endl;
	for (; i < 81 ; i++) {
		__uint128_t curr_move = (__uint128_t)1 << i;
		if (possible_moves & curr_move) {
			if (curr_idx == move_idx)
				return curr_move;
			curr_idx++;
		}
	}
	return 0;
}

float	rollout(State state, bool player, __uint128_t move) {
	__uint8_t	sq;
	__uint128_t	possible_moves;
	bool		result;

	// int	count = 0;
	while (true) {
		// cout << count++ << endl;
		sq = id_sq(move);
		if (sq == 10) {
			cerr << "WTF SQ" << endl;
			exit(0);
		}
		// get all of the possible moves
		possible_moves = get_possible_moves(state, sq);

		// pick a random one
		move = pick_move(possible_moves);

		// apply the move to the board
		state._boards[player] |= move;
 
		// pboard(state, move, player, cout);
		// cout << endl;

		// get the square on which the new move was played
		sq = which_sq(move);
		result = sq_is_win(state._boards[player], sq);
		if (result) {
			// cout << "sq won |||" << (uint64_t)sq << endl;
			set_sq_as_won(state, player, sq);
			set_sq_as_lost(state, !player, sq);
		}
		else if ( ( result = sq_is_draw(state, player, sq) ) ) {
			// cerr << "sq drawn sq |||" << (uint64_t)sq << endl;
			set_sq_as_won(state, !player, sq);
			set_sq_as_won(state, player, sq);
		}

		if (result) {
			// check for terminal state 
			if (is_win(state, player)) {
				// cout << "IS WIN " << (player == CR ? "X" : "O") << endl;
				// cout << "X " << (uint32_t)popcnt_u128(state._boards[CR]) << endl;
				// print_board(state._boards[CR]);
				// cout << "Y " << (uint32_t)popcnt_u128(state._boards[CL]) << endl;
				// print_board(state._boards[CL]);
				return player == pl_mark ? 1 : 0;
			}
			if (is_draw(state)) {
				// cout << "IS DRAW" << endl;
				return 0.5;
			}
		}

		player = !player;
	}
	return 0;
}

inline __uint8_t	id_sq(__uint128_t move) {
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
