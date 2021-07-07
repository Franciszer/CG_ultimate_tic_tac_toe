/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maps.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francisco <francisco@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 20:13:15 by francisco         #+#    #+#             */
/*   Updated: 2021/07/07 19:22:35 by francisco        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.cpp"
#include <gtest/gtest.h>

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
	for (__uint128_t i = 0 ; i < BOARD_SZ ; i++) {
		for (__uint128_t j = 0 ; j < BOARD_SZ ; j++) {
			EXPECT_FALSE(state.is_marking(state._player, i, j));
		}
	}
	for (__uint128_t i = 0 ; i < BOARD_SZ ; i++) {
		for (__uint128_t j = 0 ; j < BOARD_SZ ; j++) {
			state.set_marking(i, j);
		}
	}
	for (__uint128_t i = 0 ; i < BOARD_SZ ; i++) {
		for (__uint128_t j = 0 ; j < BOARD_SZ ; j++) {
			EXPECT_TRUE(state.is_marking(state._player, i, j));
		}
	}
	state._player = !state._player;
	for (__uint128_t i = 0 ; i < BOARD_SZ ; i++) {
		for (__uint128_t j = 0 ; j < BOARD_SZ ; j++) {
			EXPECT_FALSE(state.is_marking(state._player, i, j));
		}
	}
	state._boards[state._player] = 0;
	for (__uint128_t i = 0 ; i < BOARD_SZ ; i++) {
		for (__uint128_t j = 0 ; j < BOARD_SZ ; j++) {
			EXPECT_FALSE(state.is_marking(state._player, i, j));
		}
	}
	// testing for specific positions
	state.set_marking(3, 6);
	state.set_marking(5, 7);
	state.set_marking(8, 8);
	state.set_marking(0, 0);
	for (__uint128_t i = 0 ; i < BOARD_SZ ; i++) {
		for (__uint128_t j = 0 ; j < BOARD_SZ ; j++) {
			if ((i == 3 && j == 6) || (i == 5 && j == 7) || (i == 8 && j == 8) || (i == 0 && j == 0)) {
				EXPECT_TRUE(state.is_marking(state._player, i, j)) << "x " << (uint)i << " y " << (uint)j;
			}
			else
				EXPECT_FALSE(state.is_marking(state._player, i, j)) << "x " << (uint)i << " y " << (uint)j;
		}
	}
}
