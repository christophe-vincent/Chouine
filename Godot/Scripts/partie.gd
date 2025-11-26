@tool
extends Control
class_name Partie

# Scene principale
@onready var pioche: Pile = $Pioche
@onready var retourne: Pile = $Piles/Retourne
@onready var main_joueur: Pile = $Piles/MainJoueur
@onready var main_ordi: Pile = $Piles/MainOrdi
@onready var tapis: Pile = $DeposeCartes
@onready var joueur_gagnees: Pile = $Piles/JoueurGagnees
@onready var ordi_gagnees: Pile = $Piles/OrdiGagnees
@onready var zone_jeu: CollisionShape2D = $ZoneJeu/ZoneJeu/CollisionShape2D
@onready var annonces_ordi: Annonces = $AnnoncesOrdi
@onready var annonces_joueur: Annonces = $AnnoncesJoueur
@onready var choix_annonces: Control = $ChoixAnnonces
@onready var confirmation_annuler: ConfirmationDialog = $Annuler/ConfirmationDialog

var card_packed_scene: Resource = preload("res://Scenes/carte.tscn")
var score_scene: Resource = preload("res://Scenes/scores.tscn")
var cartes: Dictionary[String, Carte] = {}
var mains: Dictionary[int, Control] = {}
var cartes_pioche: Array[String] = []
var chouine: IChouine = IChouine.new()
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
var joueur_gagnees_size: Vector2 = Vector2(0, 0)
var ordi_gagnees_size: Vector2 = Vector2(0, 0)
var joueur_gagnees_position: Vector4 = Vector4(0, 0, 0, 0)
var ordi_gagnees_position: Vector4 = Vector4(0, 0, 0, 0)


enum JOUEURS {
	ORDI = 0,
	HUMAIN = 1
}

func _ready() -> void:
	mains = {0: main_ordi, 1: main_joueur}
	joueur_gagnees_size = joueur_gagnees.size
	joueur_gagnees_position = Vector4(
		joueur_gagnees.anchor_bottom,
		joueur_gagnees.anchor_left,
		joueur_gagnees.anchor_right,
		joueur_gagnees.anchor_top)
	ordi_gagnees_size = ordi_gagnees.size
	ordi_gagnees_position = Vector4(
		ordi_gagnees.anchor_bottom,
		ordi_gagnees.anchor_left,
		ordi_gagnees.anchor_right,
		ordi_gagnees.anchor_top)
	nb_points_ordi = 0
	nb_points_joueur = 0
	nb_manches_ordi = 0
	nb_manches_joueur = 0
	restauration_partie()
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
	annonces_ordi.set_milieu_tapis(tapis.position + tapis.size/2)
	annonces_joueur.set_milieu_tapis(tapis.position + tapis.size/2)
	_display_points()
	$Scores/Continuer.connect("pressed", _partie_suivante.bind())
	creer_cartes()
	get_tree().root.size_changed.connect(on_viewport_size_changed)
	demarrer_jeu()
	

func on_viewport_size_changed() -> void:
	annonces_ordi.set_milieu_tapis(tapis.position + tapis.size/2)
	annonces_joueur.set_milieu_tapis(tapis.position + tapis.size/2)

func creer_cartes() -> void:
	# lecture du fichier de la description des cartes
	var json_as_text: String = FileAccess.get_file_as_string("res://Assets/Data/jeu-carte.json")
	var jeu_cartes: Variant = JSON.parse_string(json_as_text)
	for couleur: String in jeu_cartes:
		for valeur: String in jeu_cartes[couleur]:
			var nom_carte: String = valeur + "-" + couleur
			if not cartes.has(nom_carte):
				var card_instance: Node = card_packed_scene.instantiate()
				card_instance.card_name = nom_carte
				card_instance.front_image = load("res://Assets/Cartes/" + jeu_cartes[couleur][valeur]['image'])
				card_instance.back_image = load("res://Assets/Cartes/back_1.svg")
				card_instance.show_front = false
				card_instance.position = tapis.position + tapis.size/2
				add_child(card_instance)
				cartes[nom_carte] = card_instance

