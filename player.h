#ifndef PLAYER_H
#define PLAYER_H
#include "card.h"
#include <vector>
#include <memory>

class Player {
	bool isComputer;
	std::vector<std::shared_ptr<Card>> hand;
	std::vector<std::shared_ptr<Card>> discards;
	int id;
	int score = 0;

	public:
	Player(bool isComputer, int id);
	bool getIsComputer() const;
	void setIsComputer(bool isComputer);
	std::vector<std::shared_ptr<Card>> getHand() const;
	std::vector<std::shared_ptr<Card>> getDiscards() const;
	int getId() const;
	int getScore() const;
	void setScore(int newScore);
	void addCardToHand(std::shared_ptr<Card> card);
	std::shared_ptr<Card> removeCardFromHand(std::string card);
	void addDiscard(std::shared_ptr<Card> card);
	void clearHand();
	void clearDiscards();
};

#endif
