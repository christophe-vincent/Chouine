extends Node

@onready var UnPoint: TextureButton = $UnPoint
@onready var TroisPoints: TextureButton = $TroisPoints
@onready var CinqPoints: TextureButton = $CinqPoints
@onready var UneManche: TextureButton = $UneManche
@onready var DeuxManches: TextureButton = $DeuxManches

var scene_partie: Resource = preload("res://Scenes/partie.tscn")
var scene_regles: Resource = preload("res://Scenes/regles.tscn")
var scene_options: Resource = preload("res://Scenes/options.tscn")

func _ready() -> void:
	# lecture du fichier d'options, s'il existe
	Global.lecture_options()

	if Global.options["nb_points"] == 1:
		_on_un_point_pressed()
	if Global.options["nb_points"] == 5:
		_on_cinq_points_2_pressed()
	if Global.options["nb_manches"] == 2:
		_on_deux_manches_pressed()
	$Intro.text = Settings.TEXT_INTRO
	if FileAccess.file_exists(Settings.SAVE_FILE):
		$PartieEnCours.visible = true


func _on_un_point_pressed() -> void:
	Global.options["nb_points"] = 1
	UnPoint.disabled = true
	TroisPoints.disabled = false
	TroisPoints.set_pressed(false)
	CinqPoints.disabled = false
	CinqPoints.set_pressed(false)
	Global.enregistrer_options()


func _on_trois_points_pressed() -> void:
	Global.options["nb_points"] = 3
	TroisPoints.disabled = true
	UnPoint.disabled = false
	UnPoint.set_pressed(false)
	CinqPoints.disabled = false
	CinqPoints.set_pressed(false)
	Global.enregistrer_options()


func _on_cinq_points_2_pressed() -> void:
	Global.options["nb_points"] = 5
	CinqPoints.disabled = true
	TroisPoints.disabled = false
	TroisPoints.set_pressed(false)
	UnPoint.disabled = false
	UnPoint.set_pressed(false)
	Global.enregistrer_options()


func _on_une_manche_pressed() -> void:
	Global.options["nb_manches"] = 1
	UneManche.disabled = true
	DeuxManches.disabled = false
	DeuxManches.set_pressed(false)
	Global.enregistrer_options()


func _on_deux_manches_pressed() -> void:
	Global.options["nb_manches"] = 2
	DeuxManches.disabled = true
	UneManche.disabled = false
	UneManche.set_pressed(false)
	Global.enregistrer_options()


func _on_partie_en_cours_pressed() -> void:
	Global.help_mode = false
	get_tree().change_scene_to_packed(scene_partie)


func _on_jouer_pressed() -> void:
	# la partie en cours est supprimée
	if FileAccess.file_exists(Settings.SAVE_FILE):
		DirAccess.remove_absolute(Settings.SAVE_FILE)
	Global.help_mode = false
	get_tree().change_scene_to_packed(scene_partie)


func _on_regles_pressed() -> void:
	get_tree().change_scene_to_packed(scene_regles)


func _on_aide_pressed() -> void:
	Global.help_mode = true
	get_tree().change_scene_to_packed(scene_partie)


func _on_cartes_visibles_pressed() -> void:
	Global.enregistrer_options()


func _on_options_pressed() -> void:
	get_tree().change_scene_to_packed(scene_options)
