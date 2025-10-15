@tool
extends Control

# Scene principale
@onready var pioche: Pile = $Piles/Pioche
@onready var retourne: Pile = $Piles/Retourne
@onready var main_joueur: Pile = $Piles/MainJoueur
@onready var main_ordi: Pile = $Piles/MainOrdi
@onready var tapis: Pile = $Tapis
@onready var joueur_gagnees: Pile = $Piles/JoueurGagnees
@onready var ordi_gagnees: Pile = $Piles/OrdiGagnees
@onready var zone_jeu: CollisionShape2D = $ZoneJeu/CollisionShape2D
@onready var texte_gagne: Label = $Scores/Gagne
@onready var texte_perdu: Label = $Scores/Perdu
@onready var points: Label = $Scores/Points
@onready var dix_de_der: Label = $Scores/DixDeDer
@onready var annonces_ordi: Node2D = $AnnoncesOrdi
@onready var annonces_joueur: Node2D = $AnnoncesJoueur
@onready var jouer: TextureButton = $Jouer

var card_packed_scene = preload("res://Scenes/carte.tscn")
var cartes: Dictionary[String, Carte] = {}
var mains: Dictionary[int, Control] = {}
var cartes_pioche: Array[String] = []
var chouine = IChouine.new()
var carte_atout: Carte = null
var sept_atout: Carte = null
var coup_joueur: bool = false

enum JOUEURS {
	ORDI = 0,
	HUMAIN = 1
}

func _ready():
	mains = {0: main_ordi, 1: main_joueur}
	joueur_gagnees.original_size = joueur_gagnees.size
	ordi_gagnees.original_size = ordi_gagnees.size
	joueur_gagnees.original_position = joueur_gagnees.position
	ordi_gagnees.original_position = ordi_gagnees.position
	init_jeu()
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

func init_jeu():
	retourne.draggable = false
	pioche.card_size = Settings.DEFAULT_CARD_SIZE	
	pioche.card_size = Settings.DEFAULT_CARD_SIZE
	pioche.face_visible = false
	pioche.init_cartes()
	
	main_joueur.card_size = Settings.DEFAULT_CARD_SIZE
	main_joueur.draggable = true
	main_joueur.face_visible = true
	main_joueur.init_cartes()
	
	main_ordi.card_size = Settings.DEFAULT_CARD_SIZE
	main_ordi.draggable = false
	main_ordi.face_visible = true
	main_ordi.init_cartes()
	
	tapis.card_size = Settings.DEFAULT_CARD_SIZE
	tapis.draggable = false
	tapis.face_visible = true
	tapis.adaptatif = false
	tapis.init_cartes()
	
	joueur_gagnees.card_size = Settings.DEFAULT_CARD_SIZE
	joueur_gagnees.Type = Pile.TypePile.PILE
	joueur_gagnees.draggable = false
	joueur_gagnees.face_visible = true
	joueur_gagnees.init_cartes()
	joueur_gagnees.size = joueur_gagnees.original_size
	joueur_gagnees.position = joueur_gagnees.original_position
	
	ordi_gagnees.card_size = Settings.DEFAULT_CARD_SIZE
	ordi_gagnees.Type = Pile.TypePile.PILE
	ordi_gagnees.draggable = false
	ordi_gagnees.face_visible = false
	ordi_gagnees.init_cartes()
	ordi_gagnees.size = ordi_gagnees.original_size
	ordi_gagnees.position = ordi_gagnees.original_position
	
	zone_jeu.disabled = true
	annonces_joueur.partie = self
	
	texte_perdu.visible = false
	texte_gagne.visible = false
	points.visible = false
	dix_de_der.visible = false
	
	annonces_ordi.reset()
	annonces_ordi.visible = true
	annonces_joueur.reset()
	annonces_joueur.visible = true


func carte_jouee(nom):
	if coup_joueur == false:
		return -1
	print ("Choix joueur: " + nom)
	if carte_atout != null && carte_atout.card_name == nom:
		print("ERREUR: Carte d'atout déposée sur la zone de jeu")
		return -1
	var ret = chouine.set_choix_joueur(nom)
	if ret != 0:
		print("ERREUR: Erreur dans le choix du joueur")
		return -1
	coup_joueur = false
	tapis.ajouter_carte(cartes[nom], 0.1)
	main_joueur.supprimer_carte(nom)
	# est-ce la fin du pli ?
	ret = chouine.fin_pli()
	if ret == -1:
		# l'ordi doit jouer
		coup_ordi()
	else:
		fin_pli()
	return 0

func carte_main_joueur(nom):
	# une carte est déposée dans la main du joueur, ca ne peut être que la carte d'atout
	if carte_atout != null && nom == carte_atout.card_name:
		# chercher le sept d'atout
		main_joueur.ajouter_carte(carte_atout)
		main_joueur.supprimer_carte(sept_atout.card_name)
		main_joueur.calcul_positions(Settings.DUREE_MOUVEMENT)
		retourne.ajouter_carte(sept_atout)
		retourne.supprimer_carte(carte_atout.card_name)
		chouine.echanger_carte_atout(JOUEURS.HUMAIN)
		print("Joueur      : Echange carte atout: ", carte_atout.card_name)
		cartes_pioche.pop_front()
		cartes_pioche.push_front(sept_atout.card_name)
		carte_atout = null
		sept_atout.draggable = false
		return 0
	return -1

func melanger():
	cartes_pioche = []
	for c in Array(chouine.partie().split(" ")):
		await get_tree().create_timer(0.05).timeout
		c = c.replace('*', '')
		cartes_pioche.append(c)
		pioche.ajouter_carte(cartes[c])

