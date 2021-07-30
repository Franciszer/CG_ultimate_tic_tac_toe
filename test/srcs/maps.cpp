/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maps.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francisco <francisco@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 20:13:15 by francisco         #+#    #+#             */
/*   Updated: 2021/07/29 21:30:21 by francisco        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "uttt.hpp"
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

TEST_F(TUTT_maps, popcnt_u128) {
	for (auto i = 0 ; i < 128 ; i++) {
		__uint128_t val = (__uint128_t)1 << i;
		EXPECT_EQ(popcnt_u128(val), 1);
	}
}

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

TEST_F(TUTT_maps, which_sq) {

	for (auto i = 0 ; i < 81 ; i++) {
		__uint128_t	move = (__uint128_t)1 << i;
		__uint8_t	expected_value;
		__uint8_t	x, y;

		x = i / BOARD_SZ;
		y = i % BOARD_SZ;

		if (x < 3) {
			if (y < 3)
				expected_value = 0;
			else if (y < 6)
				expected_value = 1;
			else if (y < 9)
				expected_value = 2;
		}
		else if (x < 6) {
			if (y < 3)
				expected_value = 3;
			else if (y < 6)
				expected_value = 4;
			else if (y < 9)
				expected_value = 5;
		}
		else if (x < 9) {
			if (y < 3)
				expected_value = 6;
			else if (y < 6)
				expected_value = 7;
			else if (y < 9)
				expected_value = 8;
		}
		ASSERT_EQ(expected_value, which_sq(move));
	}
}


TEST_F(TUTT_maps, sq_is_finished) {
	for (auto i = 0 ; i < 8 ; i++) {
		state._boards[CR] = board_masks[i];
		EXPECT_TRUE(state.sq_is_finished(i));
		state._boards[CR] = 0;
		EXPECT_FALSE(state.sq_is_finished(i));
	}
}

TEST_F(TUTT_maps, get_possible_moves) {
	for (auto i = 0 ; i < 8 ; i++) {
		__uint128_t possible_moves = state.get_possible_moves(i);
		__uint128_t expected = board_masks[i];
		
		ASSERT_EQ(possible_moves, expected);
		
		state._boards[CR] = board_masks[i];

		possible_moves = state.get_possible_moves(i);
		expected = (~board_masks[i]) & fullmap_mask;
		ASSERT_EQ(possible_moves, expected);
	}
	state._boards[CR] = board_masks[0] | board_masks[8];
	EXPECT_EQ(state.get_possible_moves(0), ~(board_masks[0] | board_masks[8]) & fullmap_mask);
}

TEST_F(TUTT_maps, id_sq) {
	__uint128_t	move;
	for (int i = 0 ; i < 9 ; i++) {
		for (int j = 0 ; j < 9 ; j++) {
			move = (__uint128_t)1 << (i * BOARD_SZ + j);
			ASSERT_TRUE(id_sq(move) < 10);
		}
	}
	
	move = (__uint128_t)1 << 53;
	ASSERT_EQ(id_sq(move), 8);
 }

TEST_F(TUTT_maps, which_bit) {
	__uint128_t move;
	for (auto i = 0; i < 81; i++) {
		move = (__uint128_t) 1 << i;
		EXPECT_EQ(which_bit(move), i);
	}
}
TEST_F(TUTT_maps, sq_is_win) {
	state.set_marking(CL, 2, 0);	
	state.set_marking(CL, 1, 1);	
	state.set_marking(CL, 0, 2);
	ASSERT_TRUE(sq_is_win(state._boards[CL], 0));

	state.set_marking(CL, 6, 6);
	state.set_marking(CL, 6, 7);
	state.set_marking(CL, 6, 8);
	ASSERT_TRUE(sq_is_win(state._boards[CL], 8));

	for (auto i = 0 ; i < BOARD_SZ ; i++) {
		bool result = sq_is_win(state._boards[CL], i);
		if (i == 0 || i == 8)
			ASSERT_TRUE(result);
		else
			ASSERT_FALSE(result);
	}
}

TEST_F(TUTT_maps, set_sq_as_lost) {
	state._boards[CR] = board_masks[4] | board_masks[2];
	set_sq_as_lost(state, CR, 4);
	EXPECT_EQ((uint32_t)popcnt_u128(state._boards[CR]), 9);
}