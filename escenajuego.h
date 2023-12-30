#ifndef ESCENAJUEGO_H
#define ESCENAJUEGO_H


#include <QGraphicsScene>
#include <QTransform>

#include "juego.h"
#include "punto.h"
#include <QPixmap>

class QGraphicsPixmapItem;
class QTimer;
class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GameScene(QObject *parent = nullptr);

signals:

protected:
    // Métodos virtuales protegidos para manejar eventos de teclado.
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;

private:
    // Métodos privados para funcionalidades específicas de la clase.
    void clampXpos();          // Limita la posición X de algún objeto.
    void drawScore();          // Dibuja la puntuación en la interfaz.
    void reset();              // Reinicia el estado interno de la clase.
    void renderScene();        // Renderiza la escena del juego.

    // Miembros de datos.
    Game m_game;               // Objeto de la clase Game
    float m_time_since_last_iteration;  // Tiempo transcurrido desde la última iteración del juego.
    const float m_iteration_value;      // Valor de iteración constante para el juego.

    bool m_leftMove;           // Bandera que indica si se está moviendo hacia la izquierda.
    bool m_rightMove;          // Bandera que indica si se está moviendo hacia la derecha.

    int m_heroXpos;            // Posición X del héroe.
    int m_heroYpos;            // Posición Y del héroe.
    const int m_deltaX;         // Cambio en la posición X.
    float m_deltaY;            // Cambio en la posición Y
    int m_height;               // Altura

    QGraphicsPixmapItem* m_heroItem;  // Elemento gráfico del héroe. (!!)
    QTransform m_heroTransform;       // Transformación del héroe.
    bool m_facingRight;               // Indica si el héroe está mirando hacia la derecha.

    const int m_countOfPlatforms;    // Número de plataformas en el juego.
    Point m_platforms[10];           // Arreglo de estructuras Point que almacena las plataformas.

    QTimer *m_timer;                // Temporizador utilizado para actualizar la escena periódicamente.

    QPixmap m_heroPixmap;           // Mapa de píxeles para la imagen del héroe.
    QPixmap m_numberPixmap;         // Mapa de píxeles para los números (posiblemente utilizados en la puntuación).
    QPixmap m_platformPixmap;       // Mapa de píxeles para las plataformas.

private slots:
    // Método de ranura (slot) que se conecta con la señal del temporizador para realizar actualizaciones periódicas.
    void update();
};
#endif // ESCENAJUEGO_H
