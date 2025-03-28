#include "xyk.h"

int XYK::evaluatePosition(int x, int y, int player_) {
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
        if (cnt1 + cnt2 - 1 >= NEED_FIRST) {
            ans += patterns[0];
        }
        else if (cnt1 + cnt2 >= NEED_FIRST && free1 >= 1 && free2 >= 1) {
            ans += patterns[1];
        }
        else if (cnt1 + cnt2 + 1 >= NEED_FIRST && (free1 == 0 || free2 == 0)) {
            ans += patterns[4];
        }
        else if (cnt1 + cnt2 + 2 >= NEED_FIRST && free1 + free2 >= 3) {
            ans += patterns[5];
        }
        else {
            ans += patterns[6] * (cnt1 + cnt2 - 1) + patterns[7] * (free1 + free2);
        }

        if (cntt1 + cntt2 - 1 >= NEED_SECOND) {
            ans += patterns[2];
        }
        else if (cntt1 + cntt2 >= NEED_SECOND && freee1 >= 1 && freee2 >= 1) {
            ans += patterns[3];
        }
        else if (freee1 == 0 && gg1 || freee2 == 0 && gg2) {
            ans += patterns[7];
        }
    }
    return ans;
}

int XYK::evaluatePositionOnBoard(int current_player) {
    int ans = 0;
    if (current_player == 0) {
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
                        if (cnt_empty == 1) {
                            ans -= patterns[1];
                        }
                        else if (cnt_empty == 2) {
                            ans -= patterns[5];
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
                            ans -= patterns[0];
                        }
                        else if (cnt_empty == 1) {
                            ans -= patterns[5];
                        }
                        ans -= cnt_my * patterns[6];
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
                        if (cnt_empty == 1) {
                            ans -= patterns[1];
                        }
                        else if (cnt_empty == 2) {
                            ans -= patterns[5];
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
                            ans -= patterns[0];
                        }
                        else if (cnt_empty == 1) {
                            ans -= patterns[5];
                        }
                        ans -= cnt_my * patterns[6];
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
                        if (cnt_empty == 1) {
                            ans -= patterns[1];
                        }
                        else if (cnt_empty == 2) {
                            ans -= patterns[5];
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
                            ans -= patterns[0];
                        }
                        else if (cnt_empty == 1) {
                            ans -= patterns[5];
                        }
                        ans -= cnt_my * patterns[6];
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
                            ans += patterns[3];
                        }
                        else if (cnt_empty == 3) {
                            ans += patterns[4];
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
                            ans += patterns[2];
                        }
                        else if (cnt_empty == 2) {
                            ans += patterns[4];
                        }
                        ans += cnt_my * patterns[7];
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
                            ans += patterns[3];
                        }
                        else if (cnt_empty == 3) {
                            ans += patterns[4];
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
                            ans += patterns[2];
                        }
                        else if (cnt_empty == 2) {
                            ans += patterns[4];
                        }
                        ans += cnt_my * patterns[7];
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
                            ans += patterns[3];
                        }
                        else if (cnt_empty == 3) {
                            ans += patterns[4];
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
                            ans += patterns[2];
                        }
                        else if (cnt_empty == 2) {
                            ans += patterns[4];
                        }
                        ans += cnt_my * patterns[7];
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
                            ans += patterns[3];
                        }
                        else if (cnt_empty == 3) {
                            ans += patterns[4];
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
                            ans += patterns[2];
                        }
                        else if (cnt_empty == 2) {
                            ans += patterns[4];
                        }
                        ans += cnt_my * patterns[7];
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
                        if (cnt_empty == 1) {
                            ans -= patterns[3];
                        }
                        else if (cnt_empty == 2) {
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
                        if (cnt_empty == 1) {
                            ans -= patterns[3];
                        }
                        else if (cnt_empty == 2) {
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
                        if (cnt_empty == 1) {
                            ans -= patterns[3];
                        }
                        else if (cnt_empty == 2) {
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
                        if (cnt_empty == 1) {
                            ans -= patterns[3];
                        }
                        else if (cnt_empty == 2) {
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
