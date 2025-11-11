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

var draggable: bool = true
var dragging: bool = false
var moving: bool = false
var drag_offset: Vector2 = Vector2.ZERO
var zone_jeu: bool = false
var main_joueur: bool = false
var position_initiale: Vector2 = Vector2(0, 0)
var orientation_initiale: float = 0
var card_z_index: int = 0
var screen_center: Vector2 = Vector2(0, 0)
var move_tween: Tween


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	screen_center = get_viewport_rect().size / 2.0
	collision.shape.size = Settings.DEFAULT_CARD_SIZE
	front_face_texture.texture = front_image
	back_face_texture.texture = back_image
	var texture_scale: Vector2 = Settings.DEFAULT_CARD_SIZE / front_image.get_size() 
	front_face_texture.scale = texture_scale
	back_face_texture.scale = texture_scale
	if show_front:
		front_face_texture.visible = true
		back_face_texture.visible = false
	else:
		front_face_texture.visible = false
		back_face_texture.visible = true
	carte.input_event.connect(_on_input_event)
	carte.position = position
	screen_center = get_viewport_rect().size / 2.0

func move(pos: Vector2, orientation: float=0, duree_effet: float=0, _z_index: int=0) -> void:
	moving = true
	card_z_index = _z_index
	if card_z_index != 0: 
		carte.z_index = card_z_index
	position_initiale = pos
	orientation_initiale = orientation
	if move_tween:
		move_tween.kill()
	move_tween = get_tree().create_tween()
	move_tween.tween_property(carte, "position", pos, duree_effet)
	move_tween.connect("finished", stop_moving)
	if orientation != -100:
		move_tween.tween_property(carte, "rotation", orientation, duree_effet)

func stop_moving() -> void:
	move_tween = null
	moving = false
	if card_z_index != 0: 
		carte.z_index = card_z_index

func size() -> Vector2:
	return card_size
	
func face_visible(v: bool) -> void:
	if v:
		front_face_texture.visible = true
		back_face_texture.visible = false
	else:
		front_face_texture.visible = false
		back_face_texture.visible = true


func is_topmost_card_at_position(pos: Vector2) -> bool:
	var space_state: PhysicsDirectSpaceState2D = get_world_2d().direct_space_state
	var query: PhysicsPointQueryParameters2D = PhysicsPointQueryParameters2D.new()
	query.position = pos
	query.collide_with_areas = true
	query.collide_with_bodies = false
	
	var results: Array[Dictionary] = space_state.intersect_point(query)
	
	# Find the highest z_index among all cards at this position
	var highest_z: int = carte.z_index
	var is_topmost: bool = true
	
	for result: Dictionary in results:
		var collider: CollisionObject2D = result.collider
		# Check if it's a card's Area2D
		if collider != carte and collider.get_parent() is Carte:
			if collider.z_index > highest_z:
				is_topmost = false
				break
	
	return is_topmost


func _on_input_event(_viewport: Viewport, event: InputEvent, _shape_idx: int) -> void:
	if draggable and (not moving) and event is InputEventMouseButton:
		if event.button_index == MOUSE_BUTTON_LEFT and is_topmost_card_at_position(get_global_mouse_position()):
			if event.pressed:
				# Start dragging
				dragging = true
				carte.z_index = 1000
				position_initiale = carte.position
				drag_offset = carte.global_position - get_global_mouse_position()
			else:
				# Stop dragging
				dragging = false
				carte.z_index = card_z_index
				var ret: int = 1
				if zone_jeu == true:
					ret = get_parent().carte_jouee(card_name)
				elif main_joueur == true:
					ret = get_parent().carte_main_joueur(card_name)
				if ret != 0:
					# on retourne à la position initiale
					carte.position = position_initiale
					carte.rotation = orientation_initiale


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta: float) -> void:
	if dragging:
		if get_global_mouse_position().y > screen_center.y:
			carte.rotation = orientation_initiale*(get_global_mouse_position().y - screen_center.y)/(position_initiale.y-screen_center.y)
		carte.global_position = get_global_mouse_position() + drag_offset


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
