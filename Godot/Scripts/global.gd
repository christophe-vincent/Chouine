extends Node

# Options
var options: Dictionary = {
	"nb_points": 3,
	"nb_manches": 1,
	"cartes_visibles": true,
	"dos_carte": 0
}

# scene de la partie en mode aide
var help_mode: bool = false


func lecture_options() -> void:
	if not FileAccess.file_exists(Settings.OPTIONS_FILE):
		return
	var options_file: FileAccess = FileAccess.open(Settings.OPTIONS_FILE, FileAccess.READ)
	while options_file.get_position() < options_file.get_length():
		var json_string: String = options_file.get_line()
		var json: JSON = JSON.new()
		var parse_result: Error = json.parse(json_string)
		if not parse_result == OK:
			print("Erreurr dans le fichier d'options")
			return
		options = json.data
		print(options)


func enregistrer_options() -> void:
	var save_file: FileAccess = FileAccess.open(Settings.OPTIONS_FILE, FileAccess.WRITE)
	save_file.store_line(JSON.stringify(options))