func init_jeu() -> void:
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
	main_ordi.face_visible = Global.cartes_visibles
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
	joueur_gagnees.set_anchor_and_offset(SIDE_LEFT, joueur_gagnees_position.y, -Settings.DEFAULT_CARD_SIZE.x/2, false)
	joueur_gagnees.set_anchor_and_offset(SIDE_RIGHT, joueur_gagnees_position.z, Settings.DEFAULT_CARD_SIZE.x/2, false)

	ordi_gagnees.card_size = Settings.DEFAULT_CARD_SIZE
	ordi_gagnees.type = Pile.TypePile.PILE
	ordi_gagnees.type_position = Pile.TypePosition.EVENTAIL_BAS
	ordi_gagnees.draggable = false
	ordi_gagnees.face_visible = false
	ordi_gagnees.init_cartes()
	ordi_gagnees.size = ordi_gagnees_size
	ordi_gagnees.set_anchor_and_offset(SIDE_LEFT, ordi_gagnees_position.y, -Settings.DEFAULT_CARD_SIZE.x/2, false)
	ordi_gagnees.set_anchor_and_offset(SIDE_RIGHT, ordi_gagnees_position.z, Settings.DEFAULT_CARD_SIZE.x/2, false)
	
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
	var c: String = cartes_pioche.pop_back()
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
	zone_jeu.disabled = false
	coup_joueur = true


func melanger() -> void:
	cartes_pioche = []
	for c: String in Array(chouine.partie().split(" ")):
		await get_tree().create_timer(0.05).timeout
		c = c.replace('*', '')
		cartes_pioche.append(c)
		pioche.ajouter_carte(cartes[c])


#
# Sauvegarde de la partie en cours
#
func enregistrer_partie() -> void:
	"""Enregistre une nouvelle partie"""
	var save_file: FileAccess = FileAccess.open(Settings.SAVE_FILE, FileAccess.WRITE)
	var points: Dictionary = {
		'points': Global.nb_points,
		'points_joueur': nb_points_joueur,
		'points_ordi': nb_points_ordi,
		'manches': Global.nb_manches,
		'manches_joueur': nb_manches_joueur,
		'manches_ordi': nb_manches_ordi
	}
	save_file.store_line(JSON.stringify(points))


func restauration_partie() -> bool:
	if not FileAccess.file_exists(Settings.SAVE_FILE):
		print("Pas de backup")
		return false

	var save_file: FileAccess = FileAccess.open(Settings.SAVE_FILE, FileAccess.READ)
	while save_file.get_position() < save_file.get_length():
		var json_string: String = save_file.get_line()
		var json: JSON = JSON.new()
		var parse_result: Error = json.parse(json_string)
		if not parse_result == OK:
			print("Error de backup de la partie")
			return false
		Global.nb_points = int(json.data['points'])
		nb_points_joueur = int(json.data['points_joueur'])
		nb_points_ordi = int(json.data['points_ordi'])
		print(nb_points_joueur)
		Global.nb_manches = json.data['manches']
		nb_manches_joueur = json.data['manches_joueur']
		nb_manches_ordi = json.data['manches_ordi']
	return true

func enregistrer_action() -> void:
	pass
	
func supprimer_sauvegarde() -> void:
	if FileAccess.file_exists(Settings.SAVE_FILE):
		DirAccess.remove_absolute(Settings.SAVE_FILE)

#
# Actions utilisateur
#
func carte_jouee(nom: String) -> int:
	"""Une carte est jouée par le joueur"""
	if coup_joueur == false:
		return -1
	print ("Choix joueur: " + nom)
	if carte_atout != null && carte_atout.card_name == nom:
		print("ERREUR: Carte d'atout déposée sur la zone de jeu")
		return -1
	var ret: int = chouine.set_choix_joueur(nom)
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


func carte_main_joueur(nom: String) -> int:
	"""une carte est déposée dans la main du joueur, ca ne peut être que la carte d'atout"""
	if carte_atout != null && nom == carte_atout.card_name:
		# chercher le sept d'atout
		main_joueur.ajouter_carte(carte_atout)
		main_joueur.supprimer_carte(sept_atout.card_name)
		main_joueur.calcul_positions(Settings.DUREE_MOUVEMENT)
		retourne.ajouter_carte(sept_atout)
		retourne.init_cartes()
		chouine.echanger_carte_atout(JOUEURS.HUMAIN)
		print("Joueur      : Echange carte atout: ", carte_atout.card_name)
		cartes_pioche.pop_front()
		cartes_pioche.push_front(sept_atout.card_name)
		carte_atout = null
		sept_atout.draggable = false
		choix_annonces.annonces_autorisees(chouine.annonces_en_main_joueur(1))
		return 0
	return -1

func annonce_joueur(annonce: String) -> void:
	"""Le joueur a validé une annonce"""
	var ret: int = chouine.annonce_joueur(annonce)
	if ret == 0:
		# annonce autorisee
		annonces_joueur.add(annonce)
		print("Joueur      : " + annonce)
		if chouine.fin_partie():
			fin_pli()

