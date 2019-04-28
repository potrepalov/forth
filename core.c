// Copyleft  by Potrepalov I.S.

// --------------------------------
/* Core and Core extention words */


#include "loop.c"


i( store ) // ! 	x a --
{
	CELL *p;
	cds( 2, 0 );
	p = (CELL*) __get(0);
	*p = __get(1);
	__ndrop(2);
}


i( number_sign ) // #   	ud1 -- ud2
{
	uCELL u;
	uCELL d;
	uchar dig;
	cds( 2, 0 );

	if ( hld <= _hld ) Throw( -17 ); // pictured numeric output string overflow
	u = __get(1);
	d = u % base;
	if ( d < 10 )
		dig = d + '0';
	else
		dig = d - 10 + 'A';
	*(--hld) = dig;
	__put( 1, u / base );
}


i( number_sign_greater ) // #>	xd -- a u
{
	cds( 2, 0 );
	__put( 0, sizeof(_hld) - (hld-_hld) - 1 );
	__put( 1, (CELL)hld );
}


i( number_sign_s ) // #S    	ud -- 0 0
{
	cds( 2, 0 );
	do {
		F( number_sign );
	} while ( __get(1) );
	__put( 0, 0 );
}


i( tick ) // '   	"<spaces>name<space>"  -- xt
{
	NextWord();
	F( _FIND );
	if ( !__pop() )
		Throw( -13 ); // undefined word
}


i( paren ) // (   	"string<)>" --
{
	if ( _source_id > 0 )
	{
		do {
		        char *a;
			push( (CELL) ')' );
			F( PARSE );
			a = ((char*)__get(1)) + (int)__get(0);
			__ndrop(2);
			if ( ')' == *a )
				break;

			F( REFILL );
			if ( !__pop() )
				break;
		} while (1);
	} else
	{
		push( (CELL) ')' );
		F( PARSE );
		__ndrop(2);
	}
}


i( mul ) // *   	n1 n2 -- n1*n2
{
	sCELL i2;
	cds( 2, 0 );
	i2 = (sCELL) __pop();
	__put( 0, (CELL) (i2 * (sCELL)__get(0)) );
}


i( star_slash ) // */	n1 n2 n3 -- n4
{
	F( star_slash_mod );
	F( NIP );
}


i( star_slash_mod ) // */MOD	n1 n2 n3 -- n4 n5
{
	F( to_r );
	F( m_star );
	F( r_from );
	F( s_m_slash_rem );
}


i( plus ) // +  	x1 x2 -- x1+x2
{
	CELL x2;
	cds( 2, 0 );
	x2 = __pop();
	__put( 0, x2 + __get(0) );
}


i( plus_store ) // +!	x a --
{
	cds( 2, 0 );
	*((CELL*)__get(0)) += __get(1);
	__ndrop(2);
}


i( comma ) // ,   	x --
{
	allot( sizeof(CELL) );
	*(((CELL*)dp)-1) = pop();
#if DEBUG
printf( "HERE=%d ", (char*)dp );
fflush(0);
#endif
}


i( minus ) // -   	x1 x2 -- x1-x2
{
	CELL x2;
	cds( 2, 0 );
	x2 = __pop();
	__put( 0, __get(0) - x2 );
}


i( dot ) // .    	n --
{
	cds( 0, 1 );
	dsign();
	F( d_dot );
}


i( dot_quote ) // ."   "string<">"  --
{
	push( (CELL) '"' );
	F( PARSE );
	if ( state )
	{
		F( SLITERAL );
		push( (CELL) &hide_wl[i_TYPE] );
		F( comma );
	}
	else
		F( TYPE );
}


i( dot_paren ) // .(   "string<)>"  --
{
	push( (CELL) ')' );
	F( PARSE );
	F( TYPE );
}


i( dot_r ) // .R    	n1 n2 --
{
	cds( 0, 1 );
	F( SWAP );
	dsign();
	F( ROT );
	F( d_dot_r );
}


i( slash ) // /  	n1 n2 -- n1/n2
{
	cds( 2, 0 );
	__put( 1, ((sCELL)__get(1))/((sCELL)__get(0)) );
	__ndrop(1);
}


i( slash_mod ) // /MOD 	n1 n2 -- (n1 mod n2) (n1/n2)
{
	sCELL n1, n2;
	cds( 2, 0 );
	n2 = __get(0);
	n1 = __get(1);
	__put( 1, n1 % n2 );
	__put( 0, n1 / n2 );
}


i( zero_less ) // 0<	n -- n<0
{
	cds( 1, 0 );
	__put( 0, (0 > (sCELL) __get(0)) ? _TRUE : _FALSE );
}


i( zero_not_equals ) // 0<> 	x -- 0<>x
{
	cds( 1, 0 );
	__put( 0, __get(0) ? _TRUE : _FALSE );
}


