# blackjack-game-simulator
Fully functional and thorough blackjack simulator written in c++. LVS as well as LW rules are used. Player uses basic strategy.

### Requirements
1. A c++ compiler is required in order to compile the source code, preferably g++ version 4.8.4 .
2. jscharts.js should be placed in the same directory as the source code file and the executable.
3. Firefox browser in order to display simulation's output and graphs.

### Executable
In order to produce the executable file, named `blackjack` you have to type:
```bash
make
```
into source code's home directory.

### How to use
Just run:
```
./blackjack --help
```
for usage information.

### Output
After you run your simulation, several files will be generated:
* BJgame.html: Detailed output of the simulation
* overall.hmtl: Overall simulation statistics as well as a pie graph about Wins/ Defeats/ Ties and a line graph about players balance.
* trip_overall.html: Overall statistics about every simulated trip.
* shoe_overall.html: Overall statistics about every simulated shoe.
* win_lose_streak.html: Bar graph showing player's win/lose streak.
* WLdata.xml, linedata.xml: These files hold information needed for the generation of graphs.
