#include "subject.h"
#include "observer.h"

void Subject::subscribe ( Observer* newView ) {
    observers_.insert( newView );
}


void Subject::unsubscribe ( Observer* formerView ) {
    observers_.erase( formerView );
}


void Subject::notify (bool doPrints) {
    for ( auto i : observers_ ) i->update(doPrints);
}
