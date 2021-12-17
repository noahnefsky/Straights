#include "StraightsModel.h"
#include "StraightsController.h"
#include "straightsView.h"
#include <chrono>

int main( int argc, char * argv[] ) {
    auto model = std::make_shared<StraightsModel>();                           // Create model
    // if argc == 1 then there are no command line args
    if (argc == 1)
            model->getDeck()->setSeed(std::chrono::system_clock::now().time_since_epoch().count());
    else {
	    // This is what enables the bonus features
	    if (std::string(argv[1]) == "-enablebonus")
		    model->setEnhance(true);
	    // If the first if doesn't hit then the seed is the second argyment
	    else if (argc == 2)
		    model->getDeck()->setSeed(std::stoi(argv[1]));
	    if (argc == 3) {
		    model->getDeck()->setSeed(std::stoi(argv[2]));
	    }
    }
    auto controller = std::make_shared<StraightsController>(model);        // Create controller
    StraightsView   view( controller, model ); // Create the view -- is passed handle to controller and model.
                                            // The view's constructor will start the UI
} // main
