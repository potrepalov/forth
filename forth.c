// Copyleft  by Potrepalov I.S.
// e-mail: potrepalov@list.ru

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <setjmp.h>
#include <limits.h>


#define DEBUG 0


#define ADD_STACK_SIZE     	400
#define ADD_DATA_SIZE      	655200
#define RES_DATA_SIZE      	5048
#define NUMBER_OF_WORDLISTS	150
#define align( a )         	(   (~((CELL)(sizeof(CELL)-1))) \
                           	  & (CELL)((uchar*)(a)+sizeof(CELL)-1) )
#define QUIT_ior           	-56
#define FIB_LEN        	   	256


typedef unsigned long int	CELL; 	// for sizeof(CELL) and most functions
typedef long int         	sCELL;	// for signed arithmetics
typedef unsigned long int	uCELL;	// for unsigned arithmetics

typedef unsigned char    	uchar;	// all chars are unsigned

// range of double numbers is equal with range of single numbers
// first cell of double number is 0 if number >=0, and -1 if number <0
// (see dsign())


//#define F_ATTR      	__attribute__((regparm(1)))
#define F_ATTR
#define F_DECLARE(n)	F_ATTR void n( void ) /* see execute() */
#define F_USE(n)    	n ()


CELL context[16];
uchar pad[520];
uchar _hld[260];
uchar tib[260];


#define _TRUE 	(~0)
#define _FALSE	0


struct _env_strings {
	char *name;
	CELL val;
};

struct _env_strings env_list[] = {
	{ "FLOORED", (10 / -7 == -2) ? _TRUE : _FALSE },
		// FLOORED must be first! [see main(), fm_slash_mod]
	{ "/COUNTED-STRING", UCHAR_MAX },
	{ "/HOLD", sizeof(_hld)/sizeof(_hld[0]) - 1 },
	{ "/PAD", (sizeof(pad)/sizeof(pad[0]))/2 - 1 },
	{ "ADDRESS-UNIT-BITS", CHAR_BIT },
	{ "CORE", _TRUE },
	{ "CORE-EXT", _TRUE },
	{ "MAX-CHAR", UCHAR_MAX },
	{ "MAX-N", LONG_MAX },
	{ "MAX-U", ULONG_MAX },
	{ "EXCEPTION", _TRUE },
	{ "EXCEPTION-EXT", _TRUE },
	{ "STRING", _TRUE },
	{ "MEMORY-ALLOC", _TRUE },
	{ "WORDLISTS", sizeof(context)/sizeof(context[0]) },
	{ "SEARCH-ORDER", _TRUE },
	{ "SEARCH-ORDER-EXT", _TRUE },
	{ "DOUBLE", _TRUE },
	{ "DOUBLE-EXT", _TRUE },
	{ "FILE", _TRUE },
	{ "FILE-EXT", _TRUE },
	{ "BLOCK", _TRUE },

	// MAX-D and MAX-UD see in  environment_q (core.c)
	{ 0, 0 }
};


#define h( n )        	F_DECLARE( n );
#define i( n )        	F_DECLARE( n );
#define ii( n, f )    	F_DECLARE( f );
#define if( n, fl )   	F_DECLARE( n );
#define iif(n, f, fl )	F_DECLARE( f );
#define v( n, var )
#define c( n, c )
#define q( n, q )

#include "words.inc"

i( paren )
i( comma )
i( c_comma )
i( bracket_tick )
i( tick )
i( backslash )
i( dot_quote )
i( dot_paren )
i( abort_quote )


#undef h
#undef i
#undef ii
#undef if
#undef iif
#undef v
#undef c
#undef q



typedef F_ATTR void (*vfp)(void);

struct _xt {
	vfp f;
	CELL pfa;
	int flags;
#define _NONE     	1
#define _IMMEDIATE	2
#define _SMUDGE   	4

	uchar name[32];
};

typedef struct _xt * xt;
extern struct _xt forth_wl[];
extern struct _xt hide_wl[];
extern struct _xt min_wl[];



void Throw( CELL ior );
xt get_xt( int wid );
void NextWord( void ); // "<spaces>word<space>"  -- a u


struct _stack {
	unsigned int	occupied;	// occupied cells in stack
	unsigned int	empty;   	// empty cells in stack
	CELL        	*top;    	// pointer to TOS; *top is occupied;
	            	         	// *(top+1) is empty (if it is exist)
	CELL        	*v;      	// vector of cells
};

