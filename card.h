#ifndef CARD_H
#define CARD_H
#include <string>

class Card {
	char suit;
	char type;
	int value;

	public:
	Card(char suit, char type, int value);
	char getSuit() const;
	char getType() const;
	int getValue() const;
	std::string getPrintValue() const;
};

#endif
