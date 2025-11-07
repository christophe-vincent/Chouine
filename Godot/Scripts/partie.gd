@tool
extends Control
class_name Partie

# Scene principale
@onready var pioche: Pile = $Piles/Pioche
@onready var retourne: Pile = $Piles/Retourne
@onready var main_joueur: Pile = $Piles/MainJoueur
@onready var main_ordi: Pile = $Piles/MainOrdi
@onready var tapis: Pile = $Tapis
@onready var joueur_gagnees: Pile = $Piles/JoueurGagnees
@onready var ordi_gagnees: Pile = $Piles/OrdiGagnees
@onready var zone_jeu: CollisionShape2D = $ZoneJeu/CollisionShape2D
@onready var annonces_ordi: Annonces = $AnnoncesOrdi
@onready var annonces_joueur: Node2D = $AnnoncesJoueur
@onready var choix_annonces: Node2D = $ChoixAnnonces
@onready var confirmation_annuler: ConfirmationDialog = $Annuler/ConfirmationDialog

var card_packed_scene = preload("res://Scenes/carte.tscn")
var score_scene = preload("res://Scenes/scores.tscn")
var cartes: Dictionary[String, Carte] = {}
var mains: Dictionary[int, Control] = {}
var cartes_pioche: Array[String] = []
var chouine = IChouine.new()
var carte_atout: Carte = null
var sept_atout: Carte = null
var coup_joueur: bool = false
var nb_points_ordi: int = 0
var nb_points_joueur: int = 0
var nb_manches_ordi: int = 0
var nb_manches_joueur: int = 0
var jeton_point_actif: Resource = load("res://Assets/point_partie_actif.png")
var jeton_point_inactif: Resource = load("res://Assets/point_partie_inactif.png")
var manche_actif: Resource = load("res://Assets/point_manche_actif.png")
var jetons_ordi: Array[TextureRect] = []
var jetons_joueur: Array[TextureRect] = []
var manches_ordi: Array[TextureRect] = []
var manches_joueur: Array[TextureRect] = []
var partie_terminee: bool = false
var joueur_gagnees_size := Vector2(0, 0)
var ordi_gagnees_size := Vector2(0, 0)
var joueur_gagnees_position := Vector2(0, 0)
var ordi_gagnees_position := Vector2(0, 0)


enum JOUEURS {
	ORDI = 0,
	HUMAIN = 1
}

func _ready():
	mains = {0: main_ordi, 1: main_joueur}
	joueur_gagnees_size = joueur_gagnees.size
	joueur_gagnees_position = joueur_gagnees.position
	ordi_gagnees_size = ordi_gagnees.size
	ordi_gagnees_position = ordi_gagnees.position
	nb_points_ordi = 0
	nb_points_joueur = 0
	nb_manches_ordi = 0
	nb_manches_joueur = 0
	if Global.nb_manches == 1:
		$ScoreJoueur/ScoreJoueur/ManchesJoueur.visible = false
		$ScoreOrdi/ScoreOrdi/ManchesJoueur.visible = false
		$ScoreOrdi.size.y = 100
		$ScoreJoueur.size.y = 100
	if Global.nb_points == 1:
		$ScoreJoueur/ScoreJoueur/PointsJoueur.visible = false
		$ScoreOrdi/ScoreOrdi/PointsJoueur.visible = false
	elif Global.nb_points <= 3:
		$ScoreJoueur/ScoreJoueur/PointsJoueur/Point5.visible = false
		$ScoreOrdi/ScoreOrdi/PointsJoueur/Point5.visible = false
		$ScoreJoueur/ScoreJoueur/PointsJoueur/Point4.visible = false
		$ScoreOrdi/ScoreOrdi/PointsJoueur/Point4.visible = false
	jetons_ordi.push_back($ScoreOrdi/ScoreOrdi/PointsJoueur/Point1)
	jetons_ordi.push_back($ScoreOrdi/ScoreOrdi/PointsJoueur/Point2)
	jetons_ordi.push_back($ScoreOrdi/ScoreOrdi/PointsJoueur/Point3)
	jetons_ordi.push_back($ScoreOrdi/ScoreOrdi/PointsJoueur/Point4)
	jetons_ordi.push_back($ScoreOrdi/ScoreOrdi/PointsJoueur/Point5)
	jetons_joueur.push_back($ScoreJoueur/ScoreJoueur/PointsJoueur/Point1)
	jetons_joueur.push_back($ScoreJoueur/ScoreJoueur/PointsJoueur/Point2)
	jetons_joueur.push_back($ScoreJoueur/ScoreJoueur/PointsJoueur/Point3)
	jetons_joueur.push_back($ScoreJoueur/ScoreJoueur/PointsJoueur/Point4)
	jetons_joueur.push_back($ScoreJoueur/ScoreJoueur/PointsJoueur/Point5)
	manches_ordi.push_back($ScoreOrdi/ScoreOrdi/ManchesJoueur/JoueurManche1)
	manches_ordi.push_back($ScoreOrdi/ScoreOrdi/ManchesJoueur/JoueurManche2)
	manches_joueur.push_back($ScoreJoueur/ScoreJoueur/ManchesJoueur/JoueurManche1)
	manches_joueur.push_back($ScoreJoueur/ScoreJoueur/ManchesJoueur/JoueurManche2)
	$Scores/Button.connect("pressed", _partie_suivante.bind())
	setup_game()
	demarrer_jeu()

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
	$Scores.visible = false
	retourne.draggable = false
	
	pioche.draggable = false
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
	joueur_gagnees.type = Pile.TypePile.PILE
	joueur_gagnees.draggable = false
	joueur_gagnees.face_visible = true
	joueur_gagnees.spreadable = true
	joueur_gagnees.init_cartes()
	joueur_gagnees.size = joueur_gagnees_size
	joueur_gagnees.position = joueur_gagnees_position
	
	ordi_gagnees.card_size = Settings.DEFAULT_CARD_SIZE
	ordi_gagnees.type = Pile.TypePile.PILE
	ordi_gagnees.type_position = Pile.TypePosition.EVENTAIL_BAS
	ordi_gagnees.draggable = false
	ordi_gagnees.face_visible = false
	ordi_gagnees.init_cartes()
	ordi_gagnees.size = ordi_gagnees_size
	ordi_gagnees.position = ordi_gagnees_position
	
	zone_jeu.disabled = true
	choix_annonces.partie = self
	
	annonces_ordi.reset()
	annonces_ordi.visible = true
	annonces_joueur.reset()
	annonces_joueur.visible = true
	choix_annonces.reset()
	choix_annonces.visible = false
	choix_annonces.set_partie(self)


