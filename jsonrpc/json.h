/*
 * =====================================================================================
 *
 *       Filename:  json.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/18/2015 08:13:38 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../src/link_list.h"
#include "../src/stack.h"

struct json_record_s;
struct json_object_s;
#ifdef ff
typedef enum json_type_e {
	JS_STRING,
	JS_INTEGER,
	JS_DOUBLE,
	JS_NULL,
	JS_FALSE,
	JS_TRUE,
	JS_ARRAY,
	JS_OBJECT,
	JS_BOOLIAN
} json_type_t;
#endif

//typedef yytokentype  json_type_t;

typedef struct json_string_s {
	char *value;
	int len;
} json_string_t;

json_string_t *  json_string_new();
void json_string_init(json_string_t *string, char *value);

struct json_record_s *json_record_new_string(char *record_name,char *value);
struct json_record_s *json_record_new_double(char *record_name,double value);
struct json_record_s *json_record_new_integer(char *record_name,int value);
struct json_record_s *json_record_new_array(char *record_name);
struct json_record_s *json_record_new_object(char *record_name);
struct json_record_s *json_record_new_boolian(char *record_name,char value);


struct json_array_s *  json_record_get_array(struct json_record_s *json_record);
struct json_object_s * json_record_get_object(struct json_record_s *json_record);
json_string_t * json_record_get_name(struct json_record_s *json_record);



typedef struct json_object_s {
	link_list_t json_objects;
} json_object_t;
int json_object_add_new_string(struct json_object_s *json_obj, char *record_name,char *value);
int json_object_add_new_double(struct json_object_s *json_obj,char *record_name,double value);
int json_object_add_new_integer(struct json_object_s *json_obj,char *record_name,int value);
//int json_object_add_new_array (struct json_object_s *json_obj,char *record_name,struct json_array_s *value);
int json_object_add_new_object(struct json_object_s *json_obj,char *record_name,struct json_object_s *value);
int json_object_add_new_boolian(struct json_object_s *json_obj,char *record_name,char value);
struct json_object_s * json_object_add_record(struct json_object_s *json_obj,struct json_record_s  *value);

int json_object_add_new_record(struct json_object_s *json_obj,struct json_record_s *json_record);
json_string_t *       json_object_get_string(struct json_object_s *json_obj, char *record_name);
double                json_object_get_double(struct json_object_s *json_obj,char *record_name);
int                   json_object_get_int(struct json_object_s *json_obj,char *record_name);
struct json_array_s * json_object_get_array(struct json_object_s *json_obj,char *record_name);
struct json_object_s  json_object_get_object(struct json_object_s *json_obj,char *record_name);
char                  json_object_get_boolian(struct json_object_s *json_obj,char *record_name);
json_object_t *json_object_new();


typedef struct json_array_s {
	link_list_t json_records;
} json_array_t;

int json_array_add_new_string(struct json_array_s *json_obj, char *value);
int json_array_add_new_double(struct json_array_s *json_obj,double value);
int json_array_add_new_integer(struct json_array_s *json_obj,int value);
int json_array_add_new_array(struct json_array_s *json_obj,struct json_array_s *value);
int json_array_add_new_object(struct json_array_s *json_obj,struct json_object_s *value);
int json_array_add_new_boolian(struct json_array_s *json_obj,char value);
struct json_array_s * json_array_add_record(struct json_array_s *json_obj,struct json_record_s  *value);

json_array_t *json_array_new();



typedef union json_val_type_u {
	int integer;
	double real;
	char boolian;
	struct json_string_s string;
	struct json_array_s array;
	struct json_object_s object;
} json_val_type_t;


typedef union json_value_u {
	int type;	
	json_val_type_t u;
} json_value_t;


typedef struct json_record_s {
	json_string_t name;
	json_value_t value;
} json_record_t;

void set_stack_array(parser_stack_t *stack);
void set_stack_object(parser_stack_t *stack);
void set_json_object(json_object_t *jobj);

void set_json_array(json_array_t *jobj);
json_record_t *get_tmp_record();
	
json_object_t * json_parser(const char *text);


