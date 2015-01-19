/* hogeweg-hesper.c 
 *grow L-system strings as decribed in Hogeweg and Hesper's July 1973 paper
 * titled A Model Study on Biomorphological Description, available at
 * http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.97.6640&rep=rep1&type=pdf
 *
 * by Kelley Nielsen
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hogeweg-hesper.h"

#define DEBUG_STRING 0
#define DEBUG_TREE 0

/* == tree generation ==================================================== */

static branchnode * mkbranchnode(branchnode * parent){
  branchnode * new;
  int i;
  new = malloc(sizeof(struct branchnode));
  new->branches =(struct branchnode **)malloc(
		  MAX_ITERATIONS * sizeof(struct branchnode *));
  for (i = 0; i < MAX_ITERATIONS; i++)
  new->branches[i] = NULL;
  new->sidebranchcount = 0;
  new->spreadangle = 0.0;
  return new;
}

/* todo: finish, plus make free for entire tree */
void frbranchnode(branchnode * doomed){
  int i;
  if (doomed->sidebranchcount > 0){
  for (i = 1; i < doomed->sidebranchcount + 1; i++){
    frbranchnode(doomed->branches[i]);
  	}
  }
  if (doomed->branches[0] != NULL){
    frbranchnode(doomed->branches[0]);
  }
  free(doomed->branches);
  free(doomed);
}

/* assemble a tree data structure representing the pictoral representation
 * of a Hogeweg-Hesper L-system generated string
 */
branchnode * mkhogewegtree(char * repstring){
  branchnode * root;
  branchnode * current;
  float spiral = 0.;
  /* warning: there are currently no range checks done on the stack */
  branchnode * stack[MAX_ITERATIONS];
  int stackindex = 0;
  int repindex = 2;

  /* begin by adding the first branch, since
   * the first char in the representing string will always be '1' */
  root = mkbranchnode(NULL);
  current = root;
  while (repindex < strlen(repstring) - 1){
    switch(repstring[repindex]){
      /* add axis branch --note, we don't test for
       * multiple axis branches which should not occur */
    case '0':
    case '1':
      if (DEBUG_TREE) printf("Adding axis branch\n");
      current->branches[0] = mkbranchnode(current);
/*       current->branches[0]->spiralangle = spiral; */
/*       spiral += SPIRALINC; */
/*       if (spiral > 359.9) spiral -= 360.0; */
      current = current->branches[0];
      repindex++;
      break;
      /* add side branch -- a '1' always follows an opening bracket */
    case'[':
      if (current->sidebranchcount < MAX_ITERATIONS - 1){
      if (DEBUG_TREE) 
	printf("Adding side branch at array position %d\n", current->sidebranchcount + 1);
      current->branches[current->sidebranchcount + 1] = mkbranchnode(current);
/*       current->branches[current->sidebranchcount + 1]->spiralangle = spiral; */
/*       spiral += SPIRALINC; */
/*       if (spiral > 359.9) spiral -= 360.0; */
      stack[stackindex] = current;
      stackindex++;
      current->sidebranchcount++;
      current->spreadangle = 360.0 / current->sidebranchcount;
      current = current->branches[current->sidebranchcount];
      }
      repindex += 2;
      break;
      /* move one node back up the tree */
    case ']':
      stackindex--;
      current = stack[stackindex];
/*       spiral = current->spiralangle; */
      repindex++;
      break;
    default:
      break;
    }
  }
  return root;
}

/* provides a simple ascii check of the generated tree's correctness.
 * is useable, but ugly and spread out. Not particularly useful for large trees.
 * what this *should* do is print out the order and branch number of the tree,
 * like the gl rendering version
 */
