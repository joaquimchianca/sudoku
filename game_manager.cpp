#include "game_manager.h"

using namespace Color;
using namespace std;

/*
	Função que inicializa o game loop do Sudoku, faz a leitura dos argumentos na linha de comando, definindo as options do jogo
	(número de checks, arquivo de entrada). Define a quantidade de puzzles no input.txt, cria a matriz condicional (que expressa o estado do digito)
*/
void GameManager::game_initialize( int argc, char * argv[] ){
	m_ncheck = 3;
	input_filename = "input.txt";
	if( argc > 1 ){
		if( argv[1] == "-help" or argv[1] == "--help" or argv[1] == "-h" or argv[1] == "--h" ){
			usage(argv[0]);
		}
		else if( argv[1] == "-ncheck" or argv[1] == "--ncheck" or argv[1] == "-c" or argv[1] == "--c" ){
			if( stoi(argv[2]) > 0 ){
				m_ncheck = stoi( argv[2] );
				input_filename = argv[3];
			}
			else{
				cout << "Invalid num of checks, please enter a number greater than zero.\n";
				exit(0);
			}
		}
		else{
			input_filename = argv[1];
		}
	}
	read_puzzle( input_filename );
	welcome_msg_sucess( input_filename );
	a_puzzle = r_puzzle;

	c_puzzle.resize( m_num_puzzles-1 );
	//zerando todos elementos da matriz de condição
	short k = 0;
	while ( k < m_num_puzzles ){
		for( short i{}; i < 9; i++ ){
			for( short j{}; j < 9; j++ ){
				if( r_puzzle[k].board[i][j] > 0 ){
					c_puzzle[k].board[i][j] = 0;
				}
				else{
					c_puzzle[k].board[i][j] = -1;
				}
				
			}
		}
		k++;	
	}

	game_state = State::START;
	m_curr_puzzle = 0;
	cout << "Press enter to continue.\n";
	auto temp = cin.get();
}


/*	Funçao que imprime a mensagem de boas vindas, caso a leitura ocorra com êxito, antes de imprimir o tabuleiro */
void GameManager::welcome_msg_sucess( std::string input ){
	cout << tcolor( ">>> Opening input file [../" + input + "].\n", 32);
	cout << tcolor( ">>> Processing data, please wait.\n", 32);
	cout << tcolor( ">>> Finished reading input data file.\n", 32);
	cout << endl;
	cout << "======================================================================" << endl;
	cout << "\t Welcome to a terminal version of Sudoku, v1.0" << endl;
	cout << "\t Copyright (C) 2021, Joaquim Chianca" << endl;
	cout << "======================================================================" << endl;
	cout << endl;
}

/*	Funçao que imprime a mensagem de boas vindas, caso a leitura falhe, antes de imprimir o tabuleiro */
void GameManager::welcome_msg_failure( std::string input ){
	cout << tcolor( ">>> Opening input file [../" + input + "].\n", 32);
	cout << tcolor( ">>> Processing data, please wait.\n", 32);
	cout << tcolor( ">>> Failure in read [../" + input + "], try again.\n", 31);
	exit(0);
}

/* Funcao que le o arquivo de entrada e armazena num vector de matrizes que servirá como "gabarito" */
void GameManager::read_puzzle( std::string input ){
	ifstream file( input );
	r_puzzle.clear();
	m_num_puzzles = 0;
	short row{};
	if(!file) { welcome_msg_failure( input ); }

	string line;
	while( getline( file >> ws, line ) ){
		stringstream iss(line);
		for( short col{}; col < 9; col++ ){
			iss >> puzz.board[row][col];
		}
		row++;
		if( row == 9 ){
			r_puzzle.push_back(puzz);
			row = 0;
			m_num_puzzles++;
		}
	}
	file.close();
}

// Função que imprime a mensagem de uso do aplicativo ao ser solicitado ajuda na linha de comando.
void GameManager::usage( string app_name ){
	cout <<  "Usage: $ " << app_name << " [<options>] [<input_puzzle_file>]\n";
	cout << "	Game Options: " << endl;
	cout << "		--ncheck	<num> Number of checks per game. Default = 3." <<endl;
	cout << "		--help			  Print this help text." << endl;
}

