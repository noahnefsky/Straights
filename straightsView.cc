#include "straightsView.h"
#include "StraightsModel.h"
#include "StraightsController.h"
#include <iostream>
#include <regex>
#include <sstream>
#include <cctype>

// The constructor for StraightsView acts as a game intro as well.
StraightsView::StraightsView(std::shared_ptr<StraightsController> c, std::shared_ptr<StraightsModel> m) : model_{m}, controller_{c}
{
	model_->subscribe(this);
	int playerCount = 1;
	bool hasAtleastOneComputer = false;
	// Players are set here (based on input of h or c). Handles invalid inputs.
	while(playerCount < 5) {
		std::string input;
		char hOrC;
		std::cout << "Is Player" << playerCount << " a human (h) or a computer (c)?" << std::endl;
		std::getline(std::cin, input);
		std::stringstream ssInput{input};
		ssInput >> hOrC;
		if (ssInput >> hOrC) {
			std::cout << "Invalid input. Try again." << std::endl;
		} else if (hOrC == 'h') {
			m->addPlayer(false, playerCount);
			playerCount++;
		} else if (hOrC == 'c') {
			hasAtleastOneComputer = true;
			m->addPlayer(true, playerCount);
                        playerCount++;
		} else {
			std::cout << "Invalid input. Try again." << std::endl;
		}
	}
	// #BONUS FEATURE IN HERE#
	// If ehancements/bonusfeature are enabled (enhance = true), the the
	//    user is prompted to put in if they want easy or hard mode.
	//    Invalid inputs are handled.
	if (model_->getEnhance() && hasAtleastOneComputer) {
		while(true) {
			std::cout << "Would you like to play on easy (type 'e') or hard (type 'h') mode?" << std::endl;
			char easyOrHard;
			std::cin >> easyOrHard;
			if (easyOrHard == 'h') {
				model_->setEasyMode(false);
				break;
			} else if (easyOrHard == 'e')
				break; //already defaulted to easy
			else {
				std::cout << "Invalid input. Type 'e' or 'h'" << std::endl;
				continue;
			}
		}
	}
	// This will be the first round upcoming so we want to print before a turn.
	update(true);
}

StraightsView::~StraightsView() {}

void StraightsView::update(bool doPrints) {
	int sumPlayerCardsInHand = 0;
	// Specifies if it is the beginnig of a round (resetForNewRound() would
	//    have been called if didReset = true;
	bool didReset = false;
	// Adds the amount of a cards all players have combined in their hand.
        for (auto player : model_->getPlayers()) {
                sumPlayerCardsInHand += player->getHand().size();
        }
	// If no players have cards then either it is the end of the round or
	//    beginning (resetForNewRound() not yet called)
	if (sumPlayerCardsInHand == 0) {
		auto winnerIds = model_->winnerAfterRound();
		// This first if checks if there is a winner
                if (!(winnerIds.size() == 1 && winnerIds[0] == -1)) {
                        printEndOfGame(winnerIds);
			doPrints = false;
		} else {
			//If no winner, we need another round
			model_->resetForNewRound();
			didReset = true;
                	// currentPlayerIndex is set in resetForNewRound when the cards are dealt
                	std::cout << "A new round begins." << std::endl; 
			doPrints = true;
                }
	}
	auto legalPlays = model_->getLegalPlays();
	if (doPrints && !model_->getPlayers()[model_->currentPlayerIndex]->getIsComputer()) {
		printBeforeTurn(legalPlays);
	}
	// Checks if it is a players turn (sumPlayerCardsInHand = 0 if we just
	//    started a new round)
	if (sumPlayerCardsInHand != 0 || didReset) {
		if (model_->getPlayers()[model_->currentPlayerIndex]->getIsComputer())
			handleComputerTurn();
		else {
			handlePlayerTurn(legalPlays);
		}
	}
}

void StraightsView::printBeforeTurn(std::vector<std::string> legalPlays) {
	std::cout << "It’s Player" << model_->currentPlayerIndex + 1 << "’s turn to play." << std::endl;
	std::cout << "Cards on the table:" << std::endl;
	std::cout << "Clubs:";
	for (auto card : model_->getClubs()) {
		if (card->getType() == 'n')
			std::cout << " " << card->getValue();
		else
			std::cout << " " << card->getType();
	}
	std::cout << std::endl;
	std::cout << "Diamonds:";
	for (auto card : model_->getDiamonds()) {
		if (card->getType() == 'n')
                        std::cout << " " << card->getValue();
                else
                        std::cout << " " << card->getType();
        }
	std::cout << std::endl;
	std::cout << "Hearts:";
	for (auto card : model_->getHearts()) {
                if (card->getType() == 'n')
                        std::cout << " " << card->getValue();
                else
                        std::cout << " " << card->getType();
        }
	std::cout << std::endl;
	std::cout << "Spades:";
	for (auto card : model_->getSpades()) {
                if (card->getType() == 'n')
                        std::cout << " " << card->getValue();
                else
                        std::cout << " " << card->getType();
        }
	std::cout << std::endl;
	std::cout << "Your hand:";
	for (auto card : model_->getPlayers()[model_->currentPlayerIndex]->getHand()) {
		std::cout << " " << card->getPrintValue();
	}
	std::cout << std::endl;
	std::cout << "Legal plays:";
	for (auto card : legalPlays) std::cout << " " << card;
	std::cout << std::endl;
}

