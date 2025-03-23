#include "Graphics.hpp"
#include <vector>
#include <iostream>
#include <random>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <set>
#include <unordered_set>
#include <chrono>
#include <thread>

std::random_device rd;
std::mt19937 gen(rd());

std::vector<int> sophisticated_queue;
int N_GRID_SIZE = 3;
int M_GRID_SIZE = 3;
int CELL_SIZE = 100;
const int R_VARIOTY = 2;
const int MAX_LOG_GRID_SIZE = 6;
int N_WINDOW_SIZE = N_GRID_SIZE * CELL_SIZE;
int M_WINDOW_SIZE = M_GRID_SIZE * CELL_SIZE;
int EMPTY = -1;
int BORDER = 10;
int NEED_FIRST = 3;
int NEED_SECOND = 3;
int QUEUE = 0;
int FIRST_PLAYER = 0;
int SECOND_PLAYER = 0;
int INF = 1e9;
int MAX_DEPTH_ALPHA_BETA = 5;
int MAX_DEPTH_MINIMAX_DEPTH = 5;
int MAX_CONDITIONS_ON_ONE_LEVEL = 9;
int MAX_DEPTH_RADIAL_SEARCH = 4;
double PROBABILITY = 0.5;
int currentTurn = 0;
std::chrono::steady_clock::time_point start_time;
std::chrono::milliseconds time_limit(3000);
std::chrono::milliseconds time_limit_for_MCST(1000);
std::vector<std::pair<int, int>> CheckDirections = { {1,1}, {1,0}, {0,1}, {1,-1} };
// patterns for first:
// x-1 rocks: 1000000
// y-1 rocks: 5000 (win > not lose)
// x-2 rocks with 2 free: 80000
// y-2 rocks with 2 free: 4000
// y-2 rocks with 1 free: 1000
// y-3 rocks with >= 3 free: 2000
// close your rocks: 10
// close enemy rocks: 1
// 
std::vector<int> patterns = { 1000000, 5000, 80000, 4000, 1000, 2000, 10, 1 };
std::ofstream cout("output.txt");

class TicTacToe {
public:
    // Initialization
    TicTacToe() : board_(M_GRID_SIZE, std::vector<int>(N_GRID_SIZE, EMPTY)), currentPlayer_(0) {}

    // Reset
    void reset() {
        board_.assign(M_GRID_SIZE, std::vector<int>(N_GRID_SIZE, EMPTY));
        moves_.clear();
        currentTurn = 0;
        currentPlayer_ = 0;
        winner_ = -1;
    }

    // Check that point in grid
    bool InGrid(int x, int y) {
        if (x >= 0 && y >= 0 && x < M_GRID_SIZE && y < N_GRID_SIZE) {
            return true;
        }
        return false;
    }

    // Make move
    bool makeMove(int x, int y) {
        if (board_[x][y] == EMPTY) {
            moves_.push_back({ x, y });
            board_[x][y] = currentPlayer_;
            checkWinner(x, y);
            currentTurn++;
            currentPlayer_ = sophisticated_queue[currentTurn];
            return true;
        }
        return false;
    }

    int getWinner() {
        return winner_;
    }

    int getPlayer() {
        return currentPlayer_;
    }

    const std::vector<std::vector<int>>& getBoard() const {
        return board_;
    }

    // Check if someone win and change constant
    void checkWinner(int x, int y) {
        for (auto [i, j] : CheckDirections) {
            int cnt1 = 1, cnt2 = 1;
            while (InGrid(x + cnt1 * i, y + cnt1 * j) && board_[x + cnt1 * i][y + cnt1 * j] == board_[x][y]) {
                cnt1++;
            }
            while (InGrid(x - cnt2 * i, y - cnt2 * j) && board_[x - cnt2 * i][y - cnt2 * j] == board_[x][y]) {
                cnt2++;
            }
            if (board_[x][y] == 0) {
                if (cnt1 + cnt2 - 1 >= NEED_FIRST) {
                    winner_ = 0;
                    return;
                }
            }
            else {
                if (cnt1 + cnt2 - 1 >= NEED_SECOND) {
                    winner_ = 1;
                    return;
                }
            }
        }
    }

