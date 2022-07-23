//------------------------------------------
//CP 386
//Name： MengyuanLiu ; Huiqiao Yang
//Student Id: 186802750 ; 186803710
//------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
typedef struct f_node {
    int size;
    int start;
    int end;
    int number_size;
    struct f_node *next;
} FREE;

typedef struct allo_node {
    int size;
    char *p_name;
    unsigned char *memory;
    int start;
    int end;
    struct allo_node *next;
} ALLOCATED;

void allocate_memory(FREE **f_mem, ALLOCATED **allo_mem, char alg, char *p_name, int size);
void free_memory(FREE **f_mem, ALLOCATED **allo_mem, char *p_name);

void free_insert(FREE **f_mem, FREE *node){
    if(*f_mem){
        FREE *ptr = *f_mem;
        if(ptr->start > node->start){
            node->next = ptr;
            *f_mem = node;
        } else {
            while(ptr->next){
                if(node->start < ptr->next->start){
                    node->next = ptr->next;
                    ptr->next = node;
                    return;
                } else {
                    ptr = ptr->next;
                }
            }
            if(ptr->next == NULL){
                ptr->next = node;
            }
        }
    } else {
        *f_mem = node;
    }
}

void alloc_insert(ALLOCATED **allo_mem, ALLOCATED *node){
    if(*allo_mem){
        ALLOCATED *ptr = *allo_mem;
        if(ptr->start > node->start){
            node->next = ptr;
            *allo_mem = node;
        } else {
            while(ptr->next){
                if(node->start < ptr->next->start){
                    node->next = ptr->next;
                    ptr->next = node;
                    return;
                } else {
                    ptr = ptr->next;
                }
            }
            if(ptr->next == NULL){
                ptr->next = node;
            }
        }
    } else {
        *allo_mem = node;
    }
}

unsigned char* memory;
int t_memory;
int m_memory = 0;;

int main(int argc, char *argv[]) {
    FREE *f_mem = NULL;
    ALLOCATED *allo_mem = NULL;
    char command[20];
    t_memory = 1048576;
    memory = (char*)malloc(t_memory*sizeof(char));

    f_mem = (FREE*)malloc(sizeof(FREE));
    f_mem->size = t_memory;
    f_mem->start = 0;
    f_mem->end = t_memory-1;
    f_mem->next = NULL;
    printf("Allocated %d bytes of memory\n", t_memory);
        printf("allocator>");
    while(scanf("%[^\n]%*c", command)) {
        if (strcmp(command,"Exit")==0){
            free(memory);
            exit(0);
        }
        if (strcmp(command,"Status")==0) {
            printf("Paritions [Allocated memory = %d]:\n", m_memory);
            ALLOCATED *ptr = allo_mem;
            while(ptr){
                printf("Address [%d:%d] Process %s\n", ptr->start, ptr->end, ptr->p_name);
                ptr = ptr->next;
            }
            printf("\nHoles [Free memory = %d]:\n", t_memory - m_memory);
            FREE *fptr = f_mem;
            while(fptr){
                printf("Address [%d:%d] len = %d\n", fptr->start, fptr->end, fptr->size);
                fptr = fptr->next;
            }
        }
        else {
            char *token = strtok(command," ");
            if (strcmp(token,"RQ")==0) {
                char *processNum = (char*)malloc(4*sizeof(char));
                int processSize;
                token = strtok(NULL, " ");
                strcpy(processNum,token);
                token = strtok(NULL, " ");
                processSize = atoi(token);
                token = strtok(NULL, " ");
                char type = *token;
                allocate_memory(&f_mem, &allo_mem, type, processNum, processSize);
            }
            else if (strcmp(token,"RL")==0) {
                char *processNum = (char*)malloc(4*sizeof(char));
                token = strtok(NULL, " ");
                strcpy(processNum,token);
                free_memory(&f_mem, &allo_mem, processNum);
            }
            else if (strcmp(token,"C")==0) {
                ALLOCATED *cur = allo_mem;
                ALLOCATED *prev = NULL;
                while(cur){
                    if(prev && prev->end != cur->start-1){
                        cur->start = prev->end+1;
                        cur->end = cur->start + cur->size - 1;
                        cur->memory = memory + cur->start;
                    } else if(prev == NULL && cur->start != 0){
                        cur->start = 0;
                        cur->end = cur->size - 1;
                        cur->memory = memory;
                    }
                    prev = cur;
                    cur = cur->next;
                }
                FREE *cur_free = f_mem;
                FREE *prev_free = NULL;
                f_mem = (FREE*)malloc(sizeof(FREE));
                f_mem->size = t_memory - m_memory;
                f_mem->start = m_memory;
                f_mem->end = m_memory + f_mem->size - 1;
                f_mem->next = NULL;
                while(cur_free){
                    prev_free = cur_free;
                    cur_free = cur_free->next;
                    free(prev_free);
                }
                printf("Compaction process is successful\n");
            }
        }
        printf("allocator>");
    }
    return 0;
}

