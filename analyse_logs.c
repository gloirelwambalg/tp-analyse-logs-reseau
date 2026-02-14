#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 256
#define MAX_PORT 10000

typedef struct {
    char date[11];
    char heure[9];
    char ip[20];
    int port;
    char protocole[10];
    char statut[10];
} LogEntry;

/* ================= LECTURE DES LOGS ================= */

LogEntry* lire_logs(const char* filename, int* total) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Erreur ouverture fichier\n");
        exit(1);
    }

    LogEntry* logs = NULL;
    char line[MAX_LINE];
    *total = 0;

    while (fgets(line, sizeof(line), file)) {
        logs = realloc(logs, (*total + 1) * sizeof(LogEntry));

        char* token = strtok(line, ";");
        strcpy(logs[*total].date, token);

        token = strtok(NULL, ";");
        strcpy(logs[*total].heure, token);

        token = strtok(NULL, ";");
        strcpy(logs[*total].ip, token);

        token = strtok(NULL, ";");
        logs[*total].port = atoi(token);

        token = strtok(NULL, ";");
        strcpy(logs[*total].protocole, token);

        token = strtok(NULL, ";\n");
        strcpy(logs[*total].statut, token);

        (*total)++;
    }

    fclose(file);
    return logs;
}

/* ================= ANALYSE ================= */

void analyser_logs(LogEntry* logs, int total) {

    int succes = 0, echec = 0;
    int ports[MAX_PORT] = {0};

    char ips[1000][20];
    int ip_count[1000] = {0};
    int unique_ips = 0;

    for (int i = 0; i < total; i++) {

        if (strcmp(logs[i].statut, "SUCCES") == 0)
            succes++;
        else
            echec++;

        ports[logs[i].port]++;

        int found = -1;
        for (int j = 0; j < unique_ips; j++) {
            if (strcmp(ips[j], logs[i].ip) == 0) {
                found = j;
                break;
            }
        }

        if (found == -1) {
            strcpy(ips[unique_ips], logs[i].ip);
            ip_count[unique_ips] = 1;
            unique_ips++;
        } else {
            ip_count[found]++;
        }
    }

    int max_port = 0;
    for (int i = 1; i < MAX_PORT; i++)
        if (ports[i] > ports[max_port])
            max_port = i;

    int max_ip_index = 0;
    for (int i = 1; i < unique_ips; i++)
        if (ip_count[i] > ip_count[max_ip_index])
            max_ip_index = i;

    printf("\n===== RESUME DES STATISTIQUES =====\n");
    printf("Total connexions : %d\n", total);
    printf("Succes : %d\n", succes);
    printf("Echecs : %d\n", echec);
    printf("Port le plus utilise : %d\n", max_port);
    printf("IP la plus active : %s\n", ips[max_ip_index]);

    /* Détection suspecte */
    FILE* rapport = fopen("rapport_analyse.txt", "w");

    fprintf(rapport, "===== RAPPORT D'ANALYSE RESEAU =====\n\n");
    fprintf(rapport, "Statistiques generales :\n");
    fprintf(rapport, "Total connexions : %d\n", total);
    fprintf(rapport, "Succes : %d\n", succes);
    fprintf(rapport, "Echecs : %d\n", echec);
    fprintf(rapport, "Port le plus utilise : %d\n", max_port);
    fprintf(rapport, "IP la plus active : %s\n\n", ips[max_ip_index]);

    printf("\n===== LA LISTE DES IPs SUSPECTES =====\n");
    fprintf(rapport, "Activites suspectes :\n");

    for (int i = 0; i < unique_ips; i++) {

        int echec_port[MAX_PORT] = {0};

        for (int j = 0; j < total; j++) {
            if (strcmp(logs[j].ip, ips[i]) == 0 &&
                strcmp(logs[j].statut, "ECHEC") == 0) {

                echec_port[logs[j].port]++;
            }
        }

        for (int p = 0; p < MAX_PORT; p++) {
            if (echec_port[p] > 5) {
                printf("IP suspecte : %s (Port %d)\n", ips[i], p);
                fprintf(rapport, "%s (Port %d)\n", ips[i], p);
            }
        }
    }

    fclose(rapport);
}

/* ================= MAIN ================= */

int main() {
    int total = 0;
    LogEntry* logs = lire_logs("network_log.txt", &total);
    analyser_logs(logs, total);
    free(logs);
    return 0;
}
