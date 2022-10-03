
//Using libs SDL, glibc
#include <grrlib.h>
#include <ogc/lwp_watchdog.h> 
#include <stdlib.h>
#include <stdio.h>
#include <wiiuse/wpad.h>
#include <fat.h>
#include <asndlib.h>

// Textures
#include "lunarFont_ttf.h"
// RGBA Colors
#define GRRLIB_BLACK   0x000000FF
#define GRRLIB_MAROON  0x800000FF
#define GRRLIB_GREEN   0x008000FF
#define GRRLIB_OLIVE   0x808000FF
#define GRRLIB_NAVY    0x000080FF
#define GRRLIB_PURPLE  0x800080FF
#define GRRLIB_TEAL    0x008080FF
#define GRRLIB_GRAY    0x808080FF
#define GRRLIB_SILVER  0xC0C0C0FF
#define GRRLIB_RED     0xFF0000FF
#define GRRLIB_LIME    0x00FF00FF
#define GRRLIB_YELLOW  0xFFFF00FF
#define GRRLIB_BLUE    0x0000FFFF
#define GRRLIB_FUCHSIA 0xFF00FFFF
#define GRRLIB_AQUA    0x00FFFFFF
#define GRRLIB_WHITE   0xFFFFFFFF

typedef struct paddle 
{
	int x,y;
	int w,h;
} paddle_t;

typedef struct ball_s 
{
	int x, y; /* position on the screen */
	int w,h; // ball width and height
	int dx, dy; /* movement vector */
} ball_t;

static ball_t ball;

static paddle_t paddle[2];
int score[] = {0,0};
const int screenW = 640;
const int screenH = 480;
const int paddleSpeed = 7;


static void draw_ball() 
{
	GRRLIB_Rectangle(ball.x, ball.y, ball.w, ball.h, GRRLIB_WHITE, 1);
}
static void draw_paddle() 
{
	
	for (int i = 0; i < 2; i++) 
	{
		GRRLIB_Rectangle(paddle[i].x, paddle[i].y, paddle[i].w, paddle[i].h, GRRLIB_WHITE, 1);
	}
}


static void init_game() {
	
	ball.x = screenW / 2;
	ball.y = screenH / 2;
	ball.w = 10;
	ball.h = 10;
	ball.dy = 0;
	ball.dx = 3;
	
	paddle[0].x = 20;
	paddle[0].y = screenH / 2 - 50 ;
	paddle[0].w = 10;
	paddle[0].h = 50;

	paddle[1].x = screenW - 20 - 10;
	paddle[1].y = screenH / 2 - 50;
	paddle[1].w = 10;
	paddle[1].h = 50;
	
}
static void move_paddle(int d, int player) 
{

	// if the down arrow is pressed move paddle down
	if (d == 0) {
		
		if(paddle[player].y >= screenH - paddle[player].h) {
		
			paddle[player].y = screenH - paddle[player].h;
		
		} else { 
		
			paddle[player].y += paddleSpeed;
		}
	}
	
	// if the up arrow is pressed move paddle up
	if (d == 1) {

		if(paddle[player].y <= 0) {
		
			paddle[player].y = 0;

		} else {
		
			paddle[player].y -= paddleSpeed;
		}
	}
}
int check_collision(ball_t a, paddle_t b) {

	int left_a, left_b;
	int right_a, right_b;
	int top_a, top_b;
	int bottom_a, bottom_b;

	left_a = a.x;
	right_a = a.x + a.w;
	top_a = a.y;
	bottom_a = a.y + a.h;

	left_b = b.x;
	right_b = b.x + b.w;
	top_b = b.y;
	bottom_b = b.y + b.h;
	

	if (left_a > right_b) {
		return 0;
	}

	if (right_a < left_b) {
		return 0;
	}

	if (top_a > bottom_b) {
		return 0;
	}

	if (bottom_a < top_b) {
		return 0;
	}

	return 1;
}
static void move_ball() {
	
	/* Move the ball by its motion vector. */
	ball.x += ball.dx;
	ball.y += ball.dy;
	
	/* Turn the ball around if it hits the edge of the screen. */
	if (ball.x < 0) {
		
		score[1] += 1;
		init_game();
	}

	if (ball.x > screenW - 10) { 
		
		score[0] += 1;
		init_game();
	}

	if (ball.y < 0 || ball.y > screenH - 10) {
		
		ball.dy = -ball.dy;
	}

	//check for collision with the paddle
	int i;

	for (i = 0; i < 2; i++) {
		
		int c = check_collision(ball, paddle[i]); 

		//collision detected	
		if (c == 1) {
			
			//ball moving left
			if (ball.dx < 0) {
					
				ball.dx -= 1;

			//ball moving right
			} else {
					
				ball.dx += 1;
			}
			
			//change ball direction
			ball.dx = -ball.dx;
			
			//change ball angle based on where on the paddle it hit
			int hit_pos = (paddle[i].y + paddle[i].h) - ball.y;

			if (hit_pos >= 0 && hit_pos < 7) {
				ball.dy = 4;
			}

			if (hit_pos >= 7 && hit_pos < 14) {
				ball.dy = 3;
			}
			
			if (hit_pos >= 14 && hit_pos < 21) {
				ball.dy = 2;
			}

			if (hit_pos >= 21 && hit_pos < 28) {
				ball.dy = 1;
			}

			if (hit_pos >= 28 && hit_pos < 32) {
				ball.dy = 0;
			}

			if (hit_pos >= 32 && hit_pos < 39) {
				ball.dy = -1;
			}

			if (hit_pos >= 39 && hit_pos < 46) {
				ball.dy = -2;
			}

			if (hit_pos >= 46 && hit_pos < 53) {
				ball.dy = -3;
			}

			if (hit_pos >= 53 && hit_pos <= 60) {
				ball.dy = -4;
			}

			//ball moving right
			if (ball.dx > 0) {

				//teleport ball to avoid mutli collision glitch
				if (ball.x < 30) {
				
					ball.x = 30;
				}
				
			//ball moving left
			} else {
				
				//teleport ball to avoid mutli collision glitch
				if (ball.x > 600) {
				
					ball.x = 600;
				}
			}
		}
	}
}
static void move_paddle_ai(int paddleI) {

	int center = paddle[paddleI].y + 25;
	int screen_center = screenW / 2 - 25;
	int ball_speed = ball.dy;

	if (ball_speed < 0) {
	
		ball_speed = -ball_speed;
	}

	//ball moving right
	if (ball.dx > 0) {
		
		//return to center position
		if (center < screen_center) {
			
			paddle[paddleI].y += ball_speed;
		
		} else {
		
			paddle[paddleI].y -= ball_speed;	
		}

	//ball moving left
	} else {
	
		//ball moving down
		if (ball.dy > 0) { 
			
			if (ball.y > center) { 
				
				paddle[paddleI].y += ball_speed;

			} else { 
			
				paddle[paddleI].y -= ball_speed;
			}
		}
		
		//ball moving up
		if (ball.dy < 0) {
		
			if (ball.y < center) { 
				
				paddle[paddleI].y -= ball_speed;
			
			} else {
			
				paddle[paddleI].y += ball_speed;
			}
		}

		//ball moving stright across
		if (ball.dy == 0) {
			
			if (ball.y < center) { 
			
				paddle[paddleI].y -= paddleSpeed;

			} else {
			
				paddle[paddleI].y += paddleSpeed;
			}
		}	 		
	}
}

