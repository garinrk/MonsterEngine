#include "Game.h"



Game::Game(int board_size, MonsterPoint2D &player_start_pos, int kill_freq) :
	board_bounds_(board_size),
	kill_monsters_every_(kill_freq),
	kill_monster_counter_(kill_freq)
{
	main_player_ = new Player("Player_One");
	main_player_->SetPosition(player_start_pos);
}

Game::~Game()
{
	if (master_monster_list.size() > 0)
		master_monster_list.clear();
	if (main_player_)
		delete main_player_;
}

void Game::Start() {
	printf("=======================================\n");
	printf("==============MONSTER CHASE============\n");
	printf("=======================================\n");

	GetAndSetUserName();
	GetAndSetNumberOfMonsters();
	InitializeMonsters();
	PlayGame();
	printf("=======================================\n");
	printf("==============EXITING GAME=============\n");
	printf("=======================================\n");
	printf("Press any key to exit the game\n");
	_getch();
}

void Game::AddMonster()
{
	printf("\n\n=======================================\n");
	printf("===========A MONSTER HAS APPEARED======\n");
	printf("=======================================\n\n");
	
	Monster* new_monster = new Monster(std::to_string(master_monster_list.size() + 1), board_bounds_,board_bounds_);
	master_monster_list.push_back(new_monster);
}

void Game::InitializeMonsters() {
	
	for (int i = 0; i < number_of_monsters_; i++) {
		Monster* new_monster = new Monster(std::to_string(i + 1), board_bounds_, board_bounds_);
		//if (i % 3 == 0) {
		//	new_monster->Zombify(main_player_);
		//}
		master_monster_list.push_back(new_monster);
	}
	
}

void Game::GetAndSetUserName() {
	while (true) {
		printf("\nWhat is your name? [Max Length of 80]:");
		std::cin.getline(user_name_buffer_,sizeof(user_name_buffer_));

		if (user_name_buffer_ != 0) {
			break;
		}
		else
			printf("\nINVALID TRY AGAIN");
	}

	main_player_->SetName(user_name_buffer_);
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

	size_t string_length = strlen(input);
	int user_number = atoi(input);
	if (string_length == 0 || user_number > 100 || user_number <= 0)
		return false;
	return true;
}

void Game::PlayGame() {

	printf("=======================================\n");
	printf("==============GAME START===============\n");
	printf("=======================================\n\n\n");

	while (true) {
		DisplayGameState();

		main_player_->Update();

		if ((reinterpret_cast<PlayerController*>(main_player_->GetController())->GetGameOverState()))
			return;

		/*Main Loop :
		Query user for Player movement directory or quit request.
		If quit condition has been satisfied quit the game.Otherwise :
		Update Monster positions based on their AI.
		Determine if any Monsters need to be created or destroyed and do so.
		Update Player position based on user input.
		Go to step 1.
		*/

		//kill monster randomly

		kill_monster_counter_--;

		if (kill_monster_counter_ == 0) {
			//kill_monster_counter_ = kill_monsters_every_;
			//int monster_to_kill = MMath::GetRandomIntInBounds(0, number_of_monsters_ - 1);
			//KillMonster(monster_to_kill);

			KillMonster();

		}

		//update monsters
/*
		for (std::vector<int>::iterator it = myvector.begin(); it != myvector.end(); ++it)
			std::cout << ' ' << *it;*/

		for (std::vector<Monster*>::iterator iter = master_monster_list.begin(); iter != master_monster_list.end(); ++iter) {
			(*iter)->Update();
			//vector of pointers, dereferencing the iterator gives a pointer. 
			//to get to the members, you must dereference one more
		}

		if (MonsterEngine::RandomTrueOrFalse()) {
			AddMonster();
		}
		
		time_step_++;
		
	}//main loop

}

//void Game::DisplayGameState() {const} not legal with iterator
void Game::DisplayGameState(){

	printf("\n\n=======================================\n");
	printf("==============Day Number %d=============\n", time_step_);
	printf("=======================================\n");
	printf("\n\n%s%I64u%s\n", "There are ", master_monster_list.size(), " monster(s) in the dungeon");

	//for (std::vector<Monster*>::iterator iter = master_monster_list.begin(); iter != master_monster_list.end(); ++iter) {
	//	(*iter)->Update();
	//	//vector of pointers, dereferencing the iterator gives a pointer. 
	//	//to get to the members, you must dereference one more
	//}

	//TODO: Can't use iterator in const functions?
	for (std::vector<Monster*>::iterator it = master_monster_list.begin(); it != master_monster_list.end(); ++it) {
		
		if ((*it)->is_zombie_)
		{
			printf("Zombie Monster %s is at %.1f, %.1f and is %d day(s) old\n", (*it)->GetName().c_str(), (*it)->GetPosition().x(), (*it)->GetPosition().y(), (*it)->GetAge());
		}
		else 
		{
			printf("Monster %s is at %.1f, %.1f and is %d day(s) old\n", (*it)->GetName().c_str(), (*it)->GetPosition().x(), (*it)->GetPosition().y(), (*it)->GetAge());
		}
	}

	printf("You are at (%.1f, %.1f)\n\n", main_player_->GetPosition().x(), main_player_->GetPosition().y());

}

void Game::KillMonster()
{
	printf("\n\n=======================================\n");
	printf("===========A MONSTER HAS DIED==========\n");
	printf("=======================================\n\n");

	master_monster_list.pop_back();
}
