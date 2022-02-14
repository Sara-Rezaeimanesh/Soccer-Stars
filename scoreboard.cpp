#include "RSDL-master/src/rsdl.hpp"
#include "scoreboard.hpp"
#include <vector>
#include <sstream>
#include <iostream>
#define FIRST 0 
#define NOSCORE 0 
#define NUM_OF_TEAMS 2
#define FIRST 0 
#define SECOND 1 
#define ONESCORE 1
#define ONEGAME 1
#define INITIALIZED 1
#define FINISHED 0 
#define TXTSTRT1_Y 455
#define TXTSTRT1_X 5
#define FONTSIZE 20
#define HALF 400
#define RESULTSTRT 0 

ScoreBoard::ScoreBoard()
{
	game_status = INITIALIZED ; 
	turn = FIRST ; 
	for( int i = 0 ; i < NUM_OF_TEAMS ; i++ )
	{
		scores.push_back( NOSCORE ) ;
		gameswon.push_back( NOSCORE ) ;
	}
}

void ScoreBoard::add_info( int rounds_ , int goals_ ) 
{
	rounds = rounds_ ; 
	goals = goals_ ;
}

void ScoreBoard::change_turns( int turn ) 
{
	if( turn == SECOND )
		turn = FIRST ; 
	else 
		turn = SECOND ; 
}

void ScoreBoard::update_score( int goaled_gate  ) 
{
	int winner_idx = ( goaled_gate == SECOND ) ? FIRST : SECOND ; 
	if( scores[winner_idx] + ONESCORE != goals )
		scores[winner_idx] += ONESCORE ;
	else
	{
		scores[winner_idx] += ONESCORE ;
		scores[FIRST] = NOSCORE ; 
		scores[SECOND] = NOSCORE ; 
		if( gameswon[FIRST] + gameswon[SECOND] + ONEGAME != rounds )
			gameswon[winner_idx] += ONEGAME ; 
		else
		{
			gameswon[winner_idx] += ONEGAME ; 
			game_status = FINISHED ;
		}
	}
}

void ScoreBoard::draw( Window* window ) 
{
	for( int i = 0 ; i < NUM_OF_TEAMS ; i++ )
	{
		std::ostringstream os ;
		os << "Team No." << i + 1 << "-> Score: " << scores[i] << " Games Won: " << gameswon[i] ;
		window->show_text( os.str() , Point( TXTSTRT1_X + i * HALF , TXTSTRT1_Y )
			, WHITE , "FreeSansBold-Rdmo.otf", FONTSIZE ) ;
	}
}

void ScoreBoard::show_results( Window* window  ) 
{
	std::ostringstream os ;
	int winner = ( gameswon[FIRST] > gameswon[SECOND] ) ? FIRST  : SECOND ; 
	if( gameswon[FIRST] == gameswon[SECOND] )
		os << "GAME ENDED IN A TIE" << std::endl ;
	else
		os << "THE WINNER IS: " << "TEAM.NO." << winner + 1 << "           Games Won: " << gameswon[winner] ;
	window->show_text( os.str() , Point( RESULTSTRT , RESULTSTRT ) , WHITE , "FreeSansBold-Rdmo.otf", FONTSIZE ) ;
}

bool ScoreBoard::game_is_finished() 
{
	if( game_status == FINISHED )
		return true ; 
	return false ; 
}