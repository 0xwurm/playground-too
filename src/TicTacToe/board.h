#pragma once

namespace TTT{

    // 0: Padding
    // x: Board

    // xxx00
    // xxx00
    // xxx000
    typedef unsigned short Map;
    using Bit = Map;

    Bit shifted(int x, int y){
        return 1 << (x + 2 * (y - 1) + 3 * y - 1);
    }

    class Board{
    public:
        Map p1 = 0;
        Map p2 = 0;
        Map legalMoves = 0b1110011100111000;
        bool turn = true;

        explicit Board(const std::string& fen){
            int x = 3;
            int y = 3;
            for (char token: fen){
                if (token == '/'){
                    y--;
                    x = 3;
                }
                else if (token > 48 && token < 58)
                    x -= token - 48;
                else{
                    if      (token == 'x') p1 |= shifted(x, y);
                    else if (token == 'o') p2 |= shifted(x, y);
                    x--;
                }
            }
            legalMoves ^= p1 | p2;
        }

        void place(int x, int y){
            Bit pos = 0b100000000 >> (x + 3 * y - 4);
            if (pos & legalMoves){
                legalMoves ^= pos;
                if (turn)   p1 |= pos;
                else        p2 |= pos;
                turn ^= 1;
            }
            else std::cout << "Illegal Placement!" << std::endl;
        }
    };

    std::ostream& operator<<(std::ostream& stream, Board& b){
        for (int i = 3; i > 0; i--){
            for (int j = 3; j > 0; j--){
                if      (shifted(j, i) & b.p1) stream << "x";
                else if (shifted(j, i) & b.p2) stream << "o";
                else stream << "-";
            }
            stream << "\n";
        }
        return stream;
    }

}
