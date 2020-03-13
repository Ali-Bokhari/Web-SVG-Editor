/*
Ali Bokhari
0970498
*/

#include <math.h>
#include <stdlib.h>
#include "SVGParser.h"


void buildAttrList(List* list, xmlAttr *first) {
	xmlAttr *attr;
	Attribute *tmpattr;
	int len;
	for (attr = first; attr != NULL; attr = attr->next)
	{
		tmpattr = (Attribute*)malloc(sizeof(Attribute));
		xmlNode *value = attr->children;
		len = strlen((char*)(attr->name)) + 2;
		tmpattr->name = (char*)malloc(sizeof(char)*len);
		strcpy(tmpattr->name, (char*)(attr->name));
		//char *attrName = (char *)attr->name;
		len = strlen((char*)(value->content)) + 2;
		tmpattr->value = (char*)malloc(sizeof(char)*len);
		strcpy(tmpattr->value, (char*)(value->content));
		//char *cont = (char *)(value->content);
		insertBack(list, (void*)tmpattr);
		//printf("\tattribute name: %s, attribute value = %s\n", tmpattr->name, tmpattr->value);
	}
}

Rectangle* buildRectangle(xmlAttr *first) {
	xmlAttr *attr;
	Rectangle *tmpattr = (Rectangle*)malloc(sizeof(Rectangle));
	tmpattr->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
	Attribute *tempattribute;
	int len;
	for (attr = first; attr != NULL; attr = attr->next)
	{
		xmlNode *value = attr->children;
		if(strcmp((char*)(attr->name), "x") == 0) {
			char *end;
			tmpattr->x = strtof((char*)(value->content), &end);
			strncpy(tmpattr->units, end, 49);
		} else if(strcmp((char*)(attr->name), "y") == 0) {
			tmpattr->y = strtof((char*)(value->content), NULL);
		} else if(strcmp((char*)(attr->name), "width") == 0) {
			tmpattr->width = strtof((char*)(value->content), NULL);
		} else if(strcmp((char*)(attr->name), "height") == 0) {
			tmpattr->height = strtof((char*)(value->content), NULL);
		} else {
			tempattribute = (Attribute*)malloc(sizeof(Attribute));
			len = strlen((char*)(attr->name)) + 2;
			tempattribute->name = (char*)malloc(sizeof(char)*len);
			strcpy(tempattribute->name, (char*)(attr->name));
			len = strlen((char*)(value->content)) + 2;
			tempattribute->value = (char*)malloc(sizeof(char)*len);
			strcpy(tempattribute->value, (char*)(value->content));
			insertBack(tmpattr->otherAttributes, (void*)tempattribute);
		}
	}
	return tmpattr;
}

Path* buildPath(xmlAttr *first) {
	xmlAttr *attr;
	Path *tmpattr = (Path*)malloc(sizeof(Path));
	tmpattr->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
	Attribute *tempattribute;
	int len;
	for (attr = first; attr != NULL; attr = attr->next)
	{
		xmlNode *value = attr->children;
		if(strcmp((char*)(attr->name), "d") == 0) {
			tmpattr->data = (char*)malloc(sizeof(char)*strlen((char*)(value->content)) + 1);
			strcpy(tmpattr->data, (char*)(value->content));
		} else {
			tempattribute = (Attribute*)malloc(sizeof(Attribute));
			len = strlen((char*)(attr->name)) + 2;
			tempattribute->name = (char*)malloc(sizeof(char)*len);
			strcpy(tempattribute->name, (char*)(attr->name));
			len = strlen((char*)(value->content)) + 2;
			tempattribute->value = (char*)malloc(sizeof(char)*len);
			strcpy(tempattribute->value, (char*)(value->content));
			insertBack(tmpattr->otherAttributes, (void*)tempattribute);
		}
	}
	return tmpattr;
}

Circle* buildCircle(xmlAttr *first) {
	xmlAttr *attr;
	Circle *tmpattr = (Circle*)malloc(sizeof(Circle));
	tmpattr->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
	Attribute *tempattribute;
	int len;
	for (attr = first; attr != NULL; attr = attr->next)
	{
		xmlNode *value = attr->children;
		if(strcmp((char*)(attr->name), "cx") == 0) {
			char *end;
			tmpattr->cx = strtof((char*)(value->content), &end);
			strncpy(tmpattr->units, end, 49);
		} else if(strcmp((char*)(attr->name), "cy") == 0) {
			tmpattr->cy = strtof((char*)(value->content), NULL);
		} else if(strcmp((char*)(attr->name), "r") == 0) {
			tmpattr->r = strtof((char*)(value->content), NULL);
		} else {
			tempattribute = (Attribute*)malloc(sizeof(Attribute));
			len = strlen((char*)(attr->name)) + 2;
			tempattribute->name = (char*)malloc(sizeof(char)*len);
			strcpy(tempattribute->name, (char*)(attr->name));
			len = strlen((char*)(value->content)) + 2;
			tempattribute->value = (char*)malloc(sizeof(char)*len);
			strcpy(tempattribute->value, (char*)(value->content));
			insertBack(tmpattr->otherAttributes, (void*)tempattribute);
		}
	}
	return tmpattr;
}

void buildGroup(Group *img, xmlNode *a_node) {
	xmlNode *cur_node = NULL;
	//char *tmp;

	for (cur_node = a_node; cur_node != NULL; cur_node = cur_node->next) {
			if (cur_node->type == XML_ELEMENT_NODE) {

				//printf("node type: Element, name: %s\n", cur_node->name);

				/*if (strcmp((char*)(cur_node->name), "g") == 0) {
					img->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
					buildAttrList(img->otherAttributes, cur_node->properties);
					buildGroup(img, cur_node->children);
				} else */
				if (strcmp((char*)(cur_node->name), "rect") == 0) {
					Rectangle *rec = buildRectangle(cur_node->properties);
					insertBack(img->rectangles, (void*)rec);
				} else if (strcmp((char*)(cur_node->name), "path") == 0) {
					Path *p = buildPath(cur_node->properties);
					insertBack(img->paths, (void*)p);
				} else if (strcmp((char*)(cur_node->name), "circle") == 0) {
					Circle *c = buildCircle(cur_node->properties);
					insertBack(img->circles, (void*)c);
				} else if (strcmp((char*)(cur_node->name), "g") == 0) {
					Group *g = malloc(sizeof(Group));
					g->rectangles = initializeList(&rectangleToString, &deleteRectangle, &compareRectangles);
					g->paths = initializeList(&pathToString, &deletePath, &comparePaths);
					g->circles = initializeList(&circleToString, &deleteCircle, &compareCircles);
					g->groups = initializeList(&groupToString, &deleteGroup, &compareGroups);
					g->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
					buildAttrList(g->otherAttributes, cur_node->properties);
					buildGroup(g, cur_node->children);
					insertBack(img->groups, (void*)g);
			}
			//buildSVGimage(img, cur_node->children);
	}
}
}


