#include "uttt.hpp"

// moves root to node corresponding to opp's move
Node*   identify_move(Node* root, __uint128_t opponentRow, __uint128_t opponentCol) {
    __uint128_t marked = ((__uint128_t)1 << (opponentRow * BOARD_SZ + opponentCol));
    unsigned char i = 0;
    for (; i < root->nb ; i++) {
        if (root->children[i].move == marked)
            break ;
    }
    return i < root->nb ? &root->children[i] : &root->children[0];
}

void    print_best_move(__uint128_t move) {
    for (__uint128_t i = 0 ; i < 81 ; i++) {
        if (move & ((__uint128_t)1 << i))
            cout << (__uint64_t)(i / BOARD_SZ) << ' ' << (__uint64_t)(i % BOARD_SZ);
    }
}

int main()
{
	State	state;
    Node*   root;

    g_map.reserve(5'000'000);
    root = memory;
    root->parent = NULL;
    root->visits = 1;
    root->wins = 0;
    current_address++;
    state._boards[CR] = 0;
    state._boards[CL] = 0;
    // first round
    int opponentRow;
    int opponentCol;
    cin >> opponentRow >> opponentCol; cin.ignore();
    int validActionCount;
    cin >> validActionCount; cin.ignore();
    int row;
    int col;
    for (int i = 0; i < validActionCount; i++) {
        cin >> row >> col; cin.ignore();
    }

    if (opponentRow == -1) {
        pl_mark = CR;
        state._boards[pl_mark] |= (__uint128_t)1 << (4 * BOARD_SZ + 4) ;
    }
    else {
        pl_mark = CL;
        state._boards[CR] |= (__uint128_t)1 << (opponentRow * BOARD_SZ + opponentCol);
        if (opponentRow == 4 && opponentCol == 4) {
            state._boards[pl_mark] |= (__uint128_t)1 << (5 * BOARD_SZ + 5) ;
        }
        else {
            state._boards[pl_mark] |= (__uint128_t)1 << (4 * BOARD_SZ + 4) ;
        }
    }
    root->move = state._boards[pl_mark];
    expand_node(root, state);
    // cout << "THERE 2" << endl;
    // cerr << "ROOT" << endl;
    // cerr << *root << endl;
    // cerr << "END OF ROOT" << endl;
    // cerr << "first" << endl;
    // pboard(state, 0, !pl_mark, cerr);
    mcts(root, state, 1000);
    // for (int i = 0 ; i < root->nb ; i++) {
    //     cerr << "CHILD " << i << endl;
    //     cerr << "visits: " << root->children[i].visits << endl;
    //     cerr << "wins: " << root->children[i].wins << endl;
    //    print_board(root->children[i].children[0].move);
    // }
    cerr << "nb of nodes: " << current_address - memory << endl;
    exit(0);
    // rest of the game
    while (1) {
        cin >> opponentRow >> opponentCol; cin.ignore();
        cin >> validActionCount; cin.ignore();
        for (int i = 0; i < validActionCount; i++) {
            cin >> row >> col; cin.ignore();
        }

        root = identify_move(root, opponentRow, opponentCol);
        mcts(root, state, 90);
        __uint128_t best_move = get_best_move(root);
        if (!best_move) {
            cout << "error: get_best_move: abort" << endl;
        }
        print_best_move(best_move);
        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;

        // cout << "0 0" << endl;
    }
}