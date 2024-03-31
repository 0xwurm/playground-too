#pragma once
#include <immintrin.h>

namespace TTT{

    // 0: Padding
    // x: Board

    // xxx00
    // xxx00
    // xxx000
    typedef unsigned short Map;
    using Bit = Map;
    enum Result: signed char{
        loss = -1,
        draw = 0,
        win = 1,
        undecided = 2
    };

    class Bot{
    public:
        static Result result(const Map us, const Map them){
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

        static Result search(const Map us, const Map them){
            const Result res = result(us, them);
            if (res != undecided) return res;

            Map legal_moves = ~(us | them) & 0b1110011100111000;
            if (!legal_moves) return draw;

            Result val = loss;
            for(;legal_moves; legal_moves = _blsr_u64(legal_moves)){
                Bit move = _blsi_u32(legal_moves);
                auto branch_val = Result(-search(them, us | move));
                if (branch_val == win) return win;
                if (branch_val > val)
                    val = branch_val;
            }
            return val;
        }
    };

}
