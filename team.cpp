#include "RSDL-master/src/rsdl.hpp"
#include "player.hpp"
#include "team.hpp"
#include <string>
#include <iostream>
#include <vector>
#define PLAYERTYPE1ADD "player1.png"
#define PLAYERTYPE2ADD "player2.png"
#define FIRST_TEAM 1 
#define NODELTA 0 
#define NOTFOUND -1 

Team::Team( std::vector <Player> players_ , int team_no_ )
{
	players = players_ ; 
	team_no = team_no_ ;
}

int Team::find_player_num( Point curr_pos ) 
{
	for( int i = 0 ; i < players.size() ; i++ )
		if( players[i].is_selected( curr_pos ) )
			return i ; 
	return NOTFOUND ;
}

void Team::draw_initial_setup( Window *window ) 
{
	std::string player_img_add = ( team_no == FIRST_TEAM ) ? PLAYERTYPE1ADD : PLAYERTYPE2ADD ;
	for( int i = 0 ; i < players.size() ; i++ )
		players[i].draw( window , Point( NODELTA , NODELTA ) , player_img_add );

}

Player* Team::find_selected_player( int player_no ) 
{
	return &players[player_no] ;
}

int Team::find_encountered_idx( Player* player ) 
{
	for( int i = 0 ; i < players.size() ; i++ )
	{
		Point position = players[i].get_position() ;
 		if( player->encounters( position ) ) 
 			return i ; 
	}
	throw std::runtime_error( "no encounters" ) ;
}

