# lit_christmas_ornament
There are a few modes, the default option (ALL OF THEM) is to have all the modes play after each-other. But if you would like to select just one that you like, there is a dropdown where you can select just one. Since these are controlled by a mcp23017, there are essentially 16 LED outputs, in the form of 2 bytes (8 bits), there are many ways to set a pattern.

The power button is the only way to select which pattern you want to repeat.
So when turned on, it will flash one of 2 leds on the garage of the house. 
Right led will indicate "Select" mode. 
Left led will indicate "Repeat" mode.
every power cycle will switch modes, so you start in select mode, run through a few patterns, find one you like, then while it is displaying that pattern, turn off the ornament. Then turn it back on and it will be in Repeat mode which would repeat the last pattern indefinitely.


ALL THE MODES (looping)

ALL - ON

ALL - Flashing

Alternating even/odd - (flashing)

Alternating even/odd - (on-off)

Binary Counter

Grey Code Counter

Knight Rider Scanner (only middle row)

Looping Around Roof

Random Flashing

right shift in 1 until all on, then left shift in 0 until all off
