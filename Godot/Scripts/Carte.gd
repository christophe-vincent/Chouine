"""
Copyright 2025,2026 Christophe Vincent
This file is part of La Chouine.
La Chouine is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or any later version.
La Chouine is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with Foobar. If not, see <https://www.gnu.org/licenses/>
"""
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

enum State {IDLE, DRAGGING, MOVING, SELECTED}

var draggable: bool = true
var state: State = State.IDLE
var drag_offset: Vector2 = Vector2.ZERO
var zone_jeu: bool = false
var zone_pioche: bool = false
var main_joueur: bool = false
var position_initiale: Vector2 = Vector2(0, 0)
var orientation_initiale: float = 0
var card_z_index: int = 0
var screen_center: Vector2 = Vector2(0, 0)
var move_tween: Tween
var sept_atout: bool = false
var retourne: bool = false


# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	screen_center = get_viewport_rect().size / 2.0
	collision.shape.size = Settings.DEFAULT_CARD_SIZE
	front_face_texture.texture = front_image
	back_face_texture.texture = back_image
	var texture_scale: Vector2 = Settings.DEFAULT_CARD_SIZE / front_image.get_size() 
	front_face_texture.scale = texture_scale
	texture_scale = Settings.DEFAULT_CARD_SIZE / back_image.get_size() 
	back_face_texture.scale = texture_scale
	if show_front:
		front_face_texture.visible = true
		back_face_texture.visible = false
	else:
		front_face_texture.visible = false
		back_face_texture.visible = true
	if not carte.input_event.is_connected(_on_input_event):
		carte.input_event.connect(_on_input_event)
	carte.position = screen_center

func move(pos: Vector2, orientation: float=0, duree_effet: float=0, _z_index: int=0) -> void:
	if move_tween:
			move_tween.kill()
	if duree_effet == 0.0:
		carte.position = pos
		carte.rotation = orientation
	else:
		card_z_index = _z_index
		if card_z_index != 0: 
			carte.z_index = card_z_index
		position_initiale = pos
		orientation_initiale = orientation
		state = State.MOVING
		move_tween = get_tree().create_tween()
		move_tween.tween_property(carte, "position", pos, duree_effet).set_ease(Tween.EASE_IN_OUT).set_trans(Tween.TRANS_SINE)
		if orientation != -100:
			move_tween.set_parallel()
			move_tween.tween_property(carte, "rotation", orientation, duree_effet/8).set_ease(Tween.EASE_IN_OUT).set_trans(Tween.TRANS_SINE)
		move_tween.connect("finished", stop_moving)

func stop_moving() -> void:
	move_tween = null
	state = State.IDLE
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


func unselect() -> void:
	var tween: Tween
	tween = get_tree().create_tween()
	tween.tween_property(carte, "scale", Vector2(1.0, 1.0), 0.2)
	carte.z_index = card_z_index
	state = State.IDLE

func _on_input_event(_viewport: Viewport, event: InputEvent, _shape_idx: int) -> void:
	if draggable and (state != State.MOVING) and event is InputEventMouseButton:
		if event.button_index == MOUSE_BUTTON_LEFT:
			if event.pressed and is_topmost_card_at_position(get_global_mouse_position()) and state == State.IDLE:
				# Start dragging
				state = State.DRAGGING
				carte.z_index = 1000
				position_initiale = carte.position
				drag_offset = carte.global_position - get_global_mouse_position()
			else:
				# Stop dragging
				var ret: int = 1
				if zone_pioche == true:
					ret = get_parent().pose_sept_atout(card_name)
					carte.z_index = card_z_index
				if zone_jeu == true:
					ret = await get_parent().carte_jouee(card_name, 0.1)
					carte.z_index = card_z_index
				elif main_joueur == true:
					ret = get_parent().prise_carte_atout(card_name)
					carte.z_index = card_z_index
				if ret != 0:
					if carte.position.distance_to(position_initiale) < 20 and is_topmost_card_at_position(get_global_mouse_position()):
						if state == State.SELECTED:
							var tween: Tween
							tween = get_tree().create_tween()
							tween.tween_property(carte, "scale", Vector2(1.0, 1.0), 0.2)
							state = State.IDLE
							carte.z_index = card_z_index
							get_parent().carte_jouee(card_name)
							return
						if state == State.DRAGGING:
							var tween: Tween
							tween = get_tree().create_tween()
							tween.tween_property(carte, "scale", Vector2(1.2, 1.2), 0.2)
							state = State.SELECTED
							get_tree().create_timer(2.0).timeout.connect(unselect)
							get_parent().selection_carte(card_name)
					else:
						carte.z_index = card_z_index
					# retourne à la position initiale
					carte.position = position_initiale
					carte.rotation = orientation_initiale
				if state != State.SELECTED:
					state = State.IDLE


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta: float) -> void:
	if state == State.DRAGGING:
		if get_global_mouse_position().y > screen_center.y and not retourne and position_initiale.y != screen_center.y:
			carte.rotation = orientation_initiale*(get_global_mouse_position().y - screen_center.y)/(position_initiale.y-screen_center.y)
		carte.global_position = get_global_mouse_position() + drag_offset


func _on_area_2d_area_shape_entered(_area_rid: RID, area: Area2D, _area_shape_index: int, _local_shape_index: int) -> void:
	if state != State.DRAGGING:
		return
	if area.name == "ZoneJeu":
		zone_jeu = true
		zone_pioche = false
		get_parent().zone_pioche_actif(false)
	elif area.name == "ZonePioche" and sept_atout:
		zone_pioche = true
		zone_jeu = false
		get_parent().zone_pioche_actif(true)
	elif area.name == "MainJoueur":
		main_joueur = true


func _on_area_2d_area_shape_exited(_area_rid: RID, area: Area2D, _area_shape_index: int, _local_shape_index: int) -> void:
	if area.name == "ZoneJeu":
		zone_jeu = false
	elif area.name == "ZonePioche":
		zone_pioche = false
		get_parent().zone_pioche_actif(false)
	elif area.name == "MainJoueur":
		main_joueur = false