#
# Actions de l'ordinateur
#
func annonce_ordi(_annonce: String) -> void:
	annonces_ordi.add(_annonce)
	var annonce: String = _annonce
	var couleur: String = ""
	if "-" in _annonce:
		couleur = _annonce.split("-")[1]
		annonce = _annonce.split("-")[0]
	if annonce == "quarante":
		main_ordi.afficher_cartes("as-" + couleur)
		annonce = "tierce"
	if annonce == "tierce":
		main_ordi.afficher_cartes("valet-" + couleur)
		annonce = "mariage"
	if annonce == "mariage":
		main_ordi.afficher_cartes("roi-" + couleur)
		main_ordi.afficher_cartes("dame-" + couleur)
	if annonce == "quinte" or annonce == "chouine":
		for c: String in main_ordi.cartes():
			main_ordi.afficher_cartes(c)
	await get_tree().create_timer(4.0).timeout
	main_ordi.calcul_positions(0.5)
	await get_tree().create_timer(0.5).timeout


func coup_ordi() -> void:
	var choix_ordi: Array = chouine.choix_joueur().split("|")
	var choix_carte: String = choix_ordi[0]
	print("Choix Ordi  : " + choix_carte)
	var annonce: String = choix_ordi[1]
	if annonce != "":
		print("Annonce Ordi: " + annonce)
		await annonce_ordi(annonce)

	var echange_sept_atout: String = choix_ordi[2]
	if echange_sept_atout != "" && carte_atout != null:
		if retourne.cartes().size() == 0:
			print("ERREUR: La retourne est vide")
		else:
			# la carte de joueur rejoint la retourne
			main_ordi.supprimer_carte(sept_atout.card_name)
			main_ordi.ajouter_carte(carte_atout)
			main_ordi.calcul_positions(Settings.DUREE_MOUVEMENT)
			retourne.init_cartes()
			retourne.ajouter_carte(sept_atout)
			print("Ordi        : Echange sept atout - ", carte_atout.card_name)
			cartes_pioche.pop_front()
			cartes_pioche.push_front(sept_atout.card_name)
			carte_atout = null
			sept_atout.draggable = true

	main_ordi.supprimer_carte(choix_carte)
	tapis.ajouter_carte(cartes[choix_carte])
	# est-ce la fin du pli ?
	var ret: int = chouine.fin_pli()
	if ret >= 0:
		fin_pli()
	else:
		coup_joueur = true


#
# Gestion du pli
#
func distribution_carte(joueur: int) -> void:
	"""Une carte est donnée à un joueur"""
	await get_tree().create_timer(Settings.DUREE_DISTRIBUTION).timeout
	var c: String = cartes_pioche.pop_back()
	var ret: bool = pioche.supprimer_carte(c)
	if ret == false:
		retourne.init_cartes()
	mains[joueur].ajouter_carte(cartes[c])
	# Si la carte d'atout se retrouve dans une main alors elle devient une carte normale
	if carte_atout != null && carte_atout.card_name == c:
		carte_atout = null

func distribution_cartes(nb_cartes: int) -> bool:
	"""Des cartes sont distribuées aux joueurs"""
	var joueur: int = chouine.gagnant_pli()
	if cartes_pioche.size() == 0:
		main_joueur.calcul_positions(Settings.DUREE_MOUVEMENT)
		main_ordi.calcul_positions(Settings.DUREE_MOUVEMENT)
		return false
	for i: int in range(nb_cartes):
		await distribution_carte(joueur)
		joueur = (joueur + 1) % 2
		await distribution_carte(joueur)
		joueur = (joueur + 1) % 2
	
	# on affiche les carte de la main du joueur dans l'ordre donné par l'algo
	chouine.trier_cartes(JOUEURS.HUMAIN)
	var cartes_joueur_str: PackedStringArray = chouine.cartes_joueur(JOUEURS.HUMAIN).split(" ")
	main_joueur.ordre_cartes(cartes_joueur_str)
	
	# le sept d'atout est-il dans la main du joueur ?
	var sept_atout_en_main: bool = chouine.sept_atout_en_main(JOUEURS.HUMAIN)
	if sept_atout_en_main && carte_atout != null && cartes_pioche.size() > 0:
		carte_atout.draggable = true
	return true


