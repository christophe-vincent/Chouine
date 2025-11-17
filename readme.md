# Chouine

Jeu de carte se jouant à 2 joueurs qui est probablement issu de la Brisque, Mariage. 

L'algorithme est développé en C++ et la partie graphique grâce à Godot.


# TODO list
- Algo: Prise de la carte d'atout pour completer une annonce


## bugs connus
- Annonce chouine par le joueur: reste une carte sur le tapis.
- Gain de la partie par une chouine: ne pas afficher les scores

# Algorithme

Il est popssible de tester l'algorithme grace à une app de test: chouinetest

Compilation et éxécution (sous Linux):
> mkdir build && cd build \
> cmake .. \
> make \
> ./chouinetest


# Déploiement

## Compilation de l'algorithme

L'algorithme est programmé en c++ et nécessite l'extention Godot c++.
Voir https://docs.godotengine.org/en/4.4/tutorials/scripting/gdextension/gdextension_cpp_example.html

Il sera donc nécessaire de compiler l'algorithme séparemment.

Pour cela créer un virtualenv python et installer 'scons'.

Activation de **emsdk**:
> <*repertoire emsdk*>/emsdk_env.bat


Compilation pour **Windows** (pour lancer le jeu avec Godot):
> cd Chouine\Algo\wrapper_godot \
> scons platform=windows


Compilation pour **html5**:
> cd Chouine\Algo\wrapper_godot \
> scons platform=web target=template_release

Compilation pour **Android**

Release mode:
> scons platform=android target=template_release arch=arm32
> scons platform=android target=template_release arch=arm64 generate_android_binaries=yes

Debug mode used to debug from Godot:
> scons platform=android target=template_debug arch=arm32
> scons platform=android target=template_debug arch=arm64 generate_android_binaries=yes

Les fichiers sont automatiquement copiés dans le répertoire bin de godot.

## Exporter vers HTML5

### Compilation du GDExtention pour le web
Suivre les instructions de la section **GDExtension**:
https://docs.godotengine.org/en/latest/engine_details/development/compiling/compiling_for_web.html#doc-compiling-for-web

Le fichier compilé (web_dlink_release.zip) est ensuite copié ici:
C:\Users\<USER>\AppData\Roaming\Godot\export_templates\4.5.stable

### Mise en ligne
La copie vers un bucket S3 et mise à jour du cache CloudFront s'effectue grace à ce script:

> ./deploiement/upload_site_web.py

ANDROID_HOME=/mnt/c/Users/cvincent/AppData/Local/Android/Sdk


# Algorithme
Choix du premier joueur

|       |         |         | **Atout** | **Atout**  | **Atout** | Choix |
| ----- | ------- | ------- | --------- |----------- | --------- | ----- |
| **0** | **2-4** | **>10** |   **0**   |  **2-4**   |  **>10**  |  |
| >0    | ?       | ?       | ?         | ?          | ?         | Carte la plus faible|
| 0     | >0      | ?       | ?         | ?          | ?         | Vérification des annonces possibles. Choisir la carte ayant une proba faible de faire une annonce|
| 0     | 0       | >0      | >0        | ?          | ?         | ?? |
| 0     | 0       | >0      | 0         | >0         | ?         | ?? |
| 0     | 0       | >0      | 0         | 0          | >0        | 10, puis As (sans atout) |
| 0     | 0       | 0       | >0        | ?          | ?         | Carte la plus faible sauf le 7. Si uniquement 7, échanger avec la retourne si la retourne est une carte à points |
| 0     | 0       | 0       | 0         | >0         | >0        | Carte la plus faible |


Choix du second joueur
|Carte Adv. |       |         |         | **Atout** | **Atout**  | **Atout** | Choix |
| --------  | ----- | ------- | ------- | --------- |----------- | --------- | ----- |
|           | **0** | **2-4** | **>10** |   **0**   |  **2-4**   |  **>10**  |  |
| **0**     | >0    | ?       | ?       | ?         | ?          | ?         | Carte la plus faible|
| **0**     | 0     | >0      | ?       | ?         | ?          | ?         | Vérification des annonces possibles. Choisir la carte ayant une proba faible de faire une annonce|
| **0**     | 0     | 0       | >0      | >0        | ?          | ?         | ?? |
| **0**     | 0     | 0       | >0      | 0         | >0         | ?         | ?? |
| **0**     | 0     | 0       | >0      | 0         | 0          | >0        | 10, puis As (sans atout) |
| **0**     | 0     | 0       | 0       | >0        | ?          | ?         | Carte la plus faible sauf le 7. Si uniquement 7, échanger avec la retourne si la retourne est une carte à | **0**     points |
| **2-4**     | 0     | 0       | 0       | 0         | >0         | >0        | Carte la plus faible |
| **2-4**     | >0    | ?       | ?       | ?         | ?          | ?         | Carte la plus faible|
| **2-4**     | 0     | >0      | ?       | ?         | ?          | ?         | Vérification des annonces possibles. Choisir la carte ayant une proba faible de faire une annonce|
| **2-4**     | 0     | 0       | >0      | >0        | ?          | ?         | ?? |
| **2-4**     | 0     | 0       | >0      | 0         | >0         | ?         | ?? |
| **2-4**     | 0     | 0       | >0      | 0         | 0          | >0        | 10, puis As (sans atout) |
| **2-4**     | 0     | 0       | 0       | >0        | ?          | ?         | Carte la plus faible sauf le 7. Si uniquement 7, échanger avec la retourne si la retourne est une carte à | **0**     points |
| **2-4**     | 0     | 0       | 0       | 0         | >0         | >0        | Carte la plus faible |