/*Função que imprime o tabuleiro com sua devida formatação*/
void GameManager::print_board( short index, short pick_col=0, short pick_row=0 ){
	std::string row_mark{"                     "};
	std::string     cols{"1 2 3   4 5 6   7 8 9"};
	if( pick_col != 0){
		auto n = cols.find( '0'+ pick_col );
		row_mark[n] = 'v';
	}
	

	cout << "       " << tcolor(row_mark, 31) << endl;
	cout << "       " << tcolor(cols , 94);
	cout << endl;
	cout << "     +-------+-------+-------+\n";
	for( short r{}; r < 9; r++ ){
		if ((r == 3) or (r == 6))
		{
			std::cout << "     |-------+-------+-------|\n";
		}
		if( pick_row == r+1 and pick_row != 0 ){
			std::cout << tcolor(" > ", 31) << tcolor( to_string( r + 1 ), 94 ) << " | ";
		}
		else{
			std::cout << "   " << tcolor( to_string( r + 1 ), 94 ) << " | ";
		}
		for ( short c{ 0 }; c < 9; c++ ) {
			if ( a_puzzle[index].board[r][c] <= 0 ) {
				std::cout << ' ';
			}
			else
			{
				if( c_puzzle[index].board[r][c] == 1 ){
					//se a jogada for valida eu printo em azul
					cout << tcolor( to_string( a_puzzle[index].board[r][c] ), 36);
				}
				else if( c_puzzle[index].board[r][c] == 2 ){
					//se a jogada nao for valida, printo em vermelho
					cout << tcolor( to_string( a_puzzle[index].board[r][c] ), 31);
				}
				else if( c_puzzle[index].board[r][c] == 3 ){
					//jogada correta, printo verde
					cout << tcolor( to_string( a_puzzle[index].board[r][c] ), 32);
				}
				else if( c_puzzle[index].board[r][c] == 4 ){
					//jogada incorreta, printa em vermelho
					cout << tcolor( to_string( a_puzzle[index].board[r][c] ), 31);
				}
				else{
					std::cout << a_puzzle[index].board[r][c];
				}
				
			}

			if ((c == 2) or (c == 5)) {
				std::cout << " | ";
			}
			else if (c == 8) {
				std::cout << " | \n";
			}
			else
				std::cout << ' ';
		}
	}
	std::cout << "     +-------+-------+-------+\n";
	cout << endl;
}

//Função que imprime a Main Screen do jogo.
//@param
//	string interface_msg - Mensagem que printa na tela, fazendo a comunicação jogo-jogador.
void GameManager::print_mainscreen( string interface_msg = " " ){
	cout << endl;
	cout << tcolor("|--------[ MAIN SCREEN ]--------|", 36) << endl;
	print_board( m_curr_puzzle );
	cout << tcolor("MSG:  [" + interface_msg + "]\n", 33) << endl;
	cout << "1-Play	 2-New Game  3-Quit  4-Help" << endl;
	cout << "Select option [1,4] > ";
}

//Função que imprime a tela de ajuda da Main Screen.
void GameManager::print_help_ms(){
	cout << tcolor("\n------------------------------------------------------------------------------", 92) << endl;
	cout << tcolor("  The goal of Sudoku is to fill a 9x9 grid with numbers so that each row,", 92) << endl;
	cout << tcolor("  column and 3x3 section (nonet) contain all of the digits between 1 and 9.\n", 92) << endl;
	cout << tcolor("  The Sudoku rules are:", 92) << endl;
	cout << tcolor("  1. Each row, column and nonet can contain each number (typicallly 1 to 9)", 92) << endl;
	cout << tcolor("     exactly once.", 92) << endl;
	cout << tcolor("  2. The sum of all numbers in any nonet, row or column must be equal to 45.", 92) << endl;
	cout << tcolor("------------------------------------------------------------------------------\n", 92) << endl;
	cout << tcolor("Press enter to go back.", 92) << endl;
}

