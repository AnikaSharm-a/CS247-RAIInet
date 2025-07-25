#pragma once
#include "link.h"
#include "cellType.h"

class Cell {
    Link* link = nullptr;
    CellType type = CellType::Normal;
    int ownerId = -1;

public:
    Cell() = default;
    Cell(CellType type, int ownerId = -1) : type(type), ownerId(ownerId) {}

    bool isEmpty() const { return link == nullptr; }
    Link* getLink() const { return link; }
    CellType getType() const { return type; }
    int getOwnerId() const { return ownerId; }

    void setLink(Link* l) { link = l; }
    void removeLink() { link = nullptr; }
};
