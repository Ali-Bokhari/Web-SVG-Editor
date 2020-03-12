#include <stdio.h>
#include <stdlib.h>
#include "SVGParser.h"

int main (int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "Wrong number of arguments\n");
    exit(0);
  }

  SVGimage* hello = createValidSVGimage(argv[1], argv[2]);
  if (hello == NULL) {
    fprintf(stderr, "Not valid mate; create\n");
    exit(0);
  }

  /*if (validateSVGimage(hello, argv[2]) == true) {
    fprintf(stderr, "validates :)\n");
  } else {
    fprintf(stderr, "not valideeee\n");
  }

  if (hello == NULL) {
    fprintf(stderr, "Not valid mate\n");
    exit(0);
  }*/

  Attribute *new = malloc(sizeof(Attribute));
  new->name = malloc(sizeof(char)*10);
  new->value = malloc(sizeof(char)*10);
  strcpy(new->name, "width");
  strcpy(new->value, "2");


  setAttribute(hello, SVG_IMAGE, -2, new);

  char* attrlist = attrListToJSON(hello->otherAttributes);
  char* circlist = circListToJSON(hello->circles);
  char* rectlist = rectListToJSON(hello->rectangles);
  char* pathlist = pathListToJSON(hello->paths);
  char* grouplist = groupListToJSON(hello->groups);

  printf("%s \n%s \n%s \n%s \n%s\n", attrlist, circlist, rectlist, pathlist, grouplist);

  free(attrlist);
  free(circlist);
  free(rectlist);
  free(pathlist);
  free(grouplist);
  writeSVGimage(hello, "gangster.svg");
  deleteSVGimage(hello);
  return 0;
}

