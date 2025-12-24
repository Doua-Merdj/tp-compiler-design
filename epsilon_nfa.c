#include <stdio.h> 
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 20
#define MAX_SYMBOLS 10

// We use symbol index MAX_SYMBOLS to represent epsilon

int n_states, n_symbols; 
char symbols[MAX_SYMBOLS];
int start_state;
int is_final[MAX_STATES];

// transitions[state][symbol_index][dest_state] 
int transitions[MAX_STATES][MAX_SYMBOLS + 1][MAX_STATES];

// epsilon-closure of each state
 int eclosure[MAX_STATES][MAX_STATES];

// new transitions without epsilon
 int new_transitions[MAX_STATES][MAX_SYMBOLS][MAX_STATES];
 int new_final[MAX_STATES];

/*  Task 1: Read the NFA */ 
void readNFA() { 
 int i, j, k; 
 printf("Number of states: ");
 scanf("%d", &n_states);

printf("Number of input symbols: ");
scanf("%d", &n_symbols);

printf("Enter symbols: ");
for (i = 0; i < n_symbols; i++)
    scanf(" %c", &symbols[i]);

printf("Start state: ");
scanf("%d", &start_state);

printf("Final states (0/1 for each state):\n");
for (i = 0; i < n_states; i++)
    scanf("%d", &is_final[i]);

memset(transitions, 0, sizeof(transitions));

printf("Enter transitions (from to symbol), use e for epsilon, -1 to stop:\n");
while (1) {
    int from, to;
    char sym;
    scanf("%d", &from);
    if (from == -1) break;
    scanf("%d %c", &to, &sym);

    if (sym == 'e') {
        transitions[from][MAX_SYMBOLS][to] = 1;
    } else {
        for (k = 0; k < n_symbols; k++) {
            if (symbols[k] == sym) {
                transitions[from][k][to] = 1;
                break;
            }
        }
    }
}

}

/*  Task 2: Compute epsilon-closure  */ 
void computeEclosure() { 
    int i, j, k;

for (i = 0; i < n_states; i++) {
    memset(eclosure[i], 0, sizeof(eclosure[i]));
    eclosure[i][i] = 1;

    int stack[MAX_STATES], top = 0;
    stack[top++] = i;

    while (top > 0) {
        int s = stack[--top];
        for (j = 0; j < n_states; j++) {
            if (transitions[s][MAX_SYMBOLS][j] && !eclosure[i][j]) {
                eclosure[i][j] = 1;
                stack[top++] = j;
            }
        }
    }
}

}

/*  Task 3: Compute new transitions without epsilon  */ 
void computeNewTransitions() {
    int i, j, k, p, q;
     memset(new_transitions, 0, sizeof(new_transitions));

for (i = 0; i < n_states; i++) {
    for (k = 0; k < n_symbols; k++) {
        for (p = 0; p < n_states; p++) {
            if (eclosure[i][p]) {
                for (q = 0; q < n_states; q++) {
                    if (transitions[p][k][q]) {
                        int r;
                        for (r = 0; r < n_states; r++) {
                            if (eclosure[q][r])
                                new_transitions[i][k][r] = 1;
                        }
                    }
                }
            }
        }
    }
}

}

/* Task 4: Determine new final states */ 
 void computeNewFinalStates() {
     int i, j;
     memset(new_final, 0, sizeof(new_final));

for (i = 0; i < n_states; i++) {
    for (j = 0; j < n_states; j++) {
        if (eclosure[i][j] && is_final[j]) {
            new_final[i] = 1;
            break;
        }
    }
}

}

/* Task 5: Display the new automaton */
 void displayNFA() {
     int i, j, k;
     printf("\nNFA without epsilon transitions:\n"); 
     printf("States: %d\n", n_states);
     printf("Start state: %d\n", start_state);

printf("Final states: ");
for (i = 0; i < n_states; i++)
    if (new_final[i]) printf("%d ", i);
printf("\n\nTransitions:\n");

for (i = 0; i < n_states; i++) {
    for (k = 0; k < n_symbols; k++) {
        for (j = 0; j < n_states; j++) {
            if (new_transitions[i][k][j])
                printf("%d --%c--> %d\n", i, symbols[k], j);
        }
    }
}

}

int main() { 
    readNFA();
    computeEclosure();
     computeNewTransitions();
     computeNewFinalStates();
     displayNFA();
     return 0;
 }
