#include "player.h"

Player::Player(bool isComputer, int id) : isComputer{isComputer}, id{id} {}

bool Player::getIsComputer() const { return isComputer; }

void Player::setIsComputer(bool newIsComputer) {
	isComputer = newIsComputer;
}

std::vector<std::shared_ptr<Card>> Player::getHand() const { return hand; }

std::vector<std::shared_ptr<Card>> Player::getDiscards() const { return discards; }

int Player::getId() const { return id; }

int Player::getScore() const { return score; }

void Player::setScore(int newScore) { score = newScore; }

// Inserts the given card into the player's hand
void Player::addCardToHand(std::shared_ptr<Card> card) {
	hand.push_back(card);
}

// Removes the given card from a player's hand and returns it
std::shared_ptr<Card> Player::removeCardFromHand(std::string card) {
	int index = 0;
	for (auto c : hand) {
		if (c->getPrintValue() == card)
			break;
		index++;
	}
	std::shared_ptr<Card> removedCard = hand[index];
	hand.erase(hand.begin() + index);
	return removedCard;
}

void Player::addDiscard(std::shared_ptr<Card> card) {
	this->discards.push_back(card);
}

void Player::clearHand() {
	this->hand.clear();
}

void Player::clearDiscards() {
	this->discards.clear();
}
