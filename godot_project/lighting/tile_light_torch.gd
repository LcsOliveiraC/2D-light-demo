extends Node2D
class_name TileLightTorch


export(int, 0, 32) var light_range: int = 32
var last_cell := Vector2.ZERO


func _ready():
	TileLightAutoload.add_dynamic_ligth(self)


func _process(delta):
	pass


func _exit_tree():
	TileLightAutoload.remove_dynamic_ligth(self)
