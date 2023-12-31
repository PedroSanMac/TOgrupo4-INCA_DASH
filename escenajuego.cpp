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

void GameScene::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Left:
    case Qt::Key_Right:
        m_rightMove = false;
        m_leftMove = false;
        break;
    }
    QGraphicsScene::keyReleaseEvent(event);
}


void GameScene::clampXpos()
{
    // Verifica la dirección en la que está mirando el héroe
    if (m_heroTransform.m11() == -1) // si mira izquierda
    {
        // Si el héroe está mirando hacia la izquierda...
        qDebug() << "Mirando Izquierda";
        // Verifica si la posición horizontal del héroe menos el ancho del boundingRect es menor que 0
        if (m_heroXpos - m_heroItem->boundingRect().width() < 0)
        {
            qDebug() << "Ajustando Izquierda";
            // Si la posición es menor que 0, ajusta la posición para que el héroe no se salga del límite izquierdo
            m_heroXpos = m_heroItem->boundingRect().width();
        }
    }
    else
    {
        // Si el héroe no está mirando hacia la izquierda.
        qDebug() << "Mirando Derecha";
        // Verifica si la posición horizontal del héroe más el ancho del boundingRect es mayor que la resolución del juego
        if (m_heroXpos + m_heroItem->boundingRect().width() > Game::RESOLUTION.width())
        {
            qDebug() << "Ajsuntado Derecha";
            // Si la posición es mayor que la resolución del juego, ajusta la posición para que el héroe no se salga del límite derecho
            m_heroXpos = Game::RESOLUTION.width() - m_heroItem->boundingRect().width();
        }
    }
}


void GameScene::drawScore()
{
    // Convertir el puntaje a texto
    QString scoreText = QString::number(m_game.POINTS);

    // Variables para almacenar las partes del puntaje
    int unityPartVal = 0;
    int decimalPartValue = 0;
    int hendredthPartValue = 0;

    // Determinar las partes del puntaje en función de la longitud del texto
    if (scoreText.length() == 1)
    {
        unityPartVal = scoreText.toInt();
        decimalPartValue = 0;
        hendredthPartValue = 0;
    }
    else if (scoreText.length() == 2)
    {
        unityPartVal = scoreText.last(1).toInt();
        decimalPartValue = scoreText.first(1).toInt();
        hendredthPartValue = 0;
    }
    else if (scoreText.length() == 3)
    {
        unityPartVal = scoreText.last(1).toInt();
        hendredthPartValue = scoreText.first(1).toInt();
        QString copyVal = scoreText;
        copyVal.chop(1);
        decimalPartValue = copyVal.last(1).toInt();
    }

    // Crear elementos gráficos para cada parte del puntaje y añadirlos a la escena
    QGraphicsPixmapItem *unityPartScoreItem = new QGraphicsPixmapItem(m_numberPixmap.copy(unityPartVal * Game::NUMBER_SIZE.width(), 0, Game::NUMBER_SIZE.width(), Game::NUMBER_SIZE.width()));
    unityPartScoreItem->moveBy(Game::RESOLUTION.width() - Game::NUMBER_SIZE.width(), 0);
    addItem(unityPartScoreItem);

    QGraphicsPixmapItem *decimalPartScoreItem = new QGraphicsPixmapItem(m_numberPixmap.copy(decimalPartValue * Game::NUMBER_SIZE.width(), 0, Game::NUMBER_SIZE.width(), Game::NUMBER_SIZE.width()));
    decimalPartScoreItem->moveBy(Game::RESOLUTION.width() - 2 * Game::NUMBER_SIZE.width(), 0);
    addItem(decimalPartScoreItem);

    QGraphicsPixmapItem *hundrethPartScoreItem = new QGraphicsPixmapItem(m_numberPixmap.copy(hendredthPartValue * Game::NUMBER_SIZE.width(), 0, Game::NUMBER_SIZE.width(), Game::NUMBER_SIZE.height()));
    hundrethPartScoreItem->moveBy(Game::RESOLUTION.width() - 3 * Game::NUMBER_SIZE.width(), 0);
    addItem(hundrethPartScoreItem);
}


void GameScene::reset()
{
    m_heroXpos = 100;
    m_heroYpos = 100;
    m_height = 200;
    for (int i = 0; i < m_countOfPlatforms; i++)
    {
        m_platforms[i].x = rand() % Game::RESOLUTION.width();
        m_platforms[i].y = rand() % Game::RESOLUTION.height();
    }

    m_game.STATE = Game::State::Active;
    m_game.POINTS = 0;
}


