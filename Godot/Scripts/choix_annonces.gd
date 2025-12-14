"""
Copyright 2025,2026 Christophe Vincent
This file is part of La Chouine.
La Chouine is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or any later version.
La Chouine is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with Foobar. If not, see <https://www.gnu.org/licenses/>
"""
extends Control

var partie: Partie = null

func set_partie(_partie: Partie) -> void:
	partie = _partie

func reset() -> void:
	# reset pour remettre à jour
	var nodes: Array = $Panel/Grid.get_children()
	for node: Node in nodes:
		var button: Button
		if node is Button and node.name != "Exit":
			button = node
			button.disabled = true

func annonces_autorisees(annonces_autorisee: String) -> void:
	reset()
	if annonces_autorisee != "":
		# change le status des annonces possibles
		print("Annonces autorisées: ", annonces_autorisee)
		for a: String in annonces_autorisee.split(" "):
			if a != "":
				if a.find("chouine-") == 0:
					a = "chouine"
				var node: Button = $Panel/Grid.find_child(a, false) as Button
				if node:
					node.disabled = false


func _on_button_pressed() -> void:
	visible = false


func _on_mariagecoeur_pressed() -> void:
	partie.annonce_joueur("mariage-coeur")
	visible = false


func _on_tiercecoeur_pressed() -> void:
	partie.annonce_joueur("tierce-coeur")
	visible = false


func _on_quarantecoeur_pressed() -> void:
	partie.annonce_joueur("quarante-coeur")
	visible = false
	

func _on_mariagepic_pressed() -> void:
	partie.annonce_joueur("mariage-pic")
	visible = false


func _on_tiercepic_pressed() -> void:
	partie.annonce_joueur("tierce-pic")
	visible = false


func _on_quarantepic_pressed() -> void:
	partie.annonce_joueur("quarante-pic")
	visible = false


func _on_mariagecarreau_pressed() -> void:
	partie.annonce_joueur("mariage-carreau")
	visible = false


func _on_tiercecarreau_pressed() -> void:
	partie.annonce_joueur("tierce-carreau")
	visible = false


func _on_quarantecarreau_pressed() -> void:
	partie.annonce_joueur("quarante-carreau")
	visible = false


func _on_mariagetrefle_pressed() -> void:
	partie.annonce_joueur("mariage-trefle")
	visible = false


func _on_tiercetrefle_pressed() -> void:
	partie.annonce_joueur("tierce-trefle")
	visible = false


func _on_quarantetrefle_pressed() -> void:
	partie.annonce_joueur("quarante-trefle")
	visible = false


func _on_chouine_pressed() -> void:
	partie.annonce_joueur("chouine")
	visible = false


func _on_quinte_pressed() -> void:
	partie.annonce_joueur("quinte")
	visible = false