func demarrer_jeu() -> void:
	init_jeu()
	
	await melanger()
	
	chouine.distribution_cartes()
	print("Cartes pioche: " + str(cartes_pioche))
	print("")
	# qui commence ?
	await get_tree().create_timer(1).timeout
	await distribution_cartes(5)
	
	choix_annonces.annonces_autorisees(chouine.annonces_en_main_joueur(1))
	
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
		choix_annonces.annonces_autorisees(chouine.annonces_en_main_joueur(1))
		return 0
	return -1


func melanger():
	cartes_pioche = []
	for c in Array(chouine.partie().split(" ")):
		await get_tree().create_timer(0.05).timeout
		c = c.replace('*', '')
		cartes_pioche.append(c)
		pioche.ajouter_carte(cartes[c])


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
		main_joueur.calcul_positions(Settings.DUREE_MOUVEMENT)
		main_ordi.calcul_positions(Settings.DUREE_MOUVEMENT)
		return false
	for i in range(nb_cartes):
		await distribution_carte(joueur)
		joueur = (joueur + 1) % 2
		await distribution_carte(joueur)
		joueur = (joueur + 1) % 2
	
	# on affiche les carte de la main du joueur dans l'ordre donné par l'algo
	chouine.trier_cartes(JOUEURS.HUMAIN)
	var cartes_joueur_str: Array = chouine.cartes_joueur(JOUEURS.HUMAIN).split(" ")
	for carte in cartes_joueur_str:
		print("CARTE : " + carte)
	main_joueur.ordre_cartes(cartes_joueur_str)
	
	
	# le sept d'atout est-il dans la main du joueur ?
	var sept_atout_en_main: bool = chouine.sept_atout_en_main(JOUEURS.HUMAIN)
	if sept_atout_en_main && carte_atout != null && cartes_pioche.size() > 0:
		carte_atout.draggable = true
	return true


func coup_ordi():
	var choix_ordi: Array = chouine.choix_joueur().split("|")
	var choix_carte: String = choix_ordi[0]
	print("Choix Ordi  : " + choix_carte)
	var annonce = choix_ordi[1]
	if annonce != "":
		annonces_ordi.add(annonce)
		print("Annonce Ordi: " + annonce)
	var echange_sept_atout = choix_ordi[2]
	if echange_sept_atout != "" && carte_atout != null:
		if retourne.cartes().size() == 0:
			print("ERREUR: La retourne est vide")
		else:
			# la carte de joueur rejoint la retourne
			main_ordi.supprimer_carte(sept_atout.card_name)
			main_ordi.ajouter_carte(carte_atout)
			main_ordi.calcul_positions(Settings.DUREE_MOUVEMENT)
			retourne.supprimer_carte(carte_atout.card_name)
			retourne.ajouter_carte(sept_atout)
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
	choix_annonces.annonces_autorisees(chouine.annonces_en_main_joueur(1))

	await get_tree().create_timer(1.0).timeout
	
	if chouine.fin_partie() == 1:
		# fin de la partie
		fin_partie()
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
		annonces_joueur.add(annonce)
		print("Joueur      : " + annonce)