struct _stack
	dstack = { 0, 0, NULL, NULL },
	rstack = { 0, 0, NULL, NULL };

void no_memory( void )
{
	Throw( ENOMEM );
}


void s_overflow( struct _stack *s, unsigned int cells, int ior )
{
	CELL *p;

	p = realloc( s->v, (s->occupied + cells + ADD_STACK_SIZE) * sizeof(CELL) );
	if ( !p )
		Throw( ior );
	s->v = p;
	s->top = p + s->occupied;
	s->empty += (cells + ADD_STACK_SIZE);
}

#define cds( o, e )  	do { if ( dstack.occupied < (o) ) Throw(-4); \
                     	     if ( dstack.empty < (e) ) \
                     	        	s_overflow( &(dstack), (e), -3 ); \
                     	} while (0)

#define crs( o, e )  	do { if ( rstack.occupied < (o) ) Throw(-6); \
                     	     if ( rstack.empty < (e) ) \
                     	        	s_overflow( &(rstack), (e), -5 ); \
                     	} while (0)

#define __pop()     	(-- dstack.occupied, ++ dstack.empty, \
                   		*(dstack.top --))
#define __rpop()   	(-- rstack.occupied, ++ rstack.empty, \
                   		*(rstack.top --))
#define __push(w)  	(++ dstack.occupied, -- dstack.empty, \
                   		*(++ dstack.top) = (w))
#define __rpush(w) 	(++ rstack.occupied, -- rstack.empty, \
                   		*(++ rstack.top) = (w))

#define __get(i)   	(*(dstack.top - (i)))
#define __rget(i)  	(*(rstack.top - (i)))
#define __put(i,w) 	*(dstack.top - (i)) = (w)
#define __rput(i,w)	*(rstack.top - (i)) = (w)
#define __ndrop(i) 	(dstack.occupied -= (i), dstack.empty += (i), dstack.top -= (i))


CELL pop()
{
	cds( 1, 0 );
	return __pop();
}

CELL rpop()
{
	crs( 1, 0 );
	return __rpop();
}

void push( CELL w )
{
	cds( 0, 1 );
	__push( w );
}

void rpush( CELL w )
{
	crs( 0, 1 );
	__rpush( w );
}

CELL get( unsigned int z )
{
	cds( (z+1), 0 );
	return __get(z);
}

CELL rget( unsigned int z )
{
	crs( (z+1), 0 );
	return __rget(z);
}

void put( unsigned int z, CELL w )
{
	cds( (z+1), 0 );
	__put( z, w );
}

void rput( unsigned int z, CELL w )
{
	crs( (z+1), 0 );
	__rput( z, w );
}




/* Variables */

xt last_xt = 0;
uchar *hld;

uCELL base = 10;
CELL state = 0;
sCELL _source_id = 0;
CELL current;
uCELL number_tib = 0;
uCELL to_in = 0;
CELL blk = 0;
CELL span = 0;

uchar *source_a = NULL;
uCELL source_u = 0;


// -------------------
/* Code interpreter */

xt  	*IP = 0;            	// instruction pointer
uchar	*abort_text = NULL; 	// text for ABORT"
uCELL	abort_text_len = 0;
uchar	*dp;               	// data pointer for HERE
uchar	*last_word;        	// think of:  ' asdfghjkl
uCELL   last_word_len;     	// last_word === "asdfghjkl"; last_inter_word === "'"
uchar	*last_inter_word;
uCELL   last_inter_word_len;

xt run_xt = 0;

void execute( xt x )
{
#if DEBUG
	if ( x ) {
		printf( "\nexecute: %s\n", x->name );
	} else {
		printf( "\nexecute with 0\n" );
		return;
	}
	fflush( NULL );
#endif

	if ( x && x->f )
	{
		run_xt = x; // sometime  run_xt->pfa  used (see doCOLON)
		(x->f) ();  // All internal functions are `void func( void )'
		            // (see `#define F_DECLARE( n )' above)
	}
}

