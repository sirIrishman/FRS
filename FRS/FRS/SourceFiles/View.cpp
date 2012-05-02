#include "View.h"
#include "Guard.h"

using namespace FRS;

View::View(Model* model, Controller* controller, QWidget *parent, Qt::WFlags flags) : QMainWindow(parent, flags) {
    Utils::Guard::argumentNotNull(model, "model");
    Utils::Guard::argumentNotNull(controller, "controller");
    initialize(model, controller);
    _ui.setupUi(this);
    subscribeToEvents();
}

View::~View() {
    _model->detach(this);
    //delete _model;
    //delete _controller;
}

void View::initialize(Model* model, Controller* controller) {
    _controller = controller;
    _model = model;
    _model->attach(this);
}

void View::subscribeToEvents() {
    connect(_ui.pshBttn_LoadImage, SIGNAL(clicked()), this, SLOT(pshBtn_LoadImage_Clicked()));
}

void View::update() {
    _ui.lbl_Frame->setPixmap(QPixmap::fromImage(_model->frame()));
}

void View::pshBtn_LoadImage_Clicked() {
    _controller->loadImage("d:\\cat_smiles.jpg");
}