    // Is it possible for someone to win
    bool canWin() {
        for (int x = 0; x < M_GRID_SIZE; x++) {
            for (int y = 0; y < N_GRID_SIZE; y++) {
                for (auto [i, j] : CheckDirections) {
                    int cnt1 = 1, cnt2 = 1;
                    while (InGrid(x + cnt1 * i, y + cnt1 * j) && (board_[x + cnt1 * i][y + cnt1 * j] == -1 || board_[x + cnt1 * i][y + cnt1 * j] == board_[x][y])) {
                        cnt1++;
                    }
                    while (InGrid(x - cnt2 * i, y - cnt2 * j) && (board_[x - cnt2 * i][y - cnt2 * j] == -1 || board_[x - cnt2 * i][y - cnt2 * j] == board_[x][y])) {
                        cnt2++;
                    }
                    if (board_[x][y] == 0) {
                        if (cnt1 + cnt2 - 1 >= NEED_FIRST) {
                            return true;
                        }
                    }
                    else {
                        if (cnt1 + cnt2 - 1 >= NEED_SECOND) {
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }

    // Evaluation position
    int evaluatePosition(int x, int y, int player_) {
        int ans = 0;
        for (auto [i, j] : CheckDirections) {
            bool g1 = true, g2 = true;
            int cnt1 = 1, cnt2 = 1;
            if (!InGrid(x + cnt1 * i, y + cnt1 * j)) {
                g1 = false;
            }
            while (InGrid(x + cnt1 * i, y + cnt1 * j) && board_[x + cnt1 * i][y + cnt1 * j] == player_) {
                cnt1++;
                if (!InGrid(x + cnt1 * i, y + cnt1 * j)) {
                    g1 = false;
                    break;
                }
            }
            if (!InGrid(x - cnt2 * i, y - cnt2 * j)) {
                g2 = false;
            }
            while (InGrid(x - cnt2 * i, y - cnt2 * j) && board_[x - cnt2 * i][y - cnt2 * j] == player_) {
                cnt2++;
                if (!InGrid(x - cnt2 * i, y - cnt2 * j)) {
                    g2 = false;
                    break;
                }
            }
            int free1 = 0, free2 = 0;
            while (InGrid(x + (cnt1 + free1) * i, y + (cnt1 + free1) * j) && board_[x + (cnt1 + free1) * i][y + (cnt1 + free1) * j] == -1) {
                free1++;
                if (!InGrid(x + (cnt1 + free1) * i, y + (cnt1 + free1) * j)) {
                    g1 = false;
                    break;
                }
            }
            while (InGrid(x - (cnt2 + free2) * i, y - (cnt2 + free2) * j) && board_[x - (cnt2 + free2) * i][y - (cnt2 + free2) * j] == -1) {
                free2++;
                if (!InGrid(x - (cnt2 + free2) * i, y - (cnt2 + free2) * j)) {
                    g2 = false;
                    break;
                }
            }

            bool gg1 = true, gg2 = true;
            int cntt1 = 1, cntt2 = 1;
            if (!InGrid(x + cntt1 * i, y + cntt1 * j)) {
                gg1 = false;
            }
            while (InGrid(x + cntt1 * i, y + cntt1 * j) && board_[x + cntt1 * i][y + cntt1 * j] == 1 - player_) {
                cntt1++;
                if (!InGrid(x + cntt1 * i, y + cntt1 * j)) {
                    gg1 = false;
                    break;
                }
            }
            if (!InGrid(x - cntt2 * i, y - cntt2 * j)) {
                gg2 = false;
            }
            while (InGrid(x - cntt2 * i, y - cntt2 * j) && board_[x - cntt2 * i][y - cntt2 * j] == 1 - player_) {
                cntt2++;
                if (!InGrid(x - cntt2 * i, y - cntt2 * j)) {
                    gg2 = false;
                    break;
                }
            }
            int freee1 = 0, freee2 = 0;
            while (InGrid(x + (cntt1 + freee1) * i, y + (cntt1 + freee1) * j) && board_[x + (cntt1 + freee1) * i][y + (cntt1 + freee1) * j] == -1) {
                freee1++;
                if (!InGrid(x + (cntt1 + freee1) * i, y + (cntt1 + freee1) * j)) {
                    gg1 = false;
                    break;
                }
            }
            while (InGrid(x - (cntt2 + freee2) * i, y - (cntt2 + freee2) * j) && board_[x - (cntt2 + freee2) * i][y - (cntt2 + freee2) * j] == -1) {
                freee2++;
                if (!InGrid(x - (cntt2 + freee2) * i, y - (cntt2 + freee2) * j)) {
                    gg2 = false;
                    break;
                }
            }
            if (player_ == 0) {
                if (cnt1 + cnt2 - 1 >= NEED_FIRST) {
                    ans += patterns[0];
                }
                else if (cnt1 + cnt2 >= NEED_FIRST && free1 >= 1 && free2 >= 1) {
                    ans += patterns[2];
                }
                else if (cnt1 + cnt2 + 1 >= NEED_FIRST && (free1 == 0 || free2 == 0)) {
                    ans += patterns[4];
                }
                else if (cnt1 + cnt2 + 2 >= NEED_FIRST && free1 + free2 >= 3) {
                    ans += patterns[5];
                }
                else {
                    //cout << ans << "----------------------\n";
                    ans += patterns[6] * (cnt1 + cnt2 - 1) + patterns[7] * (free1 + free2);
                    //cout << ans << "----------------------\n";
                }

                if (cntt1 + cntt2 - 1 >= NEED_SECOND) {
                    ans += patterns[1];
                }
                else if (cntt1 + cntt2 >= NEED_SECOND && freee1 >= 1 && freee2 >= 1) {
                    ans += patterns[3];
                }
                else if (freee1 == 0 && gg1 || freee2 == 0 && gg2) {
                    //cout << ans << "----------------------\n";
                    ans += patterns[7];
                    //cout << ans << "----------------------\n";
                }
            }
            else {
                if (cnt1 + cnt2 - 1 >= NEED_SECOND) {
                    ans += patterns[0];
                }
                else if (cnt1 + cnt2 >= NEED_SECOND && free1 >= 1 && free2 >= 1) {
                    ans += patterns[2];
                }
                else if (cnt1 + cnt2 + 1 >= NEED_SECOND && (free1 == 0 || free2 == 0)) {
                    ans += patterns[4];
                }
                else if (cnt1 + cnt2 + 2 >= NEED_SECOND && free1 + free2 >= 3) {
                    ans += patterns[5];
                }
                else {
                    //cout << ans << "----------------------\n";
                    ans += patterns[6] * (cnt1 + cnt2 - 1) + patterns[7] * (free1 + free2);
                    //cout << ans << "----------------------\n";
                }
                //cout << cntt1 << " " << cntt2 << "\n";
                if (cntt1 + cntt2 - 1 >= NEED_FIRST) {
                    ans += patterns[1];
                }
                else if (cntt1 + cntt2 >= NEED_FIRST && freee1 >= 1 && freee2 >= 1) {
                    ans += patterns[3];
                }
                else if (freee1 == 0 && gg1 || freee2 == 0 && gg2) {
                    //cout << ans << "----------------------\n";
                    ans += patterns[7];
                    //cout << ans << "----------------------\n";
                }
            }
            //cout << x << " " << y << " " << ans << " - \n";
        }
        return ans;
    }

    // Random move
    //void makeRandomMove() {
    //    int p = *std::next(freeFields.begin(), gen() % freeFields.size());
    //    //cout << (p >> MAX_LOG_GRID_SIZE) << " " << p - ((p >> MAX_LOG_GRID_SIZE) << MAX_LOG_GRID_SIZE) << std::endl;
    //    //cout << currentTurn << std::endl;
    //    makeMove(p >> MAX_LOG_GRID_SIZE, p - ((p >> MAX_LOG_GRID_SIZE) << MAX_LOG_GRID_SIZE));
    //}

    // Random move close to others
    void makeRandomCloseMove() {
        std::vector<std::pair<int, int>> untriedMoves;
        for (int i = 0; i < M_GRID_SIZE; i++) {
            for (int j = 0; j < N_GRID_SIZE; j++) {
                if (board_[i][j] == EMPTY) {
                    bool g = false;
                    for (int ii = -1; ii <= 1; ii++) {
                        if (i + ii < 0 || i + ii >= M_GRID_SIZE) {
                            continue;
                        }
                        for (int jj = -1; jj <= 1; jj++) {
                            if (j + jj < 0 || j + jj >= N_GRID_SIZE) {
                                continue;
                            }
                            if (board_[i + ii][j + jj] != EMPTY) {
                                g = true;
                                break;
                            }
                        }
                        if (g) {
                            break;
                        }
                    }
                    if (g) {
                        untriedMoves.push_back(std::pair<int, int>(i, j));
                    }
                }
            }
        }
        if (untriedMoves.empty()) {
            int i = M_GRID_SIZE / 2, j = N_GRID_SIZE / 2;
            for (int ii = -1; ii <= 1; ii++) {
                if (i + ii < 0 || i + ii >= M_GRID_SIZE) {
                    continue;
                }
                for (int jj = -1; jj <= 1; jj++) {
                    if (j + jj < 0 || j + jj >= N_GRID_SIZE) {
                        continue;
                    }
                    if (board_[i + ii][j + jj] == EMPTY) {
                        untriedMoves.push_back(std::pair<int, int>(i + ii, j + jj));
                    }
                }
            }
        }
        auto p = untriedMoves[gen() % untriedMoves.size()];
        makeMove(p.first, p.second);
    }

    // Simple minimax algo
    int makeMinimax(int x, int y, int turn) {
        checkWinner(x, y);
        if (!canWin()) {
            return 0;
        }
        if (winner_ == currentPlayer_) {
            return 10;
        }
        else if (winner_ != -1) {
            return -10;
        }
        int ans = -INF;
        std::vector<std::pair<int, int>> allmoves = allowedMoves(board_);
        for (auto [i, j] : allmoves) {
            board_[i][j] = sophisticated_queue[turn + 1];
            if (ans == -INF) {
                ans = makeMinimax(i, j, turn + 1);
            }
            else if (sophisticated_queue[turn] == currentPlayer_) {
                ans = std::min(ans, makeMinimax(i, j, turn + 1));
            }
            else {
                ans = std::max(ans, makeMinimax(i, j, turn + 1));
            }
            board_[i][j] = -1;
            winner_ = -1;
        }
        if (ans == -INF) {
            ans = 0;
        }
        return ans;
    }

    void makeMinimaxMove() {
        std::vector<std::pair<int, int>> allmoves = allowedMoves(board_);
        std::pair<int, int> ans;
        int val = -INF;
        for (auto [i, j] : allmoves) {
            board_[i][j] = currentPlayer_;
            int tmp = makeMinimax(i, j, currentTurn);
            if (val < tmp) {
                val = tmp;
                ans = { i, j };
            }
            board_[i][j] = -1;
            winner_ = -1;
        }
        makeMove(ans.first, ans.second);
    }

    // Alpha beta optimization
    int makeMinimaxWithAlphaBeta(int x, int y, int alpha, int beta, int turn) {
        checkWinner(x, y);
        if (!canWin()) {
            return 0;
        }
        if (winner_ == currentPlayer_) {
            return 10;
        }
        else if (winner_ != -1) {
            return -10;
        }
        std::vector<std::pair<int, int>> allmoves = allowedMoves(board_);
        int ans = -INF;
        for (auto [i, j] : allmoves) {
            board_[i][j] = sophisticated_queue[turn + 1];
            int result = makeMinimaxWithAlphaBeta(i, j, alpha, beta, turn + 1);
            if (sophisticated_queue[turn] == currentPlayer_) {
                beta = std::min(result, beta);
            }
            else {
                alpha = std::max(result, alpha);
            }
            if (ans == -INF) {
                ans = result;
            }
            else if (sophisticated_queue[turn] == currentPlayer_) {
                ans = std::min(ans, result);
            }
            else {
                ans = std::max(ans, result);
            }
            board_[i][j] = -1;
            winner_ = -1;
            if (alpha > beta) {
                break;
            }
        }
        if (ans == -INF) {
            ans = 0;
        }
        return ans;
    }

    void makeMinimaxWithAlphaBetaMove() {
        std::vector<std::pair<int, int>> allmoves = allowedMoves(board_);
        std::pair<int, int> ans;
        int val = -INF;
        for (auto [i, j] : allmoves) {
            board_[i][j] = currentPlayer_;
            int tmp = makeMinimaxWithAlphaBeta(i, j, -INF, +INF, currentTurn);
            if (tmp > val) {
                val = tmp;
                ans = { i, j };
            }
            board_[i][j] = -1;
            winner_ = -1;
        }
        makeMove(ans.first, ans.second);
    }

    // Using depth for stopping minimax
    int makeMinimaxWithDepth(int x, int y, int depth, int turn) {
        checkWinner(x, y);
        if (!canWin()) {
            return 0;
        }
        if (winner_ == currentPlayer_) {
            return 1000 - depth;
        }
        else if (winner_ != -1) {
            return -1000 + depth;
        }
        int ans = -INF;
        std::vector<std::pair<int, int>> allmoves = allowedMoves(board_);
        for (auto [i, j] : allmoves) {
            if (board_[i][j] == -1) {
                board_[i][j] = sophisticated_queue[turn + 1];
                if (depth + 1 <= MAX_DEPTH_ALPHA_BETA) {
                    int result = makeMinimaxWithDepth(i, j, depth + 1, turn + 1);
                    if (ans == -INF) {
                        ans = result;
                    }
                    else if (sophisticated_queue[turn] == currentPlayer_) {
                        ans = std::min(ans, result);
                    }
                    else {
                        ans = std::max(ans, result);
                    }
                }
                board_[i][j] = -1;
                winner_ = -1;
            }
        }
        if (ans == -INF) {
            ans = 0;
        }
        return ans;
    }

    void makeMinimaxWithDepthMove() {
        std::vector<std::pair<int, int>> allmoves = allowedMoves(board_);
        std::pair<int, int> ans;
        int val = -INF;
        for (auto [i, j] : allmoves) {
            if (board_[i][j] == -1) {
                board_[i][j] = currentPlayer_;
                int tmp = makeMinimaxWithDepth(i, j, 0, currentTurn);
                if (tmp > val) {
                    val = tmp;
                    ans = { i, j };
                }
                board_[i][j] = -1;
                winner_ = -1;
            }
        }
        makeMove(ans.first, ans.second);
    }

    // Using time limit for depth minimax
    int makeMinimaxWithDepthAndTime(int x, int y, int depth, int turn) {
        checkWinner(x, y);
        if (!canWin()) {
            return 0;
        }
        if (winner_ == currentPlayer_) {
            return 100 - depth;
        }
        else if (winner_ != -1) {
            return -100 + depth;
        }
        std::vector<std::pair<int, int>> allmoves = allowedMoves(board_);
        int ans = -INF;
        for (auto [i, j] : allmoves) {
            if (board_[i][j] == -1) {
                board_[i][j] = sophisticated_queue[turn + 1];
                int result = makeMinimaxWithDepthAndTime(i, j, depth + 1, turn + 1);
                if (ans == -INF) {
                    ans = result;
                }
                else if (sophisticated_queue[turn] == currentPlayer_) {
                    ans = std::min(ans, result);
                }
                else {
                    ans = std::max(ans, result);
                }
                board_[i][j] = -1;
                winner_ = -1;
                if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time) > time_limit) {
                    break;
                }
            }
        }
        if (ans == -INF) {
            ans = 0;
        }
        return ans;
    }

    void makeMinimaxWithDepthAndTimeMove() {
        std::vector<std::pair<int, int>> allmoves = allowedMoves(board_);
        std::pair<int, int> ans;
        int val = -INF;
        start_time = std::chrono::steady_clock::now();
        for (auto [i, j] : allmoves) {
            if (board_[i][j] == -1) {
                board_[i][j] = currentPlayer_;
                int tmp = makeMinimaxWithDepthAndTime(i, j, 0, currentTurn);
                if (tmp > val) {
                    val = tmp;
                    ans = { i, j };
                }
                board_[i][j] = -1;
                winner_ = -1;
                if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time) > time_limit) {
                    break;
                }
            }
        }
        makeMove(ans.first, ans.second);
    }

    // Greedy search using evaluating position
    void makeGreedySearch(int player) {
        int ans = -INF, x = 0, y = 0;
        std::vector<std::pair<int, int>> allmoves = allowedMoves(board_);
        for (auto [i, j] : allmoves) {
            int val = evaluatePosition(i, j, player);
            if (val > ans) {
                ans = val;
                x = i;
                y = j;
            }
            else if (val == ans && gen() % R_VARIOTY == 1) {
                ans = val;
                x = i;
                y = j;
            }
        }
        makeMove(x, y);
    }

    void makeGreedySearchMove() {
        makeGreedySearch(currentPlayer_);
    }

    // algorithm A* can't be used here, because it's impossible to find h-function for end position

    // Evaluation board position
    int evaluatePositionOnBoard(int current_player) {
        int ans = 0;
        if (current_player == 0) {
            int need_to_win = NEED_FIRST;
            int player_ = 0;
            if (N_GRID_SIZE >= need_to_win) {
                for (int i = 0; i < M_GRID_SIZE; i++) {
                    int cnt_empty = 0, cnt_my = 0;
                    for (int j = 0; j < N_GRID_SIZE; j++) {
                        if (board_[i][j] == -1) {
                            cnt_empty++;
                        }
                        else if (board_[i][j] == player_) {
                            cnt_my++;
                        }
                        else {
                            cnt_empty = 0;
                            cnt_my = 0;
                        }
                        if (cnt_my + cnt_empty == need_to_win + 1) {
                            if (cnt_empty == 2) {
                                ans += patterns[1];
                            }
                            else if (cnt_empty == 3) {
                                ans += patterns[5];
                            }
                            if (board_[i][j - need_to_win] == -1) {
                                cnt_empty--;
                            }
                            else {
                                cnt_my--;
                            }
                        }
                        if (cnt_my + cnt_empty == need_to_win) {
                            if (cnt_empty == 1) {
                                ans += patterns[0];
                            }
                            else if (cnt_empty == 2) {
                                ans += patterns[5];
                            }
                            ans += cnt_my * patterns[6];
                        }
                    }
                }
            }
            if (M_GRID_SIZE >= need_to_win) {
                for (int j = 0; j < N_GRID_SIZE; j++) {
                    int cnt_empty = 0, cnt_my = 0;
                    for (int i = 0; i < M_GRID_SIZE; i++) {
                        if (board_[i][j] == -1) {
                            cnt_empty++;
                        }
                        else if (board_[i][j] == player_) {
                            cnt_my++;
                        }
                        else {
                            cnt_empty = 0;
                            cnt_my = 0;
                        }
                        if (cnt_my + cnt_empty == need_to_win + 1) {
                            if (cnt_empty == 2) {
                                ans += patterns[1];
                            }
                            else if (cnt_empty == 3) {
                                ans += patterns[5];
                            }
                            if (board_[i - need_to_win][j] == -1) {
                                cnt_empty--;
                            }
                            else {
                                cnt_my--;
                            }
                        }
                        if (cnt_my + cnt_empty == need_to_win) {
                            if (cnt_empty == 1) {
                                ans += patterns[0];
                            }
                            else if (cnt_empty == 2) {
                                ans += patterns[5];
                            }
                            ans += cnt_my * patterns[6];
                        }
                    }
                }
            }
            if (N_GRID_SIZE >= need_to_win && M_GRID_SIZE >= need_to_win) {
                for (int ij = need_to_win - 1; ij < N_GRID_SIZE + M_GRID_SIZE - need_to_win; ij++) {
                    int cnt_empty = 0, cnt_my = 0;
                    for (int i = std::max(0, ij - N_GRID_SIZE + 1); i <= std::min(ij, M_GRID_SIZE - 1); i++) {
                        int j = ij - i;
                        if (board_[i][j] == -1) {
                            cnt_empty++;
                        }
                        else if (board_[i][j] == player_) {
                            cnt_my++;
                        }
                        else {
                            cnt_empty = 0;
                            cnt_my = 0;
                        }
                        if (cnt_my + cnt_empty == need_to_win + 1) {
                            if (cnt_empty == 2) {
                                ans += patterns[1];
                            }
                            else if (cnt_empty == 3) {
                                ans += patterns[5];
                            }
                            if (board_[i - need_to_win][j + need_to_win] == -1) {
                                cnt_empty--;
                            }
                            else {
                                cnt_my--;
                            }
                        }
                        if (cnt_my + cnt_empty == need_to_win) {
                            if (cnt_empty == 1) {
                                ans += patterns[0];
                            }
                            else if (cnt_empty == 2) {
                                ans += patterns[5];
                            }
                            ans += cnt_my * patterns[6];
                        }
                    }
                }
                for (int ij = M_GRID_SIZE - need_to_win; ij >= need_to_win - N_GRID_SIZE; ij--) {
                    int cnt_empty = 0, cnt_my = 0;
                    for (int j = std::max(0, -ij); j <= std::min(N_GRID_SIZE - 1, M_GRID_SIZE - ij - 1); j++) {
                        int i = ij + j;
                        if (board_[i][j] == -1) {
                            cnt_empty++;
                        }
                        else if (board_[i][j] == player_) {
                            cnt_my++;
                        }
                        else {
                            cnt_empty = 0;
                            cnt_my = 0;
                        }
                        if (cnt_my + cnt_empty == need_to_win + 1) {
                            if (cnt_empty == 2) {
                                ans += patterns[1];
                            }
                            else if (cnt_empty == 3) {
                                ans += patterns[5];
                            }
                            if (board_[i - need_to_win][j - need_to_win] == -1) {
                                cnt_empty--;
                            }
                            else {
                                cnt_my--;
                            }
                        }
                        if (cnt_my + cnt_empty == need_to_win) {
                            if (cnt_empty == 1) {
                                ans += patterns[0];
                            }
                            else if (cnt_empty == 2) {
                                ans += patterns[5];
                            }
                            ans += cnt_my * patterns[6];
                        }
                    }
                }
            }

            need_to_win = NEED_SECOND;
            player_ = 1;
            if (N_GRID_SIZE >= need_to_win) {
                for (int i = 0; i < M_GRID_SIZE; i++) {
                    int cnt_empty = 0, cnt_my = 0;
                    for (int j = 0; j < N_GRID_SIZE; j++) {
                        if (board_[i][j] == -1) {
                            cnt_empty++;
                        }
                        else if (board_[i][j] == player_) {
                            cnt_my++;
                        }
                        else {
                            cnt_empty = 0;
                            cnt_my = 0;
                        }
                        if (cnt_my + cnt_empty == need_to_win + 1) {
                            if (cnt_empty == 2) {
                                ans -= patterns[3];
                            }
                            else if (cnt_empty == 3) {
                                ans -= patterns[4];
                            }
                            if (board_[i][j - need_to_win] == -1) {
                                cnt_empty--;
                            }
                            else {
                                cnt_my--;
                            }
                        }
                        if (cnt_my + cnt_empty == need_to_win) {
                            if (cnt_empty == 0) {
                                ans -= patterns[2];
                            }
                            else if (cnt_empty == 1) {
                                ans -= patterns[4];
                            }
                            ans -= cnt_my * patterns[7];
                        }
                    }
                }
            }
            if (M_GRID_SIZE >= need_to_win) {
                for (int j = 0; j < N_GRID_SIZE; j++) {
                    int cnt_empty = 0, cnt_my = 0;
                    for (int i = 0; i < M_GRID_SIZE; i++) {
                        if (board_[i][j] == -1) {
                            cnt_empty++;
                        }
                        else if (board_[i][j] == player_) {
                            cnt_my++;
                        }
                        else {
                            cnt_empty = 0;
                            cnt_my = 0;
                        }
                        if (cnt_my + cnt_empty == need_to_win + 1) {
                            if (cnt_empty == 2) {
                                ans -= patterns[3];
                            }
                            else if (cnt_empty == 3) {
                                ans -= patterns[4];
                            }
                            if (board_[i - need_to_win][j] == -1) {
                                cnt_empty--;
                            }
                            else {
                                cnt_my--;
                            }
                        }
                        if (cnt_my + cnt_empty == need_to_win) {
                            if (cnt_empty == 0) {
                                ans -= patterns[2];
                            }
                            else if (cnt_empty == 1) {
                                ans -= patterns[4];
                            }
                            ans -= cnt_my * patterns[7];
                        }
                    }
                }
            }
            if (N_GRID_SIZE >= need_to_win && M_GRID_SIZE >= need_to_win) {
                for (int ij = need_to_win - 1; ij < N_GRID_SIZE + M_GRID_SIZE - need_to_win; ij++) {
                    int cnt_empty = 0, cnt_my = 0;
                    for (int i = std::max(0, ij - N_GRID_SIZE + 1); i <= std::min(ij, M_GRID_SIZE - 1); i++) {
                        int j = ij - i;
                        if (board_[i][j] == -1) {
                            cnt_empty++;
                        }
                        else if (board_[i][j] == player_) {
                            cnt_my++;
                        }
                        else {
                            cnt_empty = 0;
                            cnt_my = 0;
                        }
                        if (cnt_my + cnt_empty == need_to_win + 1) {
                            if (cnt_empty == 2) {
                                ans -= patterns[3];
                            }
                            else if (cnt_empty == 3) {
                                ans -= patterns[4];
                            }
                            if (board_[i - need_to_win][j + need_to_win] == -1) {
                                cnt_empty--;
                            }
                            else {
                                cnt_my--;
                            }
                        }
                        if (cnt_my + cnt_empty == need_to_win) {
                            if (cnt_empty == 0) {
                                ans -= patterns[2];
                            }
                            else if (cnt_empty == 1) {
                                ans -= patterns[4];
                            }
                            ans -= cnt_my * patterns[7];
                        }
                    }
                }
                for (int ij = M_GRID_SIZE - need_to_win; ij >= need_to_win - N_GRID_SIZE; ij--) {
                    int cnt_empty = 0, cnt_my = 0;
                    for (int j = std::max(0, -ij); j <= std::min(N_GRID_SIZE - 1, M_GRID_SIZE - ij - 1); j++) {
                        int i = ij + j;
                        if (board_[i][j] == -1) {
                            cnt_empty++;
                        }
                        else if (board_[i][j] == player_) {
                            cnt_my++;
                        }
                        else {
                            cnt_empty = 0;
                            cnt_my = 0;
                        }
                        if (cnt_my + cnt_empty == need_to_win + 1) {
                            if (cnt_empty == 2) {
                                ans -= patterns[3];
                            }
                            else if (cnt_empty == 3) {
                                ans -= patterns[4];
                            }
                            if (board_[i - need_to_win][j - need_to_win] == -1) {
                                cnt_empty--;
                            }
                            else {
                                cnt_my--;
                            }
                        }
                        if (cnt_my + cnt_empty == need_to_win) {
                            if (cnt_empty == 0) {
                                ans -= patterns[2];
                            }
                            else if (cnt_empty == 1) {
                                ans -= patterns[4];
                            }
                            ans -= cnt_my * patterns[7];
                        }
                    }
                }
            }
        }
        else {
            int need_to_win = NEED_SECOND;
            int player_ = 1;
            if (N_GRID_SIZE >= need_to_win) {
                for (int i = 0; i < M_GRID_SIZE; i++) {
                    int cnt_empty = 0, cnt_my = 0;
                    for (int j = 0; j < N_GRID_SIZE; j++) {
                        if (board_[i][j] == -1) {
                            cnt_empty++;
                        }
                        else if (board_[i][j] == player_) {
                            cnt_my++;
                        }
                        else {
                            cnt_empty = 0;
                            cnt_my = 0;
                        }
                        if (cnt_my + cnt_empty == need_to_win + 1) {
                            if (cnt_empty == 2) {
                                ans += patterns[1];
                            }
                            else if (cnt_empty == 3) {
                                ans += patterns[5];
                            }
                            if (board_[i][j - need_to_win] == -1) {
                                cnt_empty--;
                            }
                            else {
                                cnt_my--;
                            }
                        }
                        if (cnt_my + cnt_empty == need_to_win) {
                            if (cnt_empty == 1) {
                                ans += patterns[0];
                            }
                            else if (cnt_empty == 2) {
                                ans += patterns[5];
                            }
                            ans += cnt_my * patterns[6];
                        }
                    }
                }
            }
            if (M_GRID_SIZE >= need_to_win) {
                for (int j = 0; j < N_GRID_SIZE; j++) {
                    int cnt_empty = 0, cnt_my = 0;
                    for (int i = 0; i < M_GRID_SIZE; i++) {
                        if (board_[i][j] == -1) {
                            cnt_empty++;
                        }
                        else if (board_[i][j] == player_) {
                            cnt_my++;
                        }
                        else {
                            cnt_empty = 0;
                            cnt_my = 0;
                        }
                        if (cnt_my + cnt_empty == need_to_win + 1) {
                            if (cnt_empty == 2) {
                                ans += patterns[1];
                            }
                            else if (cnt_empty == 3) {
                                ans += patterns[5];
                            }
                            if (board_[i - need_to_win][j] == -1) {
                                cnt_empty--;
                            }
                            else {
                                cnt_my--;
                            }
                        }
                        if (cnt_my + cnt_empty == need_to_win) {
                            if (cnt_empty == 1) {
                                ans += patterns[0];
                            }
                            else if (cnt_empty == 2) {
                                ans += patterns[5];
                            }
                            ans += cnt_my * patterns[6];
                        }
                    }
                }
            }
            if (N_GRID_SIZE >= need_to_win && M_GRID_SIZE >= need_to_win) {
                for (int ij = need_to_win - 1; ij < N_GRID_SIZE + M_GRID_SIZE - need_to_win; ij++) {
                    int cnt_empty = 0, cnt_my = 0;
                    for (int i = std::max(0, ij - N_GRID_SIZE + 1); i <= std::min(ij, M_GRID_SIZE - 1); i++) {
                        int j = ij - i;
                        if (board_[i][j] == -1) {
                            cnt_empty++;
                        }
                        else if (board_[i][j] == player_) {
                            cnt_my++;
                        }
                        else {
                            cnt_empty = 0;
                            cnt_my = 0;
                        }
                        if (cnt_my + cnt_empty == need_to_win + 1) {
                            if (cnt_empty == 2) {
                                ans += patterns[1];
                            }
                            else if (cnt_empty == 3) {
                                ans += patterns[5];
                            }
                            if (board_[i - need_to_win][j + need_to_win] == -1) {
                                cnt_empty--;
                            }
                            else {
                                cnt_my--;
                            }
                        }
                        if (cnt_my + cnt_empty == need_to_win) {
                            if (cnt_empty == 1) {
                                ans += patterns[0];
                            }
                            else if (cnt_empty == 2) {
                                ans += patterns[5];
                            }
                            ans += cnt_my * patterns[6];
                        }
                    }
                }
                for (int ij = M_GRID_SIZE - need_to_win; ij >= need_to_win - N_GRID_SIZE; ij--) {
                    int cnt_empty = 0, cnt_my = 0;
                    for (int j = std::max(0, -ij); j <= std::min(N_GRID_SIZE - 1, M_GRID_SIZE - ij - 1); j++) {
                        int i = ij + j;
                        if (board_[i][j] == -1) {
                            cnt_empty++;
                        }
                        else if (board_[i][j] == player_) {
                            cnt_my++;
                        }
                        else {
                            cnt_empty = 0;
                            cnt_my = 0;
                        }
                        if (cnt_my + cnt_empty == need_to_win + 1) {
                            if (cnt_empty == 2) {
                                ans += patterns[1];
                            }
                            else if (cnt_empty == 3) {
                                ans += patterns[5];
                            }
                            if (board_[i - need_to_win][j - need_to_win] == -1) {
                                cnt_empty--;
                            }
                            else {
                                cnt_my--;
                            }
                        }
                        if (cnt_my + cnt_empty == need_to_win) {
                            if (cnt_empty == 1) {
                                ans += patterns[0];
                            }
                            else if (cnt_empty == 2) {
                                ans += patterns[5];
                            }
                            ans += cnt_my * patterns[6];
                        }
                    }
                }
            }

            need_to_win = NEED_FIRST;
            player_ = 0;
            if (N_GRID_SIZE >= need_to_win) {
                for (int i = 0; i < M_GRID_SIZE; i++) {
                    int cnt_empty = 0, cnt_my = 0;
                    for (int j = 0; j < N_GRID_SIZE; j++) {
                        if (board_[i][j] == -1) {
                            cnt_empty++;
                        }
                        else if (board_[i][j] == player_) {
                            cnt_my++;
                        }
                        else {
                            cnt_empty = 0;
                            cnt_my = 0;
                        }
                        if (cnt_my + cnt_empty == need_to_win + 1) {
                            if (cnt_empty == 2) {
                                ans -= patterns[3];
                            }
                            else if (cnt_empty == 3) {
                                ans -= patterns[4];
                            }
                            if (board_[i][j - need_to_win] == -1) {
                                cnt_empty--;
                            }
                            else {
                                cnt_my--;
                            }
                        }
                        if (cnt_my + cnt_empty == need_to_win) {
                            if (cnt_empty == 0) {
                                ans -= patterns[2];
                            }
                            else if (cnt_empty == 1) {
                                ans -= patterns[4];
                            }
                            ans -= cnt_my * patterns[7];
                        }
                    }
                }
            }
            if (M_GRID_SIZE >= need_to_win) {
                for (int j = 0; j < N_GRID_SIZE; j++) {
                    int cnt_empty = 0, cnt_my = 0;
                    for (int i = 0; i < M_GRID_SIZE; i++) {
                        if (board_[i][j] == -1) {
                            cnt_empty++;
                        }
                        else if (board_[i][j] == player_) {
                            cnt_my++;
                        }
                        else {
                            cnt_empty = 0;
                            cnt_my = 0;
                        }
                        if (cnt_my + cnt_empty == need_to_win + 1) {
                            if (cnt_empty == 2) {
                                ans -= patterns[3];
                            }
                            else if (cnt_empty == 3) {
                                ans -= patterns[4];
                            }
                            if (board_[i - need_to_win][j] == -1) {
                                cnt_empty--;
                            }
                            else {
                                cnt_my--;
                            }
                        }
                        if (cnt_my + cnt_empty == need_to_win) {
                            if (cnt_empty == 0) {
                                ans -= patterns[2];
                            }
                            else if (cnt_empty == 1) {
                                ans -= patterns[4];
                            }
                            ans -= cnt_my * patterns[7];
                        }
                    }
                }
            }
            if (N_GRID_SIZE >= need_to_win && M_GRID_SIZE >= need_to_win) {
                for (int ij = need_to_win - 1; ij < N_GRID_SIZE + M_GRID_SIZE - need_to_win; ij++) {
                    int cnt_empty = 0, cnt_my = 0;
                    for (int i = std::max(0, ij - N_GRID_SIZE + 1); i <= std::min(ij, M_GRID_SIZE - 1); i++) {
                        int j = ij - i;
                        if (board_[i][j] == -1) {
                            cnt_empty++;
                        }
                        else if (board_[i][j] == player_) {
                            cnt_my++;
                        }
                        else {
                            cnt_empty = 0;
                            cnt_my = 0;
                        }
                        if (cnt_my + cnt_empty == need_to_win + 1) {
                            if (cnt_empty == 2) {
                                ans -= patterns[3];
                            }
                            else if (cnt_empty == 3) {
                                ans -= patterns[4];
                            }
                            if (board_[i - need_to_win][j + need_to_win] == -1) {
                                cnt_empty--;
                            }
                            else {
                                cnt_my--;
                            }
                        }
                        if (cnt_my + cnt_empty == need_to_win) {
                            if (cnt_empty == 0) {
                                ans -= patterns[2];
                            }
                            else if (cnt_empty == 1) {
                                ans -= patterns[4];
                            }
                            ans -= cnt_my * patterns[7];
                        }
                    }
                }
                for (int ij = M_GRID_SIZE - need_to_win; ij >= need_to_win - N_GRID_SIZE; ij--) {
                    int cnt_empty = 0, cnt_my = 0;
                    for (int j = std::max(0, -ij); j <= std::min(N_GRID_SIZE - 1, M_GRID_SIZE - ij - 1); j++) {
                        int i = ij + j;
                        if (board_[i][j] == -1) {
                            cnt_empty++;
                        }
                        else if (board_[i][j] == player_) {
                            cnt_my++;
                        }
                        else {
                            cnt_empty = 0;
                            cnt_my = 0;
                        }
                        if (cnt_my + cnt_empty == need_to_win + 1) {
                            if (cnt_empty == 2) {
                                ans -= patterns[3];
                            }
                            else if (cnt_empty == 3) {
                                ans -= patterns[4];
                            }
                            if (board_[i - need_to_win][j - need_to_win] == -1) {
                                cnt_empty--;
                            }
                            else {
                                cnt_my--;
                            }
                        }
                        if (cnt_my + cnt_empty == need_to_win) {
                            if (cnt_empty == 0) {
                                ans -= patterns[2];
                            }
                            else if (cnt_empty == 1) {
                                ans -= patterns[4];
                            }
                            ans -= cnt_my * patterns[7];
                        }
                    }
                }
            }
        }
        return ans;
    }

    // Greedy search using evaluating position on full board
    void makeGreedySearch2(int player) {
        int ans = -INF, x = 0, y = 0;
        std::vector<std::pair<int, int>> allmoves = allowedMoves(board_);
        for (auto [i, j] : allmoves) {
            board_[i][j] = player;
            int val = -evaluatePositionOnBoard(1 - player);
            board_[i][j] = -1;
            if (val > ans) {
                ans = val;
                x = i;
                y = j;
            }
            else if (val == ans && gen() % R_VARIOTY == 1) {
                ans = val;
                x = i;
                y = j;
            }
        }
        makeMove(x, y);
    }

    void makeGreedySearch2Move() {
        makeGreedySearch2(currentPlayer_);
    }

    // Using depth for stopping minimax with new function
    int makeMinimaxWithDepth2(int x, int y, int depth, int player) {
        checkWinner(x, y);
        if (winner_ == 0) {
            return -INF + depth;
        }
        else if (winner_ == 1) {
            return INF - depth;
        }
        if (depth == MAX_DEPTH_MINIMAX_DEPTH) {
            if (player == 1) {
                return -evaluatePositionOnBoard(0);
            }
            else {
                return evaluatePositionOnBoard(1);
            }
        }
        std::vector<std::pair<std::pair<int, int>, int>> moves;
        std::unordered_set<int> close;
        for (auto [i, j] : moves_) {
            for (int ii = -1; ii <= 1; ii++) {
                if (i + ii < 0 || i + ii >= M_GRID_SIZE) {
                    continue;
                }
                for (int jj = -1; jj <= 1; jj++) {
                    if (j + jj < 0 || j + jj >= N_GRID_SIZE) {
                        continue;
                    }
                    if (board_[i + ii][j + jj] == -1) {
                        close.insert(((i + ii) << MAX_LOG_GRID_SIZE) + (j + jj));
                    }
                }
            }
        }
        for (auto u : close) {
            int i = u >> MAX_LOG_GRID_SIZE, j = u - ((u >> MAX_LOG_GRID_SIZE) << MAX_LOG_GRID_SIZE);
            if (board_[i][j] == -1) {
                board_[i][j] = 1 - player;
                moves_.push_back({ i, j });
                moves.push_back({ {i, j}, makeMinimaxWithDepth2(i, j, depth + 1, 1 - player) });
                moves_.pop_back();
                board_[i][j] = -1;
                winner_ = -1;
            }
        }
        if (moves.size() == 0) {
            return 0;
        }
        auto best = moves[0];
        if (player == 0) {
            for (auto i : moves) {
                if (i.second > best.second) {
                    best = i;
                }
            }
        }
        else {
            for (auto i : moves) {
                if (i.second < best.second) {
                    best = i;
                }
            }
        }
        return best.second;
    }

    void makeMinimaxWithDepth2Move() {
        std::vector<std::pair<std::pair<int, int>, int>> moves;
        std::unordered_set<int> close;
        for (auto [i, j] : moves_) {
            for (int ii = -1; ii <= 1; ii++) {
                if (i + ii < 0 || i + ii >= M_GRID_SIZE) {
                    continue;
                }
                for (int jj = -1; jj <= 1; jj++) {
                    if (j + jj < 0 || j + jj >= N_GRID_SIZE) {
                        continue;
                    }
                    if (board_[i + ii][j + jj] == -1) {
                        close.insert(((i + ii) << MAX_LOG_GRID_SIZE) + (j + jj));
                    }
                }
            }
        }
        for (auto u : close) {
            int i = u >> MAX_LOG_GRID_SIZE, j = u - ((u >> MAX_LOG_GRID_SIZE) << MAX_LOG_GRID_SIZE);
            if (board_[i][j] == -1) {
                board_[i][j] = 1;
                moves_.push_back({ i, j });
                moves.push_back({ {i, j}, makeMinimaxWithDepth2(i, j, 0, 1) });
                moves_.pop_back();
                board_[i][j] = -1;
                winner_ = -1;
            }
        }
        auto best = moves[0];
        for (auto i : moves) {
            if (i.second > best.second) {
                best = i;
            }
        }
        makeMove(best.first.first, best.first.second);
    }

    // luchevoi poisk v shirinu
    int makeRadialSearch(int x, int y, int depth, int player) {
        checkWinner(x, y);
        if (winner_ == 0) {
            return -INF + depth;
        }
        else if (winner_ == 1) {
            return INF - depth;
        }
        if (depth == MAX_DEPTH_RADIAL_SEARCH) {
            if (player == 1) {
                return -evaluatePositionOnBoard(0);
            }
            else {
                return evaluatePositionOnBoard(1);
            }
        }
        std::vector<std::pair<std::pair<int, int>, int>> moves;
        std::unordered_set<int> close;
        for (auto [i, j] : moves_) {
            for (int ii = -1; ii <= 1; ii++) {
                if (i + ii < 0 || i + ii >= M_GRID_SIZE) {
                    continue;
                }
                for (int jj = -1; jj <= 1; jj++) {
                    if (j + jj < 0 || j + jj >= N_GRID_SIZE) {
                        continue;
                    }
                    if (board_[i + ii][j + jj] == -1) {
                        close.insert(((i + ii) << MAX_LOG_GRID_SIZE) + (j + jj));
                    }
                }
            }
        }
        std::vector<std::pair<int, int>> close_moves;
        for (auto u : close) {
            int i = u >> MAX_LOG_GRID_SIZE, j = u - ((u >> MAX_LOG_GRID_SIZE) << MAX_LOG_GRID_SIZE);
            if (board_[i][j] == -1) {
                board_[i][j] = 1 - player;
                if (player == 1) {
                    close_moves.push_back({ u, -evaluatePositionOnBoard(0) });
                }
                else {
                    close_moves.push_back({ u, evaluatePositionOnBoard(1) });
                }
                board_[i][j] = -1;
                winner_ = -1;
            }
        }
        if (player == 0) {
            std::sort(close_moves.begin(), close_moves.end(), [](std::pair<int, int> l, std::pair<int, int> r) { return l.second < r.second; });
        }
        else {
            std::sort(close_moves.begin(), close_moves.end(), [](std::pair<int, int> l, std::pair<int, int> r) { return l.second > r.second; });
        }
        std::vector<int> top_close(std::min((int)close_moves.size(), MAX_CONDITIONS_ON_ONE_LEVEL));
        for (int i = 0; i < std::min((int)close_moves.size(), MAX_CONDITIONS_ON_ONE_LEVEL); i++) {
            top_close[i] = close_moves[i].first;
        }
        for (auto u : top_close) {
            int i = u >> MAX_LOG_GRID_SIZE, j = u - ((u >> MAX_LOG_GRID_SIZE) << MAX_LOG_GRID_SIZE);
            if (board_[i][j] == -1) {
                board_[i][j] = 1 - player;
                moves_.push_back({ i, j });
                moves.push_back({ {i, j}, makeRadialSearch(i, j, depth + 1, 1 - player) });
                moves_.pop_back();
                board_[i][j] = -1;
                winner_ = -1;
            }
        }
        if (moves.size() == 0) {
            return 0;
        }
        auto best = moves[0];
        if (player == 0) {
            for (auto i : moves) {
                if (i.second > best.second) {
                    best = i;
                }
            }
        }
        else {
            for (auto i : moves) {
                if (i.second < best.second) {
                    best = i;
                }
            }
        }
        return best.second;
    }
    void makeRadialSearchMove() {
        std::vector<std::pair<std::pair<int, int>, int>> moves;
        std::unordered_set<int> close;
        for (auto [i, j] : moves_) {
            for (int ii = -1; ii <= 1; ii++) {
                if (i + ii < 0 || i + ii >= M_GRID_SIZE) {
                    continue;
                }
                for (int jj = -1; jj <= 1; jj++) {
                    if (j + jj < 0 || j + jj >= N_GRID_SIZE) {
                        continue;
                    }
                    if (board_[i + ii][j + jj] == -1) {
                        close.insert(((i + ii) << MAX_LOG_GRID_SIZE) + (j + jj));
                    }
                }
            }
        }
        std::vector<std::pair<int, int>> close_moves;
        for (auto u : close) {
            int i = u >> MAX_LOG_GRID_SIZE, j = u - ((u >> MAX_LOG_GRID_SIZE) << MAX_LOG_GRID_SIZE);
            if (board_[i][j] == -1) {
                board_[i][j] = 1;
                close_moves.push_back({ u, evaluatePositionOnBoard(1) });
                board_[i][j] = -1;
                winner_ = -1;
            }
        }
        std::sort(close_moves.begin(), close_moves.end(), [](std::pair<int, int> l, std::pair<int, int> r) { return l.second > r.second; });
        std::vector<int> top_close(std::min((int)close_moves.size(), MAX_CONDITIONS_ON_ONE_LEVEL));
        for (int i = 0; i < std::min((int)close_moves.size(), MAX_CONDITIONS_ON_ONE_LEVEL); i++) {
            top_close[i] = close_moves[i].first;
        }
        for (auto u : top_close) {
            int i = u >> MAX_LOG_GRID_SIZE, j = u - ((u >> MAX_LOG_GRID_SIZE) << MAX_LOG_GRID_SIZE);
            if (board_[i][j] == -1) {
                board_[i][j] = 1;
                moves_.push_back({ i, j });
                moves.push_back({ {i, j}, makeRadialSearch(i, j, 0, 1) });
                moves_.pop_back();
                board_[i][j] = -1;
                winner_ = -1;
            }
        }
        auto best = moves[0];
        for (auto i : moves) {
            if (i.second > best.second) {
                best = i;
            }
        }
        makeMove(best.first.first, best.first.second);
    }
    
    // geneticheskiy algorithm is too slow
    // because of a lot of variants of boards and conditions for win it's stupid to learn bots

    std::vector<std::pair<int, int>> allowedMoves(std::vector<std::vector<int>>& board) {
        std::vector<std::pair<int, int>> untriedMoves;
        for (int i = 0; i < M_GRID_SIZE; i++) {
            for (int j = 0; j < N_GRID_SIZE; j++) {
                if (board[i][j] == EMPTY) {
                    bool g = false;
                    for (int ii = -1; ii <= 1; ii++) {
                        if (i + ii < 0 || i + ii >= M_GRID_SIZE) {
                            continue;
                        }
                        for (int jj = -1; jj <= 1; jj++) {
                            if (j + jj < 0 || j + jj >= N_GRID_SIZE) {
                                continue;
                            }
                            if (board[i + ii][j + jj] != EMPTY) {
                                g = true;
                                break;
                            }
                        }
                        if (g) {
                            break;
                        }
                    }
                    if (g) {
                        untriedMoves.push_back(std::pair<int, int>(i, j));
                    }
                }
            }
        }
        if (untriedMoves.empty()) {
            int i = M_GRID_SIZE / 2, j = N_GRID_SIZE / 2;
            for (int ii = -1; ii <= 1; ii++) {
                if (i + ii < 0 || i + ii >= M_GRID_SIZE) {
                    continue;
                }
                for (int jj = -1; jj <= 1; jj++) {
                    if (j + jj < 0 || j + jj >= N_GRID_SIZE) {
                        continue;
                    }
                    if (board[i + ii][j + jj] == EMPTY) {
                        untriedMoves.push_back(std::pair<int, int>(i + ii, j + jj));
                    }
                }
            }
        }
        return untriedMoves;
    }

    int giveWinner(int x, int y, std::vector<std::vector<int>>& board) {
        for (auto [i, j] : CheckDirections) {
            int cnt1 = 1, cnt2 = 1;
            while (InGrid(x + cnt1 * i, y + cnt1 * j) && board[x + cnt1 * i][y + cnt1 * j] == board[x][y]) {
                cnt1++;
            }
            while (InGrid(x - cnt2 * i, y - cnt2 * j) && board[x - cnt2 * i][y - cnt2 * j] == board[x][y]) {
                cnt2++;
            }
            if (board[x][y] == 0) {
                if (cnt1 + cnt2 - 1 >= NEED_FIRST) {
                    return 0;
                }
            }
            else {
                if (cnt1 + cnt2 - 1 >= NEED_SECOND) {
                    return 1;
                }
            }
        }
        return -1;
    }

    // monte carlo
    struct Node {
        std::vector<std::vector<int>> board;
        int player; // current player in this node
        std::pair<int, int> move; // move that we did to get into this node
        Node* parent;
        std::vector<Node*> children;
        double wins = 0; // win = 1, draw = 0.5, lose = 0
        int visits = 0;
        std::vector<std::pair<int, int>> untriedMoves;

        std::vector<std::pair<int, int>> allowedNodeMoves() {
            std::vector<std::pair<int, int>> untriedMoves;
            for (int i = 0; i < M_GRID_SIZE; i++) {
                for (int j = 0; j < N_GRID_SIZE; j++) {
                    if (board[i][j] == EMPTY) {
                        bool g = false;
                        for (int ii = -1; ii <= 1; ii++) {
                            if (i + ii < 0 || i + ii >= M_GRID_SIZE) {
                                continue;
                            }
                            for (int jj = -1; jj <= 1; jj++) {
                                if (j + jj < 0 || j + jj >= N_GRID_SIZE) {
                                    continue;
                                }
                                if (board[i + ii][j + jj] != EMPTY) {
                                    g = true;
                                    break;
                                }
                            }
                            if (g) {
                                break;
                            }
                        }
                        if (g) {
                            untriedMoves.push_back(std::pair<int, int>(i, j));
                        }
                    }
                }
            }
            if (untriedMoves.empty()) {
                int i = M_GRID_SIZE / 2, j = N_GRID_SIZE / 2;
                for (int ii = -1; ii <= 1; ii++) {
                    if (i + ii < 0 || i + ii >= M_GRID_SIZE) {
                        continue;
                    }
                    for (int jj = -1; jj <= 1; jj++) {
                        if (j + jj < 0 || j + jj >= N_GRID_SIZE) {
                            continue;
                        }
                        if (board[i + ii][j + jj] == EMPTY) {
                            untriedMoves.push_back(std::pair<int, int>(i + ii, j + jj));
                        }
                    }
                }
            }
            return untriedMoves;
        }

        Node(std::vector<std::vector<int>> brd, int p, const std::pair<int, int>& m, Node* par) : board(brd), player(p), move(m), parent(par) {
            untriedMoves = allowedNodeMoves();
        }

        ~Node() {
            for (auto child : children) {
                delete child;
            }
        }

        Node* selectChild() {
            Node* bestChild = nullptr;
            double bestValue = -1;
            for (auto child : children) {
                double uct = (double)child->wins / (child->visits + 1e-4) + sqrtl(2 * log(visits + 1) / (child->visits + 1e-4));
                cout << child->move.first << " " << child->move.second << " " << uct << std::endl;
                if (uct > bestValue) {
                    bestValue = uct;
                    bestChild = child;
                }
            }
            cout << std::endl;
            return bestChild;
        }

        Node* addChild(const std::pair<int, int>& m, int nextPlayer) {
            std::vector<std::vector<int>> newBoard = board;
            newBoard[m.first][m.second] = player;
            Node* child = new Node(newBoard, nextPlayer, m, this);
            auto it = remove_if(untriedMoves.begin(), untriedMoves.end(), [&m](const std::pair<int, int>& move) {
                return move.first == m.first && move.second == m.second;
                });
            untriedMoves.erase(it, untriedMoves.end());
            children.push_back(child);
            return child;
        }

        void update(double result) {
            visits++;
            wins += result;
        }
    };

    int simulate(std::vector<std::vector<int>> board, int player, std::pair<int, int> last) {
        int win = giveWinner(last.first, last.second, board);
        if (win != -1) {
            return win;
        }
        while (true) {
            std::vector<std::pair<int, int>> moves = allowedMoves(board);
            if (moves.empty()) {
                for (int i = 0; i < M_GRID_SIZE; i++) {
                    for (int j = 0; j < N_GRID_SIZE; j++) {
                        cout << board[i][j] << " ";
                    }
                    cout << std::endl;
                }
                cout << "lolkek" << std::endl;
                return -1;
            }
            int index = gen() % moves.size();
            cout << moves[index].first << " " << moves[index].second << std::endl;
            for (int i = 0; i < M_GRID_SIZE; i++) {
                for (int j = 0; j < N_GRID_SIZE; j++) {
                    cout << board[i][j] << " ";
                }
                cout << std::endl;
            }
            cout << "uuuuu" << std::endl;
            board[moves[index].first][moves[index].second] = player;
            int win = giveWinner(moves[index].first, moves[index].second, board);
            if (win != -1) {
                return win;
            }
            player = 1 - player;
        }
    }

    std::pair<int, int> MCTS(std::vector<std::vector<int>> rootBoard, int currentPlayer) {
        std::vector<std::pair<int, int>> legal = allowedMoves(rootBoard);

        if (legal.empty()) {
            return { -1, -1 };
        }

        for (const auto& move : legal) {
            rootBoard[move.first][move.second] = currentPlayer;
            int win = giveWinner(move.first, move.second, rootBoard);
            rootBoard[move.first][move.second] = -1;
            if (win != -1) {
                return move;
            }
        }
        start_time = std::chrono::steady_clock::now();
        Node* root = new Node(rootBoard, currentPlayer, std::make_pair(-1, -1), nullptr);
        for (int i = 0; i < M_GRID_SIZE; i++) {
            for (int j = 0; j < N_GRID_SIZE; j++) {
                cout << rootBoard[i][j] << " ";
            }
            cout << std::endl;
        }
        cout << std::endl;
        for (int i = 0;; i++) {
            if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time) > time_limit_for_MCST) {
                break;
            }
            Node* node = root;
            int player = currentPlayer;
            std::vector<std::vector<int>> board = rootBoard;
            std::pair<int, int> last = { -1,-1 };
            int result = -1;
            // Selection
            while (node->untriedMoves.empty() && !node->children.empty()) {
                node = node->selectChild();
                board[node->move.first][node->move.second] = player;
                last = node->move;
                player = node->player;
            }

            // Expansion
            if (result == -1 && !node->untriedMoves.empty()) {
                std::pair<int, int> m = node->untriedMoves[gen() % node->untriedMoves.size()];
                board[m.first][m.second] = node->player;
                last = m;
                node = node->addChild(m, 1 - node->player);
                player = node->player;
            }

            // Simulation
            if (result == -1) {
                result = simulate(board, player, last);
            }

            // Backpropagation
            while (node != nullptr) {
                if (node->player == result) {
                    node->update(1);
                }
                else if (result == -1) {
                    node->update(0.5);
                }
                else {
                    node->update(0);
                }
                node = node->parent;
            }
        }


