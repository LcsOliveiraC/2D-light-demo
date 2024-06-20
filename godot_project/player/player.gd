extends KinematicBody2D


var input_direction := Vector2.ZERO
var walk_acceleration: float = 480
var speed: float = 100
var air_speed_modifier: float = 1
var gravity_force: float = 800
var jump_force: float = 250
var velocity: Vector2 = Vector2.ZERO
var max_velocity: Vector2 = Vector2(500, 500)
var max_snap := Vector2(0, 4)
var snap := max_snap
var standing_gravity: float = 0.1
var last_slide := Vector2.ZERO

enum State {
	IDLE,
	WALKING,
	FALLING,
	JUMPING,
}
var state: int = State.IDLE setget set_state


func _physics_process(delta):
	handle_states(delta)
	velocity.x = clamp(velocity.x, -max_velocity.x, max_velocity.x)
	velocity.y = clamp(velocity.y, -max_velocity.y, max_velocity.y)
	last_slide = move_and_slide_with_snap(velocity, snap, Vector2.UP, true, 4, deg2rad(60))
	
	if Input.is_action_just_pressed("throw_flare"):
		throw_flare()


func throw_flare():
	var flare: RigidBody2D = load("res://flare/flare.tscn").instance()
	flare.global_position = global_position
	get_parent().add_child(flare)
	flare.apply_central_impulse(get_local_mouse_position() * 2)


func set_state(new_state: int):
	var old_state := state
	state = new_state
	if new_state == old_state:
		return
	
	match state:
		State.IDLE:
			velocity.y = standing_gravity
		State.WALKING:
			velocity.y = standing_gravity
		State.FALLING:
			velocity.y = 0
		State.JUMPING:
			velocity.y = -jump_force


func handle_states(delta: float):
	match state:
		State.IDLE:
			if not is_on_floor():
				set_state(State.FALLING)
				return
			if Input.is_action_just_pressed("jump"):
				set_state(State.JUMPING)
				return
			if input_direction.x != 0:
				set_state(State.WALKING)
				return
			input_direction.x = get_input_direction()
			velocity = last_slide
			velocity.x = move_toward(velocity.x, input_direction.x * speed, walk_acceleration * delta)
		
		State.WALKING:
			if not is_on_floor():
				set_state(State.FALLING)
				return
			if Input.is_action_just_pressed("jump"):
				set_state(State.JUMPING)
				return
			if input_direction.x == 0:
				set_state(State.IDLE)
				return
			velocity = last_slide
			input_direction.x = get_input_direction()
			velocity.x = move_toward(velocity.x, input_direction.x * speed, walk_acceleration * delta)
		
		State.FALLING:
			if is_on_floor():
				set_state(State.IDLE)
				return
			input_direction.x = get_input_direction()
			velocity.x = move_toward(velocity.x, input_direction.x * speed, walk_acceleration * delta)
			velocity.y += gravity_force * delta
		
		State.JUMPING:
			if velocity.y > 0:
				set_state(State.FALLING)
				return
			if is_on_ceiling():
				set_state(State.FALLING)
				return
			input_direction.x = get_input_direction()
			velocity.x = move_toward(velocity.x, input_direction.x * speed, walk_acceleration * delta)
			velocity.y += gravity_force * delta


func get_input_direction() -> float:
	var dir: float = Input.get_action_strength("move_right") - Input.get_action_strength("move_left")
	return dir


func _on_Hurtbox_damage_taken(amount: float, attacker: Node2D, knockback: float):
	var direction := Vector2.ZERO
	direction.x = sign(attacker.global_position.direction_to(global_position).x)
	velocity = direction * knockback + Vector2(0, -200)