i( zero_equal ) // 0=	x -- 0=x
{
	cds( 1, 0 );
	__put( 0, __get(0) ? _FALSE : _TRUE );
}


i( zero_greater ) // 0> 	n -- n>0
{
	cds( 1, 0 );
	__put( 0, (0 < (sCELL) __get(0)) ? _TRUE : _FALSE );
}


i( one_plus ) // 1+	x -- x+1
{
	cds( 1, 0 );
	__put( 0, 1 + __get(0) );
}


i( one_minus ) // 1-	x -- x-1
{
	cds( 1, 0 );
	__put( 0, __get(0) - 1 );
}


i( two_store ) // 2! 	x1 x2 a --
{
	CELL *p;
	cds( 3, 0 );
	p = (CELL*) __get(0);
	*p = __get(1);
	*(p+1) = __get(2);
	__ndrop(3);
}


i( two_star ) // 2* 	n -- n*2
{
	cds( 1, 0 );
	__put( 0, __get(0) << 1 );
}


i( two_slash ) // 2/ 	n -- n/2
{
	cds( 1, 0 );
	__put( 0, ((sCELL)__get(0)) >> 1 );
}


i( two_to_r ) // 2>R	x1 x2 --  ;r -- x1 x2
{
	cds( 2, 0 ); crs( 0, 2 );
	__rpush( __get(1) );
	__rpush( __get(0) );
	__ndrop( 2 );
}


i( two_fetch ) // 2@ 	a -- x1 x2
{
	CELL *p;
	cds( 1, 1 );
	p = (CELL*) __get(0);
	__push( *p );
	__put( 1, *(p+1) );
}


i( two_drop ) // 2DROP	x1 x2 --
{
	cds( 2, 0 );
	__ndrop( 2 );
}


i( two_dup ) // 2DUP 	xd -- xd xd
{
	F( OVER );
	F( OVER );
}


i( two_over ) // 2OVER	xd1 xd2 -- xd1 xd2 xd1
{
	CELL x1, x2;
	cds( 4, 2 );
	x1 = __get(2);
	x2 = __get(3);
	__push( x2 );
	__push( x1 );
}


i( two_r_from ) // 2R> 	-- x1 x2  ;r x1 x2 --
{
	cds( 0, 2 ); crs( 2, 0 );
	__push( __rget(1) );
	__push( __rpop() );
	__rpop();
}


i( two_r_fetch ) // 2R@ 	-- x1 x2  ;r x1 x2 -- x1 x2
{
	cds( 0, 2 ); crs( 2, 0 );
	__push( __rget(1) );
	__push( __rget(0) );
}


i( two_swap ) // 2SWAP 		xd1 xd2 -- xd2 xd1
{
	CELL x1, x2;
	cds( 4, 0 );
	x1 = __get(0);
	x2 = __get(1);
	__put( 1, __get(3) );
	__put( 3, x2 );
	__put( 0, __get(2) );
	__put( 2, x1 );
}


i( colon ) // :  "<spaces>name<space>" -- colon-sys
{
	F( CREATE );
	last_xt->f = doCOLON;
	last_xt->flags |= _SMUDGE;
	F( right_bracket );
}


i( colon_noname ) // :NONAME 	-- xt colon-sys
{
	xt x;

	F( ALIGN );
	check_data_space( RES_DATA_SIZE );
	x = get_xt( current );
	push( (CELL)x );
	allot( sizeof(CELL) ); // for DOES>
	x->f = doCOLON;
	x->pfa = (CELL) dp;
	x->flags = _NONE;
	F( right_bracket ); // ]
}


i( semicolon ) // ; 	colon-sys --
{
	push( (CELL) & hide_wl[i_doEXIT] );
	F( comma );
	F( left_bracket );
	last_xt->flags &= ~_SMUDGE;
}


i( less_than ) // < 	n1 n2 -- n1 < n2
{
	sCELL i2;
	cds( 2, 0 );
	i2 = (sCELL) __pop();
	__put( 0, ((sCELL) __get(0)) < i2 ? _TRUE : _FALSE );
}


i( less_number_sign ) // <# 	--
{
	hld = _hld + sizeof(_hld);
	*(--hld) = '\0';
}


i( not_equal ) // <>	x1 x2 -- x1 <> x2
{
	CELL x2;
	cds( 2, 0 );
	x2 = __pop();
	__put( 0, (x2 != __get(0)) ? _TRUE : _FALSE );
}


i( equal ) // = 	x1 x2 -- x1 = x2
{
	CELL x2;
	cds( 2, 0 );
	x2 = __pop();
	__put( 0, (x2 == __get(0)) ? _TRUE : _FALSE );
}


i( greater_than ) // >	n1 n2 -- n1 > n2
{
	F( SWAP );
	F( less_than );
}


i( to_body ) // >BODY	xt -- a
{
	cds( 1, 0 );
	__put( 0, ((xt)__get(0))->pfa );
}


