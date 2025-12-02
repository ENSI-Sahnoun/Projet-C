# 🌐 SNS eChat - Système de Réseau Social

<p align="center">
  <img src="https://img.shields.io/badge/Langage-C-blue?style=for-the-badge&logo=c" alt="Langage C">
  <img src="https://img.shields.io/badge/Version-2.0-green?style=for-the-badge" alt="Version 2.0">
  <img src="https://img.shields.io/badge/Statut-Actif-brightgreen?style=for-the-badge" alt="Statut">
  <img src="https://img.shields.io/badge/Plateforme-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey?style=for-the-badge" alt="Plateforme">
</p>

Une application console de **Réseau Social** développée en C, similaire à Facebook. Cette application gère les utilisateurs, les amitiés, les abonnements et les publications. Conçue comme projet universitaire pour démontrer les structures de données avancées et la gestion de fichiers.

---

## 📋 Table des Matières

- [Fonctionnalités](#-fonctionnalités)
- [Structures de Données](#-structures-de-données)
- [Structure du Projet](#-structure-du-projet)
- [Installation](#-installation)
- [Utilisation](#-utilisation)
- [Options du Menu](#-options-du-menu)
- [Formats de Fichiers](#-formats-de-fichiers)
- [Détails Techniques](#-détails-techniques)
- [Améliorations Futures](#-améliorations-futures)
- [Auteurs](#-auteurs)
- [Licence](#-licence)

---

## ✨ Fonctionnalités

| Fonctionnalité | Description |
|----------------|-------------|
| 👤 **Gestion des Utilisateurs** | Ajouter, rechercher et supprimer des profils avec des IDs uniques |
| 🤝 **Amitiés** | Créer des amitiés bidirectionnelles entre utilisateurs |
| 📢 **Système d'Abonnement** | S'abonner à d'autres utilisateurs (unidirectionnel) |
| 📝 **Publications** | Publier des messages avec horodatage automatique |
| 📰 **Fil d'Actualité** | Voir les publications des amis par ordre chronologique |
| 💾 **Persistance des Données** | Toutes les données sont sauvegardées et chargées au démarrage |
| 📊 **Liste des Utilisateurs** | Afficher tous les utilisateurs enregistrés |

---

## 🏗️ Structures de Données

L'application utilise plusieurs structures de données optimisées pour différentes opérations :

### Arbre Binaire de Recherche (ABR) - Utilisateurs

```
        [User 50]
        /       \
   [User 25]  [User 75]
    /    \      /    \
 [10]   [30]  [60]  [90]
```

Les utilisateurs sont stockés dans un **Arbre Binaire de Recherche** pour des opérations efficaces O(log n) de recherche, insertion et suppression.

### Listes Chaînées - Relations & Publications

```
User -> Amis: [ID:5] -> [ID:12] -> [ID:88] -> NULL
     -> Abonnements: [ID:69] -> [ID:123] -> NULL
     -> Publications: [Post1] -> [Post2] -> [Post3] -> NULL
```

Chaque utilisateur maintient trois listes chaînées :
- **Amis** (`amis`) : Amitiés bidirectionnelles
- **Abonnements** (`abonnements`) : Utilisateurs suivis
- **Publications** : Messages publiés

### Définitions des Structures

```c
typedef struct User {
    int id;                    // Identifiant unique
    char name[30];             // Nom d'utilisateur
    Relation *amis;            // Liste chaînée des amis
    Relation *abonnements;     // Liste chaînée des abonnements
    Publication *publications; // Liste chaînée des publications
    struct User *left, *right; // Pointeurs ABR
} User;

typedef struct Relation {
    int id;                    // ID de l'utilisateur lié
    struct Relation *next;     // Relation suivante
} Relation;

typedef struct Publication {
    char message[200];         // Contenu du message
    char date[20];             // Horodatage
    struct Publication *next;  // Publication suivante
} Publication;
```

---

## 📁 Structure du Projet

```
Projet-C-SNS/
├── interface.c          # Code source principal (toutes les fonctionnalités)
├── Makefile             # Automatisation de la compilation
├── CONTRIBUTING.md      # Guide de contribution
├── .gitignore           # Règles d'exclusion Git
└── README.md            # Ce fichier

# Fichiers générés à l'exécution :
# ├── utilisateurs.bin   # Fichier binaire des utilisateurs
# ├── relations.txt      # Fichier texte des amitiés et abonnements
# └── publications.txt   # Stockage des publications
```

---

## 🚀 Installation

### Prérequis

- **Compilateur GCC** (GNU Compiler Collection) ou tout compilateur C
- Terminal / Invite de commandes

### Compilation sur Linux/macOS

```bash
# Cloner le dépôt
git clone https://github.com/ENSI-Sahnoun/Projet-C-SNS.git
cd Projet-C-SNS

# Compiler
gcc -o sns_echat interface.c

# Exécuter
./sns_echat
```

### Compilation sur Windows

```cmd
# Avec GCC (MinGW)
gcc -o sns_echat.exe interface.c

# Exécuter
sns_echat.exe
```

### Utilisation d'un IDE

1. Ouvrir votre IDE préféré (Code::Blocks, Visual Studio)
2. Créer un nouveau projet C
3. Ajouter `interface.c` au projet
4. Compiler et exécuter

---

## 💻 Utilisation

### Lancement de l'Application

Au démarrage, vous verrez :

```
----------------------------------------
        ,-----.,--.               ,--.   
 ,---. '  .--./|  ,---.  ,--,--.,-'  '-. 
| .-. :|  |    |  .-.  |' ,-.  |'-.  .-' 
\   --.'  '--'\|  | |  |\ '-'  |  |  |   
 `----' `-----'`--' `--' `--`--'  `--'   
------------------------------------------------------------------
Realise Par Youssef Sahnoun & Ameur Masmoudi & Mohamed Amine Walha
------------------------------------------------------------------
Chargement des utilisateurs...
Chargement des relations...
Chargement des publications...
Chargement terminé.
```

---

## 📖 Options du Menu

| Option | Action | Description |
|--------|--------|-------------|
| **1** | Ajouter utilisateur | Créer un nouvel utilisateur avec ID et nom |
| **2** | Supprimer utilisateur | Supprimer un utilisateur et ses données |
| **3** | Rechercher utilisateur | Trouver un utilisateur par son ID |
| **4** | Gérer les relations | Ajouter/supprimer amis, suivre/ne plus suivre |
| **5** | Publier un message | Poster un nouveau message (horodatage auto) |
| **6** | Afficher utilisateurs | Afficher tous les utilisateurs enregistrés |
| **7** | Afficher statistiques | *Bientôt disponible* |
| **8** | Afficher fil d'actualité | Voir les publications des amis |
| **9** | Sauvegarder & Quitter | Sauvegarder et quitter l'application |

### Gestion des Relations (Option 4)

```
1. Ajouter ami
2. Suivre utilisateur
3. Supprimer ami
4. Arrêter de suivre
```

---

## 📄 Formats de Fichiers

### utilisateurs.bin (Binaire)

Stocke les données utilisateurs en format binaire pour une E/S efficace :
```c
struct { int id; char name[30]; }
```

### relations.txt (Texte)

Format lisible pour les relations :
```
ID: 5
Friends: 12 88 123
Following: 69 56

ID: 12
Friends: 5
Following: 
```

### publications.txt (Texte)

Stocke toutes les publications :
```
[UserID](Date): Contenu du message
[5](2024-01-15 14:30:00): Bonjour le monde !
```

---

## 🔧 Détails Techniques

### Algorithmes Utilisés

| Opération | Algorithme | Complexité |
|-----------|------------|------------|
| Recherche utilisateur | Recherche ABR | O(log n) |
| Insertion utilisateur | Insertion ABR | O(log n) |
| Suppression utilisateur | Suppression ABR | O(log n) |
| Vérification ami | Recherche linéaire | O(k) |
| Tri du fil | Tri à bulles | O(p²) |

*Où n = nombre d'utilisateurs, k = nombre d'amis, p = nombre de publications*

### Gestion de la Mémoire

- Toute la mémoire dynamique est allouée avec `malloc()`
- La mémoire est libérée lors de la suppression des utilisateurs (free)
- Les listes chaînées sont parcourues et libérées à la suppression

---

## 🔮 Améliorations Futures

- [ ] Implémentation des statistiques (utilisateurs actifs, nombre d'amis)
- [ ] Édition et suppression des publications
- [ ] Affichage des listes d'amis et d'abonnés
- [ ] Recherche avancée (par nom)
- [ ] Protection par mot de passe
- [ ] Réactions/likes sur les messages
- [ ] Support des hashtags

---

## 👥 Auteurs

<table>
  <tr>
    <td align="center">
      <b>Youssef Sahnoun</b><br>
      <sub>Étudiant ENSI II1A</sub>
    </td>
    <td align="center">
      <b>Ameur Masmoudi</b><br>
      <sub>Étudiant ENSI II1A</sub>
    </td>
    <td align="center">
      <b>Mohamed Amine Walha</b><br>
      <sub>Étudiant ENSI II1B</sub>
    </td>
  </tr>
</table>

**Institution :** ENSI (École Nationale des Sciences de l'Informatique)

---

## 📜 Licence

Ce projet est développé à des fins éducatives dans le cadre du projet du cours de Programmation C à l'ENSI.

---

## 🔗 Liens

- 📚 [Wiki - Documentation](https://github.com/ENSI-Sahnoun/Projet-C-SNS/wiki/Accueil)

---

<p align="center">
  Fait avec ❤️ | Sahnoun - Masmoudi - Walha<br>
  <b>Version Actuelle :</b> 2.0
</p>