F_ATTR void doCOLON( void )
{
#if DEBUG
	printf( "Do Colon (IP=%d)\n", (int)IP );
#endif
	if ( !run_xt->pfa )
	{
#if DEBUG
	printf( "Do Colon: pfa == 0!!!\n" );
#endif
		return;
	}

	rpush( (CELL) IP );
	IP = (xt*) (run_xt->pfa);
	do
	{
		xt x = *(IP++);
		execute( x ); // change IP (in exit() or doBRANCH() for ex.)
	} while ( IP );
	IP = (xt*) rpop();
}

F_ATTR void doDOES( void )
{
	push( run_xt->pfa );
	// doCOLON( * (((CELL*)(run_xt->pfa)) - 1) );
	rpush( (CELL) IP );
	IP = (xt*) ( * (((CELL*)(run_xt->pfa)) - 1) );
	do
	{
		xt x = *(IP++);
		execute( x ); // change IP (in exit() or doBRANCH() for ex.)
	} while ( IP );
	IP = (xt*) rpop();
}


F_ATTR void doVALUE( void )    { push( *((CELL*)(run_xt->pfa)) ); }
F_ATTR void doQUAN( void )     { execute( (xt)(run_xt->pfa) ); }
F_ATTR void doVAR( void )      { push( run_xt->pfa ); }
F_ATTR void doCREATE( void )   { push( run_xt->pfa ); }
F_ATTR void doCONSTANT( void ) { push( run_xt->pfa ); }
F_ATTR void doBRANCH( void )   { IP = *(xt**) IP; }
F_ATTR void noop( void )       { return; }

F_ATTR void do_q_branch( void ) // ?BRANCH     f --
{
        if ( pop() )
        {
                ++ IP;
        } else {
		IP = *(xt**) IP;
        }
}


jmp_buf global_jmp_buf;
int global_jmp_buf_is_valid = 0;

CELL Catch( xt _x ) // i*x -- y*x 0 / i*x -- i*x` ior
{
	jmp_buf jb;
	volatile int flag = global_jmp_buf_is_valid;
	volatile unsigned int dd, rd;
	volatile CELL ior;
	volatile int z;
	volatile xt x = _x;
	xt *sIP;
	int y;

	memcpy( &jb, &global_jmp_buf, sizeof(jmp_buf) );
	global_jmp_buf_is_valid = 1;

	if ( 0 != (ior=setjmp(global_jmp_buf)) )
	{
		// exception!!!

		int t = dstack.occupied - dd;
		dstack.top -= t;
		dstack.empty += t;
		dstack.occupied = dd;

		t = rstack.occupied - rd;
		rstack.top -= t;
		rstack.empty += t;
		rstack.occupied = rd;

		IP = sIP;
		memcpy( &global_jmp_buf, &jb, sizeof(jmp_buf) );
		global_jmp_buf_is_valid = flag;

		// restore source input
		if ( flag )
		{
			y = z;
			cds( 0, y+1 );
			crs( y, 0 );
			while ( y )
			{
				__push( __rpop() );
				-- y;
			}
			push( z );
			F_USE( restore_input );
			__ndrop(1);
		}

#if DEBUG
		printf( "Catch (in `%s`): %d\n", x->name, (int)ior );
#endif

	} else {
		// save source input
		F_USE( save_input );
		z = y = __pop();
		crs( 0, y );
		while ( y )
		{
			__rpush( __pop() );
			-- y;
		}

		dd = dstack.occupied;
		rd = rstack.occupied;
		sIP = IP;
		IP = 0;

#if DEBUG
printf( "\nTry to Catch: %s\n", x->name );
fflush( NULL );
#endif

		execute( x );

#if DEBUG
printf( "\nNot Catched: %s\n", x->name );
fflush( NULL );
#endif

		IP = sIP;
		y = z;
		cds( 0, y );
		crs( y, 0 );
		while ( y )	{ __rpop(); -- y; }

		memcpy( &global_jmp_buf, &jb, sizeof(jmp_buf) );
		global_jmp_buf_is_valid = flag;
	}

	return ior;
}


void say_inter( void )
{
	F_USE( CR );
	push( (CELL)last_inter_word );
	push( last_inter_word_len );
	F_USE( TYPE );
}


