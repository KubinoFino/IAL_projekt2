/*
 * Binární vyhledávací strom — iterativní varianta
 *
 * S využitím datových typů ze souboru btree.h, zásobníku ze souboru stack.h 
 * a připravených koster funkcí implementujte binární vyhledávací 
 * strom bez použití rekurze.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializace stromu.
 *
 * Uživatel musí zajistit, že inicializace se nebude opakovaně volat nad
 * inicializovaným stromem. V opačném případě může dojít k úniku paměti (memory
 * leak). Protože neinicializovaný ukazatel má nedefinovanou hodnotu, není
 * možné toto detekovat ve funkci. 
 */
void bst_init(bst_node_t **tree) {
  (*tree) = NULL;
}

/*
 * Vyhledání uzlu v stromu.
 *
 * V případě úspěchu vrátí funkce hodnotu true a do proměnné value zapíše
 * hodnotu daného uzlu. V opačném případě funkce vrátí hodnotu false a proměnná
 * value zůstává nezměněná.
 * 
 * Funkci implementujte iterativně bez použité vlastních pomocných funkcí.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  if (tree == NULL) {
    return false;
  }

  bst_node_t *current = tree;
  while (current != NULL) {
    if (current->key == key) {
      if (value != NULL){
        *value = current->value;
      }
      return true;
    } else if (key > current->key) {
      current = current->right;
    } else {
      current = current->left;
    }
  }	
  return false;
}

/*
 * Vložení uzlu do stromu.
 *
 * Pokud uzel se zadaným klíče už ve stromu existuje, nahraďte jeho hodnotu.
 * Jinak vložte nový listový uzel.
 *
 * Výsledný strom musí splňovat podmínku vyhledávacího stromu — levý podstrom
 * uzlu obsahuje jenom menší klíče, pravý větší. 
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  if (*tree == NULL) {
    bst_node_t *new_node = malloc(sizeof(bst_node_t));
    new_node->key = key;
    new_node->value = value;
    new_node->left = NULL;
    new_node->right = NULL;
    *tree = new_node;
    return;
  }
  bool found = false;
  bst_node_t *current = *tree;
  bst_node_t *parent = NULL;
  while (current != NULL) {
    if (current->key == key) {
      found = true;
      current->value = value;
      return;
    } else if (key > current->key) {
      parent = current;
      current = current->right;
    } else {
      parent = current;
      current = current->left;
    }
  }

  if (found == 1 && current != NULL){
    current->value = value;
  } else {
    bst_node_t *new_node = malloc(sizeof(bst_node_t));
    new_node->key = key;
    new_node->value = value;
    new_node->left = NULL;
    new_node->right = NULL;
    if (key > parent->key) {
      parent->right = new_node;
    } else {
      parent->left = new_node;
    }
  }
}

/*
 * Pomocná funkce která nahradí uzel nejpravějším potomkem.
 * 
 * Klíč a hodnota uzlu target budou nahrazené klíčem a hodnotou nejpravějšího
 * uzlu podstromu tree. Nejpravější potomek bude odstraněný. Funkce korektně
 * uvolní všechny alokované zdroje odstraněného uzlu.
 *
 * Funkce předpokládá, že hodnota tree není NULL.
 * 
 * Tato pomocná funkce bude využita při implementaci funkce bst_delete.
 *
 * Funkci implementujte iterativně bez použití vlastních pomocných funkcí.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  
  bst_node_t *parent = (*tree);
  
  if (parent->right == NULL && parent->left == NULL){
    target->key = parent->key;
    target->value = parent->value;
    free(parent);
    *tree = NULL;
    return;
  }
  
  bst_node_t *current = parent->right;

  while(current->right != NULL) {
    parent = current;
    current = parent->right;
  }

  if (current->right == NULL ){
    target->key = current->key;
    target->value = current->value;
  }

  if (current->left != NULL) {
    parent->right = current->left;
  } else {
    parent->right = NULL;
  }
  free(current);
}

/*
 * Odstranění uzlu ze stromu.
 *
 * Pokud uzel se zadaným klíčem neexistuje, funkce nic nedělá.
 * Pokud má odstraněný uzel jeden podstrom, zdědí ho rodič odstraněného uzlu.
 * Pokud má odstraněný uzel oba podstromy, je nahrazený nejpravějším uzlem
 * levého podstromu. Nejpravější uzel nemusí být listem.
 * 
 * Funkce korektně uvolní všechny alokované zdroje odstraněného uzlu.
 * 
 * Funkci implementujte iterativně pomocí bst_replace_by_rightmost a bez
 * použití vlastních pomocných funkcí.
 */
