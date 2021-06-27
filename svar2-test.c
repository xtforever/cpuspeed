#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "mls.h"
#include "svar2.h"

int read_keys_from_file( char *filename )
{
    int keys = m_create(1000, sizeof(int) );
    FILE *fp = fopen( filename, "r" );
    assert(fp);
    int buf = m_create(100,1);
    while( m_fscan2(buf,'\n', fp ) == '\n' ) {
	int sbuf = s_memcpy( 0, buf, SVAR_MAX );
	m_put(keys, &sbuf );
	m_clear(buf);
    }
    fclose(fp);
    m_free(buf);
    return keys;
}

void check_hash_speed( int seconds, int keys  )
{
    // fprintf(stderr, "trying %d seconds hashing\n",seconds );
    struct timespec tp0,tp1, tp2; // tv_sec, tv_nsec
    clock_gettime(CLOCK_MONOTONIC, &tp0);
    srand( (int) tp0.tv_nsec );
    unsigned count=0;


    int buf = s_printf(0,0,"hashspeed.");
    int buf_len = m_len(buf)-1;
    m_setlen(buf, SVAR_MAX);

    
    while(1)
	{
	    count++;
	    int keynum = rand() % m_len(keys);
	    int keystr = INT(keys, keynum);
	    m_write(buf,buf_len, m_str(keystr), m_len(keystr) );

	    svar_lookup(buf , SVAR_FLOAT );
	    clock_gettime(CLOCK_MONOTONIC, &tp1);
	    timespec_diff(&tp1,&tp0,&tp2);	    
	    if( tp2.tv_sec  >= seconds ) break;
	}
    
    long double n = tp2.tv_sec;
    n = n * 1E6 + (tp2.tv_nsec / 1E3); 
    n = (count * 1.0) / n;
    printf("hash/µsec: %Le, hash: %u,  %lu sec, %lu nsec\n\n", n, count, tp2.tv_sec,tp2.tv_nsec);
    m_free( buf );
}


void check3(void)
{
    int keys = read_keys_from_file( "john.txt" ); 
    check_hash_speed( 5, keys  );
    m_free_list_of_list(keys);
}

int main()
{
    m_init();
    trace_level=0;
    svar_create();


    check3();
    

    svar_destruct();
    m_destruct();
}