/*
	Função que recebe uma string de jogada do usuario e faz a alteração no puzzle de acesso do usuário (a_puzzle)
	de acordo com o comando dado.

	P - Posiciona um número numa linha e coluna específica.
	R - Remove o número de uma coordenada
	C - Verifica se as jogadas feitas pelo usuário estão de acordo com o tabuleiro gabarito
	U - Desfaz a última jogada feita pelo jogador.
@param
	string command - string lida no terminal, jogada do usuário
*/
void GameManager::user_play( string command ){

	auto act = command[0];
	int row{};
	int col{};
	if( act == 'r' or act == 'R' or act == 'p' or act == 'P' ){
		row = command[2] - '0';
		col = command[4] - '0';
	}

	//usuario escolheu Play
	if( act == 'p' or act == 'P' ){
		if( check_ac_input( command ) )
			game_command.push( command );

		if( not ( c_puzzle[m_curr_puzzle].board[row-1][col-1] == 0 ) ){
			//se o digito nao for fixo...
			a_puzzle[m_curr_puzzle].board[row-1][col-1] = command[6] - '0';
			check_play( command );
		}
	}
	//usuario escolheu R
	else if( act == 'r' or act == 'R' ){
		if( check_ac_input( command ) )
			game_command.push( command );

		if( a_puzzle[m_curr_puzzle].board[row-1][col-1] > 0 )
			a_puzzle[m_curr_puzzle].board[row-1][col-1] *= -1;
		game_command.push( command );
	}

	else if( act == 'c' or act == 'C' ){
		if( m_ncheck > 0 )
			checker( );
		m_ncheck--;
	}

	else if( act == 'u' or act == 'U'){
		undo();
	}
}

