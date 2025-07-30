#ifndef CELL_H
#define CELL_H

#include "link.h"
#include "cellType.h"
#include <memory>

class Cell {
    std::shared_ptr<Link> link = nullptr;
    CellType type = CellType::Normal;
    int ownerId = -1;

public:
    Cell() = default;
    Cell(CellType type, int ownerId = -1) : type(type), ownerId(ownerId) {}

    bool isEmpty() const { return link == nullptr; }
    std::shared_ptr<Link> getLink() const { return link; }
    Link* getLinkRaw() const { return link.get(); }  // For backward compatibility
    CellType getType() const { return type; }
    int getOwnerId() const { return ownerId; }

    void setLink(std::shared_ptr<Link> l) { link = l; }
    void setLink(Link* l) { link = std::shared_ptr<Link>(l); }  // For backward compatibility
    void removeLink() { link = nullptr; }

    void setType(CellType newType) { type = newType; }

};

#endif // CELL_H
