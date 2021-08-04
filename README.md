**Dodgeball! (the most fun you can have sitting in front of a UC Funkit!)**
---
Author: Jackie Chen and Scott Lilly <br>
Date started: 1st October 2020

<br>
Dodgeball is played on two UC Funkits with an attacker and a defender. The playing field consists of both funkits' combined matrix displays.

Gameplay
---

The goal of the attacker is to take out the defender as quickly as possible by launching balls at the them. <br>
The goal of the defender is to last as long as possible!

The game is split into two rounds, where each player takes turn attacking and defending.

The round ends when the defending player is hit twice.

The game ends when both players have died defending.

The matrix screens will update to display "Winner!" and "Loser..." depending on which player is determined to have won the match.

The winner is the player who lasts for the longest time during their defending round.


Setup Guide
---
1. Connect the UCFK to the PC containing the game files.
2. Open up the terminal with the directory set to the game folder.
3. Type in `make program` to build and start the game.
4. When the game is over, press down on the nav switch to restart for another game


Attacker
---
- Represented by a single pixel at the bottom of the screen.
- Move with LEFT or RIGHT - cannot move off or through the screen border.
- Fire a ball with UP.
- Initial fire rate is slow (leaves enough space for the defender to move between balls fairly easily) but as the game progresses the attacker can eventually essentially fill the screen with balls and guarantee a hit.
- Can have no more than 6 balls on the field at any one time, must wait for other balls to leave the screen before launching new ones.

Defender
---
- Two lives - start as 2x2 square, when hit you become a 1x1 square. A final hit will finish you off and the round is over!
- Move with LEFT or RIGHT - cannot move off or through the screen border.
- Survive as long as possible to accumulate as many points as possible.

Note: directional controls assumes the navswitch is facing towards the screen.
