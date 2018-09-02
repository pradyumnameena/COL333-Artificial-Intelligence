# Yinsh Game

## Goal
The goal of this assignment is to learn the adversarial search algorithms (mini- max and alpha beta pruning), which arise in sequential deterministic adversarial situations.

## The Game of Yinsh
### Setup
The game board is almost a regular hexagon of side N as shown. There are M rings with each player. Typical values for N are 5 and of M is also 5.
### Objective
There are 2 players, Black and White. The objective for both the players is to remove L rings before the other player does. To remove a ring you must create K markers in a line (explained below).
### Player Moves
The game proceeds turn by turn. The first M moves for both the players are placing the M rings each player has on the board on a position of their choice that is not already occupied. The subsequent moves are either moving a ring or removing a ring when you have at least 5 markers in a row. Moving a ring leaves a marker of the ring’s color behind.
### Moving a ring
You can move a ring from its initial position to another position on the board, but it can’t cross any other ring. If you are moving across markers, you can only move to the first empty space after the first continuous line of markers. Any number of spaces can be skipped before the first continuous line of markers. Any number of spaces can also be skipped if there are no markers on the line. In other words, you are allowed at most one (possibly long) jump across the markers. When a ring crosses a marker, the color of the marker toggles (black to white and white to black). This is applicable for all markers the ring crosses.
### Moving a ring
If you get 5 markers in a row, you can remove them by clicking on one of the end markers of the 5 in a line. If there are more than 5 in a line or two 5 lines crossing each other, clicking at the two ends will remove the 5 markers in the middle. After removing 5 continuous markers, you can remove a ring of your choice. Find more details in the link to the rules above. Catch : unlike in the rules, where you can remove any 5 continuous markers of yours choice, you can only remove 5 continuous markers from one end.<br />
Click [here](http://www.gipf.com/yinsh/rules/rules.html) for rules of the game.

## Performance Equivalence Classes
To rank your performance we will first divide each game into a set of equivalence classes:
Win > Stalemate win > Draw > Stalemate Loss > Loss
### Wins / Losses
You remove L (usually 3) rings before the other player does. The other player loses. For winning, 3-0 is better than 3-1 which is better than 3-2. The opposite is for losing.
### Stalemate Win / Stalemate Loss
When no more legitimate moves are possible, the player with more rings outside the board wins. The opponent loses the stalemate. For stalemate win, 2-0 is better than 2-1 and 1-0 (same bracket). The opposite for losing.
### Draw
When no legitimate moves are possible and both the players have the same number of rings outside the board. For draw, 2-2 is better than 1-1 which is better than 0-0.
### Final Ranking
Within each equivalence class, the performance will be ranked by the difference of the number of markers (you - opponent) on the board at the time the game ended. Each pair of players will play 2 games, each getting a chance to start first in one game. The final ranking will be decided by combining the ranking of all the games.
