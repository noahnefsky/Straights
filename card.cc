#include "card.h"
#include <string>
#include <ctype.h>
Card::Card(char suit, char type, int value) : suit{suit}, type{type}, value{value} {}

char Card::getSuit() const { return suit; }

char Card::getType() const { return type; }

int Card::getValue() const { return value; }

// Returns the the proper card string representation.
// I.e. if value/type is 2 and suit is hearts --> getPrintValue() = "2H"
std::string Card::getPrintValue() const {
	std::string cardAsString = "";
	// If it's value is between 1 and 11, then need to parse the value
	if (value < 11 && value > 1)
		cardAsString += std::to_string(value);
	// If it's a char need to add char as string to cardAsString
	else
		cardAsString += std::string(1, type);
	cardAsString += std::string(1, suit);
	return cardAsString;
}
