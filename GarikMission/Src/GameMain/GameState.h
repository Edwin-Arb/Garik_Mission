#pragma once
#include "../Manager/CollisionManager.h"
#include "../Manager/FpsManager.h"
#include "../Manager/ParticleSystemManager.h"

#include "../UserInterface/UIText.h"
#include "../GameObjects/KeyObject.h"
#include "../GameObjects/ChestObject.h"

// Подключение типов врагов
#include "../Enemy/BossEnemy/BossEnemy.h"
#include "../Enemy/BaseEnemy/BaseRedEnemy.h"
#include "../Enemy/BaseEnemy/BaseBlueEnemy.h"
#include "../Enemy/BaseEnemy/BaseGreenEnemy.h"


/**
 * @brief Класс AGameState представляет текущее состояние игры.
 * Он управляет инициализацией игры, обновлением ввода, игрового процесса, камеры и отрисовкой.
 */
class AGameState
{
public:
    /**
     * @brief Конструктор класса AGameState.
     */
    AGameState();

    /**
     * @brief Деструктор класса AGameState.
     */
    ~AGameState();

    /**
     * @brief Инициализирует начальное состояние игры, включая игровые объекты и менеджеры.
     */
    void InitGame();

    /**
     * @brief Обновляет ввод пользователя.
     * @param DeltaTime Время, прошедшее с последнего кадра.
     */
    void UpdateInput(float DeltaTime);

    /**
     * @brief Обновляет игровой процесс, включая физику и логику объектов.
     * @param DeltaTime Время, прошедшее с последнего кадра.
     */
    void UpdateGameplay(float DeltaTime);

    /**
     * @brief Обновляет камеру игры в зависимости от положения персонажа.
     * @param Window Окно для отрисовки игры.
     */
    void UpdateCamera(sf::RenderWindow& Window);

    /**
     * @brief Отрисовывает текущее состояние игры в окне.
     * @param Window Окно для отрисовки игры.
     */
    void DrawGame(sf::RenderWindow& Window) const;

private:
    // TODO: Если размер экрана не пригодиться в использование, то удалить
    sf::FloatRect ScreenRect; // Прямоугольник экрана игры

    APlayer* PlayerPtr; // Указатель на игрока
    AGameMap* GameMapPtr; // Указатель на игровую карту
    std::vector<AEnemy*> EnemyVectorPtr; // Вектор врагов
    std::vector<ABullet*> BulletsVectorPtr; // Вектор пуль

    ASpriteManager* SpriteManagerPtr; // Менеджер спрайтов
    ACollisionManager* CollisionManagerPtr; // Менеджер коллизий
    AParticleSystemManager* ParticleSystemPtr; // Менеджер частиц
    AFpsManager* FpsManagerPtr; // Менеджер FPS
    AUIText* GameTextPtr; // Текст для игры
    std::vector<AKeyObject*> KeysVectorPtr; // Вектор указателей на ключи, для выполения миссии в игре
    std::vector<AChestObject*> ChestVectorPtr; // Вектор указателей на сундуки, для выполения миссии в игре

    sf::Clock DelayShotTimerHandle; // Таймер для задержки выстрелов
    sf::View ViewPlayer; // Камера для отображения игрового мира
};
