#ifndef DECK_H
#define DECK_H
#include "card.h"
#include <memory>
#include <vector>

class Deck {
	std::vector<std::shared_ptr<Card>> cards;
	unsigned seed;

	public:
	Deck();
	unsigned getSeed() const;
	void setSeed(unsigned seedIn);
	std::vector<std::shared_ptr<Card>> getCards() const;
	void shuffleDeck();
};

#endif
