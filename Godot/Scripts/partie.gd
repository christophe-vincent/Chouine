@tool
extends Control

# Scene principale
@onready var pioche: Control = $Piles/Pioche
@onready var retourne: Control = $Piles/Retourne
@onready var main_joueur: Control = $Piles/MainJoueur
@onready var main_ordi: Control = $Piles/MainOrdi
@onready var tapis: Control = $Tapis
@onready var joueur_gagnees: Control = $Piles/JoueurGagnees
@onready var ordi_gagnees: Control = $Piles/OrdiGagnees
@onready var dix_de_der: Control = $DixDeDer
@onready var zone_jeu: CollisionShape2D = $ZoneJeu/CollisionShape2D
@onready var texte_gagne: Label = $Gagne
@onready var texte_perdu: Label = $Perdu
@onready var points: Label = $Points
@onready var annonces_ordi: Node2D = $AnnoncesOrdi
@onready var annonces_joueur: Node2D = $AnnoncesJoueur

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
	zone_jeu.disabled = true
	annonces_joueur.partie = self
	setup_game()

func setup_game():
	creer_cartes()
	
func creer_cartes():
	# lecture du fichier de la description des cartes
	var json_as_text = FileAccess.get_file_as_string("res://Assets/Data/jeu-carte.json")
	var jeu_cartes = JSON.parse_string(json_as_text)
	for couleur in jeu_cartes:
		for valeur in jeu_cartes[couleur]:
			var nom_carte = valeur + "-" + couleur
			if not cartes.has(nom_carte):
				var card_instance = card_packed_scene.instantiate()
				card_instance.card_name = nom_carte
				card_instance.front_image = load("res://Assets/Cartes/" + jeu_cartes[couleur][valeur]['image'])
				card_instance.back_image = load("res://Assets/Cartes/back_1.svg")
				card_instance.show_front = false
				card_instance.position = tapis.position + tapis.size/2
				add_child(card_instance)
				cartes[nom_carte] = card_instance

func carte_jouee(nom):
	print("Choix joueur: " + nom)
	var ret = chouine.set_choix_joueur(nom)
	if ret == -1:
		print("Erreur dans le choix du joueur")
	tapis.ajouter_carte(nom, cartes[nom], 0.1)
	main_joueur.supprimer_carte(nom)
	# est-ce la fin du pli ?
	ret = chouine.fin_pli()
	if ret == -1:
		# l'ordi doit jouer
		coup_ordi()
	else:
		fin_pli()
	
func melanger():
	cartes_pioche = []
	for c in Array(chouine.partie().split(" ")):
		await get_tree().create_timer(0.05).timeout
		c = c.replace('*', '')
		cartes_pioche.append(c)
		pioche.ajouter_carte(c, cartes[c])

func _on_texture_button_pressed() -> void:
	# remove cards
	#for k in cartes:
	#	remove_child(cartes[k])
	#cartes.clear()
	pioche.init_cartes()
	main_joueur.init_cartes()
	main_ordi.init_cartes()
	tapis.init_cartes()
	dix_de_der.visible = false
	texte_gagne.visible = false
	texte_perdu.visible = false
	points.visible = false
	
	pioche.draggable = false
	pioche.face_visible = false
	annonces_ordi.reset()
	annonces_joueur.reset()
	
	await melanger()
	
	chouine.distribution_cartes()
	print("Cartes pioche: " + str(cartes_pioche))
	print("")
	# qui commence ?
	await get_tree().create_timer(1).timeout
	await distribution_cartes(5)
	
	# carte de la retourne
	var c = cartes_pioche.front()
	pioche.supprimer_carte(c)
	retourne.ajouter_carte(c, cartes[c])
	
	await get_tree().create_timer(1).timeout
	if chouine.gagnant_pli() == JOUEURS.ORDI:
		coup_ordi()
	#	print(choix_ordi)
	zone_jeu.disabled = false
	pass # Replace with function body.
	
func distribution_cartes(nb_cartes):
	var joueur = chouine.gagnant_pli()
	if cartes_pioche.size() == 0:
		return false
	for i in range(nb_cartes):
		await get_tree().create_timer(0.5).timeout
		var c = cartes_pioche.pop_back()
		pioche.supprimer_carte(c)
		mains[joueur].ajouter_carte(c, cartes[c])
		print("Joueur: " + str(joueur) + " Distribution: " + c)
		await get_tree().create_timer(0.5).timeout
		joueur = (joueur + 1) % 2
		c = cartes_pioche.pop_back()
		pioche.supprimer_carte(c)
		mains[joueur].ajouter_carte(c, cartes[c])
		print("Joueur: " + str(joueur) + " Distribution: " + c)
		joueur = (joueur + 1) % 2
	return true

func coup_ordi():
	var choix_ordi = chouine.choix_joueur().split("|")
	var choix_carte = choix_ordi[0]
	print("Choix Ordi: " + choix_carte)
	var annonce = choix_ordi[1]
	if annonce:
		annonces_ordi.point(annonce)
		print("Annonce: " + annonce)
	var sept_atout = choix_ordi[2]
	if sept_atout != "":
		if retourne.cartes().size() == 0:
			print("Erreur, la retourne est vide")
		else:
			var carte_retourne = retourne.cartes().keys()[0]
			# la carte de joueur rejoint la retourne
			retourne.ajouter_carte(sept_atout)
			main_ordi.ajouter_carte(carte_retourne)
			retourne.supprimer_carte(carte_retourne)
			main_ordi.supprimer_carte(sept_atout)
			print(carte_retourne)
			
	main_ordi.supprimer_carte(choix_carte)
	tapis.ajouter_carte(choix_carte, cartes[choix_carte])
	# est-ce la fin du pli ?
	var ret = chouine.fin_pli()
	if ret >= 0:
		fin_pli()
	
func fin_pli():
	await get_tree().create_timer(1.0).timeout
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

	print(" ")
	await distribution_cartes(1)
	annonces_joueur.annonces_autorisees(chouine.annonces_en_main_joueur(1))
	
	await get_tree().create_timer(1.0).timeout
	
	if chouine.fin_partie() == 1:
		# fin de la partie
		if gagnant == JOUEURS.HUMAIN:
			dix_de_der.visible = true
			zone_jeu.disabled = true
			texte_gagne.visible = true
		else:
			texte_perdu.visible = true
		var points_ordi = chouine.points_joueur(JOUEURS.ORDI)
		var points_joueur = chouine.points_joueur(JOUEURS.HUMAIN)
		points.text = str(points_joueur) + " - " + str(points_ordi)
		points.visible = true
		joueur_gagnees.Type = Pile.TypePile.MAIN
		joueur_gagnees.size = Vector2(845, 190)
		joueur_gagnees.calcul_positions(0)
		ordi_gagnees.Type = Pile.TypePile.MAIN
		ordi_gagnees.size = Vector2(845, 190)
		ordi_gagnees.calcul_positions(0)
		return
	# debug: affiche les cartes
	print("Ordi  : " + chouine.cartes_joueur(0))
	print("Joueur: " + chouine.cartes_joueur(1))
	if gagnant == JOUEURS.ORDI:
		coup_ordi()


func annonce_joueur(annonce):
	var ret: int = chouine.annonce_joueur(annonce)
	if ret == 0:
		# annonce autorisee
		annonces_joueur.point(annonce)
	print(annonce)