void buildSVGimage(SVGimage *img, xmlNode *a_node) {
	xmlNode *cur_node = NULL;
	char *tmp;

	for (cur_node = a_node; cur_node != NULL; cur_node = cur_node->next) {
			if (cur_node->type == XML_ELEMENT_NODE) {

				//printf("node type: Element, name: %s\n", cur_node->name);

				if (strcmp((char*)(cur_node->name), "svg") == 0) {
					strncpy(img->namespace, (char*)(cur_node->ns->href), 49);
					buildAttrList(img->otherAttributes, cur_node->properties);
					buildSVGimage(img, cur_node->children);
				} else if (strcmp((char*)(cur_node->name), "title") == 0) {
					tmp = (char*)xmlNodeGetContent(cur_node);
					strncpy(img->title, tmp, 255);
					free(tmp);
				} else if (strcmp((char*)(cur_node->name), "desc") == 0) {
					tmp = (char*)xmlNodeGetContent(cur_node);
					strncpy(img->description, tmp, 255);
					free(tmp);
				} else if (strcmp((char*)(cur_node->name), "rect") == 0) {
					Rectangle *rec = buildRectangle(cur_node->properties);
					insertBack(img->rectangles, (void*)rec);
				} else if (strcmp((char*)(cur_node->name), "path") == 0) {
					Path *p = buildPath(cur_node->properties);
					insertBack(img->paths, (void*)p);
				} else if (strcmp((char*)(cur_node->name), "circle") == 0) {
					Circle *c = buildCircle(cur_node->properties);
					insertBack(img->circles, (void*)c);
				} else if (strcmp((char*)(cur_node->name), "g") == 0) {
					Group *g = malloc(sizeof(Group));
					g->rectangles = initializeList(&rectangleToString, &deleteRectangle, &compareRectangles);
					g->paths = initializeList(&pathToString, &deletePath, &comparePaths);
					g->circles = initializeList(&circleToString, &deleteCircle, &compareCircles);
					g->groups = initializeList(&groupToString, &deleteGroup, &compareGroups);
					g->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
					buildAttrList(g->otherAttributes, cur_node->properties);
					buildGroup(g, cur_node->children);
					insertBack(img->groups, (void*)g);
			}
			//buildSVGimage(img, cur_node->children);
	}
}
}

SVGimage* createSVGimage(char* fileName){
	xmlDoc *doc = NULL;
	xmlNode *root_element = NULL;
	SVGimage *img = malloc(sizeof(SVGimage));

	LIBXML_TEST_VERSION

	doc = xmlReadFile(fileName, NULL, 0);

	if (doc == NULL) {
		printf("error: could not parse file %s\n", fileName);
		return NULL;
	}

	root_element = xmlDocGetRootElement(doc);

	img->rectangles = initializeList(&rectangleToString, &deleteRectangle, &compareRectangles);
	img->paths = initializeList(&pathToString, &deletePath, &comparePaths);
	img->circles = initializeList(&circleToString, &deleteCircle, &compareCircles);
	img->groups = initializeList(&groupToString, &deleteGroup, &compareGroups);
	img->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);

	(img->title)[0]='\0';
	(img->description)[0]='\0';
	(img->namespace)[0]='\0';

	buildSVGimage(img, root_element);

	xmlFreeDoc(doc);
	xmlCleanupParser();

	return img;
}

char* SVGimageToString(SVGimage* img){
	if (img == NULL) {
		return NULL;
	}
	char *returned = malloc(sizeof(char)*4096);
	sprintf(returned, "SVGImage to String: \n title: %s \n description: %s\n namespace: %s\n", img->title , img->description, img->namespace);
	void* elem;

	ListIterator iter = createIterator(img->otherAttributes);
	while ((elem = nextElement(&iter)) != NULL){
		Attribute* tmpName = (Attribute*)elem;
		char* str = (img->otherAttributes)->printData(tmpName);
		strncat(returned, str, 4096 - strlen(returned) - 1);
		free(str);
	}

	strncat(returned, "--------------------Rectangles:--------------------\n", 4096 - strlen(returned) - 1);

	iter = createIterator(img->rectangles);
	while ((elem = nextElement(&iter)) != NULL){
		Rectangle* tmpName = (Rectangle*)elem;
		char* str = (img->rectangles)->printData(tmpName);
		strncat(returned, str, 4096 - strlen(returned) - 1);
		free(str);
	}

	strncat(returned, "--------------------Paths:--------------------\n", 4096 - strlen(returned) - 1);

	iter = createIterator(img->paths);
	while ((elem = nextElement(&iter)) != NULL){
		Path* tmpName = (Path*)elem;
		char* str = (img->paths)->printData(tmpName);
		strncat(returned, str, 4096 - strlen(returned) - 1);
		free(str);
	}

	strncat(returned, "--------------------Circles:--------------------\n", 4096 - strlen(returned) - 1);

	iter = createIterator(img->circles);
	while ((elem = nextElement(&iter)) != NULL){
		Circle* tmpName = (Circle*)elem;
		char* str = (img->circles)->printData(tmpName);
		strncat(returned, str, 4096 - strlen(returned) - 1);
		free(str);
	}

	strncat(returned, "--------------------Groups:--------------------\n", 4096 - strlen(returned) - 1);

	iter = createIterator(img->groups);
	while ((elem = nextElement(&iter)) != NULL){
		Group* tmpName = (Group*)elem;
		char* str = (img->groups)->printData(tmpName);
		strncat(returned, str, 4096 - strlen(returned) - 1);
		free(str);
	}
	return returned;
}

void deleteSVGimage(SVGimage* img){

	if (img == NULL) {
		return;
	}

	freeList(img->otherAttributes);
	freeList(img->rectangles);
	freeList(img->paths);
	freeList(img->circles);
	freeList(img->groups);
	free(img);
}

void deleteAttribute( void* data){
	Attribute* tmpName;

	if (data == NULL){
		return;
	}

	tmpName = (Attribute*)data;

	free(tmpName->name);
	free(tmpName->value);
	free(tmpName);
}
char* attributeToString( void* data){
	char* tmpStr;
	Attribute* tmpName;
	int len;

	if (data == NULL){
		return NULL;
	}

	tmpName = (Attribute*)data;

	len = strlen(tmpName->name)+strlen(tmpName->value)+17;
	tmpStr = (char*)malloc(sizeof(char)*len);

	sprintf(tmpStr, "\tName: %s Value: %s\n", tmpName->name, tmpName->value);

	return tmpStr;
}
int compareAttributes(const void *first, const void *second){
	return 1;
}

void deleteDummy( void* data) {
	return;
}

void deleteRectangle(void* data) {
	Rectangle* tmpName;

	if (data == NULL){
		return;
	}

	tmpName = (Rectangle*)data;
	freeList(tmpName->otherAttributes);
	free(tmpName);
}
char* rectangleToString(void* data) {
	char* tmpStr;
	Rectangle* tmpName;
	//int len;

	if (data == NULL){
		return NULL;
	}

	tmpName = (Rectangle*)data;

	//len = strlen(tmpName->name)+strlen(tmpName->value)+17;
	tmpStr = (char*)malloc(sizeof(char)*1024);

	sprintf(tmpStr, "\tx: %f y: %f width: %f height %f units: %s\n", tmpName->x, tmpName->y, tmpName->width, tmpName->height, tmpName->units);

	void *elem;
	ListIterator iter = createIterator(tmpName->otherAttributes);
	while ((elem = nextElement(&iter)) != NULL){
		Attribute* tmpN = (Attribute*)elem;
		char* str = (tmpName->otherAttributes)->printData(tmpN);
		strcat(tmpStr, str);
		free(str);
	}

	return tmpStr;
}
int compareRectangles(const void *first, const void *second) {
	return 1;
}

