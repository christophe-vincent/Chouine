"""
Copyright 2025,2026 Christophe Vincent
This file is part of La Chouine.
La Chouine is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or any later version.
La Chouine is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with Foobar. If not, see <https://www.gnu.org/licenses/>
"""
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
