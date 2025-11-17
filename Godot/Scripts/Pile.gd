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
@export var spreadable: bool = true


var draggable: bool = true
var moving: bool = false
var dragging: bool = false
# si non adaptatif, alors les cartes ont une position fixe, quelque soit leurs nombre
var adaptatif: bool = true
var nb_cartes_max: int = 2
var duree_effet: float = Settings.DUREE_MOUVEMENT
var original_size: Vector2 = Vector2(0, 0)
var original_position: Vector2 = Vector2(0, 0)
var rng:RandomNumberGenerator = RandomNumberGenerator.new()
var spread_offset: Vector2 = Vector2(0, 0)

var _cartes: Array[Carte] = []

func _ready() -> void:
	get_tree().root.size_changed.connect(on_viewport_size_changed)


func on_viewport_size_changed() -> void:
	calcul_positions(0)


func init_cartes() -> void:
	_cartes = []
	
func cartes() -> Dictionary[String, Carte]:
	# retourne les cartes de la pile
	var ret: Dictionary[String, Carte] = {}
	for c: Carte in _cartes:
		ret[c.card_name] = c
	return ret


func inversser() -> void:
	_cartes.reverse()


func visibilite(_visible: bool) -> void:
	for c: Carte in _cartes:
		c.face_visible(_visible)


func afficher_cartes(carte: String) -> void:
	for c: Carte in _cartes:
		if c.card_name == carte:
			c.face_visible(true)
			var ori_pos: Vector2 = c.carte.position
			c.move(c.carte.position + Vector2(0, 200), -100, 1)
			await get_tree().create_timer(2.0).timeout
			c.move(ori_pos, -100, 0.5)
			await get_tree().create_timer(0.5).timeout
			c.face_visible(face_visible)

func supprimer_carte(nom: String) -> bool:
	for c: Carte in _cartes:
		if c.card_name == nom:
			_cartes.erase(c)
			return true
	return false

func ordre_cartes(ordre: PackedStringArray) -> void:
	var index: int = 0
	for nom_carte: String in ordre:
		if _cartes[index].card_name != nom_carte:
			# recherche ou est la carte que l'on recherche
			for i: int in range(index, _cartes.size()):
				if _cartes[i].card_name == nom_carte:
					# on peut echanger les cartes aux index: index et i
					var carte: Carte = _cartes[index]
					_cartes[index] = _cartes[i]
					_cartes[i] = carte
					break
		index += 1
	calcul_positions(duree_effet)

func ajouter_carte(carte: Carte, duration: float=-1.0) -> void:
	_cartes.push_back(carte)
	carte.face_visible(face_visible)
	var duree: float = duration
	if duration == -1.0:
		duree = duree_effet
	#carte.carte.z_index = z_order + _cartes.size() + 1
	if type == TypePile.PILE:
		var rnd_x: float = rng.randf_range(-1.0, 1.0) * size.x / 15
		var rnd_y: float = rng.randf_range(-1.0, 1.0) * size.y / 15
		carte.move(position + size/2 + Vector2(rnd_x, rnd_y),
			rng.randf_range(-1.0, 1.0) / 10,
			duree, 
			z_order + _cartes.size() + 1)
	else:
		calcul_positions(duree)

	carte.draggable = draggable

func calcul_positions(effet_duree: float) -> void:
	if type == TypePile.PILE:
		position_pile(effet_duree)
	elif type_position == TypePosition.NORMAL:
		position_normal(effet_duree)
	else:
		position_eventail(effet_duree)
		
func position_pile(effet_duree: float) -> void:
	for carte: Carte in _cartes:
		var rnd_x: float = rng.randf_range(-1.0, 1.0) * size.x / 15
		var rnd_y: float = rng.randf_range(-1.0, 1.0) * size.y / 15
		carte.move(position + size/2 + Vector2(rnd_x, rnd_y),
			rng.randf_range(-1.0, 1.0) / 10,
			effet_duree, 
			z_order + _cartes.size() + 1)

func position_normal(effet_duree: float) -> void:
	var nb_cartes: int = _cartes.size()
	if not adaptatif:
		nb_cartes = nb_cartes_max
	var slot_1: float = card_size.x/2 + (size.x - (card_size.x * nb_cartes))/(nb_cartes + 1)
	var diff_slot: float = card_size.x + (size.x - (card_size.x * nb_cartes))/(nb_cartes + 1)
	var index: int = 1
	for carte: Carte in _cartes:
		var x: float = position.x + slot_1 + (diff_slot * (index - 1))
		#var x = position.x + carte.size().x/2 + (size.x - (carte.size().x * index))/(index + 1)
		var y: float = position.y + size.y/2
		carte.move(Vector2(x, y), 0, effet_duree)
		index += 1
		
func position_eventail(effet_duree: float) -> void:
	var nb_cartes: int = _cartes.size()
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
	
	var rayon_x: float = size.x / 2
	var rayon_y: float = size.y / 2
	var angle: float = angle_max + PI/2
	var index: int = 0
	for carte: Carte in _cartes:
		var x: float = rayon_x * cos((angle))
		var y: float = -rayon_y * sin((angle))
		var inclinaison: float = PI/2 - angle
		if type_position == TypePosition.EVENTAIL_BAS:
			inclinaison -= PI
		carte.move(Vector2(x, y) + offset, inclinaison, effet_duree, z_order + index + 1)
		angle -= delta_angle
		index += 1

func _gui_input(event: InputEvent) -> void:
	if spreadable and (not moving) and event is InputEventMouseButton:
		if event.button_index == MOUSE_BUTTON_LEFT:
			if event.pressed:
				# Start dragging
				dragging = true
				spread_offset = get_global_mouse_position() - position
			else:
				# Stop dragging
				dragging = false
				_empiler()


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(_delta: float) -> void:
	if dragging and _cartes.size() > 1:
		var index: int = 0
		for carte: Carte in _cartes:
			var offset: Vector2 = index * (get_global_mouse_position() - position - spread_offset) / (_cartes.size() - 1)
			carte.move(position + size/2 + offset)
			index += 1

func _empiler() -> void:
	var index: int = 0
	for carte: Carte in _cartes:
		var rnd_x: float = rng.randf_range(-1.0, 1.0) * size.x / 15
		var rnd_y: float = rng.randf_range(-1.0, 1.0) * size.y / 15
		carte.move(position + size/2 + Vector2(rnd_x, rnd_y),
			rng.randf_range(-1.0, 1.0) / 10,
			0, 
			z_order + index + 1)
		index += 1
