/* Parser for Thorleif Bundgaard's soma solution files.
 * Explanation of the solution format is given at *** 
 * Only works for one set figures,
 * double set and partial set figures are not supported.
 * does not check for malformed solutions or files.
 *
 * Directions are reckoned thusly:
 * across the token is x, tokens across the line are y,
 * lines down the file are z.
 *
 * Due to the way the tokens naturally load into the figurelayout array,
 * x and z are mixed up in piececube.position--
 * length (x coordinate) is stored in index 2 and
 * width (z coordinate) is stored in index 0.
 * Also, the y axis is inverted--
 * due to the order in which cube positions are
 * presented in the solution file format,
 * y position increases downward.
 */



#include <stdio.h>   
#include <stdlib.h> 
#include <ctype.h>  
#include <string.h> 
#include <X11/Xlib.h>

enum connections_t{right, down, ahead, left, up, behind};

typedef struct{
  float position[3];
  enum connections_t adjoins[3];
  int num_adjoins;
} piececube;


static char figurelayout[32][32][32];
static int figlength, figdepth, figwidth;
FILE * emitfile;
int (*ofinders[8])(piececube *, int) = {NULL};



/* Helper for findrefcube() and orientationfinder*().
 * determines if a cube in a piece
 * is connected to another cube in the piece
 * in a certain direction.
 * accepts a _single_ piececube and the adjoin to search for.
 */
Bool contains(piececube * cube, enum connections_t adjoiner )
{
  Bool does_it_contain = False;
  int iterator;
  for (iterator = 0; iterator < cube->num_adjoins; iterator++)
    {
      if (cube->adjoins[iterator] == adjoiner)
	{
          does_it_contain = True;
          break;
        }
    }
  return does_it_contain;
}

/* -------------------------------------------------------- */

/* various orientation finders, one for each piece.
 * @@@ describe file where the orientations are defined.
 */

int orientfinder1(piececube * ocube, int refcube)
{
  int porientation;

  if(contains(&ocube[refcube], right))
    {
      if(contains(&ocube[refcube], down)) porientation = 15;
      if(contains(&ocube[refcube], ahead)) porientation = 7;
      if(contains(&ocube[refcube], up)) porientation = 6;
      if(contains(&ocube[refcube], behind)) porientation = 13;
    }

  if(contains(&ocube[refcube], down))
    {
      if(contains(&ocube[refcube], ahead)) porientation = 3;
      if(contains(&ocube[refcube], behind)) porientation = 2;
    }
  
  if(contains(&ocube[refcube], left))
    {
      if(contains(&ocube[refcube], down)) porientation = 14;
      if(contains(&ocube[refcube], ahead)) porientation = 9;
      if(contains(&ocube[refcube], up)) porientation = 4;
      if(contains(&ocube[refcube], behind)) porientation = 11;
    }
  
  if(contains(&ocube[refcube], up))
    {
      if(contains(&ocube[refcube], ahead)) porientation = 0;
      if(contains(&ocube[refcube], behind)) porientation = 1;
    }
  
  return porientation;
}

int orientfinder2(piececube * ocube, int refcube)
{
  int porientation;
  porientation = orientfinder1(ocube, refcube);
  if (refcube == 1)
    {
      if (porientation == 1) porientation = 5;
      if (porientation == 2) porientation = 12;
      if (porientation == 4) porientation = 19;
      if (porientation == 14) porientation = 20;
      if (porientation == 13) porientation = 21;
    }
  if (refcube == 2)
    {
      if (porientation == 0) porientation = 10;
      if (porientation == 6) porientation = 16;
      if (porientation == 9) porientation = 18;
      if (porientation == 11) porientation = 22;
    }
  if ((porientation == 3) && (ocube[1].num_adjoins == 1)) porientation = 8;
  if ((porientation == 15) && (ocube[1].num_adjoins == 1)) porientation = 23;
  if ((porientation == 7) && (ocube[1].num_adjoins == 2)) porientation = 17;
  
  return porientation;
}