func _on_texture_button_pressed() -> void:
	init_jeu()
	
	await melanger()
	
	chouine.distribution_cartes()
	print("Cartes pioche: " + str(cartes_pioche))
	print("")
	# qui commence ?
	await get_tree().create_timer(1).timeout
	await distribution_cartes(5)
	
	# carte de la retourne
	var c = cartes_pioche.pop_back()
	pioche.supprimer_carte(c)
	retourne.ajouter_carte(cartes[c])
	cartes_pioche.push_front(c)
	carte_atout = cartes[c]
	sept_atout = cartes["7-" + c.split("-")[1]]
	var sept_atout_en_main: bool = chouine.sept_atout_en_main(JOUEURS.HUMAIN)
	if sept_atout_en_main:
		carte_atout.draggable = true
	
	await get_tree().create_timer(1).timeout
	if chouine.gagnant_pli() == JOUEURS.ORDI:
		coup_ordi()
	#	print(choix_ordi)
	zone_jeu.disabled = false
	coup_joueur = true

func distribution_carte(joueur):
	await get_tree().create_timer(Settings.DUREE_DISTRIBUTION).timeout
	var c = cartes_pioche.pop_back()
	pioche.supprimer_carte(c)
	mains[joueur].ajouter_carte(cartes[c])
	# Si la carte d'atout se retrouve dans une main alors elle devient une carte normale
	if carte_atout != null && carte_atout.card_name == c:
		carte_atout = null

func distribution_cartes(nb_cartes):
	var joueur = chouine.gagnant_pli()
	if cartes_pioche.size() == 0:
		return false
	for i in range(nb_cartes):
		await distribution_carte(joueur)
		joueur = (joueur + 1) % 2
		await distribution_carte(joueur)
		joueur = (joueur + 1) % 2
	
	# le sept d'atout est-il dans la main du joueur ?
	var sept_atout_en_main: bool = chouine.sept_atout_en_main(JOUEURS.HUMAIN)
	if sept_atout_en_main && carte_atout != null:
		carte_atout.draggable = true
	return true

func coup_ordi():
	var choix_ordi = chouine.choix_joueur().split("|")
	var choix_carte = choix_ordi[0]
	print("Choix Ordi  : " + choix_carte)
	var annonce = choix_ordi[1]
	if annonce != "":
		annonces_ordi.point(annonce)
		print("Annonce Ordi: " + annonce)
	var echange_sept_atout = choix_ordi[2]
	if echange_sept_atout != "" && carte_atout != null:
		if retourne.cartes().size() == 0:
			print("ERREUR: La retourne est vide")
		else:
			# la carte de joueur rejoint la retourne
			retourne.ajouter_carte(sept_atout)
			main_ordi.ajouter_carte(carte_atout)
			retourne.supprimer_carte(carte_atout)
			main_ordi.supprimer_carte(sept_atout)
			print("Ordi        : Echange sept atout - ", carte_atout.card_name)
			cartes_pioche.pop_front()
			cartes_pioche.push_front(sept_atout.card_name)
			carte_atout = null
			sept_atout.draggable = true

	main_ordi.supprimer_carte(choix_carte)
	tapis.ajouter_carte(cartes[choix_carte])
	# est-ce la fin du pli ?
	var ret = chouine.fin_pli()
	if ret >= 0:
		fin_pli()
	else:
		coup_joueur = true

func fin_pli():
	await get_tree().create_timer(1.0).timeout
	var gagnant = chouine.gagnant_pli()
	var cartes_tapis = tapis.cartes()
	for c in cartes_tapis:
		tapis.supprimer_carte(c)
	if gagnant == JOUEURS.ORDI:
		print("Gagnant     : Ordi")
		for c in cartes_tapis:
			ordi_gagnees.ajouter_carte(cartes_tapis[c])
	else:
		print("Gagnant     : Joueur")
		for c in cartes_tapis:
			joueur_gagnees.ajouter_carte(cartes_tapis[c])

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
		var screen_size = get_viewport_rect().size
		joueur_gagnees.Type = Pile.TypePile.MAIN
		joueur_gagnees.size = Vector2(screen_size.x - 2*Settings.DEFAULT_CARD_SIZE.x, Settings.DEFAULT_CARD_SIZE.y)
		joueur_gagnees.position.x = Settings.DEFAULT_CARD_SIZE.x
		joueur_gagnees.calcul_positions(0)
		ordi_gagnees.Type = Pile.TypePile.MAIN
		ordi_gagnees.visibilite(true)
		ordi_gagnees.size = Vector2(screen_size.x - 2*Settings.DEFAULT_CARD_SIZE.x, Settings.DEFAULT_CARD_SIZE.y)
		ordi_gagnees.position.x = Settings.DEFAULT_CARD_SIZE.x
		ordi_gagnees.calcul_positions(0)
		annonces_ordi.reset()
		annonces_ordi.visible = false
		annonces_joueur.reset()
		annonces_joueur.visible = false
		return
	# debug: affiche les cartes
	print("Ordi        : " + chouine.cartes_joueur(0))
	print("Joueur      : " + chouine.cartes_joueur(1))
	if gagnant == JOUEURS.ORDI:
		coup_ordi()
	else:
		coup_joueur = true


func annonce_joueur(annonce):
	var ret: int = chouine.annonce_joueur(annonce)
	if ret == 0:
		# annonce autorisee
		annonces_joueur.point(annonce)
		print("Joueur      : " + annonce)
