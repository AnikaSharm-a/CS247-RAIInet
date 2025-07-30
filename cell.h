#ifndef CELL_H
#define CELL_H

#include "link.h"
#include "cellType.h"
#include <memory>
using namespace std;

// represents a single square on the game board
class Cell {
    shared_ptr<Link> link = nullptr; // link occupying this cell (if any)
    CellType type = CellType::Normal;     // type of cell (normal, firewall, server port, fog)
    int ownerId = -1;                     // ID of player who owns this cell (for firewalls/server ports)

public:
    Cell() = default;
    Cell(CellType type, int ownerId = -1) : type(type), ownerId(ownerId) {}

    // checks if this cell is empty (no link)
    bool isEmpty() const { return link == nullptr; }
    
    // returns the link in this cell as a shared_ptr
    shared_ptr<Link> getLink() const { return link; }
    
    // returns the type of this cell
    CellType getType() const { return type; }
    
    // returns the ID of the player who owns this cell
    int getOwnerId() const { return ownerId; }

    // sets the link in this cell using a shared_ptr
    void setLink(shared_ptr<Link> l) { link = l; }
    
    // sets the link in this cell using a raw pointer
    void setLink(Link* l) { link = shared_ptr<Link>(l); }
    
    // removes the link from this cell
    void removeLink() { link = nullptr; }

    // sets the type of this cell
    void setType(CellType newType) { type = newType; }

};

#endif