/*
	Função que printa a Action Mode.
@param
	string interface_msg - mensagem de interface, que faz a comunicação entre o jogo e jogador.
	string command - jogada do usuário.
*/
void GameManager::print_action_mode( string interface_msg = " ", string command = " " ){
	auto act = command[0];
	int row{};
	int col{};
	if( act == 'r' or act == 'R' or act == 'P' or act == 'p' ){
		row = command[2] - '0';
		col = command[4] - '0';
	}

	cout << endl;
	cout << tcolor("|--------[ ACTION MODE ]--------|", 36) << endl;
	print_board( m_curr_puzzle, col, row );

	short m_digits[] {9,9,9,9,9,9,9,9,9};
	for(short i{}; i < 9; i++){
		for(short j{}; j < 9; j++ ){
			if( a_puzzle[m_curr_puzzle].board[i][j] > 0 ){
				auto aux = a_puzzle[m_curr_puzzle].board[i][j];
				m_digits[aux-1]--;
			}
		}
	}



	cout << tcolor("Checks Left: [ " + to_string(m_ncheck) + " ]", 33) << endl;
	cout << tcolor("Digits left: [ ", 33);
	for( short i{}; i < 9; i++ ){
		if( m_digits[i] > 0 ){
			cout << tcolor( to_string( i+1 ), 33 ) << " ";
		}
		else{
			cout << tcolor( " ", 33 ) << " ";
		}
	}
	cout << tcolor("]", 33) << endl;
	cout << tcolor("MSG:  [" + interface_msg + "]\n", 33) << endl;

	std::cout << tcolor("Commands syntax:", 32) << endl;
	std::cout << tcolor("  'enter' (without typing anything)  -> go back to previous menu.\n", 32);
	std::cout << tcolor("  'p' <row> <col> <number> + 'enter' -> place <number> on board at location (<row>, <col>).\n", 32 );
	std::cout << tcolor("  'r' <row> <col> + 'enter'          -> remove number on board at location (<row>, <col>).\n", 32);
	std::cout << tcolor("  'c' + 'enter'                      -> check which moves made are correct.\n", 32);
	std::cout << tcolor("  'u' + 'enter'                      -> undo last play.\n", 32);
	std::cout << tcolor("  <row>, <col>, <number> must be in range [1,9].\n\n", 32);
	std::cout << tcolor("Enter command > ", 33);

}
/*
	Função que determina se a entrada do usuário no Action Mode é válida, retornando true caso seja válida e, caso inválida,
	retorna false.

@param
	string command - entrada do usuário no Action Mode.

@return
	true - caso a entrada siga todas as condições estabelecidas
	false - caso haja algum erro na entrada do usuário
*/
bool GameManager::check_ac_input( string command ){
	

	if( ( command[0] == 'p' or command[0] == 'P') and ( command.size() == 7 or command.size() == 8 ) ){ //se for play e tiver todos os caracteres...
		if( (command[2] - '0') < 9 or (command[2] - '0') > 1 ){ //checa se os valores de entrada: linha, coluna e numero estao no range [1;9]
			if( (command[4] - '0') < 9 or (command[4] - '0') > 1 ){
				if( (command[6] - '0') < 9 or (command[6] - '0') > 1 ){
					if( not ( c_puzzle[m_curr_puzzle].board[(command[2]-'0') - 1][(command[4]-'0')-1] == 0 ) ){
						return true;	
					}
				}
			}
		}
	}

	else if( ( command[0] == 'r' or command[0] == 'R' ) and ( command.size() == 5 or command.size() == 6 ) ){
		if( (command[2] - '0') < 9 or (command[2] - '0') > 1 ){
			if( (command[4] - '0') < 9 or (command[4] - '0') > 1 ){
				if( not ( c_puzzle[m_curr_puzzle].board[(command[2]-'0') - 1][(command[4]-'0')-1] == 0 ) ){
					return true;
				}
			}
		}
	}

	else if( ( command[0] == 'c' or command[0] == 'C' ) and command.size() == 1 ){
		return true;
	}

	else if( ( command[0] == 'U' or command[0] == 'u' ) and command.size() == 1 ){
		return true;
	}

	else if( command.size() == 0 ){
		return true;
	}

	return false;
}
/*
	Função que recebe a jogada do usuário (quando solicitado o comando de Place) e retorna true caso a jogada seja válida,
	ou retorna false, caso a jogada seja inválida - faz as devida alsterações na matriz condicional (c_puzzle).
	Jogada é válida de acordo com as regras do Sudoku (veja comando 4-Help na Main Screen para mais informações).
@param
	string command - comando de posicionamento de um número

@return
	true - jogada válida
	false - jogada inválida
*/
bool GameManager::check_play( string command ){
	int row = command[2] - '0';
	int col = command[4] - '0';
	int num = command[6] - '0';
	int s_row;
	int s_col;
	short count{};

	for( short i{}; i < 9; i++ ){
		//checa na linha toda
		if( num == a_puzzle[m_curr_puzzle].board[row-1][i]){
			//se o numero ja estiver no board, jogada invalida..
			count++;
			
		}
	}

	for( short i{}; i < 9; i++ ){
		//checa na coluna toda
		if( a_puzzle[m_curr_puzzle].board[i][col-1] == num ){
			//se o numero ja estiver no board, jogada invalida..
			count++;
		}
	}

	//checa onde foi a jogada para guardar a linha inicial do nonet
	if( row >= 1 and row <= 3 ){
		s_row = 0;
	}
	else if( row >= 4 and row <= 6 ){
		s_row = 3;
	}
	else if( row >= 7 and row <= 9 ){
		s_row = 6;
	}

	//checa onde foi a jogada para guardar a coluna inicial do nonet
	if( col >= 1 and col <= 3 ){
		s_col = 0;
	}
	else if( col >= 4 and col <= 6 ){
		s_col = 3;
	}
	else if( col >= 7 and col <= 9 ){
		s_col = 6;
	}

	//verificação de nonet
	for( short i{}; i < 3; i++ ){
		for( short j{}; j < 3; j++ ){
			if( a_puzzle[m_curr_puzzle].board[i + s_row][j + s_col ] == num ){
				count++;
			}
		}
	}

	if( c_puzzle[m_curr_puzzle].board[row-1][col-1] != 0 ){
		if( count == 2 ){
			c_puzzle[m_curr_puzzle].board[row-1][col-1] = 1;
			return true;
		}
		else{
			c_puzzle[m_curr_puzzle].board[row-1][col-1] = 2;
		}
	}

	return false;
}

