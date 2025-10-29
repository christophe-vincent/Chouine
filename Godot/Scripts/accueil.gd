extends Node

@onready var UnPoint: TextureButton = $UnPoint
@onready var TroisPoints: TextureButton = $TroisPoints
@onready var CinqPoints: TextureButton = $CinqPoints

#var scene_partie = preload("res://Scenes/partie.tscn")

func _on_jouer_pressed() -> void:
	#get_tree().change_scene_to_packed(scene_partie)
	get_tree().change_scene_to_file("res://Scenes/partie.tscn")


func _on_un_point_pressed() -> void:
	Global.nb_points = 1
	UnPoint.disabled = true
	TroisPoints.disabled = false
	TroisPoints.set_pressed(false)
	CinqPoints.disabled = false
	CinqPoints.set_pressed(false)
	
	

func _on_trois_points_pressed() -> void:
	Global.nb_points = 3
	TroisPoints.disabled = true
	UnPoint.disabled = false
	UnPoint.set_pressed(false)
	CinqPoints.disabled = false
	CinqPoints.set_pressed(false)
	


func _on_cinq_points_2_pressed() -> void:
	Global.nb_points = 5
	CinqPoints.disabled = true
	TroisPoints.disabled = false
	TroisPoints.set_pressed(false)
	UnPoint.disabled = false
	UnPoint.set_pressed(false)