void deletePath(void* data) {
	Path* tmpName;

	if (data == NULL){
		return;
	}

	tmpName = (Path*)data;
	free(tmpName->data);
	freeList(tmpName->otherAttributes);
	free(tmpName);
}
char* pathToString(void* data) {
	char* tmpStr;
	Path* tmpName;
	//int len;

	if (data == NULL){
		return NULL;
	}

	tmpName = (Path*)data;

	//len = strlen(tmpName->name)+strlen(tmpName->value)+17;
	tmpStr = (char*)malloc(sizeof(char)*2048);

	snprintf(tmpStr, 2047, "\td: %s\n", tmpName->data);

	void *elem;
	ListIterator iter = createIterator(tmpName->otherAttributes);
	while ((elem = nextElement(&iter)) != NULL){
		Attribute* tmpN = (Attribute*)elem;
		char* str = (tmpName->otherAttributes)->printData(tmpN);
		strncat(tmpStr, str, 2048 - strlen(tmpStr) - 1);
		free(str);
	}

	return tmpStr;
}
int comparePaths(const void *first, const void *second) {
	return 1;
}

void deleteCircle(void* data) {
	Circle* tmpName;

	if (data == NULL){
		return;
	}

	tmpName = (Circle*)data;
	freeList(tmpName->otherAttributes);
	free(tmpName);
}
char* circleToString(void* data) {
	char* tmpStr;
	Circle* tmpName;
	//int len;

	if (data == NULL){
		return NULL;
	}

	tmpName = (Circle*)data;

	//len = strlen(tmpName->name)+strlen(tmpName->value)+17;
	tmpStr = (char*)malloc(sizeof(char)*1024);

	sprintf(tmpStr, "\tcx: %f cy: %f r: %f units: %s\n", tmpName->cx, tmpName->cy, tmpName->r, tmpName->units);

	void *elem;
	ListIterator iter = createIterator(tmpName->otherAttributes);
	while ((elem = nextElement(&iter)) != NULL){
		Attribute* tmpN = (Attribute*)elem;
		char* str = (tmpName->otherAttributes)->printData(tmpN);
		strncat(tmpStr, str, 1024 - strlen(tmpStr) - 1);
		free(str);
	}

	return tmpStr;
}
int compareCircles(const void *first, const void *second) {
	return 1;
}

void deleteGroup(void* data) {
	Group* tmpName;

	if (data == NULL){
		return;
	}

	tmpName = (Group*)data;
	freeList(tmpName->rectangles);
	freeList(tmpName->paths);
	freeList(tmpName->circles);
	freeList(tmpName->groups);
	freeList(tmpName->otherAttributes);
	free(tmpName);
}
char* groupToString( void* data) {
	char* returned;
	Group* tmpName;
	//char* atmp;
	//int len;

	if (data == NULL){
		return NULL;
	}

	tmpName = (Group*)data;

	//len = strlen(tmpName->name)+strlen(tmpName->value)+17;
	returned = (char*)malloc(sizeof(char)*2048);

	strcpy(returned, "Group:\n");

	void *elem;
	ListIterator iter = createIterator(tmpName->otherAttributes);
	while ((elem = nextElement(&iter)) != NULL){
		Attribute* tmpAttr = (Attribute*)elem;
		char* str = (tmpName->otherAttributes)->printData(tmpAttr);
		strncat(returned, str, 2048 - strlen(returned) - 1);
		free(str);
	}

	strncat(returned, "Rectangles:\n", 2048 - strlen(returned) - 1);

	iter = createIterator(tmpName->rectangles);
	while ((elem = nextElement(&iter)) != NULL){
		Rectangle* tmpAttr = (Rectangle*)elem;
		char* str = (tmpName->rectangles)->printData(tmpAttr);
		strncat(returned, str, 2048 - strlen(returned) - 1);
		free(str);
	}

	strncat(returned, "Paths:\n", 2048 - strlen(returned) - 1);

	iter = createIterator(tmpName->paths);
	while ((elem = nextElement(&iter)) != NULL){
		Path* tmpAttr = (Path*)elem;
		char* str = (tmpName->paths)->printData(tmpAttr);
		strncat(returned, str, 2048 - strlen(returned) - 1);
		free(str);
	}

	strncat(returned, "Circles:\n", 2048 - strlen(returned) - 1);

	iter = createIterator(tmpName->circles);
	while ((elem = nextElement(&iter)) != NULL){
		Circle* tmpAttr = (Circle*)elem;
		char* str = (tmpName->circles)->printData(tmpAttr);
		strncat(returned, str, 2048 - strlen(returned) - 1);
		free(str);
	}

	strncat(returned, "----------Groups----------:\n", 2048 - strlen(returned) - 1);

	iter = createIterator(tmpName->groups);
	while ((elem = nextElement(&iter)) != NULL){
		Group* tmpAttr = (Group*)elem;
		char* str = (tmpName->groups)->printData(tmpAttr);
		strncat(returned, str, 2048 - strlen(returned) - 1);
		free(str);
	}
	return returned;
}
int compareGroups(const void *first, const void *second) {
	return 1;
}

List* getRects(SVGimage* img) {
	if (img == NULL) {
		return NULL;
	}
	List *rectangles = initializeList(&rectangleToString, &deleteDummy, &compareRectangles);

	void *elem;
	ListIterator iter = createIterator(img->rectangles);
	while ((elem = nextElement(&iter)) != NULL){
		//Rectangle* tmpAttr = (Rectangle*)elem;
		insertBack(rectangles, elem);
	}

	void *elem2;
	ListIterator iter2;

	List *gs = getGroups(img);
	iter = createIterator(gs);
	while ((elem = nextElement(&iter)) != NULL){
		Group* tmpAttr = (Group*)elem;
		iter2 = createIterator(tmpAttr->rectangles);
		while ((elem2 = nextElement(&iter2)) != NULL) {
			insertBack(rectangles, elem2);
		}
	}
	freeList(gs);
	return rectangles;
}

List* getCircles(SVGimage* img) {
	if (img == NULL) {
		return NULL;
	}
	List *circles = initializeList(&circleToString, &deleteDummy, &compareCircles);;

	void *elem;
	ListIterator iter = createIterator(img->circles);
	while ((elem = nextElement(&iter)) != NULL){
		//Rectangle* tmpAttr = (Rectangle*)elem;
		insertBack(circles, elem);
	}

	void *elem2;
	ListIterator iter2;

	List *gs = getGroups(img);
	iter = createIterator(gs);
	while ((elem = nextElement(&iter)) != NULL){
		Group* tmpAttr = (Group*)elem;
		iter2 = createIterator(tmpAttr->circles);
		while ((elem2 = nextElement(&iter2)) != NULL) {
			insertBack(circles, elem2);
		}
	}
	freeList(gs);
	return circles;
}

