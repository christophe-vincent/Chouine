extends Area2D


# Declare member variables here. Examples:
# var a = 2
# var b = "text"

# Called when the node enters the scene tree for the first time.
func _ready():
	print("new carte")
	pass # Replace with function body.
      
var lifted = false

func _unhandled_input(event):
    if event is InputEventMouseButton and not event.pressed:
        lifted = false
        #print("click")
    if lifted and event is InputEventMouseMotion:
        position += event.relative

func _input_event(viewport, event, shape_idx):
	#print(event)
	if event is InputEventMouseButton:
		if event.button_index == BUTTON_LEFT \
	    and event.pressed:
	        print("Clicked")
	if event is InputEventMouseButton and event.pressed:
		lifted = true


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
