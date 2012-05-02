#include <qstring.h>
#include "Model.h"

namespace FRS {
    class Controller {
    public:
        Controller(Model* const& model);

        void loadImageFromFile(QString const& filePath);
        void loadVideoFromFile(QString const& filePath);
        void loadVideoFromWebcam(int webcamIndex);

    private:
        Model* _model;
    };
}