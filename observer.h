#ifndef OBSERVER_H
#define OBSERVER_H

class Subject;

// Update has parameter doPrints because we do not always want to print
//    print the lines before a turn, but we do still want to call update
//    sometimes. I.e. when "deck" is inputter.
class Observer {
  public:
    virtual void update (bool doPrints) = 0;
};


#endif