func fin_partie():
	zone_jeu.disabled = true
	$Scores/Defaite.visible = false
	$Scores/Victoire.visible = false
	$Scores.visible = true
	for c in main_ordi.cartes():
		ordi_gagnees.ajouter_carte(main_ordi[c])
	for c in main_joueur.cartes():
		joueur_gagnees.ajouter_carte(main_ordi[c])
	
	if chouine.points_joueur(JOUEURS.ORDI) > chouine.points_joueur(JOUEURS.HUMAIN):
		nb_points_ordi += 1
		if nb_points_ordi >= Global.nb_points:
			# manche gagnée par l'ordi
			nb_manches_ordi += 1
			nb_points_joueur = 0
			nb_points_ordi = 0
			if nb_manches_ordi >= Global.nb_manches:
				# partie gagnée par l'ordi
				partie_terminee = true
				$Scores/Info.visible = false
				$Scores/Victoire.visible = false
				$Scores/Defaite.visible = true
			else:
				$Scores/Info.visible = true
				$Scores/Info.text = "Vous avez perdu cette partie et cette manche"
		else:
			$Scores/Info.visible = true
			$Scores/Info.text = "Vous avez perdu cette partie"
	else:
		nb_points_joueur += 1
		if nb_points_joueur >= Global.nb_points:
			# manche gagnée par le joueur
			nb_manches_joueur += 1
			nb_points_joueur = 0
			nb_points_ordi = 0
			if nb_manches_joueur >= Global.nb_manches:
				# partie gagnée par le joueur
				partie_terminee = true
				$Scores/Info.visible = false
				$Scores/Defaite.visible = false
				$Scores/Victoire.visible = true
			else:
				$Scores/Info.visible = true
				$Scores/Info.text = "Vous avez gagné cette partie et cette manche"
		else:
			$Scores/Info.visible = true
			$Scores/Info.text = "Vous avez gagné cette partie"
	
	var pt_joueur: Array = chouine.points_joueur_str(JOUEURS.HUMAIN).split("|")
	$Scores/Scores/CartesJoueur.text = pt_joueur[1]
	$Scores/Scores/AnnoncesJoueur.text = pt_joueur[2]
	$Scores/Scores/DixDerJoueur.text = pt_joueur[3]
	$Scores/Scores/TotalJoueur.text = pt_joueur[0]
	var pt_ordi: Array = chouine.points_joueur_str(JOUEURS.ORDI).split("|")
	$Scores/Scores/CartesOrdi.text = pt_ordi[1]
	$Scores/Scores/AnnoncesOrdi.text = pt_ordi[2]
	$Scores/Scores/DixDerOrdi.text = pt_ordi[3]
	$Scores/Scores/TotalOrdi.text = pt_ordi[0]
	
	var screen_size = get_viewport_rect().size
	joueur_gagnees.type = Pile.TypePile.MAIN
	joueur_gagnees.size = Vector2(screen_size.x - 2*Settings.DEFAULT_CARD_SIZE.x, Settings.DEFAULT_CARD_SIZE.y)
	joueur_gagnees.position.x = Settings.DEFAULT_CARD_SIZE.x
	joueur_gagnees.calcul_positions(0)
	joueur_gagnees.spreadable = false
	#ordi_gagnees.inversser()
	ordi_gagnees.type = Pile.TypePile.MAIN
	ordi_gagnees.type_position = Pile.TypePosition.NORMAL
	ordi_gagnees.visibilite(true)
	ordi_gagnees.size = Vector2(screen_size.x - 2*Settings.DEFAULT_CARD_SIZE.x, Settings.DEFAULT_CARD_SIZE.y)
	ordi_gagnees.position.x = Settings.DEFAULT_CARD_SIZE.x
	ordi_gagnees.position.y = 0
	ordi_gagnees.calcul_positions(0)
	annonces_ordi.reset()
	annonces_ordi.visible = false
	annonces_joueur.reset()
	annonces_joueur.visible = false
	
	# gestion des points et manches
	if Global.nb_points > 1:
		if nb_points_joueur == 0:
			for i in range(Global.nb_points):
				jetons_joueur[i].texture = jeton_point_inactif
		if nb_points_ordi == 0:
			for i in range(Global.nb_points):
				jetons_ordi[i].texture = jeton_point_inactif
		for i in range(nb_points_joueur):
			jetons_joueur[i].texture = jeton_point_actif
		for i in range(nb_points_ordi):
			jetons_ordi[i].texture = jeton_point_actif
	if Global.nb_manches > 1:
		for i in range(nb_manches_joueur):
			manches_joueur[i].texture = manche_actif
		for i in range(nb_manches_ordi):
			manches_ordi[i].texture = manche_actif


func _partie_suivante():
	if partie_terminee == true:
		var error = get_tree().change_scene_to_file("res://Scenes/accueil.tscn")
		if error != OK:
			print("Scene change failed with error: ", error)
	else:
		demarrer_jeu()

func _on_button_pressed() -> void:
	confirmation_annuler.visible = true


func _on_confirmation_dialog_confirmed() -> void:
	confirmation_annuler.visible = true
	var error = get_tree().change_scene_to_file("res://Scenes/accueil.tscn")
	if error != OK:
		print("Scene change failed with error: ", error)


func _on_confirmation_dialog_canceled() -> void:
	confirmation_annuler.visible = true


func _on_annonce_pressed() -> void:
	choix_annonces.visible = true
