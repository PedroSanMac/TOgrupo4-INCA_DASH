#ifndef JUEGO_H
#define JUEGO_H


#include <QSize>
#include <QString>


class Game
{
public:
    Game();
    // Inicializa la configuración de la aplicación.
    static void init();

    // Tamaño de la resolución de la aplicación.
    static QSize RESOLUTION;

    // Rutas a los archivos de imagen para los elementos de fondo, héroe, plataforma, números, y fondos de pausa y fin de juego.
    QString PATH_TO_BACKGROUND_PIXMAP;
    QString PATH_TO_BACKGROUND2_PIXMAP;
    QString PATH_TO_HERO_PIXMAP;
    QString PATH_TO_PLATFORM_PIXMAP;
    QString PATH_TO_ALL_NUMBERS_PIXMAP;
    QString PATH_TO_PAUSED_BG;
    QString PATH_TO_GAME_OVER_BG;

    // Retardo en la ejecución de la aplicación.
    static float DELAY;

    // Fuerza de salto del héroe.
    static const float JUMP_FORCE;

    // Variables para mejorar el salto.
    // Offset en las coordenadas X e Y para un mejor salto.
    static const int X_OFFSET;
    static const int Y_OFFSET;
    // Tamaño del héroe.
    static const QSize HERO_SIZE;

    // Velocidad de salto del héroe.
    static const float JUMP_SPEED;

    // Nivel en el que el héroe se considera muerto.
    static const int DEAD_LEVEL;

    // Tamaño de las plataformas.
    static const QSize PLATFORM_SIZE;

    // Tamaño de los números utilizados para mostrar los puntos.
    static const QSize NUMBER_SIZE;

    // Puntos acumulados en el juego.
    int POINTS;


    enum class State{
        Active, Paused, Game_Over
    };

    State STATE;
};

#endif // JUEGO_H
