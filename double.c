// Copyleft  by Potrepalov I.S.

// -------------------------
/* Double number word set */



i( two_literal ) // 2LITERAL	C: x1 x2 --   R: -- x1 x2
{
	if ( state )
	{
		F( SWAP );
		F( LITERAL );
		F( LITERAL );
	}
}


i( two_rot ) // 2ROT	d1 d2 d3 -- d2 d3 d1
{
	F( two_to_r );
	F( two_swap );
	F( two_r_from );
	F( two_swap );
}


i( two_variable ) // 2VARIABLE  "<spaces>name<space>" --
{
	F( CREATE );
	allot( 2 * sizeof(CELL) );
	last_xt->f = doVAR;
	check_data_space( 0 );
}


i( d_plus ) // D+       d1 d2 -- d1+d2
{
	cds( 4, 0 );
	__put( 3, (sCELL)__get(3) + (sCELL)__get(1) );
	__ndrop(3);
	dsign();
}


i( d_minus ) // D- 	d1 d2 -- d1-d2
{
	cds( 4, 0 );
	__put( 3, (sCELL)__get(3) - (sCELL)__get(1) );
	__ndrop(3);
	dsign();
}


i( d_dot ) // D.		d --
{
	rpush( get(0) );
	F( DABS );
	F( less_number_sign );
	F( number_sign_s );
	push( __rpop() );
	F( SIGN );
	F( number_sign_greater );
	F( TYPE );
	F( SPACE );
}


i( d_dot_r ) // D.R		d n --
{
	F( to_r );
	rpush( get(0) );
	F( DABS );
	F( less_number_sign );
	F( number_sign_s );
	__push( __rpop() );
	F( SIGN );
	F( number_sign_greater );
	__push( __rpop() ); // a u n --
	F( OVER );
	F( minus );
	F( SPACES );
	F( TYPE );
}


i( d_zero_less ) // D0<	 	d -- f
{
	pop();
	F( zero_less );
}


i( d_zero_equal ) // D0= 	d -- f
{
	F( OR );
	F( zero_equal );
}


i( d_two_star ) // D2* 		d -- d*2
{
	cds( 2 ,0 );
	__ndrop(1);
	__put( 0, 2*(sCELL)__get(0) );
	dsign();
}


i( d_two_slash ) // D2/   	d -- d/2
{
	cds( 2, 0 );
	__put( 1, ((sCELL)__get(1))/2 );
}


i( d_less_than ) // D<   	d1 d2 -- f
{
	F( DROP );
	F( NIP );
	F( less_than );
}


i( d_equal ) // D=   		d1 d2 -- f
{
	CELL r = _FALSE;

	cds( 4, 0 );

	if ( (__get(0) == __get(2))
	     && (__get(1) == __get(3)) )
	{
		r = _TRUE;
	}

	__ndrop(3);
	__put(0, r);
}


i( DABS ) // d -- |d|
{
	F( DROP );
	F( ABS );
	dsign();
}


i( DMAX ) // d1 d2 -- max(d1,d2)
{
	F( DROP );
	F( NIP );
	F( MAX );
	dsign();
}


i( DMIN ) // d1 d2 -- min(d1,d2)
{
	F( DROP );
	F( NIP );
	F( MIN );
	dsign();
}


i( DNEGATE ) // d -- -d
{
	F( DROP );
	F( NEGATE );
	dsign();
}


i( d_u_less_than ) // DU<	ud1 ud2 -- f
{
	F( DROP );
	F( NIP );
	F( u_less_than );
}


i( m_plus ) // M+	d1 n -- d1+n
{
	F( NIP );
	F( plus );
	dsign();
}


// End of file
