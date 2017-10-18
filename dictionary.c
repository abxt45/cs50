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
struct node *new_temp_node_load; //this is a place to temporarily save the address of a new node
struct node *temp_node_load; //this is the node we are currently creating in the trie in the load function


/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word){

    struct node* temp_node_check = root; //this is the node we are currently checking in the trie in the check function

    int s = strlen(word);

    for(int i = 0; i <= s; i++){//read the string char by char until \0

        int cur_char = *(word+i);

        //normalize the chars to value 0 to 26.
        if (cur_char == 39){ //the character is apostrophe (\')
            cur_char = 26; //which should be at cell 26 in a node
        }
        else if (cur_char >= 97 && cur_char < 123){//if char is uppercase aplhabetical,
            cur_char = cur_char -97; //they are at cell 0 to 26.
        }
        else if (cur_char >= 65 && cur_char < 91){//if char is lowercase aplhabetical,
             cur_char = cur_char -65; //they are at cell 0 to 26.
        }

        //compare chars to cells. for each letter traverse the trie. If null, .
        //If not null, next letter. If last letter, not null && is_word==true, word is true.

        if (i<s && temp_node_check->children[cur_char]!=NULL){//we are not at the last char of word, and the next letter exists
            temp_node_check = temp_node_check->children[cur_char]; // we will look at the next letter
        }
        else if (i==s && temp_node_check->is_word == true){ // we are at last char of word and a word DOES end at this node
            return true; //word does exist
        }
        else if (i==s && temp_node_check->is_word == false){ // we are at last char of word and a word DOES NOT end at this node
            return false; //word does not exist
        }
        else if (i<s && temp_node_check->children[cur_char]==NULL){ //we are not at last char of word, but the next letter does not exist
            return false; // word does not exist,
        }
    }
    if (temp_node_check != NULL){
    free(temp_node_check);
    }
    return false; //this should never be reached but is is necessary for the compiler
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary){
    // loads the dictionary file as trie and returns bool value.
    // the pointer created here is used in the check, size and unload functions to check words against

    FILE* dict_mem = fopen(dictionary, "r");     //load the dictionary file into memory
    if(dict_mem == NULL){
        return 8;
    }

    temp_node_load = calloc(1, sizeof(struct node)); //give mem to temp node
    if (temp_node_load == NULL){
        return 9;
    }
    for(int i = 0; i < 27; i++){
        temp_node_load->children[i] = NULL;
    }

    root = temp_node_load; //save the start node address in root node.

    int cur_char; //variable to save the output of fgetc

    while ((cur_char = fgetc(dict_mem)) != EOF){     //read the dictionary char by char

        //this part ends the previous word
        if (cur_char == 10){ // the current character is newline \n
            temp_node_load->is_word = true; //a word ends at this node
            dict_size++; //up the word count
            temp_node_load = root; //for the next word start looking at root again
            continue;
        }

    //this part normalizes the characters to node cell numbers
        else if (cur_char == 39){ //the character is apostrophe (\')
            cur_char = 26; //which should be at cell 27 in a node
        }
        else if (cur_char >= 97 && cur_char < 123){//these chars are lowercase aplhabetical,
            cur_char = cur_char -97; //they are at cell 0 to 26.
        }

    //this part puts the letters in the trie
        if (temp_node_load->children[cur_char]!=NULL){ //this situation means a longer word has ALREADY been put in the trie here
            temp_node_load = temp_node_load->children[cur_char]; // go to the next char / node
        }
        else if (temp_node_load->children[cur_char]==NULL){ //this situation means a word has NOT YET been put in the trie here
            new_temp_node_load = calloc(1, sizeof(struct node)); //create a new node
            if (new_temp_node_load == NULL){
                return 10;
            }
            new_temp_node_load->is_word = false; //put the values of the new node to null/false.
            for(int i = 0; i < 27; i++){
                new_temp_node_load->children[i] = NULL;
            }
            temp_node_load->children[cur_char] = new_temp_node_load; //fill the current node with the address of the new node
            temp_node_load = new_temp_node_load; //make the next iteration of the loop look at that new node
        }
    }

    fclose(dict_mem);
    if (root == NULL){
        return false, 2;
    }
    else{
        return true;
    }
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded. I do this by counting newline chars.
 */
unsigned int size(void){
    return dict_size;
}

void unload_rec(struct node* n){

    // unload the dictionary file
        // free all memory allocated in 'load'
        // all temporary pointers are contained in root pointer. If root
        // is freed correctly, so are they

        //howto:
        //unload from BOTTOM to TOP
        //travel to lowest possible node
            //free all pointers in children
            //go one node up
            //repeat free all pointers in children
            //stop once root node is freed
        //recursion!

        //base case: a node has no children. If so, free the node
        //recursive call: to free x, free all children of x

    for(int j = 0; j < 27; j++){ //check all children of the node
        if(n->children[j] != NULL){ //if a child is not null
           unload_rec(n->children[j]); //call unload_rec on the child
          // children_null=false; //
        }
    }
    free(n); //free the working node
}


/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void){
    unload_rec(root);
    //unload_rec(new_temp_node_load);
    //unload_rec(temp_node_load);
    return true;
}