extends ColorRect
class_name Pile

enum TypePile {
	PILE,
	MAIN
}

@export var Type: TypePile
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

var _cartes = {}

func init_cartes():
	_cartes = {}
	
func cartes():
	# retourne les cartes de la pile
	var ret = {}
	for c in _cartes:
		ret[c] = _cartes[c]['carte']
	return ret

func visibilite(_visible: bool):
	for c in _cartes:
		_cartes[c].carte.face_visible(_visible)

func supprimer_carte(nom):
	_cartes.erase(nom)

func ajouter_carte(carte: Carte, duration=-1.0):
	_cartes[carte.card_name] = {"carte": carte, "index": _cartes.size()+1}
	carte.face_visible(face_visible)
	var duree = duration
	if duration == -1.0:
		duree = duree_effet
	carte.carte.z_index = z_order + _cartes.size()+1
	if Type == TypePile.PILE:
		carte.move(position + size/2, duree)
	else:
		calcul_positions(duree)

	carte.draggable = draggable
	pass 

func calcul_positions(effet_duree):
	var nb_cartes = _cartes.size()
	if not adaptatif:
		nb_cartes = nb_cartes_max
	var slot_1 = card_size.x/2 + (size.x - (card_size.x * nb_cartes))/(nb_cartes + 1)
	var diff_slot = card_size.x + (size.x - (card_size.x * nb_cartes))/(nb_cartes + 1)
	var index: int = 1
	for c in _cartes:
		#var index = _cartes[c]['index']
		var carte = _cartes[c]['carte']
		var x = position.x + slot_1 + (diff_slot * (index - 1))
		#var x = position.x + carte.size().x/2 + (size.x - (carte.size().x * index))/(index + 1)
		var y = position.y + size.y/2
		carte.move(Vector2(x, y), effet_duree)
		index += 1
