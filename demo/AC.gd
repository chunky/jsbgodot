extends Node


func _input(event):
	if Input.is_action_pressed("ui_right"):
		$JSBSimSpatial.input_roll = -1
	elif Input.is_action_pressed("ui_left"):
		$JSBSimSpatial.input_roll = 1
	else:
		$JSBSimSpatial.input_roll = 0
	
	if Input.is_action_pressed("ui_down"):
		$JSBSimSpatial.input_pitch = -1
	elif Input.is_action_pressed("ui_up"):
		$JSBSimSpatial.input_pitch = 1
	else:
		$JSBSimSpatial.input_pitch = 0
		
	if event is InputEventKey:
		if event.scancode == KEY_A:
			$JSBSimSpatial.input_throttle += 0.1
		elif event.scancode == KEY_Z:
			$JSBSimSpatial.input_throttle -= 0.1
		$JSBSimSpatial.input_throttle = clamp($JSBSimSpatial.input_throttle, 0.0, 1.0)
		
		if event.scancode == KEY_ESCAPE:
			get_tree().quit(0)


# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