List* getPaths(SVGimage* img) {
	if (img == NULL) {
		return NULL;
	}
	List *paths = initializeList(&pathToString, &deleteDummy, &comparePaths);

	void *elem;
	ListIterator iter = createIterator(img->paths);
	while ((elem = nextElement(&iter)) != NULL){
		//Rectangle* tmpAttr = (Rectangle*)elem;
		insertBack(paths, elem);
	}

	void *elem2;
	ListIterator iter2;

	List *gs = getGroups(img);
	iter = createIterator(gs);
	while ((elem = nextElement(&iter)) != NULL){
		Group* tmpAttr = (Group*)elem;
		iter2 = createIterator(tmpAttr->paths);
		while ((elem2 = nextElement(&iter2)) != NULL) {
			insertBack(paths, elem2);
		}
	}
	freeList(gs);
	return paths;
}

void getGs(List* gs, List* groups) {
	if (getLength(groups)) {
		void *elem;
		ListIterator iter = createIterator(groups);
		while ((elem = nextElement(&iter)) != NULL){
			Group* tmpAttr = (Group*)elem;
			insertBack(gs, tmpAttr);
			getGs(gs, tmpAttr->groups);
		}
	}
}

List* getGroups(SVGimage* img) {
	if (img == NULL) {
		return NULL;
	}
	List *gs = initializeList(&groupToString, &deleteDummy, &compareGroups);
	getGs(gs, img->groups);
	return gs;
}

int numRectsWithArea(SVGimage* img, float area) {

	if (img == NULL) {
		return 0;
	}

	List *rects = getRects(img);
	float narea;
	int total = 0;

	void *elem;
	ListIterator iter = createIterator(rects);
	while ((elem = nextElement(&iter)) != NULL){
		Rectangle* tmpAttr = (Rectangle*)elem;
		narea = (tmpAttr->width)*(tmpAttr->height);
		if (ceil(narea) == ceil(area)) {
			total ++;
		}
	}

	freeList(rects);

	return total;
}

int numCirclesWithArea(SVGimage* img, float area) {

	if (img == NULL) {
		return 0;
	}

	List *circles = getCircles(img);
	float narea;
	int total = 0;

	void *elem;
	ListIterator iter = createIterator(circles);
	while ((elem = nextElement(&iter)) != NULL){
		Circle* tmpAttr = (Circle*)elem;
		narea = 3.14159265358979323846*((tmpAttr->r)*(tmpAttr->r));
		if (ceil(narea) == ceil(area)) {
			total ++;
		}
	}

	freeList(circles);

	return total;
}

int numPathsWithdata(SVGimage* img, char* data) {

	if (img == NULL) {
		return 0;
	}

	List *paths = getPaths(img);
	int total = 0;

	void *elem;
	ListIterator iter = createIterator(paths);
	while ((elem = nextElement(&iter)) != NULL){
		Path* tmpAttr = (Path*)elem;
		if (strcmp(tmpAttr->data, data) == 0) {
			total ++;
		}
	}

	freeList(paths);

	return total;
}

int numGroupsWithLen(SVGimage* img, int len){
	if (img == NULL) {
		return 0;
	}

	List *groups = getGroups(img);
	int num;
	int total = 0;

	void *elem;
	ListIterator iter = createIterator(groups);
	while ((elem = nextElement(&iter)) != NULL){
		Group* tmpAttr = (Group*)elem;
		num = getLength(tmpAttr->rectangles) + getLength(tmpAttr->circles) + getLength(tmpAttr->paths) + getLength(tmpAttr->groups);
		if (num == len) {
			total ++;
		}
	}

	freeList(groups);

	return total;
}

int numAttr(SVGimage* img) {
	if (img == NULL) {
		return 0;
	}

	int total = 0;

	List *rects = getRects(img);

	void *elem;
	ListIterator iter = createIterator(rects);
	while ((elem = nextElement(&iter)) != NULL){
		Rectangle* tmpAttr = (Rectangle*)elem;
		total += getLength(tmpAttr->otherAttributes);
	}

	freeList(rects);

	List *circles = getCircles(img);

  iter = createIterator(circles);
	while ((elem = nextElement(&iter)) != NULL){
		Circle* tmpAttr = (Circle*)elem;
		total += getLength(tmpAttr->otherAttributes);
	}

	freeList(circles);

	List *paths = getPaths(img);

  iter = createIterator(paths);
	while ((elem = nextElement(&iter)) != NULL){
		Path* tmpAttr = (Path*)elem;
		total += getLength(tmpAttr->otherAttributes);
	}

	freeList(paths);

	List *groups = getGroups(img);

  iter = createIterator(groups);
	while ((elem = nextElement(&iter)) != NULL){
		Group* tmpAttr = (Group*)elem;
		total += getLength(tmpAttr->otherAttributes);
	}

	freeList(groups);

	total+= getLength(img->otherAttributes);

	return total;
}

