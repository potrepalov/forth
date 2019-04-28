// Copyleft  by Potrepalov I.S.

// -----------------------
/* File access word set */

#define FAM_RO	1
#define FAM_WO	2
#define FAM_RW	(FAM_RO|FAM_WO)
#define FAM_BIN	8


i( BIN ) // fam1 -- fam2
{
	cds( 1, 0 );
	__put( 0, FAM_BIN | __get(0) );
}


i( close_file ) // fileid -- ior
{
	cds( 1, 0 );
	if ( !fclose((FILE*)__get(0)) )
		__put( 0, 0 );
	else if ( EBADF == errno )
		__put( 0, -24 ); // invalid numeric argument
	else
		__put( 0, (CELL) errno );
}


i( create_file ) // a u fam -- fileid ior
{
	size_t u;
	char *p;
	unsigned int fam;
	char cfam[6];
	FILE *f;

	cds( 3, 0 );

	fam = __pop();
	u = (size_t) __get(0);
	p = malloc( u + 1 );
	if ( !p )
	{
		__put( 0, errno );
		return;
	}
	memcpy( p, (uchar*)__get(1), u );
	p[u] = '\0';

	cfam[0] = 'w';
	u = 0;
	if ( FAM_BIN & fam )
		cfam[++u] = 'b';
	if ( FAM_WO != (fam & 7) )
		cfam[++u] = '+';
	cfam[++u] = '\0';

	f = fopen( p, cfam );

	__put( 1, (CELL) f );
	if ( f )
		__put( 0, 0 );
	else if ( ENOENT == errno )
		__put( 0, -38 ); // non-existent file
	else
		__put( 0, errno );

	free(p);
}


i( delete_file ) // a u -- ior
{
	char *p;
	size_t u;

	cds( 2, 0 );

	u = (size_t) __pop();
	p = malloc( u + 1 );
	if ( !p )
	{
		__put( 0, errno );
		return;
	}
	memcpy( p, (void*) __get(0), u );
	p[u] = '\0';

	if ( !remove( p ) )
		__put( 0, 0 );
	else if ( ENOENT == errno )
		__put( 0, -38 ); // non-existent file
	else
		__put( 0, errno );

	free( p );
}


i( file_position ) // fileid -- ud ior
{
	long r;
 	cds( 1, 2 );
	r = ftell( (FILE*)__get(0) );
	if ( -1 == r )
	{
		__put( 0, -1 );
		__push( -1 );
		if ( EBADF == errno )
			__push( -24 ); // invalid numeric argument
		else
			__push( errno );
	} else {
		__put( r, 0 );
		__push( 0 );
		__push( 0 );
	}
}


i( file_size ) // fileid -- ud ior
{
	FILE *f;
	fpos_t cur;

	cds( 1, 2 );

	f = (FILE*) __get(0);
	if ( !fgetpos(f, &cur)
	     && !fseek(f, 0, SEEK_END) )
	{
		F( file_position );
		if ( fsetpos(f, &cur) )
			__put( 0, -37 ); // file I/O exception
		return;
	}

	__put( 0, -1 );
	__push( -1 );
	if ( EBADF == errno )
		__push( -24 ); // invalid numeric argument
	else
		__push( errno );
}




i( file_status ) // a u -- x ior
{
	cds( 2, 0 );
	__put( 1, 0 );
	__put( 0, -21 ); // unsupported operation
}


i( flush_file ) // fileid -- ior
{
	cds( 1, 0 );
	if ( fflush( (FILE*)__get(0)) )
	{
		if ( EBADF == errno )
			__put( 0, -24 ); // invalid numeric argument
		else
			__put( 0, errno );
	}
	else
		__put( 0, 0 );
}


i( include_file ) // i*x fileid -- j*x
{
	int y, z;
	uchar fib[FIB_LEN + 2];

	F( save_input );
	z = y = __pop();

	crs( 0, y );

	while ( y )
	{
		__rpush( __pop() );
		-- y;
	}

	_source_id = pop();
	source_a = fib;

	do {
		F( REFILL );
		if ( !__pop() )
			break;
		F( INTERPRET );
	} while (1);

	y = z;
	cds( 0, y+1 );
	crs( y, 0 );
	while ( y )
	{
		__push( __rpop() );
		-- y;
	}
	push( z );
	F( restore_input );
	__ndrop(1);
}


i( INCLUDED ) // i*x a u -- j*x
{
	CELL fid;
	push( FAM_RO /*| FAM_BIN*/ );
	F( open_file );
	F( THROW );
	fid = __get(0);
	F( include_file );
	push( fid );
	F( close_file );
	F( THROW );
}


