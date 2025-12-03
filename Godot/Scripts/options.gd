extends Control
class_name Options

@onready var Dos1: PanelContainer = $DosCartes/Dos1
@onready var Dos2: PanelContainer = $DosCartes/Dos2
@onready var Dos3: PanelContainer = $DosCartes/Dos3
@onready var Dos4: PanelContainer = $DosCartes/Dos4
@onready var Dos5: PanelContainer = $DosCartes/Dos5
@onready var Dos6: PanelContainer = $DosCartes/Dos6
@onready var Jeton1: TextureRect = $DosCartes/Dos1/MarginContainer/VBoxContainer/ChoixDos
@onready var Jeton2: TextureRect = $DosCartes/Dos2/MarginContainer/VBoxContainer/ChoixDos
@onready var Jeton3: TextureRect = $DosCartes/Dos3/MarginContainer/VBoxContainer/ChoixDos
@onready var Jeton4: TextureRect = $DosCartes/Dos4/MarginContainer/VBoxContainer/ChoixDos
@onready var Jeton5: TextureRect = $DosCartes/Dos5/MarginContainer/VBoxContainer/ChoixDos
@onready var Jeton6: TextureRect = $DosCartes/Dos6/MarginContainer/VBoxContainer/ChoixDos

var selected_style: StyleBoxFlat = StyleBoxFlat.new()
var non_selected_style: StyleBoxFlat = StyleBoxFlat.new()
var jeton_actif: Resource = load("res://Assets/point_partie_actif.png")
var jeton_inactif: Resource = load("res://Assets/point_partie_inactif.png")
var dos: Array[PanelContainer] = []
var jetons_dos: Array[TextureRect] = []

func _ready() -> void:
	dos = [Dos1, Dos2, Dos3, Dos4, Dos5, Dos6]
	jetons_dos = [Jeton1, Jeton2, Jeton3, Jeton4, Jeton5, Jeton6]
	selected_style.bg_color = Color(0.635, 0.635, 0.635, 0.196)
	selected_style.border_width_left = 1
	selected_style.border_width_top = 1
	selected_style.border_width_right = 1
	selected_style.border_width_bottom = 1
	selected_style.border_color = Color.WHITE_SMOKE
	selected_style.set_corner_radius_all(30)
	non_selected_style.bg_color = Color(0, 0, 0, 0)
	select_dos(Global.options["dos_carte"])
	$CartesVisibles.button_pressed = Global.options["cartes_visibles"]


func select_dos(index: int) -> void:
	Global.options["dos_carte"] = index
	Global.enregistrer_options()
	for i: int in range(dos.size()):
		# modifier le background
		if i == index:
			dos[i].add_theme_stylebox_override("panel", selected_style)
			jetons_dos[i].texture = jeton_actif
		else:
			dos[i].add_theme_stylebox_override("panel", non_selected_style)
			jetons_dos[i].texture = jeton_inactif

func _on_dos1_pressed() -> void:
	select_dos(0)

func _on_dos2_pressed() -> void:
	select_dos(1)

func _on_dos3_pressed() -> void:
	select_dos(2)

func _on_dos4_pressed() -> void:
	select_dos(3)

func _on_dos5_pressed() -> void:
	select_dos(4)

func _on_dos6_pressed() -> void:
	select_dos(5)


func _on_acceuil_pressed() -> void:
	var error: Error = get_tree().change_scene_to_file("res://Scenes/accueil.tscn")
	if error != OK:
		print("Scene change failed with error: ", error)


func _on_cartes_visibles_pressed() -> void:
	Global.options["cartes_visibles"] = $CartesVisibles.button_pressed
	Global.enregistrer_options()
