#define i_dest  	-2
#define i_orig  	-1
h  ( doDO )
#define i_doDO      	0
h  ( doLOOP )
#define i_doLOOP    	1
h  ( do_plus_loop )
#define i_do_plus_loop	2
h  ( doLITERAL )
#define i_doLITERAL	3
h  ( do_q_do )
#define i_do_q_do	4
h  ( doSLITERAL )
#define i_doSLITERAL	5
h  ( EXIT )
#define i_doEXIT	6
h  ( doBRANCH )
#define i_doBRANCH	7
h  ( do_q_branch )
#define i_do_q_branch	8
h  ( INTERPRET )
#define i_INTERPRET	9
h  ( TYPE )
#define i_TYPE     	10
h  ( do_abort_quote )
#define i_do_abort_quote 11
h  ( comma )
#define i_comma  	12
h  ( store )
#define i_store  	13
h  ( QUIT_ )
#define i_quit         	14
h  ( BLOCK )
#define i_BLOCK		15

h  ( _FIND )



c  ( -1,            	-1 )
c  ( 0,             	0 )
c  ( 1,             	1 )
c  ( BL,            	' ' )
c  ( CELL,          	sizeof(CELL) )
c  ( FALSE,         	_FALSE )
c  ( PAD,           	pad+sizeof(pad)/2 )
c  ( R/O,           	FAM_RO )
c  ( R/W,           	FAM_RW )
c  ( TRUE,          	_TRUE )
c  ( W/O,           	FAM_WO )

