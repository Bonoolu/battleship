#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <stddef.h>

/**
 * @brief The Configuration struct. Contains settings like amount of ships or a setting to enable cheats
 */
struct Configuration {
    static const size_t minShipLength = 2;
    static const size_t maxShipLength = 5;
    static constexpr size_t amountShips[maxShipLength] {0, 4, 3, 2, 1};
    static const size_t gridSize = 10;
    static const bool cheats = false; // true: reveals all enemy ships
};

#endif // CONFIGURATION_H
