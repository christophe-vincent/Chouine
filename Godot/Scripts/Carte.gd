extends Node2D

@onready var area: Area2D = $Area2D
@onready var front_face_texture: Sprite2D = $Area2D/FrontFace
#@onready var back_face_texture: Sprinte2D = $Area2D/BackFace
@onready var collision: CollisionShape2D= $Area2D/CollisionShape2D

## The name of the card.
@export var card_name: String
## The size of the card.
@export var card_size: Vector2 = Settings.DEFAULT_CARD_SIZE
## The texture for the front face of the card.
@export var front_image: Texture2D
## The texture for the back face of the card.
@export var back_image: Texture2D
## Whether the front face of the card is shown.
## If true, the front face is visible; otherwise, the back face is visible.
@export var show_front: bool = true:
	set(value):
		if value:
			front_face_texture.visible = true
			#back_face_texture.visible = false
		else:
			front_face_texture.visible = false
			#back_face_texture.visible = true


var dragging = false
var drag_offset = Vector2.ZERO
var zone_jeu = false


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	collision.shape.size = Settings.DEFAULT_CARD_SIZE
	front_face_texture.texture = front_image
	var texture_scale = Settings.DEFAULT_CARD_SIZE / front_image.get_size() 
	front_face_texture.scale = texture_scale
	area.mouse_entered.connect(_on_mouse_entered)
	area.mouse_exited.connect(_on_mouse_exited)
	area.input_event.connect(_on_input_event)
	area.position = position


func _on_input_event(_viewport, event, _shape_idx):
	if event is InputEventMouseButton:
		if event.button_index == MOUSE_BUTTON_LEFT:
			if event.pressed:
				# Start dragging
				dragging = true
				drag_offset = area.global_position - get_global_mouse_position()
			else:
				# Stop dragging
				dragging = false
				if zone_jeu == true:
					get_parent().carte_jouee(card_name)
					print("Courte jouée")


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta: float) -> void:
	if dragging:
		area.global_position = get_global_mouse_position() + drag_offset

func _on_mouse_entered():
	pass

func _on_mouse_exited():
	pass


func _on_area_2d_area_shape_entered(area_rid: RID, area: Area2D, area_shape_index: int, local_shape_index: int) -> void:
	if area.name == "ZoneJeu":
		zone_jeu = true


func _on_area_2d_area_shape_exited(area_rid: RID, area: Area2D, area_shape_index: int, local_shape_index: int) -> void:
	if area.name == "ZoneJeu":
		zone_jeu = false
