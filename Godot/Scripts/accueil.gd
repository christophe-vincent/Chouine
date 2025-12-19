"""
Copyright 2025,2026 Christophe Vincent
This file is part of La Chouine.
La Chouine is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or any later version.
La Chouine is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with Foobar. If not, see <https://www.gnu.org/licenses/>
"""

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
var scene_regles: Resource = preload("res://Scenes/texte.tscn")
var scene_options: Resource = preload("res://Scenes/options.tscn")
var alt_api: bool = false


func _ready() -> void:
	# lecture du fichier d'options, s'il existe
	get_info_chouine()
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
		$PlayButtons/PartieEnCours.visible = true

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
	Global.texte = Global.TypeTexte.REGLES
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


func _on_credits_pressed() -> void:
	Global.texte = Global.TypeTexte.CREDITS
	get_tree().change_scene_to_packed(scene_regles)

func get_info_chouine() -> void:
	if Global.chouine_infos.size() > 0:
		# on a deja récupérer les infos
		_mise_a_jour_infos()
		return
	$HTTPRequest.request_completed.connect(_on_request_completed)
	var url:String = ""
	if alt_api:
		url = Global.api_config["alt"]["url"]
	else:
		url = Global.api_config["main"]["url"]
	$HTTPRequest.request(url + "/info")


func _on_request_completed(_result: int,
						response_code: int,
						_headers: PackedStringArray,
						body: PackedByteArray) -> void:
	var json: Variant = JSON.parse_string(body.get_string_from_utf8())
	if response_code != 200:
		if not alt_api:
			alt_api = true
			get_info_chouine()
		return
	# classement des infos
	for info: Variant in json:
		if not info.has("Id"):
			return
		Global.chouine_infos[info["Id"]["N"]] = {
			"Enabled": info["Enabled"]["BOOL"],
			"Text": info["Text"]["S"]
		}
		if "StartDate" in info:
			Global.chouine_infos[info["Id"]["N"]]["StartDate"] = info["StartDate"]["S"]
		if "EndDate" in info:
			Global.chouine_infos[info["Id"]["N"]]["EndDate"] = info["EndDate"]["S"]
		if "Version" in info:
			Global.chouine_infos[info["Id"]["N"]]["Version"] = info["Version"]["S"]
	_mise_a_jour_infos()

func _mise_a_jour_infos() -> void:
	# infos principales
	for i: int in range(0, 2):
		if Global.chouine_infos.has(str(i)):
			if not Global.chouine_infos[str(i)]["Enabled"]:
				continue
			if Global.chouine_infos[str(i)]["Text"] == "":
				continue
			if Global.chouine_infos[str(i)].has("StartDate") and Global.chouine_infos[str(i)].has("EndDate"):
				var start: int = Time.get_unix_time_from_datetime_string(Global.chouine_infos[str(i)]["StartDate"])
				var end: int = Time.get_unix_time_from_datetime_string(Global.chouine_infos[str(i)]["EndDate"])
				var now: float = Time.get_unix_time_from_system()
				if now < start or now > end:
					continue
			$Intro.text = Global.chouine_infos[str(i)]["Text"]
		break

	# vérification de la version
	if Global.chouine_infos.has("100") and Global.chouine_infos["100"].has("Enabled") and Global.chouine_infos["100"]["Enabled"]:
		var new_version: Dictionary = Global.chouine_infos["100"]
		var start: int = Time.get_unix_time_from_datetime_string(new_version["StartDate"])
		var end: int = Time.get_unix_time_from_datetime_string(new_version["EndDate"])
		var now: float = Time.get_unix_time_from_system()
		if now > start and now < end and _is_version_greater(new_version["Version"], Version.get_version()):
			$NouvelleVersion.text = new_version["Text"]
	

func _is_version_greater(version1: String, version2: String) -> bool:
	var v1_parts: Array = version1.split(".")
	var v2_parts: Array = version2.split(".")
	
	for i: int in range(max(v1_parts.size(), v2_parts.size())):
		var v1_num: int = int(v1_parts[i]) if i < v1_parts.size() else 0
		var v2_num: int = int(v2_parts[i]) if i < v2_parts.size() else 0
		
		if v1_num > v2_num:
			return true
		elif v1_num < v2_num:
			return false
	
	return false