v  ( #TIB,          	number_tib )
v  ( >IN,           	to_in )
v  ( BASE,          	base )
v  ( BLK,           	blk )
v  ( SPAN,          	span )
v  ( STATE,         	state )
v  ( TIB,           	tib )

q  ( SOURCE-ID,     	_source_id )

ii ( !,             	store )
ii ( #,             	number_sign )
ii ( #>,            	number_sign_greater )
ii ( #S,            	number_sign_s )
ii ( *,             	mul )
ii ( */,            	star_slash )
ii ( */MOD,         	star_slash_mod )
ii ( +,             	plus )
ii ( +!,            	plus_store )
iif( +LOOP,         	plus_loop, _IMMEDIATE )
ii ( -,             	minus )
ii ( -TRAILING,     	minus_trailing )    	// String word set
ii ( .,             	dot )
ii ( .R,            	dot_r )
ii ( .S,            	dot_s )             	// Programming tools word set
ii ( /,             	slash )
ii ( /MOD,          	slash_mod )
ii ( /STRING,       	slash_string )      	// String word set
ii ( 0<,            	zero_less )
ii ( 0<>,           	zero_not_equals )
ii ( 0=,            	zero_equal )
ii ( 0>,            	zero_greater )
ii ( 1+,            	one_plus )
ii ( 1-,            	one_minus )
ii ( 2!,            	two_store )
ii ( 2*,             	two_star )
ii ( 2/,            	two_slash )
ii ( 2>R,           	two_to_r )
ii ( 2@,            	two_fetch )
ii ( 2DROP,         	two_drop )
ii ( 2DUP,           	two_dup )
ii ( 2LITERAL,      	two_literal )       	// Double number word set
ii ( 2OVER,         	two_over )
ii ( 2R>,           	two_r_from )
ii ( 2R@,           	two_r_fetch )
ii ( 2ROT,          	two_rot )           	// Double number word set
ii ( 2SWAP,         	two_swap )
ii ( 2VARIABLE,     	two_variable )      	// Double number word set
ii ( :,             	colon )
ii ( :NONAME,       	colon_noname )
iif( ;,             	semicolon, _IMMEDIATE )
ii ( <,             	less_than )
ii ( <#,            	less_number_sign )
ii ( <>,             	not_equal )
ii ( =,              	equal )
ii ( >,             	greater_than )
ii ( >BODY,         	to_body )
ii ( >NUMBER,       	to_number )
ii ( >R,            	to_r )
iif( ?DO,           	q_do, _IMMEDIATE )
ii ( ?DUP,          	q_dup )
ii ( @,             	fetch )
i  ( ABORT )        	                    	// Exception word set
i  ( ABS )
i  ( ACCEPT )
if ( AGAIN,         	_IMMEDIATE )
i  ( AHEAD )        	                    	// Programming tools word set
i  ( ALIGN )
i  ( ALIGNED )
i  ( ALLOCATE )     	                    	// Memory allocation word set
i  ( ALLOT )
i  ( ALSO )         	                    	// Search order word set
i  ( AND )
if ( BEGIN,          	_IMMEDIATE )
i  ( BIN )          	                    	// File access word set
i  ( BLANK )        	                    	// String word set
i  ( BLOCK )        	                    	// Block word set
i  ( BUFFER )       	                    	// Block word set
i  ( BYE )          	                    	// Programming tools word set
ii ( C!,            	c_store )
ii ( C@,            	c_fetch )
i  ( CATCH )        	                    	// Exception word set
ii ( CELL+,         	cell_plus )
i  ( CELLS )
i  ( CHAR )
ii ( CHAR+,         	char_plus )
i  ( CHARS )
ii ( CLOSE-FILE,    	close_file )        	// File access word set
i  ( CMOVE )        	                    	// String word set
ii ( CMOVE>,        	cmove_up )          	// String word set
i  ( COMPARE )      	                    	// String word set
i  ( CONSTANT )
i  ( CONVERT )
i  ( COUNT )
i  ( CR )
i  ( CREATE )
ii ( CREATE-FILE,   	create_file )       	// File access word set
ii ( D+,            	d_plus )            	// Double number word set
ii ( D-,            	d_minus )           	// Double number word set
ii ( D.,            	d_dot )             	// Double number word set
ii ( D.R,           	d_dot_r )           	// Double number word set
ii ( D0<,           	d_zero_less )       	// Double number word set
ii ( D0=,           	d_zero_equal )      	// Double number word set
ii ( D2*,           	d_two_star )        	// Double number word set
ii ( D2/,           	d_two_slash )       	// Double number word set
ii ( D<,            	d_less_than )       	// Double number word set
ii ( D=,            	d_equal )           	// Double number word set
ii ( D>S,           	DROP )              	// Double number word set
i  ( DABS )         	                    	// Double number word set
i  ( DECIMAL )
i  ( DEFINITIONS )  	                    	// Search order word set
ii ( DELETE-FILE,   	delete_file )       	// File access word set
i  ( DEPTH )
i  ( DMAX )         	                    	// Double number word set
i  ( DMIN )         	                    	// Double number word set
i  ( DNEGATE )      	                    	// Double number word set
if ( DO,            	_IMMEDIATE )
ii ( DOES>,          	does )
i  ( DROP )
ii ( DU<,           	d_u_less_than )     	// Double number word set
i  ( DUP )
if ( ELSE,          	_IMMEDIATE )
i  ( EMIT )
ii ( ENVIRONMENT?,  	environment_q )
i  ( ERASE )
i  ( EVALUATE )
i  ( EXECUTE )
i  ( EXIT )
i  ( EXPECT )
ii ( FILE-POSITION, 	file_position )     	// File access word set
ii ( FILE-SIZE,     	file_size )         	// File access word set
ii ( FILE-STATUS,   	file_status )       	// File access word set
i  ( FILL )
i  ( FIND )         	                    	// Search order word set
i  ( FLUSH )        	                    	// Block word set
ii ( FLUSH-FILE,    	flush_file )   	    	// File access word set
ii ( FM/MOD,        	fm_slash_mod )
i  ( FORTH )        	                    	// Search order word set
ii ( FORTH-WORDLIST,	forth_wordlist )    	// Search order word set
i  ( FREE )         	                    	// Memory allocation word set
ii ( GET-CURRENT,   	get_current )       	// Search order word set
ii ( GET-ORDER,     	get_order )         	// Search order word set
i  ( HERE )
i  ( HEX )
i  ( HOLD )
i  ( I )
if ( IF,            	_IMMEDIATE )
i  ( IMMEDIATE )
ii ( INCLUDE-FILE,  	include_file )      	// File access word set
i  ( INCLUDED )     	                    	// File access word set
i  ( INVERT )
i  ( J )
i  ( KEY )
i  ( LEAVE )
if ( LITERAL,       	_IMMEDIATE )
i  ( LOAD )         	                    	// Block word set
if ( LOOP,          	_IMMEDIATE )
i  ( LSHIFT )
ii ( M*,            	m_star )
ii ( M+,            	m_plus )            	// Double number word set
i  ( MAX )
i  ( MIN )
i  ( MOD )
i  ( MOVE )
i  ( NEGATE )
i  ( NIP )
i  ( ONLY )         	                    	// Search order word set
ii ( OPEN-FILE,     	open_file )         	// File access word set
i  ( OR )
i  ( ORDER )        	                    	// Search order word set
i  ( OVER )
i  ( PARSE )
i  ( PICK )
if ( POSTPONE,      	_IMMEDIATE )
i  ( PREVIOUS )     	                    	// Search order word set
i  ( QUERY )
i  ( QUIT )
ii ( R>,            	r_from )
ii ( R@,            	r_fetch )
i  ( RDROP )
ii ( READ-FILE,     	read_file )         	// File access word set
ii ( READ-LINE,     	read_line )         	// File access word set
if ( RECURSE,       	_IMMEDIATE )
i  ( REFILL )
ii ( RENAME-FILE,   	rename_file )       	// File access word set
if ( REPEAT,        	_IMMEDIATE )
ii ( REPOSITION-FILE,	reposition_file )   	// File access word set
i  ( RESIZE )       	                    	// Memory allocation word set
ii ( RESIZE-FILE,   	resize_file )       	// File access word set
ii ( RESTORE-INPUT, 	restore_input )
i  ( ROLL )
i  ( ROT )
i  ( RSHIFT )
ii ( S>D,           	s_to_d )
ii ( SAVE-BUFFERS,  	save_buffers )      	// Block word set
ii ( SAVE-INPUT,    	save_input )
i  ( SEARCH )       	                    	// String word set
ii ( SEARCH-WORDLIST,	search_wordlist )   	// Search order word set
ii ( SET-CURRENT,   	set_current )       	// Search order word set
ii ( SET-ORDER,     	set_order )         	// Search order word set
i  ( SIGN )
if ( SLITERAL,      	_IMMEDIATE )        	// String word set
ii ( SM/REM,        	s_m_slash_rem )
i  ( SOURCE )
i  ( SPACE )
i  ( SPACES )
i  ( SWAP )
if ( THEN,          	_IMMEDIATE )
i  ( THROW )        	                    	// Exception word set
if ( TO,            	_IMMEDIATE )
i  ( TUCK )
i  ( TYPE )
ii ( U.,            	u_dot )
ii ( U.R,           	u_dot_r )
ii ( U<,            	u_less_than )
ii ( U>,            	u_greater_than )
ii ( UM*,           	u_m_star )
ii ( UM/MOD,        	u_m_slash_mod )
i  ( UNLOOP )
if ( UNTIL,         	_IMMEDIATE )
i  ( UNUSED )
i  ( UPDATE )       	                    	// Block word set
i  ( VALUE )
i  ( VARIABLE )
if ( WHILE,             _IMMEDIATE )
i  ( WITHIN )
i  ( WORD )
i  ( WORDS )        	                    	// Programming tools word set
i  ( WORDLIST )     	                    	// Search order word set
ii ( WRITE-FILE,    	write_file )        	// File access word set
ii ( WRITE-LINE,    	write_line )        	// File access word set
i  ( XOR )
iif( [,             	left_bracket, _IMMEDIATE )
ii ( ],             	right_bracket )
