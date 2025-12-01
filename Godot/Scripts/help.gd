@tool
extends Control
class_name Help

@export_multiline var text: String = "help":
	set(value):
		text = value
		if is_inside_tree():
			$Panel/RichTextLabel.text = text

func _ready() -> void:
	$Panel/RichTextLabel.text = text