int orientfinder3(piececube * ocube, int refcube)
{
  int porientation;
  if((contains(&ocube[refcube], right)) && (contains(&ocube[refcube], left)))
    {
      if (contains(&ocube[refcube], down)) porientation = 14; 
      if (contains(&ocube[refcube], up)) porientation = 4; 
      if (contains(&ocube[refcube], ahead)) porientation = 17; 
      if (contains(&ocube[refcube], behind)) porientation = 21; 
    }
  if((contains(&ocube[refcube], down)) && (contains(&ocube[refcube], up)))
    {
      if (contains(&ocube[refcube], right)) porientation = 16; 
      if (contains(&ocube[refcube], left)) porientation = 19; 
      if (contains(&ocube[refcube], ahead)) porientation = 3; 
      if (contains(&ocube[refcube], behind)) porientation = 1; 
    }
  if((contains(&ocube[refcube], ahead)) && (contains(&ocube[refcube], behind)))
    {
      if (contains(&ocube[refcube], right)) porientation = 7; 
      if (contains(&ocube[refcube], left)) porientation = 9; 
      if (contains(&ocube[refcube], down)) porientation = 2; 
      if (contains(&ocube[refcube], up)) porientation = 0; 
    }
  return porientation;
}

int orientfinder4(piececube * ocube, int refcube)
{
  int translate1to4[] = {10, 1, 2, 8, 4, -1, 16, 7, -1, 18, -1, 22, -1, 13, 14, 23};
  return translate1to4[orientfinder1(ocube, refcube)];
}

int orientfinder5(piececube * ocube, int refcube)
{
  int translate1to5[] = {0, -1, -1, 3, 19, -1, 6, 7, -1, 9, -1, 11, -1, 21, 20, 15};
  int porientation;
  porientation = translate1to5[orientfinder1(ocube, refcube)];
  if ((porientation == 15) && (ocube[1].num_adjoins == 1))
    {
      porientation = 23;
    }
  if ((porientation == 7) && (ocube[1].num_adjoins == 2))
    {
      porientation = 17;
    }
  return porientation;
}

int orientfinder6(piececube * ocube, int refcube)
{
  int translate1to6[] = {-1, 5, 12, 8, 19, -1, 6, 17, -1, 9, -1, 11, -1, 21, 20, 15};
  int porientation;
  porientation = translate1to6[orientfinder1(ocube, refcube)];
  if ((porientation == 15) && (ocube[1].num_adjoins == 1))
    {
      porientation = 23;
    }
/*   if ((porientation == 7) && (ocube[1].num_adjoins == 1)) */
/*     { */
/*       porientation = 17; */
/*     } */
  return porientation;
}

int orientfinder7(piececube * ocube, int refcube)
{
  int porientation;
  if(contains(&ocube[refcube], right))
    {
      if(contains(&ocube[refcube], down))
	{
	  if(contains(&ocube[refcube], ahead)) porientation = 8;
          else porientation = 12;
        }
      else
        {
	  if(contains(&ocube[refcube], ahead)) porientation = 6;
          else porientation = 5;
        }
    }
  else
    {
      if(contains(&ocube[refcube], down))
	{
	  if(contains(&ocube[refcube], ahead)) porientation = 3;
          else porientation = 2;
        }
      else
        {
	  if(contains(&ocube[refcube], ahead)) porientation = 0;
          else porientation = 1;
        }
    }
  return porientation;
}

/* --------------------------------------------------- */


/* determine which of the cubes in a piece will be used
 * as the "reference cube", which will be used to locate the piece in space.
 * Accepts an integer identifying the piece and
 * an array of four cubes comprising a piece
 * (last is unused in the case of piece 1).
 *
 * The reference cube is always (one of the) cube(s) in the crotch of
 * (one of the) v-shape(s) in the piece.
 * Piece 2 is the only difficult case, as there is no symmetry,
 * and there is an internal cube which is not a reference cube.
 * For all the other pieces, either a cube is an end cube with only 1 adjoin
 * or it can be used as the reference cube.
 * For pieces 4, 5 and 6, either of the two internal cubes can be used
 * because the piece is symmetrical about the plane adjoining these cubes.
 * For piece 7, if the first cube found (at index 0) is not the reference
 * cube the second found will be, because that is the only possible adjoin
 * from any of the other three cubes which are all end cubes.
 * For piece 3, an end cube is always found first and the refcube second--
 * it is impossible to find the reference cube first, because no matter how
 * the piece is oriented there will always be one cube above, left, or behind
 * the reference cube--but ignoring this simplifies the cases.
 * For piece 1, there is only one cube with two adjoins.
 */
int find_refcube(int whichpiece, piececube * whichisrefcube)
{
  piececube * possrefcube = NULL;
  int possindex;
  int found;

  if (whichpiece == 2)
    {
      if (whichisrefcube[0].num_adjoins == 2)
	{
          possrefcube = &whichisrefcube[0];
          possindex = 0;
        }
      else
        {
          possrefcube = &whichisrefcube[1];
          possindex = 1;
        }
      if ((contains(possrefcube, right)) && (contains(possrefcube, left)) ||
          (contains(possrefcube, down))  && (contains(possrefcube, up)) ||
          (contains(possrefcube, ahead)) && (contains(possrefcube, behind)))
	{
          found = possindex + 1;
        }
      else
        {
          found = possindex;
        }
    } /* handle piece 2 */
  else
    {
      if (whichisrefcube[0].num_adjoins > 1)
	{
          found = 0;
        }
      else
	{
          found = 1;
        }
    }
          return found;
}


