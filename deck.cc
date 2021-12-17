#include "deck.h"
#include <algorithm>
#include <random>

unsigned Deck::getSeed() const { return seed; }

void Deck::setSeed(unsigned seedIn) { seed = seedIn; }

void Deck::shuffleDeck() {
	// The seed is set in the main.cc file
	std::shuffle( cards.begin(), cards.end(), std::default_random_engine(seed) );
}

Deck::Deck() {
	// Need to build a deck with proper cards. (not shuffled here)
	char type = '0';
	for (int i = 1; i <= 13; i++) {
		if (i == 1)
			type = 'A';
		else if (i == 11)
			type = 'J';
		else if (i == 12)
			type = 'Q';
		else if (i == 13)
			type = 'K';
		else
			type = 'n';

		auto club = std::make_shared<Card>('C', type, i);
		this->cards.push_back(club);
	}
	for (int i = 1; i <= 13; i++) {
                if (i == 1)
                        type = 'A';
                else if (i == 11)
                        type = 'J';
                else if (i == 12)
                        type = 'Q';
                else if (i == 13)
                        type = 'K';
                else
                        type = 'n';

                auto diamond = std::make_shared<Card>('D', type, i);
		this->cards.push_back(diamond);
        }
	for (int i = 1; i <= 13; i++) {
                if (i == 1)
                        type = 'A';
                else if (i == 11)
                        type = 'J';
                else if (i == 12)
                        type = 'Q';
                else if (i == 13)
                        type = 'K';
                else
                        type = 'n';

                auto heart = std::make_shared<Card>('H', type, i);
		this->cards.push_back(heart);
        }
	for (int i = 1; i <= 13; i++) {
                if (i == 1)
                        type = 'A';
                else if (i == 11)
                        type = 'J';
                else if (i == 12)
                        type = 'Q';
                else if (i == 13)
                        type = 'K';
                else
                        type = 'n';

                auto spade = std::make_shared<Card>('S', type, i);
		this->cards.push_back(spade);
        }
}

std::vector<std::shared_ptr<Card>> Deck::getCards() const { return cards; }
