extends Area2D

# Declare member variables here. Examples:
# var a = 2
# var b = "text"

func _init(texture):
	._init()
	var scale = global.echelle_carte
	print(scale)
	var texture_carte                #the texture of the powerup will live here
	texture_carte = load(texture)
	var carte_sprite = Sprite.new()
	carte_sprite.set_texture(texture_carte)
	carte_sprite.set_scale(scale)
	var size = carte_sprite.texture.get_size()*scale
	add_child(carte_sprite)
	
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
	add_child(powerUpCollision)

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
