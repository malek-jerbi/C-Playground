#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STARTING_BUCKETS 8
#define MAX_KEY_SIZE 32

typedef uint32_t Hash;

typedef struct Node {
  char* key;
  void* value;
  struct Node* next;
  Hash hash;
} Node;

typedef struct Hashmap {
  Node** buckets;
  int num_buckets;
} Hashmap;

Hash djb2(char* key) {
  Hash h = 0;
  char ch;
  while ( (ch = *key++)) {
    h += (h << 5) + h + ch;
  }
  return h;
}

Hashmap* Hashmap_new() {
   Hashmap* h = malloc(sizeof(Hashmap));
   h->buckets = calloc(STARTING_BUCKETS, sizeof(Node*));
   h->num_buckets = STARTING_BUCKETS;
   return h;
}

void Hashmap_free (Hashmap* h) {
  // TODO free all the nodes
  free(h->buckets);
  free(h);
}

void Hashmap_set(Hashmap* h, char* key, void* value) {
  Hash hash = djb2(key);
  Node* currNode = h->buckets[hash % h->num_buckets];
  while (currNode) {
    if (currNode->hash == hash && strncmp(key, currNode->key, MAX_KEY_SIZE) == 0) {
      currNode->value = value;
      return;
    }
    currNode = currNode->next;
  }
  // TODO handle collisions
  currNode = malloc(sizeof(Node));
  currNode->hash = hash;
  currNode->key = strdup(key);
  currNode->value = value;
  currNode->next = h->buckets[hash % h->num_buckets];
  h->buckets[hash % h->num_buckets] = currNode;
}

void* Hashmap_get(Hashmap* h, char* key) {
  // TODO Handle collision
  Hash hash = djb2(key);
  Node* node = h->buckets[hash % h->num_buckets];
  while (node) {
    if (node->hash == hash && strncmp(node->key, key, MAX_KEY_SIZE) == 0) {
      return node->value;
    }
    node = node->next;
  }
  return NULL;
}

void Hashmap_delete(Hashmap* h, char* key) {
  Hash hash = djb2(key);
  int i = hash % h->num_buckets;
  Node* n = h->buckets[i];
  if (!n){
    return;
  }

  Node* prevNode = NULL;
  while (n) {
    if (n->hash == hash && strncmp(n->key, key, MAX_KEY_SIZE) == 0) {
      break;
    }
    prevNode = n;
    n = n->next;
  }
  if (!n) {
    return;
  }
  if (!prevNode) {
    h->buckets[i] = n->next;
    free(n);
    return;
  }
  prevNode->next = n->next;
  free(n); // TODO only free if not null
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

  // handle collisions correctly
  // note: this doesn't necessarily test expansion
  int i, n = STARTING_BUCKETS * 10, ns[n];
  char key[MAX_KEY_SIZE];
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
  /*
     stretch goals:
     - expand the underlying array if we start to get a lot of collisions
     - support non-string keys
     - try different hash functions
     - switch from chaining to open addressing
     - use a sophisticated rehashing scheme to avoid clustered collisions
     - implement some features from Python dicts, such as reducing space use,
     maintaing key ordering etc. see https://www.youtube.com/watch?v=npw4s1QTmPg
     for ideas
     */
  printf("ok\n");
}
