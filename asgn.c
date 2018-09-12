#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "tree.h"
#include "mylib.h"
#include "htable.h"


int main(int argc, char **argv) {

    FILE *document;
    char *document_name = NULL;
    int table_size = 113; /* Default size of hashtable */
    const char *optstring = "Tc:deoprs:t:h";
    char option;
    int snaphots = 0;
    htable h;
    tree t;
    
    /* Command Line Flags */
    unsigned int data_stucture = 0; /* default is hash table */
    unsigned int collision_strategy = 0; /* default is LINEAR_P */
    unsigned int display_entire_contents = 0;
    unsigned int output_tree_representation = 0;
    unsigned int print_stats_info = 0;
    unsigned int spell_check = 0;
    unsigned int use_snapshots = 0;
    unsigned int tree_type = 0; /* default is bst */
    unsigned int help = 0;


    if (argc > 1) {
        while ((option = getopt(argc, argv, optstring)) != EOF) {
            switch (option) {
                case 'T' :
                    data_stucture = 1;
                    break;
                case 'c' :
                    document_name = optarg;
                    spell_check = 1;
                    break;
                case 'd' :
                    collision_strategy = 1;
                    break;
                case 'e' :
                    display_entire_contents = 1;
                    break;
                case 'o' :
                    output_tree_representation = 1;
                    break;
                case 'p' :
                    print_stats_info = 1;
                    break;
                case 'r' :
                    tree_type = 1;
                    break;
                case 's' :
                    use_snapshots = 1;
                    snaphots = atoi(optarg);
                    break;
                case 'h' :
                    help = 1;
                    break;
                case 't' :
                    table_size = atoi(optarg);
                    break;
                default:
                    break;
            }
        }                                                     
   

    


        print_help(help);
        if (spell_check) {
            document = open_file(document_name);
        }
    
        /* Tree */
        if (data_stucture) {
            /* Process tree arguments */
            if (tree_type) {
                t = tree_new(RBT);
            } else {;
                t = tree_new(BST);
            }
            t = insert_words_into_tree(t, stdin);
            if (spell_check) {
                search_tree(t, document);
                print_basic_stats();
            }
            /* When -c is given the -p and -o options should be ignored */
            if (output_tree_representation && spell_check == 0) {
                tree_output_dot(t, stdout);
            }
            tree_free(t);
        } else {
            /* Process hash table arguments */
            table_size = get_next_prime(table_size);
            if (collision_strategy) {
                h = htable_new(table_size, DOUBLE_H);
            } else {
                h = htable_new(table_size, LINEAR_P);
            }
            insert_words_into_htable(h, stdin);

            
            
            if (display_entire_contents) {
                htable_print_entire_table(h, stderr);
            }
            if (spell_check) {
                search_htable(h, document);
                print_basic_stats();
            }

            /* When -c is given the -p and -o options should be ignored */
            if (spell_check == 0) {
                if (print_stats_info && use_snapshots) {
                    htable_print_stats(h, stdout, snaphots);
                }
            }
            htable_free(h);
        }
        if (spell_check) {
            fclose(document);
        }
    } else {
        print_help(1);
    }
    return EXIT_SUCCESS;
}
