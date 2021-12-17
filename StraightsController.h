#ifndef STRAIGHTSCONTROLLER_H
#define STRAIGHTSCONTROLLER_H
#include <memory>
class StraightsModel;

class StraightsController {
	std::shared_ptr<StraightsModel> model_;
	
	public:
	StraightsController( std::shared_ptr<StraightsModel> m);
	void handlePlay( std::string card );
	void handleDiscard( std::string card );
	void handleRageQuit();
	std::string handleComputerTurn();
};

#endif