int main () {
	
	bool isMenu = true;
	bool isPaused = false;
	int players = 1;
	GRRLIB_Init();

	GRRLIB_ttfFont *myFont = GRRLIB_LoadTTF(lunarFont_ttf, lunarFont_ttf_size);
	WPAD_Init();
	init_game();
	
	while(1)
	{
		GRRLIB_FillScreen(GRRLIB_BLACK);  
		WPAD_ScanPads();  // Scan the Wiimotes
		if ( WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME)  break;
		if (isMenu)
		{
			GRRLIB_PrintfTTF(screenH/2, screenH/2, myFont, "Press 1 or 2 to choose players", 12, GRRLIB_RED);
			if ( WPAD_ButtonsDown(0)  & WPAD_BUTTON_1)
			{
				players = 1;
				init_game();
				isMenu = false;
				
			}
			if ( WPAD_ButtonsDown(0)  & WPAD_BUTTON_2)
			{
				players = 2;
				init_game();
				isMenu = false;
				
				
			}
		}
		else
		{
			if (WPAD_ButtonsDown(0) & WPAD_BUTTON_PLUS) isPaused = !isPaused;
			GRRLIB_PrintfTTF(screenW/2, screenH-30, myFont, "Press (+) to Pause ", 12, GRRLIB_RED);
			GRRLIB_PrintfTTF(screenW/2, screenH-20, myFont, "Press (-) to exit ", 12, GRRLIB_RED);
			if (!isPaused)
			{
			

				//Player 1
				if (WPAD_ButtonsHeld(0) & WPAD_BUTTON_DOWN) move_paddle(0, 1);
				if (WPAD_ButtonsHeld(0) & WPAD_BUTTON_UP) move_paddle(1, 1);
				

				if (players == 2)
				{
					//Player 2
					if (WPAD_ButtonsHeld(1) & WPAD_BUTTON_DOWN) move_paddle(0, 0);
					if (WPAD_ButtonsHeld(1) & WPAD_BUTTON_UP) move_paddle(1, 0);
				}
				else
				{
					//If theres no player the paddle is moved by the AI
					move_paddle_ai(0);
			
				}
				if (WPAD_ButtonsHeld(0) & WPAD_BUTTON_MINUS)
				{
					isMenu = true;
				}
				move_ball();
			}
			else
				{
					GRRLIB_PrintfTTF(screenW/2, screenH/2, myFont, "PAUSED", 40, GRRLIB_RED);
				}
			draw_ball();
			draw_paddle();
		}
		
		GRRLIB_Render();  
	}

	GRRLIB_Exit(); // Be a good boy, clear the memory allocated by GRRLIB

    exit(0);  // Use exit() to exit a program, do not use 'return' from main()
	
}