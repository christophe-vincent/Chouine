"""
Copyright 2025,2026 Christophe Vincent
This file is part of La Chouine.
La Chouine is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or any later version.
La Chouine is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
You should have received a copy of the GNU General Public License along with Foobar. If not, see <https://www.gnu.org/licenses/>
"""
extends Control

var regles: String = """[center][font_size={60}][b]Règles[/b][/font_size][/center]
[p][/p]
Le but de la Chouine est de gagner le plus de points possible qui serviront à gagner des parties.
Une Chouine se joue généralement en 3 ou 5 parties constituant une manche et en 1 ou 2 manches gagnantes.

[b] Déroulement du jeu[/b]

Chaque joueur reçoit [b]5[/b] cartes d'un jeu de 32 cartes.
Après la distribution des cartes, la carte se trouvant au-dessus de la pioche est retournée et placée à côté de la pioche. 
Cette carte détermine la couleur de l'[b]atout[/b]. Une carte de la couleur de l'atout est plus forte qu'une carte d'une autre couleur.

Tant qu'il reste des cartes dans la pioche, il n'est pas nécessaire de monter, c'est-à-dire prendre la carte de l'adversaire si cela est possible.
Il est aussi possible de ne pas jouer la couleur du joueur adverse.
Quand la pioche est vide, alors il faut jouer la même couleur que l'adversaire, monter si cela est possible. 
Si vous n'avez de carte de la couleur d l'adversaire, alors vous devrez jouer un atout si vous en possédez.

[b] Points des cartes[/b]

Voici la valeur des points pour chaque carte :
[ul][b]7, 8, 9[/b]: 0 point[/ul]
[ul][b]Valet[/b]: 2 points[/ul]
[ul][b]Dame[/b]: 3 points[/ul]
[ul][b]Roi[/b]: 4 points[/ul]
[ul][b]10[/b]: 10 points[/ul]
[ul][b]As[/b]: 11 points[/ul]

Les [b]10[/b] et les [b]As[/b] sont appelés [b]Brisques[/b].

[b] Annonces[/b]

Certaines combinaisons de cartes permettent de réaliser des annonces. Ces dernières ajouteront des points au décompte final.
Les cartes des annonces, excepté la quinte, seront toutes de la même couleur.
Si la couleur de l'annonce est celle de l'atout, les points sont doublés (40 pour un mariage).

[table=4,center,center]
[cell padding=60,0,30,10][b]Mariage[/b][/cell]
[cell padding=0,0,30,10]Dame Roi[/cell]
[cell padding=0,0,30,10]20 points[/cell]
[cell padding=0,0,30,10][center][img=100,center,center]res://Assets/Annonces/mariage-coeur.svg[/img][/center][/cell]
[cell padding=60,0,30,10][b]Tierce[/b][/cell]
[cell padding=0,0,30,10]Valet Dame Roi    [/cell]
[cell padding=0,0,30,10]30 points[/cell]
[cell padding=0,0,30,10 valign=center][img=100]res://Assets/Annonces/tierce-coeur.svg[/img][/cell]
[cell padding=60,0,30,10][b]Quarante[/b][/cell]
[cell padding=0,0,30,10]Valet Dame Roi As[/cell]
[cell padding=0,0,30,10]40 points[/cell]
[cell padding=0,0,30,10][img=100,center]res://Assets/Annonces/quarante-coeur.svg[/img][/cell]
[cell padding=60,0,30,10][b]Quinte[/b][/cell]
[cell padding=0,0,30,10]5 brisques (10 et As)[/cell]
[cell padding=0,0,30,10]50 points[/cell]
[cell padding=0,0,30,10][img=100,center]res://Assets/Annonces/quinte.svg[/img][/cell]
[cell padding=60,0,30,10][b]Chouine[/b][/cell]
[cell padding=0,0,30,10]Valet Dame Roi 10 As[/cell]
[cell padding=0,0,30,10][b]Partie gagnée ![/b][/cell]
[/table]

[b] 10 de der[/b]

Le joueur qui remporte le dernier pli gagne 10 points, appelés "10 de der".
"""

var credits: String = """
[center][font_size={60}][b]La Chouine[/b][/font_size][/center]

Ce jeu est entièrement Open Source sous licence [b]GPL v3[/b] [img=100]res://Assets/gplv3-127x51.png[/img] et disponible sur github:
[indent][url]https://github.com/christophe-vincent/Chouine[/url][/indent]
Vous pouvez contacter l'auteur à cette adresse: christophevinc@gmail.com
Merci à :
[ul] Léandre d'apporter sa connaissance du moteur de jeu[/ul]
[ul] Héloïse pour le choix des couleurs[/ul]
[ul] Alain et Ludovic les testeurs en avant-première[/ul]

[center][font_size={60}][b]Open Source[/b][/font_size][/center]
[font_size={40}][b]Godot[/b][/font_size]
[indent]Ce logiciel a été réalisé avec le moteur de jeu Open Source [b]Godot[/b] sous licence MIT.
[url=https://godotengine.org/license]https://godotengine.org/license[/url]
[/indent]

[font_size={40}][b]Cartes[/b][/font_size]
[indent]Auteur: David Bellot <david.bellot@free.fr>
[url]https://svg-cards.sourceforge.net/[/url] (LGPL license)
[/indent]

[font_size={40}][b]Police d'écriture[/b][/font_size]
[indent]Macondo font - Copyright 1997 - 2011, John Vargas Beltran (www.johnvargasbeltran.com|john.vargasbeltran@gmail.com), with Reserved Font Name Macondo.
This Font Software is licensed under the SIL Open Font License, Version 1.1
[url]https://openfontlicense.org/open-font-license-official-text[/url]
[/indent]
"""


var coef: float = 1
var offset: float = -0.42


func _ready() -> void:
	$Version.text = "v" + Version.get_version()
	if Global.texte == Global.TypeTexte.REGLES:
		$Regles.text = regles
	else:
		$Regles.text = credits


func _on_retour_pressed() -> void:
	var error: Error = get_tree().change_scene_to_file("res://Scenes/accueil.tscn")
	if error != OK:
		print("Scene change failed with error: ", error)


func _on_regles_meta_clicked(meta: Variant) -> void:
	OS.shell_open(str(meta))
