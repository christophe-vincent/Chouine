extends Node
class_name GameSave

var lut_cartes: Dictionary = {
	"7-coeur" : 0,
	"8-coeur" : 1,
	"9-coeur" : 2,
	"valet-coeur" : 3,
	"dame-coeur" : 4,
	"roi-coeur" : 5,
	"10-coeur" : 6,
	"as-coeur" : 7,
	"7-carreau" : 8,
	"8-carreau" : 9,
	"9-carreau" : 10,
	"valet-carreau" : 11,
	"dame-carreau" : 12,
	"roi-carreau" : 13,
	"10-carreau" : 14,
	"as-carreau" : 15,
	"7-pic" : 16,
	"8-pic" : 17,
	"9-pic" : 18,
	"valet-pic" : 19,
	"dame-pic" : 20,
	"roi-pic" : 21,
	"10-pic" : 22,
	"as-pic" : 23,
	"7-trefle" : 24,
	"8-trefle" : 25,
	"9-trefle" : 26,
	"valet-trefle" : 27,
	"dame-trefle" : 28,
	"roi-trefle" : 29,
	"10-trefle" : 30,
	"as-trefle" : 31
}

var lut_annonces: Dictionary = {
	"mariage-coeur" : 0,
	"mariage-carreau" : 1,
	"mariage-pic" : 2,
	"mariage-trefle" : 3,
	"tierce-coeur" : 4,
	"tierce-carreau" : 5,
	"tierce-pic" : 6,
	"tierce-trefle" : 7,
	"quarante-coeur" : 8,
	"quarante-carreau" : 9,
	"quarante-pic" : 10,
	"quarante-trefle" : 11,
	"chouine-coeur" : 12,
	"chouine-carreau" : 13,
	"chouine-pic" : 14,
	"chouine-trefle" : 15,
	"quinte": 16
}

var pioche: Array = []
var starter: int = 0
var coups_joueur: Array = []
var coups_ordi: Array = []
var annonces_joueur: Array = []
var annonces_ordi: Array = []
var echange_atout_joueur: bool = false
var annonce_joueur: String = ""
var errors: Array[String] = []
var http_request: HTTPRequest = HTTPRequest.new()
var alt_api: bool = false


func set_scene(scene: Node) -> void:
	scene.add_child(http_request)


func premier_joueur(joueur: int) -> void:
	starter = joueur

func atout_joueur() -> void:
	echange_atout_joueur = true


func set_annonce_joueur(annonce: String) -> void:
	annonce_joueur = annonce

func add_error(error: String) -> void:
	errors.append(error)

func set_pioche(cartes: Array) -> void:
	for carte: String in cartes:
		pioche.append(Marshalls.utf8_to_base64(str(lut_cartes[carte])))


func coup_joueur(carte: String) -> void:
	var coup: int = lut_cartes[carte]
	if annonce_joueur != "":
		coup += 0x40
		annonces_joueur.append(Marshalls.utf8_to_base64(str(lut_annonces[annonce_joueur])))
		annonce_joueur = ""
	if echange_atout_joueur:
		coup += 0x80
		echange_atout_joueur = false
	coups_joueur.append(Marshalls.utf8_to_base64(str(coup)))


func coup_ordi(carte: String, annonce: String, echange_atout: bool) -> void:
	var coup: int = lut_cartes[carte]
	if annonce != "":
		coup += 0x40
		annonces_ordi.append(Marshalls.utf8_to_base64(str(lut_annonces[annonce])))
	if echange_atout:
		coup += 0x80
	coups_ordi.append(Marshalls.utf8_to_base64(str(coup)))

func save_game() -> void:
	var url:String = ""
	var key: String = ""
	if alt_api:
		url = Global.api_config["alt"]["url"]
		key = Global.api_config["alt"]["key"]
	else:
		url = Global.api_config["main"]["url"]
		key = Global.api_config["main"]["key"]
	# errors.append("test")
	var data: Dictionary = {
		"UserId": Global.options["user_id"],
		"Timestamp": Time.get_datetime_string_from_system(),
		"Version": Version.get_version(),
		"Pioche": pioche,
		"Starter": starter,
		"J0": coups_ordi,
		"J1": coups_joueur,
		"A_J0": annonces_ordi,
		"A_J1": annonces_joueur,
		"Errors": errors
	}
	var json: String = JSON.stringify(data)
	var headers: Array = ["Content-Type: application/json", "x-api-key: " + key]
	if not http_request.request_completed.is_connected(_on_request_completed):
		http_request.request_completed.connect(_on_request_completed)
	var error:Error = http_request.request(url + "/partie", headers, HTTPClient.METHOD_POST, json)
	if error != 0:
		print(error)

func _on_request_completed(_result: int,
						response_code: int,
						_headers: PackedStringArray,
						_body: PackedByteArray) -> void:
	print("Reponse code: " + str(response_code))
	if response_code != 200:
		if not alt_api:
			alt_api = true
			save_game()
		return
