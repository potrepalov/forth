// Copyleft  by Potrepalov I.S.

// ------------------------
/* Search order word set */


int context_size = 0;

xt wordlists[1+NUMBER_OF_WORDLISTS];

void destroy_wordlists( void )
{
	xt *p = & wordlists[NUMBER_OF_WORDLISTS];
	do
	{
		xt tmp = *p;
		while ( tmp && (tmp != forth_wl) && (tmp != min_wl) )
		{
			xt t = tmp;
			tmp = *(xt*)tmp;
			free( t );
		}
	} while ( --p >= wordlists );
}


void init_wordlists( void )
{
        int z = 1;
        while ( forth_wl[z].f ) ++z;
	forth_wl[0].flags = forth_wl[0].pfa = z - 1;

	z = 1;
        while ( min_wl[z].f ) ++z;
	min_wl[0].flags = min_wl[0].pfa = z - 1;

        memset( wordlists, 0, sizeof(wordlists) );
	wordlists[0] = forth_wl;
	wordlists[1] = min_wl;
	atexit( destroy_wordlists );
	context_size = 1;
	context[0] = 1;
	current = 1;
}



i( ALSO ) // --
{
	int z;
	if ( sizeof(context)/sizeof(context[0]) <= context_size )
	{
		Throw( -49 ); // search order overflow
	}
	z = ++ context_size;
	do {
		context[z] = context[z-1];
	} while ( --z );
}


i( DEFINITIONS ) // --
{
	current = context[0];
}


i( _FIND ) // a u -- 0 | a u -- xt 1 | a u -- xt -1
{
	uchar *a;
	int z;
	uCELL l;
	int forth = 0;

	cds( 2, 1 );

#if DEBUG
printf( "\nTry to find: " );
F( two_dup );
F( TYPE );
F( CR );
fflush(NULL);
#endif

	l = __pop();
	a = (uchar*)__get(0);
	z = 0;
	while ( z < context_size )
	{
		if ( 1 == context[z] )
			forth = 1;
		__put( 0, (CELL)a );
		__push( l );
		__push( context[z] );
		F( search_wordlist );
		if ( __get(0) )
			return;
		++z;
	}

	if ( !forth )
	{
		// search in  BYE ONLY FORTH FORTH-WORDLIST SET-ORDER
		__put( 0, (CELL)a );
		__push( l );
		__push( 2 );
		F( search_wordlist );
	}
}


i( FIND ) // a -- a 0 | a -- xt 1 | a -- xt -1
{
	uchar *a;
	uchar c;

	cds( 1, 1 );

	a = (uchar*)__get(0);
	c = *a;
	++a;
	__put( 0, (CELL)a );
	__push( (CELL) c );
	F( _FIND );
	if ( !__get(0) )
	{
		__put( 0, (CELL)(--a) );
		__push( 0 );
	}
}


i( FORTH ) // --
{
	context[0] = 1;
}


i( forth_wordlist ) // FORTH-WORDLIST	-- wid
{
	push( 1 );
}


i( get_current ) // GET-CURRENT 	-- wid
{
	push( current );
}


i( get_order ) // GET-ORDER     	-- widn .. wid2 wid1 n
{
	int z = context_size;
	while ( z ) push( context[--z] );
	push( context_size );
}


i( ONLY ) // --
{
        if ( !context_size )
        {
        	F( FORTH );
	}
	context_size = 1;
}


void print_wl( int wid )
{
	struct _xt * x = wordlists[wid-1];
	int i = 0;
	while ( x )
	{
		struct _xt * z = x + x->pfa;
		while ( z != x )
		{
			int l = strlen( (char*)z->name );
			
			if ( l )
			{
			        if ( (i + 15-i%15) > 70 )
				{
					putchar( '\n' );
					i = 0;
				}
				while ( i%15 )
				{
					putchar( ' ' );
					++i;
				}
				printf( "%s ", z->name );
				i += l + 1;
			}
			-- z;
		}
		x = (struct _xt*) (x->f);
	}
	if ( i )
	{
		putchar( '\n' );
	}
}

i( ORDER ) // --
{
	int i = 0;
	int forth = 0;
	printf( "\nsearch order:\n" );
	while ( i < context_size )  // context[i] )
	{
		print_wl( context[i] );
		if ( 1 == context[i] ) forth = 1;
		++i;
	}
	if ( !forth ) print_wl( 2 );

	printf( "\n\ncurrent wordlist:\n" );
	if ( current ) print_wl( current );

	printf( "\n\n" );
}


