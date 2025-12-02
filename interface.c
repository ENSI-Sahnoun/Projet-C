#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* -----
   Types
   ------ */

typedef struct Relation {
    int id;                       
    struct Relation *next;        
} Relation;

typedef struct Publication {
    char message[200];
    char date[20];                 
    struct Publication *next;      
} Publication;

typedef struct User {
    int id;
    char name[30];

    Relation *amis;                
    Relation *abonnements;         
    Publication *publications;     
    struct User *left;
    struct User *right;
} User;

typedef struct {
    int id;
    char name[30];
} UserSave;

typedef struct TimelinePost {
    int id;
    char name[30];
    char message[200];
    char date[20];
} TimelinePost;

/* -------------------------
   Prototypes
   ------------------------- */

void titlescreen(void);
void credits(void);
int questions(void);

User* create_user(int id, const char *name);
User* insert_user(User *root, int id, char *name);
User* add_user(User *root);

User* search_user(User *root, int id);
User* find_min(User *root);
User* delete_user(User *root, int id);

void inorder(User *root);

void save_users(User *root);
User* load_users(void);

void save_all_relations(User *root);
void load_relations(User *root);

void save_all_publications(User *root);
void load_publications(User *root);

void save_users_rec(User *root, FILE *file);
void save_relations(User *root, FILE *file);
void save_publications(User *root, FILE *file);

int relation_exists(Relation *head, int id);
void add_relation_front(Relation **head, int id);
void add_friend(User *root, User *u, int friend_id);
void follow(User *root, User *u, int target_id);

void push_publication(User *u, const char *message, const char *date);


void titlescreen() {
    printf("----------------------------------------\n"
           "        ,-----.,--.               ,--.   \n"
           " ,---. '  .--./|  ,---.  ,--,--.,-'  '-. \n"
           "| .-. :|  |    |  .-.  |' ,-.  |'-.  .-' \n"
           "\\   --.'  '--'\\|  | |  |\\ '-'  |  |  |   \n"
           " `----' `-----'`--' `--' `--`--'  `--'   \n"
           "-------------------------------------------------------------------\n");
}
void credits()
{
    printf("Realise Par Youssef Sahnoun & Ameur Masmoudi & Mohamed Amine Walha\n");
    printf("-------------------------------------------------------------------\n");
}
int questions()
{
    int choice;
    printf("1. Créer utilisateur \n"
           "2. Supprimer utilisateur \n"
           "3. Rechercher utilisateur \n"
           "4. Gérer les relations \n"
           "5. Publier un message \n"
           "6. Afficher les utilisateurs\n"
           "7. Afficher les statistiques \n"
           "8. Afficher la timeline \n"
           "9. Enregistrer & Quitter \n");
    printf("Choice: ");
    if (scanf("%d",&choice) != 1) {
        while (getchar() != '\n'); 
        return -1;
    }
    getchar();
    return choice;
}


User *create_user(int id, const char *name) {
    User *u = (User*)malloc(sizeof(User));
    if (!u) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    u->id = id;
    strncpy(u->name, name, sizeof(u->name)-1);
    u->name[sizeof(u->name)-1] = '\0';
    u->amis = NULL;
    u->abonnements = NULL;
    u->publications = NULL;
    u->left = u->right = NULL;
    return u;
}


User* insert_user(User *root, int id, char *name) {
    if (root == NULL)
        return create_user(id, name);

    if (id < root->id)
        root->left = insert_user(root->left, id, name);

    else if (id > root->id)
        root->right = insert_user(root->right, id, name);

    else
        printf("Utilisateur avec l'ID %d existe déjà.\n", id);

    return root;
}


User* add_user(User *root) {
    int id;
    char name[30];
    printf("Nouvel ID: ");
    if (scanf("%d", &id) != 1) { while (getchar() != '\n'); printf("ID invalide.\n"); return root; }
    getchar();
    printf("Nom: ");
    if (!fgets(name, sizeof(name), stdin)) name[0] = '\0';
    name[strcspn(name, "\n")] = '\0';
    root = insert_user(root, id, name);
    return root;
}

User* search_user(User *root, int id) {
    if (root == NULL || root->id == id)
        return root;

    if (id < root->id)
        return search_user(root->left, id);
    else
        return search_user(root->right, id);
}

User* find_min(User *root) {
    while (root && root->left != NULL)
        root = root->left;
    return root;
}

