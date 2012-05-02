#include <qimage.h>
#include <qstring.h>
#include "Observer.h"
#include "Enums.h"

using namespace Patterns;

namespace FRS {
    class Model : public ObservableSubject {
    public:
        Model();

        void setSource(FrameSource const& frameSource);
        void setSource(FrameSource const& frameSource, QString const& filePath);
        QImage frame() const;

    private:
        FrameSource _frameSource;
        QString _filePath;
    };
}