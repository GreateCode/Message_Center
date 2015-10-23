#ifndef TIPWIDGET_H
#define TIPWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QPoint>
#include <QPaintEvent>

namespace Ui {
class tipWidget;
}

class tipWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit tipWidget(QWidget *parent = 0);
    ~tipWidget();

    void setTitle (const QString& title);
    void setInfoContent (const QString& content);

protected:
    void paintEvent(QPaintEvent*);
    
signals:
    void clickedClose ();
    void clickedMore (bool);

private:
    void initUI ();
    void initConn ();

    void mousePressEvent(QMouseEvent*   event);
    void mouseMoveEvent (QMouseEvent*   event);

private:
    Ui::tipWidget *ui;
    QPoint          m_dPos;
};

#endif // TIPWIDGET_H