SVGimage* createValidSVGimage(char* fileName, char* schemaFile) {
	xmlDoc *doc = NULL;
	xmlSchemaParserCtxtPtr parserCtxt = NULL;
	xmlSchemaPtr schema = NULL;
  xmlSchemaValidCtxtPtr validCtxt = NULL;
	int r;

	doc = xmlReadFile(fileName, NULL, 0);
	if (doc == NULL) {
		printf("error: could not parse file %s\n", fileName);
		return NULL;
	}



	parserCtxt = xmlSchemaNewParserCtxt(schemaFile);

  if (parserCtxt == NULL) {
      fprintf(stderr, "Could not create XSD schema parsing context.\n");
      return NULL;
  }

  schema = xmlSchemaParse(parserCtxt);

  if (schema == NULL) {
      fprintf(stderr, "Could not parse XSD schema.\n");
      return NULL;
  }

  validCtxt = xmlSchemaNewValidCtxt(schema);

  if (!validCtxt) {
      fprintf(stderr, "Could not create XSD schema validation context.\n");
      exit(0);
  }
	r = xmlSchemaValidateDoc(validCtxt, doc);

	xmlSchemaFreeParserCtxt(parserCtxt);
	xmlSchemaFree(schema);
  xmlSchemaFreeValidCtxt(validCtxt);
	xmlFreeDoc(doc);
	xmlCleanupParser();

	if (r==0) {
		return createSVGimage(fileName);
	}

	return NULL;
}

 void buildGXML(Group *img, xmlNodePtr root_node) {
	xmlNodePtr node = NULL;
	void *elem;
	void *elem2;
	char tmp[256];

	ListIterator iter = createIterator(img->otherAttributes);
	while ((elem = nextElement(&iter)) != NULL){
		Attribute* tmpName = (Attribute*)elem;
		xmlNewProp(root_node, BAD_CAST tmpName->name, BAD_CAST tmpName->value);
	}

	ListIterator iter2;
	iter = createIterator(img->rectangles);
	while ((elem = nextElement(&iter)) != NULL){
		Rectangle* tmpName = (Rectangle*)elem;
		node = xmlNewChild(root_node, NULL, BAD_CAST "rect", NULL);
		sprintf(tmp, "%f%s", tmpName->x, tmpName->units);
		xmlNewProp(node, BAD_CAST "x", BAD_CAST tmp);
		sprintf(tmp, "%f%s", tmpName->y, tmpName->units);
		xmlNewProp(node, BAD_CAST "y", BAD_CAST tmp);
		sprintf(tmp, "%f%s", tmpName->width, tmpName->units);
		xmlNewProp(node, BAD_CAST "width", BAD_CAST tmp);
		sprintf(tmp, "%f%s", tmpName->height, tmpName->units);
		xmlNewProp(node, BAD_CAST "height", BAD_CAST tmp);
		iter2 = createIterator(tmpName->otherAttributes);
		while ((elem2 = nextElement(&iter2)) != NULL){
			Attribute* tmpName2 = (Attribute*)elem2;
			xmlNewProp(node, BAD_CAST tmpName2->name, BAD_CAST tmpName2->value);
		}
	}

	iter = createIterator(img->circles);
	while ((elem = nextElement(&iter)) != NULL){
		Circle* tmpName = (Circle*)elem;
		node = xmlNewChild(root_node, NULL, BAD_CAST "circle", NULL);
		sprintf(tmp, "%f%s", tmpName->cx, tmpName->units);
		xmlNewProp(node, BAD_CAST "cx", BAD_CAST tmp);
		sprintf(tmp, "%f%s", tmpName->cy, tmpName->units);
		xmlNewProp(node, BAD_CAST "cy", BAD_CAST tmp);
		sprintf(tmp, "%f%s", tmpName->r, tmpName->units);
		xmlNewProp(node, BAD_CAST "r", BAD_CAST tmp);
		iter2 = createIterator(tmpName->otherAttributes);
		while ((elem2 = nextElement(&iter2)) != NULL){
			Attribute* tmpName2 = (Attribute*)elem2;
			xmlNewProp(node, BAD_CAST tmpName2->name, BAD_CAST tmpName2->value);
		}
	}

	iter = createIterator(img->paths);
	while ((elem = nextElement(&iter)) != NULL){
		Path* tmpName = (Path*)elem;
		node = xmlNewChild(root_node, NULL, BAD_CAST "path", NULL);
		xmlNewProp(node, BAD_CAST "d", BAD_CAST tmpName->data);
		iter2 = createIterator(tmpName->otherAttributes);
		while ((elem2 = nextElement(&iter2)) != NULL){
			Attribute* tmpName2 = (Attribute*)elem2;
			xmlNewProp(node, BAD_CAST tmpName2->name, BAD_CAST tmpName2->value);
		}
	}

	iter = createIterator(img->groups);
	while ((elem = nextElement(&iter)) != NULL){
		Group* tmpName = (Group*)elem;
		node = xmlNewChild(root_node, NULL, BAD_CAST "g", NULL);
		buildGXML(tmpName, node);
	}
}

xmlDocPtr SVGtoXML(SVGimage *img) {
	xmlDocPtr doc = NULL;
  xmlNodePtr root_node = NULL, node = NULL;
	char tmp[256];

	doc = xmlNewDoc(BAD_CAST "1.0");
  root_node = xmlNewNode(NULL, BAD_CAST "svg");
  xmlDocSetRootElement(doc, root_node);

	xmlSetNs(root_node, xmlNewNs(root_node, BAD_CAST img->namespace, NULL));
	xmlNewChild(root_node, NULL, BAD_CAST "title", BAD_CAST img->title);
	xmlNewChild(root_node, NULL, BAD_CAST "desc", BAD_CAST img->description);


	void *elem;
	void *elem2;
	ListIterator iter = createIterator(img->otherAttributes);
	while ((elem = nextElement(&iter)) != NULL){
		Attribute* tmpName = (Attribute*)elem;
		xmlNewProp(root_node, BAD_CAST tmpName->name, BAD_CAST tmpName->value);
	}

	ListIterator iter2;
	iter = createIterator(img->rectangles);
	while ((elem = nextElement(&iter)) != NULL){
		Rectangle* tmpName = (Rectangle*)elem;
		node = xmlNewChild(root_node, NULL, BAD_CAST "rect", NULL);
		sprintf(tmp, "%f%s", tmpName->x, tmpName->units);
		xmlNewProp(node, BAD_CAST "x", BAD_CAST tmp);
		sprintf(tmp, "%f%s", tmpName->y, tmpName->units);
		xmlNewProp(node, BAD_CAST "y", BAD_CAST tmp);
		sprintf(tmp, "%f%s", tmpName->width, tmpName->units);
		xmlNewProp(node, BAD_CAST "width", BAD_CAST tmp);
		sprintf(tmp, "%f%s", tmpName->height, tmpName->units);
		xmlNewProp(node, BAD_CAST "height", BAD_CAST tmp);
		iter2 = createIterator(tmpName->otherAttributes);
		while ((elem2 = nextElement(&iter2)) != NULL){
			Attribute* tmpName2 = (Attribute*)elem2;
			xmlNewProp(node, BAD_CAST tmpName2->name, BAD_CAST tmpName2->value);
		}
	}

	iter = createIterator(img->circles);
	while ((elem = nextElement(&iter)) != NULL){
		Circle* tmpName = (Circle*)elem;
		node = xmlNewChild(root_node, NULL, BAD_CAST "circle", NULL);
		sprintf(tmp, "%f%s", tmpName->cx, tmpName->units);
		xmlNewProp(node, BAD_CAST "cx", BAD_CAST tmp);
		sprintf(tmp, "%f%s", tmpName->cy, tmpName->units);
		xmlNewProp(node, BAD_CAST "cy", BAD_CAST tmp);
		sprintf(tmp, "%f%s", tmpName->r, tmpName->units);
		xmlNewProp(node, BAD_CAST "r", BAD_CAST tmp);
		iter2 = createIterator(tmpName->otherAttributes);
		while ((elem2 = nextElement(&iter2)) != NULL){
			Attribute* tmpName2 = (Attribute*)elem2;
			xmlNewProp(node, BAD_CAST tmpName2->name, BAD_CAST tmpName2->value);
		}
	}

	iter = createIterator(img->paths);
	while ((elem = nextElement(&iter)) != NULL){
		Path* tmpName = (Path*)elem;
		node = xmlNewChild(root_node, NULL, BAD_CAST "path", NULL);
		xmlNewProp(node, BAD_CAST "d", BAD_CAST tmpName->data);
		iter2 = createIterator(tmpName->otherAttributes);
		while ((elem2 = nextElement(&iter2)) != NULL){
			Attribute* tmpName2 = (Attribute*)elem2;
			xmlNewProp(node, BAD_CAST tmpName2->name, BAD_CAST tmpName2->value);
		}
	}

	iter = createIterator(img->groups);
	while ((elem = nextElement(&iter)) != NULL){
		Group* tmpName = (Group*)elem;
		node = xmlNewChild(root_node, NULL, BAD_CAST "g", NULL);
		buildGXML(tmpName, node);
		/*iter2 = createIterator(tmpName->otherAttributes);
		while ((elem2 = nextElement(&iter2)) != NULL){
			Attribute* tmpName2 = (Attribute*)elem2;
			xmlNewProp(node, BAD_CAST tmpName2->name, BAD_CAST tmpName2->value);
		}*/
	}

	return doc;
}

