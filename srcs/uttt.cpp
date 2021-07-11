#include "uttt.hpp"

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

static inline   uint_fast8_t    popcnt_u128 (__uint128_t n)
{
    const uint64_t      n_hi    = n >> 64;
    const uint64_t      n_lo    = n;
    const uint_fast8_t  cnt_hi  = __builtin_popcountll(n_hi);
    const uint_fast8_t  cnt_lo  = __builtin_popcountll(n_lo);
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
	_boards[player] = _boards[player] & board_masks[sq];
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
	return (popcnt_u128(board_masks[sq] & _boards[CR]) == 9 ||\
			popcnt_u128(board_masks[sq] & _boards[CL]) == 9);
}

// get the possible moves following current state
// sq is the number of the square to check
// does not work if the opponent hasn't played before
// each set bit corresponds to a possible position
__uint128_t			State::get_possible_moves(__uint8_t sq) {
	__uint128_t	possible_moves = 0;

	if (sq_is_finished(sq))
		possible_moves = ~(_boards[CR] | _boards[CL]);
	else 
		possible_moves = ((~(_boards[CR] | _boards[CL])) & board_masks[sq]);

	return possible_moves;
}


// much like sq_is_win, but for the whole board
bool	State::is_win(bool player) {
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
	for (__uint8_t i = 0 ; i < 81 ; i++)
		if (n & ((__uint128_t)1 << i))
			return i;
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

Node*	mcts(Node *root) {
	auto 		 millisec_since_epoch = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	auto 		 timer				  = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

	while ((timer = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() - millisec_since_epoch) < 90) {
			State	state;
			bool	player = pl_mark;
			Node*	leaf = traverse(root, state, player);
			expand_node(leaf, state);
			float	simulation_result = rollout(state, player, leaf->move);
			backpropagate(root, leaf, simulation_result);
	}

	return get_best_move(root);
}

Node*		get_best_move(Node* root) {
	__uint8_t	most_visits = 0;
	Node*		best_move;
	for (auto i = 0 ; i < root->nb ; i++)
		if (root->children[i].visits > most_visits)
			best_move = &root->children[i];
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
		state._boards[pl_mark] = state._boards[pl_mark] | node->move;
		player = !player;
		node = rollout_policy(node);
	}
	return result(node);
}

Node*	rollout_policy(Node* node) {
	float	best_score = FLT_MIN;
	Node*	best_child = NULL;

	for (auto i = 0 ; i < node->nb ; i++) {
		float score = node->children[i].wins / node->children[i].visits + sqrt(2) * sqrt(log(node->visits) / node->children[i].visits);
		if (best_score < score) {
			best_score = score;
			best_child = &node->children[i];
		}
	}
	return best_child;
}

void	expand_node(Node* leaf, State& state) {
	__uint128_t	possible_moves =\
		state.get_possible_moves(State::which_square(leaf->move));
	leaf->nb = 0;
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
	uniform_int_distribution<__uint8_t>	distribution(0, popcnt_u128(possible_moves));

	__uint8_t	move_idx = distribution(generator);
	__int8_t	curr_move = 0;
	for (__uint8_t i = 0 ; i < 81 ; i++)
		if (possible_moves & ((__uint128_t)1 << i))
			if (curr_move++ == move_idx)
				return (__uint128_t)1 << i;
	return 0;
}

float	rollout(State state, bool player, const __uint128_t& last_move) {
	__uint8_t	sq = State::which_square(last_move);
	while (true) {
		__uint128_t	possible_moves = state.get_possible_moves(sq);
		__uint128_t	move = pick_random_move(possible_moves);
		state._boards[pl_mark] = state._boards[pl_mark] | move;
		sq = State::which_square(move);
		__uint8_t	set_bit = which_bit(move);
		__uint8_t	x = set_bit / BOARD_SZ;
		__uint8_t	y = set_bit % BOARD_SZ;
		
		if (state.sq_is_win(player, x, y)) {
			state.set_sq_as_won(player, sq);
			if (state.is_win(player))
				return player == pl_mark ? 1 : 0;
		}
	}
	return 0;
}

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