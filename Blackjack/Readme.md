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
There are some slight variations on the rules and procedure of blackjack. Below is the simplified procedure that we will use for this assignment. We will not be using insurance, surrender or dealer peeking, which are options in a standard casino game.

# Problem Statement
In this assignment your task is to compute the policy for an optimal player. As usual the first step is to carefully think of the state space that you will need. Design a state transition function, and reward model to encode the dynamics of the play. Solve the game to compute the best play in each state. The best play is defined as the action (hit/stand/double/split) that maximizes the expected return. Make sure you double or split only in the states it is allowed. <br /><br />Assume that the player bets $1. Program for a BlackJack(p) game. Assume that the probability of getting a face card is p (an input to the program) and the probability of getting all other cards, 2-9 and Ace, is uniformly (1-p)/9. Note that p = 4/13 captures the standard Blackjack game.<br /><br />
After you solve the problem, the solution to BlackJack(4/13) should look very close to this. In the first column, representing your hand, a single integer represents the sum of the two cards, and indicates that they are not a pair and that neither is an ace. For the output of this assignment, you need to return only the first action that you will take. Thus your output need not distinguish between "D" and "DS".

# Output Format

# What is being provided
We provide an output checker, formatcheck.py, that tests whether your output is in the required format or not. To help students unfamiliar with shell scripting, we also provide a sample run.sh script that takes the input parameter from the command line and passes it to your own program (assuming your program is called BlackJackPlayer). Modify this file to replace “BlackJackPlayer” with the name of your executable.

# Important information
To familiarize yourself with the game, play it online for a few minutes. There are many online applets available, for example, [this](http://www.hitorstand.net/game_m.html). The rules they follow may be slight variations of those used in the assignment, but you will get the general idea.
<br />
<br />
Please supply a compile.sh script and a run.sh script. We will run your code as follows: <br />
./compile.sh<br />
./run.sh 0.307<br />
<br />
The parameter in front of the ./run.sh will be the value p that represents the probability of the face card. When we call run.sh in the above format, it needs to save the policy to a file “Policy.txt” in the present working directory.