User* delete_user(User *root, int id) {
    if (root == NULL)
        return NULL;

    if (id < root->id)
        root->left = delete_user(root->left, id);

    else if (id > root->id)
        root->right = delete_user(root->right, id);

    else {
        
        if (root->left == NULL && root->right == NULL) {
            Relation *r, *rn;
            for (r = root->amis; r; r = rn) { rn = r->next; free(r); }
            for (r = root->abonnements; r; r = rn) { rn = r->next; free(r); }
            Publication *p, *pn;
            for (p = root->publications; p; p = pn) { pn = p->next; free(p); }
            free(root);
            return NULL;
        }
        if (root->left == NULL) {
            User *tmp = root->right;
            Relation *r, *rn;
            for (r = root->amis; r; r = rn) { rn = r->next; free(r); }
            for (r = root->abonnements; r; r = rn) { rn = r->next; free(r); }
            Publication *p, *pn;
            for (p = root->publications; p; p = pn) { pn = p->next; free(p); }

            free(root);
            return tmp;
        }
        if (root->right == NULL) {
            User *tmp = root->left;

            Relation *r, *rn;
            for (r = root->amis; r; r = rn) { rn = r->next; free(r); }
            for (r = root->abonnements; r; r = rn) { rn = r->next; free(r); }
            Publication *p, *pn;
            for (p = root->publications; p; p = pn) { pn = p->next; free(p); }

            free(root);
            return tmp;
        }

        User *succ = find_min(root->right);
        root->id = succ->id;
        strcpy(root->name, succ->name);
        root->right = delete_user(root->right, succ->id);
    }

    return root;
}

void inorder(User *root) {
    if (root == NULL) return;
    else {
    inorder(root->left);
    printf("ID=%d | Nom=%s\n", root->id, root->name);
    inorder(root->right);
    }
}

void save_users_rec(User *root, FILE *file) {
    if (!root) return;

    save_users_rec(root->left, file);

    UserSave tmp;
    tmp.id = root->id;
    strncpy(tmp.name, root->name, sizeof(tmp.name)-1);
    tmp.name[sizeof(tmp.name)-1] = '\0';
    fwrite(&tmp, sizeof(UserSave), 1, file);

    save_users_rec(root->right, file);
}

void save_users(User *root) {
    FILE *file = fopen("utilisateurs.bin", "wb");
    
    save_users_rec(root, file);
    fclose(file);


}

User* load_users() {
    FILE *file = fopen("utilisateurs.bin", "rb");
    
    User *root = NULL;
    UserSave tmp;

    while (fread(&tmp, sizeof(UserSave), 1, file) == 1) {
        root = insert_user(root, tmp.id, tmp.name);
    }

    fclose(file);
    return root;
}

/* -------------------------
   Relations save/load (text)
   ------------------------- */

void save_relations(User *root, FILE *file) {
    if (!root) return;

    save_relations(root->left, file);

    fprintf(file, "ID: %d\n", root->id);

    fprintf(file, "Friends:");
    Relation *f = root->amis;
    while (f) {
        fprintf(file, " %d", f->id);
        f = f->next;
    }
    fprintf(file, "\n");

    fprintf(file, "Abonnements:");
    Relation *fl = root->abonnements;
    while (fl) {
        fprintf(file, " %d", fl->id);
        fl = fl->next;
    }
    fprintf(file, "\n\n");

    save_relations(root->right, file);
}

void save_all_relations(User *root) {
    FILE *file = fopen("relations.txt", "w");
    if (!file) {
        printf("Erreur ouverture relations.txt\n");
        return;
    }

    save_relations(root, file);
    fclose(file);

}


int relation_exists(Relation *head, int id) {
    Relation *r = head;
    while (r) {
        if (r->id == id) return 1;
        r = r->next;
    }
    return 0;
}
void add_relation_front(Relation **head, int id) {
    if (relation_exists(*head, id)) return;
    Relation *n = (Relation*)malloc(sizeof(Relation));
    n->id = id;
    n->next = *head;
    *head = n;
}

void add_friend(User *root, User *u, int friend_id) {
    if (!u) return;
    if (u->id == friend_id) { printf("\nVous ne pouvez pas être ami avec vous-même.\n"); return; }
    User *f = search_user(root, friend_id);
    if (relation_exists(u->amis, friend_id)) {
        printf("\n%s et %s sont déjà amis.\n", u->name, f->name);
        return;
    }
    add_relation_front(&u->amis, friend_id);
    add_relation_front(&f->amis, u->id);
    printf("-> %s et %s sont maintenant amis.\n", u->name, f->name);
}

void follow(User *root, User *u, int target_id) {
    if (!u) return;
    if (u->id == target_id) { printf("Vous ne pouvez pas vous abonner à vous-même.\n"); return; }
    User *t = search_user(root, target_id);
    if (relation_exists(u->abonnements, target_id)) {
        printf("\n%s suit déjà %s.\n", u->name, t->name);
        return;
    }
    add_relation_front(&u->abonnements, target_id);
    printf("-> %s suit maintenant %s.\n", u->name, t->name);
    
}

