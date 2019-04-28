// Copyleft  by Potrepalov I.S.

// ---------------------
/* Exception word set */

i( ABORT ) // i*x --
{
	Throw( -1 );
}


i( do_abort_quote ) // x --
{
	abort_text_len = pop();
	abort_text = (uchar*) pop();
	if ( pop() ) Throw( -2 );
}


i( abort_quote ) // ABORT" 	"text<">" --
{
	push( (CELL) '"' );
	F( PARSE );
	F( SLITERAL );
	push( (CELL) & hide_wl[i_do_abort_quote] );
	F( comma );
}


i( CATCH ) // i*x xt -- y*x' 0 / i*x xt -- i*x' ior
{
	push( Catch((xt)pop()) );
}


i( THROW ) // ior --
{
	Throw( pop() );
}


// End of file
