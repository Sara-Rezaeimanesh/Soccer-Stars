#ifndef MAIN_H
#define MAIN_H "MAIN_H"
#include <iostream>
#include "game.hpp"
#include "field.hpp"
#include "ball.hpp"
#include "RSDL-master/src/rsdl.hpp"
#include "playermove.hpp"
#define NOMOVE -1 
#define NOEVENT -1 
#define PENDING 1 
#define INACTION 2 
#define NONE -1
#define NOTINITIATED 3 
#define CONTACT true
#define PLAYER 2
#define BALL 1 

void handle_lclick( Window* window ,  Game &soccer_game ,
					 PlayerMove* player_move , Event* e , Point selected_player_position )
{
	if( player_move->is_in_action() )
		return ; 
	selected_player_position = e->get_mouse_position();
	try
	{	
		Player* selected_player = soccer_game.find_selected_player( selected_player_position ) ;
		soccer_game.change_turns() ;
		player_move->add_active_player( selected_player ) ;
	}
	catch(...)
	{
		player_move->update_status( NOTINITIATED ) ;
	}
}

void handle_mmotion( Window* window ,  Game &soccer_game , PlayerMove* player_move , Event* e , Point mouse_pos )
{
	if( player_move->is_pending() )
	{
		mouse_pos = e->get_mouse_position() ;
		player_move->save_speed_properties( mouse_pos ) ;
	}
}

void handle_lrelease( Window* window ,  Game &soccer_game , PlayerMove* player_move , Event* e , Point mouse_pos )
{
	if( player_move->is_initiated() )
		return ; 
	player_move->update_status( INACTION ) ;
	mouse_pos = e->get_mouse_position() ;
	player_move->save_speed_properties( mouse_pos ) ;
}

void handle_mouse_events(Window* window ,  Game &soccer_game ,
					 PlayerMove* player_move , Event e , Point selected_player_position , Point mouse_pos )
{
	e = window->poll_for_event();
	switch( e.get_type() )
	{
		case Event::EventType::QUIT:
			exit(0);
			break;
		case Event::EventType::LCLICK:		
			handle_lclick( window , soccer_game , player_move , &e , selected_player_position ) ; 
			break ;				
		case Event::EventType::MMOTION:
			handle_mmotion( window , soccer_game , player_move , &e , mouse_pos ) ;
			break ;
		case Event::EventType::LRELEASE:
			handle_lrelease( window , soccer_game , player_move , &e , mouse_pos ) ;
			break ; 
	}
}

void update( Window* window ,  Game &soccer_game , PlayerMove* player_move )
{
	Event e ;
	Point mouse_pos = Point(0,0), selected_player_position ;
	while( window->has_pending_event() )
		handle_mouse_events( window , soccer_game , player_move , e , selected_player_position , mouse_pos ) ;
	if( player_move->is_in_action() )
		soccer_game.run_action( player_move ) ;	
}

void draw( Window *window , Game &soccer_game , PlayerMove* player_move )
{
	window->clear();
	soccer_game.draw_setup( window , soccer_game , player_move ) ;
	window->update_screen() ;
}

#endif