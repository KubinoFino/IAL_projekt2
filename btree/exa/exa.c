/*
 * Použití binárních vyhledávacích stromů.
 *
 * S využitím Vámi implementovaného binárního vyhledávacího stromu (soubory ../iter/btree.c a ../rec/btree.c)
 * implementujte funkci letter_count. Výstupní strom může být značně degradovaný (až na úroveň lineárního seznamu) 
 * a tedy implementujte i druhou funkci (bst_balance), která strom, na požadavek uživatele, vybalancuje.
 * Funkce jsou na sobě nezávislé a tedy automaticky NEVOLEJTE bst_balance v letter_count.
 * 
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


/**
 * Vypočítání frekvence výskytů znaků ve vstupním řetězci.
 * 
 * Funkce inicilializuje strom a následně zjistí počet výskytů znaků a-z (case insensitive), znaku 
 * mezery ' ', a ostatních znaků (ve stromu reprezentováno znakem podtržítka '_'). Výstup je v 
 * uložen ve stromu.
 * 
 * Například pro vstupní řetězec: "abBccc_ 123 *" bude strom po běhu funkce obsahovat:
 * 
 * key | value
 * 'a'     1
 * 'b'     2
 * 'c'     3
 * ' '     2
 * '_'     5
 * 
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce.
*/
void letter_count(bst_node_t **tree, char *input) {
    bst_init(tree);
    for (int i = 0; i < sizeof(input) ; i++) {
        if (input[i] >= 'a' && input[i] <= 'z') {
            if (bst_search(*tree, input[i], NULL)) {
                (*tree)->value++;
            } else {
                bst_insert(tree, input[i], 1);
            }
        }else if (input[i] >= 'A' && input[i] <= 'Z') {
            if (bst_search(*tree, input[i] + 32, NULL)) {
                (*tree)->value++;
            } else {
                bst_insert(tree, input[i] + 32, 1);
            }
        }else if (input[i] == ' ') {
            if (bst_search(*tree, input[i], NULL)) {
                (*tree)->value++;
            } else {
                bst_insert(tree, input[i], 1);
            }
        }else {
            if (bst_search(*tree, '_', NULL)) {
                (*tree)->value++;
            } else {
                bst_insert(tree, '_', 1);
            }
        }
    }
}



/**
 * Vyvážení stromu.
 * 
 * Vyvážený binární vyhledávací strom je takový binární strom, kde hloubka podstromů libovolného uzlu se od sebe liší maximálně o jedna.
 * 
 * Předpokládejte, že strom je alespoň inicializován. K získání uzlů stromu využijte vhodnou verzi vámi naimplmentovaného průchodu stromem.
 * Následně můžete například vytvořit nový strom, kde pořadím vkládaných prvků zajistíte vyváženost.
 *  
 * Pro implementaci si můžete v tomto souboru nadefinovat vlastní pomocné funkce. Není nutné, aby funkce fungovala *in situ* (in-place).
*/
void bst_balance(bst_node_t **tree) {
    bst_items_t *items = malloc(sizeof(bst_items_t));
    bst_inorder(*tree, items);

    int mid = items->size / 2;
    bst_node_t *new_tree = NULL;
    bst_insert(&new_tree, items->nodes[mid]->key, items->nodes[mid]->value);
    for (int i = 0; i < items->size; i++) {
        if (i != mid) {
            bst_insert(&new_tree, items->nodes[i]->key, items->nodes[i]->value);
        }
    }
    *tree = new_tree;
    return;
}