#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <stdlib.h>

#define N 10000
#define MAX_CHILDS 1000

typedef struct dirent dirent;
typedef struct Node {
  char name[20];
  __pid_t pid;
  __pid_t parent;
  __pid_t *childs;
  int child_num;
} Node;

Node *all_nodes;
int size;

int all_digit(char s[]) {
  for (int i = 0; s[i] != 0; i++) {
    if (!isdigit(s[i])) return 0;
  }
  return 1;
}

void get_info(Node* nodeptr, dirent *entry) {
  nodeptr->child_num = 0;
  char temp[100];
  FILE *fp = fopen(strcat(strcat(strcpy(temp, "/proc/"), entry->d_name), "/status"), "r");
  
  fscanf(fp, "Name:%s\n", nodeptr->name);
  int cnt = 4;
  while (cnt--) {
    fgets(temp, 99, fp);
  }
  fscanf(fp, "Pid:%d\n", &(nodeptr->pid));
  fscanf(fp, "PPid:%d\n", &(nodeptr->parent));
  fclose(fp);
}

void add_child(__pid_t ppid, __pid_t pid) {
  all_nodes[ppid].childs[all_nodes[ppid].child_num] = pid;
  all_nodes[ppid].child_num += 1;
  // if (all_nodes[ppid].child_num >= MAX_CHILDS) {
  //   for (int i = 0; i < all_nodes[ppid].child_num; i++) {
  //     printf("%d\n", all_nodes[ppid].childs[i]);
  //   }
  // }
  assert(all_nodes[ppid].child_num < MAX_CHILDS);
}

void print_tree(__pid_t pid, int depth) {
  int t = depth;
  while (t--) {
    printf("\t");
  }
  printf("%s(%d)\n", all_nodes[pid].name, all_nodes[pid].pid);
  for (int i = 0; i < all_nodes[pid].child_num; i++) {
    print_tree(all_nodes[pid].childs[i], depth + 1);
  }
}

void show(int version, int show_pids, int numeric_sort) {
  if (version) {
    printf("My pstree\nVersion 1.0\n");
  }
  if (show_pids || numeric_sort) {
    all_nodes = (Node*)malloc(sizeof(Node) * N);
    size = N;
    memset(all_nodes, 0, sizeof(Node*) * size);
    DIR *proc = opendir("/proc");
    assert(proc);
    dirent *entry;
    while ((entry = readdir(proc)) != NULL) {
      if (all_digit(entry->d_name) && entry->d_type == DT_DIR) {
        Node node;
        get_info(&node, entry);
        node.childs = (__pid_t*)malloc(sizeof(__pid_t) * MAX_CHILDS);
        while (node.pid >= size || node.parent >= size) {
          all_nodes = realloc(all_nodes, sizeof(Node) * (N + size));
          size += N;
        }
        all_nodes[node.pid] = node;
  
      }
    }
    // construct tree, childs are ensured to be in order this way.
    for (int i = 0; i < size; i++) {
      if (all_nodes[i].pid != 0 && all_nodes[i].parent != 1 && all_nodes[i].parent != 0) {
        add_child(all_nodes[i].parent, i);
      }
    }
    // print tree
    for (int i = 0; i < size; i++) {
      if (all_nodes[i].pid != 0 && all_nodes[i].parent == 1) {
        print_tree(i, 0);
      }
    }
  } 
}


int main(int argc, char *argv[]) {
  int show_pids = 0;
  int numeric_sort = 0;
  int version = 0;
  for (int i = 0; i < argc; i++) {
    assert(argv[i]);
    printf("argv[%d] = %s\n", i, argv[i]);
    
    if (strcmp(argv[i], "-p") == 0) {
      show_pids = 1;
    }
    else if (strcmp(argv[i], "-n") == 0) {
      numeric_sort = 1;
    }
    else if (strcmp(argv[i], "-V") == 0) version = 1;

  }
  show(version, show_pids, numeric_sort);
  assert(!argv[argc]);
  return 0;
}
