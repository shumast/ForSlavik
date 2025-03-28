#pragma once
#include <chrono>
#include <future>
#include <map>
#include <random>
#include <thread>
#include <vector>

inline int N_GRID_SIZE = 15;
inline int M_GRID_SIZE = 15;
inline int CELL_SIZE = 50;
inline int N_WINDOW_SIZE = N_GRID_SIZE * CELL_SIZE;
inline int M_WINDOW_SIZE = M_GRID_SIZE * CELL_SIZE;
inline int EMPTY = -1;
inline int BORDER = 5;
inline int NEED_FIRST = 5;
inline int NEED_SECOND = 5;
inline int QUEUE = 0;
inline int FIRST_PLAYER = 0;
inline int SECOND_PLAYER = 0;
inline int INF = 1e9;
inline int MAX_DEPTH_ALPHA_BETA = 4;
inline int MAX_DEPTH_MINIMAX_DEPTH = 4;
inline int MAX_CONDITIONS_ON_ONE_LEVEL = 9;
inline int MAX_DEPTH_RADIAL_SEARCH = 6;
inline int currentTurn = 0;
inline const int R_VARIOTY = 2;
inline const int MAX_LOG_GRID_SIZE = 6;
inline double PROBABILITY = 0.5;
inline unsigned long long int currentHash = 0;
inline std::chrono::steady_clock::time_point start_time;
inline std::chrono::seconds time_limit(5);
inline std::chrono::seconds time_limit_for_MCST(5);
inline std::vector<std::pair<int, int>> CheckDirections = { {1,1}, {1,0}, {0,1}, {1,-1} };
// patterns for first:
// x-1 rocks: 1000000
// y-1 rocks: 5000 (win > not lose)
// x-2 rocks with 2 free: 80000
// x-2 rocks with 2 free: 4000
// y-2 rocks with 1 free: 1000
// y-3 rocks with >= 3 free: 2000
// close your rocks: 10
// close enemy rocks: 1
// 
inline std::vector<int> patterns = { 1000000, 10000, 80000, 4000, 2000, 1000, 10, 1 };
inline std::vector<int> sophisticated_queue;
inline std::vector<std::vector<std::vector<unsigned long long int>>> ZobristTable;
inline std::map<unsigned long long int, int> costPosition;

inline std::random_device rd;
inline std::mt19937 gen(rd());

unsigned long long int randomInt();

class XYK {
public:
    // Initialization
    XYK();

    // Reset
    void reset();

    // Make move
    bool makeMove(int x, int y);

    int getWinner();

    int getPlayer();

    const std::vector<std::vector<int>>& getBoard() const;

    const std::vector<std::pair<int, int>>& getMoves() const;

    // Check if someone win and change constant
    void checkWinner(int x, int y);

    // Is it possible for someone to win
    bool canWin();

    // Evaluation position
    int evaluatePosition(int x, int y, int player_);

    // Evaluation board position
    int evaluatePositionOnBoard(int current_player);

    // Random move
    void makeRandomCloseMove();

    // Simple minimax algo
    int makeMinimax(int x, int y, int turn, unsigned long long int hash);

    void makeMinimaxMove();

    // Alpha beta optimization for minimax
    int makeMinimaxWithAlphaBeta(int x, int y, int alpha, int beta, int turn, unsigned long long int hash);

    void makeMinimaxWithAlphaBetaMove();

    // Using depth for stopping minimax
    int makeMinimaxWithDepth(int x, int y, int depth, int turn, unsigned long long int hash);

    void makeMinimaxWithDepthMove();

    // Using time limit for depth minimax
    int makeMinimaxWithDepthAndTime(int x, int y, int depth, int turn, unsigned long long int hash);

    void makeMinimaxWithDepthAndTimeMove();

    // Greedy search using evaluating position
    void makeGreedySearch(int player);

    void makeGreedySearchMove();

    // Greedy search using evaluating position for full board
    void makeGreedySearch2(int player);

    void makeGreedySearch2Move();

    // algorithm A* can't be used here, because it's impossible to find h-function for end position

    // Using depth for stopping minimax with new function
    int makeMinimaxWithDepth2(int x, int y, int depth, int turn, unsigned long long int hash);

    void makeMinimaxWithDepth2Move();

    int makeMinimaxWithDepth3(int x, int y, int depth, int turn, unsigned long long int hash);

    void makeMinimaxWithDepth3Move();

    // luchevoi poisk v shirinu
    int makeRadialSearch(int x, int y, int depth, int turn, unsigned long long int hash);

    void makeRadialSearchMove();

    // geneticheskiy algorithm is too slow
    // because of a lot of variants of boards and conditions for win it's stupid to learn bots

    void makeMCTSmove();
    // ant colonie is a not good algorithm for this game, MCTS is more better

private:
    std::vector<std::vector<int>> board_;
    std::vector<std::pair<int, int>> moves_;
    int currentPlayer_;
    int winner_;
};

// monte carlo
struct Node;

std::vector<std::pair<int, int>> allowedMoves(std::vector<std::vector<int>>& board);

std::pair<int, int> MCTS(std::vector<std::vector<int>> rootBoard, int currentturn);

void threadWork(std::vector<std::vector<int>> rootBoard, int position);

int simulate(std::vector<std::vector<int>> board, int turn, std::pair<int, int> last);

void paralel_simulate(int iter, std::vector<std::vector<int>> board, int turn, std::pair<int, int> last);

int giveWinner(int x, int y, std::vector<std::vector<int>>& board);

// Check that point in grid
bool InGrid(int x, int y);
