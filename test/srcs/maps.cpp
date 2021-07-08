/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maps.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francisco <francisco@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 20:13:15 by francisco         #+#    #+#             */
/*   Updated: 2021/07/08 18:33:59 by francisco        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.cpp"
#include <gtest/gtest.h>

std::ostream&
operator<<( std::ostream& dest, __int128_t value )
{
    std::ostream::sentry s( dest );
    if ( s ) {
        __uint128_t tmp = value < 0 ? -value : value;
        char buffer[ 128 ];
        char* d = std::end( buffer );
        do
        {
            -- d;
            *d = "0123456789"[ tmp % 10 ];
            tmp /= 10;
        } while ( tmp != 0 );
        if ( value < 0 ) {
            -- d;
            *d = '-';
        }
        int len = std::end( buffer ) - d;
        if ( dest.rdbuf()->sputn( d, len ) != len ) {
            dest.setstate( std::ios_base::badbit );
        }
    }
    return dest;
}

class	TUTT_maps: public ::testing::Test {
	public:
		TUTT_maps() {}

		State	state;
	protected:
		virtual void	SetUp() {}

		virtual void	TearDown() {}
};

TEST_F(TUTT_maps, insert_and_access) {
	// testing for whole map
	bool player = CR;
	for (__uint128_t i = 0 ; i < BOARD_SZ ; i++) {
		for (__uint128_t j = 0 ; j < BOARD_SZ ; j++) {
			EXPECT_FALSE(state.is_marking(player, i, j));
		}
	}
	for (__uint128_t i = 0 ; i < BOARD_SZ ; i++) {
		for (__uint128_t j = 0 ; j < BOARD_SZ ; j++) {
			state.set_marking(player, i, j);
		}
	}
	for (__uint128_t i = 0 ; i < BOARD_SZ ; i++) {
		for (__uint128_t j = 0 ; j < BOARD_SZ ; j++) {
			EXPECT_TRUE(state.is_marking(player, i, j));
		}
	}
	player = !player;
	for (__uint128_t i = 0 ; i < BOARD_SZ ; i++) {
		for (__uint128_t j = 0 ; j < BOARD_SZ ; j++) {
			EXPECT_FALSE(state.is_marking(player, i, j));
		}
	}
	state._boards[player] = 0;
	for (__uint128_t i = 0 ; i < BOARD_SZ ; i++) {
		for (__uint128_t j = 0 ; j < BOARD_SZ ; j++) {
			EXPECT_FALSE(state.is_marking(player, i, j));
		}
	}
	// testing for specific positions
	state.set_marking(player, 3, 6);
	state.set_marking(player, 5, 7);
	state.set_marking(player, 8, 8);
	state.set_marking(player, 0, 0);
	for (__uint128_t i = 0 ; i < BOARD_SZ ; i++) {
		for (__uint128_t j = 0 ; j < BOARD_SZ ; j++) {
			if ((i == 3 && j == 6) || (i == 5 && j == 7) || (i == 8 && j == 8) || (i == 0 && j == 0)) {
				EXPECT_TRUE(state.is_marking(player, i, j)) << "x " << (uint)i << " y " << (uint)j;
			}
			else
				EXPECT_FALSE(state.is_marking(player, i, j)) << "x " << (uint)i << " y " << (uint)j;
		}
	}
}

TEST_F(TUTT_maps, which_square) {
	EXPECT_EQ(State::which_square(0, 0), 0);
	EXPECT_EQ(State::which_square(0, 1), 0);
	EXPECT_EQ(State::which_square(0, 2), 0);
	EXPECT_EQ(State::which_square(1, 0), 0);
	EXPECT_EQ(State::which_square(1, 1), 0);
	EXPECT_EQ(State::which_square(1, 2), 0);
	EXPECT_EQ(State::which_square(2, 0), 0);
	EXPECT_EQ(State::which_square(2, 1), 0);
	EXPECT_EQ(State::which_square(2, 2), 0);

	EXPECT_EQ(State::which_square(0, 3), 1);
	EXPECT_EQ(State::which_square(1, 3), 1);
	EXPECT_EQ(State::which_square(2, 3), 1);
	EXPECT_EQ(State::which_square(0, 4), 1);
	EXPECT_EQ(State::which_square(0, 4), 1);
	EXPECT_EQ(State::which_square(1, 4), 1);
	EXPECT_EQ(State::which_square(1, 5), 1);
	EXPECT_EQ(State::which_square(2, 5), 1);
	EXPECT_EQ(State::which_square(2, 5), 1);
	
	EXPECT_EQ(State::which_square(0, 6), 2);
	EXPECT_EQ(State::which_square(1, 6), 2);
	EXPECT_EQ(State::which_square(2, 6), 2);
	EXPECT_EQ(State::which_square(0, 7), 2);
	EXPECT_EQ(State::which_square(1, 7), 2);
	EXPECT_EQ(State::which_square(2, 7), 2);
	EXPECT_EQ(State::which_square(0, 8), 2);
	EXPECT_EQ(State::which_square(1, 8), 2);
	EXPECT_EQ(State::which_square(2, 8), 2);


	EXPECT_EQ(State::which_square(3, 0), 3);
	EXPECT_EQ(State::which_square(3, 1), 3);
	EXPECT_EQ(State::which_square(3, 2), 3);
	EXPECT_EQ(State::which_square(4, 0), 3);
	EXPECT_EQ(State::which_square(4, 1), 3);
	EXPECT_EQ(State::which_square(4, 2), 3);
	EXPECT_EQ(State::which_square(5, 0), 3);
	EXPECT_EQ(State::which_square(5, 1), 3);
	EXPECT_EQ(State::which_square(5, 2), 3);
	
	EXPECT_EQ(State::which_square(3, 3), 4);
	EXPECT_EQ(State::which_square(3, 4), 4);
	EXPECT_EQ(State::which_square(3, 5), 4);
	EXPECT_EQ(State::which_square(4, 3), 4);
	EXPECT_EQ(State::which_square(4, 4), 4);
	EXPECT_EQ(State::which_square(4, 5), 4);
	EXPECT_EQ(State::which_square(5, 3), 4);
	EXPECT_EQ(State::which_square(5, 4), 4);
	EXPECT_EQ(State::which_square(5, 5), 4);
	
	EXPECT_EQ(State::which_square(3, 6), 5);
	EXPECT_EQ(State::which_square(3, 7), 5);
	EXPECT_EQ(State::which_square(3, 8), 5);
	EXPECT_EQ(State::which_square(4, 6), 5);
	EXPECT_EQ(State::which_square(4, 7), 5);
	EXPECT_EQ(State::which_square(4, 8), 5);
	EXPECT_EQ(State::which_square(5, 6), 5);
	EXPECT_EQ(State::which_square(5, 7), 5);
	EXPECT_EQ(State::which_square(5, 8), 5);
	
	EXPECT_EQ(State::which_square(6, 0), 6);
	EXPECT_EQ(State::which_square(6, 1), 6);
	EXPECT_EQ(State::which_square(6, 2), 6);
	EXPECT_EQ(State::which_square(7, 0), 6);
	EXPECT_EQ(State::which_square(7, 1), 6);
	EXPECT_EQ(State::which_square(7, 2), 6);
	EXPECT_EQ(State::which_square(8, 0), 6);
	EXPECT_EQ(State::which_square(8, 1), 6);
	EXPECT_EQ(State::which_square(8, 2), 6);

	EXPECT_EQ(State::which_square(6, 3), 7);
	EXPECT_EQ(State::which_square(6, 4), 7);
	EXPECT_EQ(State::which_square(6, 5), 7);
	EXPECT_EQ(State::which_square(7, 3), 7);
	EXPECT_EQ(State::which_square(7, 4), 7);
	EXPECT_EQ(State::which_square(7, 5), 7);
	EXPECT_EQ(State::which_square(8, 3), 7);
	EXPECT_EQ(State::which_square(8, 4), 7);
	EXPECT_EQ(State::which_square(8, 5), 7);
	
	EXPECT_EQ(State::which_square(6, 6), 8);
	EXPECT_EQ(State::which_square(6, 7), 8);
	EXPECT_EQ(State::which_square(6, 8), 8);
	EXPECT_EQ(State::which_square(7, 6), 8);
	EXPECT_EQ(State::which_square(7, 7), 8);
	EXPECT_EQ(State::which_square(7, 8), 8);
	EXPECT_EQ(State::which_square(8, 6), 8);
	EXPECT_EQ(State::which_square(8, 7), 8);
	EXPECT_EQ(State::which_square(8, 8), 8);
}

// __uint128_t	square_masks[11] {
	
// }

// TEST_F(TUTT_maps, masks) {
// 	state.set_marking(CL, 0, 0);	
// 	state.set_marking(CL, 0, 1);	
// 	state.set_marking(CL, 0, 2);
// 	auto p = uint128_encode(state._boards[CL]);
// 	cerr << hex << p.first << '\t' << p.second << endl;
// 	cerr << state << endl;
// }