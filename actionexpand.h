#ifndef ACTIONEXPAND_H
#define ACTIONEXPAND_H

#include <QAction>
#include <QEvent>
#include <QIcon>
#include <QDebug>

class ActionExpand : public QAction
{
    Q_OBJECT
public:
    explicit ActionExpand(QObject *parent = 0);

signals:

public slots:

protected:
    bool event(QEvent *e);

};

#endif // ACTIONEXPAND_H
