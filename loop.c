// Copyleft  by Potrepalov I.S.

// --------------------
/* +LOOP ?DO DO I J LOOP */

/*
 * Data space (code file):
 *
 *     doDO   ( do_q_do )
 * +-- link
 * |     loop body
 * |     .........
 * |   doLOOP ( do_plus_loop )
 * +->
 *
 *
 * Run time return stack (loop-sys):
 *
 * TOS:  I - limit
 *       limit
 *       start loop body address (may be CONTINUE will be usefull...)
 *       link (for LEAVE)
 *       ........
 */

i( do_plus_loop ) //	n -- ;r loop-sys1 -- loop-sys2 | loop-sys1 --
{
	sCELL n = pop();
	sCELL lc;
	crs( 4, 0 );
	lc = __rget(0);
	if ( lc < 0 )
	{
		lc += n;
		if ( lc >= 0  &&  n > 0 ) { F( LEAVE ); }
		else { __rput( 0, lc ); IP = (xt*) __rget(2); }
	} else {
		lc += n;
		if ( lc < 0  &&  n < 0 ) { F( LEAVE ); }
		else { __rput( 0, lc ); IP = (xt*) __rget(2); }
	}
}


i( plus_loop ) // +LOOP 	C: do-sys --
{
	xt *tmp = (xt*)dp, *tmp2;
	cds( 2, 0 );
	if ( __get(0) != i_doDO ) Throw( -22 ); // control structure mismatch
	__put( 0, (CELL) & hide_wl[i_do_plus_loop] );
	F( comma );
	*(CELL*)__pop() = (CELL)dp;
}


i( do_q_do ) // n1 n2 -- ;R -- loop-sys | n1 n1 -- ;R --
{
	CELL start, limit;
	cds( 2, 0 ); crs( 0, 4 );
	start = __get(0);
	limit = __get(1);
	if ( start != limit )
	{
		__rpush( (CELL) *IP );
		__rpush( (CELL) ++IP );
		__rpush( limit );
		__rpush( start - limit );
	} else {
		IP = (xt*) *(IP);
	}
	__ndrop(2);
}


i( q_do ) // ?DO 	C: -- do-sys
{
	cds( 0, 2 );
	__push( (CELL) &hide_wl[i_do_q_do] );
	F( comma );
	__push( (CELL) dp );
	dp = (uchar*) (((CELL*)dp) + 1);
//	++(CELL*)dp;
	__push( i_doDO );
}


i( doDO ) // n1 n2 -- ;r -- loop-sys
{
	CELL limit;
	cds( 2, 0 ); crs( 0, 4 );
#if DEBUG
printf( "Start: %d, limit %d", (int)__get(0), (int) __get(1) );
#endif
	__rpush( (CELL) *IP );
	__rpush( (CELL) ++IP );
	limit = __get(1);
	__rpush( limit );
	__rpush( __get(0) - limit );
	__ndrop(2);
}


i( DO ) // C: -- do-sys
{
	cds( 0, 2 );
	__push( (CELL) & hide_wl[i_doDO] );
	F( comma );
	__push( (CELL) dp );
	dp = (uchar*) (((CELL*)dp) + 1);
//	++(CELL*)dp;
	__push( i_doDO );
}


i( I ) // -- n ;r loop-sys -- loop-sys
{
	crs( 4, 0 );
	push( __rget(0) + __rget(1) );
}


i( J ) // -- n ;r loop-sys1 loop-sys2 -- loop-sys1 loop-sys2
{
	crs( 8, 0 );
	push( __rget(4) + __rget(5) );
}


i( LEAVE ) // -- ;r loop-sys --
{
	crs( 4, 0 );
	__rpop(); __rpop(); __rpop();
	IP = (xt*)__rpop();
}



i( doLOOP ) // -- ;r loop-sys1 -- loop-sys2 | loop-sys1 --
{
	sCELL lc;
	crs( 3, 0 );
	lc = __rget(0) + 1;
	if ( lc ) { __rput( 0, lc ); IP = (xt*) __rget(2); }
	else F( LEAVE );
}


i( LOOP ) // C: do-sys --
{
	cds( 2, 0 );
	if ( __get(0) != i_doDO ) Throw( -22 ); // control structure mismatch
	__put( 0, (CELL) & hide_wl[i_doLOOP] );
	F( comma );
	*(CELL*)__pop() = (CELL)dp;
}


i( UNLOOP ) // --  ;r loop-sys --
{
	crs( 4, 0 );
	__rpop(); __rpop(); __rpop(); __rpop();
}


// End of file
