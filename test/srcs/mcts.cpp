/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mcts.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francisco <francisco@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/09 23:59:51 by francisco         #+#    #+#             */
/*   Updated: 2021/07/30 12:15:30 by francisco        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>
#include "uttt.hpp"

class	TUTT_mcts: public ::testing::Test {
	public:
		TUTT_mcts() {}

		State	state;
	protected:
		virtual void	SetUp() {
			state._boards[CR] = 0;
			state._boards[CL] = 0;
		}
		
		virtual void	TearDown() {}
};

TEST_F(TUTT_mcts, rollout) {
	map<float, int>	result_map;
	result_map[0] = 0;
	result_map[0.5] = 0;
	result_map[1] = 0;
	__uint128_t	move = (__uint128_t)1 << 40;
	state._boards[CR] |= move;
	pl_mark = CL;
	int	count = 0;

	struct timeval time_now{};
    gettimeofday(&time_now, nullptr);
    time_t msecs_time = (time_now.tv_sec * 1000) + (time_now.tv_usec / 1000);
	while (true) {
		float result = rollout(state, pl_mark, move);
		result_map[result]++;
		count++;
		gettimeofday(&time_now, nullptr);
		time_t t2 = (time_now.tv_sec * 1000) + (time_now.tv_usec / 1000);
		if (t2 - msecs_time > 100)
			break;
	}
	cout << "total_simulations: " << count << endl;
	cout << "R: " << result_map[0] << "\t" << result_map[0.5] << "\t" << result_map[1] << endl;
}