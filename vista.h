#ifndef VISTA_H
#define VISTA_H

#include <QGraphicsView>
class GameScene;

class View : public QGraphicsView
{
    Q_OBJECT
public:
    explicit View();

signals:

private:
    GameScene *m_gameScene;

    // QWidget interface
protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
};
#endif // VISTA_H
