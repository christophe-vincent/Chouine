extends Node2D

# Declare member variables here. Examples:
# var a = 2
# var b = "text"
var cartes = []

# Called when the node enters the scene tree for the first time.
func _ready():
	var position = 100
	for i in 4:
		var carte = preload("carte.gd").new("res://textures/carreau-7.svg")
		carte.position = Vector2(position,100)
		position += 50
		carte.z_index = 4-i
		add_child(carte)
		cartes.append(carte)
		
	cartes[3].z_index = 10
	
	
	pass # Replace with function body.

# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
