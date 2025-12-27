#include "pg.h"

#define SOGLIA 1

/* lettura e scrittura su file */
int pg_read(FILE *fp, pg_t *pgp){
    if (fscanf(fp, " %s %s %s", pgp->cod, pgp->nome, pgp->classe) != 3) return -1;
    printf("Leggo personaggio %s...\n", pgp->cod);
    stat_read(fp, &pgp->b_stat);

    pgp->equip = equipArray_init();
    return 0;
}
/* non essendo struct dinamica, pulisce chiamando il distruttire di equipArray */
void pg_clean(pg_t *pgp){
    equipArray_free(pgp->equip);
}

//can be private
stat_t computeStat(pg_t* pgp){
    stat_t ret = {
        pgp->b_stat.hp + pgp->eq_stat.hp,
        pgp->b_stat.mp + pgp->eq_stat.mp,
        pgp->b_stat.atk + pgp->eq_stat.atk,
        pgp->b_stat.def + pgp->eq_stat.def,
        pgp->b_stat.mag + pgp->eq_stat.mag,
        pgp->b_stat.spr + pgp->eq_stat.spr
    };

    return ret;
}

void pg_print(FILE *fp, pg_t *pgp, invArray_t invArray){
    fprintf(fp, "%s -- %s, %s\n", pgp->cod, pgp->nome, pgp->classe);
    stat_t stat = computeStat(pgp);
    stat_print(fp, &stat, SOGLIA);
}
/* modifica personaggio aggiungendo/togliendo un equipaggiamento selezionato da inventario:
di fatto e' sufficiente chiamare l'opportuna funzione dal modulo equipArray */
void pg_updateEquip(pg_t *pgp, invArray_t invArray){
    equipArray_update(pgp->equip, invArray);
    int j;
    for (int i = 0; i < EQUIP_SLOT; i++){
        if ((j = equipArray_getEquipByIndex(pgp->equip, i)) != -1){
            stat_t i_stat = invArray_getByIndex(invArray, j)->stat;
            pgp->eq_stat.hp += i_stat.hp;
            pgp->eq_stat.mp += i_stat.mp;
            pgp->eq_stat.atk += i_stat.atk;
            pgp->eq_stat.def += i_stat.def;
            pgp->eq_stat.mag += i_stat.mag;
            pgp->eq_stat.spr += i_stat.spr;
        }
    }
}