"""
Copyright 2025,2026 Christophe Vincent
This file is part of La Chouine.
La Chouine is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or any later version.
La Chouine is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with Foobar. If not, see <https://www.gnu.org/licenses/>
"""
extends Node

# Options
var options: Dictionary = {
	"nb_points": 3,
	"nb_manches": 1,
	"cartes_visibles": false,
	"dos_carte": 0,
	"couleur_tapis": 0
}

var stats: Dictionary = {
	"total": {
		"parties": {
			"W": 0,
			"L": 0
		},
		"chouines": {
			"W": 0,
			"L": 0
		},
	},
	"annees": {},
	"mois": {},
	"nb_points": {
		"1": 0,
		"2": 0,
		"3": 0,
		"5": 0,
		"6": 0,
		"10": 0
	}
}

# Regles ou credits
enum TypeTexte {REGLES, CREDITS}
var texte: TypeTexte = TypeTexte.REGLES

#
# Panels for selections
#
var selected_panel: StyleBoxFlat = StyleBoxFlat.new()
var non_selected_panel: StyleBoxFlat = StyleBoxFlat.new()

var jeton_actif: Resource = load("res://Assets/point_partie_actif.png")
var jeton_inactif: Resource = load("res://Assets/point_partie_inactif.png")

# scene de la partie en mode aide
var help_mode: bool = false


func _ready() -> void:
	selected_panel.bg_color = Color(0.635, 0.635, 0.635, 0.196)
	selected_panel.border_width_left = 1
	selected_panel.border_width_top = 1
	selected_panel.border_width_right = 1
	selected_panel.border_width_bottom = 1
	selected_panel.border_color = Color.WHITE_SMOKE
	selected_panel.set_corner_radius_all(30)
	non_selected_panel.bg_color = Color(0, 0, 0, 0)

func lecture_fichier(filename: String, data: Variant) -> Variant:
	if not FileAccess.file_exists(filename):
		return data
	var json_as_text: String = FileAccess.get_file_as_string(filename)
	var json: Variant = JSON.parse_string(json_as_text)
	return json if json != null else data

func enregistrer_fichier(filename: String, data: Variant) -> void:
	var save_file: FileAccess = FileAccess.open(filename, FileAccess.WRITE)
	save_file.store_line(JSON.stringify(data))


func lecture_options() -> void:
	options = lecture_fichier(Settings.OPTIONS_FILE, options)
	
func enregistrer_options() -> void:
	enregistrer_fichier(Settings.OPTIONS_FILE, options)

func lecture_stats() -> void:
	stats = lecture_fichier(Settings.STATS_FILE, stats)
	var now: Dictionary = Time.get_date_dict_from_system()
	# suppression des mois précédents
	for index: int in range(1, 12):
		if index != now["month"] and str(index) in stats["mois"]:
			stats["mois"].erase(str(index))
	# ajout du mois en cours
	var year: String = str(now['year'])
	var month: String = str(now['month'])
	if year not in stats["annees"]:
		stats["annees"][year] = {"parties": {"W": 0, "L": 0}, "chouines": {"W": 0, "L": 0}}
	if month not in stats["mois"]:
		stats["mois"][month] = {"parties": {"W": 0, "L": 0}, "chouines": {"W": 0, "L": 0}}

func enregistrer_stats() -> void:
	enregistrer_fichier(Settings.STATS_FILE, stats)


#
# Gestion des stats
#
func ajouter_point(resultat: bool, type: String) -> void:
	var now: Dictionary = Time.get_date_dict_from_system()
	var year: String = str(now['year'])
	var month: String = str(now['month'])
	if year not in stats["annees"]:
		stats["annees"][year] = {"parties": {"W": 0, "L": 0}, "chouines": {"W": 0, "L": 0}}
	if month not in stats["mois"]:
		stats["mois"][month] = {"parties": {"W": 0, "L": 0}, "chouines": {"W": 0, "L": 0}}
	var WL: String = "W" if resultat else "L"
	stats['total'][type][WL] += 1
	stats["annees"][year][type][WL] += 1
	stats["mois"][month][type][WL] += 1
	enregistrer_fichier(Settings.STATS_FILE, stats)


func fin_chouine(resultat: bool) -> void:
	var index: int = options["nb_points"]*options["nb_manches"]
	stats["nb_points"][str(index)] += 1
	ajouter_point(resultat, "chouines")

func fin_partie(resultat: bool) -> void:
	ajouter_point(resultat, "parties")
