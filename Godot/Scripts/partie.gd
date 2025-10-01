@tool
extends Control
# Scene principale
@onready var card_container: Control = $CardContainer
@onready var drop_zone: Area2D = $DropZone

#var card_packed_scene = preload("res://Scenes/carte.tscn")
var card_packed_scene = preload("res://Scenes/carte.tscn")
var cartes = {}

func _ready():
	setup_game()

func setup_game():
	# read card info file
	pass
	#var card_instance = card_packed_scene.instantiate()
	#card_instance.card_size = Vector2(120, 180)
	#card_instance.position = Vector2(100, 100)
	#add_child(card_instance)
	# move_child(card_instance, 0)

func carte_jouee(name):
	print(name)
	
func mouse_released():
	print("Carte jouée: ")
	print(drop_zone.cards_inside)


func _on_texture_button_pressed() -> void:
	# remove cards
	for k in cartes:
		remove_child(cartes[k])
	cartes.clear()
	
	# lecture du fichier de la description des cartes
	var json_as_text = FileAccess.get_file_as_string("res://Assets/Data/jeu-carte.json")
	var jeu_cartes = JSON.parse_string(json_as_text)
	var x = 100
	var y = 100
	for couleur in jeu_cartes:
		for valeur in jeu_cartes[couleur]:
			var nom_carte = couleur + "-" + valeur
			if not cartes.has(nom_carte):
				var card_instance = card_packed_scene.instantiate()
				card_instance.card_name = nom_carte
				card_instance.front_image = load("res://Assets/Cartes/" + jeu_cartes[couleur][valeur]['image'])
				card_instance.position = Vector2(x, y)
				add_child(card_instance)
				cartes[nom_carte] = card_instance
				x += 10
				y += 10
		x += 120
		y = 100
	print(cartes)
		
	pass # Replace with function body.
