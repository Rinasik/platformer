#pragma once

#include "../enemy/enemy.hpp"

class Warrior : virtual public Enemy
{
private:
public:
    Warrior(int ix, int iy);

    auto Run(std::unordered_set<Object *> neighbours) -> void;
    auto Draw() -> void;

    auto GetBonus() -> std::optional<Bonus *>;
};

Warrior::Warrior(int ix, int iy) : Enemy{
                                       ix,
                                       iy,
                                       1, 1, 2,
                                       std::vector<Direction>({Direction::Right, Direction::Right, Direction::Left, Direction::Left, Direction::Left, Direction::Left, Direction::Right, Direction::Right}),
                                       MapEncoding::Warrior}
{
    _velX = ENEMY_X_VELOCITY;
}

void CreateWarrior(Position position, std::unordered_set<Entity *> &entities)
{
    entities.emplace(new Warrior(position.ix, position.iy));
}

auto Warrior::Draw() -> void
{
    if (!_lives)
    {
        return;
    }

    glColor4f(0.87f, 0.0f, 0.32f, 1.0f);

    glBegin(GL_QUADS);

    glVertex2f(_x - 1.f, _y - 1.f);
    glVertex2f(_x - 1.f, _y + DELTA_Y * _sizeY - 1.f);
    glVertex2f(_x + DELTA_X * _sizeX - 1.f, _y + DELTA_Y * _sizeY - 1.f);
    glVertex2f(_x + DELTA_X * _sizeX - 1.f, _y - 1.f);

    glEnd();
}

auto Warrior::Run(std::unordered_set<Object *> neighbours) -> void
{
    if (!_lives)
    {
        isDestroyed = true;
        return;
    }

    float currentPosX = abs((_x - _prevX) / DELTA_X);

    if (currentPosX >= 1)
    {
        int _nextStep = (_currentStep + 1) % _pattern.size();
        if (_pattern[_currentStep] != _pattern[_nextStep])
        {
            _velX = -_velX;
        }

        _prevX = _x;
        _currentStep = _nextStep;
    }
    objectsCollisionX(neighbours, _velX);

    _x += _velX;
}

auto Warrior::GetBonus() -> std::optional<Bonus *>
{
    if (std::rand() % 10 <= 1)
    {
        return new Bonus(_x, _y, BonusType::OneLife);
    }

    return std::nullopt;
}