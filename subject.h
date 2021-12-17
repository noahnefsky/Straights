#ifndef SUBJECT_H
#define SUBJECT_H

#include <set>


class Observer;

class Subject {
  public:
    void subscribe( Observer* );
    void unsubscribe( Observer* );

  protected:
    void notify(bool doPrints);

  private:
    typedef std::set< Observer* > Observers;
    Observers observers_;
}; // Subject


#endif