void remove_relation(Relation **head, int id) {
    Relation *prev = NULL;
    Relation *curr = *head;
    while (curr) {
        if (curr->id == id) {
            if (prev) prev->next = curr->next;
            else *head = curr->next;
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void remove_friend(User *root, User *u, int friend_id) {
    if (!u) return;
    User *f = search_user(root, friend_id);
    if (!f) { printf("Ami non trouvé.\n"); return; }
    if (!relation_exists(u->amis, friend_id)) {
        printf("\n%s et %s ne sont pas amis.\n", u->name, f->name);
        return;
    }
    remove_relation(&u->amis, friend_id);
    remove_relation(&f->amis, u->id);
    printf("-> %s et %s ne sont plus amis.\n", u->name, f->name);
}

void unfollow(User *root, User *u, int target_id) {
    if (!u) return;
    User *t = search_user(root, target_id);
    if (!t) { printf("Utilisateur non trouvé.\n"); return; }
    if (!relation_exists(u->abonnements, target_id)) {
        printf("\n%s ne suit pas %s.\n", u->name, t->name);
        return;
    }
    remove_relation(&u->abonnements, target_id);
    printf("-> %s ne suit plus %s.\n", u->name, t->name);
}

void manage_relations(User *root){
    int id1, id2, op;
    printf("1. Ajouter ami\n2. Suivre utilisateur\n3. Supprimer ami\n4. Arreter de suivre utilisateur\nChoix: ");
    if (scanf("%d", &op) != 1) { while (getchar() != '\n'); printf("Option invalide.\n"); return; }
    getchar();
    printf("Votre ID: ");
    if (scanf("%d", &id1) != 1) { while (getchar() != '\n'); printf("ID invalide.\n"); return; }
    getchar();
    User *u = search_user(root, id1);
    if (!u) { printf("Utilisateur non trouvé.\n"); return; }
    printf("ID cible: ");
    if (scanf("%d", &id2) != 1) { while (getchar() != '\n'); printf("ID invalide.\n"); return; }
    getchar();
    User *c = search_user(root, id2);
    if (!c) { printf("Utilisateur non trouvé.\n"); return; }
    
    switch (op) {
        case 1:
            add_friend(root, u, id2);
            break;
        case 2:
            follow(root, u, id2);
            break;
        case 3:
            remove_friend(root, u, id2);
            break;
        case 4:
            unfollow(root, u, id2);
            break;
        default:
            printf("Option invalide.\n");
            break;
    }
}

void load_relations(User *root) {
    FILE *file = fopen("relations.txt", "r");
    char line[512];
    int current_id = -1;

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "ID:", 3) == 0) {
            sscanf(line, "ID: %d", &current_id);
        }
        else if (strncmp(line, "Amis:", 5) == 0) {
            User *u = search_user(root, current_id);
            if (!u) continue;

            char *p = line + 5;
            char *token = strtok(p, " \t\n");
            while (token) {
                int fid;
                if (sscanf(token, "%d", &fid) == 1)
                    add_friend(root, u, fid);
                token = strtok(NULL, " \t\n");
            }
        }
        else if (strncmp(line, "Abonnements:", 12) == 0) {
            User *u = search_user(root, current_id);
            if (!u) continue;

            char *p = line + 12;
            char *token = strtok(p, " \t\n");
            while (token) {
                int fid;
                if (sscanf(token, "%d", &fid) == 1)
                    follow(root, u, fid);
                token = strtok(NULL, " \t\n");
            }
        }
    }

    fclose(file);
}


void save_publications(User *root, FILE *file) {
    if (!root) return;

    save_publications(root->left, file);

    Publication *p = root->publications;
    while (p) {
        fprintf(file,"[%d](%s): %s\n", root->id, p->date, p->message);
        p = p->next;
    }

    save_publications(root->right, file);
}

void save_all_publications(User *root) {
    FILE *file = fopen("publications.txt", "w");
    save_publications(root, file);
    fclose(file);

}

void push_publication(User *u, const char *message, const char *date) {
    if (!u) return;
    Publication *p = (Publication*)malloc(sizeof(Publication));
    strncpy(p->message, message, sizeof(p->message)-1);
    p->message[sizeof(p->message)-1] = '\0';
    strncpy(p->date, date, sizeof(p->date)-1);
    p->date[sizeof(p->date)-1] = '\0';
    p->next = u->publications;
    u->publications = p;
}