i( PREVIOUS ) // --
{
	int z;
	if ( !context_size ) Throw( -50 ); // search order underflow
	-- context_size;
	z = 0;
	while ( z < context_size )
	{
		context[z] = context[z+1];
		++ z;
	}
	context[z] = 0;
}


i( search_wordlist ) // SEARTH-WORDLIST
//	a u wid -- 0 | a u wid -- xt 1 | a u wid -- xt -1
{
	CELL wid, u;
	cds( 3, 0 );
	wid = __get(0);
	u = __get(1);
	if ( wid && (wid <= NUMBER_OF_WORDLISTS) && (u<32) )
	{
		xt t = wordlists[wid-1];
		uchar name[32];
		memcpy( name, (uchar*)__get(2), u );
		name[u] = '\0';
		++u;

#if DEBUG
printf( "Will search: %s\n", name );
fflush( NULL );
#endif

		while ( t )
		{
			xt tt = t->pfa + (struct _xt*)t; //((xt*)t)[t->pfa];
			while ( tt != t )
			{
				if ( !(tt->flags & _SMUDGE)
				     && tt->name
				     && !memcmp(name, tt->name, u) )
				{
					__ndrop(1);
					__put( 1, (CELL) tt );
					__put( 0, tt->flags & _IMMEDIATE ? 1 : -1 );
					return;
				}
				--tt;
			}
			t = (xt)(t->f);
		}
	}
	__ndrop(2);
	__put( 0, 0 );
#if DEBUG
printf( "Search was fall.\n" );
fflush( NULL );
#endif
}


struct _xt * get_chunk( void )
{
	struct _xt *ret;
	ret = malloc( sizeof(struct _xt) * 51 ); // FIXME  50 - minimum number of words in list
	if ( !ret ) Throw( -8 ); // dictionary overflow
	memset( ret, 0, sizeof(struct _xt) * 51 );
	ret->flags = 50;
#if DEBUG
printf( "chunk gotten " );
fflush(0);
#endif
	return ret;
}


xt get_xt( int wid )
{
	struct _xt *t, *cur;
	// cur[0].pfa - last occupied xt in this chunk
	// cur[0].flags - number of xts in this chunk
	// cur[0].f - link to prev chunk

	if ( !wid || (wid > NUMBER_OF_WORDLISTS) )
		Throw( -47 ); // compilation word list deleted

	cur = wordlists[wid-1];
	if ( !cur ) cur = get_chunk();
	else if ( cur->pfa >= cur->flags )
	{
		t = get_chunk();
		t->f = (vfp) cur;
		cur = t;
	}
	wordlists[wid-1] = cur;

	++(cur->pfa);
#if DEBUG
printf( "in current chunk %d words ", (int)cur->pfa );
fflush(0);
#endif
	return (last_xt = & (cur[cur->pfa]));
}


i( WORDLIST ) // -- wid
{
        CELL z = 1;
        do {
        	if ( !wordlists[z] )
        	{
        		xt x = get_xt( z+1 ); // waste 1 xt
        		x->f = noop;
        		push( z+1 );
        		return;
        	}
        	++ z;
        } while ( z <= NUMBER_OF_WORDLISTS );
        Throw( -49 );
}


i( WORDS ) // --
{
	if ( context[0] )
	{
		print_wl( context[0] );
	}
}


i( set_current ) // SET-CURRENT 	wid --
{
	CELL p = pop();
	if ( !p 
	     || (p > NUMBER_OF_WORDLISTS)
	     || !wordlists[p-1]
	   )
	{
		Throw( -24 ); // invalid numeric argument
	}
	current = p;
}


i( set_order ) // SET-ORDER     	widn .. wid2 wid1 n --
{
	CELL z = pop();
	if ( z <= 0 )
	{
		F( ONLY );
		return;
	}

	if ( z > sizeof(context)/sizeof(context[0]) )
		Throw( -49 ); // search-order overflow

	context_size = z;
	z = 0;
	while ( z < context_size )
	{
		CELL p = pop();
		if ( !p 
		     || (p > NUMBER_OF_WORDLISTS)
		     || !wordlists[p-1]
		   )
		{
			Throw( -24 ); // invalid numeric argument
		}
		context[z++] = p;
	}
}


// End of file
