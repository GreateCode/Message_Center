#ifndef BUTTONLABEL_H
#define BUTTONLABEL_H

#include <QLabel>
class QCursor;
class ButtonLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ButtonLabel(QWidget *parent = 0);
    void setLabelText(const QString text);
    void setMouseClicked(bool isClicked);
    void setUnderLine(bool underline);

signals:
    void labelClicked(bool);

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *e);

private:
    QCursor m_preCursor;
    bool    m_underline;
    bool    m_preUnderline;
};

#endif // BUTTONLABEL_H