void GameScene::update()
{
    // static int i {};
    qDebug() << "delta X" << m_deltaX;
    qDebug() << "delta Y" << m_deltaY;
    // i++;

    clear();
    m_heroItem = new QGraphicsPixmapItem(QPixmap(m_heroPixmap.copy(0, 0, Game::HERO_SIZE.width(), Game::HERO_SIZE.height())));
    // Mostrar bg
    QGraphicsPixmapItem *bgIteam = new QGraphicsPixmapItem(QPixmap(m_game.PATH_TO_BACKGROUND_PIXMAP).scaled(Game::RESOLUTION.width(), Game::RESOLUTION.height()));
    addItem(bgIteam);

    m_time_since_last_iteration += m_iteration_value;
    while (m_time_since_last_iteration > Game::DELAY && m_game.STATE == Game::State::Active)
    {
        // Reinicia el temporizador para la próxima iteración
        m_time_since_last_iteration = -Game::DELAY;
        // Manejo del movimiento hacia la izquierda
        if (m_leftMove)
        {
            // Ajusta la posición X del héroe
            m_heroXpos -= m_deltaX;
            // Escala original
            if (m_heroTransform.m11() != -1)
            {
                m_heroTransform = m_heroTransform.scale(-1, 1); // Refleja
                m_facingRight = false;                          // el héroe ahora mira hacia la izquierda
                m_heroXpos += m_heroItem->boundingRect().width();
            }
        }
        // Manejo del movimiento hacia la derecha
        else if (m_rightMove)
        {
            // Ajusta la posición X del héroe
            m_heroXpos += m_deltaX;
            if (m_heroTransform.m11() != 1)
            {
                m_heroTransform = m_heroItem->transform();
                m_facingRight = true; // el héroe ahora mira hacia la derecha
                m_heroXpos -= m_heroItem->boundingRect().width();
            }
        }

        m_deltaY += Game::JUMP_SPEED;
        m_heroYpos += m_deltaY;
        // Comprueba que no choque a la parte baja
        if (m_heroYpos > Game::DEAD_LEVEL)
        {
            m_deltaY = Game::JUMP_FORCE;
            m_game.STATE = Game::State::Game_Over;
        }

        if (m_heroYpos < m_height)
        {
            for (int i = 0; i < m_countOfPlatforms; i++)
            {
                m_heroYpos = m_height;
                m_platforms[i].y = m_platforms[i].y - m_deltaY;
                // Si una plataforma se ha movido más allá del borde de la pantalla.
                if (m_platforms[i].y > Game::RESOLUTION.height())
                {
                    m_game.POINTS++;
                    // Reinicia la posición vertical de la plataforma en la parte superior de la pantalla.
                    m_platforms[i].y = 0;
                    // Asigna una nueva posición horizontal aleatoria a la plataforma.
                    m_platforms[i].x = rand() % Game::RESOLUTION.width();
                }
            }
        }

        clampXpos();
        for (int i = 0; i < m_countOfPlatforms; i++)
        {
            if (m_facingRight)
            {
                if ((m_heroXpos + Game::X_OFFSET > m_platforms[i].x) && (m_heroXpos + Game::X_OFFSET < (m_platforms[i].x + Game::PLATFORM_SIZE.width())) && (m_heroYpos + Game::Y_OFFSET > m_platforms[i].y) && (m_heroYpos + Game::Y_OFFSET < (m_platforms[i].y + Game::PLATFORM_SIZE.height())) && (m_deltaY > 0))
                {
                    m_deltaY = Game::JUMP_FORCE; // Si hay colisión y el héroe se está moviendo hacia arriba, aplica un impulso hacia arriba.
                }
            }
            else
            {
                if ((m_heroXpos - Game::X_OFFSET > m_platforms[i].x) && (m_heroXpos - Game::X_OFFSET < (m_platforms[i].x + Game::PLATFORM_SIZE.width())) && (m_heroYpos + Game::Y_OFFSET > m_platforms[i].y) && (m_heroYpos + Game::Y_OFFSET < (m_platforms[i].y + Game::PLATFORM_SIZE.height())) && (m_deltaY > 0))
                {
                    m_deltaY = Game::JUMP_FORCE; // Aplica el impulsos
                }
            }
        }
    }

    clampXpos();
    // Mostraremos plataformas
    for (int i = 0; i < m_countOfPlatforms; ++i)
    {
        QGraphicsPixmapItem *platform_item = new QGraphicsPixmapItem(m_platformPixmap.scaled(Game::PLATFORM_SIZE.width(), Game::PLATFORM_SIZE.height()));
        platform_item->setPos(m_platforms[i].x, m_platforms[i].y);
        addItem(platform_item);
    }

    // Cambia la imagen del heroe cuando salta o cae
    if (m_deltaY < 0)
    {
        // El héroe está saltando (moviéndose hacia arriba)
        // Se establece la imagen del héroe durante el salto (jump pixmap)
        m_heroItem->setPixmap(m_heroPixmap.copy(Game::HERO_SIZE.width(), 0, Game::HERO_SIZE.width(), Game::HERO_SIZE.height()));
    }
    else
    {
        // El héroe está cayendo (moviéndose hacia abajo)
        // Se establece la imagen del héroe durante la caída (fall pixmap)
        m_heroItem->setPixmap(m_heroPixmap.copy(0, 0, Game::HERO_SIZE.width(), Game::HERO_SIZE.height()));
    }

    m_heroItem->setTransform(m_heroTransform);
    m_heroItem->setPos(m_heroXpos, m_heroYpos);
    addItem(m_heroItem);
    drawScore();

    // Muestra pantallas segun cada estado
    if (m_game.STATE == Game::State::Paused)
    {
        QGraphicsPixmapItem *bgItem = new QGraphicsPixmapItem(QPixmap(m_game.PATH_TO_PAUSED_BG).scaled(Game::RESOLUTION.width(), Game::RESOLUTION.height()));
        addItem(bgItem);
    }
    else if (m_game.STATE == Game::State::Game_Over)
    {
        QGraphicsPixmapItem *bgItem = new QGraphicsPixmapItem(QPixmap(m_game.PATH_TO_GAME_OVER_BG).scaled(Game::RESOLUTION.width(), Game::RESOLUTION.height()));
        addItem(bgItem);
    }

}