void Throw( CELL ior ) // --
{
#if DEBUG
	printf( "\nThrow: %d", (int)ior );
#endif
START:
#if DEBUG
	printf( "\nThrow loop: %d\n", (int)ior );
	printf( "global_jmp_buf_is_valid=%d\n", (int)global_jmp_buf_is_valid );
	fflush( NULL );
#endif
	if ( ior )
	{
		if ( global_jmp_buf_is_valid )
		{
#if DEBUG
			printf( "long jmp...\n" );
			fflush( NULL );
#endif

			longjmp( global_jmp_buf, ior );
		}

		fflush( NULL );

		if ( -2 == ior ) // ABORT
		{
			if ( abort_text_len && abort_text )
			{
				putchar( '\n' );
				do
				{
					putchar( *abort_text );
					++abort_text;
				} while ( --abort_text_len );
				putchar( '\n' );
			} else
				printf( "\nABORT\" without text..." );

		}
	        if ( (-1 == ior) || (-2 == ior ) ) // ABORT
	        {
			dstack.top = dstack.v;
			dstack.empty += dstack.occupied;
			dstack.occupied = 0;
	        	ior = QUIT_ior;
	        }

		if ( -3 == ior )
		{
			say_inter();
			printf( " - stack overflow\n" );
			ior = QUIT_ior;
		}
		else if ( -4 == ior )
		{
			say_inter();
			printf( " - stack underflow\n" );
			ior = QUIT_ior;
		} else if ( -5 == ior )
		{
			say_inter();
			printf( " - return stack overflow\n" );
			ior = QUIT_ior;
		} else if ( -6 == ior )
		{
			say_inter();
			printf( " - return stack underflow\n" );
		} else if ( -13 == ior )
		{
#if DEBUG
			printf( "In Throw: undefined word (last_word=%p)", last_word );
			printf( " (tib=%p)", tib );
			fflush( NULL );
#endif
			push( (CELL)last_word );
			push( last_word_len );
#if DEBUG
			printf( " (%s)\n", last_word );
			fflush( NULL );
#endif
			F_USE( TYPE );
			printf( " - undefined word\n" );
			ior = QUIT_ior;
		} else if ( -17 == ior )
		{
			fprintf( stderr, "\nPictured numeric output string overflow\n(%s)\n", _hld );
		} else if ( -38 == ior )
		{
			fprintf( stderr, "\nNon-existent file\n" );
		}


		if ( QUIT_ior == ior )
		{
			state = 0;
			rstack.top = rstack.v;
			rstack.empty += rstack.occupied;
			rstack.occupied = 0;
			_source_id = 0;
			blk = 0;
			ior = Catch( (xt) &hide_wl[i_quit] );
			goto START;
		}
		else
			fprintf( stderr, "\nUnexpected exception: %d\n", (int)ior );
		exit( ior );
	}
}


uchar * current_data_space = 0;

void allot( sCELL n )
{
	if ( n > 0 )
	{
		if ( n + dp > ADD_DATA_SIZE + RES_DATA_SIZE + current_data_space )
			Throw( -8 ); // dictionary overflow
	} else {
		if ( n + dp < current_data_space )
			Throw( -15 ); // invalid FORGET
	}
	dp += n;
}


void check_data_space( uCELL need )
{
	if ( dp + need > current_data_space + ADD_DATA_SIZE + RES_DATA_SIZE )
	{
		int se = errno;
		uchar **p = malloc( ADD_DATA_SIZE + RES_DATA_SIZE + sizeof(uchar**) );
		if ( p )
		{
			*p = current_data_space;
			current_data_space = (uchar*)(p+1);
			dp = (uchar*) align( current_data_space );
		}
		errno = se;
	}
}


void destroy_space_list( uchar ** p )
{
	-- p;
	do {
		uchar *tmp;
		tmp = *p;
		free( p );
		p = (uchar**) tmp;
	} while ( p );
}

void destroy_spaces( void )
{
	if ( current_data_space )
		destroy_space_list( (uchar**) current_data_space );
	if ( dstack.v ) free( dstack.v );
	if ( rstack.v ) free( rstack.v );
}


void init_spaces( void )
{
	uchar **p;

	atexit( destroy_spaces );

	p = malloc( ADD_DATA_SIZE + RES_DATA_SIZE + sizeof(uchar**) );
	if ( !p )
	{
		Throw( errno );
		exit( ENOMEM );
	}

	*p = NULL;
	current_data_space = (uchar*)(p+1);
	dp = (uchar*) align( current_data_space );
}