/* --------------------------------------------------------------- */

/*  functions that do the bulk of the figure reading and processing work. */

/* write solution database to file. */
void emit(piececube epieces[][4], int * erefindices, int * eorients, int numfigs)
{
  int itor;
  float alignment;
  Bool first = False;

  if ((emitfile = fopen("somafigures.h", "a")) == (FILE *)NULL)
  {
    printf("process: Cannot open file %s\n", emitfile);
    return;
  }

  if (ftell(emitfile) == 0)
    {
      first = True;
      fprintf(emitfile, "#ifndef SOMAFIGURES_H\n#define SOMAFIGURES_H\n");
        fprintf(emitfile, "typedef struct {\n");
      fprintf(emitfile, "  GLfloat refcubex[8];\n");
      fprintf(emitfile, "  GLfloat refcubey[8];\n");
      fprintf(emitfile, "  GLfloat refcubez[8];\n");
      fprintf(emitfile, "  int     orientation[8];\n");
      fprintf(emitfile, "} figure;\n\n");
     fprintf(emitfile, "figure somafigures[] = {\n");
    }

  if(numfigs != 0)
    {
      fprintf(emitfile, "};\n");
      fprintf(emitfile, "int somafigurescount = %d;\n\n", numfigs);
      fprintf(emitfile, "#endif\n");
    }

  else
    {
      if (!first) fprintf(emitfile, ",\n");
  fprintf(emitfile, "{{0.0, ");
  alignment = figlength / 2;
  if (figlength % 2 == 0) alignment -= 0.5;
  for (itor = 1; itor < 8; itor++)
    {
      fprintf(emitfile, "%f", epieces[itor][erefindices[itor]].position[2] - alignment);
      if (itor < 7) fprintf(emitfile, ", ");
    }
  fprintf(emitfile, "}, {0.0, ");
  alignment = figdepth / 2;
  if (figdepth % 2 == 0) alignment -= 0.5;
  for (itor = 1; itor < 8; itor++)
    {
      fprintf(emitfile, "%f", (float)(figdepth - epieces[itor][erefindices[itor]].position[1]) - alignment);
      if (itor < 7) fprintf(emitfile, ", ");
    }
  fprintf(emitfile, "}, {0.0, ");
  alignment = figwidth / 2;
  if (figwidth % 2 == 0) alignment -= 0.5;
  for (itor = 1; itor < 8; itor++)
    {
      fprintf(emitfile, "%f", (float)epieces[itor][erefindices[itor]].position[0] - alignment);
      if (itor < 7) fprintf(emitfile, ", ");
    }
  fprintf(emitfile, "}, {-1, ");
  for (itor = 1; itor < 8; itor++)
    {
      fprintf(emitfile, "%d", eorients[itor]);
      if (itor < 7) fprintf(emitfile, ", ");
    }
  /* these aren't really needed in the main program,
   * add them to the struct definition if they turn out to be after all
   */
/*   fprintf(emitfile, "}, %d, %d, %d}", figlength, figdepth, figwidth); */
  fprintf(emitfile, "}}");
    }

  fclose(emitfile);

}


/* Finds all the cubes belonging to a particular piece in a figure,
 * called from process() once for each piece in the figure.
 *
 * params: piece number, array of piece's cubes (to be filled in),
 * index into this array denoting cube to search on (0 on first call).
 * return value is used to track index across recursive calls.
 */
