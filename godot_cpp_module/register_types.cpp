#include "register_types.h"
#include "core/engine.h"
#include "core/class_db.h"
#include "tile_light.h"
#include "matrix.h"

void register_light_demo_types() {
    ClassDB::register_class<TileLight>();
    ClassDB::register_class<Matrix>();
}

void unregister_light_demo_types() {
}