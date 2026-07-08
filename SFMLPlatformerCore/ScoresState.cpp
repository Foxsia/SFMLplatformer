#include "ScoresState.h"
#include "GameContext.h"
#include "StateManager.h"

namespace fp
{
	void ScoresState::update(float dt, GameContext& context)
	{
		if (context.input->isActionPressed("MENU_BACK"))
		{
			context.stateManager->changeState(StateType::Menu);
		}
	}
	void ScoresState::render(sf::RenderWindow& window, GameContext& context)
	{
		window.setView(window.getDefaultView());
		window.clear();

		scores = ScoreManager::loadScores();

		sf::Text title;
		title.setFont(*context.font);
		title.setString("HIGH SCORES");
		title.setCharacterSize(40);
		title.setPosition(250.f, 80.f);
		window.draw(title);

		float y = 160.f;

		for (size_t i = 0; i < scores.size() && i < 10; i++)
		{
			sf::Text line;
			line.setFont(*context.font);

			line.setString(
				std::to_string(i + 1) + ". " +
				scores[i].name + "  " +
				std::to_string(scores[i].score) + "  " +
				scores[i].time + "  " +
				scores[i].level
			);

			line.setCharacterSize(24);
			line.setPosition(150.f, y);

			window.draw(line);

			y += 35.f;
		}

		sf::Text back;
		back.setFont(*context.font);
		back.setString("PRESS M TO GO BACK");
		back.setCharacterSize(20);
		back.setPosition(250.f, 500.f);
		window.draw(back);
	}
}