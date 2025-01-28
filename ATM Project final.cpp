#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Classe de base pour les comptes
class Compte {
protected:
    string ID;
    int PIN;
    string Nom;
    string Prenom;
    double Balance;
    double Limite; // 
    bool Status; // true = blocked, false = active
    int essaisEchoues; // Tentatives échouees de connexion

public:
    // Constructeur
    Compte(string id, int pin, string nom, string prenom, double balance, double limite)
        : ID(id), PIN(pin), Nom(nom), Prenom(prenom), Balance(balance), Limite(limite), Status(false), essaisEchoues(0) {}

    // Getters
    string getID() { return ID; }
    int getPIN() { return PIN; }
    string getNom() { return Nom; }
    string getPrenom() { return Prenom; }
    double getBalance() { return Balance; }
    double getLimite() { return Limite; }
    bool getStatus() { return Status; }

    // Setters
    void setBalance(double balance) { Balance = balance; }
    void setLimite(double limite) { Limite = limite; }
    void setStatus(bool status) { Status = status; }
    void resetTentatives() { essaisEchoues = 0; }


    // !! Main functions !!
    bool authenticate(string inputID, int inputPIN) {
        if (ID == inputID) {
            if (Status) {
                cout << "Ce compte est bloque.\n";
                return false;
            }
            if (PIN == inputPIN) {
                essaisEchoues = 0; // restarted the counter after sucess
                return true;
            } else {
                essaisEchoues++;
                if (essaisEchoues >= 3) {
                    Status = true; // Bloque le compte
                    cout << "Compte bloque apres 3 tentatives échouees.\n";
                } else {
                    cout << "PIN incorrect. Tentative " << essaisEchoues << "/3.\n";
                }
                return false;
            }
        }
        return false;
    }
};

// Classe Client
class Client : public Compte {
public:
    Client(string id, int pin, string nom, string prenom, double balance, double limite)
        : Compte(id, pin, nom, prenom, balance, limite) {}

    void afficherInformations() {
        cout << "--- INFORMATIONS DU COMPTE ---\n";
        cout << "Nom: " << Nom << "\nPrenom: " << Prenom << "\nID: " << ID
             << "\nSolde: " << Balance << " Dh\nLimite: " << Limite << " Dh\n";
        cout << "Statut: " << (Status ? "Bloque" : "Actif") << "\n";
    }

    // Voir le solde
    void voirSolde() {
        cout << "Votre solde actuel est de " << Balance << " Dh.\n";
    }

    // Retrait d'argent
    void retirerArgent(double montant) {
        if (montant > Limite) {
            cout << "Le montant depasse votre limite de retrait.\n";
        } else if (montant > Balance) {
            cout << "Fonds insuffisants.\n";
        } else {
            Balance -= montant;
            cout << "Retrait de " << montant << " Dh effectue avec succes.\n";
            cout << "Nouveau solde : " << Balance << " Dh.\n";
        }
    }

    // Dépôt d'argent
    void deposerArgent(double montant) {
        Balance += montant;
        cout << "Depot de " << montant << " Dh effectue avec succes.\n";
        cout << "Nouveau solde : " << Balance << " Dh.\n";
    }

    // Transfert d'argent
    bool transfererArgent(Client& destinataire, double montant) {
        if (montant > Balance) {
            cout << "Fonds insuffisants pour effectuer ce transfert.\n";
            return false;
        }
        Balance -= montant;
        destinataire.setBalance(destinataire.getBalance() + montant);
        cout << "Transfert de " << montant << " Dh a " << destinataire.getNom()
             << " " << destinataire.getPrenom() << " effectue avec succes.\n";
        cout << "Nouveau solde : " << Balance << " Dh.\n";
        return true;
    }
};

// Classe Admin
class Admin : public Compte {
public:
    Admin(string id, int pin, string nom, string prenom)
        : Compte(id, pin, nom, prenom, 0, 0) {}

