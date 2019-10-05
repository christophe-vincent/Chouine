extends Node2D

# Declare member variables here. Examples:
# var a = 2
# var b = "text"

# Called when the node enters the scene tree for the first time.
func _ready():
	print("jeu")
	var scale = Vector2(0.25, 0.25)
    
	var carte = Area2D.new()
	carte.set_script(load("res://carte.gd"))
	    
	var powerUpImage                #the texture of the powerup will live here
	powerUpImage = load("res://textures/carreau-7.svg")
	var carte_img = Sprite.new()
	carte_img.set_texture(powerUpImage)
	carte_img.set_scale(scale)
	var size = carte_img.texture.get_size()*scale
	
	carte.position = Vector2(100,100)
	carte.
	carte.add_child(carte_img)
	add_child(carte)
	
	pass # Replace with function body.

# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
