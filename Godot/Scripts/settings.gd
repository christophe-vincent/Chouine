class_name Settings
extends RefCounted

const SAVE_FILE: String = "user://savegame.save"
const OPTIONS_FILE: String = "user://options.save"
const STATS_FILE: String = "user://stats.save"

const DEFAULT_CARD_SIZE: Vector2 = Vector2(240, 360)

const FACTEUR_RAPIDITE: float = 1.0
const DUREE_MOUVEMENT: float = 1 / FACTEUR_RAPIDITE
const DUREE_DISTRIBUTION: float = 0.2 / FACTEUR_RAPIDITE

const TAILLE_ANNONCE: int = 90

const TEXT_INTRO: String = "Le jeu de la Chouine est un jeu de cartes très ancien qui se pratique dans le centre-ouest de la France, notamment dans la vallée du Loir.
Ce jeu a pour ancêtres les jeux de Brisque ou Mariage. Ces derniers étant contemporains de Rabelais et cités dans Gargantua."
