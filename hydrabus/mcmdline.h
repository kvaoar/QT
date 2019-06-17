#ifndef MCMDLINE_H
#define MCMDLINE_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QKeyEvent>

#define HISTORY_MAX (10000)

class MCmdLine : public QLineEdit
{
    Q_OBJECT
public:
    explicit MCmdLine(QWidget *parent = Q_NULLPTR);
    explicit MCmdLine(const QString &text, QWidget *parent = Q_NULLPTR);
    ~MCmdLine();

    Q_PROPERTY(int historyLength READ getHistoryLen WRITE setHistoryLen)

    bool event(QEvent* event);

    int getHistoryLen();

public slots:
    void setHistoryLen(int l);
    void cmdComplete(QString s);

signals:
    void tabPressed(QString s);
    void enterPressed(QString s);
    void upPressed();
    void downPressed();

private:
    QStringList history;
    int history_point = 0;
    int history_length = 100;
    int complete_pos = 0;
    bool wait_complete = false;

};

#endif // MCMDLINE_H
