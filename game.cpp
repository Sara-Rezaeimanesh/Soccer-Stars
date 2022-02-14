#include "RSDL-master/src/rsdl.hpp"
#include "game.hpp"
#include <vector>
#include <iostream>
#include <cmath>
#include "team.hpp"
#include "player.hpp"
#include "field.hpp"
#include "ball.hpp"
#include "scoreboard.hpp"
#include "playermove.hpp"
#include <stdexcept>
#define INITIAL_DELTAX 0
#define INITIAL_DELTAY 0
#define PLAYERRADIUS 30
#define ALL_POSITIONS 5
#define NUM_OF_TEAMS 2 
#define NOTFOUND -1 
#define PRADIUS 30
#define BALL 1
#define PLAYER 2 
#define PLAYER_MASS 2
#define SECOND 1
#define FIRST 0
#define INITIALIZED 1
#define FINISHED 0  
#define MAX_IDX 4
#define NONE -1
#define ERROR_MSG "no players selected"
#define RCT_DST_X 800
#define RCT_DST_Y 480
#define SRC 0 

enum InitialPositions
{
	first_posx = 330 ,
	first_posy = 180 ,
	second_posy = 270 ,
	gate_keeper_posx = PRADIUS , 
	gate_keeper_posy = 225 ,
	second_posx = 165 ,
	third_posy = 145 ,
	fourth_posy = 305 								 
};

Game::Game( Field field_ , Ball ball_ , Window *window_ ) : 
			field(field_) , ball(ball_) 
{
	window = window_ ;
	turn = FIRST ; 
	status = INITIALIZED ; 
}

Team Game::make_team( int team_no , Point initial_positions[] ) 
{
	std::vector <Player> players ;
	for( int i = 0 ; i < ALL_POSITIONS ; i++ )
		players.push_back( Player( initial_positions[i] , PLAYERRADIUS , &field , team_no , PLAYER_MASS ) ) ;
	return Team( players , team_no ) ;
}

Player* Game::find_selected_player( Point curr_mpos ) 
{
	if( teams[turn].find_player_num( curr_mpos ) != NOTFOUND )
		return teams[turn].find_selected_player( teams[turn].find_player_num( curr_mpos ) ) ;
	throw std::runtime_error( ERROR_MSG ) ;
}

void Game::make_teams() 
{
	Point initial_positions[ALL_POSITIONS] = { Point(first_posx,first_posy) , Point(first_posx,second_posy) , 
											   Point(gate_keeper_posx,gate_keeper_posy) 
											   , Point(second_posx,third_posy) ,  Point(second_posx,fourth_posy) };
															
	for( int i = 0 ; i < NUM_OF_TEAMS ; i++ )
		teams.push_back( make_team( i+1 , initial_positions ) ) ;
}

void Game::place_players()
{
	for( int i = 0 ; i < NUM_OF_TEAMS ; i++ )
		teams[i].draw_initial_setup( window ) ;
}

void Game::draw_initial_setup() 
{
	field.draw( window ) ;
	ball.draw( window , Point( INITIAL_DELTAX , INITIAL_DELTAY ) ) ;
	place_players() ;
}

void Game::change_turns() 
{
	if( turn == SECOND )
		turn = FIRST ; 
	else 
		turn = SECOND ;
	score_board.change_turns( turn ) ;
}

void Game::draw_score_board() 
{
	score_board.draw( window ) ;
}
int Game::find_encountered_idx( PlayerMove* player_move ) 
{
	int encountered_idx ;
	for( int i = 0 ; i < NUM_OF_TEAMS ; i++ )
	{
		try
		{
			int encountered_idx = player_move->find_encountered_idx( teams[i] ) ;
			return encountered_idx + i * 5 ;
		}
		catch(...)
		{
			if( i == NUM_OF_TEAMS - 1 )
				throw std::runtime_error( ERROR_MSG ) ;
		}
	}
	return 0 ; 
}

bool Game::ball_is_hit( PlayerMove* player_move ) 
{
	if( ball.is_hit( player_move ) )
		return true ; 
	return false ; 
}

bool Game::is_finished() 
{
	if( status == FINISHED )
		return true ; 
	return false ; 
}
void Game::handle_contact(  int encountered_idx , int type  , PlayerMove* player_move ) 
{
	if( type == PLAYER )
	{
		Player* hit_player ;
		hit_player = ( encountered_idx > MAX_IDX ) ? teams[SECOND].find_selected_player( encountered_idx - ALL_POSITIONS ) : 
						teams[FIRST].find_selected_player( encountered_idx ) ;
		if( player_move->hit_already_added( encountered_idx ) )
			return ; 
		player_move->save_hit_player( hit_player , encountered_idx , type ) ;
	}
	if( type == BALL )
	{
		Point new_hit_speed = player_move->handle_contact( ball.get_position() , type , ball.get_mass() ) ;
		ball.save_new_speed( new_hit_speed ) ;
	} 
}

void Game::run_action( PlayerMove* player_move ) 
{
	player_move->run_move() ;
	if( player_move->get_contacted_type() == BALL )
	{
		ball.save_field( &field ) ;
		ball.run_action( player_move , &score_board ) ;
		if( score_board.game_is_finished() )
			status = FINISHED ; 
		return ; 
	}
	player_move->run_player_action() ;
	find_encounteres( player_move ) ;
	std::vector <PlayerMove*> hit_moves = player_move->get_hit_moves() ;
	for( int i = 0 ; i < hit_moves.size() ; i++ )
	{
		if( hit_moves[i]->get_contacted_type() == BALL )
		{
			ball.save_field( &field ) ;
			ball.run_action( hit_moves[i] , &score_board ) ;
			if( score_board.game_is_finished() )
				status = FINISHED ; 
			return ; 
		}
		hit_moves[i]->run_player_action() ;
		find_encounteres( hit_moves[i] ) ;
	}
}

void Game::instantiate_scoreboard( int rounds , int goals ) 
{
	score_board.add_info( rounds , goals ) ;
}

void Game::show_results( Window *window ) 
{
	window->fill_rect( Rectangle( SRC , SRC , RCT_DST_X , RCT_DST_Y ) , BLACK );
	score_board.show_results( window ) ;
	window->update_screen() ;
}

void Game::draw_setup( Window *window , Game &soccer_game , PlayerMove* player_move ) 
{
	if( soccer_game.is_finished() )
	{
		show_results( window ) ;
		return ; 
	}
	soccer_game.draw_initial_setup() ;
	if( player_move->is_pending() )
		player_move->draw_shoot_magnitude( window ) ;
	soccer_game.draw_score_board() ;
	soccer_game.place_players() ;
}

void Game::find_encounteres( PlayerMove* player_move )
{
	try
	{
		int encountered_idx = find_encountered_idx( player_move ) ;
		handle_contact( encountered_idx , PLAYER , player_move ) ;
	}
	catch(...)
	{
		if( ball_is_hit( player_move ) )
			handle_contact( NONE , BALL , player_move ) ; 
	}
}
