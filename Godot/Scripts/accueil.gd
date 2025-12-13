extends Node

@onready var PointUn: PanelContainer = $Parties/Une
@onready var JetonUn: TextureRect = $Parties/Une/MarginContainer/VBoxContainer/ChoixPartie
@onready var PointTrois: PanelContainer = $Parties/Trois
@onready var JetonTrois: TextureRect = $Parties/Trois/MarginContainer/VBoxContainer/ChoixPartie
@onready var PointCinq: PanelContainer = $Parties/Cinq
@onready var JetonCinq: TextureRect = $Parties/Cinq/MarginContainer/VBoxContainer/ChoixPartie

@onready var MancheUne: PanelContainer = $Manches/Une
@onready var JetonMancheUn: TextureRect = $Manches/Une/MarginContainer/VBoxContainer/ChoixManche
@onready var MancheDeux: PanelContainer = $Manches/Deux
@onready var JetonMancheDeux: TextureRect = $Manches/Deux/MarginContainer/VBoxContainer/ChoixManche

var lut_points: Array[int] = [0, 0, 1, 1, 2, 2]
var points: Array[PanelContainer] = []
var jeton_points: Array[TextureRect] = []
var manches: Array[PanelContainer] = []
var jeton_manches: Array[TextureRect] = []
var scene_partie: Resource = preload("res://Scenes/partie.tscn")
var scene_regles: Resource = preload("res://Scenes/regles.tscn")
var scene_options: Resource = preload("res://Scenes/options.tscn")

func _ready() -> void:
	# lecture du fichier d'options, s'il existe
	Global.lecture_options()
	Global.lecture_stats()
	$Stats/Statistiques.update()
	points = [PointUn, PointTrois, PointCinq]
	jeton_points = [JetonUn, JetonTrois, JetonCinq]
	manches = [MancheUne, MancheDeux]
	jeton_manches = [JetonMancheUn, JetonMancheDeux]
	select_nb_points(Global.options["nb_points"])
	select_nb_manches(Global.options["nb_manches"])

	$Intro.text = Settings.TEXT_INTRO
	if FileAccess.file_exists(Settings.SAVE_FILE):
		$PartieEnCours.visible = true

func select_nb_points(nb_points: int) -> void:
	Global.options["nb_points"] = nb_points
	Global.enregistrer_options()
	for i: int in range(points.size()):
		# modifier le background
		if i == lut_points[nb_points]:
			points[i].add_theme_stylebox_override("panel", Global.selected_panel)
			jeton_points[i].texture = Global.jeton_actif
		else:
			points[i].add_theme_stylebox_override("panel", Global.non_selected_panel)
			jeton_points[i].texture = Global.jeton_inactif

func select_nb_manches(nb_manches: int) -> void:
	Global.options["nb_manches"] = nb_manches
	Global.enregistrer_options()
	for i: int in range(manches.size()):
		# modifier le background
		if i == nb_manches - 1:
			manches[i].add_theme_stylebox_override("panel", Global.selected_panel)
			jeton_manches[i].texture = Global.jeton_actif
		else:
			manches[i].add_theme_stylebox_override("panel", Global.non_selected_panel)
			jeton_manches[i].texture = Global.jeton_inactif


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


func _on_un_point_pressed() -> void:
	select_nb_points(1)

func _on_trois_points_pressed() -> void:
	select_nb_points(3)

func _on_cinq_points_pressed() -> void:
	select_nb_points(5)

func _on_une_manche_pressed() -> void:
	select_nb_manches(1)

func _on_deux_manches_pressed() -> void:
	select_nb_manches(2)


func _on_stats_folding_changed(is_folded: bool) -> void:
	if not is_folded:
		$Stats/Statistiques.update()
