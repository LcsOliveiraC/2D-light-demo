#ifndef TILE_LIGHT_H
#define TILE_LIGHT_H

#include "core/reference.h"
#include "scene/2d/tile_map.h"
#include "scene/main/node.h"
#include "scene/resources/texture.h"
#include "matrix.h"


class TileLight : public Reference {
    GDCLASS(TileLight, Reference);
    
protected:
    static void _bind_methods();

public:
    Ref<Matrix> cell_id_matrix;
    Ref<Matrix> light_matrix;
    Ref<Matrix> opacity_matrix;

    void set_texture(const Ref<Texture> &p_texture);
    Ref<Texture> get_texture();
    void set_light_matrix(const Ref<Matrix> &matrix);
    Ref<Matrix> get_light_matrix();
    void set_opacity_matrix(const Ref<Matrix> &matrix);
    Ref<Matrix> get_opacity_matrix();

    Ref<Texture> get_light_texture(Texture::Flags flags, Image::Format format);
    void flood_fill_recursive(int x, int y, int target_value);
    void setup_matrices(Rect2 visible_cell_rect, Array light_cell_nodes, const Node* p_tilemap, int air_opacity, int block_opacity);
    void propagate_lights(PoolVector2Array light_cells, PoolIntArray light_intensities, Rect2 visible_cell_rect);
    Ref<Matrix> get_visible_cell_id_matrix(Rect2 visible_rect, Vector2 extra_border_cells, Node* p_tilemap);
    TileLight();
};

#endif // TILE_LIGHT_H

