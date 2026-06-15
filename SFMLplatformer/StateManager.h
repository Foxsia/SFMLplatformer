#pragma once
#include <memory>
#include "IState.h"
#include "StateType.h"
#include "MenuState.h"
#include "PlayState.h"
#include "EditorState.h"
#include "GameOverState.h"

namespace fp
{
	class StateManager
	{
	public:
		void changeState(StateType type);
		IState* getCurrentState();

	private:
		std::unique_ptr<IState> currentState;
	};
}

