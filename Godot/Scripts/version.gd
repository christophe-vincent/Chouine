# version.gd
class_name Version
extends Node

const VERSION_FILE: String = "res://version.txt"

static func get_version() -> String:
	# Try to read from version file first
	if FileAccess.file_exists(VERSION_FILE):
		var file: FileAccess = FileAccess.open(VERSION_FILE, FileAccess.READ)
		if file:
			var version: String = file.get_as_text().strip_edges()
			file.close()
			return version
	
	# Fallback to project settings
	var project_version: Variant = ProjectSettings.get_setting("application/config/version", "0.0.0")
	return project_version

static func get_full_version() -> Dictionary:
	var version_string: String = get_version()
	var parts: Array = version_string.split(".")
	
	return {
		"full": version_string,
		"major": int(parts[0]) if parts.size() > 0 else 0,
		"minor": int(parts[1]) if parts.size() > 1 else 0,
		"patch": int(parts[2]) if parts.size() > 2 else 0
	}
