#include <iostream>
#include <iomanip>
#include <algorithm>
#include <limits>

using namespace std;

class Game
{
    enum class Player
    {
        blank = ' ',
        person = '0',
        algorithm = 'X'
    };

    struct Move
    {
        int x = 0;
        int y = 0;
    };

    static constexpr int coordinate = 3; //Constexpr used to Make the expression constant on start. Don't know why the fuck normal ways don't work. Also, due to nest loops and recursion the code is slow af on 6 r&c.
    Player board[coordinate][coordinate];
    int empty;

public:
    Game() : empty(coordinate* coordinate)
    {
        for (int i = 0; i < coordinate; i++)
        {
            for (int j = 0; j < coordinate; j++)
            {
                board[i][j] = Player::blank;
            }
        }
    }

    void play()
    {
        int turn = 0;
        bool breaker = false; 

        printBoard();
        cout << "Enter your move by using your numpad." << endl;

        do
        {
            //Human Turn
            if (turn%2==0)
            {
                getHumanMove();
                empty--;
                //Useless because Humans will never win.
                if (checkWin(Player::person))
                {
                    cout << "This Code doesn't work." << endl; //If they do win, sue me.
                    breaker = true;
                }
            }
            else
            {
                cout << endl << "Computer Move: ";

                Move aimove = minimax();

                cout << aimove.x << aimove.y << endl;

                board[aimove.x][aimove.y] = Player::algorithm;
                empty--;
                if (checkWin(Player::algorithm))
                {
                    cout << "Computer Wins\n";
                    breaker = true;
                }
            }

            if (isTie())
            {
                cout << endl << "*** Tie ***" << endl;
                breaker = true;
            }

            turn++;
            printBoard();

        } while (!breaker);
    }

private:
    void printBoard()
    {
        int loc = 0;
        system("CLS");
        cout << endl;
        for (int x = 0; x < 3; x++)
        {
            cout << "       |     |     \n";

            for (int y = 0; y < 3; y++)
            {
                if (y == 0)
                    cout << "  ";
                cout << "  " << static_cast<char>(board[x][y]);
                if (y == 2)
                {
                    cout << endl;
                }
                else
                    cout << "  |";
            }
            cout << "      " << ++loc << "|    ";
            cout << ++loc << "|   ";
            cout << ++loc << endl;
            if (x != 2)
                cout << "  =====|=====|=====" << endl;
        }
    }

    bool isTie()
    {
        return empty == 0; //Basically if (Empty Tiles == 0) {return == true}
    }

    bool checkWin(Player player)
    {
        // check for row or column wins
        for (int i = 0; i < coordinate; ++i)
        {
            bool rowwin = true;
            bool colwin = true;
            for (int j = 0; j < coordinate; ++j)
            {
                rowwin &= board[i][j] == player;
                colwin &= board[j][i] == player;
            }
            if (colwin || rowwin)
                return true;
        }

        // check for diagonal wins
        bool diagonalwin = true;
        for (int i = 0; i < coordinate; ++i)
            diagonalwin &= board[i][i] == player;

        if (diagonalwin)
            return true;

        diagonalwin = true;
        for (int i = 0; i < coordinate; ++i)
            diagonalwin &= board[coordinate - i - 1][i] == player;

        return diagonalwin;
    }


    Move minimax()
    {
        int score = numeric_limits<int>::max();
        Move move;
        int level = 0;

        for (int i = 0; i < coordinate; i++)
        {
            for (int j = 0; j < coordinate; j++)
            {
                if (board[i][j] == Player::blank)
                {
                    board[i][j] = Player::algorithm;
                    empty--;

                    int temp = maxSearch(level, numeric_limits<int>::min(), numeric_limits<int>::max()); //Type Casting done to avoid problems with limits on int.

                    if (temp < score)
                    {
                        score = temp;
                        move.x = i;
                        move.y = j;
                    }

                    board[i][j] = Player::blank;
                    empty++;
                }
            }
        }

        return move;
    }

    int maxSearch(int level, int alpha, int beta) //Alpha Beta Pruning to help with the Tree implementation.
    {
        if (checkWin(Player::person)) { return 10; }

        else if (checkWin(Player::algorithm)) { return -10; }
        
        else if (isTie()) { return 0; }

        int score = numeric_limits<int>::min();

        for (int i = 0; i < coordinate; i++)
        {
            for (int j = 0; j < coordinate; j++)
            {
                if (board[i][j] == Player::blank)
                {
                    board[i][j] = Player::person;
                    empty--; //Checks through the remaining blocks

                    score = max(score, minSearch(level + 1, alpha, beta) - level);
                    alpha = max(alpha, score);

                    board[i][j] = Player::blank;
                    empty++;

                    if (beta <= alpha) { return alpha; }
                }
            }
        }

        return score;
    }

    int minSearch(int level, int alpha, int beta)
    {
        if (checkWin(Player::person)) { return 10; }

        else if (checkWin(Player::algorithm)) { return -10; }
        
        else if (isTie()) { return 0; }

        int score = numeric_limits<int>::max();

        for (int i = 0; i < coordinate; i++)
        {
            for (int j = 0; j < coordinate; j++)
            {
                if (board[i][j] == Player::blank)
                {
                    board[i][j] = Player::algorithm;
                    empty--;

                    score = min(score, maxSearch(level + 1, alpha, beta) + level);
                    beta = min(beta, score);

                    board[i][j] = Player::blank;
                    empty++;

                    if (beta <= alpha) return beta;
                }
            }
        }

        return score;
    }

    void getHumanMove()
    {
        int loc = 0;
        do
        {
            cout << "Your Move: ";
            cin >> loc;
        } while (loc < 1 || loc >9);

        if (loc == 1)
            board[0][0] = Player::person;
        else if (loc == 2)
            board[0][1] = Player::person;
        else if (loc == 3)
            board[0][2] = Player::person;
        else if (loc == 4)
            board[1][0] = Player::person;
        else if (loc == 5)
            board[1][1] = Player::person;
        else if (loc == 6)
            board[1][2] = Player::person;
        else if (loc == 7)
            board[2][0] = Player::person;
        else if (loc == 8)
            board[2][1] = Player::person;
        else if (loc == 9)
            board[2][2] = Player::person;
        else
        {
            cout << "wrong input";
            cin >> loc;
        }
    }
};

int main()
{
    Game tictactoe;
    tictactoe.play();
    system("pause");
}