void printtree(branchnode * finaltree, int depth){
  branchnode * base = finaltree;
  branchnode * traveling;
  char branchrep[] = "=====+";
  char padding[] = "     |";
  int i;

  /* print the representation for this branch node*/
  for (i = 0; i < depth; i++){
    printf("%s", padding);
  }

  /* follow to the end of the straight segment starting here, if it exists */
  traveling = base;
  while (traveling != NULL){
    traveling = traveling->branches[0];
    printf("%s", branchrep);
  }
  printf("\n");

  /* travel back down the axis, 
   * recursing on each side branch found.
   */
  traveling = base;
  while (traveling != NULL){
   for (i = 0; i < traveling->sidebranchcount; i++){
    printtree(traveling->branches[i + 1], depth + 1);
  }
  printf("\n");
    traveling = traveling->branches[0];
    depth++;
 }
}

/* == string generation ==================================================== */

void printheader(void){
  int i;
  printf("Rules: ");
  for (i = 0; i < GENRULESCOUNT; i++)
    {
      printf("%s.", GENRULES[i]);
    }
  printf(" Iterations: %d\n", ITERATIONS);
  /* possibly add warning about the off by one condition */
}

void printfinal(char * finalstring)
{
  int i;
  /* strip environment chars--start one after and end one short */
  for (i = 1; i < strlen(finalstring) - 1 ; i++){
    switch(finalstring[i]){
      /* both alphanumeric characters are drawn the same way */
    case '0':
    case'1':
      putchar('0');
      break;
      /* pass braces through */
    case'[':
      putchar('[');
      break;
    case']':
      putchar(']');
      break;
    default:
      break;
    }
  }
    putchar('\n');
}

/* assemble genrules index number from current chars and context */
static int resolve(char leftc, char currentc, char rightc){
  char left[2];
  char current[2];
  char right[2];
  int index;
  left[0] = leftc;
  current[0] = currentc;
  right[0] = rightc;
  left[1] = current[1] = right[1] = '\0';
  index = 4 * atoi(left) + 2 * atoi(current) + atoi(right);
  return index;
}

/* skip over pairs of braces and their contents (i.e. lower branches)
 * to find the left context for an alphanumeric character.
 * used by generate().
 */
static char skipbracesleft(char * searchstring, int startpoint){
     int  bracecount = 0;
     int found = 0;
     char target = '1';
     int searchpoint = startpoint;
      while ((searchpoint > -1) && (found == 0)){
	switch (searchstring[searchpoint]){
	case'[' :
	  bracecount++;
	  break;
	case ']':
	  bracecount--;
	  break;
	case '0':
	case '1':
	  if (bracecount == 0) {
	    target = searchstring[searchpoint];
	    found = 1;
	    break;
	  }
	default:
	  break;
		}
	searchpoint--;
	     }
      return target;
}

