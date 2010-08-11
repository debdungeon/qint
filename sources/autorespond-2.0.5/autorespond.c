/*
	autorespond for qmail

	Copyright 1998 Eric Huss
	E-mail: e-huss@netmeridian.com

	Patched 2000 by Matthias Henze matthias@mhcsoftware.de
        Patched 2001 by Brad Dameron <bdameron@tscnet.com>

	Usage:

			autorespond time num message dir [ flag arsender ]

		time - time in seconds to consider consecutive
		num - maximum number of messages to respond to within time seconds
		message - the message file
		dir - directory to store list of e-mail addresses

		optional parameters:
     		flag - heandling of original message:
			0 - append nothing
			1 - append quoted original message without atatchments

		arsender - from adress in generated message (* = None)

		!! edit DEFAULT_MH and DEFAULT_FROM below !!

	CHANGELOG:
		MH 07/2000	1.1.0	added message handling
		MH 07/2000	1.1.0	added from adrees commandline option
		MH 07/2000	1.1.0a	changed back to 1.0.0 compatibility
					the new commandline options are optional by now
					
		BD 06/2001	2.0.0   Removed excess code, cleaned up some code
		
	MH - Matthias Henze matthias@mhcsoftware.de
        BD - Brad Dameron <bdameron@tscnet.com>

	TODO:

	- clean up 
	- better error handling (e.g. filecreation)

*/

/*
Exit codes:
0 - OK
99 - OK...stop processing lines in .qmail
100 - hard error
111 - soft error
*/

/*
SENDER - envelope sender
NEWSENDER - forwarding evelope sender
RECIPIENT - envelope "to"
USER - the user
HOME - your home directory
HOST - host of recipient address
LOCAL - local of recipient address
EXT - .qmail extension...etc.
DTLINE - Delivered-To
RPLINE - Return-Path
*/

/*Change this value here to the location of your qmail*/
#define QMAIL_LOCATION "/var/qmail"

#include <time.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/file.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

#define DEFAULT_MH	1	/* default value for message_handling flag */
#define DEFAULT_FROM	"$"	/* default "from" for the autorespond */

#define WITH_OMESSAGE	1


#define HR_BUFFER_SIZE 1024

typedef struct _headers {
	char *tag;
	char *content;
	struct _headers *next;
} headers;

headers *header = (headers *)NULL;



/*see header file for more info*/

static char *binqqargs[2] = { "bin/qmail-queue", 0 };
static char *montab[12] = { "Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec" };




/****************************************************************/
void * safe_malloc(size_t size)
{
void * ptr;

	ptr = malloc(size);
	if(ptr==NULL) {
		/*exit...no memory*/
		_exit(111);
	}

	return ptr;
}


/****************************************************************/
void * safe_realloc(void * ptr, size_t size)
{
void * p;

	p = realloc(ptr,size);
	if(p==NULL) {
		/*exit...no memory*/
		_exit(111);
	}

	return p;
}

/****************************************************************/
char * read_file(char * filename)
{
FILE * f;
unsigned long size;
char * frb;

	if(!filename) {
		/*failed*/
		return NULL;
	}

	f = fopen(filename,"rb");
	if(f==NULL) {
		/*failed*/
		return NULL;
	}

	/*seek to end*/
	if(fseek(f,0,SEEK_END)==-1) {
		/*failed*/
		fclose(f);
		return NULL;
	}

	/*this may not be portable (although it almost always is)*/
	size = ftell(f);

	/*return to the beginning*/
	if(fseek(f,0,SEEK_SET)==-1) {
		fclose(f);
		return NULL;
	}

	frb = (char *) safe_malloc(size+100);

	if(fread(frb,1,size,f)!=size) {
		fclose(f);
		return NULL;
	}
	frb[size]=0;					/*for safety*/
	fclose(f);
	return frb;
}


/****************************************************************
** A wrapper for qmail-queue
** borrowed from djb                   */

