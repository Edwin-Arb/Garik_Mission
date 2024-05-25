#include "GameState.h"


AGameState::AGameState()
	: ScreenRect{ { 0.f, 0.f }, { SCREEN_WIDTH, SCREEN_HEIGHT } }
	, EnemyPtr(new AEnemy)
	, PlayerPtr(new APlayer)
	, InputManagerPtr(new AInputManager)
	, SpriteManagerPtr(new ASpriteManager)
	{}

AGameState::~AGameState()
{
	delete EnemyPtr;
	delete PlayerPtr;
	delete InputManagerPtr;
	delete SpriteManagerPtr;

	for (const ABullet* Bullet : BulletsVectorPtr)
	{
		delete Bullet;
	}

	BulletsVectorPtr.clear();
}

void AGameState::InitGame()
{
	constexpr int CapacityVector = 100;

	EnemyPtr->InitEnemy(*SpriteManagerPtr);
	PlayerPtr->InitPlayer(*SpriteManagerPtr);

	BulletsVectorPtr.reserve(CapacityVector);
}

void AGameState::UpdateGameplay(float DeltaTime)
{
	// Обновлять состояние передвижения персонажа
	PlayerPtr->UpdatePlayerMove(DeltaTime);

	// Сделать зарежку между каждый выстрелом
	float ElapsedSeconds = DelayShotTimerHandle.getElapsedTime().asSeconds();
	if (ElapsedSeconds >= 0.05f)
	{
		// Делаем выстрел, если нажали левую кнопку мыши
		InputManagerPtr->PlayerShoots(BulletsVectorPtr, PlayerPtr->GetPlayerRect(), *SpriteManagerPtr);
		DelayShotTimerHandle.restart();
	}

	// Проверяем с чем сталкиваются пули
	InputManagerPtr->CheckBulletCollision(BulletsVectorPtr, *EnemyPtr, DeltaTime);
}

void AGameState::DrawGame(sf::RenderWindow& Window) const
{
	// Рисовать остальные объекты
	EnemyPtr->DrawEnemy(Window);
	PlayerPtr->DrawPlayer(Window);

	// Рисовать пули
	for (ABullet* bullet : BulletsVectorPtr)
	{
		bullet->DrawBullet(Window);
	}
}