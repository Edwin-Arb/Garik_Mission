#pragma once
#include "../Constants.h"
#include "../GameObjects/KeyObject.h"

class AUIText
{
public:
    ~AUIText() = default;

    void InitGameText();
    void SetPositionGameText(const sf::Vector2f& NewPosition);
    void UpdateMissionText();
    
    void DrawGameText(sf::RenderWindow& Window) const;

private:
    sf::Texture MissionText0;
    sf::Texture MissionText1;
    sf::Texture MissionText2;
    sf::Texture FinalMissionText;
    sf::Sprite MissionSprite;
};