    void afficherMenuAdmin(vector<Client>& clients) {
        int choix;
        do {
            cout << "\n--- MENU ADMINISTRATEUR ---\n";
            cout << "1. Creer un utilisateur\n";
            cout << "2. Ajouter un solde a un utilisateur\n";
            cout << "3. Bloquer un compte\n";
            cout << "4. Debloquer un compte\n";
            cout << "5. Afficher tous les utilisateurs\n";
            cout << "6. Rechercher un utilisateur\n";
            cout << "7. Quitter\n";
            cout << "Votre choix : ";
            cin >> choix;

            switch (choix) {
                case 1: {
                    string id, nom, prenom;
                    int pin;
                    double balance, limite;
                    cout << "Entrez l'ID : ";
                    cin >> id;
                    cout << "Entrez le PIN : ";
                    cin >> pin;
                    cout << "Entrez le nom : ";
                    cin >> nom;
                    cout << "Entrez le prenom : ";
                    cin >> prenom;
                    cout << "Entrez le solde initial : ";
                    cin >> balance;
                    cout << "Entrez la limite : ";
                    cin >> limite;
                    clients.push_back(Client(id, pin, nom, prenom, balance, limite));
                    cout << "Utilisateur cree avec succes.\n";
                    break;
                }
                case 2: {
                    string id;
                    double montant;
                    cout << "Entrez l'ID de l'utilisateur : ";
                    cin >> id;
                    cout << "Entrez le montant a ajouter : ";
                    cin >> montant;
                    bool trouve = false;
                    for (int i = 0; i < clients.size(); i++) {
                        if (clients[i].getID() == id) {
                            clients[i].deposerArgent(montant);
                            trouve = true;
                            break;
                        }
                    }
                    if (!trouve) cout << "Utilisateur introuvable.\n";
                    break;
                }
                case 3: {
                    string id;
                    cout << "Entrez l'ID du compte à bloquer : ";
                    cin >> id;
                    bool trouve = false;
                    for (int i = 0; i < clients.size(); i++) {
                        if (clients[i].getID() == id) {
                            clients[i].setStatus(true);
                            cout << "Compte bloqué avec succès.\n";
                            trouve = true;
                            break;
                        }
                    }
                    if (!trouve) cout << "Utilisateur introuvable.\n";
                    break;
                }
                case 4: {
                    string id;
                    cout << "Entrez l'ID du compte à débloquer : ";
                    cin >> id;
                    bool trouve = false;
                    for (int i = 0; i < clients.size(); i++) {
                        if (clients[i].getID() == id) {
                            clients[i].setStatus(false);
                            clients[i].resetTentatives();
                            cout << "Compte débloqué avec succès.\n";
                            trouve = true;
                            break;
                        }
                    }
                    if (!trouve) cout << "Utilisateur introuvable.\n";
                    break;
                }
                case 5: {
                    cout << "\n--- LISTE DES UTILISATEURS ---\n";
                    for (size_t i = 0; i < clients.size(); i++) {
                        clients[i].afficherInformations();
                    }
                    break;
                }
                case 6: {
                    string id;
                    cout << "Entrez l'ID de l'utilisateur à rechercher : ";
                    cin >> id;
                    bool trouve = false;
                    for (int i = 0; i < clients.size(); i++) {
                        if (clients[i].getID() == id) {
                            clients[i].afficherInformations();
                            trouve = true;
                            break;
                        }
                    }
                    if (!trouve) cout << "Utilisateur introuvable.\n";
                    break;
                }
                case 7:
                    cout << "Déconnexion de l'administrateur...\n";
                    break;
                default:
                    cout << "Choix invalide.\n";
            }
        } while (choix != 7);
    }
};

// Fonction menu utilisateur
void afficherMenuUtilisateur(Client& client, vector<Client>& clients) {
    int choix;
    do {
        cout << "\n--- MENU UTILISATEUR ---\n";
        cout << "1. Voir mes informations\n";
        cout << "2. Voir mon solde\n";
        cout << "3. Retirer de l'argent\n";
        cout << "4. Déposer de l'argent\n";
        cout << "5. Transférer de l'argent\n";
        cout << "6. Quitter\n";
        cout << "Votre choix : ";
        cin >> choix;

        switch (choix) {
            case 1:
                client.afficherInformations();
                break;
            case 2:
                client.voirSolde();
                break;
            case 3: {
                double montant;
                cout << "Entrez le montant à retirer : ";
                cin >> montant;
                client.retirerArgent(montant);
                break;
            }
            case 4: {
                double montant;
                cout << "Entrez le montant à déposer : ";
                cin >> montant;
                client.deposerArgent(montant);
                break;
            }
            case 5: {
                string idDestinataire;
                double montant;
                cout << "Entrez l'ID du destinataire : ";
                cin >> idDestinataire;
                cout << "Entrez le montant à transférer : ";
                cin >> montant;

                bool transfertEffectue = false;
                for (int i = 0; i < clients.size(); i++) {
                    if (clients[i].getID() == idDestinataire) {
                        transfertEffectue = true;
						client.transfererArgent(clients[i], montant);
                        break;
                    }
                }
                if (!transfertEffectue) {
                    cout << "ID du destinataire introuvable ou transfert échoué.\n";
                }
                break;
            }
            case 6:
                cout << "Déconnexion...\n";
                break;
            default:
                cout << "Choix invalide.\n";
        }
    } while (choix != 6);
}

int main() {
    vector<Client> clients;
    clients.push_back(Client("Clt1", 1234, "Client", "1", 5000, 1000));
    clients.push_back(Client("Clt2", 5678, "Client", "2", 8000, 2000));

    Admin admin("admin", 0, "Admin", "Super");

    while (true) {
        int choix;
        cout << "\n--- MENU PRINCIPAL ---\n";
        cout << "1. Authentifier\n";
        cout << "2. Quitter\n";
        cout << "Votre choix : ";
        cin >> choix;

        if (choix == 1) {
            string inputID;
            int inputPIN;

            cout << "Entrez votre ID : ";
            cin >> inputID;
            cout << "Entrez votre PIN : ";
            cin >> inputPIN;

            if (inputID == "admin" && admin.authenticate(inputID, inputPIN)) {
                admin.afficherMenuAdmin(clients);
            } else {
                bool authenticated = false;
                for (int i = 0; i < clients.size(); i++) {
                    if (clients[i].authenticate(inputID, inputPIN)) {
                        authenticated = true;
                        afficherMenuUtilisateur(clients[i], clients);
                        break;
                    }
                }
                if (!authenticated) {
                    cout << "ID ou PIN incorrect.\n";
                }
            }
        } else if (choix == 2) {
            cout << "Au revoir !\n";
            break;
        } else {
            cout << "Choix invalide.\n";
        }
    }

    return 0;
}

