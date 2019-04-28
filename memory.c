// Copyleft  by Potrepalov I.S.

// -----------------------------
/* Memory allocation word set */

i( ALLOCATE ) // u -- a ior
{
	cds( 1, 1 );
	errno = 0;
	__put( 0, (CELL) malloc((size_t)__get(0)) );
	// FIXME: when sizeof(size_t) < sizeof(CELL) ...
	__push( (CELL) errno );
}


i( FREE ) // a -- ior
{
	cds( 1, 0 );
	errno = 0;
	free( (void*)__get(0) );
	__put( 0, (CELL)errno );
}


i( RESIZE ) // a u -- a2 ior
{
	cds( 2, 0 );
	errno = 0;
	__put( 1, (CELL)realloc((void*)__get(1),(size_t)__get(0)) );
	// FIXME: when sizeof(size_t) < sizeof(CELL) ...
	__put( 0, errno );
}


// End of file
