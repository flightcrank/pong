# PONG

This is a recreation of the classic pong video game. Implemented in the C
programming language, using SDL library to display graphics on the screen.

To compile you need to have SDL installed on your system and the header files
available to you operating systems PATH environment variable

	gcc pong.c `sdl2-config --cflags --libs` -o pong

### Update 2019

This has been ported from SDL version 1 to work with SDL version 2. Also added is the ability to run the
executable with the '-f' command line argument to run in fullscreen 
(will stretch the 640 * 480 image your systems native display resolution)

I will take any feature requests like updating the AI or movement animation if there is any demand. Or 
just clone the code and do it yourself. Enjoy!

## Controls
* space bar to start a game
* arrow keys for movement
* ESC to exit game

## Images
![title screen](http://i.imgur.com/radat.png)

![game play](http://i.imgur.com/CZhqp.png)
