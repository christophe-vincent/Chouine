extends Node2D

var partie: Partie = null

func set_partie(_partie):
	partie = _partie

func reset():
	# reset pour remettre à jour
	var nodes: Array = $Panel/Grid.get_children()
	for node in nodes:
		var button: Button
		if node is Button and node.name != "Exit":
			button = node
			button.disabled = true

func annonces_autorisees(annonces_autorisee):
	reset()
	# change le status des annonces possibles
	print("- Annonces autorisées: ", annonces_autorisee)
	for a in annonces_autorisee.split(" "):
		print(a)
		print($Panel/Grid.find_child(a, false))
		var node := $Panel/Grid.find_child(a, false) as Button
		if node:
			print("Trouve")
			node.disabled = false


func _on_button_pressed() -> void:
	visible = false


func _on_mariagecoeur_pressed() -> void:
	partie.annonce_joueur("mariage-coeur")


func _on_tiercecoeur_pressed() -> void:
	partie.annonce_joueur("tierce-coeur")


func _on_quarantecoeur_pressed() -> void:
	partie.annonce_joueur("quarante-coeur")
	

func _on_mariagepic_pressed() -> void:
	partie.annonce_joueur("mariage-pic")


func _on_tiercepic_pressed() -> void:
	partie.annonce_joueur("tierce-pic")


func _on_quarantepic_pressed() -> void:
	partie.annonce_joueur("quarante-pic")


func _on_mariagecarreau_pressed() -> void:
	partie.annonce_joueur("mariage-carreau")


func _on_tiercecarreau_pressed() -> void:
	partie.annonce_joueur("tierce-carreau")


func _on_quarantecarreau_pressed() -> void:
	partie.annonce_joueur("quarante-carreau")


func _on_mariagetrefle_pressed() -> void:
	partie.annonce_joueur("mariage-trefle")


func _on_tiercetrefle_pressed() -> void:
	partie.annonce_joueur("tierce-trefle")


func _on_quarantetrefle_pressed() -> void:
	partie.annonce_joueur("quarante-trefle")


func _on_chouine_pressed() -> void:
	partie.annonce_joueur("chouine")


func _on_quinte_pressed() -> void:
	partie.annonce_joueur("quinte")
