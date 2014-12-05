#ifndef __COMMON_H__
#define __COMMON_H__

#include <algorithm>
#include <iterator>
#include <string>
#include <type_traits>

#define MAKE_SINGLETON(CLASS_NAME)\
  public:\
  static CLASS_NAME &instance() {\
    static CLASS_NAME s{};\
    return s;\
  }

#define BASIC_CMD_RGX(CMD_NAME)\
  "^\\s*?\\!" #CMD_NAME "\\s*?$"

namespace common
{

inline void program_initialize();
inline void program_cleanup();

static inline void sto_lower(std::string &str)
{
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

template <typename T, typename U>
static inline bool found_find_if(const T &iterator, const U &container)
{
  // TODO insert type-checking
  return iterator != container.end();
}

template <typename T, typename U>
static inline bool not_found_find_if(const T &iterator, const U &container)
{
  // TODO insert type-checking
  return iterator == container.end();
}

static inline bool next_tokenize(char **buffer, size_t *length, char delim, char **token)
{
  char *begin = *buffer;
  for (size_t i = 0; i < *length; ++i)
  {
    if ((*buffer)[i] == delim)
    {
      (*buffer)[i] = '\0';
      *buffer = (i == *length - 1) ? *buffer + i : *buffer + i + 1;
      *length -= (i + 1);
      *token = begin;
      return true;
    }
  }
  return false;
}

}
#endif