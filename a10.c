#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define LINESIZE 1024
#define IDSIZE 10
#define NAMESIZE 20

typedef struct{
	char last[NAMESIZE];
	char first[NAMESIZE];
} name;

typedef struct{
	char id[IDSIZE];
	name name;
	int score;
} record;

typedef struct node node;
struct node{
	record data;
	node *next;
};

typedef node *record_list;

int is_valid_id(const char s[]){
	size_t i;
	if(strlen(s) != 9)
		return 0;
	if(!(s[0] == 'a' || s[0] == 'A'))
		return 0;
	for(i = 1; s[i] != '\0'; i++)
		if(!isdigit(s[i]))
			return 0;
	return 1;
}

int parse_record(const char *s, record *prec){
	char id[LINESIZE], first[LINESIZE], last[LINESIZE];
	int score = 0;
	size_t i, j;
	if(sscanf(s, "%s %s %s %d", id, last, first, &score) == 4){
		if(!is_valid_id(id))
			return 0;
		if(score < 0 || score > 100)
			return 0;
		if(strlen(first) > NAMESIZE || strlen(last) > NAMESIZE)
			return 0;
		for(i = 0; i < strlen(first); i++)
			first[i] = tolower(first[i]);
		for(j = 0; j < strlen(last); j++)
			last[j] = tolower(last[j]);
		strcpy(prec -> id, id);
		strcpy(prec -> name.first, first);
		strcpy(prec -> name.last, last);
		prec -> score = score;
		return 1;
	}
	return 0;
}

void print_record(const record *prec){
	printf("%d %s %s %s\n", prec -> score, prec -> name.last, prec -> name.first, prec -> id);
}

int cmp_score_desc_name_asc_id_asc(const void *p, const void *q){
	const record *pp = p;
	const record *qq = q;
	int n;
	if((n = qq -> score - pp -> score) != 0)
		return n;
	if((n = strcmp(pp -> name.last, qq -> name.last)) !=0)
		return n;
	if((n = strcmp(pp -> name.first, qq -> name.first)) !=0)
		return n;
	return strcmp(pp -> id, qq -> id);
}

void list_init(record_list *plist){
	*plist = 0;
}

void list_clear(record_list *plist){
	node  *p, *q;
	for (p = *plist; p != 0; p = q) {
		q = p -> next;
		free(p);
	}
	list_init(plist);
}

int list_insert(record_list *plist, const record *prec){
	node **tracer;
	node *newnode = malloc(sizeof(node));
	if(newnode == 0)
		return 0;
	newnode -> data = *prec;
	for(tracer = plist; *tracer != 0; tracer = &(*tracer) -> next)
		if(cmp_score_desc_name_asc_id_asc(&(*tracer)->data, &newnode->data) > 0)
			break;
	newnode -> next = *tracer;
	*tracer = newnode;
	return 1;
}

int main(void){
	char line[LINESIZE];
	record record;
	record_list list;
	node *p;
	list_init(&list);
	printf("Enter record:\n");
	while(fgets(line, LINESIZE, stdin))
		if(parse_record(line, &record))
			if(!list_insert(&list, &record)){
				fprintf(stderr, "Error\n");
				exit(-1);
			}
	printf("\nSorted list:\n");
	for(p = list; p != 0; p = p->next)
		print_record(&p->data);
	list_clear(&list);
	return 0;
}
