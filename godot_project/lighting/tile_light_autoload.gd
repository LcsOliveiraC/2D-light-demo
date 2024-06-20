extends Node


var tilemap: TileMap
var dynamic_light_list := Array() # List of TileDynamicLight


func add_dynamic_ligth(node: TileLightTorch):
	if not dynamic_light_list.has(node):
		dynamic_light_list.append(node)


func remove_dynamic_ligth(node: TileLightTorch):
	var index = dynamic_light_list.find(node)
	if index != -1:
		dynamic_light_list.remove(index)


func get_cells(_tilemap: TileMap) -> PoolVector2Array:
	var cells := PoolVector2Array()
	for node in dynamic_light_list:
		if node is TileLightTorch:
			cells.append(_tilemap.world_to_map(node.global_position))
	return cells


func get_intensities() -> PoolIntArray:
	var intensities := PoolIntArray()
	for node in dynamic_light_list:
		if node is TileLightTorch:
			intensities.append(node.light_range)
	return intensities
