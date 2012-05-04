#ifndef FRS_CONTROLLER_H
#define FRS_CONTROLLER_H

#include <qstring.h>
#include "Model.h"

namespace FRS {
    class Controller {
    public:
        Controller(Model* const& model);

        void loadImage(QString const& filePath);
        void loadVideo(QString const& filePath);
        void captureVideo(int webcamIndex);
        void captureImage(int webcamIndex);

    private:
        Model* _model;
    };
}

#endif //FRS_CONTROLLER_H
