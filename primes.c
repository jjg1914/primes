#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>

#define SWITCH_PRINT_RESULTS  "--print-results"
#define SWITCH_NO_ALARM       "--no-alarm"
#define SWITCH_NO_DIAGNOSE    "--no-diagnose"
#define FLAG_PRINT_RESULTS    (0x1)
#define FLAG_NO_ALARM         (0x2)
#define FLAG_NO_DIAGNOSE      (0x4)

#define ESIEVE_RESULTS_MAX    ( UINT_MAX / ( 32 ) )
unsigned int esieve_results[ESIEVE_RESULTS_MAX];
#define ESIEVE_GET_RESULT( i ) \
  ( ( esieve_results[i >> 5] & ( 1 << ( i & 31 ) ) ) )
#define ESIEVE_SET_RESULT( i ) \
  ( esieve_results[i >> 5] |= ( 1 << ( i & 31 ) ) ) 

unsigned int t0 = 0, t1 = 0;

volatile unsigned int total_results = 0;

void alarm_handler( int z ) {
  unsigned int dt = time( NULL ) - t0;
  fprintf( stderr, "=time ellpased: %i minutes %i seconds\n",
    dt / 60, dt % 60 );
  fprintf( stderr, "=total results %i primes\n", total_results );
  alarm( 2 );
}

void eratosthenes( unsigned int flags ) {
  total_results = 1;
  unsigned int i = 0;
  while ( i < ESIEVE_RESULTS_MAX ) {
    esieve_results[i++] = 0;
  }

  ESIEVE_SET_RESULT( 0 );
  ESIEVE_SET_RESULT( 1 );

  i = 4;
  while ( i > 3 ) { // check overflow
    ESIEVE_SET_RESULT( i );
    i += 2;
  }

  i = 3;
  while ( i < ( UINT_MAX / 2 ) ) {
    if ( !ESIEVE_GET_RESULT( i ) ) {
      ++total_results;
      unsigned int j = i << 1;
      while ( j > i ) { // check overflow
        ESIEVE_SET_RESULT( j );
        j += i;
      }
    }
    i += 2;
  }

  while ( i != 0 ) {
    if ( !ESIEVE_GET_RESULT( i ) ) {
      ++total_results;
    }
    i += 1;
  }

  t1 = time( NULL );

  if ( flags & FLAG_PRINT_RESULTS ) {
    i = 0;
    do {
      if ( !ESIEVE_GET_RESULT( i ) ) {
        printf( "%i\n", i );
      }
      ++i;
    } while ( i != 0 );
  }
}

int main( int argc, char *argv[] ) {
  unsigned int flags = 0, i = 1, dt;
  assert( sizeof(unsigned int) == 4 );

  while( i < argc ) {
    if ( strcmp( SWITCH_PRINT_RESULTS, argv[i] ) == 0 ) {
      fprintf( stderr, "print results\n" );
      flags |= FLAG_PRINT_RESULTS;
    } else if ( strcmp( SWITCH_NO_ALARM, argv[i] ) == 0 ) {
      flags |= FLAG_NO_ALARM;
    } else if ( strcmp( SWITCH_NO_DIAGNOSE, argv[i] ) == 0 ) {
      flags |= FLAG_NO_DIAGNOSE;
    }
    ++i;
  }

  t0 = time( NULL );
  if ( !( flags & FLAG_NO_ALARM ) ) {
    signal( SIGALRM, alarm_handler );
    alarm( 2 );
  }

  eratosthenes( flags );

  dt = t1 - t0;
  if ( !( flags & FLAG_NO_DIAGNOSE ) ) {
    fprintf( stderr, "=total time: %i minutes %i seconds\n", dt / 60, dt % 60 );
    fprintf( stderr, "=total results %i primes\n", total_results );
  }

  return 0;
}