        Node* bestChild = nullptr;
        double bestWinRate = 1.0;
        for (auto child : root->children) {
            double winRate = (double)child->wins / (child->visits + 1e-4);
            if (winRate <= bestWinRate) {
                bestWinRate = winRate;
                bestChild = child;
            }
        }
        std::pair<int, int> bestMove = bestChild->move;
        delete root;
        return bestMove;
    }

    void makeMCTSmove() {
        auto ans = MCTS(board_, currentPlayer_);
        makeMove(ans.first, ans.second);
    }

    // ant colonie


    std::vector<std::vector<int>> board_;
private:
    std::vector<std::pair<int, int>> moves_;
    int currentPlayer_ = 0;
    int winner_ = -1;
};

void drawBoard(sf::RenderWindow& window, const TicTacToe& game) {
    window.clear(sf::Color::White);
    for (int i = 0; i < M_GRID_SIZE; i++) {
        for (int j = 0; j < N_GRID_SIZE; j++) {
            sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
            cell.setPosition(j * CELL_SIZE, i * CELL_SIZE);
            cell.setOutlineThickness(BORDER);
            cell.setOutlineColor(sf::Color::Black);
            cell.setFillColor(sf::Color::Transparent);
            window.draw(cell);

            if (game.getBoard()[i][j] == 0) {
                sf::CircleShape circle(CELL_SIZE / 2 - 2 * BORDER);
                circle.setPosition(j * CELL_SIZE + 2 * BORDER, i * CELL_SIZE + 2 * BORDER);
                circle.setOutlineThickness(BORDER);
                circle.setOutlineColor(sf::Color::Black);
                circle.setFillColor(sf::Color::Transparent);
                window.draw(circle);
            }
            else if (game.getBoard()[i][j] == 1) {
                sf::CircleShape circle(CELL_SIZE / 2 - 2 * BORDER);
                circle.setPosition(j * CELL_SIZE + 2 * BORDER, i * CELL_SIZE + 2 * BORDER);
                circle.setOutlineThickness(BORDER);
                circle.setOutlineColor(sf::Color::Red);
                circle.setFillColor(sf::Color::Transparent);
                window.draw(circle);
            }
        }
    }
    window.display();
}