/* the function that does the bulk of the work */
void generate(char * result, int iterations, int printintermediate){
  /* grow if necessary */
  char thispass[STRINGSIZE];
  char lastpass[STRINGSIZE];
  char holdbrace[2];
  char leftc, currentc, rightc;
  int genrulesindex;
  int current = 1;
  int bracecount;
  int braceindex;
  int found;
  int i;

  if (iterations < 1) {
    strcpy(result, startsym);
    return;
  }
  strcpy(thispass, startsym);
  holdbrace[1] = '\0';

  for (i = 0; i < iterations; i++){
    strcpy(lastpass, thispass);
     strcpy(thispass, "1");
     current = 1;
   while (current < strlen(lastpass) - 1){
     if (DEBUG_STRING) printf("Parsing string position %d, pass %d--", current, i);
      currentc = lastpass[current];

      /* find left context */
       leftc = lastpass[current - 1];
       if (leftc == ']'){
	 /* skip backward over all pairs of braces and their contents,
	  * returning the first 0 or 1 character found.
	  */
	 leftc = skipbracesleft(lastpass, current - 1);
       }
       if (leftc == '['){
	 /* skip backward over this bracket, then over
	  * any pairs of brackets and their contents if they exist,
	  * assigning the first 0 or 1 character found.
	  */
	 leftc = skipbracesleft(lastpass, current - 2);
       }
       /* find right context */
      rightc = lastpass[current + 1];
      if (rightc == ']') rightc = '1';
      if (rightc == '[') {
	/* skip forward over all pairs of braces and their contents,
	 * assigning the first 0 or 1 char at the end of them.
	 * If we get to the EOS before one is found, assign '1'
	 */
      bracecount = 0;
      braceindex = current + 1;
      found = 0;
      while ((braceindex < strlen(lastpass)) && (found == 0)){
	switch (lastpass[braceindex]){
	case'[' :
	  bracecount++;
	  break;
	case ']':
	  /* see first comment at end of file */
	 /*  if (bracecount > 0)  */bracecount--;
	  break;
	case '0':
	case '1':
	  /* see third comment at end of file */
	  if( (bracecount == 0) && (lastpass[braceindex - 1] == ']') ) { 
/* 	    printf("Assigning %c to right context\n", lastpass[braceindex]); */
	    rightc = lastpass[braceindex];
	    found = 1;
	    break;
	  }
	default:
	  break;
		}
	braceindex++;
	     }
      /* as far as I can tell, this condition is never met,
       * but it's left in for safety */
      if ((found == 0) ) {
	rightc = '1';
      }
      }

      /* resolve alphanum by its production rule */
      if ((currentc == '0') || (currentc == '1')){
      genrulesindex = resolve(leftc, currentc, rightc);
      strcat(thispass, GENRULES[genrulesindex]);
      if (DEBUG_STRING) 
	printf("Resolving %c%c%c to %d (%s)\n", leftc, currentc, rightc, 
	       genrulesindex, GENRULES[genrulesindex]);
      }
      /* pass braces through */
      /* note: context isn't necessary for braces,
       * but that doesn't really matter for present purposes */
      else{
	holdbrace[0] = currentc;
	strcat(thispass, holdbrace);
	if (DEBUG_STRING) printf("Passing through %c\n", currentc);
      }
      current++;
    }
    strcat(thispass, "1");
    if (DEBUG_STRING) printf("Pass %d: %s\n", i + 1,  thispass);
    if (printintermediate != 0){
      printf("Pass %d: ", i + 1);
      printfinal(thispass);
    }
 }
  strcpy(result, thispass);
}


/* In the case of finding the right context when the rightmost char is a close brace:
 * The paper says "equal number of opening and closing brackets", but,
 * it probably should say "matched pairs of brackets".
 * If there are extra closing brackets after the char being evaluated,
 * they might be balanced with opening brackets from the next side branch,
 * and the found char will be misinterpreted as an axis branch.
 * (and, I think, also be interpreted as a side branch when it is found later.)
 * For instance, see this string,
 * generated during the 12th iteration of ruleset "11", "0", "0", "0", "0", "1[1]", "1", "0" (fig 3):
 *
 * 11[1][0][0[0]][11[0]][0[0]][0[0][1[1]][0[0]][1[1][0][0[0]]]]1
 *
 * The right context for the 1 in position 16 (the second 1 in a row not at the beginning)
 * is taken as the 0 in position 22 (just after a close brace)
 * when it should be the environment.
 * To prevent this from happening, I introduced the condition that the
 * brace count never drops below 0 (i.e. braces must always paired facing inward, never outward)
 * which is not mentioned in Hogeweg and Hesper's paper.
 */

/* actually, the tree *still* isn't right--it really shows on fig. 5 */
/* the string for ex. 5 is generated correctly up through 9 passes, verified by hand check
 * the visual tree agrees with the generated string but not with the picture
 * There are also some discrepancies between the visual tree and the picture 
 * with fig. 6a although it is mostly correct.
 */

/* a better way: rather than throw out odd closing braces as above,
 * don't take the candidate for the right context unless
 * the char to its left is a closing brace.
 * This fixes figure 5, but there are stil some problems.
 * Unsure what to do because it is still the case that the paper is ambiguous.
 */
 
/* possibly related: in skipbracesleft(), target has been initialized to '1'
 * to avoid a compiler warning
 * '1' was chosen because there should always be a '1' at the start of the string,
 * if nothing else, but in actuality if the condition in which target is assigned
 * is never entered then the generated string is malformed.
 */
