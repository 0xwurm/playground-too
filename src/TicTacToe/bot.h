#pragma once
#include <immintrin.h>
#include "../types.h"

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
        d1_sym1 = 0b011000010000000, d1_sym2 = 0b000001000011000,
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
            __m256i vec = _mm256_set_epi32(tg, tg, tg, tg, tg, tg, 0, 0);
            __m256i svec = _mm256_set_epi32(2, 10, 6, 12, 4, 8, 0, 0);
            __m256i shifted = _mm256_srlv_epi32(vec, svec);
            __m256i xord = vec ^ shifted;

            Map unique = full;
            if (!(xord[3] & 0x1084108400000000ull)) unique &= ~v_sym;
            if (!(xord[3] & 0x00000000001c001cull)) unique &= ~h_sym;
            if (!(xord[2] & 0x0088008800040004ull)) unique &= ~d2_sym1;
            if (!(xord[1] & 0x0208020800100010ull)) unique &= ~d1_sym1;
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
