/// glitch - Produce glitch effect in a JPG file.
/// 
/// Copyright (c) 2013 Oliver Lau <oliver@von-und-fuer-lau.de>.
/// All rights reserved.

#ifndef WIN32
#include <sys/time.h>
#endif

#ifdef WIN32
#include <Windows.h>
#endif

#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <getopt.h>

enum _long_options {
  SELECT_HELP,
  SELECT_PERCENT,
  SELECT_IN,
  SELECT_OUT,
  SELECT_ITERATIONS,
  SELECT_QUIET,
  SELECT_VERBOSE,
  SELECT_ALGORITHM
};

enum ALGORITHMS { ALGORITHM_OR, ALGORITHM_XOR };

static struct option long_options[] = {
  { "help",       no_argument,       0, SELECT_HELP },
  { "percent",    required_argument, 0, SELECT_PERCENT },
  { "iterations", required_argument, 0, SELECT_ITERATIONS },
  { "algorithm",  required_argument, 0, SELECT_ALGORITHM },
  { "in",         required_argument, 0, SELECT_IN },
  { "out",        required_argument, 0, SELECT_OUT },
  { "quiet",      no_argument,       0, SELECT_QUIET },
  { "verbose",    no_argument,       0, SELECT_VERBOSE },
  { 0,                      0,       0, 0 }
};

static int verbose = 0;
static int iterations = 10;
static bool quiet = false;
static char *infile = NULL;
static char *outfile = NULL;
static double percent = 10;
static ALGORITHMS algorithm = ALGORITHM_XOR;


void usage() {
  printf("Usage: glitch --in original.jpg --out glitched.jpg [--iterations|-i %d] [--percent|-p %lf] [--algorithm|-a XOR|OR]\n\r", iterations, percent);
}

#define MIN(x, y) (x)>(y)? (y):(x)
#define MAX(x, y) (x)<(y)? (y):(x)

double random(double x, double y) {
  double high = 0;
  double low = 0;
  double randNum = 0;
  // if there is no range, return the value
  if (x == y) return x; 			// float == ?, wise? epsilon?
  high = MAX(x, y);
  low = MIN(x, y);
  randNum = low + ((high-low) * rand()/(RAND_MAX + 1.0));
  return randNum;
}

int main(int argc, char* argv[]) {
  printf("BLAH\n\r");
  for (;;) {
    int option_index = 0;
    int c = getopt_long(argc, argv, "i:p:h?vqa:", long_options, &option_index);
    if (c == -1)
      break;
    switch (c)
    {
    case SELECT_IN:
      infile = optarg;
      break;
    case SELECT_OUT:
      outfile = optarg;
      break;
    case 'i':
    case SELECT_ITERATIONS:
      iterations = atoi(optarg);
      break;
    case 'p':
    case SELECT_PERCENT:
      percent = atof(optarg);
      break;
    case 'a':
    case SELECT_ALGORITHM:
      if (strcmp(optarg, "OR") == 0)
        algorithm = ALGORITHM_OR;
      break;
    case 'h':
      /* fall-through */
    case '?':
      /* fall-through */
    case SELECT_HELP:
      usage();
      return 0;
      break;
    case SELECT_VERBOSE:
      /* fall-through */
    case 'v':
      ++verbose;
      break;
    case SELECT_QUIET:
      /* fall-through */
    case 'q':
      quiet = true;
      break;
    default:
      usage();
      exit(EXIT_FAILURE);
      break;
    }
  }
  if (infile == NULL || outfile == NULL) {
    usage();
    return EXIT_FAILURE;
  }
  if (percent >= 100) {
    usage();
    return EXIT_FAILURE;
  }
  if (iterations < 1) {
    usage();
    return EXIT_FAILURE;
  }
  printf("%s -> %s\n\r", infile, outfile); 
  FILE *in = fopen(infile, "rb");
  fseek(in, 0L, SEEK_END);
  long sz = ftell(in);
  fseek(in, 0L, SEEK_SET);
  unsigned char *buf = (unsigned char*) malloc(sz);
  fread(buf, 1, sz, in);
  fclose(in);
  FILE *out = fopen(outfile, "wb+");
#ifdef WIN32
  srand(GetTickCount());
#else
  struct timeval tv;
  gettimeofday(&tv, 0);
  long int n = (tv.tv_sec ^ tv.tv_usec) ^ getpid();
  srand(n);
#endif
  long firstPos = (long)(1e-2 * sz * percent);
  printf("in between %lu and %lu\n\r", firstPos, sz); 
  for (int i = 0; i < iterations; ++i) {
    const long pos = (long) random(firstPos, sz);
    const int bit = rand() % 8;
    const unsigned char oldByte = buf[pos];
    unsigned char newByte = oldByte;
    switch (algorithm) {
    default:
      // fall-through
    case ALGORITHM_XOR:
      newByte ^= (1 << bit);
      break;
    case ALGORITHM_OR:
      newByte |= (1 << bit);
      break;
    }
    buf[pos] = newByte;
    printf("glitching at position %ld:%d (%02xh->%02xh)\n", pos, bit, (unsigned int) oldByte, (unsigned int) newByte);
  }
  fwrite(buf, 1, sz, out);
  fclose(out);
  free(buf);
  return EXIT_SUCCESS;
}
