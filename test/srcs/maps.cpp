/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   maps.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francisco <francisco@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 20:13:15 by francisco         #+#    #+#             */
/*   Updated: 2021/07/07 15:24:07 by francisco        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <gtest/gtest.h>
#include <list>

// CAPACITY TESTS
class	TList_Capacity: public ::testing::Test {
	public:
		TList_Capacity() {}

		ft::list<int>		*ft_list = nullptr;
	protected:
		virtual void	SetUp() {}

		virtual void	TearDown() {
			if (ft_list != nullptr) {
				delete ft_list;
				ft_list = nullptr;
			}
		}
};

TEST_F(TList_Capacity, empty) {
    ft_list = new ft::list<int>;

    EXPECT_TRUE(ft_list->empty());
    ft_list->push_back(1);
    EXPECT_FALSE(ft_list->empty());
}
