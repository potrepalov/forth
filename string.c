// Copyleft  by Potrepalov I.S.

// ------------------
/* String word set */

i ( minus_trailing ) // -TRAILING 	a u -- a u1
{
	uchar *p;
	CELL u;

	cds( 2, 0 );

	u = __get( 0 );
	p = (uchar*) __get( 1 );
	p += u;
	while ( u && (' ' == *(--p)) )
		--u;
	__put( 0, u );
}


i( slash_string ) // /STRING    	a u n -- a+n u-n
{
	CELL n;

	cds( 3, 0 );

	n = __pop();
	__put( 0, __get(0)-n );
	__put( 1, __get(1)+n );
}


i( BLANK ) // a u --
{
	push( ' ' );
	F( FILL );
}


i( CMOVE ) // a1 a2 u --
{
	uchar *from, *to;
	uCELL u;
	cds( 3, 0 );
	u = __get(0);
	to = (uchar*)__get(1);
	from = (uchar*) __get(2);
	__ndrop(3);
	while ( u )
		*(to++) = *(from++), --u;
}


i( cmove_up ) // CMOVE> 	a1 a2 u --
{
        uchar *a1, *a2;
        uCELL u;

	cds( 3, 0 );

	u = __get(0);
	a2 = (uchar*)__get(1);
	a1 = (uchar*)__get(2);
	__ndrop(3);
	if ( u )
	{
		a1 += u; a2 += u;
		do
			*(--a2) = *(--a1);
		while ( --u );
	}
}


i( COMPARE ) // a1 u1 a2 u2 -- n
{
	uCELL u1, u2;
	int r;

	cds( 4, 0 );

	u2 = __get(0);
	u1 = __get(2);
	if ( u1 < u2 )
		r = memcmp( (void*)__get(3), (void*)__get(1), u1 );
        	// FIXME: when sizeof(size_t) < sizeof(CELL) ...
	else
		r = memcmp( (void*)__get(3), (void*)__get(1), u2 );

	if ( !r )
		r = u1 - u2;

	if ( !r )
		u1 = 0;
	else if ( r < 0 )
		u1 = -1;
	else
		u1 = 1;

	__put( 3, u1 );
	__ndrop(3);
}

i( SEARCH ) // a1 u1 a2 u2 -- a3 u3
{
	uchar *a1, *a2;
	uCELL u1, u2;

	cds( 4, 0 );

	u2 = __get(0);
	a2 = (uchar*) __get(1);
	u1 = __get(2);
	a1 = (uchar*) __get(3);

	if ( u1 >= u2 )
	{
		uCELL ic = u1 - u2 + 1;
		do {
                	// FIXME: when sizeof(size_t) < sizeof(CELL) ...
			if ( !memcmp(a1,a2,u2) )
			{
				__put( 3, (CELL)a2 );
				__put( 2, (CELL)(u2 + ic - 1) );
				ic = 1;
			} else
				++ a1;
		} while ( -- ic );
	}
	__ndrop(2);
}


i( doSLITERAL ) // -- a u
{
        uCELL l;

	cds( 0, 2 );

	l = *(CELL*)IP;
	IP = (xt*)(((CELL*)IP)+1);
#if DEBUG
printf( "len of string: %d (%s) ", (int)l, (char*) IP );
fflush(0);
#endif
	__push( (CELL)IP );
	__push( (CELL)l );
#if DEBUG
printf( "IPpre=%d ,", (int)IP );
#endif
	IP = (xt*) align( ((uchar*)IP) + 1 + l );
#if DEBUG
printf( "IPaligned=%d ", (int)IP );
#endif
}


i( SLITERAL ) // a u --
{
	CELL u;
	cds( 2, 0 );

#if DEBUG
F( two_dup );
F( TYPE );
F( SPACE );
fflush(0);
#endif

	u = __get(0);

	if ( state )
	{
		xt *tmp = (xt*)dp;
		allot( sizeof(CELL) );
		*tmp = &hide_wl[i_doSLITERAL];

		F( comma );

		tmp = (xt*)dp;
		allot( u );
		memcpy( (uchar*)tmp, (uchar*)__get(0), u );
		// FIXME: when sizeof(size_t) < sizeof(CELL) ...
		__put( 0, 0 );
		F( c_comma );
		F( ALIGN );
	} else {
		static uchar buf[520];
		if ( u > sizeof(buf)-2 ) u = sizeof(buf)-2;
		memcpy( buf, (uchar*) __get(1), u );
		buf[u] = '\0';
		__put(0, u);
		__put(1, (CELL)buf);
	}
}


// End of file
