#include "Game.h"



Game::Game(int board_size, MonsterPoint2D &player_start_pos, int kill_freq) :
	board_bounds_(board_size),
	kill_monster_counter_(kill_freq)
{
	main_player_ = new Player("Player_One");
	main_player_->SetPosition(player_start_pos);
}

Game::~Game()
{
	//delete master_monster_list_;
	//delete main_player_;
}

void Game::Start() {
	printf("%s", "=======================================\n");
	printf("%s", "==============MONSTER CHASE============\n");
	printf("%s", "=======================================\n");

	GetAndSetUserName();
	GetAndSetNumberOfMonsters();
	InitializeMonsters();
	PlayGame();
	printf("%s", "=======================================\n");
	printf("%s", "==============EXITING GAME=============\n");
	printf("%s", "=======================================\n");
	
	_getch();
}

void Game::InitializeMonsters() {

}

void Game::GetAndSetUserName() {
	while (true) {
		printf("\nWhat is your name? [Max Length of 80]:");
		//fgets(user_name_buffer_, sizeof(user_name_buffer_), stdin);
		std::cin.getline(user_name_buffer_,sizeof(user_name_buffer_));

		if (user_name_buffer_ != 0) {
			break;
		}
		else
			printf("\nINVALID TRY AGAIN");
	}

	main_player_->SetName(user_name_buffer_);
	//printf("%s%s\n", "Accepted!\n\tWelcome ", userNameInput);
	printf("Welcome!\n\tHello %s\n", user_name_buffer_);
}

void Game::GetAndSetNumberOfMonsters() {

}

bool Game::CheckForNumberValidity(const char * input) const {
	return true;
}

void Game::PlayGame() {

}

void Game::DisplayGameState() const {

}