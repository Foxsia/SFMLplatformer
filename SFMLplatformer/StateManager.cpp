#include "StateManager.h"
#include "IState.h"
#include "MenuState.h"
#include "PlayState.h"
#include "EditorState.h"
#include "GameOverState.h"
#include "WinState.h"
#include "ScoresState.h"

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
			case StateType::Win:
				currentState = std::make_unique<WinState>();
				break;
			case StateType::Scores:
				currentState = std::make_unique<ScoresState>();
				break;
		}
	}
	IState* StateManager::getCurrentState()
	{
		return currentState.get();
	}
}