// This function takes in the inputs of a player and responds respectively:
//    deck --> print the deck
//    quit --> terminate the program
//    ragequit --> change current human player to computer player
//    play --> play the card
//    discard --> discard the card
//    [Invalid inputs are handled]
void StraightsView::handlePlayerTurn(std::vector<std::string> legalPlays) {
	// If the player prints something invalid then they are prompted to try again
	while (true) {
		std::string input = "";
		try {
			std::getline(std::cin, input);
			if (input.empty())
				std::getline(std::cin, input);
			if (input == "deck") {
				handleDeck();
				break;
			} else if (input == "quit")
				break;
			else if (input == "ragequit") {
				std::cout << "Player" <<  model_->currentPlayerIndex + 1
					<< " ragequits. A computer will now take over." << std::endl;
				handleRageQuit();
                                break;
			} else if ((input.size() == 7 || input.size() == 8) && input.substr(0,4) == "play") {
				// Error handled in handlePlay
				std::string inputCard = input.substr(5, input.size());
				inputCard[inputCard.size() - 1] = std::toupper(inputCard[inputCard.size() - 1]);
				handlePlay(inputCard);
				break;
                        } else if ((input.size() == 10 || input.size() == 11) && input.substr(0,7) == "discard") {
				if (!legalPlays.empty())
					throw std::invalid_argument("You have a legal play. You may not discard.");
				// Not in hand error handled in handleDiscard
				std::string inputCard = input.substr(8, input.size());
				inputCard[inputCard.size() - 1] = std::toupper(inputCard[inputCard.size() - 1]);
				handleDiscard(inputCard);
				break;
			} else {
				throw std::invalid_argument("You entered an invalid command. Try again");
			}
		} catch (const std::invalid_argument& ia) {
			std::cout << ia.what() << std::endl;
			continue;
		}
	}
}

// If the card type given is lowercase is converts it to upper
//   because the card is represented with uppercase. This allows
//   the program to handle a lowercase or upper case input.
std::string convertCardToCaps(std::string card) {
	if (card[0] == 'j' || card[0] == 'q' || card[0] == 'k' || card[0] == 'a')
		card[0] = std::toupper(card[0]);
	return card;
}

// Prints what is needed at the end of the game.
void StraightsView::printEndOfGame(std::vector<int> winnerIds) {
	auto players = model_->getPlayers();

	std::cout << "Player1's discards:";
	int playerDiscardSum = model_->addScores(0);
	for (auto card : players[0]->getDiscards()) { std::cout << " " << card->getPrintValue(); }
	std::cout << std::endl;
	std::cout << "Player1's score: " << players[0]->getScore() - playerDiscardSum << " + "
		<< playerDiscardSum << " = " << players[0]->getScore() << std::endl;

	std::cout << "Player2's discards:";
	playerDiscardSum = model_->addScores(1);
	for (auto card : players[1]->getDiscards()) { std::cout << " " << card->getPrintValue(); }
	std::cout << std::endl;
	std::cout << "Player2's score: " << players[1]->getScore() - playerDiscardSum << " + "
		<< playerDiscardSum << " = " << players[1]->getScore() << std::endl;
	
	std::cout << "Player3's discards:";
	playerDiscardSum = model_->addScores(2);
	for (auto card : players[2]->getDiscards()) { std::cout << " " << card->getPrintValue(); }
	std::cout << std::endl;
	std::cout << "Player3's score: " << players[2]->getScore() - playerDiscardSum << " + "
		<< playerDiscardSum << " = " << players[2]->getScore() << std::endl;

	std::cout << "Player4's discards:";
	playerDiscardSum = model_->addScores(3);
	for (auto card : players[3]->getDiscards()) { std::cout << " " << card->getPrintValue(); }
	std::cout << std::endl;
	std::cout << "Player4's score: " << players[3]->getScore() - playerDiscardSum << " + "
		<< playerDiscardSum << " = " << players[3]->getScore() << std::endl;
	for (int id : winnerIds) std::cout << "Player" << id << " wins!" << std::endl;
}

bool cardInHand(std::string card, std::vector<std::shared_ptr<Card>> hand) {
	bool inHand = false;
	for (auto c : hand) {
		if (c->getPrintValue() == card) {
			inHand = true;
			break;
		}
	}
	return inHand;
}

void StraightsView::handlePlay(std::string card) {
	card = convertCardToCaps(card);
	auto legalPlays = model_->getLegalPlays();
	if (!(cardInHand(card, model_->getPlayers()[model_->currentPlayerIndex]->getHand())))
		 throw std::invalid_argument("You tried to play a card that is not in your hand.\nPlease try again.");
	else if (std::find(legalPlays.begin(), legalPlays.end(), card) == legalPlays.end())
                throw std::invalid_argument("This is not a legal play.");
	else {
		std::cout << "Player" << model_->currentPlayerIndex + 1 << " plays " << card << std::endl;
		controller_->handlePlay(card);
	}
}

void StraightsView::handleDiscard(std::string card) {
	card = convertCardToCaps(card);
	if (cardInHand(card, model_->getPlayers()[model_->currentPlayerIndex]->getHand())) {
		std::cout << "Player" << model_->currentPlayerIndex + 1 << " discards " << card << std::endl;
		controller_->handleDiscard(card);
	}
        else
                throw std::invalid_argument("You tried to discard a card that is not in your hand.\nPlease try again.");
}

void StraightsView::handleDeck() {
	auto deck = model_->getDeck()->getCards();
	for (int i = 0; i < 52; ++i) {
		std::cout << deck[i]->getPrintValue();
		if ((i + 1) % 13 == 0)
			std::cout << std::endl;
		else
			std::cout << " ";
	}
	// We send false because we don't want the beginning of turn print
	//    statements.
	update(false);
}
	
void StraightsView::handleRageQuit() {
	controller_->handleRageQuit();
}

void StraightsView::handleComputerTurn() {
	std::cout << controller_->handleComputerTurn() << std::endl;
	update(true);
}
