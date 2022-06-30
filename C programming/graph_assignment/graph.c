#include <stdlib.h>
#include<stdio.h>
#include<stdbool.h>

#include "graph.h"

/* This is a testing file just to be able to compile the 
 * examples of binary files to convert graph from txt->bin
 * and bin -> txt
 */

// - function -----------------------------------------------------------------
graph_t* allocate_graph() {
   graph_t *graph = (graph_t *)malloc(sizeof(graph_t) * 1);

   if (graph == NULL) {
      fprintf(stderr, "Error while memory allocation!\n");
      exit(100);
   }
   graph->capacity = 100;
   graph->from = (int *)malloc(graph->capacity * sizeof(int));
   graph->to = (int *)malloc(graph->capacity * sizeof(int));
   graph->value = (int *)malloc(graph->capacity * sizeof(int));
   return graph;
}

// - function -----------------------------------------------------------------
void free_graph(graph_t **graph) {
   free((*graph)->from);
   free((*graph)->to);
   free((*graph)->value);
   free((*graph));
}

// - function -----------------------------------------------------------------
void load_txt(const char *fname, graph_t *graph) {
   FILE *file_name = fopen(fname, "r");

   int i = 0;
   while(fscanf(file_name, "%d %d %d\n", &graph->from[i], &graph->to[i], &graph->value[i]) == 3 &&
         !feof(file_name)){

      if((i+1) == graph->capacity) {
         graph->capacity *= 2;
         graph->from = realloc(graph->from, graph->capacity * (sizeof(int)));
         graph->to = realloc(graph->to, graph->capacity * (sizeof(int)));
         graph->value = realloc(graph->value, graph->capacity * (sizeof(int)));
      }

      i++;
   }
   fclose(file_name);
   
   graph->value[++i] = GRAPH_END;
}

// - function -----------------------------------------------------------------
void load_bin(const char *fname, graph_t *graph) {
   FILE *file_name = fopen(fname, "rb");

   int i = 0;
   while(fread(&graph->from[i], sizeof(int), 1, file_name) &&
         fread(&graph->to[i], sizeof(int), 1, file_name)   &&
         fread(&graph->value[i], sizeof(int), 1, file_name)&&
         !feof(file_name))                                  {

      if((i+1) == graph->capacity) {
         graph->capacity *= 2;
         graph->from = realloc(graph->from, graph->capacity * (sizeof(int)));
         graph->to = realloc(graph->to, graph->capacity * (sizeof(int)));
         graph->value = realloc(graph->value, graph->capacity * (sizeof(int)));
      }
      i++;
   }
   fclose(file_name);
   graph->value[i] = GRAPH_END;
}

// - function -----------------------------------------------------------------
void save_txt(const graph_t * const graph, const char *fname) {
   FILE *file_name = fopen(fname, "w");
   int i = 0;
   while (graph->value[i] != GRAPH_END) {
      fprintf(file_name,"%d %d %d\n", graph->from[i], graph->to[i], graph->value[i]);
      i++;
   }
   fclose(file_name);
}

// - function -----------------------------------------------------------------
void save_bin(const graph_t * const graph, const char *fname) {
   FILE *file_name = fopen(fname, "wb");
   int i = 0;
   while (graph->value[i] != GRAPH_END) {
      fwrite(&graph->from[i], sizeof(int), 1, file_name); 
      fwrite(&graph->to[i], sizeof(int), 1, file_name); 
      fwrite(&graph->value[i], sizeof(int), 1, file_name); 
      i++;
   }
   fclose(file_name);
}