int send_message(char * msg, char * from, char ** recipients, int num_recipients)
{
/*	...Adds Date:
	...Adds Message-Id:*/
int r;
int wstat;
int i;
struct tm * dt;
unsigned long msgwhen;
FILE * fdm;
FILE * fde;
pid_t pid;
int pim[2];				/*message pipe*/
int pie[2];				/*envelope pipe*/
FILE *mfp;
char msg_buffer[256];

	/*open a pipe to qmail-queue*/
	if(pipe(pim)==-1 || pipe(pie)==-1) {
		return -1;
	}
	pid = vfork();
	if(pid == -1) {
		/*failure*/
		return -1;
	}
	if(pid == 0) {
		/*I am the child*/
		close(pim[1]);
		close(pie[1]);
		/*switch the pipes to fd 0 and 1
		  pim[0] goes to 0 (stdin)...the message*/
		if(fcntl(pim[0],F_GETFL,0) == -1) {
/*			fprintf(stderr,"Failure getting status flags.\n");*/
			_exit(120);
		}
		close(0);
		if(fcntl(pim[0],F_DUPFD,0)==-1) {
/*			fprintf(stderr,"Failure duplicating file descriptor.\n");*/
			_exit(120);
		}
		close(pim[0]);
		/*pie[0] goes to 1 (stdout)*/
		if(fcntl(pie[0],F_GETFL,0) == -1) {
/*			fprintf(stderr,"Failure getting status flags.\n");*/
			_exit(120);
		}
		close(1);
		if(fcntl(pie[0],F_DUPFD,1)==-1) {
/*			fprintf(stderr,"Failure duplicating file descriptor.\n");*/
			_exit(120);
		}
		close(pie[0]);
		if(chdir(QMAIL_LOCATION) == -1) {
			_exit(120);
		}
		execv(*binqqargs,binqqargs);
		_exit(120);
	}

	/*I am the parent*/
	fdm = fdopen(pim[1],"wb");					/*updating*/
	fde = fdopen(pie[1],"wb");
	if(fdm==NULL || fde==NULL) {
		return -1;
	}
	close(pim[0]);
	close(pie[0]);

	/*prepare to add date and message-id*/
	msgwhen = time(NULL);
	dt = gmtime((long *)&msgwhen);
	/*start outputting to qmail-queue
	  date is in 822 format
	  message-id could be computed a little better*/
	fprintf(fdm,"Date: %u %s %u %02u:%02u:%02u -0000\nMessage-ID: <%lu.%u.blah>\n"
		,dt->tm_mday,montab[dt->tm_mon],dt->tm_year+1900,dt->tm_hour,dt->tm_min,dt->tm_sec,msgwhen,getpid() );

	mfp = fopen( msg, "rb" );
	
	while ( fgets( msg_buffer, sizeof(msg_buffer), mfp ) != NULL )
	{
		fprintf(fdm,"%s",msg_buffer);
	}

	fclose(mfp);

	fclose(fdm);


	/*send the envelopes*/

	fprintf(fde,"F%s",from);
	fwrite("",1,1,fde);					/*write a null char*/
	for(i=0;i<num_recipients;i++) {
		fprintf(fde,"T%s",recipients[i]);
		fwrite("",1,1,fde);					/*write a null char*/
	}
	fwrite("",1,1,fde);					/*write a null char*/
	fclose(fde);

	/*wait for qmail-queue to close*/
	do {
		r = wait(&wstat);
	} while ((r != pid) && ((r != -1) || (errno == EINTR)));
	if(r != pid) {
		/*failed while waiting for qmail-queue*/
		return -1;
	}
	if(wstat & 127) {
		/*failed while waiting for qmail-queue*/
		return -1;
	}
	/*the exit code*/

	if((wstat >> 8)!=0) {
		/*non-zero exit status
		  failed while waiting for qmail-queue*/
		return -1;
	}
	return 0;
}

/****************************************************************
** reading header and break it down to it's tag and contet
   joins continued headers, stops on start of body  matthias@mhcsoftware.de
*/