void showMenu() {
    sf::RenderWindow menuWindow(sf::VideoMode(400, 420), "Menu", sf::Style::Close);
    sf::Font font;
    font.loadFromFile("arial.ttf");

    sf::Text title("Settings", font, 30);
    title.setFillColor(sf::Color::Black);
    title.setPosition(100, 20);

    sf::Text boardWidthText("DeskWidth:", font, 20);
    boardWidthText.setFillColor(sf::Color::Black);
    boardWidthText.setPosition(50, 80);

    sf::Text gridWidthText(std::to_string(N_GRID_SIZE), font, 20);
    gridWidthText.setFillColor(sf::Color::Black);
    gridWidthText.setPosition(250, 80);

    sf::Text boardHeightText("DeskHeight:", font, 20);
    boardHeightText.setFillColor(sf::Color::Black);
    boardHeightText.setPosition(50, 120);

    sf::Text gridHeightText(std::to_string(M_GRID_SIZE), font, 20);
    gridHeightText.setFillColor(sf::Color::Black);
    gridHeightText.setPosition(250, 120);

    sf::Text winBlackConditionText("Black win condition: ", font, 20);
    winBlackConditionText.setFillColor(sf::Color::Black);
    winBlackConditionText.setPosition(50, 160);

    sf::Text conditionBlackText(std::to_string(NEED_FIRST), font, 20);
    conditionBlackText.setFillColor(sf::Color::Black);
    conditionBlackText.setPosition(250, 160);

    sf::Text winWhiteConditionText("White win condition: ", font, 20);
    winWhiteConditionText.setFillColor(sf::Color::Black);
    winWhiteConditionText.setPosition(50, 200);

    sf::Text conditionWhiteText(std::to_string(NEED_SECOND), font, 20);
    conditionWhiteText.setFillColor(sf::Color::Black);
    conditionWhiteText.setPosition(250, 200);

    std::string firstPlayerText1 = "First human";
    if (FIRST_PLAYER == 1) {
        firstPlayerText1 = "First easy bot";
    }
    else if (FIRST_PLAYER == 2) {
        firstPlayerText1 = "First medium bot";
    }
    else if (FIRST_PLAYER == 3) {
        firstPlayerText1 = "First hard bot";
    }
    sf::Text turnText1(firstPlayerText1, font, 20);
    turnText1.setFillColor(sf::Color::Black);
    turnText1.setPosition(50, 240);

    std::string firstPlayerText2 = "Second human";
    if (SECOND_PLAYER == 1) {
        firstPlayerText2 = "Second easy bot";
    }
    else if (SECOND_PLAYER == 2) {
        firstPlayerText2 = "Second medium bot";
    }
    else if (SECOND_PLAYER == 3) {
        firstPlayerText2 = "Second hard bot";
    }
    sf::Text turnText2(firstPlayerText2, font, 20);
    turnText2.setFillColor(sf::Color::Black);
    turnText2.setPosition(50, 285);

    std::string firstPlayerText3 = "Ordinary queue"; // (WB)
    if (QUEUE == 1) {
        firstPlayerText3 = "Marcel queue"; // W(BBWW)
    }
    else if (QUEUE == 2) {
        firstPlayerText3 = "(2, 2) queue"; // (WWBB)
    }
    else if (QUEUE == 3) {
        firstPlayerText3 = "Progressive queue"; // WBBWWWBBBB...
    }
    else if (QUEUE == 4) {
        firstPlayerText3 = "Random queue"; // with probability p
    }
    sf::Text turnText3(firstPlayerText3, font, 20);
    turnText3.setFillColor(sf::Color::Black);
    turnText3.setPosition(50, 330);

    std::string firstPlayerText4 = "Probability queue";
    sf::Text turnText4(firstPlayerText4, font, 20);
    turnText4.setFillColor(sf::Color::Black);
    turnText4.setPosition(50, 375);

    sf::RectangleShape inputBox(sf::Vector2f(100, 30));
    inputBox.setFillColor(sf::Color::White);
    inputBox.setOutlineColor(sf::Color::Black);
    inputBox.setOutlineThickness(2);
    inputBox.setPosition(250, 375);

    sf::Text inputText;
    inputText.setFont(font);
    inputText.setFillColor(sf::Color::Black);
    inputText.setCharacterSize(24);
    inputText.setPosition(inputBox.getPosition().x + 10, inputBox.getPosition().y + 1);
    std::string currentInput = "0.5";
    inputText.setString(currentInput);

    sf::RectangleShape boardWidthButtonUp(sf::Vector2f(30, 30));
    boardWidthButtonUp.setPosition(320, 80);
    boardWidthButtonUp.setFillColor(sf::Color::Green);
    sf::RectangleShape boardWidthButtonDown(sf::Vector2f(30, 30));
    boardWidthButtonDown.setPosition(285, 80);
    boardWidthButtonDown.setFillColor(sf::Color::Red);

    sf::RectangleShape boardHeightButtonUp(sf::Vector2f(30, 30));
    boardHeightButtonUp.setPosition(320, 120);
    boardHeightButtonUp.setFillColor(sf::Color::Green);
    sf::RectangleShape boardHeightButtonDown(sf::Vector2f(30, 30));
    boardHeightButtonDown.setPosition(285, 120);
    boardHeightButtonDown.setFillColor(sf::Color::Red);

    sf::RectangleShape conditionBlackButtonUp(sf::Vector2f(30, 30));
    conditionBlackButtonUp.setPosition(320, 160);
    conditionBlackButtonUp.setFillColor(sf::Color::Green);
    sf::RectangleShape conditionBlackButtonDown(sf::Vector2f(30, 30));
    conditionBlackButtonDown.setPosition(285, 160);
    conditionBlackButtonDown.setFillColor(sf::Color::Red);

    sf::RectangleShape conditionWhiteButtonUp(sf::Vector2f(30, 30));
    conditionWhiteButtonUp.setPosition(320, 200);
    conditionWhiteButtonUp.setFillColor(sf::Color::Green);
    sf::RectangleShape conditionWhiteButtonDown(sf::Vector2f(30, 30));
    conditionWhiteButtonDown.setPosition(285, 200);
    conditionWhiteButtonDown.setFillColor(sf::Color::Red);

    sf::RectangleShape toggleTurnButton1(sf::Vector2f(100, 30));
    toggleTurnButton1.setPosition(250, 240);
    toggleTurnButton1.setFillColor(sf::Color::Black);

    sf::RectangleShape toggleTurnButton2(sf::Vector2f(100, 30));
    toggleTurnButton2.setPosition(250, 285);
    toggleTurnButton2.setFillColor(sf::Color::Red);

    sf::RectangleShape toggleTurnButton3(sf::Vector2f(100, 30));
    toggleTurnButton3.setPosition(250, 330);
    toggleTurnButton3.setFillColor(sf::Color::Blue);

    while (menuWindow.isOpen()) {
        sf::Event event;
        while (menuWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                PROBABILITY = std::stof(currentInput);
                if (QUEUE == 0) { // (WB)
                    for (int i = 0; i < N_GRID_SIZE * M_GRID_SIZE; i++) {
                        sophisticated_queue.push_back(i & 1);
                    }
                }
                else if (QUEUE == 1) { // W(BBWW)
                    sophisticated_queue.push_back(0);
                    for (int i = 0; i < N_GRID_SIZE * M_GRID_SIZE - 1; i++) {
                        if (i % 4 < 2) {
                            sophisticated_queue.push_back(1);
                        }
                        else {
                            sophisticated_queue.push_back(0);
                        }
                    }
                }
                else if (QUEUE == 2) { // (WWBB)
                    for (int i = 0; i < N_GRID_SIZE * M_GRID_SIZE; i++) {
                        if (i % 4 < 2) {
                            sophisticated_queue.push_back(0);
                        }
                        else {
                            sophisticated_queue.push_back(1);
                        }
                    }
                }
                else if (QUEUE == 3) { // WBBWWWBBBB...
                    int turn = 0, cap = 1;
                    for (int i = 0; i < N_GRID_SIZE * M_GRID_SIZE; i++) {
                        if (cap > 0) {
                            sophisticated_queue.push_back(turn & 1);
                            cap--;
                        }
                        else {
                            turn++;
                            cap = turn;
                            sophisticated_queue.push_back(turn & 1);
                        }
                    }
                }
                else if (QUEUE == 4) { // with probability p
                    for (int i = 0; i < N_GRID_SIZE * M_GRID_SIZE; i++) {
                        if ((double)gen() / RAND_MAX / RAND_MAX / 4 < PROBABILITY) {
                            sophisticated_queue.push_back(0);
                        }
                        else {
                            sophisticated_queue.push_back(1);
                        }
                    }
                }
                menuWindow.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    // increasing sizes
                    if (boardWidthButtonUp.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        N_GRID_SIZE++;
                        N_WINDOW_SIZE = N_GRID_SIZE * CELL_SIZE;
                        gridWidthText.setString(std::to_string(N_GRID_SIZE));
                    }
                    else if (boardHeightButtonUp.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        M_GRID_SIZE++;
                        M_WINDOW_SIZE = M_GRID_SIZE * CELL_SIZE;
                        gridHeightText.setString(std::to_string(M_GRID_SIZE));
                    }
                    // decreasing sizes
                    else if (boardWidthButtonDown.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        N_GRID_SIZE--;
                        N_WINDOW_SIZE = N_GRID_SIZE * CELL_SIZE;
                        gridWidthText.setString(std::to_string(N_GRID_SIZE));
                    }
                    else if (boardHeightButtonDown.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        M_GRID_SIZE--;
                        M_WINDOW_SIZE = M_GRID_SIZE * CELL_SIZE;
                        gridHeightText.setString(std::to_string(M_GRID_SIZE));
                    }
                    // increasing win condition
                    else if (conditionBlackButtonUp.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        NEED_FIRST++;
                        conditionBlackText.setString(std::to_string(NEED_FIRST));
                    }
                    else if (conditionWhiteButtonUp.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        NEED_SECOND++;
                        conditionWhiteText.setString(std::to_string(NEED_SECOND));
                    }
                    // decreasing win condition
                    else if (conditionBlackButtonDown.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        NEED_FIRST--;
                        conditionBlackText.setString(std::to_string(NEED_FIRST));
                    }
                    else if (conditionWhiteButtonDown.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        NEED_SECOND--;
                        conditionWhiteText.setString(std::to_string(NEED_SECOND));
                    }
                    // choose first player or bot
                    else if (toggleTurnButton1.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        FIRST_PLAYER = (FIRST_PLAYER + 1) % 4;
                        if (FIRST_PLAYER == 1) {
                            firstPlayerText1 = "First easy bot";
                        }
                        else if (FIRST_PLAYER == 2) {
                            firstPlayerText1 = "First medium bot";
                        }
                        else if (FIRST_PLAYER == 3) {
                            firstPlayerText1 = "First hard bot";
                        }
                        else {
                            firstPlayerText1 = "First human";
                        }
                        turnText1.setString(firstPlayerText1);
                    }
                    else if (toggleTurnButton2.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        SECOND_PLAYER = (SECOND_PLAYER + 1) % 4;
                        if (SECOND_PLAYER == 1) {
                            firstPlayerText2 = "Second easy bot";
                        }
                        else if (SECOND_PLAYER == 2) {
                            firstPlayerText2 = "Second medium bot";
                        }
                        else if (SECOND_PLAYER == 3) {
                            firstPlayerText2 = "Second hard bot";
                        }
                        else {
                            firstPlayerText2 = "Second human";
                        }
                        turnText2.setString(firstPlayerText2);
                    }
                    else if (toggleTurnButton3.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        QUEUE = (QUEUE + 1) % 5;
                        if (QUEUE == 1) {
                            firstPlayerText3 = "Marcel queue"; // W(BBWW)
                        }
                        else if (QUEUE == 2) {
                            firstPlayerText3 = "(2, 2) queue"; // (WWBB)
                        }
                        else if (QUEUE == 3) {
                            firstPlayerText3 = "Progressive queue"; // WBBWWWBBBB...
                        }
                        else if (QUEUE == 4) {
                            firstPlayerText3 = "Random queue"; // with probability p
                        }
                        else {
                            firstPlayerText3 = "Ordinary queue"; // (WB)
                        }
                        turnText3.setString(firstPlayerText3);
                    }
                }
            }
            if (event.type == sf::Event::TextEntered) {
                // Обработка клавиши Backspace (ASCII 8)
                if (event.text.unicode == 8) {
                    if (!currentInput.empty())
                        currentInput.pop_back();
                }
                // Обработка вводимых символов (ASCII < 128)
                else if (event.text.unicode < 128) {
                    char enteredChar = static_cast<char>(event.text.unicode);
                    // Разрешаем ввод только цифр, точки и знака минус
                    if ((enteredChar >= '0' && enteredChar <= '9') || enteredChar == '.') {
                        // Разрешаем только одну точку
                        if (enteredChar == '.' && currentInput.find('.') != std::string::npos)
                            continue;
                        currentInput += enteredChar;
                    }
                }
                // Обновляем отображаемый текст
                inputText.setString(currentInput);
            }
        }

        menuWindow.clear(sf::Color::White);
        menuWindow.draw(title);
        menuWindow.draw(boardWidthText);
        menuWindow.draw(gridWidthText);
        menuWindow.draw(boardHeightText);
        menuWindow.draw(gridHeightText);
        menuWindow.draw(winBlackConditionText);
        menuWindow.draw(conditionBlackText);
        menuWindow.draw(winWhiteConditionText);
        menuWindow.draw(conditionWhiteText);
        menuWindow.draw(turnText1);
        menuWindow.draw(turnText2);
        menuWindow.draw(turnText3);
        menuWindow.draw(turnText4);
        menuWindow.draw(boardWidthButtonUp);
        menuWindow.draw(boardWidthButtonDown);
        menuWindow.draw(boardHeightButtonUp);
        menuWindow.draw(boardHeightButtonDown);
        menuWindow.draw(conditionBlackButtonUp);
        menuWindow.draw(conditionBlackButtonDown);
        menuWindow.draw(conditionWhiteButtonUp);
        menuWindow.draw(conditionWhiteButtonDown);
        menuWindow.draw(toggleTurnButton1);
        menuWindow.draw(toggleTurnButton2);
        menuWindow.draw(toggleTurnButton3);
        menuWindow.draw(inputBox);
        menuWindow.draw(inputText);
        menuWindow.display();
    }
}

