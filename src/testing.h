#pragma once
#include <immintrin.h>

namespace Test{

    __attribute__((noinline)) auto sim_shift(){
        __m256i var = {16, 16, 16, 16};
        __m256i instruction = {2, 1, 4, 5};

        __m256i shifted_var = _mm256_srlv_epi64(var, instruction);

        return shifted_var;
    }

    __attribute__((noinline)) void adressing_256(){
        __m256i var = _mm256_set_epi32(1, 2, 3, 4, 5, 6, 7, 8);

        std::cout << var[0] << std::endl;
        std::cout << var[1] << std::endl;
        std::cout << var[2] << std::endl;
        std::cout << var[3] << std::endl << std::endl;
    }

}
