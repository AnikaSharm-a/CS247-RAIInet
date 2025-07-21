#include "controller.h"
#include <iostream>
using namespace std;

/*
 * updated main function to take argument "-graphics" for part b.
 * If -graphics argument is provided, forward this to the controller
 */
int main (int argc, char *argv[]) {
    bool useGraphics = (argc == 2 && string(argv[1]) == "-graphics");
    Controller c{useGraphics};
    c.play();
}
