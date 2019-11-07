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
	
	var texture_carte                #the texture of the powerup will live here
	texture_carte = load("res://textures/carreau-7.svg")
	var carte_sprite = Sprite.new()
	carte_sprite.set_texture(texture_carte)
	carte_sprite.set_scale(scale)
	var size = carte_sprite.texture.get_size()*scale
	
	carte.add_child(carte_sprite)
	
	#Create the collision shape
	var shape = RectangleShape2D.new()
	#resize the shape to be the same size as the sprite texture
	shape.set_extents(Vector2(carte_sprite.get_texture().get_width()*scale[0]/2,
							  carte_sprite.get_texture().get_height()*scale[1]/2))    
	#create the collission object
	var powerUpCollision = CollisionShape2D.new()
	#attach the shape to the object
	powerUpCollision.set_shape(shape)
	#add it as a child
	carte.add_child(powerUpCollision)
	
	add_child(carte)
	carte.position = Vector2(100,100)
	
	pass # Replace with function body.

# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass
