//task 2c

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
/* the states in the printf state-machine */
enum printf_state {
  st_printf_init,
  st_printf_percent
  //,st_printf_octal2,
  //st_printf_octal3
};


#define MAX_NUMBER_LENGTH 64
#define is_octal_char(ch) ('0' <= (ch) && (ch) <= '7')

int toy_printf(char *fs, ...);

const char *digit = "0123456789abcdef";
const char *DIGIT = "0123456789ABCDEF";

int print_int_helper(unsigned n, int radix, const char *digit) {
  int result;

  if (n < radix) {
    putchar(digit[n]);
    return 1;
  }
  else {
    result = print_int_helper(n / radix, radix, digit);
    putchar(digit[n % radix]);
    return 1 + result;
  }
}

int print_int(unsigned n, int radix, const char * digit) {
  if (radix < 2 || radix > 16) {
    toy_printf("Radix must be in [2..16]: Not %d\n", radix);
    exit(-1);
  }

  if (n > 0) {
    return print_int_helper(n, radix, digit);
  }
  if (n == 0) {
    putchar('0');
    return 1;
  }
  else {
    putchar('-');
    return 1 + print_int_helper(-n, radix, digit);
  }
}

void completeWidth(char toAdd, int width, int len){
    for(int j=0;j<width-len;j++) 
        putchar(toAdd);
}

/* SUPPORTED:
 *   %b, %d, %o, %x, %X -- 
 *     integers in binary, decimal, octal, hex, and HEX
 *   %s -- strings
 */

typedef struct state_args{
    char* fs;
    int int_value;
    char *string_value;
    char char_value;
    int arr_size;
    int* int_arr;
    char* char_arr;
    char** str_arr;
    char temp;
    int checkD;
    int width;
    int len;
    int digits_num;
    int caseleft;
    int casezero;
}state_args;

typedef struct  {
  int printed_chars;
  enum printf_state new_state;
  int state_number;
}state_result;

//sets the fields of the struct
void set_state_result(state_result* self, int chars_printed,enum printf_state new_state,int state_num){
    self -> printed_chars = chars_printed;
    self -> new_state = new_state;
    self -> state_number = state_num;
}


void init_state_args(state_args* self){
    self -> int_value = 0;
    self -> width = 0;
    self -> digits_num = 0;
    self -> caseleft = 0;
    self -> casezero = 0;
}

/*  state = st_printf_init */

// case '%'
state_result percent_case_init(va_list args, state_result state_res, state_args* state_args){
    state_result toreturn;
    set_state_result(&toreturn, state_res.printed_chars, st_printf_percent,1);
    return toreturn;
}

//default case
state_result def_case_init(va_list args, state_result state_res, state_args* state_args){
    state_result toreturn;
    putchar(*(state_args->fs));
    set_state_result(&toreturn, state_res.printed_chars+1, st_printf_init,0);
    return toreturn;
}

//function from task0
state_result init_state_handler(va_list args, state_result state_res, state_args* state_ar){
    state_result toreturn;
    int i;
    state_result (*func_array[128]) (va_list args, state_result state_res, state_args* state_ar);
    for(i = 0; i<128; i++) func_array[i] = def_case_init;
    func_array['%'] = percent_case_init;
    toreturn = (func_array[(int)(*(state_ar -> fs))])(args,state_res,state_ar);
    return toreturn;
}

/*  state = st_printf_percent */

//case %
state_result percent_case(va_list args, state_result state_res, state_args* state_args){
    state_result toreturn;
    int temp_chars_printed = state_res.printed_chars;
    putchar('%');
    set_state_result(&toreturn, temp_chars_printed+1, st_printf_init,0);
    return toreturn;
}

//case d
state_result d_case(va_list args, state_result state_res, state_args* state_args){
    int num_of_chars_added;
    state_result toreturn;
    int temp_chars_printed = state_res.printed_chars;
    state_args ->checkD = va_arg(args, int);
        if(state_args ->checkD>=0) {
            state_args ->int_value = state_args ->checkD;
        }
        else {
            state_args ->int_value = -(state_args ->checkD);
            putchar('-');
            temp_chars_printed++;
        }
        
        if(state_args ->checkD<0) (state_args ->digits_num)++;
        while(state_args ->checkD!=0){
            state_args ->checkD = (state_args ->checkD)/10;
            (state_args ->digits_num)++;
        }
        
        if(state_args ->digits_num <= state_args ->width && state_args ->casezero) completeWidth('0',state_args ->width, state_args ->digits_num);
        num_of_chars_added = print_int(state_args ->int_value, 10, digit);
        set_state_result(&toreturn,temp_chars_printed + num_of_chars_added , st_printf_init,0);
        return toreturn;
}

