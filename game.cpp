
#pragma GCC optimize("O3,inline,omit-frame-pointer,unroll-loops","unsafe-math-optimizations","no-trapping-math")
#pragma GCC option("arch=native","tune=native","no-zero-upper")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,mmx,avx,avx2,popcnt,rdrnd,abm,bmi2,fma")
// #ifdef __GNUC__
// #include <cfenv>
// #endif
#include <iostream>
#include <chrono>

#include <immintrin.h>
#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;
#define uint128_t __int128
    
const int WIDTH = 9;
const int HEIGHT = 9;
const int SIZE = 9 * 9;

template <class T>
static inline void set_bit(T& val, int n){
    val |= (T)1 << n;
}
template <class T>
static inline void reset_bit(T& val, int n){
    val &= ~((T)1 << n);
}
template <class T>
static inline bool check_bit(const T& a, int n){
    return (a >> n) & 1;
}

class State {
public:  
    uint128_t con[4] = {};
    int player_pos[3] = {};
    State() {};
    void set_wall(int pos, int type){
    if (type == 0){
        reset_bit(con[2], pos);
        reset_bit(con[2], pos+1);
        reset_bit(con[0], pos+WIDTH);
        reset_bit(con[0], pos+WIDTH+1);
    }
    else{
        reset_bit(con[1], pos);
        reset_bit(con[1], pos+WIDTH);
        reset_bit(con[3], pos+1);
        reset_bit(con[3], pos+1+WIDTH);
    }
    }

};

class Game {
public:

    State cur_state{};
    uint128_t player_won[3] = {};
    int bfs_rolls = 0;
    Game() {
    // activate all connections
    for (int i = 0; i < 4; i++){
        cur_state.con[i] = ((uint128_t)1 << 81) - (uint128_t)1;
    }
    // remove boundary connections
    for (int i = 0; i < WIDTH; i++) {
        reset_bit(cur_state.con[0], i);
        reset_bit(cur_state.con[1], i * WIDTH + WIDTH - 1);
        reset_bit(cur_state.con[2], i + WIDTH * (HEIGHT - 1));
        reset_bit(cur_state.con[3], i * WIDTH);
    }
    
    // prepare win-condition masks
    for (int i = 0; i < WIDTH; i++){     
        set_bit(player_won[0],(i*WIDTH + HEIGHT - 1));
        set_bit(player_won[1], i*WIDTH);
        set_bit(player_won[2], (HEIGHT - 1)*HEIGHT + i);      
    }
    
    }
    
    int get_path_len(State& s, int owner){
    int pos = s.player_pos[owner];
    if (check_bit(player_won[owner], pos)){
        return 0;
    }
    bfs_rolls += 1;
    uint128_t win = player_won[owner];
    uint128_t cur = (uint128_t)1<<pos;
    uint128_t visits = cur;
    int dist = 0;
    while (true) {
        if (cur==0){
        return -1;
        }
        if ((cur&win)>0){
        return dist;
        }
        cur = (cur&s.con[0])>>WIDTH | (cur&s.con[1])<<1 | (cur&s.con[2])<<WIDTH | (cur&s.con[3])>>1;
        cur &= ~visits;
        visits |= cur;
        dist += 1;
    }
    return -1;    
    }

    void init_cur_state(){
    //init player position
    cur_state.player_pos[0] = 2 + 4 * WIDTH;
    //add some vertical walls
    cur_state.set_wall(3+3*WIDTH, 1);
    cur_state.set_wall(4+4*WIDTH, 1);
    }
    
    void test() {
    system("cat /proc/cpuinfo | grep \"model name\" | head -1 >&2");
    system("cat /proc/cpuinfo | grep \"cpu MHz\" | head -1 >&2");

    ios::sync_with_stdio(false);
// #ifdef __GNUC__
//     feenableexcept(FE_DIVBYZERO | FE_INVALID | FE_OVERFLOW);
// #endif

    init_cur_state();
    int total_dist = 0;
    int roll_count = 1000000;
    auto start = high_resolution_clock::now();
    for (int i = 0; i<roll_count; i++){
        total_dist += get_path_len(cur_state, 0);
    }
    auto end = high_resolution_clock::now();
    auto full_count = duration_cast<microseconds>(end - start).count()/1000;
    cerr << "total execution time " << full_count << "ms with " << bfs_rolls << " rolls"<<  endl;
    cerr << "calculated min distance " << total_dist/bfs_rolls << endl;
    }
};

int main()
{
    Game g = Game();
    g.test();
}
