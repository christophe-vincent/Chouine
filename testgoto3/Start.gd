extends Button

# Declare member variables here. Examples:
# var a = 2
# var b = "text"

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.

# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass


func _on_Start_pressed():
	print("play")
	var jeu = load("res://Jeu.tscn")
	var node = jeu.instance()
	add_child(node)
	pass # Replace with function body.