i( to_number ) // >NUMBER 	ud1 a1 u1 -- ud2 a2 u2
{
	uchar *a;
	CELL u;
	CELL ud;
	uchar ch;

	cds( 4, 0 );

	u = __get(0);
	a = (uchar*) __get(1);
	ud = __get(3);
#if DEBUG
printf( "In >NUMBER: " );
F( two_dup );
F( TYPE );
F( SPACE );
fflush(0);
#endif
	while ( u )
	{
		unsigned int t = UINT_MAX;
		const char c1[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		const char c2[] = "abcdefghijklmnopqrstuvwxyz";
		const char *ci = NULL;
		ch = *a;
		if ( !ch ) break;
		if ( (ch >= '0') && (ch <= '9') )
			t = ch - '0';
		else if ( NULL != (ci = strchr(c1,ch)) )
			t = ci - c1 + 10;
		else if ( NULL != (ci = strchr(c2,ch)) )
			t = ci - c2 + 10;

		if ( t >= base ) break;

		ud = ud * base + t;
		++a;
		--u;
	}

	__put( 0, u );
	__put( 1, (CELL) a );
	__put( 3, ud );
}


i( to_r ) // >R  	x -- ;r -- x
{
	rpush( pop() );
}


i( q_dup ) // ?DUP 	0 -- 0 / x -- x x
{
	CELL x;
	cds( 1, 1 );
	x = __get(0);
	if ( x ) __push( x );
}


i( fetch ) // @  	a -- x
{
	cds( 1, 0 );
	__put( 0, *(CELL*)__get(0) );
}


i( ABS ) // n -- |n|
{
	sCELL x;
	cds( 1, 0 );
	x = (sCELL) __get(0);
	if ( x < 0 ) __put( 0, (CELL) -x );
}


static int accepted = 0;
i( ACCEPT ) // a u -- u2
{
	uCELL u, u2;
	uchar *a;
	int ch;

	cds( 2, 0 );

	u = __pop();
	a = (uchar*)__get(0);
	u2 = 0;
	ch = getchar();
	accepted = (EOF != ch);
	while ( (EOF != ch) && ('\n' != ch) )
	{
		if ( u )
		{
			*a = ch;
			++a; ++u2;
			--u;
		}
		ch = getchar();
	}
	__put( 0, u2 );
}


i( AGAIN ) //      	C: dest --
{
	cds( 2, 0 );
	if ( i_dest != __get(0) ) Throw( -22 ); // control structure mismatch
	__put(0, (CELL) &hide_wl[i_doBRANCH] );
	F( comma );
	F( comma );
}


i( ALIGN ) // --
{
	dp = (uchar*)align( dp );
}


i( ALIGNED ) // a -- a'
{
	cds( 1, 0 );
	__put( 0, (CELL)align(__get(0)) );
}


i( ALLOT ) // n --
{
	allot( pop() );
}


i( AND ) // x1 x2 -- x1 & x2
{
	CELL x2;
	cds( 2, 0 );
	x2 = __pop();
	__put( 0, x2 & __get(0) );
}


i( BEGIN ) //     	C: -- dest
{
	cds( 0, 2 );
	__push( (CELL) dp );
	__push( i_dest );
}


i( c_store ) // C!	c a --
{
	cds( 2, 0 );
	*((uchar*)__get(0)) = (uchar) __get(1);
	__ndrop(2);
}


i( c_comma ) // C,	c --
{
	allot( sizeof(uchar) );
	*(dp - 1) = (uchar) pop();
}


i( c_fetch ) // C@	a -- c
{
	cds( 1, 0 );
	__put( 0, (CELL) *((uchar*)__get(0)) );
}


i( cell_plus ) // CELL+ 	x -- x+sizeof(CELL)
{
	cds( 1, 0 );
	__put( 0, sizeof(CELL) + __get(0) );
}


i( CELLS ) // n -- n*sizeof(CELL)
{
	cds( 1, 0 );
	__put( 0, sizeof(CELL) * (sCELL)__get(0) );
}


i( CHAR ) // "<spaces>word<space>" -- c
{
	NextWord();
	if ( __pop() )
		__put( 0, (CELL)(*(uchar*)__get(0)) );
	else
		__put( 0, 0 );
}


i( char_plus ) // CHAR+ 	x -- x+sizeof(uchar)
{
	cds( 1, 0 );
	__put( 0, sizeof(uchar) + __get(0) );
}


i( CHARS ) // n -- n*sizeof(uchar)
{
	cds( 1, 0 );
	__put( 0, sizeof(uchar) * (sCELL)__get(0) );
}


i( CONSTANT ) // "<spaces>name<space>"  x --
{
	F( CREATE );
	last_xt->f = doCONSTANT;
	last_xt->pfa = pop();
	check_data_space(0);
}


i( CONVERT ) // 	ud1 a1 -- ud2 a2
{
	uchar *a;
	CELL ud;
	uchar ch;

	cds( 4, 0 );

	a = (uchar*) __get(0);
	ud = __get(2);

	do
	{
		unsigned int t = UINT_MAX;
		const char c1[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		const char c2[] = "abcdefghijklmnopqrstuvwxyz";
		const char *ci = NULL;
		ch = *(++a);
		if ( !ch ) break;
		if ( (ch >= '0') && (ch <= '9') )
			t = ch - '0';
		else if ( NULL != (ci = strchr(c1,ch)) )
			t = ci - c1;
		else if ( NULL != (ci = strchr(c2,ch)) )
			t = ci - c2;

		if ( t >= base ) break;

		ud = ud * base + t;
	} while (1);

	__put( 0, (CELL) a );
	__put( 2, ud );
}


i( COUNT ) // a -- a+sizeof(uchar) u
{
	uchar *a;
	cds( 1, 1 );
	a = (uchar*) __get(0);
	__push( (CELL)*a );
	__put( 1, (CELL)(a+1) );
}


i( CR ) // --
{
	putchar( '\n' );
}


i( CREATE ) //  "<spaces>name<space>" --
{
	uCELL u;
	NextWord();
	u = __get(0);
	if ( !u )
		Throw( -16 ); // attempt to use zero-length string as a name
	if ( 31 < u )
		Throw( -19 ); // definition name too long
	F( colon_noname );
	F( left_bracket );
	F( DROP );
#if DEBUG
printf( "in CREATE (%p %p %d)", (uchar*)last_xt, (uchar*)__get(1), (int) u );
fflush(0);
#endif
	memcpy( &(last_xt->name[0]), (uchar*)__get(1), u );
	last_xt->name[u] = '\0';
	last_xt->f = doCREATE;
	__ndrop(2);
}


i( DECIMAL ) // --
{
	base = 10;
}


i( DEPTH )
{
	unsigned int t = dstack.occupied;
	push( t );
}


i( does ) // DOES> 	C: colon-sys1 -- colon-sys2
{
	CELL *t;
	if ( !last_xt || !last_xt->pfa ) Throw( -257 );
	t = (CELL*) last_xt->pfa;
	*(t-1) = (CELL) IP;
	last_xt->f = doDOES;
	F( EXIT );
}


i( DROP ) // x --
{
	pop();
}


i( DUP ) // x -- x x
{
	CELL x;
	cds( 1, 1 );
	x = __get(0);
	__push( x );
}


i( ELSE ) //     	C: orig1 -- orig2
{
	cds( 2, 0 );
	if ( i_orig != __get(0) ) Throw( -22 ); // control structure mismatch

	__put( 0, (CELL) &hide_wl[i_doBRANCH] );
	F( comma );
	F( AHEAD );

	F( ROT );
	*(CELL*)__pop() = (CELL) dp;
}


i( EMIT ) // c --
{
	cds( 1, 0 );
	putchar( (int) __pop() );
}


i( environment_q ) // ENVIRONMENT? 	a u -- false / a u -- i*x true
{
	uchar *a;
	CELL u;
	int z;

	cds( 2, 0 );

	u = __get(0);
	a = (uchar*) __get(1);

	if ( (5 == u)
	     && !memcmp( "MAX-D", a, 5 ) )
	{
		__put( 1, LONG_MAX );
		__put( 0, 0 );
		push( _TRUE );
		return;
	}

	if ( (6 == u)
	     && !memcmp( "MAX-UD", a, 6 ) )
	{
		__put( 1, ULONG_MAX );
		__put( 0, 0 );
		push( _TRUE );
		return;
	}

	z = 0;
	do {
		if ( (strlen((char*)env_list[z].name) == u)
		     && !memcmp(env_list[z].name, a, u) )
		{
			__put( 0, _TRUE );
			__put( 1, env_list[z].val );
			return;
		}
	} while ( env_list[++z].name );

	__ndrop(1);
	__put( 0, _FALSE );
}


i( ERASE ) // a u --
{
	push( 0 );
	F( FILL );
}



static int is_space( char ch )
{
	return (' ' == ch) || ((' ' > ch) && (_source_id > 0));
}

void NextWord( void ) // "<spaces>name<space>"  -- a u
{

#if DEBUG
if ( source_a )
	printf( "\nNext words: %s", source_a + to_in );
else
	printf( "\nSource address is zero!" );
fflush( NULL );
#endif
	cds( 0, 2 );
	if ( source_a )
	{
		uchar *start = source_a + to_in;
		CELL z = 0;

		while ( (to_in < source_u) && is_space(*start) )
		{
			++ to_in;
			++ start;
		}
		last_word = start;
		__push( (CELL)start );
		while ( to_in < source_u )
		{
			++ to_in;
			if ( is_space(*start) )
				break;
			++ z; ++start;
		}
		last_word_len = z;
		__push( z );
	} else {
		__push( (CELL) pad );
		__push( 0 );
	}
#if DEBUG
printf( " (%p %u)\n", (uchar*) __get(1), (int) __get(0) );
fflush(0);
#endif
}


i( EVALUATE ) // a u --
{
	sCELL ssid = _source_id;
	uCELL sin = to_in;
	CELL sblk = blk;
	uchar * ssa = source_a;
	CELL ssu = source_u;
//	CELL ior;

	cds( 2, 0 );

	_source_id = -1;
	to_in = 0;
	blk = 0;
	source_a = (uchar*)__get(1);
	source_u = __get(0);
	__ndrop(2);

	// FIXME: Catch() is not need, when CATCH save and THROW restore source input
	F( INTERPRET );
//	ior = Catch( &hide_wl[i_INTERPRET] );

	source_u = ssu;
	source_a = ssa;
	blk = sblk;
	to_in = sin;
	_source_id = ssid;

//	Throw( ior );
}


void isliteral( void ) // a u -- ...
{
	int sign = 0;
	uchar *a;
	int t;

	cds( 0, 2 );
#if DEBUG
printf( "In isliteral (%d) ", dstack.occupied );
F( two_dup ); F( TYPE ); F( SPACE );
fflush(0);
#endif
	a = (uchar*) __get(1);
	if ( ('-' == *a) || ('+' == *a) )
	{
		sign = ('-' == *a);
		++a;
		__put( 1, (CELL)a );
		__put( 0, __get(0) - 1 );
	}
	__push(0); __push(0); F( two_swap );
	F( to_number );
	t = __pop();
	if ( t )
	{
		if ( t > 1 )
			Throw( -13 ); // undefined word
		if ( '.' != *(uchar*)__pop() )
			Throw( -13 );
		if ( sign )
			F( DNEGATE );
		F( two_literal );
	}
	else
	{
		__ndrop(2);
		if ( sign )
			F( NEGATE );
		F( LITERAL );
	}
}


i( INTERPRET ) // i*x -- j*x
{
	CELL len, a;
	do
	{
		int flag;
		NextWord();
		if ( !__get(0) )
			break;

		last_inter_word_len = __get(0);
		last_inter_word = (uchar*)__get(1);
#if DEBUG
printf( "(%p %d) ", (uchar*) __get(1), (int)__get(0) );
//F( two_dup );
F( OVER );
F( OVER );
printf( "Next word (%p %d): ", (uchar*) __get(1), (int)__get(0) );
F( TYPE );
fflush( NULL );
#endif
		a = __get(1);
		len = __get(0);
		F( _FIND );
		flag = __pop();
		if ( !flag )
		{
			__push(a);
			__push(len);
			isliteral();
		}
		else if ( !state || (flag > 0) )
			F( EXECUTE );
		else
			F( comma );
	} while ( 1 );
	__ndrop(2);
}


i( EXECUTE ) // xt --
{
	xt * sIP = IP;
	IP = 0;
	execute( (xt) pop() );
	IP = sIP;
}


i( EXIT ) // --
{
	IP = 0;
#if DEBUG
	printf( "IP=%d (exit)\n", (int)IP );
	fflush( NULL );
#endif
}


i( EXPECT ) // a u -- u2
{
	uCELL u;
	uchar *a;
	cds( 2, 0 );
	u = __get(0);
	a = (uchar*)__get(1);
	__ndrop(2);
	span = 0;
	while ( u )
	{
		int ch = getchar();
		if ( (EOF == ch) || ('\n' == ch) ) break;
		*a = ch;
		++a; ++span;
		--u;
	}
}


i( FILL ) // a u c --
{
	cds( 3, 0 );
	memset( (uchar*)__get(2), (uchar)__get(0), (size_t)__get(1) );
	__ndrop(3);
}


i( fm_slash_mod ) // FM/MOD 	d n -- n2=(d mod n) n3=d/n
{
	sCELL n1, d1;
	sCELL r, q;
	cds( 3, 0 );

	n1 = __pop();
	d1 = __get(1);
	r = d1 % n1;
	q = d1 / n1;
	if ( r
	     && (_FALSE == env_list[0].val)	// it isn't floored division
	     && (    ((d1 > 0) && (n1 < 0))
	          || ((d1 < 0) && (n1 > 0)) )
	   )
	{
		r += n1;
		--q;
	}
	__put( 1, r );
	__put( 0, q );
}


i( HERE ) // -- a
{
	push( (CELL)dp );
}


i( HEX ) // --
{
	base = 16;
}


i( HOLD ) // c --
{
	if ( hld <= _hld ) Throw( -17 ); // pictured numeric output string overflow
	*(--hld) = (uchar) pop();
}


i( IF ) // 	C: -- orig
{
	push( (CELL) &hide_wl[i_do_q_branch] );
	F( comma );
	F( AHEAD );
}


i( IMMEDIATE ) //
{
	if ( !last_xt || !last_xt->pfa ) Throw( -257 );
	last_xt->flags |= _IMMEDIATE;
}


i( INVERT ) // x -- x2
{
	cds( 1, 0 );
	__put( 0, ~__get(0) );
}


i( KEY ) // -- c
{
	push( getchar() );
}


i( doLITERAL ) // -- x
{
	push( *(CELL*)IP );
	IP = (xt*) (((CELL*)IP) + 1);
}


i( LITERAL ) // x --
{
	if ( state )
	{
		push( (CELL) &hide_wl[i_doLITERAL] );
		F( comma );
		F( comma );
	}
}


i( LSHIFT ) // u1 u2 -- u1 << u2
{
	CELL u;
	cds( 2, 0 );
	u = __pop();
	__put( 0, ((uCELL)__get(0)) << u );
}


i( m_star ) // M*  	n1 n2 -- d
{
	cds( 2, 0 );
	__put( 1, ((sCELL)__get(0)) * (sCELL)__get(1) );
	F( DROP );
	dsign();
}


i( MAX ) // n1 n2 -- max(n1,n2)
{
	sCELL n2;
	cds( 2, 0 );
	n2 = __pop();
	if ( n2 > (sCELL)__get(0) ) __put( 0, n2 );
}


i( MIN ) // n1 n2 -- min(n1,n2)
{
	sCELL n2;
	cds( 2, 0 );
	n2 = __pop();
	if ( n2 < (sCELL)__get(0) ) __put( 0, n2 );
}


i( MOD ) // n1 n2 -- n3
{
	sCELL n2;
	cds( 2, 0 );
	n2 = __pop();
	__put( 0, ((sCELL)__get(0)) % n2 );
}


i( MOVE ) // a1 a2 u --
{
	cds( 3, 0 );
	memmove( (uchar*)__get(1), (uchar*)__get(2), (size_t)__get(0) );
	__ndrop(3);
}


i( NEGATE ) // n -- -n
{
	cds( 1, 0 );
	__put( 0, -(sCELL)__get(0) );
}


i( NIP ) // x1 x2 -- x2
{
	CELL x;
	cds( 2, 0 );
	x = __get(0);
	__put( 1, x );
	__ndrop(1);
}


i( OR ) // x1 x2 -- x1 | x2
{
	CELL x2;
	cds( 2, 0 );
	x2 = __pop();
	__put( 0, x2 | __get(0) );
}


i( OVER ) // x1 x2 -- x1 x2 x1
{
	CELL z;
	cds( 2, 1 );
	z = __get(1);
	__push( z );
}


i( PARSE ) // uchar "ccc<uchar>" -- a u
{
	uchar *start = source_a + to_in;
	uchar *p = start;
	uchar ch;
	cds( 1, 1 );
	if ( !source_a )
	{
		__put( 0, (CELL) pad );
		__push( 0 );
		return;
	}

	ch = (uchar) __get(0);
	__put( 0, (CELL)start );

	while ( to_in < source_u )
	{
		++ to_in;
		if (    (*p == ch)
		     || ((' ' == ch) && is_space(*p)) )
		{
			break;
		}
		++ p;
	}
	__push( (CELL) (p - start) );
}


i( PICK ) // xn .. x1 x0 n -- xn .. x1 x0 xn
{
	CELL k;
	cds( 1, 0 );
#if DEBUG
printf( "PICK (%d:%d): %d ", __get(0), dstack.occupied, __get(1+__get(0)) );
fflush(0);
#endif
	k = __get(0);
	++k;
	k = __get(k);
	__put( 0, k );
#if DEBUG
printf( "Ok " );
fflush(0);
#endif
}


i( POSTPONE ) // "<spaces>name<space>" --
{
	xt x;
	if ( !state ) Throw( -14 ); // interpreting a compile-only word
	F( tick );
	x = (xt) __get(0);
	if ( x->flags & _IMMEDIATE )
	{
		F( comma );
	} else {
		F( LITERAL );
		__push( (CELL) & hide_wl[i_comma] );
		F( comma );
	}
}


i( QUERY ) // --
{
	cds( 0, 2 );
	__push( (CELL) tib );
	__push( (CELL)(sizeof(tib) - 1) );
	_source_id = 0;
	source_a = tib;
	if ( !state )
	{
		printf( "> " );
	}
	F( ACCEPT );
	source_u = __pop();
	to_in = 0;
}


i( QUIT_ ) // --
{
	unsigned int s;
	do {
		F( REFILL );
		if ( !__pop() ) break;

		s = rstack.occupied;

		F( INTERPRET );

		if ( rstack.occupied < s )
		{
			Throw( -6 );
		} else if ( rstack.occupied > s )
		{
			Throw( -5 );
		}		
			
		if ( !state ) printf( " Ok" );
		F( CR );
		fflush(0);
	} while (1);
}


i( QUIT ) // --
{
	Throw( QUIT_ior );
}


i( r_from ) // R> 	-- x ;r x --
{
        push( rpop() );
}


i( r_fetch ) // R@	-- x ;r x -- x
{
	push( rget(0) );
}


i( RDROP ) // -- ;r x --
{
	rpop();
}


i( RECURSE ) //
{
	push( (CELL)last_xt );
	F( comma );
}


i( REFILL ) // -- f
{
	cds( 0, 3 );
	if ( 0 == _source_id )
	{
		if ( blk )
		{
			// read next block
			CELL ior;

			++ blk;
			__push( blk );
			ior = Catch( &hide_wl[i_BLOCK] );
			if ( ior )
			{
				__put( 0, _FALSE );
			} else {
				__put( 0, _TRUE );
				to_in = 0;
			}
		} else {
			// read line from terminal
			F( QUERY );
			if ( accepted )
			{
				__push( _TRUE );
				to_in = 0;
			} else {
				__push( _FALSE );
			}
		}
	} else if ( -1 == _source_id ) {
		// string (via EVALUATE)
		__push( _FALSE );
	} else {
		// read line from file
		__push( (CELL)source_a );
		__push( FIB_LEN );
		__push( _source_id );
		F( read_line );
		F( THROW );
		if ( __pop() )
		{
			source_u = __get(0);
			__put( 0, _TRUE );
			to_in = 0;
		} else {
			__put( 0, _FALSE );
		}
	}
}


i( REPEAT ) // C: orig dest --
{
	F( AGAIN );
	F( THEN );
/*
	cds( 4, 0 );

	if ( i_dest != __pop() ) Throw( -22 ); // control structure mismatch
	__push( (CELL) & hide_wl[i_doBRANCH] );
	F( comma );
	F( comma );

	if ( i_orig != __pop() ) Throw( -22 );
	*(*CELL)__pop() = (CELL) dp;
*/
}


i( restore_input ) // xn ... x1 n -- flag
{
	CELL n;

	cds( 6, 0 );

	n = __get(0);
	if ( 5 != n )
	{
		cds( n, 0 );
		__ndrop( n );
		__put(0, _TRUE);
	} else
	{
		_source_id = __get(5);
		to_in  = __get(4);
		blk = __get(3);
		source_a = (uchar*) __get(2);
		source_u = __get(1);
		__ndrop(5);
		__put(0, _FALSE);
	}
}


i( RSHIFT ) // u1 u2 -- u1 >> u2
{
	CELL u;
	cds( 2, 0 );
	u = __pop();
	__put( 0, ((uCELL)__get(0)) >> u );
}


i( ROLL ) // xn xn-1 .. x1 x0 n -- xn-1 .. x1 x0 xn
{
	CELL u, n;

	n = pop();
	cds( n+1, 0 );
	u = __get(n);

	while ( n )
	{
		__put( n, __get(n-1) );
		--n;
	}
	__put( 0, u );
}


i( ROT ) // x1 x2 x3 -- x2 x3 x1
{
	CELL t;
	cds( 3, 0 );
	t = __get(2);
	put( 2, __get(1) );
	put( 1, __get(0) );
	put( 0, t );
}


i( s_to_d ) // S>D 	n -- d
{
	cds( 1, 1 );
	dsign();
}


i( save_input ) // -- xn ... x1 n
{
	cds( 0, 6 );
	__push( (CELL)_source_id );
	__push( (CELL)to_in );
	__push( (CELL)blk );
	__push( (CELL)source_a );
	__push( (CELL)source_u );
	__push( (CELL)5 );
}


i( SIGN ) // n --
{
	if ( 0 > (sCELL)pop() )
	{
		__push( (CELL) '-' );
		F( HOLD );
	}
}


i( s_m_slash_rem ) // SM/REM	d n -- n1 n2
{
	F( NIP );
	F( slash_mod );
}


i( SOURCE ) // -- a u
{
	cds( 0, 2 );
	__push( (CELL)source_a );
	__push( source_u );
}


i( SPACE ) // --
{
	putchar( ' ' );
}


i( SPACES ) // n --
{
	sCELL n = pop();
	while ( n > 0 )
	{
		putchar( ' ' );
		--n;
	}
}


i( SWAP ) // x1 x2 -- x2 x1
{
	CELL t;
	cds( 2, 0 );
	t = __get(0);
	__put( 0, __get(1) );
	__put( 1, t );
}


i( THEN ) // C: orig --
{
	cds( 2, 0 );
	if ( i_orig != __pop() ) Throw( -22 ); // control structure mismatch
	*(CELL*)__pop() = (CELL) dp;
}


i( TO ) // "<spaces>name<space>"  x --
{
	xt x;
	F( tick );
	x = (xt) __pop();
	if ( (x->f != doVALUE)
	     && (x->f != doQUAN) )
	{
		Throw( -32 ); // invalid name argument
	}
	if ( state )
	{
		__push( (CELL) (x->pfa) );
		F( LITERAL );
		__push( (CELL) & hide_wl[i_store] );
		F( comma );
	} else
		*((CELL*)(x->pfa)) = __pop();
}


i( TUCK ) // x1 x2 -- x2 x1 x2
{
	CELL x;
	cds( 2, 1 );
	x = __get(0);
	__push( x );
	__put( 1, __get(2) );
	__put( 2, __get(0) );
}


i( TYPE ) // a u --
{
	CELL u;
	uchar *a;

	cds( 2, 0 );
	u = __get(0);
	a = (uchar*) __get(1);
	while ( u )
	{
		putchar( *a );
		++a; --u;
	}
	__ndrop(2);
}


i( u_dot ) // U.		u --
{
	push( 0 );
	F( less_number_sign );
	F( number_sign_s );
	F( number_sign_greater );
	F( TYPE );
	F( SPACE );
}


i( u_dot_r ) // U.R      	u1 u2 --
{
	rpush( pop() );
	__push( 0 );
	F( less_number_sign );
	F( number_sign_s );
	F( number_sign_greater );  // a u --
	push( __rpop() );          // a u u2 --
	F( OVER );
	F( minus );
	F( SPACES );
	F( TYPE );
}


i( u_less_than ) // U<   	u1 u2 -- u1 < u2
{
	uCELL u2;
	cds( 2, 0 );
	u2 = (uCELL) __pop();
	__put( 0, ((uCELL) __get(0)) < u2 ? _TRUE : _FALSE );
}


i( u_greater_than ) // U>	u1 u2 -- u1 > u2
{
	F( SWAP );
	F( u_less_than );
}


i( u_m_star ) // UM* 		u1 u2 -- ud
{
	F( mul );
	dsign();
}


i( u_m_slash_mod ) // UM/MOD	du u -- u1 u2
{
	uCELL du, u;
	cds( 3, 0 );
	u = (uCELL)__get(0);
	du = (uCELL)__get(2);
	__ndrop(1);
	__put( 1, du % u );
	__put( 0, du / u );
}


i( UNTIL ) // f --  C: dist --
{
	cds( 2, 0 );
	if ( i_dest != __get(0) ) Throw( -22 ); // control structure mismatch
	__put( 0, (CELL) & hide_wl[i_do_q_branch] );
	F( comma );
	F( comma );
}


i( UNUSED ) // -- u
{
	push( ADD_DATA_SIZE + RES_DATA_SIZE - (dp - current_data_space) );
}


i( VALUE ) // "<spaces>name<space>"  x --
{
	F( CREATE );
	last_xt->f = doVALUE;
	last_xt->pfa = (CELL)dp;
	F( comma );
	check_data_space(0);
}


i( VARIABLE ) // "<spaces>name<space>" --
{
	F( CREATE );
	allot( sizeof(CELL) );
	last_xt->f = doVAR;
	check_data_space(0);
}


i( WHILE ) // f --  C: dist -- orig dist
{
	F( IF );
	F( two_swap );
}


i( WITHIN ) // n1|u1 n2|u2 n3|u3 -- f
// u2 <= u1 < u3   0 <= u1-u2 < u3-u2
{
	F( OVER );
	F( minus );
	F( ROT ); F( ROT );
	F( minus );
	F( SWAP );
	F( u_less_than );
}


i( WORD ) // uchar "<uchars>ccc<uchar>" -- a
{
	int i = 0;
	uchar *p = &pad[1];

	cds( 1, 0 );
	if ( source_a )
	{
		uchar *start = source_a + to_in;
		uchar ch = (uchar) __get(0);

		while ( (to_in < source_u) 
		        && ( (*start == ch)
		             || ((' ' == ch) && is_space(*start)) )
		      )
		{
			++ to_in;
			++ start;
		}
		while ( to_in < source_u )
		{
			++ to_in;
			if (    (*start == ch) 
			     || (' ' == ch && is_space(*start)) )
			{
				break;
			}

			++ i;
			if ( (UCHAR_MAX < i)
			     || ((sizeof(pad)/sizeof(pad[0]) - 2) < i) )
			{
				Throw( -18 ); // parsed string overflow
			}
			*p = *start;
			++p; ++start;
		}
	}

	*p = ' ';
	pad[0] = (uchar) i;
	__put( 0, (CELL) pad );
}


i( XOR ) // x1 x2 -- x1 ^ x2
{
	CELL x2;
	cds( 2, 0 );
	x2 = __pop();
	__put( 0, x2 ^ __get(0) );
}


i( left_bracket ) // [  	--
{
	state = _FALSE;
}


i( bracket_tick ) // ['] 	-- xt
{
	F( tick );
	F( LITERAL );
}


i( right_bracket ) // ] 	--
{
	state = _TRUE;
}


i( backslash ) // \ 	--
{
	if ( (_source_id == 0) && blk )
	{
		unsigned t = to_in % 64;
		if ( t )
			to_in += 64 - t;
	} else
		to_in = source_u;
}


// End of file