void clean(FREE **f_mem){
    FREE *cur = *f_mem;
    FREE *prev = NULL;
    while(cur){
        if(cur->size == 0){
            if(prev){
                prev->next = cur->next;
            } else {
                *f_mem = cur->next;
            }
        }
        if(prev){
            if(prev->end == cur->start - 1){
                prev->end = cur->end;
                prev->size += cur->size;
                prev->next = cur->next;
                free(cur);
                cur = prev->next;
            }
        }
        prev = cur;
        cur = cur->next;
    }
}

int first_fit_try(FREE **f_mem, int size){
    FREE *ptr = *f_mem;
    int start;
    if(ptr){
        while(ptr){
            if(ptr->size >= size){
                start = ptr->start;
                ptr->start += size;
                ptr->size -= size;
                return start;
            }
            ptr = ptr->next;
        }
    }
    return -1;
}

int best_fit_try(FREE **f_mem, int size){
    int start;
    int i =0;
    FREE *ptr = *f_mem;
    FREE *tep = NULL;
    if(ptr){
        int min = 1e9;
        while(ptr){
            if(ptr->size >= size && ptr->size < min){
                min = ptr->size;
                start = ptr->start;
                tep = ptr;
            }
            ptr = ptr->next;
            i++;
        }
        if(tep){
            tep->start += size;
            tep->size -= size;
        } else {
            start = -1;
        }
        return start;
    }
    return -1;
}

int worst_fit_try(FREE **f_mem, int size){
    int start;
    FREE *ptr = *f_mem;
    FREE *tep = NULL;
    if(ptr){
        int max = -1e9;
        while(ptr){
            if(ptr->size >= size && ptr->size > max){
                max = ptr->size;
                start = ptr->start;
                tep = ptr;
                ptr = ptr->next;
            }
            ptr = ptr->next;
        }
        if(tep){
            tep->start += size;
            tep->size -= size;
        } else {
            start = -1;
        }
        return start;
    }
    return -1;
}

void allocate_memory(FREE **f_mem, ALLOCATED **allo_mem, char alg, char *p_name, int size){
    int emp;
    if(alg == 'F'){
        emp = first_fit_try(f_mem, size);
    } else if(alg == 'B'){
        emp = best_fit_try(f_mem, size);
    } else if(alg == 'W'){
        emp = worst_fit_try(f_mem, size);
    } else {
        printf("Invalid fit algorithm\n");
        return;
    }
    if(emp == -1){
        printf("No hole of sufficient size\n");
        return;
    } else {
        ALLOCATED *n_node = (ALLOCATED*)malloc(sizeof(ALLOCATED));
        n_node->start = emp;
        n_node->end = emp + size - 1;
        n_node->size = size;
        n_node->memory = memory + emp;
        n_node->p_name = p_name;
        n_node->next = NULL;
        alloc_insert(allo_mem, n_node);
        clean(f_mem);
        m_memory+=size;
        printf("Successfully allocated %d to process %s\n", size, p_name);
    }
}

void free_memory(FREE **f_mem, ALLOCATED **allo_mem, char *p_name){
    ALLOCATED *cur = *allo_mem;
    ALLOCATED *prev = NULL;
    while(cur){
        if(strcmp(cur->p_name, p_name)==0){
            break;
        } else {
            prev = cur;
            cur = cur->next;
        }
    }
    if(cur == NULL){
        printf("There is no process of name: %s", p_name);
        return;
    } else {
        int f_size;
        int start;
        f_size = cur->size;
        start= cur->start;
        if(prev){
            prev->next = cur->next;
        } else {
            *allo_mem = cur->next;
        }
        free(cur);

        FREE *n_node = (FREE*)malloc(sizeof(FREE));
        n_node->size = f_size;
        n_node->start = start;
        n_node->end = start + f_size - 1;
        free_insert(f_mem, n_node);
        clean(f_mem);
        m_memory -= f_size;
        printf("releasing memory for process %s\n",p_name);
    }
}