void read_headers( FILE *fp )
{
	char h_buffer[HR_BUFFER_SIZE+1];
	char *ptr;
	int  len;
	headers *act_header = (headers *)NULL;


	while ( fgets( h_buffer, HR_BUFFER_SIZE, fp ) != NULL )
	{
		ptr = h_buffer;

		if ( *ptr == '\n' )
			break;

		switch( *ptr )
		{
		case ' ' :
		case '\t' : /* header continued */
			len = strlen( ptr ) + strlen(act_header->content);
			act_header->content = safe_realloc( act_header->content, len + 1 );
			strncat( act_header->content, ptr, len );
			act_header->content[len] = '\0';
			break;
		default :
			if ( act_header != (headers *)NULL )
			{
				act_header->next = safe_malloc( sizeof(headers) );
				act_header       = act_header->next;
			}
			else
				header = act_header = safe_malloc( sizeof(headers) );

			act_header->next = (headers *)NULL;

			while( *ptr != ' ' && *ptr != '\t' )
				ptr++;

			/* strip a possible : */
			len = ( *(ptr-1) == ':' ) ? ptr - h_buffer - 1 : ptr - h_buffer;

			/* skip whitspaces */
			while( *ptr == ' ' && *ptr == '\t' )
				ptr++;
	
			act_header->tag = safe_malloc( len + 1 );
			strncpy( act_header->tag, h_buffer, len );
			act_header->tag[len] = '\0';


			act_header->content = safe_malloc( strlen( ptr ) + 1 );
			strcpy( act_header->content, ptr );

			break;
		}
	}
}



/*********************************************************
** find string in string - ignore case **/

char *strcasestr( char *_s1, char *_s2 )
{
	char *s1;
	char *s2;
	char *ptr;

	s1 = strdup(_s1);
	s2 = strdup(_s2);

	for ( ptr = s1; *ptr != '\0'; ptr++ )
		*ptr = tolower( *ptr );

	for ( ptr = s2; *ptr != '\0'; ptr++ )
		*ptr = tolower( *ptr );

	ptr = strstr( s1, s2 );

	if ( ptr == (char *)NULL )
		return (char *)NULL;
	else
		return _s1 + (ptr - s1);
}




/*********************************************************
** look up header tag in chain and try to find search string 
** returns pointer to contetnt on success other wise NULL */

char *inspect_headers( char * tag, char *ss )
{
	headers *act_header;

	act_header = header;

	while ( act_header != (headers *)NULL )
	{
		if ( strcasecmp( act_header->tag, tag ) == 0 )
		{
			if ( ss == (char *)NULL )
				return act_header->content;

			if ( strcasestr( act_header->content, ss ) != (char *)NULL )
				return act_header->content;

			return (char *)NULL;
		}
		act_header = act_header->next;
	}
	return (char *)NULL;
}



/*********************************************************
** returns the content boundary string for this message */
char *get_content_boundary()
{
	char *s, *r;

	if ( (s = inspect_headers( "Content-Type", (char *)NULL )) == (char *)NULL) 
		return (char *)NULL;

	if ( (r = strcasestr( s, "boundary=" )) == (char *)NULL)
		return (char *)NULL;
	
	*(r+strlen(r)-2) = '\0'; /* delete quote at the end */

	return r+10; /* point to first cahr after quote */
}



/*********************************************************
** concat headers to one string for output **/

char *return_header( char *tag )
{
	headers *act_header;
	int len;
	char *b;

	act_header = header;
	b     = (char *) malloc( 20 );
	*b    = '\0';

	while ( act_header != (headers *)NULL )
	{
		if ( (tag != (char *)NULL && strcasecmp(tag,act_header->tag ) == 0) || tag == (char *)NULL )
		{
			len = strlen( b ) + 
			      strlen( act_header->tag ) + 1 + 
			      strlen( act_header->content );

			b = safe_realloc( b, len + 1);

			sprintf( b, "%s%s:%s", b, act_header->tag, act_header->content );

			b[len] = '\0';
		}
		act_header = act_header->next;
	}
	return( b );
}



/**********************************************************
** free header chain ***/

