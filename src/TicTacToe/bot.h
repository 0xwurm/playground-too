#pragma once
#include <immintrin.h>
#include "../types.h"

namespace TTT{

    class Bd{
    public:
        __m128i data;

        Bd(__m128i in){
            data = in;
        }
    };

    Bd operator>>(const Bd& bd, int n){
        return {_mm_srli_epi64(bd.data, n)};
    }
    Bd operator&(const Bd& bd1, const Bd& bd2){
        return {_mm_and_si128(bd1.data, bd2.data)};
    }
    Bd operator|(const Bd& bd1, const Bd& bd2){
        return {_mm_or_si128(bd1.data, bd2.data)};
    }

    // 0: Padding
    // x: Board

    // xxx00
    // xxx00
    // xxx00

    class Bot{
    public:
        FORCEINLINE static Result result(const Map us, const Map them){
            Map wH = (us >> 1) & (us >> 2);
            Map lH = (them >> 1) & (them >> 2);
            Map wV = (us >> 5) & (us >> 10);
            Map lV = (them >> 5) & (them >> 10);
            Map wD1 = (us >> 6) & (us >> 12);
            Map lD1 = (them >> 6) & (them >> 12);
            Map wD2 = (us >> 4) & (us >> 8);
            Map lD2 = (them >> 4) & (them >> 8);

            if (us & (wH | wV | wD1 | wD2))     return win;
            if (them & (lH | lV | lD1 | lD2))   return loss;
            return undecided;
        }

        FORCEINLINE static Result test(const Bd board){
            Bd ho = (board >> 1) & (board >> 2);
            Bd ve = (board >> 5) & (board >> 10);
            Bd d1 = (board >> 6) & (board >> 12);
            Bd d2 = (board >> 4) & (board >> 8);

            Bd complete = (ho | ve | d1 | d2) & board;

            if (complete.data[0]) return win;
            if (complete.data[1]) return loss;
            return undecided;
        }

        static Result search(const __m128i board){
            const Result res = test(board);
            if (res != undecided) return res;

            Map legal_moves = ~(board[0] | board[1]) & 0b111001110011100;
            if (!legal_moves) return draw;

            Result val = loss;
            for(;legal_moves; legal_moves = _blsr_u64(legal_moves)){
                Bit move = _blsi_u64(legal_moves);
                __m128i newBoard = {board[1], static_cast<long long>(board[0] | move)};
                auto branch_val = Result(-search(newBoard));
                if (branch_val == win) return win;
                if (branch_val > val)
                    val = branch_val;
            }
            return val;
        }
    };

}
