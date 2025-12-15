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

@onready var Championnat: PanelContainer = $Championnat
@onready var JetonChampionnat: TextureRect = $Championnat/MarginContainer/VBoxContainer/HBoxContainer/ChoixPartie


var lut_points: Array[int] = [-1, 0, 1, 1, 2, 2]
@onready var JetonChampionnat: TextureRect = $Championnat/MarginContainer/VBoxContainer/HBoxContainer/ChoixPartie


var lut_points: Array[int] = [-1, 0, 1, 1, 2, 2]
var points: Array[PanelContainer] = []
var jeton_points: Array[TextureRect] = []
var manches: Array[PanelContainer] = []
var jeton_manches: Array[TextureRect] = []
var scene_partie: Resource = preload("res://Scenes/partie.tscn")
var scene_regles: Resource = preload("res://Scenes/texte.tscn")
var scene_options: Resource = preload("res://Scenes/options.tscn")
var chouine_infos: Dictionary = {}

func _ready() -> void:
	# lecture du fichier d'options, s'il existe
	Global.lecture_options()
	Global.lecture_stats()
	$Stats/Statistiques.update()
	points = [PointUn, PointTrois, PointCinq]
	jeton_points = [JetonUn, JetonTrois, JetonCinq]
	manches = [MancheUne, MancheDeux]
	jeton_manches = [JetonMancheUn, JetonMancheDeux]
	if Global.options["championnat"]:
		_on_championnat_pressed()
	else:
		select_nb_points(Global.options["nb_points"])
		select_nb_manches(Global.options["nb_manches"])

	$Intro.text = Settings.TEXT_INTRO
	if FileAccess.file_exists(Settings.SAVE_FILE):
		$PlayButtons/PartieEnCours.visible = true

func select_nb_points(nb_points: int) -> void:
	if nb_points > 0:
		Global.options["nb_points"] = nb_points
		Global.options["championnat"] = false
		JetonChampionnat.texture = Global.jeton_inactif
		Championnat.add_theme_stylebox_override("panel", Global.non_selected_panel)
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
	if nb_manches > 0:
		Global.options["nb_manches"] = nb_manches
		Global.options["championnat"] = false
		JetonChampionnat.texture = Global.jeton_inactif
		Championnat.add_theme_stylebox_override("panel", Global.non_selected_panel)
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
	select_nb_manches(Global.options["nb_manches"])

func _on_trois_points_pressed() -> void:
	select_nb_points(3)
	select_nb_manches(Global.options["nb_manches"])

func _on_cinq_points_pressed() -> void:
	select_nb_points(5)
	select_nb_manches(Global.options["nb_manches"])

func _on_une_manche_pressed() -> void:
	select_nb_manches(1)
	select_nb_points(Global.options["nb_points"])

func _on_deux_manches_pressed() -> void:
	select_nb_manches(2)
	select_nb_points(Global.options["nb_points"])

func _on_championnat_pressed() -> void:
	Global.options["championnat"] = true
	JetonChampionnat.texture = Global.jeton_actif
	Championnat.add_theme_stylebox_override("panel", Global.selected_panel)
	select_nb_points(0)
	select_nb_manches(0)
	Global.enregistrer_options()



func _on_stats_folding_changed(is_folded: bool) -> void:
	if not is_folded:
		$Stats/Statistiques.update()


func _on_credits_pressed() -> void:
	Global.texte = Global.TypeTexte.CREDITS
	get_tree().change_scene_to_packed(scene_regles)
