#include "AnimationManager.h"

/**
 * @brief Конструктор класса AAnimationManager.
 * 
 * Инициализирует скорость смены кадров и текущий индекс кадра.
 */
AAnimationManager::AAnimationManager()
    : bIsAnimationFinished(false),
      bStopAtLastFrame(false),
      FrameSpeed(0.1f),
      CurrentFrameIndex(0)
{
}

bool AAnimationManager::IsAnimationFinished() const
{
    return bIsAnimationFinished;
}

/**
 * @brief Обновляет текущий кадр анимации на основе прошедшего времени.
 * 
 * @param DeltaTime Время, прошедшее с последнего обновления кадра.
 */
void AAnimationManager::AnimationUpdate(float DeltaTime)
{
    // Проверяем, если анимация уже завершена, то ничего не делаем
    if (bIsAnimationFinished)
    {
        return;
    }

    // Увеличиваем время с учётом скорости анимации
    CurrentFrameIndex += FrameSpeed * DeltaTime;

    // Если текущий индекс превысил количество кадров, начинаем сначала
    if (CurrentFrameIndex >= FrameRect.size())
    {
        if (bStopAtLastFrame)
        {
            // Устанавливаем индекс на последний кадр
            CurrentFrameIndex = FrameRect.size() - 1;
            bIsAnimationFinished = true;
        }
        else
        {
            // Если анимация должна зацикливаться, начинаем сначала
            CurrentFrameIndex = 0;
        }
    }
}

/**
 * @brief Устанавливает текущий кадр анимации по индексу.
 * 
 * @param Frame Индекс кадра анимации.
 */
void AAnimationManager::SetCurrentFrame(const int Frame)
{
    CurrentFrameIndex = static_cast<float>(Frame);
    bIsAnimationFinished = false;
}

/**
 * @brief Устанавливает флаг для остановки на последнем кадре.
 * 
 * @param bStop Флаг, указывающий, нужно ли остановить анимацию на последнем кадре.
 */
void AAnimationManager::SetStopAtLastFrame(bool bStop)
{
    bStopAtLastFrame = bStop;
}

/**
 * @brief Получает текущий индекс кадра анимации.
 * 
 * @return int Текущий индекс кадра анимации.
 */
int AAnimationManager::GetCurrentFrameIndex() const
{
    return static_cast<int>(CurrentFrameIndex);
}

/**
 * @brief Получает прямоугольник текущего кадра анимации на текстуре.
 * 
 * @return sf::IntRect Прямоугольник текущего кадра анимации.
 */
sf::IntRect AAnimationManager::GetCurrentFrame() const
{
    return FrameRect[static_cast<uint16_t>(CurrentFrameIndex)];
}
