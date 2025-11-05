# Using the provided code base make a Connect-4 game with an AI opponent that uses Negamax and Alpha Beta pruning

## My Implementation

### Game Logic
- **`setUpBoard()`**: creates 7x6 grid with the given square sprites, sets up 2 players and AI
- **`PieceForPlayer()`**: places red circle or yellow circle with gameTag 1 or 2
- **`actionForEmptyHolder()`**: logic for drop piece to lowest empty row in clicked column
- **`ownerAt()`**: check piece ownership at a board position

### Win Detection
- **`checkForWinner()`**: scans board for 4-in-a-row (horizontal, vertical, 2 diagonals)
- **`checkForDraw()`**: checks if top row is full (same as full board)
- **`checkWinInState()`**: win detection on state strings for AI sim

### State Management 
- Many methods were borrowed from the tic-tac-toe implementation last week
- **`stateString()`**: converts board to 42-char string ('0'=empty, '1'=red, '2'=yellow)
- **`setStateString()`**: reconverts string back to board state
- **`isBoardFull()`**: checks if state string contains any empty squares

### AI Implementation
- **`evaluateBoard()`**: heuristic scoring - wins=±1000, center column control=+3 per piece
- fun fact I used the heuristic scoring algorithm for a song ranking/sorting program in my own project last year :)
- **`negamax()`**: recursive minimax with alpha-beta pruning and depth 7
  - prefers faster wins (1000-depth) and slower losses (-1000+depth) 
- **`updateAI()`**: checks all 7 columns and selects move with highest negamax score