#include <_string.h>
#include <stdio.h>
#include <stdlib.h>

struct kv {
  char *key;
  char *value;
  struct kv *next;
};
typedef struct kv kv;

kv *kv_make(char *key, char *value) {
  kv *ret = malloc(sizeof(kv));
  ret->key = key;
  ret->value = value;

  return ret;
}

void kv_print(kv *to_print) {
  while(to_print) {
    printf("%s,%s\n", to_print->key, to_print->value);
    to_print = to_print->next;
  }
}

int kv_add(kv **kv_store, kv *kv_to_add) {
  if(!kv_to_add) { return 0; }
  if(!kv_store) { return 1; }
  if(!*kv_store) {
    *kv_store = kv_to_add;
    return 0;
  }

  kv *current = *kv_store;
  for(;;) {
    if(current->key == kv_to_add->key) {
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
      default:
        break;
    }
  }

  kv_print(kv_store);
}