//case u
state_result u_case(va_list args, state_result state_res, state_args* state_args){
    int num_of_chars_added;
    state_result toreturn;
    int temp_chars_printed = state_res.printed_chars;
    state_args ->int_value = va_arg(args, int);
    num_of_chars_added = print_int(state_args ->int_value, 10, digit);
    set_state_result(&toreturn,temp_chars_printed + num_of_chars_added , st_printf_init,0);
    return toreturn;
}

//case b
state_result b_case(va_list args, state_result state_res, state_args* state_args){
    int num_of_chars_added;
    state_result toreturn;
    int temp_chars_printed = state_res.printed_chars;
    state_args ->int_value = va_arg(args, int);
    num_of_chars_added = print_int(state_args ->int_value, 2, digit);
    set_state_result(&toreturn,temp_chars_printed + num_of_chars_added , st_printf_init,0);
    return toreturn;
}

//case o
state_result o_case(va_list args, state_result state_res, state_args* state_args){
    int num_of_chars_added;
    state_result toreturn;
    int temp_chars_printed = state_res.printed_chars;
    state_args ->int_value = va_arg(args, int);
    num_of_chars_added = print_int(state_args ->int_value, 8, digit);
    set_state_result(&toreturn,temp_chars_printed + num_of_chars_added , st_printf_init,0);
    return toreturn;
}

//case x
state_result x_case(va_list args, state_result state_res, state_args* state_args){
    int num_of_chars_added;
    state_result toreturn;
    int temp_chars_printed = state_res.printed_chars;
    state_args ->int_value = va_arg(args, int);
    num_of_chars_added = print_int(state_args ->int_value, 16, digit);
    set_state_result(&toreturn,temp_chars_printed + num_of_chars_added , st_printf_init,0);
    return toreturn;
}

//case X
state_result X_case(va_list args, state_result state_res, state_args* state_args){
    int num_of_chars_added;
    state_result toreturn;
    int temp_chars_printed = state_res.printed_chars;
    state_args ->int_value = va_arg(args, int);
    num_of_chars_added = print_int(state_args ->int_value, 10, DIGIT);
    set_state_result(&toreturn,temp_chars_printed + num_of_chars_added , st_printf_init,0);
    return toreturn;
}
    
//case s
state_result s_case(va_list args, state_result state_res, state_args* state_args){
    state_result toreturn;
    int temp_chars_printed = state_res.printed_chars;
    state_args ->string_value = va_arg(args, char *);
    state_args ->len = strlen(state_args ->string_value);
    if(state_args ->len<state_args ->width && state_args ->caseleft){
        completeWidth(' ', state_args ->width, state_args ->len);
    }
    while(*(state_args ->string_value)){
            temp_chars_printed++;
            putchar(*(state_args ->string_value));
            (state_args ->string_value)++;
    }
    if(state_args ->len<state_args ->width && !(state_args ->caseleft)){
        completeWidth(' ', state_args ->width, state_args ->len);
        putchar('#');
    }
    set_state_result(&toreturn,temp_chars_printed, st_printf_init,0);
    return toreturn;
}


//case c
state_result c_case(va_list args, state_result state_res, state_args* state_args){
    state_result toreturn;
    int temp_chars_printed = state_res.printed_chars;
    state_args ->char_value = (char)va_arg(args, int);
    putchar(state_args ->char_value);
    set_state_result(&toreturn,temp_chars_printed + 1 , st_printf_init,0);
    return toreturn;
}


//case A
//case As
state_result As_case(va_list args, state_result state_res, state_args* state_args){
    state_result toreturn;
    int temp_chars_printed = state_res.printed_chars;
    state_args ->temp = *((state_args ->fs)+1);
    state_args ->str_arr = va_arg(args, char **);
    state_args ->arr_size = va_arg(args, int);
    putchar('{');
    ++temp_chars_printed;
    for(int i=0;i<state_args ->arr_size;i++){
        if(i==(state_args ->arr_size)-1) temp_chars_printed +=toy_printf("\"%s\"}",state_args ->str_arr[i]);
            else temp_chars_printed +=toy_printf("\"%s\" ,",state_args ->str_arr[i]);
    }
    ++(state_args ->fs);
    set_state_result(&toreturn, temp_chars_printed, st_printf_init,0);
    return toreturn;
    
}

