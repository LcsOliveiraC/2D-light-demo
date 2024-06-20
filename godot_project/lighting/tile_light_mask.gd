extends TextureRect
class_name TileLightMask


export var tilemap_path: NodePath
var extra_border_tiles := Vector2(32, 32)
var last_cell_rect := Rect2(0,0,0,0)
var update_treshhold: float = 0.5 # Number of tiles that any corner of the
# screen needs to move in order to call for an update to the texture
var thread: Thread


func _ready():
	initialize_texture()


func _process(delta):
	var _tilemap: TileMap = get_node(tilemap_path)
	if not is_instance_valid(thread):
		thread = Thread.new()
	
	var new_cell_rect := get_visible_cell_rect(_tilemap)
	if (new_cell_rect.position.distance_to(last_cell_rect.position) > update_treshhold or
			new_cell_rect.end.distance_to(last_cell_rect.end) > update_treshhold):
		# TODO: also update when objects (flares) move
		
		if not thread.is_alive() and not thread.is_active():
			var dict := {
				cells = TileLightAutoload.get_cells(_tilemap),
				intensities = TileLightAutoload.get_intensities(),
				visible_cell_rect = get_visible_cell_rect(_tilemap, extra_border_tiles),
				tilemap = _tilemap,
				}
			thread.start(self, "update_light", dict)
			last_cell_rect = new_cell_rect


func update_light(dict: Dictionary):
	var tile_light := TileLight.new()
	tile_light.setup_matrices(dict.visible_cell_rect, [], dict.tilemap, 1, 6)
	tile_light.propagate_lights(dict.cells, dict.intensities, dict.visible_cell_rect)
	
	var _texture := tile_light.get_light_texture(Texture.FLAG_FILTER, Image.FORMAT_RGBA8)
	call_deferred("fit_texture", dict.visible_cell_rect, _texture)
	thread.call_deferred("wait_to_finish")


func fit_texture(visible_cell_rect: Rect2, _texture: Texture):
	var _tilemap: TileMap = get_node(tilemap_path)
	texture = _texture
	rect_size = _tilemap.map_to_world(visible_cell_rect.size)
	rect_global_position = _tilemap.map_to_world(visible_cell_rect.position)


func get_visible_rect() -> Rect2:
	var vp_rect := get_viewport_rect()
	var vp_t := get_viewport_transform()
	vp_rect.position = -vp_t.get_origin() / vp_t.get_scale()
	vp_rect.size /= vp_t.get_scale()
	return vp_rect


func get_visible_cell_rect(_tilemap: TileMap, _extra_border_cells: Vector2 = Vector2.ZERO) -> Rect2:
	var vp_rect := get_viewport_rect()
	var vp_t := get_viewport_transform()
	vp_rect.position = -vp_t.get_origin() / vp_t.get_scale()
	vp_rect.size /= vp_t.get_scale()
	
	var cell_rect := Rect2()
	cell_rect.position = _tilemap.world_to_map(vp_rect.position) - (_extra_border_cells / 2).floor()
	cell_rect.size = _tilemap.world_to_map(vp_rect.size) + _extra_border_cells
	return cell_rect


func initialize_texture():
	var txt := ImageTexture.new()
	var img := Image.new()
	img.create(4, 4, false, Image.FORMAT_RGBA8)
	txt.create_from_image(img, 0)
	texture = txt


func _exit_tree():
	if thread is Thread:
		if thread.is_active():
			thread.wait_to_finish()
