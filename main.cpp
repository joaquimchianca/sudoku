#include "game_manager.h"

using namespace std;


int main(int argc, char * argv[] ){
	GameManager g;

	g.game_initialize( argc, argv );

	while( not g.quit_game() ){
		g.process();
		g.update();
		g.render();
	}
	
	return 0;
}