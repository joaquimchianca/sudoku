#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <iostream>
#include "text_color.h"
#include <vector>
#include <stack>
#include <sstream>
#include <fstream>
#include <string>
#include <cstdlib>


struct board{
	int board[9][9];
};


class GameManager{
	enum State{
		START = 0,			//!< Inicio do jogo
		END,				//!< Fim do jogo
		MS_READ_INPUT,		//!< Estado que aguarda a entrada do usuário na Main Screen
		MS_INVALID_INPUT,	//!< Entrada invalida na Main Screen
		MS_HELP,			//!< Tela de ajuda da Main Screen
		MS_QUIT,			//!< Tela de Quit Main Screen
		AC_READ_INPUT,		//!< Espera entrada do usuário no Action Mode
		AC_INVALID_INPUT,	//!< Entrada invalida no Action Mode
		END_G

	};
	//=== Membros do jogo ===
	State game_state;				//!< Estado do jogo
	board puzz;						//!< Variaveis para instanciar a struct					
	short m_num_puzzles;			//!< Numero de puzzles no arquivo de entrada
	short m_ncheck;					//!< Numero de checagens durante o jogo
	std::string input_filename;		//!< Nome do arquivo de entrada lido pela linha de comando
	std::vector < board > r_puzzle;	//!< Vector que vai armazenar os tabuleiros e servir de "gabarito" para o jogo
	std::vector < board > a_puzzle;	//!< Vector que vai armazenar os puzzles que o jogador vai alterar (armazena as mudanças durante o jogo)
	std::vector< board > c_puzzle;	//!< Vector que indica a condição da jogada do usuário (válida, inválida, correta ou incorreta).
	
	/*	Legenda para a matriz condicional - que exprime o estado da jogada:
		0 - dígito fixo, parte do puzzle original, cor branca.
		1 - jogada válida, cor azul.
		2 - jogada inválida, cor vermelha.
		3 - check válido (jogada correta), cor verde. 
		4 - check inválido (jogada incorreta), cor vermelha.
	*/

	short m_curr_puzzle;				//!< Short que indica o tabuleiro usado para jogar
	std::stack< string > game_command;	//!< Stack para guardar os jogadas do usuário	
	short ms_input;						//!< Variavel que guarda a entrada da Main Screen
	string quit_option;					//!< Variavel que guarda a escolha na tela de quit
	string ac_input;					//!< String que guarda a entrada do usuario no Action Mode

public:
	//=== Game Loop Methods ===
	void game_initialize( int argc, char * argv[] );
	void process();
	void update();
	void render();
	bool quit_game(void){ return game_state == State::END; }

private:
	//=== Helper Functions ===
	void read_puzzle( std::string input );											//!< Leitura do arquivo de entrada
	void welcome_msg_sucess( std::string input );									//!< Mensagem de boas vindas caso a leitura do arquivo seja bem sucedida
	void welcome_msg_failure( std::string input );									//!< Mensagem de boas vindas caso a leitura do arquivo falhe
	void usage( string app_name );													//!< Mensagem de help antes de rodar o jogo
	void print_ms();																//!< Printa a mainscreen
	void print_board( short index, short pick_col, short pick_row );				//!< Printa o tabuleiro
	void print_mainscreen( string interface_msg );									//!< Printa Main Screen
	void print_help_ms();															//!< Printa mensagem de Help da Main Screen
	void print_action_mode( string interface_msg, string command );					//!< Printa a tela de Action Mode
	bool check_ac_input( string command );											//!< Checa se a entrada no Action Mode é válida
	bool check_play( string command );												//!< Checa se a jogada foi válida ou inválida
	void checker();																	//!< Função que checa se o tabuleiro é o espelho da entrada (se o tabuleiro foi preenchido corretamente)
	short cond_end();																//!< Função que indica a condição para o final do jogo.
	void user_play( string command );												//!< Função que processa a jogada do usuário no tabuleiro
	void undo();																	//!< Desfaz a última jogada do usuário.
};	

#endif