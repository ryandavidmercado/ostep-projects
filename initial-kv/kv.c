#include <_stdio.h>
#include <_string.h>
#include <stdio.h>
#include <stdlib.h>

struct kv {
  char *key;
  char *value;
  struct kv *next;
};
typedef struct kv kv;

int kv_add(kv **kv_store, kv *kv_to_add) {
  if(!kv_to_add) { return 0; }
  if(!kv_store) { return 1; }
  if(!*kv_store) {
    *kv_store = kv_to_add;
    return 0;
  }

  kv *current = *kv_store;
  for(;;) {
    if(!strcmp(current->key, kv_to_add->key)) {
      current->value = kv_to_add->value;
      return 0;
    }

    if(!current->next) {
      current->next = kv_to_add;
      return 0;
    }

    current = current->next;
  }
}

kv *kv_make(char *key, char *value) {
  kv *ret = malloc(sizeof(kv));
  if(!ret) {
    perror("failed to allocate new kv entry: ");
    exit(1);
  }

  ret->key = key;
  ret->value = value;

  return ret;
}

kv *kv_read_db(FILE *stream) {
  kv *ret = nullptr;

  char *line = nullptr;
  size_t linecap = 0;

  char *key;
  char *value;

  while(getline(&line, &linecap, stream) > 0) {
    key = strsep(&line, ",");
    if(!key) {
      continue;
    }

    value = strsep(&line, "\n");
    if(!value) {
      continue;
    }

    kv *entry = kv_make(key, value);
    kv_add(&ret, entry);    
  }

  if(!feof(stream)) {
    perror("failed while reading database stream: ");
    exit(1);
  }

  return ret;
}

void kv_print(kv *to_print, bool print_all, FILE *stream) {
  while(to_print) {
    fprintf(stream, "%s,%s\n", to_print->key, to_print->value);
    if(!print_all) return;

    to_print = to_print->next;
  }
}

kv *kv_get(kv *kv_store, char *key) {
  while(kv_store) {
    if(!strcmp(key, kv_store->key)) {
      return kv_store;
    }

    kv_store = kv_store->next;
  }

  return nullptr;
}

int kv_delete(kv **kv_store, char *key) {
  kv *prev = nullptr;
  kv *current = *kv_store;

  if(!current) {
    return 1;
  }

  while(current) {
    if(!strcmp(current->key, key)) {
      if(prev) {
        prev->next = current->next;
        free(current);
        return 0;
      } else {
        *kv_store = current->next;
        free(current);
        return 0;
      }
    }

    prev = current;
    current = current->next;
  }

  return 1;
}

void kv_clear(kv **kv_store) {
  kv *current = *kv_store;
  if(!current) { return; }

  while(current) {
    kv *next = current->next;
    free(current);
    current = next;
  }

  *kv_store = nullptr;
}



int readCommandStr(char **command_str, char **command, char **key, char **value) {
  *command = strsep(command_str, ",");
  if(!(*command)) {
    return 1;
  }

  switch(*command[0]) {
    case 'a':
    case 'c':
      return 0; 
    case 'd':
    case 'g':
    case 'p': {
      *key = strsep(command_str, ",");
      if(!(*key)) {
        return 1;
      }

      if(*command[0] != 'p') {
        return 0;
      }

      *value = strsep(command_str, ",");
      if(!(*value)) {
        return 1;
      }

      return 0;
    } 
    default:
      return 1;
  }
}

int main(int argc, char *argv[argc]) {
  kv *kv_store = nullptr;
  FILE *db = fopen("database.txt", "r");
  if(db) {
    kv_store = kv_read_db(db);
    if(fclose(db)) {
      perror("Failed to close database.txt after read: ");
      return 1;
    };
  }

  for(size_t i = 1; i < argc; ++i) {
    char *commandString = argv[i];

    char *command;
    char *key;
    char *value;
    
    if(readCommandStr(&commandString, &command, &key, &value)) {
      continue;
    }

    switch(command[0]) {
      case 'p':
        kv *new_kv = kv_make(key, value);
        kv_add(&kv_store, new_kv);
        break;
      case 'g':
        kv *entry = kv_get(kv_store, key);
        if(!entry) {
          printf("%s not found\n", key);
          break;
        }

        kv_print(entry, false, stdout);
        break;
      case 'd':
        if(kv_delete(&kv_store, key)) {
          printf("%s not found\n", key);
        };
        break;
      case 'c':
        kv_clear(&kv_store);
        break;
      case 'a':
        kv_print(kv_store, true, stdout); 
        break;
      default:
        break;
    }
  }

  db = fopen("database.txt", "w");
  if(!db) {
    perror("Failed to read database.txt for write: ");
    return 1;
  }
  kv_print(kv_store, true, db);
  if(fclose(db)) {
    perror("Failed to close database.txt after write: ");
    return 1;
  }
}
