extends Node2D
class_name Carte

@onready var carte: Area2D = $Area2D
@onready var front_face_texture: Sprite2D = $Area2D/FrontFace
@onready var back_face_texture: Sprite2D = $Area2D/BackFace
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
@export var show_front: bool = true

var draggable = true
var dragging = false
var moving = false
var drag_offset = Vector2.ZERO
var zone_jeu = false
var main_joueur = false
var position_initiale = Vector2(0, 0)


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	collision.shape.size = Settings.DEFAULT_CARD_SIZE
	front_face_texture.texture = front_image
	back_face_texture.texture = back_image
	var texture_scale = Settings.DEFAULT_CARD_SIZE / front_image.get_size() 
	front_face_texture.scale = texture_scale
	back_face_texture.scale = texture_scale
	if show_front:
		front_face_texture.visible = true
		back_face_texture.visible = false
	else:
		front_face_texture.visible = false
		back_face_texture.visible = true
	carte.mouse_entered.connect(_on_mouse_entered)
	carte.mouse_exited.connect(_on_mouse_exited)
	carte.input_event.connect(_on_input_event)
	carte.position = position

func move(pos, duree_effet):
	moving = true
	var tween = get_tree().create_tween()
	tween.tween_property(carte, "position", pos, duree_effet)
	tween.connect("finished", stop_moving)
	
func stop_moving():
	moving = false

func size():
	return card_size
	
func face_visible(v):
	if v:
		front_face_texture.visible = true
		back_face_texture.visible = false
	else:
		front_face_texture.visible = false
		back_face_texture.visible = true

func _on_input_event(_viewport, event, _shape_idx):
	if draggable and (not moving) and event is InputEventMouseButton:
		if event.button_index == MOUSE_BUTTON_LEFT:
			if event.pressed:
				# Start dragging
				dragging = true
				position_initiale = carte.position
				drag_offset = carte.global_position - get_global_mouse_position()
			else:
				# Stop dragging
				dragging = false
				var ret: int = 1
				if zone_jeu == true:
					ret = get_parent().carte_jouee(card_name)
				elif main_joueur == true:
					ret = get_parent().carte_main_joueur(card_name)
				if ret != 0:
					# on retourne à la position initiale
					carte.position = position_initiale


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta: float) -> void:
	if dragging:
		carte.global_position = get_global_mouse_position() + drag_offset

func _on_mouse_entered():
	pass

func _on_mouse_exited():
	pass


func _on_area_2d_area_shape_entered(_area_rid: RID, area: Area2D, _area_shape_index: int, _local_shape_index: int) -> void:
	if area.name == "ZoneJeu":
		zone_jeu = true
	elif area.name == "MainJoueur":
		main_joueur = true


func _on_area_2d_area_shape_exited(_area_rid: RID, area: Area2D, _area_shape_index: int, _local_shape_index: int) -> void:
	if area.name == "ZoneJeu":
		zone_jeu = false
	elif area.name == "MainJoueur":
		main_joueur = false
