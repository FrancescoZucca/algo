#include "equipArray.h"

struct equipArray_s {
    int slot[EQUIP_SLOT];
}; 

/* creatore e disruttore */
equipArray_t equipArray_init(){
    equipArray_t ret = malloc(sizeof(struct equipArray_s));
    for (int i = 0; i < EQUIP_SLOT; i++)
        ret->slot[i] = -1;
    return ret;
}
void equipArray_free(equipArray_t equipArray){
    free(equipArray);
}

/* quanti equipaggiamenti sono in uso e qual è il primo slot libero */
int equipArray_inUse(equipArray_t equipArray, int* freeSlot){
    int ret = 0;
    *freeSlot = -1;
    for (int i = 0; i < EQUIP_SLOT; i++)
        if (equipArray->slot[i] != -1) ret++;
        else if (*freeSlot == -1) *freeSlot = i;

    return ret;
}

/* scrittura su file */
void equipArray_print(FILE *fp, equipArray_t equipArray, invArray_t invArray){
    for (int i = 0; i < EQUIP_SLOT; i++) {
        printf("Slot %d - ", i);
        if (equipArray->slot[i] != -1)
            invArray_printByIndex(fp, invArray, equipArray->slot[i]);
        else
            printf("Vuoto.\n");
    }
}
/* modifica equipaggiamento scegliendo un oggetto da inventario */
void equipArray_update(equipArray_t equipArray, invArray_t invArray){
    equipArray_print(stdout, equipArray, invArray);
    printf("Che slot vuoi occupare? - ");
    int slot = -1;
    scanf(" %d", &slot);
    if (slot < 0 || slot >= EQUIP_SLOT) return;
    printf("Indica che item vuoi equipaggiare.\n");
    invArray_print(stdout, invArray);
    printf("Che item vuoi equipaggiare?\n- ");
    scanf(" %d", &equipArray->slot[slot]);
}
/* torna indice (nel vettore inventario) dell'oggetto in posizione index (0..EQUIP_SLOT-1) di equipArray */
int equipArray_getEquipByIndex(equipArray_t equipArray, int index){
    if (index < 0 || index >= EQUIP_SLOT) return -1;
    return equipArray->slot[index];
}