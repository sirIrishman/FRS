#include "nameDialog.h"

using namespace FRS::Native;

NameDialog::NameDialog(QWidget* const& parent, Qt::WindowFlags f) 
    : QDialog(parent, f) {
        _ui.setupUi(this);
        layout()->setSizeConstraint(QLayout::SetFixedSize);
        connect(_ui.lnEdt_Name, SIGNAL(textEdited(QString)), this, SLOT(lnEdt_Name_TextEdited(QString)));
}

QString NameDialog::name() const {
    return _ui.lnEdt_Name->text();
}

void NameDialog::lnEdt_Name_TextEdited(QString const& text) {
    _ui.bttn_Ok->setEnabled(!text.isNull() && !text.isEmpty());
}

QString NameDialog::getName(QWidget* const& parent) {
    NameDialog* nameDialog = new NameDialog(parent);
    QString name = (nameDialog->exec() == QDialog::Accepted) 
        ? QString(nameDialog->name()) 
        : QString();
    delete nameDialog;
    return name;
}
