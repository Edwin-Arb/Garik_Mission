#include "UIText.h"
#include <assert.h>

void AUIText::InitGameText()
{
    // Инициализация шрифта для текста
    assert(MissionText0.loadFromFile(ASSETS_PATH + "MainTiles/Mission0.png"));
    assert(MissionText1.loadFromFile(ASSETS_PATH + "MainTiles/Mission1.png"));
    assert(MissionText2.loadFromFile(ASSETS_PATH + "MainTiles/Mission2.png"));
    assert(FinalMissionText.loadFromFile(ASSETS_PATH + "MainTiles/FinalMission.png"));

    // TextForMissions.setFont(Font);
    // TextForMissions.setCharacterSize(10);
    // TextForMissions.setFillColor(sf::Color::White);
    // TextForMissions.setPosition(100, 600);
    // //TextForMissions.setOrigin(offset_x, offset_y);
    // TextForMissions.setString("Test");
    // TextForMissions.setScale(1.f, 1.f);
    //
    // // Отключение сглаживания
    // TextForMissions.setStyle(sf::Text::Regular);


    //MissionSprite.setTexture(MissionText);
    MissionSprite.setScale(0.05f, 0.05f);
}

void AUIText::SetPositionGameText(const sf::Vector2f& NewPosition)
{
    MissionSprite.setPosition(NewPosition);
}

void AUIText::UpdateMissionText()
{
    // if(AKeyObject::KeyCount)
    // {
    //     
    // }
    switch (AKeyObject::KeysCountFound)
    {
    case 0:
        {
            MissionSprite.setTexture(MissionText0);
            break;
        }
    case 1:
        {
            MissionSprite.setTexture(MissionText1);
            break;
        }
    case 2:
        {
            MissionSprite.setTexture(MissionText2);
            break;
        }
    default:
        {
            MissionSprite.setTexture(FinalMissionText);
            break;
        }
    }
}

void AUIText::DrawGameText(sf::RenderWindow& Window) const
{
    Window.draw(MissionSprite);
}
