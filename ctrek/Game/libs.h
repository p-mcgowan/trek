/**
 * Basic math library
 */
typedef struct MATH {

  int min(int a, int b) {
    return (a > b) ? b : a;
  }

  int max(int a, int b) {
    return (a > b) ? a : b;
  }

  float round(float value) {
    return (value - (int)value >= 0.5) ? value + (1 - (value - (int)value)) :
    value - (value - (int)value);
  }

} MATH;

static MATH Math;


/**
 * Debugging library
 */
#define DEBUG_LOG_NONE 0
#define DEBUG_LOG_LOW 1
#define DEBUG_LOG_MEDIUM 2
#define DEBUG_LOG_HIGH 3
#define DEBUG_DEFAULT DEBUG_LOG_MEDIUM, __FILE__, __LINE__

typedef struct DEBUG {

  int logLevel;
  int enabled;

  void init(const char *f, int level) {
    logLevel = level;
    enabled = 0;

    static FILE *logfp = fopen(f, "w");
    if (logfp) {
      dup2(fileno(logfp), STDERR_FILENO);
      fclose(logfp);
    } else {
      fprintf(stderr, "No console log");
    }
  }


  void log(int loglevel, const char *file, int line) {
    if (loglevel >= logLevel) {
      fprintf(stderr, "%s:%d: ", file, line);
    }
  }


  void log(int loglevel, const char *file, int line, char *s) {
    if (loglevel >= logLevel) {
      fprintf(stderr, "%s:%d: %s\n", file, line, s);
    }
  }


  void log(int loglevel, const char *file, int line, const char *format, ...) {
    if (loglevel >= logLevel) {
      char buf[BUFFER_MEDIUM] = {0};
      va_list args;
      va_start(args, format);
      vsnprintf(buf, BUFFER_MEDIUM - 1, format, args);
      fprintf(stderr, "%s:%d: %s\n", file, line, buf);
      va_end(args);
    }
  }


  void write(char *s) {
    fprintf(stderr, "%s", s);
  }


  void write(const char *format, ...) {
    char buf[BUFFER_MEDIUM] = {0};
    va_list args;
    va_start(args, format);
    vsnprintf(buf, BUFFER_MEDIUM - 1, format, args);
    fprintf(stderr, "%s", buf);
    va_end(args);
  }


  void write(const char *file, int line, char *s) {
    fprintf(stderr, "%s:%d: %s", file, line, s);
  }


  void write(const char *file, int line, const char *format, ...) {
    char buf[BUFFER_MEDIUM] = {0};
    va_list args;
    va_start(args, format);
    vsnprintf(buf, BUFFER_MEDIUM - 1, format, args);
    fprintf(stderr, "%s:%d: %s", file, line, buf);
    va_end(args);
  }


  void write(int loglevel, const char *file, int line, const char *format, ...) {
    if (loglevel >= logLevel) {
      char buf[BUFFER_MEDIUM] = {0};
      va_list args;
      va_start(args, format);
      vsnprintf(buf, BUFFER_MEDIUM - 1, format, args);
      fprintf(stderr, "%s:%d: %s", file, line, buf);
      va_end(args);
    }
  }

} DEBUG;

static DEBUG Debug;