//case Ac
state_result Ac_case(va_list args, state_result state_res, state_args* state_args){
    state_result toreturn;
    int temp_chars_printed = state_res.printed_chars;
    state_args ->temp = *((state_args ->fs)+1);
    
    state_args ->char_arr = va_arg(args, char *);
    state_args ->arr_size = va_arg(args, int);
    putchar('{');
    ++temp_chars_printed;
    for(int i=0;i<state_args ->arr_size;i++){
        if(i==(state_args ->arr_size)-1) temp_chars_printed +=toy_printf("%c}",state_args ->char_arr[i]);
            else temp_chars_printed +=toy_printf("%c, ",state_args ->char_arr[i]);
    }
    
    ++(state_args ->fs);
    set_state_result(&toreturn, temp_chars_printed, st_printf_init,0);
    return toreturn;
}

//case Ad
state_result Ad_case(va_list args, state_result state_res, state_args* state_args){
    state_result toreturn;
    int temp_chars_printed = state_res.printed_chars;
    state_args ->temp = *((state_args ->fs)+1);
    
    state_args ->int_arr = va_arg(args, int *);
    state_args ->arr_size = va_arg(args, int);
    putchar('{');
    ++temp_chars_printed;
    for(int i=0;i<state_args ->arr_size;i++){
        if(i==(state_args ->arr_size-1)) temp_chars_printed +=toy_printf("%d}",state_args ->int_arr[i]);
            else temp_chars_printed +=toy_printf("%d, ",state_args ->int_arr[i]);
    }
    
    ++(state_args ->fs);
    set_state_result(&toreturn, temp_chars_printed, st_printf_init,0);
    return toreturn;
}

//case Au
state_result Au_case(va_list args, state_result state_res, state_args* state_args){
    state_result toreturn;
    int temp_chars_printed = state_res.printed_chars;
    state_args ->temp = *((state_args ->fs)+1);
    
    state_args ->int_arr = va_arg(args, int *);
    state_args ->arr_size = va_arg(args, int);
    putchar('{');
    ++temp_chars_printed;
    for(int i=0;i<state_args ->arr_size;i++){
        if(i==(state_args ->arr_size)-1) temp_chars_printed +=toy_printf("%u}",state_args ->int_arr[i]);
            else temp_chars_printed +=toy_printf("%u, ",state_args ->int_arr[i]);
    }
    
    ++(state_args ->fs);
    set_state_result(&toreturn, temp_chars_printed, st_printf_init,0);
    return toreturn;
}

//case Ab
state_result Ab_case(va_list args, state_result state_res, state_args* state_args){
    state_result toreturn;
    int temp_chars_printed = state_res.printed_chars;
    state_args ->temp = *((state_args ->fs)+1);
    
    state_args ->int_arr = va_arg(args, int *);
    state_args ->arr_size = va_arg(args, int);
    putchar('{');
    ++temp_chars_printed;
    for(int i=0;i<state_args ->arr_size;i++){
        if(i==(state_args ->arr_size)-1) temp_chars_printed +=toy_printf("%b}",state_args ->int_arr[i]);
            else temp_chars_printed +=toy_printf("%b, ",state_args ->int_arr[i]);
    }
    
    ++(state_args ->fs);
    set_state_result(&toreturn, temp_chars_printed, st_printf_init,0);
    return toreturn;
}

//case Ao
state_result Ao_case(va_list args, state_result state_res, state_args* state_args){
    state_result toreturn;
    int temp_chars_printed = state_res.printed_chars;
    state_args ->temp = *((state_args ->fs)+1);
    
    state_args ->int_arr = va_arg(args, int *);
    state_args ->arr_size = va_arg(args, int);
    putchar('{');
    ++temp_chars_printed;
    for(int i=0;i<state_args ->arr_size;i++){
        if(i==(state_args ->arr_size-1)) temp_chars_printed +=toy_printf("%o}",state_args ->int_arr[i]);
            else temp_chars_printed +=toy_printf("%o, ",state_args ->int_arr[i]);
    }
    
    ++(state_args ->fs);
    set_state_result(&toreturn, temp_chars_printed, st_printf_init,0);
    return toreturn;
}

//case Ax
state_result Ax_case(va_list args, state_result state_res, state_args* state_args){
    state_result toreturn;
    int temp_chars_printed = state_res.printed_chars;
    state_args ->temp = *((state_args ->fs)+1);
    
    state_args ->int_arr = va_arg(args, int *);
    state_args ->arr_size = va_arg(args, int);
    putchar('{');
    ++temp_chars_printed;
    for(int i=0;i<state_args ->arr_size;i++){
        if(i==(state_args ->arr_size-1)) temp_chars_printed +=toy_printf("%x}",state_args ->int_arr[i]);
            else temp_chars_printed +=toy_printf("%x, ",state_args ->int_arr[i]);
    }
    
    ++(state_args ->fs);
    set_state_result(&toreturn, temp_chars_printed, st_printf_init,0);
    return toreturn;
}

