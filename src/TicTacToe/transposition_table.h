#pragma once
#include "../types.h"

namespace TTT::TT{

    struct Entry{
        Result r = undecided;
    };

    Entry table[262143]{};

    void clear(){
        for (Entry& e: table) e.r = undecided;
    }
}
