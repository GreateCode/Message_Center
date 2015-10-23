#include "buttonlabel.h"
#include <QCursor>
#include <QMouseEvent>

ButtonLabel::ButtonLabel(QWidget *parent) :
    QLabel(parent)
{
    //this->setObjectName("blueLabel");
    this->setStyleSheet("QLabel{color:blue;}");
    m_underline = m_preUnderline = this->font().underline();
}

void ButtonLabel::setMouseClicked(bool isClicked)
{
    if (isClicked)
    {
       this->setStyleSheet("QLabel{color:black;font: bold;}");
    }
    else
    {
        this->setStyleSheet("QLabel{color:blue;}");
    }
}

void ButtonLabel::setLabelText(const QString text)
{
    this->setText(text);
    this->setStyleSheet("QLabel{color:blue;}");
}

void ButtonLabel::setUnderLine(bool underline)
{
    m_underline = underline;
}

void ButtonLabel::mousePressEvent(QMouseEvent *)
{

}

void ButtonLabel::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() != Qt::LeftButton)
    {
        e->ignore();
        return;
    }

    if(rect().contains(e->pos()))
    {
        emit labelClicked(true);
    }
}

void ButtonLabel::enterEvent(QEvent *)
{
    m_preCursor = cursor();
    setCursor(Qt::OpenHandCursor);
    QFont font = this->font();
    font.setUnderline(m_underline);
    this->setFont(font);
}

void ButtonLabel::leaveEvent(QEvent *)
{
    setCursor(m_preCursor);
    QFont font = this->font();
    font.setUnderline(m_preUnderline);
    this->setFont(font);
}
