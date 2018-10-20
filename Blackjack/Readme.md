# Goal
The goal of this assignment is to find a solution to a real problem using ideas from sequential decision making under uncertainty (Markov Decision Processes).

# Scenario - The game of Blackjack

## Terminology

### Cards
A standard deck of playing cards is used, i.e., four suits (clubs, diamonds, spades, and hearts) and 13 different cards within each suit (the numbers 2 through 10, jack, queen, king, and ace). In this assignment, we will replace 10, jack, queen and king with a generic 'face card'. We will assume an infinite number of decks available in the pack.
### Card Values
The numbered cards (2 through 9) count as their numerical value. The generic face card (replacing 10, jack, queen, and king) counts as 10, and the ace may count as either 1 or 11 (whichever works in the player's favor).
### Hand Value
The value of a hand is the sum of the values of all cards in the hand. The values of the aces in a hand are such that they produce the highest value that is 21 or under (if possible). A hand where any ace is counted as 11 is called a soft hand. The suits of the cards do not matter in blackjack.

### Pair
The two card hand where both cards have the same value. (example, two aces, a pair of sixes, and for our assignment, a pair of face cards).

### Blackjack
It is a two-card hand where one card is an ace and the other card is any face card.

### Busted
A player is busted if the value of the hand has exceeded 21.

## Rules of play
