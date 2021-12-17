#include "StraightsController.h"
#include "StraightsModel.h"
#include <algorithm>
#include <map>

StraightsController::StraightsController( std::shared_ptr<StraightsModel> m) : model_{m} {} 


void StraightsController::handlePlay( std::string card) {
	model_->playCard(card);
}


void StraightsController::handleDiscard( std::string card) {
	model_->discard(card);
}


void StraightsController::handleRageQuit() {
	model_->switchToComputer();
}

// #BONUS FEATURE IN HERE#
// If the computer is on easy mode then either the first legal play is played
//    or the first card in the hand is discarded
// If the computer is on hard mode then either the legal play with the highest
//    value is played or the card with the lowest value is discarded.
// The outputStatement returned is what is outputted in
//    StraightsView::handlePlay. update is called there instead of in
//    StraightsModel play of discard. 
std::string StraightsController::handleComputerTurn() {
	int index = model_->currentPlayerIndex;
	std::string outputStatement = "";
	auto legalPlays = model_->getLegalPlays();
	auto hand = model_->getPlayers()[model_->currentPlayerIndex]->getHand();
	if (legalPlays.size() == 0 && model_->getEasyMode()) {
		auto firstCard = hand.front()->getPrintValue();
		model_->discard(firstCard);
		outputStatement = "Player" + std::to_string(index + 1) + " discards " + firstCard;
	} else if (legalPlays.size() == 0) {
		int lowest = 13;
		std::string lowestcard = "";
		for (auto card : hand) {
			int cardValue = card->getValue();
			if (cardValue <= lowest) {
				lowest = cardValue;
				lowestcard = card->getPrintValue();
			}
		}
		outputStatement = "Player" + std::to_string(index + 1) + " discards " + lowestcard;
		model_->discard(lowestcard);
	} else if (model_->getEasyMode()) { 
		model_->playCard(legalPlays[0]);
		outputStatement = "Player" + std::to_string(index + 1) + " plays " + legalPlays[0];
	} else {
		int highest = 0;
		std::string highestCard = "0C";
		for(auto card : legalPlays) {
			int cardValue = 0;
			if (card[0] == 'J')
				cardValue = 11;
			else if (card[0] == 'Q')
				cardValue = 12;
			else if (card[0] == 'K')
				cardValue = 13;
			else if (card[0] == 'A')
				cardValue = 1;
			else if (card.size() == 3)
				cardValue = std::stoi(card.substr(0, 2));
			else
				cardValue = std::stoi(card.substr(0, 1));
			if (cardValue > highest) {
				highest = cardValue;
				highestCard = card;
			}
		}
		outputStatement = "Player" + std::to_string(index + 1) + " plays " + highestCard;
		model_->playCard(highestCard);
	}
	return outputStatement;
}
