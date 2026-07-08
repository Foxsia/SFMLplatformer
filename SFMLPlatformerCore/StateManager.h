#pragma once
#include <memory>
#include "StateType.h"

namespace fp
{
	class IState;
	class MenuState;
	class PlayState;
	class EditorState;
	class GameOverState;
	class WinState;

	class StateManager
	{
	public:
		void changeState(StateType type);
		IState* getCurrentState();

	private:
		std::unique_ptr<IState> currentState;
	};
}

