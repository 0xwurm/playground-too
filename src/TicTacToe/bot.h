#pragma once
#include <immintrin.h>
#include "../types.h"

namespace TTT{

    class Board{
    public:
        __m256i data;

        Board(Map p1, Map p2):
            data(_mm256_set_epi32(p1, p1, p1, p1, p2, p2, p2, p2))
        {}
        Board(__m256i in):
            data(in)
        {}

        Board operator>>(int shift[4]) const{
            __m256i op = _mm256_set_epi32(shift[0], shift[0], shift[0], shift[0],
                                          shift[1], shift[1], shift[1], shift[1]);

            return _mm256_srlv_epi32(data, op);
        }
        Board operator&(Board b2) const{
            return data & b2.data;
        }
        Map operator[](int i) const{
            return data[i];
        }
    };

    // 0: Padding
    // x: Board

    // xxx00
    // xxx00
    // xxx00

    enum Mask : Map{
        full = 0b111001110011100,
        v_sym = 0b001000010000100,
        h_sym = 0b111000000000000,
        d1_sym1 = 0b000001000011000, d1_sym2 = 0b011000010000000,
        d2_sym1 = 0b110001000000000, d2_sym2 = 0b000000010001100
    };

    namespace Symmetry{
        const __m128i vertical1 = {v_sym, v_sym};
        const __m128i vertical2 = {v_sym >> 2, v_sym >> 2};
        const __m128i horizontal1 = {h_sym, h_sym};
        const __m128i horizontal2 = {h_sym >> 10, h_sym >> 10};
        const __m128i diagonal11 = {d1_sym1, d1_sym1};
        const __m128i diagonal12 = {d1_sym2, d1_sym2};
        const __m128i diagonal21 = {d2_sym1, d2_sym1};
        const __m128i diagonal22 = {d2_sym2, d2_sym2};
    }

    int nodes;
    class Bot{
    public:
        // the greatest function of all time!
        __attribute__((noinline)) static Result result(const Map& p1, const Map& p2){
            Board target = {p1, p2};
            int shift1[4] = {1, 5, 6, 4};
            int shift2[4] = {2, 10, 12, 8};

            Board product = target & (target >> shift1) & (target >> shift2);

            if (product[0] | product[1]) return win;
            if (product[2] | product[3]) return loss;
            return undecided;
        }

        static Map symmetries(const Map& p1, const Map& p2){
            Map unique = full;
            __m128i board = {static_cast<long long>(p1), static_cast<long long>(p2)};
            if (_mm_test_all_zeros((board >> 2) ^ board, (board >> 2) ^ board)) unique &= ~v_sym;
            if (_mm_test_all_zeros((board >> 10) ^ board, (board >> 10) ^ board)) unique &= ~h_sym;
            return unique;
        }

        template <bool one>
        static Result search(const Map p1, const Map p2){
            // nodes++;
            const Result res = result(p1, p2);
            if (res != undecided) return res;

            Map legal_moves = (p1 | p2) ^ full;
            if (!legal_moves) return draw;
            legal_moves &= symmetries(p1, p2);

            Result val = loss;
            for(;legal_moves; legal_moves = _blsr_u64(legal_moves)){
                auto branch_val = Result(-search<false>(p2, p1 | _blsi_u64(legal_moves)));
                if (branch_val == win) return win;
                if (branch_val > val) {
                    val = branch_val;
                }
            }
            return val;
        }
    };

}