int search (int searchpiece, piececube * searchcubes, int  focuscube)
{
  int curz, cury, curx;
  int nextz, nexty, nextx;
  int prevz, prevy, prevx;
  char examine;
  int connectsfound = 0;
  int temp;
  curz =  searchcubes[focuscube].position[0];
  cury = searchcubes[focuscube].position[1];
  curx = searchcubes[focuscube].position[2];
  nextz = curz + 1;
  nexty = cury + 1;
  nextx = curx + 1;
  prevz = curz - 1;
  prevy = cury - 1;
  prevx = curx - 1;

  if (((searchpiece == 1)&&(searchcubes[0].num_adjoins == 3))
    || (searchcubes[0].num_adjoins == 4))
    {
      return focuscube;
    }
  figurelayout[curz][cury][curx] = '\0';

  /* search right (z), then down (y), then ahead (x) */
  if (nextz < figwidth)
    {
  examine = figurelayout[nextz][cury][curx];
  if (atoi(&examine) == searchpiece)
    {
      connectsfound++;
      searchcubes[focuscube+connectsfound].position[0] = nextz;
      searchcubes[focuscube+connectsfound].position[1] = cury;
      searchcubes[focuscube+connectsfound].position[2] = curx;
      searchcubes[focuscube].adjoins[searchcubes[focuscube].num_adjoins]= right;
      searchcubes[focuscube].num_adjoins++;
      searchcubes[focuscube+connectsfound].adjoins[searchcubes[focuscube+connectsfound].num_adjoins]= left;
      searchcubes[focuscube+connectsfound].num_adjoins++;

      connectsfound += search (searchpiece, searchcubes, focuscube + connectsfound);
    }
    }

  if(nexty < figdepth)
    {
  examine = figurelayout[curz][nexty][curx];
if (atoi(&examine) == searchpiece)
    {
      connectsfound++;
      searchcubes[focuscube+connectsfound].position[0] = curz;
      searchcubes[focuscube+connectsfound].position[1] = nexty;
      searchcubes[focuscube+connectsfound].position[2] = curx;
      searchcubes[focuscube].adjoins[searchcubes[focuscube].num_adjoins]= down;
      searchcubes[focuscube].num_adjoins++;
      searchcubes[focuscube+connectsfound].adjoins[searchcubes[focuscube+connectsfound].num_adjoins]= up;
      searchcubes[focuscube+connectsfound].num_adjoins++;

      connectsfound += search (searchpiece, searchcubes, focuscube + connectsfound);
    }
    }

  if (nextx < figlength)
    {
  examine = figurelayout[curz][cury][nextx];
if (atoi(&examine) == searchpiece)
    {
      connectsfound++;
      searchcubes[focuscube+connectsfound].position[0] = curz;
      searchcubes[focuscube+connectsfound].position[1] = cury;
      searchcubes[focuscube+connectsfound].position[2] = nextx;
      searchcubes[focuscube].adjoins[searchcubes[focuscube].num_adjoins]= ahead;
      searchcubes[focuscube].num_adjoins++;
      searchcubes[focuscube+connectsfound].adjoins[searchcubes[focuscube+connectsfound].num_adjoins]= behind;
      searchcubes[focuscube+connectsfound].num_adjoins++;

      connectsfound += search (searchpiece, searchcubes, focuscube + connectsfound);
    }
    }

  /* repeat searches in reverse direction--used only in recursive calls */
  if (prevz >= 0)
    {
  examine = figurelayout[prevz][cury][curx];
  if (atoi(&examine) == searchpiece)
    {
      connectsfound++;
      searchcubes[focuscube+connectsfound].position[0] = prevz;
      searchcubes[focuscube+connectsfound].position[1] = cury;
      searchcubes[focuscube+connectsfound].position[2] = curx;
      searchcubes[focuscube].adjoins[searchcubes[focuscube].num_adjoins]= left;
      searchcubes[focuscube].num_adjoins++;
      searchcubes[focuscube+connectsfound].adjoins[searchcubes[focuscube+connectsfound].num_adjoins]= right;
      searchcubes[focuscube+connectsfound].num_adjoins++;

      connectsfound += search (searchpiece, searchcubes, focuscube + connectsfound);
    }
    }

  if(prevy >= 0)
    {
  examine = figurelayout[curz][prevy][curx];
if (atoi(&examine) == searchpiece)
    {
      connectsfound++;
      searchcubes[focuscube+connectsfound].position[0] = curz;
      searchcubes[focuscube+connectsfound].position[1] = prevy;
      searchcubes[focuscube+connectsfound].position[2] = curx;
      searchcubes[focuscube].adjoins[searchcubes[focuscube].num_adjoins]= up;
      searchcubes[focuscube].num_adjoins++;
      searchcubes[focuscube+connectsfound].adjoins[searchcubes[focuscube+connectsfound].num_adjoins]= down;
      searchcubes[focuscube+connectsfound].num_adjoins++;

      connectsfound += search (searchpiece, searchcubes, focuscube + connectsfound);
    }
    }

  if (prevx >= 0)
    {
  examine = figurelayout[curz][cury][prevx];
if (atoi(&examine) == searchpiece)
    {
      connectsfound++;
      searchcubes[focuscube+connectsfound].position[0] = curz;
      searchcubes[focuscube+connectsfound].position[1] = cury;
      searchcubes[focuscube+connectsfound].position[2] = prevx;
      searchcubes[focuscube].adjoins[searchcubes[focuscube].num_adjoins]= behind;
      searchcubes[focuscube].num_adjoins++;
      searchcubes[focuscube+connectsfound].adjoins[searchcubes[focuscube+connectsfound].num_adjoins]= ahead;
      searchcubes[focuscube+connectsfound].num_adjoins++;

      connectsfound += search (searchpiece, searchcubes, focuscube + connectsfound);
    }
    }

  return (connectsfound);
}


