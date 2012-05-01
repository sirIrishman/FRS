#include <QtGui/qimage.h>
#include "Observer.h"

using namespace Patterns;

namespace FRS {
    class Model : public ObservableSubject {
    public:
        QImage frame();
    };
}