bool writeSVGimage(SVGimage* image, char* fileName) {
	if (!image || !fileName) {
		return false;
	}
	xmlDocPtr doc = SVGtoXML(image);
	xmlSaveFormatFileEnc(fileName, doc, "UTF-8", 1);
	xmlFreeDoc(doc);
	xmlCleanupParser();
	return true;
}

bool validateAttributes(List *attrs) {
	if (attrs == NULL) {
		return false;
	}
	void *elem2;
	ListIterator iter2 = createIterator(attrs);
	while ((elem2 = nextElement(&iter2)) != NULL){
		Attribute* tmpName2 = (Attribute*)elem2;
		if (tmpName2->name == NULL || tmpName2->value == NULL) {
			return false;
		}
	}
	return true;
}

bool validateRectangles(List *rects) {
	void *elem2;
	ListIterator iter2 = createIterator(rects);
	while ((elem2 = nextElement(&iter2)) != NULL){
		Rectangle* tmpName2 = (Rectangle*)elem2;
		if (tmpName2->width < 0.0  || tmpName2->height < 0.0 || validateAttributes(tmpName2->otherAttributes) == false) {
			return false;
		}
	}
	return true;
}

bool validateCircles(List *rects) {
	void *elem2;
	ListIterator iter2 = createIterator(rects);
	while ((elem2 = nextElement(&iter2)) != NULL){
		Circle* tmpName2 = (Circle*)elem2;
		if (tmpName2->r < 0.0  || validateAttributes(tmpName2->otherAttributes) == false) {
			return false;
		}
	}
	return true;
}

bool validatePaths(List *rects) {
	void *elem2;
	ListIterator iter2 = createIterator(rects);
	while ((elem2 = nextElement(&iter2)) != NULL){
		Path* tmpName2 = (Path*)elem2;
		if (tmpName2->data == NULL  || validateAttributes(tmpName2->otherAttributes) == false) {
			return false;
		}
	}
	return true;
}

bool validateGroups(List *rects) {
	void *elem2;
	ListIterator iter2 = createIterator(rects);
	while ((elem2 = nextElement(&iter2)) != NULL){
		Group* tmpName2 = (Group*)elem2;
		if (tmpName2->rectangles == NULL  || tmpName2->circles == NULL  || tmpName2->paths == NULL  || tmpName2->groups == NULL  || validateAttributes(tmpName2->otherAttributes) == false) {
			return false;
		}
	}
	return true;
}

bool validateSVGimage(SVGimage* image, char* schemaFile) {
	if(image == NULL || schemaFile == NULL) {
		return false;
	}

	if (image->rectangles == NULL || image->circles == NULL || image->paths == NULL || image->groups == NULL || image->otherAttributes == NULL) {
		return false;
	}

	if (validateAttributes(image->otherAttributes) == false) {
		return false;
	}

	List *rects;
	rects = getRects(image);
	if (validateRectangles(rects) == false) {
		freeList(rects);
		return false;
	}
	freeList(rects);

	List *circles;
	circles = getCircles(image);
	if (validateCircles(circles) == false) {
		freeList(circles);
		return false;
	}
	freeList(circles);

	List *paths;
	paths = getPaths(image);
	if (validatePaths(paths) == false) {
		freeList(paths);
		return false;
	}
	freeList(paths);

	List *groups;
	groups = getPaths(image);
	if (validatePaths(groups) == false) {
		freeList(groups);
		return false;
	}
	freeList(groups);

	xmlDocPtr doc = SVGtoXML(image);
	xmlSchemaParserCtxtPtr parserCtxt = NULL;
	xmlSchemaPtr schema = NULL;
  xmlSchemaValidCtxtPtr validCtxt = NULL;
	int r;

	if (doc == NULL) {
		return false;
	}
	parserCtxt = xmlSchemaNewParserCtxt(schemaFile);

  if (parserCtxt == NULL) {
      return false;
  }

  schema = xmlSchemaParse(parserCtxt);

  if (schema == NULL) {
      return false;
  }

  validCtxt = xmlSchemaNewValidCtxt(schema);

  if (!validCtxt) {
      return false;
  }
	r = xmlSchemaValidateDoc(validCtxt, doc);

	xmlSchemaFreeParserCtxt(parserCtxt);
	xmlSchemaFree(schema);
  xmlSchemaFreeValidCtxt(validCtxt);
	xmlFreeDoc(doc);
	xmlCleanupParser();

	if (r!=0) {
		return false;
	}

	return true;
}

void *getElement(List* list, int index) {
	if (index < 0) {
		return NULL;
	}
	void *elem;
	index++;
	ListIterator iter = createIterator(list);
	for (int i = 0; i < index; i++) {
		if ((elem = nextElement(&iter)) == NULL){
			return elem;
		}
	}
	return elem;
}

void setOtherAttributes(List *attrs, Attribute* newAttribute) {
	void *elem;
	ListIterator iter = createIterator(attrs);
	while ((elem = nextElement(&iter)) != NULL){
		Attribute* tmpName = (Attribute*)elem;
		if (strcmp(tmpName->name, newAttribute->name)==0) {
			free(tmpName->value);
			tmpName->value = malloc(sizeof(char) * (strlen(newAttribute->value) + 1));
			strcpy(tmpName->value, newAttribute->value);
			deleteAttribute(newAttribute);
			return;
		}
	}
	insertBack(attrs, newAttribute);
}

void setAttribute(SVGimage* image, elementType elemType, int elemIndex, Attribute* newAttribute){
	if (image == NULL || newAttribute == NULL) {
		return;
	}
	if (newAttribute->name == NULL || newAttribute->value == NULL) {
		return;
	}

	if(elemType == SVG_IMAGE) {
		if(strcmp(newAttribute->name, "namespace") == 0) {
			strncpy(image->namespace, newAttribute->value, 255);
			deleteAttribute(newAttribute);
		} else {
			setOtherAttributes(image->otherAttributes, newAttribute);
		}
	} else if(elemType == CIRC) {
		Circle *c = getElement(image->circles, elemIndex);
		if (c == NULL) {
			return;
		}
		if(strcmp(newAttribute->name, "cx") == 0) {
			c->cx = strtof(newAttribute->value, NULL);
			deleteAttribute(newAttribute);
		} else if(strcmp(newAttribute->name, "cy") == 0) {
			c->cy = strtof(newAttribute->value, NULL);
			deleteAttribute(newAttribute);
		} else if(strcmp(newAttribute->name, "r") == 0) {
			c->r = strtof(newAttribute->value, NULL);
			deleteAttribute(newAttribute);
		} else {
			setOtherAttributes(c->otherAttributes, newAttribute);
		}
	} else if(elemType == RECT) {
		Rectangle *r = getElement(image->rectangles, elemIndex);
		if (r == NULL) {
			return;
		}
		if(strcmp(newAttribute->name, "x") == 0) {
			r->x = strtof(newAttribute->value, NULL);
			deleteAttribute(newAttribute);
		} else if(strcmp(newAttribute->name, "y") == 0) {
			r->y = strtof(newAttribute->value, NULL);
			deleteAttribute(newAttribute);
		} else if(strcmp(newAttribute->name, "width") == 0) {
			r->width = strtof(newAttribute->value, NULL);
			deleteAttribute(newAttribute);
		} else if(strcmp(newAttribute->name, "height") == 0) {
			r->height = strtof(newAttribute->value, NULL);
			deleteAttribute(newAttribute);
		} else {
			setOtherAttributes(r->otherAttributes, newAttribute);
		}
	} else if(elemType == PATH) {
		Path *p = getElement(image->paths, elemIndex);
		if (p == NULL) {
			return;
		}
		if(strcmp(newAttribute->name, "d") == 0) {
			free(p->data);
			p->data = malloc(sizeof(char) * (strlen(newAttribute->value) + 1));
			strcpy(p->data, newAttribute->value);
			deleteAttribute(newAttribute);
		} else {
			setOtherAttributes(p->otherAttributes, newAttribute);
		}
	} else if(elemType == GROUP) {
		Group *g = getElement(image->groups, elemIndex);
		if (g == NULL) {
			return;
		}
		setOtherAttributes(g->otherAttributes, newAttribute);
	}
}

