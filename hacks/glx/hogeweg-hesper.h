/* hogeweg-hesper-strings.h 
 *grow L-system strings as decribed in Hogeweg and Hesper's July 1973 paper
 * titled A Model Study on Biomorphological Description, available at
 * http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.97.6640&rep=rep1&type=pdf
 *
 * by Kelley Nielsen
 * To eventually be used as part of a graphics hack
 */

#ifndef HOGEWEG_HESPER_H
#define HOGEWEG_HESPER_H

#define STRINGSIZE 4096
#define MAX_ITERATIONS 30

typedef struct branchnode {
  /* branch stored at index 0 is an axis branch,
   * all others are side branches
   * axis branches chain together to form elongated sections
   * it's assumed that 25 should be enough if iterations are
   * restricted to that number, expand if necessary
   */
  struct branchnode ** branches;
  int sidebranchcount;
  float spreadangle;
/*   float spiralangle; */
  /* eventually add fields for display information such as
   * color, visible flag, etc */
} branchnode;

/* ===== sets of generative rules ================== */
/* indices correspond to the three 1/0 chars, interpreted as a binary number,
 * in ascending order from left to right as listed in table 1.
 */

/* bushy -- are small in screen so shrink ortho viewport */
/* genrules3 is the one that has the progression shown in the paper */
static char *genrules3 [] = {"11", "0", "0", "0", "0", "1[1]", "1", "0"};
static char *genrules2b [] = {"11", "0", "0", "1", "0",  "1[1]", "0", "0"};
static char *genrules2c [] = {"11", "0", "0", "0", "0",  "1[1]", "0", "0"};

/* makes a rosette with floor(iterations/2) branches --uninteresting */
static char *genrules2a [] = {"11", "1", "0", "0", "0",  "1[1]", "0", "0"};

/* these will explode in size and cause a segfault */
/* corresponds to line 3 of table 1,  only rule 111 (7) is important */
/* don't use more than 9 iterations */
static char *genrules3_1 [] = {"0", "1", "1", "1", "1",  "1", "0", "1[1]"};
/* >= 11 for line 2,  makes an exponentially growing straight segment*/
static char *genrules2 [] = {"0", "1", "1", "1", "1",  "1", "0", "11"};

/* corresponds to line 4.3 */
static char *genrules4_3 [] = {"", "0", "", "0", "",  "1[1]", "0", "0"};

static char *genrules4 [] = {"0", "11", "0", "0",  "0",  "1[1]", "1", "0"};

/* the illustrations of fig. 5 represent first order branches */
static char *genrules5 [] = {"0", "11", "1", "0",  "0",  "1[1]", "0", "0"};

/* raise in screen, tree curls to below base */
static char *genrules6a [] = {"0", "0", "0", "11", "0",  "1[1]", "1", "0"};

/* this seems to be the only one that differs from the paper in any way */
static char *genrules6b [] = {"0", "11", "0", "0", "0",  "1[1]", "0", "0"};

static char *genrules7a [] = {"1", "0", "0", "11", "1",  "1[1]", "1", "0"};

static char *genrules7b [] = {"0", "0", "0", "11", "1",  "1[1]", "1", "0"};

/*  tall, don't use more than 21 iterations or will segfault*/
static char *genrules8 [] = {"0", "1[1]", "0", "0", "0", "11", "0", "0"};

static char *genrules9b [] = {"0", "1[1]", "1", "1", "0",  "11", "1", "0"};

static char *genrules10a [] = {"1", "1[1]", "1", "1", "0",  "11", "1", "0"};
static char *genrules10b [] = {"0", "1[1]", "1", "1", "0",  "11", "0", "0"};

static char *genrules11a [] = {"0", "1", "0", "1[1]", "0",  "11", "0", "0"};
static char *genrules11b [] = {"0", "1", "0", "1[1]", "0",  "11", "1", "0"};
static char *genrules11c [] = {"0", "0", "1", "1[1]", "1",  "11", "1", "0"};

/* ======================= */
#define GENRULESCOUNT 8
/* pass these as parameters */
#define GENRULES genrules3
#define ITERATIONS 15

/* #define SPIRALINC 180. */

/* start symbol and environment chars are both '1', processing is done with environment chars */
static char startsym [] = "111";

/* print the rule set and number of iterations */
void printheader(void);

/* make a pretty, readable string for the end result */
void printfinal(char * finalstring);

/* expand the L-system using the current generation rules, 
 * result: holds the generated string, should hold at least STRINGSIZE chars
 * interations: number of iterative passes to make,
 *                   must be between 1 and MAX_ITERATIONS
 *                   (higher values can cause overflow for some rule sets)
 * printintermediate: nonzero to print the generated string
 *                   after each iteration, 0 to suppress
 */
void generate(char * result, int iterations, int printintermediate);

/* convert a generated L-system string as returned by generate()
 * into a tree structure suitable for drawing.
 */
branchnode * mkhogewegtree(char * repstring);

/* print a quick and (really) dirty ascii graphical representation
 * of a generated L-system string to stdout.
 */
void printtree(branchnode * finaltree, int depth);

/* free the resources associated with the tree returned by mkhogewegtree() */
void frbranchnode(branchnode * doomed);

#endif
