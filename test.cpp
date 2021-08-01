/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francisco <francisco@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/31 15:45:09 by francisco         #+#    #+#             */
/*   Updated: 2021/07/31 20:31:50 by francisco        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <bit>
#include <algorithm>
#include <stdint.h>

using namespace std;

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

// returns the nth bit using a variant of binary search
uint8_t binbits ( const __uint128_t b, const uint8_t n ) {    
    uint8_t l_bound = 0;
    uint8_t mid = 40;
    uint8_t u_bound = 81;
    
    // error: n is greater than nb of bits
    if ( popcnt_u128(b) < n )
        return 0;

    // 
    uint8_t     ct;
    __uint128_t chunk;
    do {
        chunk = b << mid;
        ct = popcnt_u128(chunk);
        cerr << "ct " << (uint64_t)ct << endl;

        uint8_t tmp = mid;
        if (ct < n) {
            mid = l_bound + mid / 2;
            u_bound = tmp;
        }
        else if ( ct > n ) {
            mid = u_bound + mid / 2;
            l_bound = tmp;
        }
    } while ( ct != n );

    uint8_t idx = mid ;
    cerr << "stuck here" << endl;
    cerr << "idx " << (uint16_t)idx << endl;
    while ( ( b << idx ) ^ (__uint128_t)1 ) {
        
        --idx;
        }
    
    return idx;
}

int main(int argc, char const *argv[])
{
    __uint128_t a = (__uint128_t)1 << 123;
    std::cout << binbits(a, 1) << std::endl;
    return 0;
}
