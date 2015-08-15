/*
 * =====================================================================================
 *
 *       Filename:  json.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/18/2015 08:12:37 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yair Gadelov (yg), yair.gadelov@gmail.com
 *        Company:  Israel
 *
 * =====================================================================================
 */

#include "json.h"
#include "libjson_a-yjson.h"

extern int yylex();
extern int yyparse();
extern FILE *yyin;
extern int line;
extern int col;
extern char *yytext;
int __attribute__((weak))  level=0;
void yy_switch_to_buffer (	struct yy_buffer_state * new_buffer  );
struct yy_buffer_state *  yy_scan_string (const char *yy_str  );
void yy_delete_buffer (struct yy_buffer_state *  b  );



/*
 * static variables 
 */
static parser_stack_t *stack_array;
static parser_stack_t *stack_object;
static json_object_t *json_object;
static json_array_t *json_array;


/**
 * Created  07/25/2015
 * @brief   initiates initiated json_string_t 
 * @param   json_string: initiated json_string_t
 * @param   value: string characters
 * @param   len: string length
 * @return  
 */
void json_string_set(json_string_t *string, char *value)
{
	string->value = value;
	string->len = strlen(value);
}


/**
 * Created  07/25/2015
 * @brief   creates new json_value_t structure
 * @param   
 * @return 0 - success,1 fail  
 */
json_value_t * json_string(char *str)
{
	json_value_t *json_value= malloc(sizeof(json_value_t));
	json_string_set (&json_value->u.string,str);
	json_value->type = JS_STRING;
	return json_value;
}



/**
 * Created  07/25/2015
 * @brief   creates new json_value_t structure
 * @param   
 * @return 0 - success,1 fail  
 */
char json_integer(json_value_t *jsvalue, int value)
{
//	json_value_t *json_value= malloc(sizeof(json_value_t));
	jsvalue->type = JS_INTEGER;
	jsvalue->u.integer = value;
	return 1;
}

/**
 * Created  07/25/2015
 * @brief   creates new json_value_t structure
 * @param   
 * @return 0 - success,1 fail  
 */
json_value_t * json_double(double value)
{
	json_value_t *json_value= malloc(sizeof(json_value_t));
	json_value->type = JS_DOUBLE;
	json_value->u.real = value;
	return json_value;
}

/**
 * Created  07/25/2015
 * @brief   creates new json_value_t structure
 * @param   
 * @return 0 - success,1 fail  
 */
json_value_t * json_boolian(char value)
{
	json_value_t *json_value= malloc(sizeof(json_value_t));
	json_value->type = JS_BOOLIAN;
	json_value->u.boolian = value;
	return json_value;
}

#ifdef ff
/**
 * Created  07/25/2015
 * @brief   creates new json_value_t structure
 * @param   
 * @return 0 - success,1 fail  
 */
json_value_t * json_object11()
{
	json_value_t *json_value= malloc(sizeof(json_value_t));
	json_value->type = JS_OBJECT;
	json_object_init ( &json_value->u.object);
	return json_value;
}

/**
 * Created  07/25/2015
 * @brief   creates new json_value_t structure
 * @param   
 * @return 0 - success,1 fail  
 */
json_value_t * json_array11()
{
	json_value_t *json_value= malloc(sizeof(json_value_t));
	json_value->type = JS_ARRAY;
	json_array_init ( &json_value->u.array);
	return json_value;
}
#endif

/**
 * Created  07/25/2015
 * @brief   inititiate new memory place for json_record
 * @param   
 * @return  
 */
void json_object_init(json_object_t *object)
{
	link_list_init (&object->json_objects);
}

/**
 * Created  07/25/2015
 * @brief   inititiate new memory place for json_record
 * @param   
 * @return  
 */
void json_array_init(json_array_t *array)
{
	link_list_init (&array->json_records);
}



/**
 * Created  07/25/2015
 * @brief   inititiate new memory place for json_record
 * @param   
 * @return  
 */
json_record_t *json_record_new()
{
	json_record_t *json_record = malloc(sizeof(json_record_t));
//	link_list_init (&json_record->json_record);
	return json_record;
}


/*
 * Created  07/25/2015
 * @brief   creates new jsoj  string object
 * @param   record_name: the nemae of the json record
 * @param   value: staring value
 * @return  0 - faile, pointer to initiated json_object_t at type JS_STRING
 */
json_record_t *json_record_new_string(char *record_name,char *value)
{
	/*
	 * Inittiates new json_record_t
	 * TODO: check for memory faile at json_string_new
	 */
	json_record_t *json_record = json_record_new();
	/*
	 * creates json string , 
	 * TODO: check for memory faile at json_string_new
	 */
	json_string_set(&json_record->name,record_name);


	/*
	 *set json value  as JS_STRING
	 */
//	json_record->js_type = JS_STRING;
	json_string_set (&json_record->value.u.string,value);
	return json_record;
}


