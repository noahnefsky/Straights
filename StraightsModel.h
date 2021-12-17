#ifndef STRAIGHTSMODEL_H
#define STRAIGHTSMODEL_H

#include "subject.h"
#include "deck.h"
#include "player.h"
#include <memory>

class StraightsModel : public Subject {
	std::vector<std::shared_ptr<Player>> players;
	std::shared_ptr<Deck> deck;
	std::vector<std::shared_ptr<Card>> clubs;
	std::vector<std::shared_ptr<Card>> diamonds;
	std::vector<std::shared_ptr<Card>> hearts;
	std::vector<std::shared_ptr<Card>> spades;
	bool easyMode = true;
	bool enhance = false;

	public:
	StraightsModel();
	int currentPlayerIndex = 0;
	std::vector<std::shared_ptr<Player>> getPlayers() const;
	void addPlayer(bool isComputer, int id);
	std::shared_ptr<Deck> getDeck() const;
	std::vector<std::shared_ptr<Card>> getClubs() const;
	std::vector<std::shared_ptr<Card>> getDiamonds() const;
	std::vector<std::shared_ptr<Card>> getHearts() const;
	std::vector<std::shared_ptr<Card>> getSpades() const;
	bool getEasyMode() const;
	void setEasyMode(bool newEasyMode);
	bool getEnhance() const;
	void setEnhance(bool newEnhance);
        void playCard(std::string card);
        void discard(std::string card);
	void switchToComputer();
	std::vector<int> winnerAfterRound();
	void resetForNewRound();
	std::vector<std::string> getLegalPlays();
	int addScores(int playerIndex); // returns how much was just added
};

#endif
