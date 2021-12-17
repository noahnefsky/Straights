#include "StraightsModel.h"
StraightsModel::StraightsModel() {
	deck = std::make_shared<Deck>();
}

std::vector<std::shared_ptr<Player>> StraightsModel::getPlayers() const { return players; }

// This function makes a Player and adds it into the model's vector of Players
void StraightsModel::addPlayer(bool isComputer, int id) {
	auto player = std::make_shared<Player>(isComputer, id);
	players.push_back(player);
}

std::shared_ptr<Deck> StraightsModel::getDeck() const { return deck; }

std::vector<std::shared_ptr<Card>> StraightsModel::getClubs() const { return clubs; }

std::vector<std::shared_ptr<Card>> StraightsModel::getDiamonds() const { return diamonds; }

std::vector<std::shared_ptr<Card>> StraightsModel::getHearts() const { return hearts; }

std::vector<std::shared_ptr<Card>> StraightsModel::getSpades() const { return spades; }

bool StraightsModel::getEasyMode() const { return easyMode; }

void StraightsModel::setEasyMode(bool newEasyMode) { easyMode = newEasyMode; }	

bool StraightsModel::getEnhance() const { return enhance; }

void StraightsModel::setEnhance(bool newEnhance) { enhance = newEnhance; }

void StraightsModel::playCard(std::string card) {
	// First the card is removed from the current player's hand
	auto removedCard = players[currentPlayerIndex]->removeCardFromHand(card);
	// If the card is being placed at the higher end of the respective suit pile,
	//    push_back is used. If it is being placed at the lower end of
	//    the respective suit pile, insert is used to put it at the front.
	if (removedCard->getSuit() == 'H') {
		if (!hearts.empty() && removedCard->getValue() < hearts[0]->getValue())
			hearts.insert(hearts.begin(), removedCard);
		else
			hearts.push_back(removedCard);
	} else if (removedCard->getSuit() == 'D') {
		if (!diamonds.empty() && removedCard->getValue() < diamonds[0]->getValue())
			diamonds.insert(diamonds.begin(), removedCard);
		else
			diamonds.push_back(removedCard);
	} else if (removedCard->getSuit() == 'C') {
		if (!clubs.empty() && removedCard->getValue() < clubs[0]->getValue())
                        clubs.insert(clubs.begin(), removedCard);
                else
                        clubs.push_back(removedCard);
	} else {
		if (!spades.empty() && removedCard->getValue() < spades[0]->getValue())
                        spades.insert(spades.begin(), removedCard);
                else {
                        spades.push_back(removedCard);
		}
	}
	// Need to change the currentPlayerIndex before notify, so store for
        //    the last check.
	int index = currentPlayerIndex;
        currentPlayerIndex++;
        if (currentPlayerIndex == 4)
                currentPlayerIndex = 0;

        if (!players[index]->getIsComputer())
                notify(true);
		
}

void StraightsModel::discard(std::string card) {
	// First the card is removed from the current player's hand.
	auto removedCard = players[currentPlayerIndex]->removeCardFromHand(card);
	// Then the card is added to the current player's discard pile.
	players[currentPlayerIndex]->addDiscard(removedCard);
	// Then the current player's score is added to.
	int newScore = players[currentPlayerIndex]->getScore() + removedCard->getValue();
	players[currentPlayerIndex]->setScore(newScore);
	// Change currentPlayerIndex and go to next turn
	int index = currentPlayerIndex;
	currentPlayerIndex++;
	if (currentPlayerIndex == 4)
                currentPlayerIndex = 0;

	if (!players[index]->getIsComputer())
                notify(true);
}

void StraightsModel::switchToComputer() {
	players[currentPlayerIndex]->setIsComputer(true);
	notify(false);
}

