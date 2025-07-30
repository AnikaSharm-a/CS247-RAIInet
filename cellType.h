#ifndef CELLTYPE_H
#define CELLTYPE_H

// defines the different types of cells that can exist on the board
enum class CellType {
    Normal,
    Firewall,
    ServerPort,
    Fog
};

#endif

