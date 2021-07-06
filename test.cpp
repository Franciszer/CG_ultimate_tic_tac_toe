#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

#define MINE 0
#define OPP 1
#define SIZE 3 // both width and height

using namespace std;
using std::ostream;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

struct State
{
    __int128_t  bitboards[2] = {0};
    void    printBoard(ostream& os) {

    }
};

__int128_t board = {0};

void    print_board() {
    for (int i = 0 ; i < SIZE ; i++) {
        for (int j = 0 ; j < SIZE ; j++) {
            if ((board & ((__int128_t)1 << (i * SIZE + j))))
                cout << '1';
            else
                cout << '0';
            cout << ' ';
        }
        cout << endl;
    }
}

int main()
{
    // cout << board << endl;
    board = (board | ((__int128_t)1 << 0));
    print_board();
}