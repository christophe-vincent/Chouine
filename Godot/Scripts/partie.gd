@tool
extends Control

# Scene principale
@onready var card_container: Control = $CardContainer
@onready var pioche: Control = $Pioche
@onready var retourne: Control = $Retourne
@onready var main_joueur: Control = $MainJoueur
@onready var main_ordi: Control = $MainOrdi
@onready var tapis: Control = $Tapis
@onready var joueur_gagnees: Control = $JoueurGagnees
@onready var ordi_gagnees: Control = $OrdiGagnees

var card_packed_scene = preload("res://Scenes/carte.tscn")
var cartes = {}
var mains = {}
var cartes_pioche = []
var chouine = IChouine.new()


enum JOUEURS {
	ORDI = 0,
	HUMAIN = 1
}

func _ready():
	mains = {0: main_ordi, 1: main_joueur}
	pioche.card_size = Settings.DEFAULT_CARD_SIZE
	
	pioche.card_size = Settings.DEFAULT_CARD_SIZE
	pioche.face_visible = true
	
	main_joueur.card_size = Settings.DEFAULT_CARD_SIZE
	main_joueur.draggable = true
	main_joueur.face_visible = true
	
	main_ordi.card_size = Settings.DEFAULT_CARD_SIZE
	main_ordi.draggable = false
	main_ordi.face_visible = true
	
	tapis.card_size = Settings.DEFAULT_CARD_SIZE
	tapis.draggable = false
	tapis.face_visible = true
	tapis.adaptatif = false
	
	joueur_gagnees.card_size = Settings.DEFAULT_CARD_SIZE
	joueur_gagnees.draggable = false
	joueur_gagnees.face_visible = true
	
	ordi_gagnees.card_size = Settings.DEFAULT_CARD_SIZE
	ordi_gagnees.draggable = false
	ordi_gagnees.face_visible = true
	
	setup_game()

func setup_game():
	pass
	

func carte_jouee(nom):
	print("Choix joueur: " + nom)
	var ret = chouine.set_choix_joueur(nom)
	if ret == -1:
		print("Erreur dans le choix du joueur")
	tapis.ajouter_carte(nom, cartes[nom])
	main_joueur.supprimer_carte(nom)
	# est-ce la fin du pli ?
	ret = chouine.fin_pli()
	if ret == -1:
		# l'ordi doit jouer
		coup_ordi()
	else:
		fin_pli()
	
	
func mouse_released():
	pass


func _on_texture_button_pressed() -> void:
	# remove cards
	for k in cartes:
		remove_child(cartes[k])
	cartes.clear()
	pioche.init_cartes()
	main_joueur.init_cartes()
	main_ordi.init_cartes()
	tapis.init_cartes()
	
	pioche.draggable = false
	pioche.face_visible = false
	
	# lecture du fichier de la description des cartes
	var json_as_text = FileAccess.get_file_as_string("res://Assets/Data/jeu-carte.json")
	var jeu_cartes = JSON.parse_string(json_as_text)
	var x = 100
	var y = 100
	for couleur in jeu_cartes:
		for valeur in jeu_cartes[couleur]:
			var nom_carte = valeur + "-" + couleur
			if not cartes.has(nom_carte):
				var card_instance = card_packed_scene.instantiate()
				card_instance.card_name = nom_carte
				card_instance.front_image = load("res://Assets/Cartes/" + jeu_cartes[couleur][valeur]['image'])
				card_instance.back_image = load("res://Assets/Cartes/back_1.svg")
				card_instance.show_front = false
				card_instance.position = Vector2(x, y)
				add_child(card_instance)
				cartes[nom_carte] = card_instance
				x += 10
				y += 10
		x += 120
		y = 100
	cartes_pioche = []
	for c in Array(chouine.partie().split(" ")):
		#await get_tree().create_timer(0.05).timeout
		cartes_pioche.append(c)
		pioche.ajouter_carte(c, cartes[c])
	
	chouine.distribution_cartes()
	print("Cartes pioche:")
	print(cartes_pioche)
	print("")
	# qui commence ?
	await get_tree().create_timer(1).timeout
	distribution_cartes(5)
	
	# carte de la retourne
	var c = cartes_pioche.pop_back()
	pioche.supprimer_carte(c)
	retourne.ajouter_carte(c, cartes[c])
	
	await get_tree().create_timer(1).timeout
	if chouine.gagnant_pli() == JOUEURS.ORDI:
		coup_ordi()
	#	print(choix_ordi)
	pass # Replace with function body.
	
func distribution_cartes(nb_cartes):
	var joueur = chouine.gagnant_pli()
	for i in range(nb_cartes):
		var c = cartes_pioche.pop_back()
		pioche.supprimer_carte(c)
		mains[joueur].ajouter_carte(c, cartes[c])
		print("Joueur: " + str(joueur) + " Distribution: " + c)
		joueur = (joueur + 1) % 2
		c = cartes_pioche.pop_back()
		pioche.supprimer_carte(c)
		mains[joueur].ajouter_carte(c, cartes[c])
		print("Joueur: " + str(joueur) + " Distribution: " + c)
		joueur = (joueur + 1) % 2

func coup_ordi():
	var choix_ordi = chouine.choix_joueur()
	print("Choix Ordi: " + choix_ordi)
	main_ordi.supprimer_carte(choix_ordi)
	tapis.ajouter_carte(choix_ordi, cartes[choix_ordi])
	# est-ce la fin du pli ?
	var ret = chouine.fin_pli()
	print("Fin pli ? ", ret)
	if ret >= 0:
		fin_pli()
	
func fin_pli():
	await get_tree().create_timer(1).timeout
	var gagnant = chouine.gagnant_pli()
	var cartes_tapis = tapis.cartes()
	for c in cartes_tapis:
		tapis.supprimer_carte(c)
	if gagnant == JOUEURS.ORDI:
		print("Gagnant: Ordi")
		for c in cartes_tapis:
			ordi_gagnees.ajouter_carte(c, cartes_tapis[c])
	else:
		print("Gagnant: Joueur")
		for c in cartes_tapis:
			joueur_gagnees.ajouter_carte(c, cartes_tapis[c])
	distribution_cartes(1)
	if gagnant == JOUEURS.ORDI:
		coup_ordi()