void bst_delete(bst_node_t **tree, char key) {
  if(*tree == NULL || bst_search(*tree, key, NULL) == false) {
    return;
  }
  bst_node_t *current = (*tree);
  bst_node_t *parent = (*tree);

  while(current != NULL){
    if (current->key == key) {
      break;
    } else if (key > current->key) {
      parent = current;
      current = parent->right;
    } else {
      parent = current;
      current = parent->left;
    }
  }
  
  if (current->left == NULL && current->right == NULL){
    if (parent == NULL){
      free(current);
      *tree = NULL;
      
      return;
    }else if (current->key > parent->key){
      free(current);
      parent->right = NULL;
      
      return;
    }else if(current->key < parent->key){
      free(current);
      parent->left = NULL;
      return;
    }
  }else if (current->left == NULL && current->right != NULL){
    if (parent == NULL){
      *tree = current->right;
      free(current);
      return;
    }else if (current->key > parent->key){
      parent->right = current->right;
      free(current);
      return;
    }else if(current->key < parent->key){
      parent->left = current->right;
      free(current);
      return;
    }
  }else if (current->right == NULL && current->left != NULL){
    if (parent == NULL){
      *tree = current->left;
      free(current);
      return;
    }else if (current->key > parent->key){
      parent->right = current->left;
      free(current);
      return;
    }else if(current->key < parent->key){
      parent->left = current->left;
      free(current);
      return;
    }
  }else {
    bst_replace_by_rightmost(current, &(current->left));
    return;
  }
}


/*
 * Zrušení celého stromu.
 * 
 * Po zrušení se celý strom bude nacházet ve stejném stavu jako po 
 * inicializaci. Funkce korektně uvolní všechny alokované zdroje rušených 
 * uzlů.
 * 
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_dispose(bst_node_t **tree) {

  if (*tree == NULL) {
    return;
  }

  stack_bst_t *stack = malloc(sizeof(stack_bst_t));
  if (stack == NULL) {
    return;
  }
  stack_bst_init(stack);

  bst_node_t *current = *tree;

  while (!stack_bst_empty(stack) || current) {
    if (current == NULL) { 
      current = stack_bst_pop(stack); 
    }

    if (current->right) {
      stack_bst_push(stack, current->right);
    }

    bst_node_t *delete = current;
    current = current->left;
    free(delete);
  }

  *tree = NULL;
  stack_bst_empty(stack);	
}

/*
 * Pomocná funkce pro iterativní preorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu.
 * Nad zpracovanými uzly zavolá bst_add_node_to_items a uloží je do zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit, bst_items_t *items) {
  bst_node_t *current = tree;

  while (current != NULL) {
    bst_add_node_to_items(current, items);
    stack_bst_push(to_visit, current);
    current = current->left;
  }
}

/*
 * Preorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_preorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_preorder(bst_node_t *tree, bst_items_t *items) {
  stack_bst_t *stack = malloc(sizeof(stack_bst_t));
  if (stack == NULL) {
    return;
  }
  stack_bst_init(stack);

  bst_leftmost_preorder(tree, stack, items);

  while (!stack_bst_empty(stack)) {
    bst_node_t *current = stack_bst_pop(stack);
    bst_leftmost_preorder(current->right, stack, items);
  }
}

/*
 * Pomocná funkce pro iterativní inorder.
 * 
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů.
 *
 * Funkci implementujte iterativně s pomocí zásobníku a bez použití 
 * vlastních pomocných funkcí.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {

  bst_node_t *current = tree;

  while (current != NULL) {
    stack_bst_push(to_visit, current);
    current = current->left;
  }
}

/*
 * Inorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_inorder a
 * zásobníku uzlů a bez použití vlastních pomocných funkcí.
 */
void bst_inorder(bst_node_t *tree, bst_items_t *items) {
  stack_bst_t *stack = malloc(sizeof(stack_bst_t));
  if (stack == NULL) {
    return;
  }
  stack_bst_init(stack);

  bst_leftmost_inorder(tree, stack);

  while (!stack_bst_empty(stack)) {
    bst_node_t *current = stack_bst_pop(stack);
    bst_add_node_to_items(current, items);
    bst_leftmost_inorder(current->right, stack);
  }
}

/*
 * Pomocná funkce pro iterativní postorder.
 *
 * Prochází po levé větvi k nejlevějšímu uzlu podstromu a ukládá uzly do
 * zásobníku uzlů. Do zásobníku bool hodnot ukládá informaci, že uzel
 * byl navštíven poprvé.
 *
 * Funkci implementujte iterativně pomocí zásobníku uzlů a bool hodnot a bez použití
 * vlastních pomocných funkcí.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {

  bst_node_t *current = tree;

  while (current != NULL) {
    stack_bst_push(to_visit, current);
    stack_bool_push(first_visit, true);
    current = current->left;
  }
}

/*
 * Postorder průchod stromem.
 *
 * Pro aktuálně zpracovávaný uzel zavolejte funkci bst_add_node_to_items.
 *
 * Funkci implementujte iterativně pomocí funkce bst_leftmost_postorder a
 * zásobníku uzlů a bool hodnot a bez použití vlastních pomocných funkcí.
 */
void bst_postorder(bst_node_t *tree, bst_items_t *items) {
  stack_bst_t *stack = malloc(sizeof(stack_bst_t));
  if (stack == NULL) {
    return;
  }
  stack_bst_init(stack);

  stack_bool_t *first_visit = malloc(sizeof(stack_bool_t));
  if (first_visit == NULL) {
    return;
  }
  stack_bool_init(first_visit);

  bst_leftmost_postorder(tree, stack, first_visit);

  while (!stack_bst_empty(stack)) {
    bst_node_t *current = stack_bst_top(stack);
    bool first = stack_bool_top(first_visit);
    if (first) {
      stack_bool_pop(first_visit);
      stack_bool_push(first_visit, false);
      bst_leftmost_postorder(current->right, stack, first_visit);
    } else {
      bst_add_node_to_items(current, items);
      stack_bst_pop(stack);
      stack_bool_pop(first_visit);
    }
  }
}