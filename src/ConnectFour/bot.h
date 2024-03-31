#pragma once
#include <immintrin.h>
#include "../types.h"

namespace CF{

    // 0: Padding
    // x: Board

    // xxxxxxx000
    // xxxxxxx000
    // xxxxxxx000
    // xxxxxxx000
    // xxxxxxx000
    // xxxxxxx000

    int nodes = 0;
    class Bot{
    public:
        FORCEINLINE static Result result(const Map us, const Map them){
            Map wH = (us >> 1) & (us >> 2) & (us >> 3);
            Map lH = (them >> 1) & (them >> 2) & (them >> 3);
            Map wV = (us >> 10) & (us >> 20) & (us >> 30);
            Map lV = (them >> 10) & (them >> 20) & (them >> 30);
            Map wD1 = (us >> 11) & (us >> 22) & (us >> 33);
            Map lD1 = (them >> 11) & (them >> 22) & (us >> 33);
            Map wD2 = (us >> 9) & (us >> 18) & (us >> 27);
            Map lD2 = (them >> 9) & (them >> 18) & (them >> 27);

            if (us & (wH | wV | wD1 | wD2))     return win;
            if (them & (lH | lV | lD1 | lD2))   return loss;
            return undecided;
        }

        static Result search(const Map us, const Map them){
            nodes++;
            const Result res = result(us, them);
            if (res != undecided) return res;

            Map legal_moves = (((us | them) << 10) | 0b1111111000) ^ (us | them);
            if (!legal_moves) return draw;

            Result val = loss;
            for(;legal_moves; legal_moves = _blsr_u64(legal_moves)){
                Bit move = _blsi_u64(legal_moves);
                auto branch_val = Result(-search(them, us | move));
                if (branch_val == win) return win;
                if (branch_val > val)
                    val = branch_val;
            }
            return val;
        }
    };

}
