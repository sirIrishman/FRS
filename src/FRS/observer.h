#ifndef FRS_OBSERVER_H
#define FRS_OBSERVER_H

#include <QtCore/qlist.h>

namespace patterns {
    class Observer {
    public:
        virtual ~Observer() {}
        virtual void update() = 0;

    protected:
        Observer() {}
    };

    class ObservableSubject {
    public:
        virtual ~ObservableSubject() {
            if(_observerCollection != NULL) {
                delete _observerCollection;
                _observerCollection = NULL;
            }
        }

        void attach(Observer* const& observer) {
            _observerCollection->append(observer);
        }
        void detach(Observer* const& observer) {
            _observerCollection->removeOne(observer);
        }

        void notify() const {
            for ( QList<Observer*>::const_iterator iterator = _observerCollection->constBegin(); iterator != _observerCollection->constEnd(); ++iterator) {
                Observer* observer = *iterator;
                observer->update();
            }
        }

    protected:
        ObservableSubject() {
            _observerCollection = new QList<Observer*>();
        }

    private:
        QList<Observer*>* _observerCollection;
    };
}

#endif //FRS_OBSERVER_H
