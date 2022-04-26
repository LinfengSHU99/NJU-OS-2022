#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <stdlib.h>
#include <

#define N 10000
#define MAX_CHILDS 50

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

void get_info(Node* nodeptr, dirent entry) {
  nodeptr->child_num = 0;
  FILE *fp = fopen(strcat(strcat("/proc/", entry.d_name), "/status"), "r");
  char temp[100];
  fscanf("Name:%s\n", nodeptr->name);
  int cnt = 4;
  while (cnt--) {
    fgets(temp, 99, fp);
  }
  fscanf(fp, "Pid:%d\n", &(nodeptr->pid));
  fscanf(fp, "PPid:%d\n", &(nodeptr->parent));
  fclose(fp);
}
// __pid_t get_pid(dirent entry) {
//   FILE *fp = fopen(strcat(strcat("/proc/", entry->d_name), "/status"), "r");
//   char temp[100];
//   int cnt = 5;
//   __pid_t pid = 0;
//   while (cnt--) {
//     fgets(temp, 99, fp);
//   }
//   fscanf(fp, "Pid:%d", &pid)
//   fclose(fp);
//   return pid;

// }

// __pid_t get_ppid(dirent entry) {
//   FILE *fp = fopen(strcat(strcat("/proc/", entry->d_name), "/status"), "r");
//   char temp[100];
//   int cnt = 6;
//   __pid_t pid = 0;
//   while (cnt--) {
//     fgets(temp, 99, fp);
//   }
//   fscanf(fp, "PPid:%d", &pid)
//   fclose(fp);
//   return pid;
// }

void add_child(__pid_t ppid, __pid_t pid) {
  all_nodes[ppid].childs[all_nodes[ppid].child_num] = pid;
  all_nodes[ppid].child_num++;
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

int show(int version, int show_pids, int numeric_sort) {
  if (version) {
    printf("My pstree\nVersion 1.0\n");
  }
  if (show_pids || numeric_sort) {
    all_nodes = (Node*)malloc(sizeof(Node) * N);
    size = N;
    memset(all_nodes, 0, sizeof(Node*) * size);
    DIR *proc = opendir("/proc");
    assert(proc);
    dirent entry;
    while ((entry = readdir(proc)) != NULL) {
      if (all_digit(entry.d_name) && entry.d_type == DT_DIR) {
        Node node;
        get_info(&node, entry);
        // __pid_t pid = get_pid(entry);
        // __pid_t ppid = get_ppid(entry);
        node.childs = (__pid_t*)malloc(sizeof(__pid_t) * MAX_CHILDS);
        while (node.pid >= size || node.parent >= size) {
          all_nodes = realloc(all_nodes, sizeof(Node) * (N + size));
          size += N;
        }
        all_nodes[node.pid] = node;
        // if (all_nodes[pid] == NULL) {
        //   Node node;
        //   node.pid = pid;
        //   node.parent = ppid;
        //   node.childs = (__pid_t*)malloc(sizeof(__pid_t) * MAX_CHILDS);
        //   all_nodes[pid] = node;
        // }
      }
    }
    // construct tree, childs are ensured to be in order this way.
    for (int i = 0; i < size; i++) {
      if (all_nodes[i].pid != 0 && all_nodes[i].parent != 1) {
        add_child(all_nodes[i].parent, i);
      }
    }
    // print tree
    for (int i = 0; i < size; i++) {
      if (all_nodes[i].pid != 0 && all_nodes[i].parent == 1) {
        print_tree(i, 1);
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
    
    if (strcmp(argv[i], "-p")) {
      show_pids = 1;
    }
    else if (strcmp(argv[i], "-n")) {
      numeric_sort = 1;
    }
    else if (strcmp(argv[i], "-V")) version = 1;

  }
  show(version, show_pids, numeric_sort);
  assert(!argv[argc]);
  return 0;
}
