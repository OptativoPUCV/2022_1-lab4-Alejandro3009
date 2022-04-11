#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
    long idx = hash(key,map->capacity);
    while (map->buckets[idx] != NULL && map->buckets[idx]->key != NULL)
    {
        if(is_equal(key,map->buckets[idx]->key) == 1)return;
        idx++;
        if(idx == map->capacity)idx = 0;
    }
    if (map->buckets[idx] != NULL){
        map->buckets[idx]->key = key;
        map->buckets[idx]->value = value;
    }
    else map->buckets[idx] = createPair(key,value);
    map->size += 1;
}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)

    Pair **oldBuckets = map->buckets;
    long i;
    long bigC = map->capacity * 2;
    free(map);
    map = createMap(bigC);
    for(i = 0; i<bigC/2; i++){
        if (oldBuckets[i] != NULL){
            insertMap(map,oldBuckets[i]->key,oldBuckets[i]->value);
        }
    }
}


HashMap * createMap(long capacity) {
  HashMap *map = (HashMap *) malloc (sizeof(HashMap));
  map->buckets = (Pair**) calloc (capacity,sizeof(Pair *));
  map->capacity = capacity;
  map->size = 0;
  map->current = -1;
  return map;
}

void eraseMap(HashMap * map,  char * key) {   
    long idx = hash(key,map->capacity);
    int esta = 0;
    while (map->buckets[idx] != NULL){
        if(is_equal(key,map->buckets[idx]->key) == 1)
        {
            esta = 1;
            break;
        }
        idx++;
    }

    if(esta != 1)return;
    else 
    {
        map->buckets[idx]->key = NULL;
        map->size -= 1;
    }
}

Pair * searchMap(HashMap * map,  char * key) {   
    long idx = hash(key,map->capacity);
    while (map->buckets[idx] != NULL){
        if(map->buckets[idx] == NULL)return NULL;
        if(is_equal(key,map->buckets[idx]->key) == 1)break;
        idx = ((idx + 1)%map->capacity);
    }

    map->current = idx;
    return map->buckets[idx];
}

Pair * firstMap(HashMap * map) {
    long i;

    for(i = 0; i<map->capacity; i++)
    {
        if(map->buckets[i] != NULL && map->buckets[i]->key != NULL)break;
    }

    map->current = i;
    return map->buckets[i];
}

Pair * nextMap(HashMap * map) {
    long i;

    for(i = map->current + 1; i<map->capacity; i++)
    {
        if(map->buckets[i] != NULL && map->buckets[i]->key != NULL)break;
        if((i+1)==map->capacity)return NULL;
    }
    
    map->current = i;
    return map->buckets[i];
}
