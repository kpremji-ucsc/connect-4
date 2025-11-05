#pragma once
#include "Game.h"

class Connect4 : public Game
{
public:
    Connect4();
    ~Connect4();

    void        setUpBoard() override;

    Player*     checkForWinner() override;
    bool        checkForDraw() override;
    std::string initialStateString() override;
    std::string stateString() override;
    void        setStateString(const std::string &s) override;
    bool        actionForEmptyHolder(BitHolder &holder) override;
    bool        canBitMoveFrom(Bit &bit, BitHolder &src) override;
    bool        canBitMoveFromTo(Bit &bit, BitHolder &src, BitHolder &dst) override;
    void        stopGame() override;

    void        updateAI() override;
    bool        gameHasAI() override { return true; }
    Grid*       getGrid() override { return _grid; }

private:
    Bit*        PieceForPlayer(const int playerNumber);
    Player*     ownerAt(int x, int y) const;
    int         getLowestEmptyRow(int column) const;
    int         evaluateBoard(const std::string& state, int playerColor);
    int         negamax(std::string& state, int depth, int alpha, int beta, int playerColor);
    bool        checkWinInState(const std::string& state, int playerTag);
    bool        isBoardFull(const std::string& state);

    Grid*       _grid;
};