// This function check if the game is done and return the winner[s]. The game
//    is only done if at least 1 player has an 80+ score. If this is true a
//    vector of the player[s] with the lowest score (can be a tie) is returned.
//    If the game is not done, a vector with only 1 element (-1) is returned.
//    (Note a player can't have a negative id so -1 can indicate no winner)
//    (Also note this function is only called at the end of a round)
std::vector<int> StraightsModel::winnerAfterRound() {
	int highestScore = 0;
	int lowestScore = players[0]->getScore();
	std::vector<int> winningPlayerIndices;
	for (int i = 0; i < 4; i++) {
		if (lowestScore > players[i]->getScore()) {
			// If the player has the lowest score (not a tie)
			lowestScore = players[i]->getScore();
			winningPlayerIndices.clear();
			winningPlayerIndices.push_back(i+1);
		} else if (lowestScore == players[i]->getScore()) {
			// If the player had tied for the loest score
			winningPlayerIndices.push_back(i+1);
		}
		if (highestScore < players[i]->getScore())
			highestScore = players[i]->getScore();
	}
	if (highestScore < 80) {
		winningPlayerIndices.clear();
		winningPlayerIndices.push_back(-1);
	}
	return winningPlayerIndices;
}

// This function resets every player to be ready for a new round.
//    (Note score isn't changed each round)
void StraightsModel::resetForNewRound() {
	for (auto player : players) {
		player->clearHand();
		player->clearDiscards();
	}
	// Reshuffles deck and resets table to being empty.
	deck->shuffleDeck();
	clubs.clear();
	diamonds.clear();
	hearts.clear();
	spades.clear();
	int index = 0;
	// Finds the player with the 7 of spaded (7S). This player starts, so
	//    currentPlayerIndex is set to this player's index in players.
	for (int i = 0; i < 52; i++) {
		if (i <= 12) {
			players[0]->addCardToHand(deck->getCards()[i]);
			index = 0;
		}
		else if (i >= 13 && i <= 25) {
			index = 1;
			players[1]->addCardToHand(deck->getCards()[i]);
		}
		else if (i >= 26 && i <= 38) {
			index = 2;
			players[2]->addCardToHand(deck->getCards()[i]);
		}
		else {
			index = 3;
			players[3]->addCardToHand(deck->getCards()[i]);
		}
		if (deck->getCards()[i]->getPrintValue() == "7S")
                        currentPlayerIndex = index;
	}
}

// This function determines if the value is a legal play in its repsective
//    suit pile. This means it is adjacent to the first or last card and should
//    be on the end or at the front.
bool legalPlay(std::vector<std::shared_ptr<Card>> suitPile, int value) {
	if (!suitPile.empty() && suitPile[0]->getValue() - value == 1)
		return true;
	else if (!suitPile.empty() && value - suitPile.back()->getValue() ==  1)
		return true;
	else
		return false;
}

std::vector<std::string> StraightsModel::getLegalPlays() {
	std::vector<std::string> legalPlays;
	for (auto card : players[currentPlayerIndex]->getHand()) {
		// If you have the 7S, you must play it
		if (card->getValue() == 7 && card->getSuit() == 'S') {
			legalPlays.clear();
			legalPlays.push_back("7S");
			break;
		} else if (card->getValue() == 7) {
			legalPlays.push_back(card->getPrintValue());
		} else if (card->getSuit() == 'C' && legalPlay(clubs, card->getValue())) {
			legalPlays.push_back(card->getPrintValue());
		} else if (card->getSuit() == 'D' && legalPlay(diamonds, card->getValue())){
			legalPlays.push_back(card->getPrintValue());
		} else if (card->getSuit() == 'H' && legalPlay(hearts, card->getValue())) {
			legalPlays.push_back(card->getPrintValue());
		} else if (card->getSuit() == 'S' && legalPlay(spades, card->getValue())) {
			legalPlays.push_back(card->getPrintValue());
		}
	}
	return legalPlays;
}

// This function returns the sum of the discards from the most recent round.
int StraightsModel::addScores(int playerIndex) {
	int sum = 0;
	for (auto card : players[playerIndex]->getDiscards()) {
		sum += card->getValue();
	}
	return sum;
}
		
