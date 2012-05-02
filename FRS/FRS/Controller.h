#include <qstring.h>
#include "Model.h"

namespace FRS {
    class Controller {
    public:
        Controller(Model* const& model);

        void loadImage(QString const& filePath);
        void loadVideo(QString const& filePath);
        void captureVideo(int webcamIndex);

    private:
        Model* _model;
    };
}