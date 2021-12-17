#ifndef STRAIGHTSVIEW_H
#define STRAIGHTSVIEW_H
#include "observer.h"
#include <string>
#include <vector>
#include <memory>

class StraightsController;
class StraightsModel;

class StraightsView: public Observer {
	std::shared_ptr<StraightsModel> model_;
	std::shared_ptr<StraightsController> controller_;

	public:
	StraightsView( std::shared_ptr<StraightsController> c, std::shared_ptr<StraightsModel> m);
	virtual ~StraightsView();
	virtual void update(bool doPrints);
	void printBeforeTurn(std::vector<std::string> legalPlays);
	void handlePlayerTurn(std::vector<std::string> legalPlays);
	void printEndOfGame(std::vector<int> winnerIds);
	void handlePlay(std::string card);
	void handleDiscard(std::string card);
	void handleDeck();
	void handleRageQuit();
	void handleComputerTurn();
};

#endif