void showWinnerWindow(sf::RenderWindow& window, int winner) {
    sf::RenderWindow winnerWindow(sf::VideoMode(300, 200), "Won");

    sf::Font font;
    font.loadFromFile("arial.ttf");
    std::string message = "Draw!";
    if (winner == 0) {
        message = "Black won!";
    }
    if (winner == 1) {
        message = "Red won!";
    }
    sf::Text text(message, font, 30);
    text.setFillColor(sf::Color::Black);
    text.setPosition(50, 50);

    sf::RectangleShape restartButton(sf::Vector2f(100, 50));
    restartButton.setFillColor(sf::Color::Green);
    restartButton.setPosition(40, 120);

    sf::Text buttonText("Restart", font, 20);
    buttonText.setFillColor(sf::Color::Black);
    buttonText.setPosition(50, 135);

    sf::RectangleShape menuButton(sf::Vector2f(100, 50));
    menuButton.setFillColor(sf::Color::Yellow);
    menuButton.setPosition(140, 120);

    sf::Text buttonMenuText("Menu", font, 20);
    buttonMenuText.setFillColor(sf::Color::Black);
    buttonMenuText.setPosition(160, 135);

    while (winnerWindow.isOpen()) {
        sf::Event event;
        while (winnerWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                winnerWindow.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::FloatRect buttonBounds = restartButton.getGlobalBounds();
                    if (buttonBounds.contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y))) {
                        winnerWindow.close();
                    }
                    sf::FloatRect buttonMenuBounds = menuButton.getGlobalBounds();
                    if (buttonMenuBounds.contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y))) {
                        showMenu();
                    }
                }
            }
        }

        winnerWindow.clear(sf::Color::White);
        winnerWindow.draw(text);
        winnerWindow.draw(restartButton);
        winnerWindow.draw(buttonText);
        winnerWindow.draw(menuButton);
        winnerWindow.draw(buttonMenuText);
        winnerWindow.display();
    }
}

