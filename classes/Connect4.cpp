#include "Connect4.h"

Connect4::Connect4()
{
    _grid = new Grid(7, 6);
}

Connect4::~Connect4()
{
    delete _grid;
}

Bit* Connect4::PieceForPlayer(const int playerNumber)
{
    Bit *bit = new Bit();
    bit->LoadTextureFromFile(playerNumber == 0 ? "red.png" : "yellow.png");
    bit->setOwner(getPlayerAt(playerNumber));
    bit->setGameTag(playerNumber + 1);
    return bit;
}

void Connect4::setUpBoard()
{
    setNumberOfPlayers(2);
    _gameOptions.rowX = 7;
    _gameOptions.rowY = 6;

    _grid->initializeSquares(80, "square.png");

    if (gameHasAI()) {
        setAIPlayer(AI_PLAYER);
    }

    startGame();
}

bool Connect4::actionForEmptyHolder(BitHolder &holder)
{
    ChessSquare* square = static_cast<ChessSquare*>(&holder);
    int column = square->getColumn();

    int targetRow = -1;
    for (int row = 5; row >= 0; row--) {
        ChessSquare* checkSquare = _grid->getSquare(column, row);
        if (checkSquare && !checkSquare->bit()) {
            targetRow = row;
            break;
        }
    }

    if (targetRow == -1) {
        return false;
    }

    Bit *bit = PieceForPlayer(getCurrentPlayer()->playerNumber());
    if (bit) {
        ChessSquare* targetSquare = _grid->getSquare(column, targetRow);
        bit->setPosition(targetSquare->getPosition());
        targetSquare->setBit(bit);
        endTurn();
        return true;
    }

    return false;
}

bool Connect4::canBitMoveFrom(Bit &bit, BitHolder &src)
{
    return false;
}

bool Connect4::canBitMoveFromTo(Bit &bit, BitHolder &src, BitHolder &dst)
{
    return false;
}

void Connect4::stopGame()
{
    _grid->forEachSquare([](ChessSquare* square, int x, int y) {
        square->destroyBit();
    });
}

Player* Connect4::ownerAt(int x, int y) const
{
    if (x < 0 || x >= 7 || y < 0 || y >= 6) {
        return nullptr;
    }

    ChessSquare* square = _grid->getSquare(x, y);
    if (!square || !square->bit()) {
        return nullptr;
    }
    return square->bit()->getOwner();
}

Player* Connect4::checkForWinner()
{
    for (int y = 0; y < 6; y++) {
        for (int x = 0; x < 7; x++) {
            Player* player = ownerAt(x, y);
            if (!player) continue;

            if (x <= 3) {
                if (player == ownerAt(x+1, y) &&
                    player == ownerAt(x+2, y) &&
                    player == ownerAt(x+3, y)) {
                    return player;
                }
            }

            if (y <= 2) {
                if (player == ownerAt(x, y+1) &&
                    player == ownerAt(x, y+2) &&
                    player == ownerAt(x, y+3)) {
                    return player;
                }
            }

            if (x <= 3 && y <= 2) {
                if (player == ownerAt(x+1, y+1) &&
                    player == ownerAt(x+2, y+2) &&
                    player == ownerAt(x+3, y+3)) {
                    return player;
                }
            }

            if (x >= 3 && y <= 2) {
                if (player == ownerAt(x-1, y+1) &&
                    player == ownerAt(x-2, y+2) &&
                    player == ownerAt(x-3, y+3)) {
                    return player;
                }
            }
        }
    }

    return nullptr;
}

bool Connect4::checkForDraw()
{
    for (int x = 0; x < 7; x++) {
        ChessSquare* topSquare = _grid->getSquare(x, 0);
        if (topSquare && !topSquare->bit()) {
            return false;
        }
    }
    return true;
}

int Connect4::getLowestEmptyRow(int column) const
{
    for (int row = 5; row >= 0; row--) {
        ChessSquare* square = _grid->getSquare(column, row);
        if (square && !square->bit()) {
            return row;
        }
    }
    return -1;
}

std::string Connect4::initialStateString()
{
    return "000000000000000000000000000000000000000000";
}

std::string Connect4::stateString()
{
    std::string s = "";
    for (int y = 0; y < 6; y++) {
        for (int x = 0; x < 7; x++) {
            ChessSquare* square = _grid->getSquare(x, y);
            Bit* bit = square ? square->bit() : nullptr;
            if (bit) {
                s += std::to_string(bit->gameTag());
            } else {
                s += '0';
            }
        }
    }
    return s;
}

