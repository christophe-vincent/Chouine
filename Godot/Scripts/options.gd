"""
Copyright 2025,2026 Christophe Vincent
This file is part of La Chouine.
La Chouine is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or any later version.
La Chouine is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with Foobar. If not, see <https://www.gnu.org/licenses/>
"""
extends Control
class_name Options

@onready var Dos1: PanelContainer = $DosCartes/Dos1
@onready var Dos2: PanelContainer = $DosCartes/Dos2
@onready var Dos3: PanelContainer = $DosCartes/Dos3
@onready var Dos4: PanelContainer = $DosCartes/Dos4
@onready var Dos5: PanelContainer = $DosCartes/Dos5
@onready var Dos6: PanelContainer = $DosCartes/Dos6
@onready var Jeton1: TextureRect = $DosCartes/Dos1/MarginContainer/VBoxContainer/ChoixDos
@onready var Jeton2: TextureRect = $DosCartes/Dos2/MarginContainer/VBoxContainer/ChoixDos
@onready var Jeton3: TextureRect = $DosCartes/Dos3/MarginContainer/VBoxContainer/ChoixDos
@onready var Jeton4: TextureRect = $DosCartes/Dos4/MarginContainer/VBoxContainer/ChoixDos
@onready var Jeton5: TextureRect = $DosCartes/Dos5/MarginContainer/VBoxContainer/ChoixDos
@onready var Jeton6: TextureRect = $DosCartes/Dos6/MarginContainer/VBoxContainer/ChoixDos
@onready var Tapis1: PanelContainer = $Tapis/Tapis1
@onready var Tapis2: PanelContainer = $Tapis/Tapis2
@onready var Tapis3: PanelContainer = $Tapis/Tapis3
@onready var JetonTapis1: TextureRect = $Tapis/Tapis1/MarginContainer/VBoxContainer/ChoixDos
@onready var JetonTapis2: TextureRect = $Tapis/Tapis2/MarginContainer/VBoxContainer/ChoixDos
@onready var JetonTapis3: TextureRect = $Tapis/Tapis3/MarginContainer/VBoxContainer/ChoixDos

var dos: Array[PanelContainer] = []
var jetons_dos: Array[TextureRect] = []
var tapis: Array[PanelContainer] = []
var jetons_tapis: Array[TextureRect] = []

func _ready() -> void:
	dos = [Dos1, Dos2, Dos3, Dos4, Dos5, Dos6]
	jetons_dos = [Jeton1, Jeton2, Jeton3, Jeton4, Jeton5, Jeton6]
	tapis = [Tapis1, Tapis2, Tapis3]
	jetons_tapis = [JetonTapis1, JetonTapis2, JetonTapis3]
	select_dos(Global.options["dos_carte"])
	select_tapis(Global.options["couleur_tapis"])
	$CartesVisibles.button_pressed = Global.options["cartes_visibles"]


func select_dos(index: int) -> void:
	Global.options["dos_carte"] = index
	Global.enregistrer_options()
	for i: int in range(dos.size()):
		# modifier le background
		if i == index:
			dos[i].add_theme_stylebox_override("panel", Global.selected_panel)
			jetons_dos[i].texture = Global.jeton_actif
		else:
			dos[i].add_theme_stylebox_override("panel", Global.non_selected_panel)
			jetons_dos[i].texture = Global.jeton_inactif

func select_tapis(index: int) -> void:
	Global.options["couleur_tapis"] = index
	Global.enregistrer_options()
	for i: int in range(tapis.size()):
		# modifier le background
		if i == index:
			tapis[i].add_theme_stylebox_override("panel", Global.selected_panel)
			jetons_tapis[i].texture = Global.jeton_actif
		else:
			tapis[i].add_theme_stylebox_override("panel", Global.non_selected_panel)
			jetons_tapis[i].texture = Global.jeton_inactif


func _on_acceuil_pressed() -> void:
	var error: Error = get_tree().change_scene_to_file("res://Scenes/accueil.tscn")
	if error != OK:
		print("Scene change failed with error: ", error)


func _on_cartes_visibles_pressed() -> void:
	Global.options["cartes_visibles"] = $CartesVisibles.button_pressed
	Global.enregistrer_options()


func _on_bouton_dos_1_pressed() -> void:
	select_dos(0)

func _on_bouton_dos_2_pressed() -> void:
	select_dos(1)

func _on_bouton_dos_3_pressed() -> void:
	select_dos(2)

func _on_bouton_dos_4_pressed() -> void:
	select_dos(3)

func _on_bouton_dos_5_pressed() -> void:
	select_dos(4)

func _on_bouton_dos_6_pressed() -> void:
	select_dos(5)

func _on_bouton_tapis_1_pressed() -> void:
	select_tapis(0)

func _on_bouton_tapis_2_pressed() -> void:
	select_tapis(1)

func _on_bouton_tapis_3_pressed() -> void:
	select_tapis(2)
