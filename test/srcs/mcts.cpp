/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mcts.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francisco <francisco@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/09 23:59:51 by francisco         #+#    #+#             */
/*   Updated: 2021/07/10 00:04:21 by francisco        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.cpp"
#include <gtest/gtest.h>

class	TUTT_mcts: public ::testing::Test {
	public:
		TUTT_mcts() {}

		State	state;
	protected:
		virtual void	SetUp() {}

		virtual void	TearDown() {}
};

TEST_F(TUTT_mcts, mcts) {
    cerr << sizeof(Node);
}