//case AX
state_result AX_case(va_list args, state_result state_res, state_args* state_args){
    state_result toreturn;
    int temp_chars_printed = state_res.printed_chars;
    state_args ->temp = *((state_args ->fs)+1);
    
    state_args ->int_arr = va_arg(args, int *);
    state_args ->arr_size = va_arg(args, int);
    putchar('{');
    ++temp_chars_printed;
    for(int i=0;i<state_args ->arr_size;i++){
        if(i==(state_args ->arr_size-1)) temp_chars_printed +=toy_printf("%X}",state_args ->int_arr[i]);
            else temp_chars_printed +=toy_printf("%X, ",state_args ->int_arr[i]);
    }
    
    ++(state_args ->fs);
    set_state_result(&toreturn, temp_chars_printed, st_printf_init,0);
    return toreturn;
}

//case Adef
state_result Adef_case(va_list args, state_result state_res, state_args* state_args){
    toy_printf("Unhandled format %%%c...\n", *(state_args ->fs));
    exit(-1);
}

state_result A_case(va_list args, state_result state_res, state_args* state_ar){
    state_result toreturn;
    int i;
    state_ar ->temp = *((state_ar ->fs)+1);
    state_result (*func_array[128]) (va_list args, state_result state_res, state_args* state_ar);
    for(i=0; i<128; i++) func_array[i] = Adef_case;
    func_array['d'] = Ad_case;
    func_array['u'] = Au_case;
    func_array['b'] = Ab_case;
    func_array['o'] = Ao_case;
    func_array['x'] = Ax_case;
    func_array['X'] = AX_case;
    func_array['s'] = As_case;
    func_array['c'] = Ac_case;
    toreturn = (func_array[(int)(state_ar ->temp)])(args,state_res,state_ar);
    return toreturn;
}

//case 1-9
state_result num_case(va_list args, state_result state_res, state_args* state_args){
    state_result toreturn;
    int temp_chars_printed = state_res.printed_chars;
    while('0'<=*(state_args ->fs) && *(state_args ->fs)<='9'){
        state_args ->width = (*(state_args ->fs) - '0') + (state_args ->width)*10;
        ++(state_args ->fs);
    }
    (state_args ->fs)--;
    set_state_result(&toreturn, temp_chars_printed, st_printf_percent,1);
    return toreturn;
}

//case 0
state_result zero_case(va_list args, state_result state_res, state_args* state_args){
    state_result toreturn;
    int temp_chars_printed = state_res.printed_chars;
    state_args ->casezero = 1;
    set_state_result(&toreturn, temp_chars_printed, st_printf_percent,1);
    return toreturn;
   
}

//case -
state_result left_case(va_list args, state_result state_res, state_args* state_args){
    state_result toreturn;
    int temp_chars_printed = state_res.printed_chars;
    state_args ->caseleft = 1;
    set_state_result(&toreturn, temp_chars_printed, st_printf_percent,1);
    return toreturn;
}

//default
state_result def_case(va_list args, state_result state_res, state_args* state_args){
    toy_printf("Unhandled format %%%c...\n", *(state_args ->fs));
    exit(-1);
}
   
    
state_result percent_state_handler(va_list args, state_result state_res, state_args* state_ar){
    state_result toreturn;
    int i;
    state_result (*func_array[128]) (va_list args, state_result state_res, state_args* state_ar);
    for(i=0; i<128; i++) func_array[i] = def_case;
    func_array['0'] = zero_case;
    for(i='1'; i<='9'; i++) func_array[i] = num_case;
    func_array['%'] = percent_case;
    func_array['-'] = left_case;
    func_array['d'] = d_case;
    func_array['u'] = u_case;
    func_array['b'] = b_case;
    func_array['o'] = o_case;
    func_array['x'] = x_case;
    func_array['X'] = X_case;
    func_array['s'] = s_case;
    func_array['c'] = c_case;
    func_array['A'] = A_case;
    toreturn = (func_array[(int)(*(state_ar -> fs))])(args,state_res,state_ar);
    return toreturn;
}
    
int toy_printf(char *fs,...) {
  state_args  sa;
  state_args * state_args_ptr = &sa;
  state_result state_res;
  va_list args;
  state_result (*func_array[2]) (va_list args, state_result state_res, state_args* state_args);
  
  state_args_ptr -> fs = fs;
  init_state_args(state_args_ptr);
  va_start(args, fs);
  set_state_result(&state_res, 0, st_printf_init,0);
  
  func_array[0] = init_state_handler;
  func_array[1] = percent_state_handler;
  

  for (; *(state_args_ptr->fs) != '\0'; ++(state_args_ptr->fs)) {
      state_res = (*func_array[state_res.state_number])(args, state_res, state_args_ptr);
  }

  va_end(args);

  return state_res.printed_chars;
}