/*
 * Created  07/25/2015
 * @brief   creates new jsoj  string object
 * @param   record_name: the nemae of the json record
 * @param   value: staring value
 * @return  0 - faile, pointer to initiated json_record_t at type JS_DOUBLE
 */
json_record_t *json_record_new_double(char *record_name,double value)
{
	/*
	 * Inittiates new json_record_t
	 * TODO: check for memory faile at json_string_new
	 */
	json_record_t *json_record = json_record_new();


	/*
	 * creates json string , 
	 * TODO: check for memory faile at json_string_new
	 */
	json_string_set(&json_record->name,record_name);
	//json_double_set(&json_record->value.u.real,value);

	
	/*
	 *set json value  as JS_DOUBLE
	 */
	json_record->value.type = JS_DOUBLE;
	json_record->value.u.real = value;
	return json_record;

}


/*
 * Created  07/25/2015
 * @brief   creates new jsoj  string object
 * @param   record_name: the nemae of the json record
 * @param   value: staring value
 * @return  0 - faile, pointer to initiated json_record_t at type JS_INTEGER
 */
json_record_t *json_record_new_integer(char *record_name,int value)
{
	/*
	 * Inittiates new json_record_t
	 * TODO: check for memory faile at json_string_new
	 */
	json_record_t *json_record = json_record_new();


	/*
	 * creates json string , 
	 * TODO: check for memory faile at json_string_new
	 */
//	json_string_init(&json_record->name,record_name);
	json_string_set(&json_record->name,record_name);


	/*
	 *set json value  as JS_INTEGER
	 */
	json_record->value.type = JS_INTEGER;
	json_record->value.u.integer = value;
	return json_record;


}


/*
 * Created  07/25/2015
 * @brief   creates new jsoj  string object
 * @param   record_name: the nemae of the json record
 * @param   value: staring value
 * @return  0 - faile, pointer to initiated json_record_t at type JS_BOOLIAN
 */
json_record_t *json_record_new_boolian(char *record_name,char value)
{
	/*
	 * Inittiates new json_record_t
	 * TODO: check for memory faile at json_string_new
	 */
	json_record_t *json_record = json_record_new();


	/*
	 * creates json string , 
	 * TODO: check for memory faile at json_string_new
	 */
	//json_string_init(&json_record->name,record_name);
	json_string_set(&json_record->name,record_name);


	
	/*
	 *set json value  as JS_INTEGER
	 */
	json_record->value.type = JS_BOOLIAN;
	json_record->value.u.boolian = value;
	return json_record;
}


/*
 * Created  07/25/2015
 * @brief   creates new jsoj  string object
 * @param   record_name: the nemae of the json record
 * @param   value: staring value
 * @return  0 - faile, pointer to initiated json_record_t at type JS_ARRAY
 */
struct json_record_s *json_record_new_array(char *record_name)
{
	/*
	 * Inittiates new json_record_t
	 * TODO: check for memory faile at json_string_new
	 */
	json_record_t *json_record = json_record_new();


	/*
	 * creates json string , 
	 * TODO: check for memory faile at json_string_new
	 */
	json_string_set(&json_record->name,record_name);
//	json_record_set(&json_record->value.u.array);
	
	/*
	 *set json value  as JS_ARRAY
	 */
	json_record->value.type = JS_ARRAY;
	link_list_init (&json_record->value.u.array.json_records);
	return json_record;
}





/*
 * Created  07/25/2015
 * @brief   creates new jsoj  string object
 * @param   record_name: the nemae of the json record
 * @param   value: staring value
 * @return  0 - faile, pointer to initiated json_record_t at type JS_OBJECT
 */
json_record_t *json_record_new_object(char *record_name)
{
	/*
	 * Inittiates new json_record_t
	 * TODO: check for memory faile at json_string_new
	 */
	json_record_t *json_record = json_record_new();


	/*
	 * creates json string , 
	 * TODO: check for memory faile at json_string_new
	 */
	//json_string_init(&json_record->name,record_name);
	json_string_set(&json_record->name,record_name);

//	json_object_set(&json_record->value.u.object);
	
	
	/*
	 *set json value  as JS_OBJECT
	 */
	json_record->value.type = JS_OBJECT;
	link_list_init(&json_record->value.u.object.json_objects);
	return json_record;
}


/**
 * Created  08/15/2015
 * @brief   return name of json_record
 * @param   
 * @return  
 */
json_string_t * json_record_get_name(struct json_record_s *json_record)
{
	return &json_record->name;
}


/**
 * Created  08/15/2015
 * @brief   return array elemnet from json_record_t
 * @param   
 * @return  0 - if obejct is not record other wise  pointer to json_array_s
 */