void dsign( void )
{
	if ( 0 > (sCELL) __get(0) )
		__push( -1 );
	else
		__push( 0 );
}


// --------------------
/* Forth words       */


#define i( n )	F_DECLARE( n )
#define F( n )  F_USE( n )


// Programming tools word set

i( BYE ) // --
{
	exit( 0 );
}


i( AHEAD ) //           C: -- orig
{
        cds( 0, 2 );
        __push( (CELL) dp );
        __push( i_orig );
        allot( sizeof(CELL) );
}


i( dot_s ) // i*x -- i*x
{
	printf( "Data stack " );
	if ( dstack.occupied )
	{
		unsigned int k = dstack.occupied;
		printf( "(%u): ", k );
		do {
			--k;
			printf( "%lu ", __get(k) );
		} while (k);
		putchar( '\n' );
	} else
		printf( "is empty\n" );
}


i( r_dot_s ) // ;R i*x -- i*x
{
	printf( "Return stack " );
	if ( rstack.occupied )
	{
		unsigned int k = rstack.occupied;
		printf( "(%u): ", k );
		do {
			--k;
			printf( "%lu ", __rget(k) );
		} while (k);
		putchar( '\n' );
	} else
		printf( "is empty\n" );
}

#include "core.c"       	// Core and Core extention words
#include "block.c"      	// Block word list
#include "except.c"     	// Exception word set
#include "double.c"     	// Double number word set
#include "file.c"       	// File access word set
#include "memory.c"     	// Allocation memory word set
#include "search.c"     	// Search order word set
#include "string.c"     	// String word set


#undef i
// #undef F


// Forth wordlist

#define h( n )
#define i( n )     	{ (vfp) n,    0,             _NONE, #n },
#define ii( n, f ) 	{ (vfp) f,    0,             _NONE, #n },
#define if( n, fl )	{ (vfp) n,    0,        fl | _NONE, #n },
#define iif(n, f, fl )	{ (vfp) f,    0,        fl | _NONE, #n },
#define v( n, var )	{ doVAR,      (CELL) &(var), _NONE, #n },
#define c( n, c )       { doCONSTANT, (CELL) (c),    _NONE, #n },
#define q( n, q )  	{ doVALUE,    (CELL) &(q),   _NONE, #n },


struct _xt forth_wl[] =
{

{ NULL, 0, 0 , "" },	// see search.c

#include "words.inc"

{ (vfp)comma,        0, _NONE, ","  },
{ (vfp)c_comma,      0, _NONE, "C," },
{ (vfp)comma,        0, _NONE, "COMPILE," },
{ (vfp)tick,         0, _NONE, "\'" },
{ (vfp)paren,        0, _NONE|_IMMEDIATE, "(" },
{ (vfp)dot_quote,    0, _NONE|_IMMEDIATE, ".\"" },
{ (vfp)dot_paren,    0, _NONE|_IMMEDIATE, ".(" },
{ (vfp)bracket_tick, 0, _NONE|_IMMEDIATE, "[\']" },
{ (vfp)backslash,    0, _NONE|_IMMEDIATE, "\\" },
{ (vfp)abort_quote,  0, _NONE|_IMMEDIATE, "ABORT\"" },
{ (vfp)r_dot_s,      0, _NONE, "R.S" },

{ NULL, 0, 0 , "" }	// see search.c

};

#undef h
#undef i
#undef ii
#undef if
#undef iif
#undef v
#undef c
#undef q


// Hidden wordlist

#define h( n )     	{ (vfp) n,    0,             _NONE, #n },
#define i( n )
#define ii( n, f )
#define if( n, fl )
#define iif(n, f, fl )
#define v( n, var )
#define c( n, c )
#define q( n, q )


struct _xt hide_wl[] =
{

#include "words.inc"

};

#undef h
#undef i
#undef ii
#undef if
#undef iif
#undef v
#undef c
#undef q


struct _xt min_wl[] =
{

{ NULL, 0, 0 , "" },	// see search.c

{ (vfp)FORTH,          0, _NONE, "FORTH" },
{ (vfp)forth_wordlist, 0, _NONE, "FORTH-WORDLIST" },
{ (vfp)set_order,      0, _NONE, "SET-ORDER" },
{ (vfp)ONLY,           0, _NONE, "ONLY" },
{ (vfp)BYE,            0, _NONE, "BYE" },

{ NULL, 0, 0 , "" }	// see search.c
};