/*
	Função que checa se o tabuleiro de ação (a_puzzle) está de acordo com o tabuleiro-gabarito (r_puzzle),
	muda o estado do dígito na matriz condicional.
*/
void GameManager::checker(){

	for( short i{}; i < 9; i++ ){
		for( short j{}; j < 9; j++ ){
			if( abs( a_puzzle[m_curr_puzzle].board[i][j] ) != abs( r_puzzle[m_curr_puzzle].board[i][j] ) ){
				//se for o digito do action puzzle for diferente do "gabarito"
				c_puzzle[m_curr_puzzle].board[i][j] = 4;
			}
			else{
				//se for igual
				if( c_puzzle[m_curr_puzzle].board[i][j] == 1 or c_puzzle[m_curr_puzzle].board[i][j] == 2 ){
					c_puzzle[m_curr_puzzle].board[i][j] = 3; 
				}
			}
		}
	}
}

/*
	Função que checa se o tabuleiro entrou nas condições de fim de jogo, ou seja, todos os espaços em branco foram preenchidos.
	Indica se o usuário ganhou ou perdeu o jogo.

@return
	0 - caso ainda existam espaços em branco no tabuleiro de ação (a_puzzle).
	1 - caso todos os espaços estejam preenchidos e exista alguma jogada incorreta (usuário perdeu o jogo).
	2 - caso todos os espaços estejam preenchidos e todas as jogadas são corretas (usuário ganhou o jogo).
*/
short GameManager::cond_end(){
	//checagem para saber se todos os espaçoes em branco do tabuleiro foram preenchidos
	for( short i{}; i < 9; i++ ){
		for( short j{}; j < 9; j++){
			if( a_puzzle[m_curr_puzzle].board[i][j] <= 0 ){
				return 0;
			}
		}
	}
	//faz a checagem no tabuleiro
	checker();

	for( short i{}; i < 9; i++ ){
		for( short j{}; j < 9; j++){
			if( c_puzzle[m_curr_puzzle].board[i][j] == 4 or c_puzzle[m_curr_puzzle].board[i][j] == 2 ){
				//se nao for digito fixo ou a jogada nao for correta, o jogo 
				return 1;
			}
		}
	}
		//se entrar nos dois laços de verificação e sair sem cumprir a condição
		//significa que o tabuleiro esta todo preenchido e só contem digitos fixos
		//e digitos corretos.

		return 2;
}
/*
	Função que acessa o stack de jogadas feitas (std::stack< string > game_command) e desfaz a última jogada realizada
	pelo usuário.
*/
void GameManager::undo(){
	auto x = game_command.top();
	game_command.pop();

	int row = x[2] - '0';
	int col = x[4] - '0';

	if( x[0] == 'r' or x[0] == 'R' ){
		a_puzzle[m_curr_puzzle].board[row-1][col-1] *= -1;
	}

	else if( x[0] == 'p' or x[0] == 'P' ){

		if( c_puzzle[m_curr_puzzle].board[row-1][col-1] > 0 and ( not( game_command.empty() ) ) ){
			auto y = game_command.top();
			if( y[2] == x[2] and y[4] == x[4] ){
				a_puzzle[m_curr_puzzle].board[row-1][col-1] = y[6] - '0';
				check_play( y );
			}
			else{
				a_puzzle[m_curr_puzzle].board[row-1][col-1] *= -1;
			}
		}
		else{
			a_puzzle[m_curr_puzzle].board[row-1][col-1] *= -1;
		}
	}
}

//=== GAME LOOP ===//
void GameManager::process(){
	if( game_state == State::START){}
	
	else if( game_state == State::MS_READ_INPUT or game_state == State::MS_INVALID_INPUT ){
		cin >> ms_input;
		if( ms_input == 2 ){
			m_curr_puzzle++;
			while( not game_command.empty() ){
				game_command.pop();
			}
			if( m_curr_puzzle > m_num_puzzles-1 ){
				m_curr_puzzle = 0;
			}
			ac_input = " ";
		}
		cin.ignore(32767,'\n');
	}

	else if( game_state == State::MS_QUIT ){
		cin >> quit_option;
		if( quit_option == "Y" or quit_option == "y" ){
			quit_game();
		}
	}

	else if( game_state == State::MS_HELP ){
		string line;
		getline(std::cin, line);
	}


	else if( game_state == State::AC_READ_INPUT or game_state == State::AC_INVALID_INPUT ){
		string buff;
		getline( std::cin, buff );
		ac_input = buff;

		if( ac_input[0] == 'c' or ac_input[0] == 'C' ){
			checker();
		}
		user_play( ac_input );
	}
}

