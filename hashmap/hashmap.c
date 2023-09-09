#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STARTING_BUCKETS 8

typedef struct Node {
  char* key;
  void* value;
  struct Node* next;
} Node;

Node* Node_new (char* key, void* value) {
  Node* node = malloc(sizeof(Node));
  node->key = key;
  node->value = value;
  Node* next_node = NULL;
  node->next = next_node;
  return node;
}

typedef struct Hashmap {
  Node** buckets;
  int number_of_buckets;
} Hashmap;

Hashmap* Hashmap_new(){
  Hashmap* hm = malloc(sizeof(Hashmap));
  hm->number_of_buckets = STARTING_BUCKETS;
  hm->buckets = malloc(hm->number_of_buckets * sizeof(void*));
  for (int i= 0; i < hm->number_of_buckets; i++) {
    hm->buckets[i] = NULL;
  }
  return hm;
}
// hash the key
int hash_key (char* key) {
  int length_of_key = 0;
  while (*(key + length_of_key)) {
    length_of_key++;
  }
  int hashed_key = length_of_key % STARTING_BUCKETS;
  return hashed_key;
}

void Hashmap_set(Hashmap* h, char* key, void* value) {
  // hash the key
  int bucket_index = hash_key(key);

  Node* created_node = Node_new(key, value);
    
  Node** current_node = &(h->buckets[bucket_index]);

  // if its the first one, ez
  if (*current_node == NULL) {
    *current_node = created_node;
  }
  
  else {
    // keep iterating on next until we hit a node that doesnt have a next
    while ((**current_node).next != NULL) {
      //if key already exists, just set its value to the new value
      if (strcmp((**current_node).key , key) == 0) { // using strcomp, to avoid doing the string comparison manually again (did it in Hashmap_get)
        (**current_node).value = value;
        return;
      }
      current_node = &(**current_node).next;
    }
    // our current_node now does not have a next
    (**current_node).next = created_node;
  }
}



void* Hashmap_get(Hashmap* h, char* key) {
  // get index
  int bucket_index = hash_key(key);
  // get the node at that index
  Node* node = *(h->buckets+bucket_index);

  // iterate through all nodes of the bucket
  while (node != NULL) {
    int character_index = 0;
    while(1) {
      // get i'th character of the existing node
      char existing_key_char = *((*node).key + character_index);
      // if i'th character of existing node == i'th character of received key, index++ and continue
      if (existing_key_char == *(key+character_index) ) {
        if (existing_key_char == '\0') {
          // we reached the end of both strings, so they are equal
          void* pointer =  (*node).value;
          printf("%p\n", pointer);
          return pointer;
        }
        character_index++;
      }
      else break;
    }

    // reaching this line means we need to check the next node
    node = (*node).next;
  }
  return NULL;
}

void Hashmap_delete(Hashmap* h, char* key) {
  int bucket_index = hash_key(key);
  Node* node = *(h->buckets+bucket_index);
  if (node == NULL) return;
  else if (strcmp((*node).key , key) == 0) {
    Node* next_node = (*node).next;
    (*(h->buckets+bucket_index)) = next_node;
    //TODO : deallocate
  }
  else {
    while (node != NULL) {
      Node* next_node = (*node).next;
      if (strcmp((*next_node).key , key) == 0) {
        (*node).next = &next_node;
        break;
      }
      node = (*node).next;
    }
  }

}

int main() {
  Hashmap *h = Hashmap_new();

  // basic get/set functionality
  int a = 5;
  float b = 7.2;
  Hashmap_set(h, "item a", &a);
  Hashmap_set(h, "item b", &b);

  assert(Hashmap_get(h, "item a") == &a);
  assert(Hashmap_get(h, "item b") == &b);

  // using the same key should override the previous value
  int c = 20;
  Hashmap_set(h, "item a", &c);
  assert(Hashmap_get(h, "item a") == &c);
  

  // basic delete functionality
  Hashmap_delete(h, "item a");
  assert(Hashmap_get(h, "item a") == NULL);

  printf("here : GOOD");

  // handle collisions correctly
  // note: this doesn't necessarily test expansion
  int i, n = STARTING_BUCKETS * 10, ns[n];
  char key[STARTING_BUCKETS];
  for (i = 0; i < n; i++) {
    ns[i] = i;
    sprintf(key, "item %d", i);
    Hashmap_set(h, key, &ns[i]);
  }
  for (i = 0; i < n; i++) {
    sprintf(key, "item %d", i);
    assert(Hashmap_get(h, key) == &ns[i]);
  }

  // Hashmap_free(h);
  // /*
  //    stretch goals:
  //    - expand the underlying array if we start to get a lot of collisions
  //    - support non-string keys
  //    - try different hash functions
  //    - switch from chaining to open addressing
  //    - use a sophisticated rehashing scheme to avoid clustered collisions
  //    - implement some features from Python dicts, such as reducing space use,
  //    maintaing key ordering etc. see https://www.youtube.com/watch?v=npw4s1QTmPg
  //    for ideas
  //    */
  // printf("ok\n");
}
