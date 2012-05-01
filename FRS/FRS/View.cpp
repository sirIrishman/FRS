#include "View.h"

//using namespace FRS;

View::View(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags) {
        ui.setupUi(this);

}

View::~View() {

}