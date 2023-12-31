#include "escenajuego.h"
#include "juego.h"
#include <QKeyEvent>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QDir>
#include <QPainter>

// Constructor de la clase GameScene, que hereda de QGraphicsScene en Qt.
GameScene::GameScene(QObject *parent)
    : QGraphicsScene{parent},             // Inicialización del constructor de la clase base QGraphicsScene con el puntero al padre.
    m_iteration_value(1000.0f / 60.0f), // Inicialización de la frecuencia de iteración para la actualización del juego.

    // Inicialización de variables miembro relacionadas con el estado del juego y el héroe.
    m_leftMove(false), m_rightMove(false), m_heroXpos(100), m_heroYpos(100),
    m_deltaX(3), m_deltaY(0.2f), m_height(200), m_facingRight(true), m_countOfPlatforms(10)
{
    // Llamada a la función estática init() de la clase Game para inicializar la configuración del juego.
    Game::init();

    // Inicialización de la semilla del generador de números aleatorios basada en el tiempo actual.
    srand(time(0));

    // Establece el rectángulo de la escena para que coincida con la resolución del juego.
    setSceneRect(0, 0, m_game.RESOLUTION.width(), m_game.RESOLUTION.height());

    // Establece el fondo de la escena con un color blanco.
    setBackgroundBrush(Qt::white);

    // Carga la imagen del héroe desde la ruta especificada en la configuración del juego.
    m_heroPixmap.load(m_game.PATH_TO_HERO_PIXMAP);

    // Crea un objeto QGraphicsPixmapItem para representar al héroe y lo agrega a la escena.
    m_heroItem = new QGraphicsPixmapItem(QPixmap(m_heroPixmap));
    m_heroTransform = m_heroItem->transform();
    addItem(m_heroItem);

    // Carga la imagen de los números utilizados en el juego.
    m_numberPixmap.load(m_game.PATH_TO_ALL_NUMBERS_PIXMAP);

    // Carga la imagen de las plataformas utilizadas en el juego.
    m_platformPixmap.load(m_game.PATH_TO_PLATFORM_PIXMAP);

    // Crea un temporizador para actualizar periódicamente la escena y conecta la señal timeout a la ranura update.
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &GameScene::update);
    m_timer->start(m_iteration_value);

    // Genera posiciones aleatorias para las plataformas del juego.
    for (int i = 0; i < m_countOfPlatforms; i++)
    {
        m_platforms[i].x = rand() % Game::RESOLUTION.width();
        m_platforms[i].y = rand() % Game::RESOLUTION.height();
    }
}

void GameScene::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Left:
    {
        if (m_game.STATE == Game::State::Active)
        {
            m_leftMove = true;
        }
    }

    break;
    case Qt::Key_Right:
    {
        if (m_game.STATE == Game::State::Active)
        {
            m_rightMove = true;
        }
    }

    break;
    case Qt::Key_P:
    {
        if (m_game.STATE == Game::State::Active)
        {
            m_game.STATE = Game::State::Paused;
        }
        else if (m_game.STATE == Game::State::Paused)
        {
            m_game.STATE = Game::State::Active;
        }
    }
    break;
    case Qt::Key_R:
    {
        if (m_game.STATE == Game::State::Game_Over)
        {
            reset();
        }
    }
    break;
    }

    QGraphicsScene::keyPressEvent(event);
}