void newWindow();

void newWindow() {
    sf::RenderWindow window(sf::VideoMode(N_WINDOW_SIZE, M_WINDOW_SIZE), "XYK");
    TicTacToe game;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (sophisticated_queue[currentTurn] == 0) {
                if (FIRST_PLAYER == 0) {
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            int x = event.mouseButton.x / CELL_SIZE;
                            int y = event.mouseButton.y / CELL_SIZE;
                            game.makeMove(y, x);
                        }
                    }
                }
                else if (FIRST_PLAYER == 1) {
                    std::this_thread::sleep_for(std::chrono::nanoseconds(100'000'000));

                    //game.makeRandomCloseMove();
                    //game.makeMinimaxMove();
                    //game.makeMinimaxWithAlphaBetaMove();
                    //game.makeMinimaxWithDepthMove();
                    //game.makeMCTSmove();
                }
                else if (FIRST_PLAYER == 2) {
                    std::this_thread::sleep_for(std::chrono::nanoseconds(100'000'000));

                    //game.makeGreedySearchMove();
                    //game.makeGreedySearch2Move();
                    //game.makeRadialSearchMove();
                }
                else if (FIRST_PLAYER == 3) {
                    std::this_thread::sleep_for(std::chrono::nanoseconds(100'000'000));

                    //game.makeMinimaxWithDepth2Move();
                    //game.makeMinimaxWithDepthAndTimeMove();
                }
            }
            else {
                if (SECOND_PLAYER == 0) {
                    if (event.type == sf::Event::MouseButtonPressed) {
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            int x = event.mouseButton.x / CELL_SIZE;
                            int y = event.mouseButton.y / CELL_SIZE;
                            game.makeMove(y, x);
                        }
                    }
                }
                else if (SECOND_PLAYER == 1) {
                    std::this_thread::sleep_for(std::chrono::nanoseconds(100'000'000));

                    game.makeRandomCloseMove();
                    //game.makeMinimaxMove();
                    //game.makeMinimaxWithAlphaBetaMove();
                    //game.makeMinimaxWithDepthMove();
                    //game.makeMCTSmove();
                }
                else if (SECOND_PLAYER == 2) {
                    std::this_thread::sleep_for(std::chrono::nanoseconds(100'000'000));

                    //game.makeGreedySearchMove();
                    //game.makeGreedySearch2Move();
                    game.makeRadialSearchMove();
                }
                else if (SECOND_PLAYER == 3) {
                    std::this_thread::sleep_for(std::chrono::nanoseconds(100'000'000));

                    //game.makeMinimaxWithDepth2Move();
                    game.makeMinimaxWithDepthAndTimeMove();
                }
            }
        }
        drawBoard(window, game);
        if (!game.canWin()) {
            showWinnerWindow(window, 2);
            window.clear();
            game.reset();
        }
        if (game.getWinner() != EMPTY) {
            std::this_thread::sleep_for(std::chrono::nanoseconds(500'000'000));
            showWinnerWindow(window, game.getWinner());
            window.clear();
            game.reset();
        }
        if (window.getSize().x != N_WINDOW_SIZE || window.getSize().y != M_WINDOW_SIZE) {
            window.close();
            newWindow();
        }
    }
}

int main() { 

    showMenu();
    newWindow();

    return 0;
}