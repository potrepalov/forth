// Copyleft  by Potrepalov I.S.

// -----------------
/* Block word set */


/*
 ANSI said: "If a system provides any standard word for accessing
 mass storage, it shall also implement the Block word set".

 So... I did it.


 Table 9.2 - THROW code assignments

 -35     invalid block number

*/


i( BLOCK ) // u -- a
{
	Throw( -35 );
}


i( BUFFER ) // u -- a
{
	Throw( -35 );
}


i( FLUSH ) // --
{
	return;
}


i( LOAD ) // i*x u -- j*x
{
	Throw( -35 );
}


i( save_buffers ) // --
{
	return;
}


i( UPDATE ) // --
{
	return;
}


// End of file
