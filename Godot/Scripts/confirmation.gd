extends Control
class_name ConfirmationPanel

@onready var ok_button: Button = $Panel/HBoxContainer/OK
@onready var cancel_button: Button = $Panel/HBoxContainer/Cancel

@export var text: String
@export var ok_text: String
@export var cancel_text: String

func _onready() -> void:
	$Panel/RichtextLabel.text = text
	ok_button.text = ok_text
	cancel_button.text = cancel_text
