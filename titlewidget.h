#ifndef TITLEWIDGET_H
#define TITLEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include "btnmodel.h"


class TitleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TitleWidget(QWidget *parent = 0);
    ~TitleWidget();

    void setTitleText(const QString &text, int size);
    void hideClose();

signals:
    void closeWidget();
    void hideInstall();
protected:
    void paintEvent(QPaintEvent *);

private:
    void initUI();

    void initConnects();

private:
    QLabel*         m_titleLabel;
    BtnModel*       m_closeBtn;
};

#endif // TITLEWIDGET_H
