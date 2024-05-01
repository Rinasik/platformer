#pragma once

#include <cmath>
#include <unordered_map>
#include <vector>
#include <cmath>

#include "../utils/utils.hpp"
#include "../utils/constants.hpp"
#include "../objects/object/object.hpp"

// Spatial Hashing
class Machine
{
private:
    std::unordered_map<int, std::unordered_map<Object *, int>> dict;
    std::unordered_map<Object *, std::vector<int>> objects;

    auto key(int x, int y) -> long long int;

public:
    auto UpdatePosition(Object *object) -> void;
    auto AddObject(Object *object) -> void;
    auto RemoveObject(Object *object) -> void;
    auto FindNearby(Object *object) -> std::unordered_set<Object *>;
    auto Clear() -> void;
};

auto Machine::key(int x, int y) -> long long int
{
    return ((x + 1) * 73856093) ^ ((y + 1) * 19349663);
}

auto Machine::FindNearby(Object *object) -> std::unordered_set<Object *>
{
    Shape shape = object->GetShape();
    auto result = std::unordered_set<Object *>();

    int left_i = floor(shape.left / DELTA_X) + decimalPart(shape.left / DELTA_X);
    int right_i = ceil(shape.right / DELTA_X) - 1;

    int top_i = ceil(shape.top / DELTA_Y) - 1;
    int bottom_i = floor(shape.bottom / DELTA_Y) + decimalPart(shape.bottom / DELTA_Y);

    int minX = left_i - 1 >= 0 ? left_i - 1 : 0;
    int minY = bottom_i - 1 >= 0 ? bottom_i - 1 : 0;

    int maxX = right_i + 1 >= WIDTH ? WIDTH : right_i + 1;
    int maxY = top_i + 1 >= HEIGHT ? HEIGHT : top_i + 1;

    for (int i = minX; i <= maxX; ++i)
    {
        for (int j = minY; j <= maxY; ++j)
        {
            auto k = key(i, j);
            for (auto vector : dict[k])
            {
                if (vector.first != object)
                {
                    result.emplace(vector.first);
                }
            }
        }
    }
    return result;
}

auto Machine::AddObject(Object *object) -> void
{
    auto shape = object->GetShape();

    int left_i = floor(shape.left / DELTA_X) + decimalPart(shape.left / DELTA_X);
    int right_i = ceil(shape.right / DELTA_X) - 1;

    int top_i = ceil(shape.top / DELTA_Y) - 1;
    int bottom_i = floor(shape.bottom / DELTA_Y) + decimalPart(shape.bottom / DELTA_Y);

    for (int x = left_i; x <= right_i; ++x)
    {
        for (int y = bottom_i; y <= top_i; ++y)
        {
            int k = key(x, y);
            auto val = dict[k];

            if (dict.find(k) == dict.end())
            {
                dict.emplace(k, std::unordered_map<Object *, int>());
            }
            dict[k].emplace(object, 1);

            if (objects.find(object) == objects.end())
            {
                objects.emplace(object, std::vector<int>());
            }
            objects[object].push_back(k);
        }
    }
}

auto Machine::RemoveObject(Object *object) -> void
{
    auto ks = objects[object];

    for (auto k : ks)
    {
        dict[k].erase(object);
    }
    objects.erase(object);
}

auto Machine::UpdatePosition(Object *object) -> void
{
    RemoveObject(object);
    AddObject(object);
}

auto Machine::Clear() -> void
{
    dict = std::unordered_map<int, std::unordered_map<Object *, int>>();
    objects = std::unordered_map<Object *, std::vector<int>>();
}