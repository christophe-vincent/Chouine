"""
Copyright 2025,2026 Christophe Vincent
This file is part of La Chouine.
La Chouine is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or any later version.
La Chouine is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with Foobar. If not, see <https://www.gnu.org/licenses/>
"""
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
