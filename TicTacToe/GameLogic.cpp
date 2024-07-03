#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

class TicTacToeGame {
private:
    vector<char> board;
    char current_winner;

    void print_board() {
        for (int i = 0; i < 9; i++) {
            cout << board[i] << " ";
            if ((i + 1) % 3 == 0) {
                cout << endl;
            }
        }
    }

    bool is_winner(char player) {
        const int winning_combinations[8][3] = {
            {0, 1, 2}, {3, 4, 5}, {6, 7, 8},
            {0, 3, 6}, {1, 4, 7}, {2, 5, 8},
            {0, 4, 8}, {2, 4, 6}
        };
        for (auto& combo : winning_combinations) {
            if (board[combo[0]] == player && board[combo[1]] == player && board[combo[2]] == player) {
                return true;
            }
        }
        return false;
    }

    bool is_draw() {
        for (char cell : board) {
            if (cell == ' ') {
                return false;
            }
        }
        return true;
    }

    int get_move() {
        int move;
        while (true) {
            cout << "Enter your move (0-8): ";
            cin >> move;
            if (move >= 0 && move < 9 && board[move] == ' ') {
                return move;
            }
            cout << "Invalid move. Try again." << endl;
        }
    }

    int get_ai_move() {
        vector<int> available_moves;
        for (int i = 0; i < 9; i++) {
            if (board[i] == ' ') {
                available_moves.push_back(i);
            }
        }
        return available_moves[rand() % available_moves.size()];
    }

public:
    TicTacToeGame() : board(9, ' '), current_winner(' ') {
        srand(time(0));
    }

    void play_game(UserManager& user_manager, const string& username) {
        cout << "Welcome to Tic-Tac-Toe!" << endl;
        char player = 'X';
        while (true) {
            print_board();
            int move;
            if (player == 'X') {
                move = get_move();
            } else {
                move = get_ai_move();
                cout << "AI moves to " << move << endl;
            }
            board[move] = player;
            if (is_winner(player)) {
                print_board();
                cout << player << " wins!" << endl;
                if (player == 'X') {
                    user_manager.update_record(username, true);
                } else {
                    user_manager.update_record(username, false);
                }
                break;
            }
            if (is_draw()) {
                print_board();
                cout << "It's a draw!" << endl;
                break;
            }
            player = (player == 'X') ? 'O' : 'X';
        }
    }
};