void addComponent(SVGimage* image, elementType elemType, void* newComponent) {
	if (image == NULL || newComponent == NULL) {
		return;
	}

	if (elemType == CIRC) {
		insertBack(image->circles, newComponent);
	} else if (elemType == RECT) {
		insertBack(image->rectangles, newComponent);
	} else if (elemType == PATH) {
		insertBack(image->paths, newComponent);
	}
}

char* attrToJSON(const Attribute *a) {
	char *final;
	if (a == NULL) {
		final = malloc(sizeof(char) * 3);
		strcpy(final, "{}");
		return final;
	}

	int len = 23 + strlen(a->name) + strlen(a->value);
	final = malloc(sizeof(char) * len);
	snprintf(final, len, "{\"name\":\"%s\",\"value\":\"%s\"}", a->name, a->value);
	return final;
}

char* circleToJSON(const Circle *c) {
	char *final;
	if (c == NULL) {
		final = malloc(sizeof(char) * 3);
		strcpy(final, "{}");
		return final;
	}
	int len = 200 + 40 + 24 + 18 + strlen(c->units);
	final = malloc(sizeof(char) * len);
	char *attrs = attrListToJSON(c->otherAttributes);
	snprintf(final, len, "{\"cx\":%.2f,\"cy\":%.2f,\"r\":%.2f,\"numAttr\":%d,\"units\":\"%s\",\"attrs\":%s}", c->cx, c->cy, c->r, getLength(c->otherAttributes), c->units, attrs);
	free(attrs);
	return final;
}

char* rectToJSON(const Rectangle *r) {
	char *final;
	if (r == NULL) {
		final = malloc(sizeof(char) * 3);
		strcpy(final, "{}");
		return final;
	}
	int len = 200 + 43 + 32 + 18 + strlen(r->units);
	final = malloc(sizeof(char) * (len));
	char *attrs = attrListToJSON(r->otherAttributes);
	snprintf(final, len, "{\"x\":%.2f,\"y\":%.2f,\"w\":%.2f,\"h\":%.2f,\"numAttr\":%d,\"units\":\"%s\",\"attrs\":%s}", r->x, r->y, r->width, r->height, getLength(r->otherAttributes), r->units, attrs);
	free(attrs);
	return final;
}

char* pathToJSON(const Path *p) {
	char *final;
	if (p == NULL) {
		final = malloc(sizeof(char) * 3);
		strcpy(final, "{}");
		return final;
	}
	int len = 300 + 19 + 64 + 18;
	final = malloc(sizeof(char) * len);
	char *attrs = attrListToJSON(p->otherAttributes);
	snprintf(final, len, "{\"d\":\"%.64s\",\"numAttr\":%d,\"attrs\":%s}", p->data, getLength(p->otherAttributes), attrs);
	free(attrs);
	return final;
}

char* groupToJSON(const Group *g) {
	char *final;
	if (g == NULL) {
		final = malloc(sizeof(char) * 3);
		strcpy(final, "{}");
		return final;
	}
	int len = 200 + 24 + 18 + 18;
	final = malloc(sizeof(char) * len);
	char *attrs = attrListToJSON(g->otherAttributes);
	int sum = getLength(g->rectangles) + getLength(g->circles) + getLength(g->paths) + getLength(g->groups);
	snprintf(final, len, "{\"children\":%d,\"numAttr\":%d,\"attrs\":%s}", sum, getLength(g->otherAttributes), attrs);
	free(attrs);
	return final;
}

char* SVGtoJSON(const SVGimage* imge) {
	char *final;
	if (imge == NULL) {
		final = malloc(sizeof(char) * 3);
		strcpy(final, "{}");
		return final;
	}

	final = malloc(sizeof(char) * (256));
	List *rects = getRects((SVGimage*)imge);
	List *circles = getCircles((SVGimage*)imge);
	List *paths = getPaths((SVGimage*)imge);
	List *groups = getGroups((SVGimage*)imge);

	snprintf(final, 255, "{\"numRect\":%d,\"numCirc\":%d,\"numPaths\":%d,\"numGroups\":%d}", getLength(rects), getLength(circles), getLength(paths), getLength(groups));

	freeList(rects);
	freeList(circles);
	freeList(paths);
	freeList(groups);

	return final;
}

char* attrListToJSON(const List *list) {
	char *final;
	if (list == NULL) {
		final = malloc(sizeof(char) * 3);
		strcpy(final, "[]");
		return final;
	}

	int len = getLength((List*)list) * 200;
	final = malloc(sizeof(char) * len);
	strcpy(final, "[");

	char i = 0;

	void *elem2;
	ListIterator iter2 = createIterator((List*)list);
	while ((elem2 = nextElement(&iter2)) != NULL){
		Attribute *tmpName = (Attribute*) elem2;
		char *tmpStr = attrToJSON(tmpName);
		if(i!=0) {
			strncat(final, ",", len-strlen(final)-1);
		}
		i=1;
		strncat(final, tmpStr, len-strlen(final)-1);
		free(tmpStr);
	}
	strncat(final, "]", len-strlen(final)-1);
	return final;
}

char* circListToJSON(const List *list) {
	{
		char *final;
		if (list == NULL) {
			final = malloc(sizeof(char) * 3);
			strcpy(final, "[]");
			return final;
		}

		int len = getLength((List*)list) * 250;
		final = malloc(sizeof(char) * len);
		strcpy(final, "[");

		char i = 0;

		void *elem2;
		ListIterator iter2 = createIterator((List*)list);
		while ((elem2 = nextElement(&iter2)) != NULL){
			Circle *tmpName = (Circle*) elem2;
			char *tmpStr = circleToJSON(tmpName);
			if(i!=0) {
				strncat(final, ",", len-strlen(final)-1);
			}
			i=1;
			strncat(final, tmpStr, len-strlen(final)-1);
			free(tmpStr);
		}
		strncat(final, "]", len-strlen(final)-1);
		return final;
	}
}

