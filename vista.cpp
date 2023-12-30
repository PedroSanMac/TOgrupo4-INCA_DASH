#include "vista.h"
#include "escenajuego.h"
#include <QApplication>
#include <QKeyEvent>

View::View()
    : QGraphicsView(), m_gameScene(new GameScene(this))
{
    // Setea la escena del graficsView
    setScene(m_gameScene);
    // Damos ancho y altura fija
    resize(Game::RESOLUTION.width() + 2, Game::RESOLUTION.height() + 2);
    // Desactivar scroll en ambos ejes
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void View::keyPressEvent(QKeyEvent *event)
{
    // Verifica si la tecla presionada no es un evento de repetición automática.
    if (!event->isAutoRepeat())
    {
        // Utiliza un switch para manejar diferentes teclas.
        switch (event->key())
        {
        // Caso en el que la tecla presionada es la tecla Escape.
        case Qt::Key_Escape:
            // Cierra la aplicación.
            QApplication::instance()->quit();
            break;
        }
    }

    QGraphicsView::keyPressEvent(event);
}
