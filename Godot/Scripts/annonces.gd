extends Node2D
class_name Annonces

var partie = null
var annonces = {}

func _ready():
	var json_as_text = FileAccess.get_file_as_string("res://Assets/Data/annonces.json")
	var _annonces = JSON.parse_string(json_as_text)
	for annonce in _annonces:
		print(annonce)
		if annonce != "quinte":
			for couleur in _annonces[annonce]:
				var image := load("res://Assets/Annonces/" + _annonces[annonce][couleur])
				var TR := TextureRect.new()
				TR.texture = image
				TR.expand_mode = TextureRect.EXPAND_IGNORE_SIZE
				TR.custom_minimum_size = Vector2(Settings.TAILLE_ANNONCE, Settings.TAILLE_ANNONCE)  # Set size
				TR.stretch_mode = TextureRect.STRETCH_KEEP_ASPECT_CENTERED
				TR.name = annonce + "-" + couleur
				annonces[annonce + "-" + couleur] = TR
		else:
			var image := load("res://Assets/Annonces/" + _annonces[annonce])
			var TR := TextureRect.new()
			TR.texture = image
			TR.expand_mode = TextureRect.EXPAND_IGNORE_SIZE
			TR.custom_minimum_size = Vector2(Settings.TAILLE_ANNONCE, Settings.TAILLE_ANNONCE)  # Set size
			TR.stretch_mode = TextureRect.STRETCH_KEEP_ASPECT_CENTERED
			TR.name = annonce
			annonces[annonce] = TR

func reset():
	# reset pour remettre à jour
	var grid := $Panel/Grid
	var children = grid.get_children()
	for c in children:
		self.remove_child(c)
		c.queue_free()

func add(annonce):
	if annonces.has(annonce):
		$Panel/Grid.add_child(annonces[annonce])