i( open_file ) // a u fam -- fileid ior
{
	char *p;
	CELL u;
	CELL fam;
	char cfam[6];
	FILE *f;

	cds( 3, 0 );

	fam = __pop();
	u = 0;
	if ( FAM_RO & fam )
		cfam[0] = 'r';
	else if ( FAM_WO & fam )
		cfam[0] = 'w';
	else
	{
		__put( 0, -24 ); // invalid numeric argument
		return;
	}
	if ( FAM_BIN & fam )
		cfam[++u] = 'b';
	if ( FAM_RW == (fam & 7) )
		cfam[++u] = '+';
	cfam[++u] = '\0';

	u = __get(0);
	p = malloc( u + 1 );
	if ( !p )
	{
		__put( 0, errno );
		return;
	}
	memcpy( p, (uchar*)__get(1), u );
	p[u] = '\0';

	f = fopen( p, cfam );

	__put( 1, (CELL) f );
	if ( f )
		__put( 0, 0 );
	else if ( ENOENT == errno )
		__put( 0, -38 ); // non-existent file
	else
		__put( 0, errno );

	free(p);
}


i( read_file ) // a u fileid -- u2 ior
{
	FILE *f;
	size_t r;
	int se = errno;

	cds( 3, 0 );

	f = (FILE*) __pop();
	clearerr( f );

	errno = 0;
	__put( 1, fread((void*)__get(1), 1, __get(0), f) );
	// FIXME: when sizeof(size_t) < sizeof(CELL) ...
	if ( feof(f) || !errno )
		__put( 0, 0 );
	else if ( EBADF == errno )
		__put( 0, -24 ); // invalid numeric argument
	else if ( ENOMEM == errno )
		__put( 0, ENOMEM );
	else
		__put( 0, -37 ); // file I/O exception

	errno = se;
}


i( read_line ) // ac u1 file_id -- u2 f ior
{
	FILE *fid;
	CELL u1;
	uchar *ac;
	int ch;
	int s = 0;
	int se = errno;

	cds( 3, 0 );

	fid = (FILE*)__get(0);
	u1 = __get(1);
	ac = (uchar*)__get(2);

	clearerr( fid );
	errno = 0;

	while ( u1 )
	{
		ch = fgetc( fid );

		if ( EOF == ch )
		{
			if ( feof(fid) || !errno ) break;
			__put( 2, ac - (uchar*)__get(2) );
			__put( 1, _FALSE );
			if ( EBADF == errno )
				__put( 0, -24 ); // invalid numeric argument
			else if ( ENOMEM == errno )
				__put( 0, ENOMEM );
			else
				__put( 0, -37 ); // file I/O exception
			errno = se;
			return;
		}

		if ( 10 == ch ) break;
		*ac = (uchar)ch;
		++ ac;
		-- u1;
	}
	if ( (0 != (ac - (uchar*)__get(2))) || !feof(fid) )
	{
		__put( 2, ac - (uchar*)__get(2) );
		__put( 1, _TRUE );
	} else
	{
		__put( 2, 0 );
		__put( 1, _FALSE );
	}
	__put( 0, 0 );
	errno = se;
	return;
}


i( reposition_file ) // ud f -- ior
{
	FILE *f;
	cds( 3, 0 );

	f = (FILE*) __pop();

	if ( !fseek(f, (long)__pop(), SEEK_SET) )
		__put( 0, 0 );
	else if ( (EINVAL == errno)
	          || (EBADF == errno) )
	{
		__put( 0, -24 ); // invalid numeric argument
	} else
		__put( 0, errno );
}


i( resize_file ) // ud f -- ior
{
	cds( 3, 0 );
	__ndrop(2);
	__put( 0, -21 ); // unsupported operation
}


i( rename_file ) // a1 u1 a2 u2 -- ior
{
	char *old = NULL, *new = NULL;
	uCELL u1, u2;
	int se = errno;
	cds( 4, 0 );

	u1 = __get(2);
	u2 = __get(0);

	old = malloc( u1 + 1 ); // FIXME: sizeof(size_t) < sizeof(CELL)
	new = malloc( u2 + 1 );

	if ( !old || !new )
	{
		errno = se;
		__put( 3, ENOMEM );
	} else
	{
		memcpy( old, (uchar*)__get(3), u1 ); // FIXME
		memcpy( new, (uchar*)__get(1), u2 );
		old[u1] = '\0';
		new[u2] = '\0';

		if ( rename(old, new) )
			__put( 3, errno );
		else
			__put( 3, 0 );
	}
	if ( new ) free(new);
	if ( old ) free(old);
	__ndrop(3);
}


i( write_file ) // a u fileid -- ior
{
	FILE *f;
	size_t r;
	CELL u;
	int se = errno;

	cds( 3, 0 );

	f = (FILE*)__pop();
	u = __pop();

	clearerr( f );
	r = fwrite( (void*) __get(0), 1, u, f );
	// FIXME: when sizeof(size_t) < sizeof(CELL) ...

	if ( r == u )
		__put( 0, 0 );
	else if ( EBADF == errno )
		__put( 0, -24 ); // invalid numeric argument
	else if ( EFBIG == errno )
		__put( 0, -36 ); // invalid file position
	else if ( ENOMEM == errno )
		__put( 0, ENOMEM );
	else
		__put( 0, -37 ); // file I/O exception

	errno = se;
}


i( write_line ) // a u fileid -- ior
{
	FILE *f;
	cds( 3, 0 );
	f = (FILE*) __get(0);
	F( write_file );
	if ( !__get(0) )
	{
		uchar eol = '\n';
		__put( 0, (CELL) &eol );
		__push( 1 );
		__push( (CELL)f );
		F( write_file );
	}
}


// End of file
