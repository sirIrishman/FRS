#include <QtGui/qimage.h>
#include "Model.h"

using namespace FRS;

QImage Model::frame() {
    return QImage("d:\\cat_smiles.jpg", "jpg");
}