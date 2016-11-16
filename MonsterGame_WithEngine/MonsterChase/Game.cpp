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
	if (master_monster_list_)
		delete[] master_monster_list_;
	if (main_player_)
		delete main_player_;
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
	
	//_getch();
}

void Game::InitializeMonsters() {

	master_monster_list_ = new Monster[number_of_monsters_];

	for (int i = 0; i < number_of_monsters_; i++) {
		Monster* new_monster = new Monster(std::to_string(i + 1), board_bounds_, board_bounds_);
		if (i % 3 == 0) {
			new_monster->Zombify(main_player_);
		}
		master_monster_list_[i] = *new_monster;
	}
	
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
	int user_number;

	while (true) {
		printf("\nHow many monsters would you like to create? [100 Max]: ");

		std::cin.getline(input_buffer_, sizeof(input_buffer_));

		user_number = atoi(input_buffer_);

		if (user_number <= 0 || user_number > 100)
			printf("Invalid input, try again");
		else
			break;
	}

	number_of_monsters_ = user_number;

}

bool Game::CheckForNumberValidity(const char * input) const {
	return true;
}

void Game::PlayGame() {

}

void Game::DisplayGameState() const {

}