#ifndef LOOKONEMSG_H
#define LOOKONEMSG_H

#include <QWidget>
#include <QObject>
#include <QMouseEvent>
#include <QPaintEvent>

namespace Ui {
class LookOneMsg;
}

class LookOneMsg : public QWidget
{
    Q_OBJECT
    
public:
    explicit LookOneMsg(QWidget *parent = 0);
    ~LookOneMsg();

    void setTitle (const QString& title);
    void setBrowseText (const QString& text);
    
private:
    void initUI ();
    void initConn ();
    void mousePressEvent(QMouseEvent*   event);
    void mouseMoveEvent (QMouseEvent*   event);
    void paintEvent(QPaintEvent *);

private:
    Ui::LookOneMsg *ui;
    QPoint      m_dPos;
};

#endif // LOOKONEMSG_H
