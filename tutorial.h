#ifndef TUTORIAL_H
#define TUTORIAL_H

#include <QDialog>

namespace Ui{
class Tutorial;
}

class Tutorial:public QDialog
{
    Q_OBJECT
public:
    explicit Tutorial(QWidget *parent=nullptr);
    ~Tutorial();

public:
    Ui::Tutorial *ui;
};


#endif // TUTORIAL_H
