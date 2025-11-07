extends Control


#var scene_partie = preload("res://Scenes/partie.tscn")


func _on_button_pressed() -> void:
	get_tree().change_scene_to_file("res://Scenes/partie.tscn")
