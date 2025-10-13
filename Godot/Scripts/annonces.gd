extends Node2D

@onready var mariage_coeur: TextureRect = $Grid/MariageCoeur
@onready var mariage_carreau: TextureRect = $Grid/MariageCarreau
@onready var mariage_pic: TextureRect = $Grid/MariagePic
@onready var mariage_trefle: TextureRect = $Grid/MariageTrefle

@onready var tierce_coeur: TextureRect = $Grid/TierceCoeur
@onready var tierce_carreau: TextureRect = $Grid/TierceCarreau
@onready var tierce_pic: TextureRect = $Grid/TiercePic
@onready var tierce_trefle: TextureRect = $Grid/TierceTrefle

@onready var quarante_coeur: TextureRect = $Grid/QuaranteCoeur
@onready var quarante_carreau: TextureRect = $Grid/QuaranteCarreau
@onready var quarante_pic: TextureRect = $Grid/QuarantePic
@onready var quarante_trefle: TextureRect = $Grid/QuaranteTrefle

@onready var quinte_coeur: TextureRect = $Grid/QuinteCoeur

var coeur_rouge_texture = load("res://Assets/coeur-rouge.svg")
var coeur_vert_texture = load("res://Assets/coeur-vert.svg")
var coeur_gris_texture = load("res://Assets/coeur-gris.svg")
var carreau_rouge_texture = load("res://Assets/carreau-rouge.svg")
var carreau_vert_texture = load("res://Assets/carreau-vert.svg")
var carreau_gris_texture = load("res://Assets/carreau-gris.svg")
var pic_rouge_texture = load("res://Assets/pic-noir.svg")
var pic_vert_texture = load("res://Assets/pic-vert.svg")
var pic_gris_texture = load("res://Assets/pic-gris.svg")
var trefle_rouge_texture = load("res://Assets/trefle-noir.svg")
var trefle_gris_texture = load("res://Assets/trefle-gris.svg")
var trefle_vert_texture = load("res://Assets/trefle-vert.svg")

var annonces = {}
var partie = null

func _ready():
	annonces = {
		"mariage": {
			"coeur": mariage_coeur,
			"carreau": mariage_carreau,
			"pic": mariage_pic,
			"trefle": mariage_trefle
		},
		"tierce": {
			"coeur": tierce_coeur,
			"carreau": tierce_carreau,
			"pic": tierce_pic,
			"trefle": tierce_trefle
		},
		"quarante": {
			"coeur": quarante_coeur,
			"carreau": quarante_carreau,
			"pic": quarante_pic,
			"trefle": quarante_trefle
		},
		"quinte": {
			"coeur": quinte_coeur
		}
	}


var couleurs_rouge = {
	"coeur": coeur_rouge_texture,
	"carreau": carreau_rouge_texture,
	"pic": pic_rouge_texture,
	"trefle": trefle_rouge_texture,
}
var couleurs_gris = {
	"coeur": coeur_gris_texture,
	"carreau": carreau_gris_texture,
	"pic": pic_gris_texture,
	"trefle": trefle_gris_texture,
}
var couleurs_vert = {
	"coeur": coeur_vert_texture,
	"carreau": carreau_vert_texture,
	"pic": pic_vert_texture,
	"trefle": trefle_vert_texture,
}

func reset():
	for a in annonces:
		for c in annonces[a]:
			annonces[a][c].texture = couleurs_gris[c]

func annonces_autorisees(annonces_autorisee):
	for a in annonces_autorisee.split(" "):
		point(a, couleurs_vert)

func point(annonce, couleurs=couleurs_rouge):
	var ann = annonce.split("-")
	if ann[0] == "chouine":
		# rien à faire, la partie est terminee
		return
	if ann[0] == "quinte":
		ann[1] = "coeur"  # une seule quinte par partie possible
	if not annonces.has(ann[0]) or not annonces[ann[0]].has(ann[1]):
		print("Annnonce non reconnue :" + annonce)
		return
	var texture = annonces[ann[0]][ann[1]]
	texture.texture = couleurs[ann[1]]

func annonce_joueur(event: InputEvent, annonce: String) -> void:
	if event is InputEventMouseButton:
		if event.button_index == MOUSE_BUTTON_LEFT:
			if event.pressed and partie != null:
				partie.annonce_joueur(annonce)


func _on_mariage_coeur_gui_input(event: InputEvent) -> void:
	annonce_joueur(event, "mariage-coeur")


func _on_tierce_coeur_gui_input(event: InputEvent) -> void:
	annonce_joueur(event, "tierce-coeur")


func _on_quarante_coeur_gui_input(event: InputEvent) -> void:
	annonce_joueur(event, "quarante-coeur")


func _on_quinte_coeur_gui_input(event: InputEvent) -> void:
	annonce_joueur(event, "quinte-coeur")


func _on_mariage_carreau_gui_input(event: InputEvent) -> void:
	annonce_joueur(event, "mariage-carreau")


func _on_tierce_carreau_gui_input(event: InputEvent) -> void:
	annonce_joueur(event, "tierce-carreau")


func _on_quarante_carreau_gui_input(event: InputEvent) -> void:
	annonce_joueur(event, "quarante-carreau")


func _on_mariage_pic_gui_input(event: InputEvent) -> void:
	annonce_joueur(event, "mariage-pic")


func _on_tierce_pic_gui_input(event: InputEvent) -> void:
	annonce_joueur(event, "tierce-pic")


func _on_quarante_pic_gui_input(event: InputEvent) -> void:
	annonce_joueur(event, "quarante-pic")


func _on_mariage_trefle_gui_input(event: InputEvent) -> void:
	annonce_joueur(event, "mariage-trefle")


func _on_tierce_trefle_gui_input(event: InputEvent) -> void:
	annonce_joueur(event, "tierce-trefle")


func _on_quarante_trefle_gui_input(event: InputEvent) -> void:
	annonce_joueur(event, "quarante-trefle")
