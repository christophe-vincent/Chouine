extends Control

@onready var parchemin: TextureRect = $Parchemin

var regles: String = """[center][font_size={60}][b]Règles[/b][/font_size][/center]
[p][/p]
Le but de la Chouine est de gagner le plus de points possible qui serviront à gagner des parties.
Une Chouine se joue généralement en 3 ou 5 parties constituant une manche et en 1 ou 2 manches gagnantes.

[b] Déroulement du jeu[/b]

Chaque joueur reçoit [b]5[/b] cartes d'un jeu de 32 cartes.
Après la distribution des cartes, la carte se trouvant au-dessus de la pioche est retournée et placée à côté de la pioche. 
Cette carte détermine la couleur de l'[b]atout[/b]. Une carte de la couleur de l'atout est plus forte qu'une carte d'une autre couleur.

Tant qu'il reste des cartes dans la pioche, il n'est pas nécessaire de monter, c'est-à-dire prendre la carte de l'adversaire si cela est possible.
Quand la pioche est vide, monter est obligatoire si une carte plus forte le permet.

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
var coef: float = 1
var offset: float = -0.42


func _ready() -> void:
	$Regles.text = regles
	setup_scrolling_shader()
	var scrollbar: VScrollBar = $Regles.get_v_scroll_bar()
	scrollbar.value_changed.connect(_on_rich_text_scrolled)
	coef = $Parchemin.size.y/$Parchemin.size.x
	if coef >= 1:
		offset = 0
	$Parchemin.material.set_shader_parameter("scroll_offset", offset)


func _on_rich_text_scrolled(value: float) -> void:
	var scrollbar: VScrollBar = $Regles.get_v_scroll_bar()
	# Calculate scroll percentage (0.0 to 1.0)
	var scroll_range: float = scrollbar.max_value - scrollbar.min_value
	var scroll_percent: float = 0.0
	
	if scroll_range > 0:
		scroll_percent = value / scroll_range
	
	# Apply to texture shader
	$Parchemin.material.set_shader_parameter("scroll_offset", offset + scroll_percent*coef)

func setup_scrolling_shader() -> void:
	# Create shader material
	
	#uniform vec4 tint_color : source_color = vec4(1, 1, 1, 0.9);
	#uniform vec4 tint_color : source_color = vec4(0.47, 0.41, 0.34, 0.9);

	var shader_code: String = """
shader_type canvas_item;

uniform float scroll_offset = 0.0;
uniform vec4 tint_color : source_color = vec4(1, 1, 1, 0.9) ;


void fragment() {
    vec2 uv = UV;
    uv.y += scroll_offset;
    COLOR = texture(TEXTURE, uv)* tint_color;
}
"""
	
	var shader: Shader = Shader.new()
	shader.code = shader_code
	
	var mat: ShaderMaterial = ShaderMaterial.new()
	mat.shader = shader
	
	parchemin.material = mat


func _on_retour_pressed() -> void:
	var error: Error = get_tree().change_scene_to_file("res://Scenes/accueil.tscn")
	if error != OK:
		print("Scene change failed with error: ", error)
