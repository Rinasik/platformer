#pragma once

#include <GL/glut.h>
#include "../entity/entity.hpp"

class Chest : virtual public Entity
{
private:
    Texture *_chest;

    auto collisionLeftDetected(Object *neighbour, Shape nshape, double &virtualDeltaX) -> void;
    auto collisionRightDetected(Object *neighbour, Shape nshape, double &virtualDeltaX) -> void;

public:
    Chest(double ix, double iy);

    auto Draw() -> void;
    auto Run(std::unordered_set<Object *> neighbours) -> void;

    auto GetBonus() -> std::optional<Bonus *>;
};

Chest::Chest(double ix, double iy) : Entity{
                                         ix,
                                         iy,
                                         1,
                                         1,
                                         MapEncoding::Chest}
{
    _chest = CHEST;
};

auto CreateChest(Position position, std::unordered_set<Entity *> &entities) -> void
{
    entities.emplace(new Chest(position.ix, position.iy));
}

auto Chest::Draw() -> void
{
    glBindTexture(GL_TEXTURE_2D, _chest->GetTexture());
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);

    glTexCoord2f(0, 0);
    glVertex2f(_x - 1.0f, _y - 1.0f);

    glTexCoord2f(0, 1);
    glVertex2f(_x - 1.0f, _y + _sizeY * DELTA_Y - 1.0f);

    glTexCoord2f(1, 1);
    glVertex2f(_x + _sizeX * DELTA_X - 1.0f, _y + _sizeY * DELTA_Y - 1.0f);

    glTexCoord2f(1, 0);
    glVertex2f(_x + _sizeX * DELTA_X - 1.0f, _y - 1.0f);

    glEnd();

    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
};

auto Chest::GetBonus() -> std::optional<Bonus *>
{
    if (std::rand() <= 2)
    {
        return new Bonus(_x, _y, BonusType::DoubleJump);
    }
    else
    {
        return new Bonus(_x, _y, BonusType::BigHit);
    }
}

auto Chest::Run(std::unordered_set<Object *> neighbours) -> void {};

auto Chest::collisionLeftDetected(Object *neighbour, Shape nshape, double &virtualDeltaX) -> void {};
auto Chest::collisionRightDetected(Object *neighbour, Shape nshape, double &virtualDeltaX) -> void {};