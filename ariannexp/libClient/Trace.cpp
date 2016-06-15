#include "Trace.h"
#include "SDL.h"
#include <sstream>


arianne::Trace::Trace(void)
  {
  std::ostringstream oss;
  oss << "logs/client_log_" << SDL_GetTicks() << ".txt";

  os=new std::ofstream(oss.str().c_str());
  }

arianne::Trace::~Trace(void)
  {
  if(os)
    {
    os->close();
    delete os;
    os=0;
    }
  }
  
void arianne::Trace::add(std::string const& method, std::string const& event, std::string const& info)
  {
  Uint32 ticks=SDL_GetTicks();
  *os << ticks << "\t" << event << "\t" << method << "\t" << info << std::endl;
  }  
  
arianne::Trace arianne::global_trace;

  