struct json_array_s * json_record_get_array(struct json_record_s *json_record)
{
	if (json_record->value.type==JS_ARRAY)
		return &json_record->value.u.array;
	else 
		return NULL;
}
/**
 * Created  08/15/2015
 * @brief   return object elemnet from json_record_t
 * @param   
 * @return  0 - if obejct is not record other wise  pointer to json_array_s
 */
struct json_object_s * json_record_get_object(struct json_record_s *json_record)
{
	if (json_record->value.type==JS_OBJECT)
		return &json_record->value.u.object;
	else 
		return NULL;
}


/**
 * Created  07/25/2015
 * @brief   inititiate new memory place for json_object_t
 * @param   
 * @return  
 */
json_object_t *json_object_new()
{
	json_object_t *json_object = malloc(sizeof(json_object_t));
	link_list_init (&json_object->json_objects);
	return json_object;
}



/**
 * Created  07/25/2015
 * @brief   adds string field to json object
 * @param   json_obj: initiated json_object_t
 * @param   record_name: record field name
 * @param   value: value of type JS_STRING
 * @return  0 - succsess,1 failed
 */
int json_object_add_new_string(struct json_object_s *json_obj, char *record_name,char *value)
{
	json_record_t *json_record  = json_record_new_string( record_name, value);
	if (!json_record)
		return 1;
	link_list_add_head(&json_obj->json_objects,json_record);
	return 0;	
}

/**
 * Created  07/25/2015
 * @brief   adds string field to json object
 * @param   json_obj: initiated json_object_t
 * @param   record_name: record field name
 * @param   value: value of type JS_DOUBLE
 * @return  0 - succsess,1 failed
 */
int json_object_add_new_double(struct json_object_s *json_obj,char *record_name,double value)
{
	json_record_t *json_record  = json_record_new_double( record_name, value);
	if (!json_record)
		return 1;
	link_list_add_head(&json_obj->json_objects,json_record);
	return 0;	
}

/**
 * Created  07/25/2015
 * @brief   adds string field to json object
 * @param   json_obj: initiated json_object_t
 * @param   record_name: record field name
 * @param   value: value of type JS_INTEGER
 * @return  0 - succsess,1 failed
 */
int json_object_add_new_integer(struct json_object_s *json_obj,char *record_name,int value)
{
	json_record_t *json_record  = json_record_new_integer( record_name, value);
	if (!json_record)
		return 1;
	link_list_add_head(&json_obj->json_objects,json_record);
	return 0;	

}

/**
 * Created  07/25/2015
 * @brief   adds string field to json object
 * @param   json_obj: initiated json_object_t
 * @param   record_name: record field name
 * @param   value: value of type JS_BOOLIAN
 * @return  0 - succsess,1 failed
 */
int json_object_add_new_boolian(struct json_object_s *json_obj,char *record_name,char value)
{
	json_record_t *json_record  = json_record_new_boolian( record_name, value);
	if (!json_record)
		return 1;
	link_list_add_head(&json_obj->json_objects,json_record);
	return 0;	
}

/**
 * Created  07/25/2015
 * @brief   adds string field to json object
 * @param   json_obj: initiated json_object_t
 * @param   record_name: record field name
 * @param   value: value of type JS_ARRAY
 * @return  0 - succsess,1 failed
 */
int json_object_add_new_array(struct json_object_s *json_obj,char *record_name,struct json_array_s *value)
{
	json_record_t *json_record  = json_record_new_array( record_name);
	if (!json_record)
		return 1;
	link_list_add_head(&json_obj->json_objects,json_record);
	return 0;
}

/**
 * Created  07/25/2015
 * @brief   adds string field to json object
 * @param   json_obj: initiated json_object_t
 * @param   record_name: record field name
 * @param   value: value of type JS_OBJECT
 * @return  0 - succsess,1 failed
 */
int json_object_add_new_object(struct json_object_s *json_obj,char *record_name,struct json_object_s *value)
{
	json_record_t *json_record  = json_record_new_object( record_name);
	if (!json_record)
		return 1;
	link_list_add_head(&json_obj->json_objects,json_record);
	return 0;
}


/**
 * Created  08/01/2015
 * @brief   adds existing json_record to json_object
 * TODO : add outof memory test
 * @param   
 * @return 0 - success, 1-faile 
 */
struct json_object_s * json_object_add_record(struct json_object_s *json_obj,json_record_t  *value)
{	link_list_add_head(&json_obj->json_objects,value);
	return json_obj;
}



/**
 * Created  07/25/2015
 * @brief   creates new json_array_t structure
 * @param   
 * @return 0 - success,1 fail  
 */
json_array_t *json_array_new()
{
	json_array_t *json_string = malloc(sizeof(json_string_t));
	return json_string;
}