void free_headers()
{
	headers *act_header, *tmp_header;

	act_header = header;

	while ( act_header != (headers *)NULL )
	{
		free( act_header->tag );
		free( act_header->content );
		tmp_header = act_header;
		act_header = act_header->next;
		free( tmp_header );
	}
}


/**********************************************************
** print header chain for deguging ***/

void print_header_chain()
{
	headers *act_header;

	act_header = header;

	while ( act_header != (headers *)NULL )
	{
		printf( "%s: %s", act_header->tag, act_header->content );
		act_header = act_header->next;
	}
}



/**********************************************************
** main */

int main(int argc, char ** argv)
{
char * sender;

char * message;
unsigned int time_message;
unsigned int timer;
unsigned int num;
char * message_filename;
char * dir;

char * ptr;
char * my_delivered_to;

DIR * dirp;
struct dirent * direntp;
unsigned int message_time;
char * address;
unsigned int count;
char filename[256];
FILE * f;
unsigned int message_handling = DEFAULT_MH;
char buffer[256];
char *content_boundary;
char *rpath = DEFAULT_FROM;
char *TheUser;
char *TheDomain;

	if(argc > 7 || argc < 5) {
		fprintf(stderr, "\nautorespond: ");
		fprintf(stderr, "usage: time num message dir [ flag arsender ]\n\n");
		fprintf(stderr, "time - amount of time to consider a message (in seconds)\n");
		fprintf(stderr, "num - maximum number of messages to allow within time seconds\n");
		fprintf(stderr, "message - the filename of the message to send\n");
		fprintf(stderr, "dir - the directory to hold the log of messages\n\n");
		fprintf(stderr, "optional parameters:\n\n");
		fprintf(stderr, "flag - handling of original message:\n\n");
		fprintf(stderr, "0 - append nothing\n");
		fprintf(stderr, "1 - append quoted original message without attachments <default>\n\n");
		fprintf(stderr, "arsender - from address in generated message, or:\n\n");
		fprintf(stderr, "+ = blank from envelope !\n");
		fprintf(stderr, "$ = To: address will be used\n\n");
		_exit(111);
	}

	TheUser= getenv("EXT");
	TheDomain= getenv("HOST");
 
	setvbuf(stderr, NULL, _IONBF, 0);

	if(argc > 7 || argc < 5) {
		fprintf(stderr, "AUTORESPOND: Invalid arguments. (%d)\n",argc);
		_exit(111);
	}

	time_message     = strtoul(argv[1],NULL,10);
	num              = strtoul(argv[2],NULL,10);
	message_filename = argv[3];
	dir              = argv[4];

	if ( argc > 5 )
		message_handling = strtoul(argv[5],NULL,10);
	if ( argc > 6 )
		rpath = argv[6];

	if ( *rpath == '+' )
		rpath = "";
	if ( *rpath == '$' )
	{
		rpath = safe_malloc( strlen(TheUser) + strlen(TheDomain) + 2);
		strncpy( rpath, TheUser, strlen(TheUser) );
		strncat( rpath, "@", 1 );
		strncat( rpath, TheDomain, strlen(TheDomain) );
	}

	timer = time(NULL);

	/*prepare the "delivered-to" string*/
	my_delivered_to = "Delivered-To: Autoresponder\n";

	read_headers( stdin );


	message = read_file(message_filename);
	if(message==NULL) {
		fprintf(stderr, "AUTORESPOND: Failed to open message file.\n");
		_exit(111);
	}

	/*don't autorespond in certain situations*/
	sender = getenv("SENDER");
	if(sender==NULL)
		sender = "";

	/*don't autorespond to a mailer-daemon*/
	if( sender[0]==0 || strncasecmp(sender,"mailer-daemon",13)==0 || strchr(sender,'@')==NULL || strcmp(sender,"#@[]")==0 ) {
		/*exit with success and continue parsing .qmail file*/
		fprintf(stderr,"AUTORESPOND:  Stopping on mail from [%s].\n",sender);
		_exit(0);
	}


	if ( inspect_headers("mailing-list", (char *)NULL ) != (char *)NULL )
	{
		fprintf(stderr,"AUTORESPOND: This looks like it's from a mailing list, I will ignore it.\n");
		_exit(0);			/*report success and exit*/
	}
	if ( inspect_headers("Delivered-To", "Autoresponder" ) != (char *)NULL )
	{
		/*got one of my own messages...*/
		fprintf(stderr,"AUTORESPOND: This message is looping...it has my Delivered-To header.\n");
		_exit(100);			/*hard error*/
	}
	if ( inspect_headers("precedence", "junk" ) != (char *)NULL ||
	     inspect_headers("precedence", "bulk" ) != (char *)NULL ||
	     inspect_headers("precedence", "list" ) != (char *)NULL )
	{
		fprintf(stderr,"AUTORESPOND: Junk mail received.\n");
		_exit(0); /* don't reply to bulk, junk, or list mail */
	}

	/*check the logs*/
	if(chdir(dir) == -1) {
		fprintf(stderr,"AUTORESPOND: Failed to change into directory.\n");
		_exit(111);
	}

	/*add entry*/
	sprintf(filename,"A%u.%u",getpid(),timer);
	f = fopen(filename,"wb");
	if(f==NULL) {
		fprintf(stderr,"AUTORESPOND: Unable to create file for [%s].",sender);
		_exit(111);
	}
	if(fwrite(sender,1,strlen(sender),f)!=strlen(sender)) {
		fprintf(stderr,"AUTORESPOND: Unable to create file for [%s].",sender);
		fclose(f);
		unlink(filename);
		_exit(111);
	}
	fclose(f);

	/*check if there are too many responses in the logs*/
	dirp = opendir(".");
	count = 0;
	while((direntp = readdir(dirp)) != NULL) {
		if(direntp->d_name[0] != 'A')
			continue;
		ptr = strchr(direntp->d_name,'.');
		if(ptr==NULL)
			continue;
		message_time = strtoul(ptr+1,NULL,10);
		if(message_time < timer-time_message) {
			/*too old..ignore errors on unlink*/
			unlink(direntp->d_name);
		} else {
			address = read_file(direntp->d_name);
			if(address==NULL) {
				/*ignore this?*/
				continue;
			}
			if(strcasecmp(address,sender)==0) {
				count++;
			}
			free(address);
		}
	}
	if(count>num) {
		fprintf(stderr,"AUTORESPOND: too many received from [%s]\n",sender);
		_exit(0); /* don't reply to this message, but allow it to be delivered */
	}

	sprintf(filename,"tmp%u.%u",getpid(),timer);
	f = fopen(filename,"wb"); 

	fprintf( f, "%sTo: %s\nFrom: %s\nSubject: Re:%s\n%s\n", 
	            my_delivered_to, sender, rpath, inspect_headers( "Subject", (char *) NULL ), message );

	if ( message_handling == 1 ) {
		fprintf( f, "%s\n\n", "-------- Original Message --------" );
		if ( (content_boundary = get_content_boundary()) == (char *)NULL )
		{
			while ( fgets( buffer, sizeof(buffer), stdin ) != NULL )
			{
				fputs( "> ", f );
				fputs( buffer, f );
			}
		} else
		{
			int content_found = 0;
			while ( fgets( buffer, sizeof(buffer), stdin ) != NULL )
			{
				if ( content_found == 1 ) 
				{
					if ( strstr( buffer, content_boundary ) != (char *)NULL )
						break;
					fputs( "> ", f );
					fputs( buffer, f );
				}
				if ( strstr( buffer, content_boundary ) != (char *)NULL )
				{
					if ( content_found == 1 )
						break;
					else
					{
						free_headers();
						read_headers( stdin );
						if ( inspect_headers("Content-Type", "text/plain" ) != (char *)NULL )
							content_found = 1;
						continue;
					}
				}
			}
		}
	}

	fprintf( f, "\n\n" );

	fclose( f );

	/*send the autoresponse...ignore errors?*/
	send_message(filename,rpath,&sender,1); 

	unlink( filename );

	_exit(0);
	return 0;					/*compiler warning squelch*/
}