func fin_pli() -> void:
	"""Le dernier joueur a posé sa carte"""
	await get_tree().create_timer(1.0).timeout
	var gagnant: int = chouine.gagnant_pli()
	var cartes_tapis: Dictionary[String, Carte] = tapis.cartes()
	for c: String in cartes_tapis:
		tapis.supprimer_carte(c)
	if gagnant == JOUEURS.ORDI:
		print("Gagnant     : Ordi")
		for c: String in cartes_tapis:
			ordi_gagnees.ajouter_carte(cartes_tapis[c])
	else:
		print("Gagnant     : Joueur")
		for c: String in cartes_tapis:
			joueur_gagnees.ajouter_carte(cartes_tapis[c])

	if chouine.fin_partie() == 1:
		# fin de la partie
		await get_tree().create_timer(2.0).timeout
		fin_partie()
		return

	print(" ")
	await distribution_cartes(1)
	choix_annonces.annonces_autorisees(chouine.annonces_en_main_joueur(1))

	await get_tree().create_timer(1.0).timeout
	
	# debug: affiche les cartes
	print("Ordi        : " + chouine.cartes_joueur(0))
	print("Joueur      : " + chouine.cartes_joueur(1))
	if gagnant == JOUEURS.ORDI:
		coup_ordi()
	else:
		coup_joueur = true

#
# Gestion des parties
#
func fin_partie() -> void:
	zone_jeu.disabled = true
	$Scores/Defaite.visible = false
	$Scores/Victoire.visible = false
	$Scores.visible = true
	for c: String in main_ordi.cartes():
		ordi_gagnees.ajouter_carte(main_ordi.cartes()[c])
	main_ordi.init_cartes()
	for c: String in main_joueur.cartes():
		joueur_gagnees.ajouter_carte(main_joueur.cartes()[c])
	main_joueur.init_cartes()
	
	if chouine.points_joueur(JOUEURS.ORDI) > chouine.points_joueur(JOUEURS.HUMAIN):
		nb_points_ordi += 1
		$Scores/Info.add_theme_color_override("font_color", Color.RED)
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
		$Scores/Info.add_theme_color_override("font_color", Color.GREEN)
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
	
	var size_main: float = get_viewport_rect().size.x - 2*Settings.DEFAULT_CARD_SIZE.x
	joueur_gagnees.type = Pile.TypePile.MAIN
	joueur_gagnees.size = Vector2(size_main, Settings.DEFAULT_CARD_SIZE.y)
	joueur_gagnees.set_anchor_and_offset(SIDE_LEFT, 0.5, -size_main/2, true)
	joueur_gagnees.set_anchor_and_offset(SIDE_RIGHT, 0.5, size_main/2, true)
	
	joueur_gagnees.calcul_positions(0)
	joueur_gagnees.spreadable = false
	#ordi_gagnees.inversser()
	ordi_gagnees.type = Pile.TypePile.MAIN
	ordi_gagnees.type_position = Pile.TypePosition.NORMAL
	ordi_gagnees.visibilite(true)
	ordi_gagnees.size = Vector2(size_main, Settings.DEFAULT_CARD_SIZE.y)
	ordi_gagnees.set_anchor_and_offset(SIDE_LEFT, 0.5, -size_main/2, true)
	ordi_gagnees.set_anchor_and_offset(SIDE_RIGHT, 0.5, size_main/2, true)
	ordi_gagnees.calcul_positions(0)
	annonces_ordi.reset()
	annonces_ordi.visible = false
	annonces_joueur.reset()
	annonces_joueur.visible = false
	# on enregistre les scores
	enregistrer_partie()
	
	# gestion des jetons points et manches
	_display_points()


func _display_points() -> void:
	if Global.nb_points > 1:
		if nb_points_joueur == 0:
			for i: int in range(Global.nb_points):
				jetons_joueur[i].texture = jeton_point_inactif
		if nb_points_ordi == 0:
			for i: int in range(Global.nb_points):
				jetons_ordi[i].texture = jeton_point_inactif
		for i: int in range(nb_points_joueur):
			jetons_joueur[i].texture = jeton_point_actif
		for i: int in range(nb_points_ordi):
			jetons_ordi[i].texture = jeton_point_actif
	if Global.nb_manches > 1:
		for i: int in range(nb_manches_joueur):
			manches_joueur[i].texture = manche_actif
		for i: int in range(nb_manches_ordi):
			manches_ordi[i].texture = manche_actif


func _partie_suivante() -> void:
	if partie_terminee == true:
		retour_accueil()
	else:
		demarrer_jeu()

func _on_button_pressed() -> void:
	confirmation_annuler.visible = true


func _on_confirmation_dialog_confirmed() -> void:
	confirmation_annuler.visible = true
	retour_accueil()

func _on_confirmation_dialog_canceled() -> void:
	confirmation_annuler.visible = true

func _on_annonce_pressed() -> void:
	choix_annonces.visible = true

func retour_accueil() -> void:
	supprimer_sauvegarde()
	var error: Error = get_tree().change_scene_to_file("res://Scenes/accueil.tscn")
	if error != OK:
		print("Scene change failed with error: ", error)