/**
 * Created  07/25/2015
 * @brief   adds string field to json object
 * @param   json_array: initiated json_array_t
 * @param   record_name: record field name
 * @param   value: value of type JS_STRING
 * @return  0 - succsess,1 failed
 */
int json_array_add_new_string(struct json_array_s *json_array, char *value)
{
	json_record_t *string   = json_record_new_string( "", value);
	if (!string)
		return 1;
	link_list_add_head(&json_array->json_records,string);
	return 0;	
}

/**
 * Created  07/25/2015
 * @brief   adds string field to json object
 * @param   json_array: initiated json_array_t
 * @param   record_name: record field name
 * @param   value: value of type JS_INTEGER
 * @return  0 - succsess,1 failed
 */
int json_array_add_new_integer(struct json_array_s *json_obj,int value)
{
	json_record_t *integer   = json_record_new_integer( "", value);
	if (!integer)
		return 1;
	link_list_add_head(&json_array->json_records,integer);
	return 0;	
}

/**
 * Created  07/25/2015
 * @brief   adds string field to json object
 * @param   json_array: initiated json_array_t
 * @param   record_name: record field name
 * @param   value: value of type JS_DOUBLE
 * @return  0 - succsess,1 failed
 */
int json_array_add_new_double(struct json_array_s *json_obj,double value)
{
	json_record_t *real   = json_record_new_double( "", value);
	if (!real)
		return 1;
	link_list_add_head(&json_array->json_records,real);
	return 0;	
}


/**
 * Created  07/25/2015
 * @brief   adds string field to json object
 * @param   json_array: initiated json_array_t
 * @param   record_name: record field name
 * @param   value: value of type JS_BOOLIAN
 * @return  0 - succsess,1 failed
 */
int json_array_add_new_boolian(struct json_array_s * json_array,  char value)
{
	json_record_t *boolian   = json_record_new_boolian( "", value);
	if (!boolian)
		return 1;
	link_list_add_head(&json_array->json_records,boolian);
	return 0;	
}

/**
 * Created  07/25/2015
 * @brief   adds string field to json object
 * @param   json_array: initiated json_array_t
 * @param   record_name: record field name
 * @param   value: value of type JS_ARRAY
 * @return  0 - succsess,1 failed
 */
int json_array_add_new_array(struct json_array_s *json_obj,struct json_array_s *value)
{
	json_record_t *array   = json_record_new_array( "");
	if (!array)
		return 1;
	link_list_add_head(&json_array->json_records,array);
	return 0;	
}

/**
 * Created  07/25/2015
 * @brief   adds string field to json object
 * @param   json_array: initiated json_array_t
 * @param   record_name: record field name
 * @param   value: value of type JS_OBJECT
 * @return  0 - succsess,1 failed
 */
int json_array_add_new_object(struct json_array_s *json_obj,struct json_object_s *value)
{
	json_record_t *object   = json_record_new_object( "");
	if (!object)
		return 1;
	link_list_add_head(&json_array->json_records,object);
	return 0;
}

/**
 * Created  08/01/2015
 * @brief   adds existing json_record to json_object
 * TODO : add outof memory test
 * @param   
 * @return 0 - success, 1-faile 
 */
struct json_array_s * json_array_add_record(struct json_array_s *json_obj,json_record_t  *value)
{	link_list_add_head(&json_array->json_records,value);
	return json_obj;
}



/**
 * Created  07/25/2015
 * @brief   collect the yacc error
 *          TODO: collect all parameters to error struct
 * @param   col_out: output cloumn 
 * @parm    line_out:  output  line
 * @param   msg: output massage
 * @return  void 
 */
void json_get_error_msg(int *col_out,int *line_out,char **msg)
{
	msg[0] = yytext;
	*col_out = col;
	*line_out = line;
}


/**
 * Created  07/25/2015
 * @brief   main json parser function
 * @param   text: string contains json text
 * @return  0 - succes, pointer to json object - succes
 */
json_object_t * json_parser(const char *text)
{
	struct yy_buffer_state * my_string_buffer;
	/* 
	 * initiate stacks and json object 
	 * TODO: add no memory exception
	 */
	stack_new(&stack_array,1024*100);
	stack_new(&stack_object,1024*100);
	json_object = json_object_new();
	json_array = json_array_new();

	/* 
	 * set the pointer for the folloinwg variables in the file yjson.y since, in both files
	 * the current one and yjson.y the following valiables to set are declared as static
	 */
	set_stack_array(stack_array);
	set_stack_object(stack_object);
	set_json_object(json_object);
	set_json_array(json_array);


	// run flex code from file
	my_string_buffer  = yy_scan_string(text);

	yy_switch_to_buffer( my_string_buffer ); // switch flex to the buffer we just created
	yyparse(); 
//	yy_delete_buffer(my_string_buffer );

	

	// pop top module from stack andfree it
	stack_free(stack_array);
	stack_free(stack_object);
	return json_object;
}