void init_stack( struct _stack *s )
{
	s_overflow( s, 1, ENOMEM );
}


void init_all( void )
{
	init_spaces();
	init_stack( &dstack );
	init_stack( &rstack );
	init_wordlists();
}




xt find_xt( uchar * name )
{
	int i = 0;
	do {
		if ( !strcmp( (char*)name, (char*)forth_wl[i].name ) )
			return &forth_wl[i];
	} while ( forth_wl[++i].f );
	return NULL;
}


char * START_TEXT[] = { " "

," FORTH ONLY FORTH ALSO DEFINITIONS "

," : ?PAIRS     ( x1 x2 -- ) "
,"     <> IF  -22 ( control structure mismatch ) THROW   THEN "
," ; "

," : CASE       ( -- 0 case-sys1 ) "
,"       0 7000 "
," ; IMMEDIATE "

," : OF         ( x1 x2 -- / x1 x2 -- x1 ; C: case-sys1 -- orig case-sys2 ) "
,"       7000 ?PAIRS "
,"       POSTPONE OVER  POSTPONE =  POSTPONE IF  POSTPONE DROP   8000 "
," ; IMMEDIATE "

," : ENDOF      ( -- ; C: orig case-sys2 -- dest case-sys1 ) "
,"       8000 ?PAIRS"
,"       POSTPONE ELSE  7000 "
," ; IMMEDIATE "

," : ENDCASE    ( x -- ; C: 0 dest1 ... destN case-sys1 -- ) "
,"       7000 ?PAIRS "
,"       POSTPONE DROP "
,"       BEGIN  DUP  WHILE "
,"           POSTPONE THEN "
,"       REPEAT DROP "
," ; IMMEDIATE "

," : 2CONSTANT  ( <spaces>name<space>  x1 x2 -- ) "
,"       CREATE  SWAP  , , "
,"       DOES>  DUP @  SWAP  CELL+ @ "
," ; "

," : [CHAR]  "
,"       CHAR  POSTPONE LITERAL "
," ; IMMEDIATE "

," : (C)   R>  DUP  COUNT 1+ CHARS +  >R ; "
," : C\"        ( \"word<\">\" -- c ) "
,"       POSTPONE (C) "
,"       [CHAR] \"  WORD "
,"       DUP C@ 1+ CHARS  HERE  SWAP  DUP ALLOT  MOVE "
,"       0 C, "
," ; IMMEDIATE "

," : S\"        ( string<\"> -- ) "
,"       [CHAR] \"  PARSE  POSTPONE SLITERAL "
," ; IMMEDIATE "

," : [COMPILE]  ( <spaces>name<space> -- ) "
,"       '  COMPILE, "
," ; IMMEDIATE "

," : COMPILE    ( <spaces>name<space> -- ) "
,"       '  POSTPONE LITERAL  POSTPONE COMPILE, "
," ; IMMEDIATE "

," : CS-ROLL    ( CSu CSu-1 ... CS0 u -- CSu-1 ... CS0 CSu ) "
,"       2* 1+  DUP >R  ROLL  R> ROLL "
," ; "

," : CS-PICK    ( CSu CSu-1 ... CS0 u -- CSu CSu-1 ... CS0 CSu ) "
,"       2* 1+  DUP >R  PICK  R> PICK "
," ; "

," : ?          ( addr -- ) "
,"       @ . "
," ; "
," : INIT       ( -- ) "
,"       S\" init.fs\" R/O OPEN-FILE 0= "
,"       IF  DUP >R  ['] INCLUDE-FILE CATCH "
,"           R> CLOSE-FILE SWAP THROW THROW "
,"       ELSE "
,"           DROP "
,"       THEN "
," ; "
," INIT "
, NULL }
;


int main( void )
{
	char **p;
	volatile int t = 10, s = -7;

	init_all();

	if ( t/s == -2 )
	{
		env_list[0].val = _TRUE;
	} else
	{
		env_list[0].val = _FALSE;
	}

	for ( p = START_TEXT; *p; ++p )
	{
		push( (CELL) *p );
		push( strlen(*p) );
		F( EVALUATE );
	}

	F( QUIT );

	return 255;
}


// End of file
