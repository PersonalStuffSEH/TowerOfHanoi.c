#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#ifndef DEFAULT_PLATES
#define DEFAULT_PLATES 8
#endif

#ifndef DEFAULT_OPTION
#define DEFAULT_OPTION "cli"
#define RESET "\e[0;0m"
#define REVIDEO "\e[7m"
#define HOME "\e[H"
#define WIDTH 10
// edge to edge for displaying the done moves
#endif

// technically a disk only needs value but i dont want to use an array because:
// i am a junior. i need to prove that i have skills
// and rod should only really have the start (for display) and end (for stack)
typedef struct disk {
  struct disk *next;
  uint8_t value;
} disk_t;
typedef struct rod {
   disk_t *head; // for display purposes
} rod_t;
typedef enum selected {LEFT, MIDDLE, RIGHT} selected_e;

////////////////////////////////////////////////////////////////////////////////
// stack related stuff
void *getTail(disk_t *const head) {
  static disk_t *iterator = NULL;
  for (iterator = head; iterator->next; iterator = iterator->next) {
  }
  return iterator;
}
void push(disk_t *const head, disk_t *const new) {
  static disk_t *dest = NULL;
  dest = getTail(head);
  dest->next = new;
}
// stores the the .next from the penultimate and returns it (penult.next = NULL)
// this is a basic stack, no need for allocations
// (according to: https://youtu.be/QpAhX-gsHMs?t=2303 )
void *pop(disk_t *const head) {
  static disk_t *iterator = NULL;
  static disk_t *last = NULL;
  if (!head) {
    return NULL;
  }
  if (!head->next) {
    return iterator;
  }  
  for (iterator = head; iterator->next->next; iterator = iterator->next) {
  }
  last = iterator->next;  
  iterator->next = NULL;
  return last;
}
// for debugging purposes
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// cli related stuff
void printRod(disk_t *const head) {
  static disk_t *iter;
  for (iter = head; iter; iter = iter->next) {
    printf("%hd", iter->value);
  }
}
// gets the cli sprite from an integer value
// reason why is because switch statements are hash tables with nearly constant lookups
void *getSpriteCli(uint8_t value) {
  switch (value) {
    // a config file wouldve been cleaner admittedly but the goal of this is
    // tower of hanoi, not a file parser (different project)
  case 1:  return "①"; // i dont need break because return is a jump
  case 2:  return "⓶";
  case 3:  return "⓷";
  case 4:  return "⓸";
  case 5:  return "⓹";
  case 6:  return "⓺";
  case 7:  return "⓻";
  case 8:  return "⓼";
  default: return " ";
  }
}
void renderCli(disk_t *const headL, disk_t *const headM, disk_t *const headR,
               const uint8_t movesDone) {
  static disk_t *iterL = NULL,   *iterM = NULL,   *iterR = NULL;
  static char   *spriteL = NULL, *spriteM = NULL, *spriteR = NULL;
  iterL = headL;                 iterM = headM;   iterR = headR;

  printf("%s╔═══╦═══╦═══╗\n║   ║   ║   ║\n", HOME);
  do {
    // print the sprite or if there are sprites on this line print a bar, else a space
    spriteL = iterL ? getSpriteCli(iterL->value) : NULL;
    spriteM = iterM ? getSpriteCli(iterM->value) : NULL;
    spriteR = iterR ? getSpriteCli(iterR->value) : NULL;
    spriteL = (spriteL || spriteM || spriteR) ? (spriteL ? spriteL : "│") : " ";
    spriteM = (spriteL || spriteM || spriteR) ? (spriteM ? spriteM : "│") : " ";
    spriteR = (spriteL || spriteM || spriteR) ? (spriteR ? spriteR : "│") : " ";

    // reverses the colours for the selected index
    printf("║%s %s ", spriteL, RESET);
    printf("║%s %s ", spriteM, RESET);
    printf("║%s %s ║\n", spriteR, RESET);
    iterL = iterL ? iterL->next : NULL;
    iterM = iterM ? iterM->next : NULL;
    iterR = iterR ? iterR->next : NULL;
  } while (iterL || iterM || iterR);
  printf("╠═╧═╩═╧═╩═╧═╣\n");
  printf("║");
  uint8_t padding = log10(movesDone);
  for (uint8_t empty = 0; empty < WIDTH - padding; printf(" "), ++empty) {}
  printf("%d║\n", movesDone);
  printf("╚═══════════╝\n");
}
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]) {
  /*
  if (argc < 2) {
    printf("usage: ./towerofhanoi <plates> ?cli|gui ?...<colours>\n");
    printf("using default args of: %d and %s", DEFAULT_PLATES, DEFAULT_OPTION);
  }
  */
  rod_t left = {.head = calloc(1, sizeof(*left.head))};
  rod_t middle = {.head = calloc(1, sizeof(*middle.head))};
  rod_t right = {.head=calloc(1, sizeof(*right.head))};
  for (uint8_t value = DEFAULT_PLATES; value > 1; --value) {
    disk_t disk = {.next = NULL, .value = value};
    push(left.head, &disk);
    }    

  return 0;
  /*
  uint8_t left[DEFAULT_PLATES] = {8,7,6,5,4,3,2,1};
  uint8_t leftIdx = DEFAULT_PLATES - 1;
  uint8_t middle[DEFAULT_PLATES] = {0};
  uint8_t middleIdx = 0;
  uint8_t right[DEFAULT_PLATES] = {0};
  uint8_t rightIdx = 0;

  uint8_t (*startArr)[];
  uint8_t *startIdx;
  uint8_t (*endArr)[];
  uint8_t *endIdx;

  uint16_t start;
  uint16_t end;
  do {
    for (uint16_t l = 0; l < DEFAULT_PLATES; ++l) {
      if (l > leftIdx) {
	printf("0");
      } else {
	printf("%d", left[l]);
      }        
    }
    printf("\n");
    for (uint16_t m = 0; m < DEFAULT_PLATES; ++m) {
      if (m > middleIdx) {
	printf("0");
      } else {
	printf("%d", middle[m]);
      }        
    }
    printf("\n");
    for (uint16_t r = 0; r < DEFAULT_PLATES; ++r) {
      if (r > rightIdx) {
	printf("0");
      } else {
	printf("%d", right[r]);
      }        
    }
    printf("\n");
    scanf("%hd", &start);
    scanf("%hd", &end);
    switch (start) {
      case 0:
	startArr = &left;
	startIdx = &leftIdx;
	break;
      case 1:
	startArr = &middle;
	startIdx= &middleIdx;
	break;
      case 2:
	startArr = &right;
	startIdx = &rightIdx;
	break;
      case 'q':
        return 0;        
    }
    switch (end) {
      case 0:
	endArr = &left;
	endIdx = &leftIdx;
	break;
      case 1:
	endArr = &middle;
	endIdx= &middleIdx;
	break;
      case 2:
	endArr = &right;
	endIdx = &rightIdx;
	break;
    }
    uint8_t effStartIdx= (*startIdx) - ((*startArr)[(*startIdx)] == 0);
    (*endArr)[(*endIdx)] = (*startArr)[effStartIdx];
    (*startArr)[(*startIdx)--] = 0;
    (*endIdx)++;
  } while (left[0] != 0);    
  
  return 0;
  */
}  
