#include "StateManager.h"

namespace fp
{
	void StateManager::changeState(StateType type)
	{
		switch (type)
		{
			case StateType::Menu:
				currentState = std::make_unique<MenuState>();
				break;
			case StateType::Play:
				currentState = std::make_unique<PlayState>();
				break;
			case StateType::Editor:
				currentState = std::make_unique<EditorState>();
				break;
			case StateType::GameOver:
				currentState = std::make_unique<GameOverState>();
				break;
		}
	}
	IState* StateManager::getCurrentState()
	{
		return currentState.get();
	}
}
