extends Node2D
class_name Annonces

var partie: Variant = null
var annonces: Dictionary[String, TextureRect] = {}
var milieu_tapis: Vector2 = Vector2(0, 0)

func _ready() -> void:
	var json_as_text: String = FileAccess.get_file_as_string("res://Assets/Data/annonces.json")
	var _annonces: Variant = JSON.parse_string(json_as_text)
	for annonce: String in _annonces:
		if annonce != "quinte":
			for couleur: String in _annonces[annonce]:
				var image: Resource = load("res://Assets/Annonces/" + _annonces[annonce][couleur])
				var TR: TextureRect = TextureRect.new()
				TR.texture = image
				TR.expand_mode = TextureRect.EXPAND_IGNORE_SIZE
				TR.custom_minimum_size = Vector2(Settings.TAILLE_ANNONCE, Settings.TAILLE_ANNONCE)  # Set size
				TR.stretch_mode = TextureRect.STRETCH_KEEP_ASPECT_CENTERED
				TR.name = annonce + "-" + couleur
				annonces[annonce + "-" + couleur] = TR
		else:
			var image: Resource = load("res://Assets/Annonces/" + _annonces[annonce])
			var TR: TextureRect = TextureRect.new()
			TR.texture = image
			TR.z_index = 1000
			TR.expand_mode = TextureRect.EXPAND_IGNORE_SIZE
			TR.custom_minimum_size = Vector2(Settings.TAILLE_ANNONCE, Settings.TAILLE_ANNONCE)  # Set size
			TR.stretch_mode = TextureRect.STRETCH_KEEP_ASPECT_CENTERED
			TR.name = annonce
			annonces[annonce] = TR

func set_milieu_tapis(pos: Vector2) -> void:
	milieu_tapis = pos

func reset() -> void:
	# reset pour remettre à jour
	var grid: Node = $Panel/Grid
	var children: Array[Node] = grid.get_children()
	for c: Node in children:
		grid.remove_child(c)
		# c.queue_free()

func add(annonce: String) -> void:
	if annonces.has(annonce):
		# get final position
		var annonce_size: Vector2 = Vector2(Settings.TAILLE_ANNONCE, Settings.TAILLE_ANNONCE)
		annonces[annonce].visible = true
		annonces[annonce].z_index = 200
		annonces[annonce].position = milieu_tapis - position - annonce_size/2
		add_child(annonces[annonce])
		var tween: Tween = get_tree().create_tween().set_parallel()
		var position_finale: Vector2 = milieu_tapis - position - 4*annonce_size
		tween.tween_property(annonces[annonce], "position", position_finale, 1).set_ease(Tween.EASE_OUT).set_trans(Tween.TRANS_CIRC)
		tween.tween_property(annonces[annonce], "scale", Vector2(8, 8), 1).set_ease(Tween.EASE_OUT).set_trans(Tween.TRANS_CIRC)
		position_finale = $Panel.position + $Panel.size/2 - annonce_size/2
		tween.chain().tween_property(annonces[annonce], "position", position_finale, 1).set_ease(Tween.EASE_OUT).set_trans(Tween.TRANS_CIRC)
		tween.set_parallel()
		tween.tween_property(annonces[annonce], "scale", Vector2(1, 1), 1).set_ease(Tween.EASE_OUT).set_trans(Tween.TRANS_CIRC)
		tween.chain()
		await tween.finished
		remove_child(annonces[annonce])
		annonces[annonce].position = Vector2(0, 0)
		$Panel/Grid.add_child(annonces[annonce])
