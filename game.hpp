#ifndef GAME_HPP
#define GAME_HPP "GAME_HPP"
#include <vector>
#include "field.hpp"
#include "team.hpp"
#include "ball.hpp"
#include "playermove.hpp"
#include "scoreboard.hpp"
#include "RSDL-master/src/rsdl.hpp"

class Game{

public:
	Game( Field field_ , Ball ball_ , Window *window_ );
	void make_teams() ;
	void run_action( PlayerMove* player_move ) ;
	void change_turns() ;
	void instantiate_scoreboard( int rounds , int goals ) ; 
	void draw_setup( Window *window , Game &soccer_game , PlayerMove* player_move ) ;
	Player* find_selected_player( Point curr_mpos ) ;
	void find_encounteres( PlayerMove* player_move ) ;

private:
	void handle_contact( int encountered_idx , int type  , PlayerMove* player_move ) ;
	void show_results( Window *window ) ; 
	void draw_initial_setup() ;
	void draw_score_board() ;
	void place_players() ;
	bool is_finished() ;
	bool ball_is_hit( PlayerMove* player_move ) ;
	int find_encountered_idx( PlayerMove* player_move ) ;
	Team make_team( int team_no , Point initial_positions[] ) ;
	ScoreBoard score_board ; 
	int turn ; 
	int status ; 
	Field field ;
	std::vector <Team> teams ; 
	Ball ball ;
	Window *window ;

} ;

#endif