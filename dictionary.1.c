/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include "dictionary.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//we use this in the load and size functions to store and tell the dictionary size
unsigned int dict_size;

//this is the root node of the dictionary that functions should call
struct node *root;
struct node *new_temp_node_load;
struct node *temp_node_load; //this variable is the node we are currently creating in the trie in the load function
struct node *temp_node_check; //this variable is the node we are currently checking in the trie in the check function



/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word){
    temp_node_check = calloc(28, sizeof(struct node)); //give mem to temp node
    if (temp_node_check == NULL){
        return 10;
    }
    for(int i = 0; i < 27; i++){
        temp_node_check->children[i] = NULL;
    }
    temp_node_check = root; //initialize the working node to root.

    int s = strlen(word);

    for(int i = 0; i < s; i++){//read the string char by char until \0

        int cur_char = *(word+i);

        //normalize the chars to value 0 to 26.
        if (cur_char == 39){ //the character is apostrophe (\')
            cur_char = 26; //which should be at cell 27 in a node
        }
        else if (cur_char >= 97 && cur_char < 123){//if char is uppercase aplhabetical,
            cur_char = cur_char -97; //they are at cell 0 to 26.
        }
        else if (cur_char >= 65 && cur_char < 91){//if char is lowercase aplhabetical,
            cur_char = cur_char -65; //they are at cell 0 to 26.
        }

        //compare chars to cells. for each letter traverse the trie. If null, .
        //If not null, next letter. If last letter, not null && is_word==true, word is true.

        if (i+1==s && temp_node_check->children[cur_char]->is_word == true){ // we are at last  char of word and is_word is true
            return true; //word exist
        }
        else if (temp_node_check->children[cur_char]==NULL){ //if the current node cell for the char is not filled
            return false; // word = false,
        }
        else{//letter is true but word not yet, continue to next node
            temp_node_check = temp_node_check->children[cur_char];
        }

    }
    return false; //this should never be reached but is is necessary for the compiler
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary){
    // loads the dictionary file as trie and returns bool value.
    // the pointer created here is used in the check, size and unload functions to check words against

    FILE* dict_mem = malloc(sizeof(dictionary));
    if(dict_mem == NULL){
        return 8;
    }
    dict_mem = fopen(dictionary, "r");     //load the dictionary file into memory

    temp_node_load = calloc(28, sizeof(struct node)); //give mem to temp node
    if (temp_node_load == NULL){
        return 9;
    }
    for(int i = 0; i < 27; i++){
        temp_node_load->children[i] = NULL;
    }

    root = temp_node_load; //save the start node address in root node.

    int cur_char = 0; //variable to save the output of fgetc
    int iteration = 0;

    while (cur_char != EOF){     //read the dictionary char by char

        iteration++;
        cur_char = fgetc(dict_mem);

        if (cur_char == 39){ //the character is apostrophe (\')
            cur_char = 26; //which should be at cell 27 in a node
        }
        else if (cur_char >= 97 && cur_char < 123){//all other possible chars are lowercase aplhabetical,
            cur_char = cur_char -97; //they are at cell 0 to 26.
        }


        if (temp_node_load->children[cur_char]!=NULL){ //if the current node cell for the char is filled
            if (cur_char == 10){ //the current character is newline \n,
                temp_node_load->is_word = true; //end word
                dict_size++;
                temp_node_load = root; //for the next word start looking at root again
                continue;
            }
            temp_node_load = temp_node_load->children[cur_char]; // go to next char / node
        }
        else if (temp_node_load->children[cur_char]==NULL){ //if the current node is not yet filled
            if (cur_char == 10){ //the current character is newline \n,
                temp_node_load->is_word = true; //end word
                dict_size++;
                temp_node_load = root; //for the next word start looking at root again
                continue;
            }
            else{
                temp_node_load->is_word = false;
                new_temp_node_load = calloc(28, sizeof(struct node)); //create a new node
                if (new_temp_node_load == NULL){
                    return 10;
                }
                for(int i = 0; i < 27; i++){
                    new_temp_node_load->children[i] = NULL;
                }
                temp_node_load->children[cur_char] = new_temp_node_load; //fill the current node with the address of the new node
                temp_node_load = new_temp_node_load; //make the next iteration of the loop look at that new node
            }
        }
    }

    if (root == NULL){
    //    while(dict_mem != NULL){
    //        free(dict_mem);
    //    }
        return false, 2;
    }
    else{
    //    while(dict_mem != NULL){
    //        free(dict_mem);
    //    }
        return true;
    }
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded. I do this by counting newline chars.
 */
unsigned int size(void){
    return dict_size;
}
    //return number of strings in a file.
//    int output=0;
//    if (dict_mem != NULL){
//        while (output != EOF){
  //          output = (getc(*dict_mem));
    //        if (output  == 10)
      //      size++;
        //    }

//    }
//
  //  else{
    //    return 3;
  //  }


/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void){
    // unload the dictionary file
    // free all memory allocated in 'load'

    node* x; //this is the working node
    node* parent; //this is the parent of the working node
    //int address; //this saves the pointer address of previous free() rounds to prevent double free

//    for (int i = 0; i < 5; i++){

//       if (i==0){
            x = root; //this is the working node
            parent = root; //this is the parent of the working node
//        }
        else if (i==1){
//            x = new_temp_node_load; //this is the working node
//            parent = new_temp_node_load; //this is the parent of the working node
//        }
//        else if (i==2){
//            x = temp_node_load; //this is the working node
//            parent = temp_node_load; //this is the parent of the working node
//        }
//        else if (i==4){
//            x = temp_node_check; //this is the working node
//            parent = temp_node_check; //this is the parent of the working node
//        }
//
        while(x != NULL){ //the process is finished if it reached root and freed it.
            for(int j = 0; j < 28; j++){ //a node has 27 children may have to be freed
               if(x->children[j] != NULL){ //if a child is not null
                   parent = x; //x becomes the parent
                   x = x->children[j]; //the child becomes x
               }
               else{ //all children are null
                    free(x); //free the working node
                    x = parent; //the parent becomes the working node
               }
            }
        }
    }

    if(root == NULL && new_temp_node_load == NULL && temp_node_load == NULL && temp_node_check == NULL){
        return true;
    }
    else{
        return false;
    }
}