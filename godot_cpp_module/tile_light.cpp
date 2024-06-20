#include "tile_light.h"

void TileLight::set_light_matrix(const Ref<Matrix> &matrix) {
    light_matrix = matrix;
}

Ref<Matrix> TileLight::get_light_matrix() {
    return light_matrix;
}

void TileLight::set_opacity_matrix(const Ref<Matrix> &matrix) {
    opacity_matrix = matrix;
}

Ref<Matrix> TileLight::get_opacity_matrix() {
    return opacity_matrix;
}

Ref<Texture> TileLight::get_light_texture(Texture::Flags flags, Image::Format format) {
    Ref<ImageTexture> texture = Ref<ImageTexture>(memnew(ImageTexture));
    Ref<Image> img = Ref<Image>(memnew(Image));
    img->create(light_matrix->get_region().size.x, light_matrix->get_region().size.y, false, format);
	img->lock();
	for (int x = 0; x < light_matrix->get_region().size.x; x++) {
        for (int y = 0; y < light_matrix->get_region().size.y; y++) {
            float light_value = float(light_matrix->get_value(x, y));
            light_value = int(-(light_value-32));
            light_value /= 32.0;

			Color color = Color(0, 0, 0, light_value);
			img->set_pixel(x, y, color);
        }
    }	
	img->unlock();
    texture->create_from_image(img, flags);
    return texture;
}

void TileLight::flood_fill_recursive(int x, int y, int target_value) {
    if (target_value <= 0) {return;}
    if (x < 0) return;
    if (y < 0) return;
    if (x >= light_matrix->get_region().size.x) return;
    if (y >= light_matrix->get_region().size.y) return;

	target_value -= opacity_matrix->get_value(x, y);
	if (light_matrix->get_value(x, y) >= target_value) return;
	
	light_matrix->set_value(target_value, x, y);
	flood_fill_recursive(x+1, y  , target_value);
	flood_fill_recursive(x-1, y  , target_value);
	flood_fill_recursive(x  , y+1, target_value);
	flood_fill_recursive(x  , y-1, target_value);
}

void TileLight::setup_matrices(Rect2 visible_cell_rect, Array light_cell_nodes, const Node* p_tilemap, int air_opacity, int block_opacity){
    light_matrix = Ref<Matrix>(memnew(Matrix));
    opacity_matrix = Ref<Matrix>(memnew(Matrix));
    light_matrix->set_region( Rect2(Vector2(0, 0), visible_cell_rect.size) );
	opacity_matrix->set_region( Rect2(Vector2(0, 0), visible_cell_rect.size) );
    const TileMap *tilemap = Object::cast_to<TileMap>(p_tilemap);
    
	for (int x = 0; x < visible_cell_rect.size.x; x++) {
        for (int y = 0; y < visible_cell_rect.size.y; y++) {
            Vector2 cell = Vector2(x, y) + (visible_cell_rect.position);
			int id = tilemap->get_cellv(cell);
			light_matrix->set_value(0, x, y);
			if (id == -1) opacity_matrix->set_value(air_opacity, x, y);
			else if (id >= 0) opacity_matrix->set_value(block_opacity, x, y);
        }
    }
}

void TileLight::propagate_lights(PoolVector2Array light_cells, PoolIntArray light_intensities, Rect2 visible_cell_rect) {
    for (int i = 0; i < light_cells.size(); i++) {
        Vector2 cell = light_cells[i];
        int intensity = light_intensities[i];
        if (visible_cell_rect.has_point(cell)){
			Vector2 matrix_coord = cell - visible_cell_rect.position;
			flood_fill_recursive(matrix_coord.x, matrix_coord.y, intensity);
        }
    }
}

Ref<Matrix> TileLight::get_visible_cell_id_matrix(Rect2 visible_rect, Vector2 extra_border_cells, Node* p_tilemap) {
    TileMap *tilemap = Object::cast_to<TileMap>(p_tilemap);
    if (tilemap) {}
    Rect2 visible_cell_rect;
	visible_cell_rect.position = tilemap->world_to_map(visible_rect.position) - (extra_border_cells / 2).floor();
	visible_cell_rect.size = tilemap->world_to_map(visible_rect.size) + extra_border_cells;

    Ref<Matrix> visible_cell_id_matrix = Ref<Matrix>(memnew(Matrix));
    visible_cell_id_matrix->set_region( Rect2(Vector2(0, 0), visible_cell_rect.size) );
    for (int x = 0; x < visible_cell_rect.size.x; x++) {
        for (int y = 0; y < visible_cell_rect.size.y; y++) {
            Vector2 cell = Vector2(x, y) + visible_cell_rect.position;
			int id = tilemap->get_cellv(cell);
			visible_cell_id_matrix->set_value(id, x, y);
        }
    }
    return visible_cell_id_matrix;
}

void TileLight::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_light_matrix", "matrix"), &TileLight::set_light_matrix);
    ClassDB::bind_method(D_METHOD("get_light_matrix"), &TileLight::get_light_matrix);
    ClassDB::bind_method(D_METHOD("set_opacity_matrix", "matrix"), &TileLight::set_opacity_matrix);
    ClassDB::bind_method(D_METHOD("get_opacity_matrix"), &TileLight::get_opacity_matrix);

    ClassDB::bind_method(D_METHOD("get_light_texture", "flags", "format"), &TileLight::get_light_texture);
    ClassDB::bind_method(D_METHOD("flood_fill_recursive", "x", "y", "target_value"), &TileLight::flood_fill_recursive);
    ClassDB::bind_method(D_METHOD("setup_matrices", "visible_cell_rect", "light_cell_nodes", "tilemap", "air_opacity", "block_opacity"), &TileLight::setup_matrices);
    ClassDB::bind_method(D_METHOD("propagate_lights", "light_cell_array", "light_intensity_array", "visible_cell_rect"), &TileLight::propagate_lights);
    ClassDB::bind_method(D_METHOD("get_visible_cell_id_matrix", "visible_rect", "extra_border_cells", "tilemap"), &TileLight::get_visible_cell_id_matrix);
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "light_matrix", PROPERTY_HINT_RESOURCE_TYPE, "Matrix"), "set_light_matrix", "get_light_matrix");
    ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "opacity_matrix", PROPERTY_HINT_RESOURCE_TYPE, "Matrix"), "set_opacity_matrix", "get_opacity_matrix");
}

TileLight::TileLight() {
}