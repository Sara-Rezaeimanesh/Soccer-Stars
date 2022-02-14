#include <iostream>
#include <math.h>
#include "main.hpp"
#include "field.hpp"
#include "game.hpp"
#include "ball.hpp"
#include "playermove.hpp"
#include "RSDL-master/src/rsdl.hpp"
#include <sstream>
#define INITIAL_SRCX 0
#define INITIAL_SRCY 0
#define INITIAL_DSTX 800
#define INITIAL_DSTY 480
#define INITIAL_FIELD_DSTY 450
#define INITIAL_POSX 390
#define INITIAL_POSY 215
#define RADIUS 10
#define BALL_MASS 1

using namespace std;

void update(Window *window , Game &soccer_game , PlayerMove* player_move );
void draw(Window *window , Game &soccer_game , PlayerMove* player_move );

int main(int argc, char const *argv[])
{
	int rounds , goals ; 
	cin >> rounds >> goals ;
	Window *window = new Window( INITIAL_DSTX , INITIAL_DSTY, "Soccer Stars" ); 
	Game soccer_game( Field( Point( INITIAL_SRCX , INITIAL_SRCY ) , Point( INITIAL_DSTX , INITIAL_FIELD_DSTY )  )
		              , Ball( Point( INITIAL_POSX , INITIAL_POSY ) , RADIUS , BALL_MASS ) , window ) ;
	soccer_game.instantiate_scoreboard( rounds , goals ) ;
	soccer_game.make_teams() ;
	while ( true )
	{
		PlayerMove* player_move = new PlayerMove() ;
		do
		{
			update( window , soccer_game , player_move ) ;
			draw( window , soccer_game , player_move ) ;
			delay(15);
		}while( !player_move->is_finished() ) ;
		delete player_move ;
	}
	return 0;
}