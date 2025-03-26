#include "xyk.h"

unsigned long long int randomInt() {
    std::uniform_int_distribution<unsigned long long int> dist(0, UINT64_MAX);
    return dist(gen);
}

XYK::XYK() : board_(M_GRID_SIZE, std::vector<int>(N_GRID_SIZE, EMPTY)), currentPlayer_(0), winner_(-1) {
    ZobristTable = std::vector<std::vector<std::vector<unsigned long long int>>>(M_GRID_SIZE, std::vector<std::vector<unsigned long long int>>(N_GRID_SIZE, std::vector<unsigned long long int>(2)));
    for (int i = 0; i < M_GRID_SIZE; i++) {
        for (int j = 0; j < N_GRID_SIZE; j++) {
            ZobristTable[i][j][0] = randomInt();
            ZobristTable[i][j][1] = randomInt();
        }
    }
}

void XYK::reset() {
    board_.assign(M_GRID_SIZE, std::vector<int>(N_GRID_SIZE, EMPTY));
    moves_.clear();
    costPosition.clear();
    ZobristTable = std::vector<std::vector<std::vector<unsigned long long int>>>(M_GRID_SIZE, std::vector<std::vector<unsigned long long int>>(N_GRID_SIZE, std::vector<unsigned long long int>(2)));
    for (int i = 0; i < M_GRID_SIZE; i++) {
        for (int j = 0; j < N_GRID_SIZE; j++) {
            ZobristTable[i][j][0] = randomInt();
            ZobristTable[i][j][1] = randomInt();
        }
    }
    sophisticated_queue.clear();
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
    sophisticated_queue.push_back(1 - sophisticated_queue.back());
    currentHash = 0;
    currentTurn = 0;
    currentPlayer_ = sophisticated_queue[0];
    winner_ = -1;
}

bool XYK::InGrid(int x, int y) {
    if (x >= 0 && y >= 0 && x < M_GRID_SIZE && y < N_GRID_SIZE) {
        return true;
    }
    return false;
}

bool XYK::makeMove(int x, int y) {
    if (board_[x][y] == EMPTY) {
        moves_.push_back({ x, y });
        board_[x][y] = currentPlayer_;
        currentHash ^= ZobristTable[x][y][currentPlayer_];
        checkWinner(x, y);
        currentTurn++;
        currentPlayer_ = sophisticated_queue[currentTurn];
        return true;
    }
    return false;
}

int XYK::getWinner() {
    return winner_;
}

int XYK::getPlayer() {
    return currentPlayer_;
}

const std::vector<std::vector<int>>& XYK::getBoard() const {
    return board_;
}

const std::vector<std::pair<int, int>>& XYK::getMoves() const {
    return moves_;
}

void XYK::checkWinner(int x, int y) {
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

bool XYK::canWin() {
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

std::vector<std::pair<int, int>> XYK::allowedMoves(std::vector<std::vector<int>>& board) {
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
        if (M_GRID_SIZE % 2 == 1 && N_GRID_SIZE % 2 == 1 && board[i][j] == EMPTY) {
            untriedMoves.push_back(std::pair<int, int>(i, j));
        }
        else {
            for (int ii = -1; ii <= 0; ii++) {
                if (i + ii < 0 || i + ii >= M_GRID_SIZE) {
                    continue;
                }
                for (int jj = -1; jj <= 0; jj++) {
                    if (j + jj < 0 || j + jj >= N_GRID_SIZE) {
                        continue;
                    }
                    if (board[i + ii][j + jj] == EMPTY) {
                        untriedMoves.push_back(std::pair<int, int>(i + ii, j + jj));
                    }
                }
            }
        }
    }
    return untriedMoves;
}

int XYK::giveWinner(int x, int y, std::vector<std::vector<int>>& board) {
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
