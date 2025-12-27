#include "inv.h"
#define max(a,b) (((a) > (b)) ? (a) : (b))

void stat_read(FILE *fp, stat_t *statp){
    fscanf(fp, " %d %d %d %d %d %d", &statp->hp, &statp->mp, &statp->atk, &statp->def, &statp->mag, &statp->spr);
}

// Stampa informazioni riguardanti le statistiche.
// Per i player, una statistica non può scendere sotto 1, quindi soglia sarà 1.
// Per gli item, soglia deve essere impostata a un valore arbitrariamente 
// negativo per consentire la corretta visualizzazione, siccome in teoria è -inf.
void stat_print(FILE *fp, stat_t *statp, int soglia){
    fprintf(fp, "HP: %d, MP: %d, ATK: %d, DEF: %d, MAG: %d, SPR: %d\n",
        max(soglia, statp->hp),
        max(soglia, statp->mp),
        max(soglia, statp->atk),
        max(soglia, statp->def),
        max(soglia, statp->mag),
        max(soglia, statp->spr)
    );
}

/* funzioni di input/output di un oggetto dell'inventario */
void inv_read(FILE *fp, inv_t *invp){
    fscanf(fp, " %s %s", invp->nome, invp->tipo);
    stat_read(fp, &invp->stat);
}


void inv_print(FILE *fp, inv_t *invp){
    fprintf(fp, "%s %s, ", invp->nome, invp->tipo);
    // vedi commento di stat_print per soglia.
    stat_print(fp, &invp->stat, -99999);
}

/* ritorna il campo stat di un oggetto dell'inventario */
stat_t inv_getStat(inv_t *invp){
    return invp->stat;
}