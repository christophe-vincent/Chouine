"""
Copyright 2025,2026 Christophe Vincent
This file is part of La Chouine.
La Chouine is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or any later version.
La Chouine is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with Foobar. If not, see <https://www.gnu.org/licenses/>
"""
@tool
extends Control
class_name Statistiques

@onready var total_chouines_total: Label = $MarginContainer/VBoxContainer/ProgressTotalChouine/Chouines/Total
@onready var total_chouines_victoires: Label = $MarginContainer/VBoxContainer/ProgressTotalChouine/Chouines/Victoires
@onready var total_parties_total: Label = $MarginContainer/VBoxContainer/ProgressTotalPartie/Parties/Total
@onready var total_parties_victoires: Label = $MarginContainer/VBoxContainer/ProgressTotalPartie/Parties/Victoires

@onready var annee_chouines_total: Label = $MarginContainer/VBoxContainer/ProgressAnneeChouine/Chouines/Total
@onready var annee_chouines_victoires: Label = $MarginContainer/VBoxContainer/ProgressAnneeChouine/Chouines/Victoires
@onready var annee_parties_total: Label = $MarginContainer/VBoxContainer/ProgressAnneePartie/Parties/Total
@onready var annee_parties_victoires: Label = $MarginContainer/VBoxContainer/ProgressAnneePartie/Parties/Victoires

@onready var mois_chouines_total: Label = $MarginContainer/VBoxContainer/ProgressMoisChouine/Chouines/Total
@onready var mois_chouines_victoires: Label = $MarginContainer/VBoxContainer/ProgressMoisChouine/Chouines/Victoires
@onready var mois_parties_total: Label = $MarginContainer/VBoxContainer/ProgressMoisPartie/Parties/Total
@onready var mois_parties_victoires: Label = $MarginContainer/VBoxContainer/ProgressMoisPartie/Parties/Victoires


var lut_mois: Dictionary = {
	"1": "Janvier",
	"2": "Février",
	"3": "Mars",
	"4": "Avril",
	"5": "Mai",
	"6": "Juin",
	"7": "Juillet",
	"8": "Août",
	"9": "Septembre",
	"10": "Octobre",
	"11": "Novembre",
	"12": "Décembre"
}


func update() -> void:
	var now: Dictionary = Time.get_date_dict_from_system()
	var year: String = str(now['year'])
	var month: String = str(now['month'])

	# Total
	var total: int = Global.stats["total"]["chouines"]["W"] + Global.stats["total"]["chouines"]["L"]
	var victoires: int = Global.stats["total"]["chouines"]["W"]
	var pc: int = 0
	if total > 0:
		pc = int(100 * victoires / float(total))
	$MarginContainer/VBoxContainer/ProgressTotalChouine.value = pc
	total_chouines_total.text = "{nb} chouine{s} ".format({'nb': total, 's': 's' if total > 1 else ''})
	total_chouines_victoires.text = "{nb} victoire{s} - {pc}%".format({'nb': victoires, 's': 's' if victoires > 1 else '', 'pc': pc})
	
	total = Global.stats["total"]["parties"]["W"] + Global.stats["total"]["parties"]["L"]
	victoires = Global.stats["total"]["parties"]["W"]
	pc = int(100 * victoires / float(total)) if total > 0 else 0
	$MarginContainer/VBoxContainer/ProgressTotalPartie.value = pc
	total_parties_total.text = "{nb} partie{s} ".format({'nb': total, 's': 's' if total > 1 else ''})
	total_parties_victoires.text = "{nb} victoire{s} - {pc}%".format({'nb': victoires, 's': 's' if victoires > 1 else '', 'pc': pc})
	
	# Annee
	$MarginContainer/VBoxContainer/Annee.text = year
	total = Global.stats["annees"][year]["chouines"]["W"] + Global.stats["annees"][year]["chouines"]["L"]
	victoires = Global.stats["annees"][year]["chouines"]["W"]
	pc = 0
	if total > 0:
		pc = int(100 * victoires / float(total))
	$MarginContainer/VBoxContainer/ProgressAnneeChouine.value = pc
	annee_chouines_total.text = "{nb} chouine{s} ".format({'nb': total, 's': 's' if total > 1 else ''})
	annee_chouines_victoires.text = "{nb} victoire{s} - {pc}%".format({'nb': victoires, 's': 's' if victoires > 1 else '', 'pc': pc})
	
	total = Global.stats["annees"][year]["parties"]["W"] + Global.stats["annees"][year]["parties"]["L"]
	victoires = Global.stats["annees"][year]["parties"]["W"]
	pc = int(100 * victoires / float(total)) if total > 0 else 0
	$MarginContainer/VBoxContainer/ProgressAnneePartie.value = pc
	annee_parties_total.text = "{nb} partie{s} ".format({'nb': total, 's': 's' if total > 1 else ''})
	annee_parties_victoires.text = "{nb} victoire{s} - {pc}%".format({'nb': victoires, 's': 's' if victoires > 1 else '', 'pc': pc})
	
	# Mois
	$MarginContainer/VBoxContainer/Mois.text = lut_mois[month]
	total = Global.stats["mois"][month]["chouines"]["W"] + Global.stats["mois"][month]["chouines"]["L"]
	victoires = Global.stats["mois"][month]["chouines"]["W"]
	pc = 0
	if total > 0:
		pc = int(100 * victoires / float(total))
	$MarginContainer/VBoxContainer/ProgressMoisChouine.value = pc
	mois_chouines_total.text = "{nb} chouine{s} ".format({'nb': total, 's': 's' if total > 1 else ''})
	mois_chouines_victoires.text = "{nb} victoire{s} - {pc}%".format({'nb': victoires, 's': 's' if victoires > 1 else '', 'pc': pc})
	
	total = Global.stats["mois"][month]["parties"]["W"] + Global.stats["mois"][month]["parties"]["L"]
	victoires = Global.stats["mois"][month]["parties"]["W"]
	pc = int(100 * victoires / float(total)) if total > 0 else 0
	$MarginContainer/VBoxContainer/ProgressMoisPartie.value = pc
	mois_parties_total.text = "{nb} partie{s} ".format({'nb': total, 's': 's' if total > 1 else ''})
	mois_parties_victoires.text = "{nb} victoire{s} - {pc}%".format({'nb': victoires, 's': 's' if victoires > 1 else '', 'pc': pc})
