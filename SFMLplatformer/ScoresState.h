#pragma once
#include "IState.h"
#include "ScoreManager.h"

namespace fp
{
	class ScoresState : public IState
	{
	public:
		void update(float dt, GameContext& context) override;
		void render(sf::RenderWindow& window, GameContext& context) override;

	private:
		std::vector<ScoreEntry> scores;
	};
}

