#pragma once
#include <immintrin.h>
#include "../types.h"
#include "transposition_table.h"

namespace TTT{

    class Board{
    public:
        __m128i data;

        Board(Map p1, Map p2):
            data(_mm_setr_epi16(p1, p1, p1, p1, p2, p2, p2, p2))
        {}
        Board(Map A, Map B, Map C, Map D, Map E, Map F, Map G, Map H):
            data(_mm_setr_epi16(A, B, C, D, E, F, G, H))
        {}
        Board(__m128i in):
            data(in)
        {}

        Board operator>>(int shift[8]) const{
            __m128i op = _mm_setr_epi16(shift[0], shift[1], shift[2], shift[3],
                                          shift[4], shift[5], shift[6], shift[7]);

            return _mm_srlv_epi16(data, op);
        }
        Board operator&(Board b2) const{
            return _mm_and_si128(data, b2.data);
        }
        Board operator^(Board b2) const{
            return _mm_xor_si128(data, b2.data);
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
        d1_sym1 = 0b011000010000000,
        d2_sym1 = 0b110001000000000
    };

    int nodes;
    class Bot{
    public:
        // the greatest function of all time!
        static Result result(const Map& p1, const Map& p2){
            Board target = {p1, p2};
            int shift1[8] = {1, 5, 6, 4, 1, 5, 6, 4};
            int shift2[8] = {2, 10, 12, 8, 2, 10, 12, 8};

            Board product = target & (target >> shift1) & (target >> shift2);

            if (product[0]) return win;
            if (product[1]) return loss;
            return undecided;
        }

        static Map symmetries(const Map& p1, const Map& p2){
            Map tg = (p1 << 16) | p2;
            __m128i target = _mm_set1_epi32(tg);
            __m128i shift = _mm_set_epi32(2, 10, 6, 12);
            target ^= _mm_srlv_epi32(target, shift);

            Map unique = full;
            if (!(target[1] & 0x1084108400000000ull)) unique &= ~v_sym;
            if (!(target[1] & 0x00000000001c001cull)) unique &= ~h_sym;
            if (!(target[0] & 0x0088008800040004ull)) unique &= ~d2_sym1;
            // checking for symmetry on diagonal one is slower
            return unique;
        }

        template <int n>
        static Result search(const Map p1, const Map p2){
            if constexpr (n > 1) {
                unsigned int index = _pext_u32((p1 << 16) | p2, 0b111001110011100111001110011100);
                if (TT::table[index].r != undecided) return TT::table[index].r;
            }

            if constexpr (n < 5) {
                const Result res = result(p1, p2);
                if (res != undecided) return res;
            }

            // if constexpr (!n) return draw; // slower? why?
            Map legal_moves = (p1 | p2) ^ full;
            if (!legal_moves) return draw;
            if constexpr (n > 6) legal_moves &= symmetries(p1, p2);

            Result val = loss;
            for(int i = 0; i < n; i++){
                Bit move = _blsi_u64(legal_moves);
                auto branch_val = Result(-search<n ? n - 1 : 0>(p2, p1 | move));
                if (branch_val == win)  return win;
                if (branch_val == draw) val = draw;
                if constexpr (n) legal_moves = _blsr_u64(legal_moves);
            }

            if constexpr (n > 1) {
                unsigned int index = _pext_u32((p1 << 16) | p2, 0b111001110011100111001110011100);
                TT::table[index].r = val;
            }
            return val;
        }
    };

}
