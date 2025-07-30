#ifndef VIEW_H
#define VIEW_H

#include "cellType.h" 
#include <iostream>
#include <string>

class Game;  // forward declaration of Game
using namespace std;

// Enum for different types of notifications
enum class NotificationType {
    CellChanged,      // A cell on the board changed
    PlayerChanged,    // Player state changed (downloads, abilities, etc.)
    GameStateChanged, // Game state changed (turn, victory, etc.)
    BoardChanged,     // Board state changed (firewalls, fog, etc.)
    LinkMoved,        // A link moved on the board
    LinkRevealed,     // A link was revealed
    LinkDownloaded    // A link was downloaded
};

// Structure to hold notification data
struct NotificationData {
    NotificationType type;
    int row = -1;
    int col = -1;
    char linkId = '\0';
    int playerId = -1;
    std::string message;
    
    // Constructor for different notification types
    NotificationData(NotificationType t) : type(t) {}
    NotificationData(NotificationType t, int r, int c) : type(t), row(r), col(c) {}
    NotificationData(NotificationType t, char id) : type(t), linkId(id) {}
    NotificationData(NotificationType t, int pid) : type(t), playerId(pid) {}
    NotificationData(NotificationType t, const std::string& msg) : type(t), message(msg) {}
};

class View {
public:
    // Observer pattern notification method
    virtual void notify(const NotificationData& data) = 0;

    // Legacy print method for backward compatibility
    virtual void print(const Game &game, ostream &out) const = 0;

    virtual ~View() = default;
};

#endif 