void load_publications(User *root) {
    FILE *file = fopen("publications.txt", "r");
    if (!file) {
        printf("Aucun fichier publications.txt trouvé.\n");
        return;
    }

    int id;
    char date[20];
    char message[200];

    while (fscanf(file, "%d;%19[^;];%199[^\n]\n", &id, date, message) == 3) {
        User *u = search_user(root, id);
        if (u) push_publication(u, message, date);
    }

    fclose(file);
}
void show_timeline(User *root) {
    int id;
    printf("\nID utilisateur: ");
    if (scanf("%d", &id) != 1) { while (getchar() != '\n'); printf("ID invalide.\n"); return; }
    getchar();
    User *u = search_user(root, id);
    if (!u) { printf("Utilisateur non trouvé.\n"); return; }
    // Count number of friend posts
    int count = 0;
    Relation *rf = u->amis;

    for (rf = u->amis; rf; rf = rf->next) {
        User *friend = search_user(root, rf->id);
        if (!friend) continue;
        Publication *p;
        for (p = friend->publications; p; p = p->next)
            count++;
    }

    if (count == 0) {
        printf("\nAucune publication provenant des amis.\n");
        return;
    }


    TimelinePost *posts = malloc(count * sizeof(TimelinePost)); // posts = [TimelinePost, TimelinePost, ...]
    int index = 0;


    for (rf = u->amis; rf; rf = rf->next) {
        User *friend = search_user(root, rf->id);
        if (!friend) continue;

        Publication *p;
        for (p = friend->publications; p; p = p->next) {
            posts[index].id = friend->id;
            strcpy(posts[index].name, friend->name);
            strcpy(posts[index].message, p->message);
            strcpy(posts[index].date, p->date);
            index++;
        }
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (strcmp(posts[j].date, posts[i].date) > 0) {
                TimelinePost tmp = posts[i];
                posts[i] = posts[j];
                posts[j] = tmp;
            }
        }
    }


    printf("\n----- Flux de %s -----\n", u->name);
    for (int i = 0; i < count; i++) {
        printf("[%d] (%s) %s: %s\n",
            posts[i].id,
            posts[i].date,
            posts[i].name,
            posts[i].message
        );
    }
    free(posts);
}

int main() {
    User *root = NULL;
    int choice;

    titlescreen();
    credits();

    printf("Chargement des utilisateurs...\n");
    root = load_users();

    printf("Chargement des relations...\n");
    load_relations(root);

    printf("Chargement des publications...\n");
    load_publications(root);

    printf("Chargement terminé.\n\n");

    while (1) {

        choice = questions();

        switch (choice) {

        case 1: {
            root = add_user(root);    
            printf("\n-> Utilisateur ajouté.\n");
            break;
        }

        case 2: {
            int id;
            printf("\nID à supprimer: ");
            if (scanf("%d", &id) != 1) { while (getchar() != '\n'); printf("ID invalide.\n"); break; }
            getchar();
            root = delete_user(root, id);
            printf("-> Utilisateur supprimé.\n");
            break;
        }

        case 3: {
            int id;
            printf("\nID à chercher: ");
            if (scanf("%d", &id) != 1) { while (getchar() != '\n'); printf("ID invalide.\n"); break; }
            getchar();
            User *u = search_user(root, id);
            if (u) printf("-> Utilisateur trouvé: %s\n", u->name);
            else   printf("Utilisateur introuvable.\n");
            break;
        }

        case 4: {
            manage_relations(root);
            break;
        }

        case 5: {
            int id;
            char msg[200];
            char date[20];

            printf("\nID de l'utilisateur: ");
            if (scanf("%d", &id) != 1) { while (getchar() != '\n'); printf("ID invalide.\n"); break; }
            getchar();
            User *u = search_user(root, id);
            if (!u) { printf("Utilisateur non trouvé.\n"); break; }
            printf("Message: ");
            if (!fgets(msg, sizeof(msg), stdin)) msg[0] = '\0';
            msg[strcspn(msg, "\n")] = '\0';

            time_t now = time(NULL);
            struct tm *tm_info = localtime(&now);
            strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", tm_info);
            push_publication(u, msg, date);
            printf("-> Publication ajoutée.\n");
            break;
        }

        case 6:
            printf("\nListe des utilisateurs :\n");
            inorder(root);
            break;

        case 7:
            printf("\nStatistiques pas encore implémentées.\n");
            break;

        case 9:
            printf("\nSauvegarde en cours...\n");
            save_users(root);
            save_all_relations(root);
            save_all_publications(root);
            printf("Sauvegarde terminée. Au revoir !\n");
            exit(0);
        case 8:
            show_timeline(root);
            break;  

        default:
            printf("\nChoix invalide.\n");
            break;
        }

        printf("\n");
    }

    return 0;
}


