#include "PlayerController.h"

void PlayerController::UpdateMonsterObject()
{
	UpdatePlayerPosition();
}

void PlayerController::UpdatePlayerPosition()
{
	//only for keyboard support atm
	char control_input = GetControllerInput();
	MonsterPoint2D movement = ParseControllerInput(control_input);
	TranslatePosition(movement);

	
}

char PlayerController::GetControllerInput()
{
	char in;
	while (true) {

		printf("%s", "\nMove in the dungeon with WASD [q for quit]: ");
		in = _getch();
		in = toupper(in);
		if (in != 'W' && in != 'A' && in != 'S' && in != 'D' && in != 'Q')
		{
			DEBUGLOG("Invalid Controller Input: %c\n", in);
			continue;
		}
		else {
			break;
		}
	}
	
	return in;
}

MonsterPoint2D PlayerController::ParseControllerInput(char in)
{
	float x_move = 0.0f;
	float y_move = 0.0f;

	if (in == 'W')
		y_move++;
	else if (in == 'A')
		x_move--;
	else if (in == 'S')
		y_move--;
	else if (in == 'D')
		x_move++;
	else if (in == 'Q')
		SetGameOverState(true);

	return MonsterPoint2D(x_move, y_move);
}

void PlayerController::TranslatePosition(MonsterPoint2D & translation)
{
	MonsterTransform mt = monster_object_->GetTransform();
	mt.Translate(translation.x(), translation.y());
	monster_object_->SetTransform(mt);
}

PlayerController::PlayerController()
{
	monster_object_ = new MonsterObject();
}


PlayerController::~PlayerController()
{
	if (monster_object_)
		delete monster_object_;
	monster_object_ = NULL;
}
