/* date = May 10th 2021 10:32 pm */

#ifndef ENTITIES_H
#define ENTITIES_H

#include "player.h"
#include "enemy.h"

/* ?? Nest all of my entities in their own struct
and then nest the entities struct in game_t?? */
typedef struct ENTITIES_STRUCT {
    player_t player;
    enemy_t enemy;
} entities_t;

#endif //ENTITIES_H
