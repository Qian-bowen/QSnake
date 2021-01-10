#include "Tutorial.h"
#include "ui_tutorial.h"

namespace Ui {
class Tutorial;
}

Tutorial::Tutorial(QWidget* parent):
    QDialog(parent),
   ui(new Ui::Tutorial)
{

    ui->setupUi(this);
    setWindowTitle(tr("Tutorial"));
    this->resize(1000,1000);


}

Tutorial::~Tutorial()
{
    delete ui;
}