void Connect4::setStateString(const std::string &s)
{
    if (s.length() != 42) return;

    for (int y = 0; y < 6; y++) {
        for (int x = 0; x < 7; x++) {
            int index = y * 7 + x;
            ChessSquare* square = _grid->getSquare(x, y);
            if (!square) continue;

            int playerTag = s[index] - '0';
            if (playerTag == 1 || playerTag == 2) {
                Bit* bit = PieceForPlayer(playerTag - 1);
                bit->setPosition(square->getPosition());
                square->setBit(bit);
            } else {
                square->setBit(nullptr);
            }
        }
    }
}

bool Connect4::checkWinInState(const std::string& state, int playerTag)
{
    char playerChar = '0' + playerTag;

    for (int y = 0; y < 6; y++) {
        for (int x = 0; x < 7; x++) {
            int idx = y * 7 + x;
            if (state[idx] != playerChar) continue;

            if (x <= 3) {
                if (state[y*7 + x+1] == playerChar &&
                    state[y*7 + x+2] == playerChar &&
                    state[y*7 + x+3] == playerChar) {
                    return true;
                }
            }

            if (y <= 2) {
                if (state[(y+1)*7 + x] == playerChar &&
                    state[(y+2)*7 + x] == playerChar &&
                    state[(y+3)*7 + x] == playerChar) {
                    return true;
                }
            }

            if (x <= 3 && y <= 2) {
                if (state[(y+1)*7 + x+1] == playerChar &&
                    state[(y+2)*7 + x+2] == playerChar &&
                    state[(y+3)*7 + x+3] == playerChar) {
                    return true;
                }
            }

            if (x >= 3 && y <= 2) {
                if (state[(y+1)*7 + x-1] == playerChar &&
                    state[(y+2)*7 + x-2] == playerChar &&
                    state[(y+3)*7 + x-3] == playerChar) {
                    return true;
                }
            }
        }
    }

    return false;
}

bool Connect4::isBoardFull(const std::string& state)
{
    return state.find('0') == std::string::npos;
}

int Connect4::evaluateBoard(const std::string& state, int playerColor)
{
    int aiTag = (playerColor == AI_PLAYER) ? 2 : 1;
    int humanTag = (playerColor == AI_PLAYER) ? 1 : 2;

    if (checkWinInState(state, aiTag)) {
        return 1000;
    }
    if (checkWinInState(state, humanTag)) {
        return -1000;
    }

    int score = 0;

    for (int y = 0; y < 6; y++) {
        if (state[y * 7 + 3] == ('0' + aiTag)) {
            score += 3;
        }
    }

    return score;
}

int Connect4::negamax(std::string& state, int depth, int alpha, int beta, int playerColor)
{
    _gameOptions.AIDepthSearches++;

    int currentPlayerTag = (playerColor == AI_PLAYER) ? 2 : 1;
    int opponentPlayerTag = (playerColor == AI_PLAYER) ? 1 : 2;

    if (checkWinInState(state, currentPlayerTag)) {
        return 1000 - depth;
    }
    if (checkWinInState(state, opponentPlayerTag)) {
        return -1000 + depth;
    }
    if (isBoardFull(state)) {
        return 0;
    }

    if (depth >= 7) {
        return evaluateBoard(state, playerColor);
    }

    int maxScore = -10000;

    for (int col = 0; col < 7; col++) {
        int row = -1;
        for (int r = 5; r >= 0; r--) {
            if (state[r * 7 + col] == '0') {
                row = r;
                break;
            }
        }

        if (row == -1) continue;

        int index = row * 7 + col;
        char oldChar = state[index];
        state[index] = '0' + currentPlayerTag;

        int score = -negamax(state, depth + 1, -beta, -alpha, -playerColor);

        state[index] = oldChar;

        maxScore = std::max(maxScore, score);
        alpha = std::max(alpha, score);

        if (alpha >= beta) {
            break;
        }
    }

    return maxScore;
}

void Connect4::updateAI()
{
    _gameOptions.AIDepthSearches = 0;

    int bestScore = -10000;
    int bestColumn = -1;
    std::string state = stateString();

    int aiTag = 2;

    for (int col = 0; col < 7; col++) {
        int row = -1;
        for (int r = 5; r >= 0; r--) {
            if (state[r * 7 + col] == '0') {
                row = r;
                break;
            }
        }

        if (row == -1) continue;

        int index = row * 7 + col;
        state[index] = '0' + aiTag;

        int score = -negamax(state, 0, -10000, 10000, HUMAN_PLAYER);

        state[index] = '0';

        if (score > bestScore) {
            bestScore = score;
            bestColumn = col;
        }
    }

    if (bestColumn != -1) {
        ChessSquare* square = _grid->getSquare(bestColumn, 0);
        if (square) {
            actionForEmptyHolder(*square);
        }
    }
}
