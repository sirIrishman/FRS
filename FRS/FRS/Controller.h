#include <qstring.h>
#include "Model.h"

namespace FRS {
    class Controller {
    public:
        Controller(Model* const& model);
        ~Controller();

        void loadImage(QString const& filePath);

    private:
        Model* _model;
    };
}