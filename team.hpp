#ifndef TEAM_HPP
#define TEAM_HPP "TEAM_HPP"
#include "RSDL-master/src/rsdl.hpp"
#include "player.hpp"
#include <string>
#include <vector>

class Team{

public:
	Team( std::vector <Player> players_ , int team_no_ ) ;
	void draw_initial_setup( Window *window ) ;
	void update_palyer_position( std::string player_img_add ) ;
	int find_player_num( Point curr_pos ) ;
	int find_encountered_idx( Player* player ) ;
	Player* find_selected_player( int player_no ) ;

private:
	std::vector <Player> players ;
	int team_no ;

} ;

#endif