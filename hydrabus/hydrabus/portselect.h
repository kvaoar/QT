#ifndef PORTSELECT_H
#define PORTSELECT_H

#include <QDialog>

namespace Ui {
class PortSelect;
}

class PortSelect : public QDialog
{
    Q_OBJECT

public:
    explicit PortSelect(QWidget *parent = nullptr);
    ~PortSelect();

private:
    Ui::PortSelect *ui;
};

#endif // PORTSELECT_H
