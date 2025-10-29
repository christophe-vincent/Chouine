extends ColorRect
class_name Pile

enum TypePile {
	PILE,
	MAIN
}

enum TypePosition {
	NORMAL,
	EVENTAIL_HAUT,
	EVENTAIL_BAS
}

@export var type: TypePile
@export var type_position: TypePosition
@export var card_size: Vector2
@export var z_order: int

@export var face_visible: bool = true
var draggable = true
# si non adaptatif, alors les cartes ont une position fixe, quelque soit leurs nombre
var adaptatif = true
var nb_cartes_max = 2
var duree_effet = Settings.DUREE_MOUVEMENT
var original_size: Vector2 = Vector2(0, 0)
var original_position: Vector2 = Vector2(0, 0)
var rng = RandomNumberGenerator.new()

var _cartes: Array = []

func init_cartes():
	_cartes = []
	
func cartes():
	# retourne les cartes de la pile
	var ret = {}
	for c in _cartes:
		ret[c.card_name] = c
	return ret


func inversser():
	_cartes.reverse()


func visibilite(_visible: bool):
	for c in _cartes:
		c.face_visible(_visible)

func supprimer_carte(nom: String):
	for c in _cartes:
		if c.card_name == nom:
			_cartes.erase(c)

func ordre_cartes(ordre: Array):
	var index: int = 0
	for nom_carte in ordre:
		if _cartes[index].card_name != nom_carte:
			# recherche ou est la carte que l'on recherche
			for i in range(index, _cartes.size()):
				if _cartes[i].card_name == nom_carte:
					# on peut echanger les cartes aux index: index et i
					var carte: Carte = _cartes[index]
					_cartes[index] = _cartes[i]
					_cartes[i] = carte
					break
		index += 1
	calcul_positions(duree_effet)

func ajouter_carte(carte: Carte, duration=-1.0):
	_cartes.push_back(carte)
	carte.face_visible(face_visible)
	var duree = duration
	if duration == -1.0:
		duree = duree_effet
	#carte.carte.z_index = z_order + _cartes.size() + 1
	if type == TypePile.PILE:
		var rnd_x = rng.randf_range(-1.0, 1.0) * size.x / 15
		var rnd_y = rng.randf_range(-1.0, 1.0) * size.y / 15
		carte.move(position + size/2 + Vector2(rnd_x, rnd_y),
			rng.randf_range(-1.0, 1.0) / 10,
			duree, 
			z_order + _cartes.size() + 1)
	else:
		calcul_positions(duree)

	carte.draggable = draggable
	pass 

func calcul_positions(effet_duree):
	if type_position == TypePosition.NORMAL:
		position_normal(effet_duree)
	else:
		position_eventail(effet_duree)

func position_normal(effet_duree):
	var nb_cartes = _cartes.size()
	if not adaptatif:
		nb_cartes = nb_cartes_max
	var slot_1 = card_size.x/2 + (size.x - (card_size.x * nb_cartes))/(nb_cartes + 1)
	var diff_slot = card_size.x + (size.x - (card_size.x * nb_cartes))/(nb_cartes + 1)
	var index: int = 1
	for carte in _cartes:
		var x = position.x + slot_1 + (diff_slot * (index - 1))
		#var x = position.x + carte.size().x/2 + (size.x - (carte.size().x * index))/(index + 1)
		var y = position.y + size.y/2
		carte.move(Vector2(x, y), 0, effet_duree)
		index += 1
		
func position_eventail(effet_duree):
	var nb_cartes = _cartes.size()
	if not adaptatif:
		nb_cartes = nb_cartes_max
		
	# angle de la carte la plus à droite
	var angle_max: float = nb_cartes*(PI/4)/5
	if nb_cartes == 1:
		angle_max = 0
	# angle de l'eventail
	var angle_eventail: float = 2 * angle_max
	# angle de la carte la plus à gauche
	var offset: Vector2 = Vector2(position.x + size.x/2, position.y)
	# angle entre 2 cartes
	var delta_angle: float = angle_eventail / (nb_cartes - 1)
	
	if type_position == TypePosition.EVENTAIL_BAS:
		angle_max -= PI
		offset.y += size.y/2 - card_size.y/2
	else:
		offset.y += card_size.y
	
	var rayon_x = size.x / 2
	var rayon_y = size.y / 2
	var angle: float = angle_max + PI/2
	var index: int = 0
	for carte in _cartes:
		var x: float = rayon_x * cos((angle))
		var y: float = -rayon_y * sin((angle))
		var inclinaison: float = PI/2 - angle
		if type_position == TypePosition.EVENTAIL_BAS:
			inclinaison -= PI
		carte.move(Vector2(x, y) + offset, inclinaison, effet_duree, z_order + index + 1)
		angle -= delta_angle
		index += 1