void process(void)
{
  piececube pieces[8][4];
  int refindices[8];
  int pieceorients[8];
  int currentpiece;
  int  startcube = 0;
  char currentchar;
  int jtor, itor, ktor;
  for (itor = 0; itor < 8; itor++)
    {
      for(jtor = 0; jtor < 4; jtor++)
	{
      pieces[itor][jtor].num_adjoins = 0;
	}
    }
  for (itor = 0; itor < figwidth; itor++)
    {
      for (jtor = 0; jtor < figdepth; jtor++)
	{
          for (ktor = 0; ktor < figlength; ktor++)
	    {
              currentchar = figurelayout[itor][jtor][ktor];
              if (isdigit(currentchar))
		{
		  currentpiece = atoi(&currentchar);
                }
	      else
		{
		  currentpiece = -1;
                }
	      if ((currentpiece > 0) && (currentpiece < 8))
                {
                  pieces[currentpiece][0].position[0] = itor;
                  pieces[currentpiece][0].position[1] = jtor;
                  pieces[currentpiece][0].position[2] = ktor;
		  startcube = search (currentpiece, pieces[currentpiece], 0);
                  refindices[currentpiece] = 
		    find_refcube(currentpiece, pieces[currentpiece]);
                  pieceorients[currentpiece] = 
		    (*ofinders[currentpiece])(pieces[currentpiece], 
					      refindices[currentpiece]);
                }
            }
        }
    }

      emit(pieces, refindices, pieceorients, 0);
}


void parse( char solfile[] )
{
  FILE * solutionfile;
  char inputline[256];
  char * inputtoken;
  char  delims[] = "/|";
  Bool figactive = False;
  int figsfound = 0;

  if ((solutionfile = fopen(solfile, "r")) == (FILE *)NULL)
  {
    printf("parse: Cannot open file %s\n", solfile);
    return;
  }

  while(fgets(inputline, 255, solutionfile)!=NULL)
  {
    if (inputline[0] == ';') continue; /* comment in SOLVER data format */
    if((strlen(inputline)<3) && (figactive == True))
      {
	    figactive = False;
            /* end of figure found--complete processing */

            process();
      }
    if(inputline[0]=='/') 
    {
      if (figactive == False)
	{
    	  /* new figure found */
      figactive = True;
          figsfound++;
          figlength = figdepth = figwidth = 0;
    memset(figurelayout, '\0', 32*32*32);
        }
      /* continue processing the figure */

        inputtoken = strtok(inputline, delims);
      if((strncmp(inputtoken, "SOMA", 4))!= 0)
	{
	  figdepth = 0;
     do
	{
      if (figlength == 0) figlength = strlen(inputtoken);
       strncpy( figurelayout[figwidth][figdepth], inputtoken, figlength);
     inputtoken = strtok(NULL, delims);
       figdepth++;
	}
     while(inputtoken != NULL);
     figwidth++;
     }
 
    } 
      

  } /* line handling -- whole file */
  fclose(solutionfile);
  emit(NULL, NULL, NULL, figsfound);

  }/* parse() */


int main( int argc, char ** argv)
  {
    if (argc < 2)
      {
     printf("Usage: solutionparser <file>\n");
    return -1;
  }

    ofinders[1] = &orientfinder1;
    ofinders[2] = &orientfinder2;
    ofinders[3] = &orientfinder3;
    ofinders[4] = &orientfinder4;
    ofinders[5] = &orientfinder5;
    ofinders[6] = &orientfinder6;
    ofinders[7] = &orientfinder7;

    remove ("somafigures.h");

    parse(argv[1]);
    return 0;

  }



/* ======================================= */

/* problems and possible improvements:
 *
 * extra delimiter at ends of lines causes dimension to be off
 * figures without a /SOMA header are ignored
 * broken figures with a header are parsed, even if body is commented out
 * no checks for broken or impossible figures is made
 * commenting in header isn't done
 * there's no way to handle off-grid pieces that's worth it
 *
 * support for partial and multiset figures (will make error checking harder)
 */