char* rectListToJSON(const List *list) {
	{
		char *final;
		if (list == NULL) {
			final = malloc(sizeof(char) * 3);
			strcpy(final, "[]");
			return final;
		}
		int len = getLength((List*)list) * 250;
		final = malloc(sizeof(char) * len);
		strcpy(final, "[");

		char i = 0;

		void *elem2;
		ListIterator iter2 = createIterator((List*)list);
		while ((elem2 = nextElement(&iter2)) != NULL){
			Rectangle *tmpName = (Rectangle*) elem2;
			char *tmpStr = rectToJSON(tmpName);
			if(i!=0) {
				strncat(final, ",", len-strlen(final)-1);
			}
			i=1;
			strncat(final, tmpStr, len-strlen(final)-1);
			free(tmpStr);
		}
		strncat(final, "]", len-strlen(final)-1);
		return final;
	}
}

char* pathListToJSON(const List *list) {
	{
		char *final;
		if (list == NULL) {
			final = malloc(sizeof(char) * 3);
			strcpy(final, "[]");
			return final;
		}

		int len = getLength((List*)list) * 250;
		final = malloc(sizeof(char) * len);
		strcpy(final, "[");

		char i = 0;

		void *elem2;
		ListIterator iter2 = createIterator((List*)list);
		while ((elem2 = nextElement(&iter2)) != NULL){
			Path *tmpName = (Path*) elem2;
			char *tmpStr = pathToJSON(tmpName);
			if(i!=0) {
				strncat(final, ",", len-strlen(final)-1);
			}
			i=1;
			strncat(final, tmpStr, len-strlen(final)-1);
			free(tmpStr);
		}
		strncat(final, "]", len-strlen(final)-1);
		return final;
	}
}

char* groupListToJSON(const List *list) {
	{
		char *final;
		if (list == NULL) {
			final = malloc(sizeof(char) * 3);
			strcpy(final, "[]");
			return final;
		}

		int len = getLength((List*)list) * 250;
		final = malloc(sizeof(char) * len);
		strcpy(final, "[");

		char i = 0;

		void *elem2;
		ListIterator iter2 = createIterator((List*)list);
		while ((elem2 = nextElement(&iter2)) != NULL){
			Group *tmpName = (Group*) elem2;
			char *tmpStr = groupToJSON(tmpName);
			if(i!=0) {
				strncat(final, ",", len-strlen(final)-1);
			}
			i=1;
			strncat(final, tmpStr, len-strlen(final)-1);
			free(tmpStr);
		}
		strncat(final, "]", len-strlen(final)-1);
		return final;
	}
}

SVGimage* JSONtoSVG(const char* svgString) {
	if (svgString == NULL) {
		return NULL;
	}
	char title[256], desc[256];
	title[0] = '\0';
	desc[0] = '\0';

	char regex[550] = "{\"title\":\"%[^\"]s\",\"descr\":\"%*s\"}";
 if (sscanf( svgString, regex, title) < 1) {
	 return NULL;
 }
 snprintf(regex, 550, "{\"title\":\"%s\",\"descr\":", title);
 strcat(regex, "\"%[^\"]s\"}");
 if (sscanf( svgString, regex, desc) < 1) {
	 return NULL;
 }

 SVGimage* img = malloc(sizeof(SVGimage));

 strncpy(img->title, title, 256);
 img->title[255]='\0';

 strncpy(img->description, desc, 256);
 img->description[255]='\0';

 strcpy(img->namespace, "http://www.w3.org/2000/svg");

 img->rectangles = initializeList(&rectangleToString, &deleteRectangle, &compareRectangles);
 img->paths = initializeList(&pathToString, &deletePath, &comparePaths);
 img->circles = initializeList(&circleToString, &deleteCircle, &compareCircles);
 img->groups = initializeList(&groupToString, &deleteGroup, &compareGroups);
 img->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);

 return img;
}

Rectangle* JSONtoRect(const char* svgString) {
	if (svgString == NULL) {
		return NULL;
	}
	char units[50];
	units[0] = '\0';
	float x=0,y=0,w=0,h=0;
	sscanf(svgString, "{\"x\":%f,\"y\":%f,\"w\":%f,\"h\":%f,\"units\":\"%49[^\"]s\"}", &x, &y, &w, &h, units);

	Rectangle *rect = malloc(sizeof(Rectangle));
	rect->x = x;
	rect->y = y;
	rect->width = w;
	rect->height = h;
	strcpy(rect->units, units);
	rect->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);

	return rect;
}

Circle* JSONtoCircle(const char* svgString) {
	if (svgString == NULL) {
		return NULL;
	}
	char units[50];
	units[0] = '\0';
	float cx=0,cy=0,r=0;
	sscanf(svgString, "{\"cx\":%f,\"cy\":%f,\"r\":%f,\"units\":\"%49[^\"]s\"}", &cx, &cy, &r, units);

	Circle *circle = malloc(sizeof(Circle));
	circle->cx = cx;
	circle->cy = cy;
	circle->r = r;
	strcpy(circle->units, units);
	circle->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);

	return circle;
}

Attribute* makeAttr(char* name, char *value) {
	Attribute *attr = malloc(sizeof(Attribute));
	char *nname = malloc(sizeof(char) * (strlen(name)+1));
	char *vvalue = malloc(sizeof(char) * (strlen(value)+1));
	strcpy(nname, name);
	strcpy(vvalue, value);
	attr->name = nname;
	attr->value = vvalue;
	return attr;
}

char* getTitle(SVGimage *img) {
	return img->title;
}

char* getDesc(SVGimage *img) {
	return img->description;
}

char* getRectListJSON(SVGimage *img) {
	return rectListToJSON(img->rectangles);
}

char* getCircListJSON(SVGimage *img) {
	return circListToJSON(img->circles);
}

char* getPathListJSON(SVGimage *img) {
	return pathListToJSON(img->paths);
}

char* getGroupListJSON(SVGimage *img) {
	return groupListToJSON(img->groups);
}

void setTitle(SVGimage *img, char *title) {
	if (!img || !title) {
		return;
	}
	strcpy(img->title, title);
}

void setDescription(SVGimage *img, char *description) {
	if (!img || !description) {
		return;
	}
	strcpy(img->description, description);
}

SVGimage *createSVG() {
	SVGimage* img = malloc(sizeof(SVGimage));

  strcpy(img->title, "");

  strcpy(img->description, "");

  strcpy(img->namespace, "http://www.w3.org/2000/svg");

  img->rectangles = initializeList(&rectangleToString, &deleteRectangle, &compareRectangles);
  img->paths = initializeList(&pathToString, &deletePath, &comparePaths);
  img->circles = initializeList(&circleToString, &deleteCircle, &compareCircles);
  img->groups = initializeList(&groupToString, &deleteGroup, &compareGroups);
  img->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);

  return img;
}

void scaleRects(SVGimage *img, float scaler) {
	List *rects = getRects(img);
	void *elem;
	ListIterator iter = createIterator(rects);
	while ((elem = nextElement(&iter)) != NULL){
		Rectangle* tmpName = (Rectangle*)elem;
		tmpName->width = (tmpName->width)*scaler;
		tmpName->height = (tmpName->height)*scaler;
	}
	freeList(rects);
}

void scaleCircs(SVGimage *img, float scaler) {
	List *circs = getCircles(img);
	void *elem;
	ListIterator iter = createIterator(circs);
	while ((elem = nextElement(&iter)) != NULL){
		Circle* tmpName = (Circle*)elem;
		tmpName->r = (tmpName->r)*scaler;
	}
	freeList(circs);
}