void GameManager::update(){
	if( game_state == State::START){
		game_state = State::MS_READ_INPUT;
	}

	else if( game_state == State::MS_READ_INPUT ){
		if( ms_input > 4 or ms_input < 1 ){
			game_state = State::MS_INVALID_INPUT;
		}
		else if( ms_input == 1 ){
			game_state = State::AC_READ_INPUT;
		}
		else if( ms_input == 4 ){
			game_state = State::MS_HELP;		
		}
		else if( ms_input == 3 ){
			game_state = State::MS_QUIT;
		}
	}

	else if( game_state == State::MS_INVALID_INPUT ){
		
		if( ms_input == 1 ){
			game_state = State::AC_READ_INPUT;
		}
		else if( ms_input == 2 ){
			game_state = State::MS_READ_INPUT;
		}
		else if( ms_input == 4 ){
			game_state = State::MS_HELP;		
		}
		else if( ms_input == 3 ){
			game_state = State::MS_QUIT;
		}
	}
	
	else if( game_state == State::MS_HELP ){
		game_state = State::MS_READ_INPUT;
	}

	else if( game_state == State::MS_QUIT ){
		if( quit_option == "y" or quit_option == "Y" ){
			game_state = State::END;
		}
		else{
			game_state = State::MS_READ_INPUT;
		}
	}

	else if( game_state == State::AC_READ_INPUT ){

		if( cond_end() == 1 or cond_end() == 2 ){
			game_state = State::END_G;
		}
		//primeiro checar se a entrada é válida!
		if( not check_ac_input( ac_input ) ){
			game_state = State::AC_INVALID_INPUT;
		}
		else{
			if( ac_input.size() == 0 ){
				//se for enter, volta para MS
				game_state = State::MS_READ_INPUT;
			}
		}

	}

	else if( game_state == State::AC_INVALID_INPUT ){
		if( check_ac_input( ac_input ) ){
			game_state = State::AC_READ_INPUT;
		}
	}

	else if( game_state == State::END_G ){
		game_state = State::MS_READ_INPUT;
	}

}

void GameManager::render(){
	if( game_state == State::START){}

	else if( game_state == State::MS_READ_INPUT ){
		print_mainscreen();
	}

	else if( game_state == State::MS_INVALID_INPUT ){
		print_mainscreen( "Invalid option! Tip 4 to see help." );
	}

	else if( game_state == State::MS_HELP ){
		print_help_ms();
		auto temp = cin.get();
	}

	else if( game_state == State::MS_QUIT ){
		cout << tcolor("MSG:  [Are you sure you want to leave this game?]\n", 33, 4) << endl;
		cout << "Your choice [Y/N] > ";
	}

	else if( game_state == State::AC_READ_INPUT ){
		if( ac_input[0] == 'p' or ac_input[0] == 'P' ){
			print_action_mode(" ", ac_input );
		}
		else if( ac_input[0] == 'r' or ac_input[0] == 'R' ){
			print_action_mode(" ", ac_input );
		}
		else if( ac_input[0] == 'c' or ac_input[0] == 'C' ){
			if( m_ncheck <=0 )
				print_action_mode(" You don't have checks! ");
			else
				print_action_mode("Checking done!", ac_input );
			
		}
		else if( ac_input[0] == 'u' or ac_input[0] == 'U' ){
			if( game_command.empty() ){
				print_action_mode(" No undo commands. ");
			}
			else{
				print_action_mode();
			}

		}

		else{
			print_action_mode();
		}
	}

	else if( game_state == State::AC_INVALID_INPUT ){
		print_action_mode( "Invalid input! Please try again!" );
	}

	else if( game_state == State::END_G ){
		if( cond_end() == 2){
			print_action_mode(" Congratulations! You solved the Sudoku Puzzle. Press enter to continue. ");
			string line;
			getline(cin, line);
		}
		else{
			print_action_mode(" You lost. Press enter to continue. ");
			string line;
			getline(cin, line);
		}
	}

}



















